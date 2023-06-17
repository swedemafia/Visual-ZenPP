#include "pch.h"

void StatusDialogManager::DisplayWindow(void)
{
	ShowWindow(this->Handle, SW_SHOW);
	SetWindowPos(this->Handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetForegroundWindow(this->Handle);
	this->IsShowing = TRUE;

	// Set progress bar colors
	SendMessage(this->CurrentSlotProgress, PBM_SETBARCOLOR, 0, BLUEGREEN);
	SendMessage(this->TotalProgress, PBM_SETBARCOLOR, 0, BLUEGREEN);
}

void StatusDialogManager::HideWindow(void)
{
	ShowWindow(this->Handle, SW_HIDE);
	this->IsShowing = FALSE;
}

BOOLEAN StatusDialogManager::IsVisible(void)
{
	return this->IsShowing;
}

void StatusDialogManager::SetSlotBytes(unsigned int CurrentSlot, float SlotBytes)
{
	char Buffer[32] = "";

	this->SlotBytesWritten = 0;
	this->TotalSlotBytes = SlotBytes;

	// Reset current slot position
	SendMessage(this->CurrentSlotProgress, PBM_SETPOS, 0, 0);

	// Update slot status label
	wsprintfA(Buffer, _c("Writing to slot #%d..."), CurrentSlot);
	SetWindowTextA(this->SlotStatusLabel, Buffer);
}

void StatusDialogManager::SetTotalBytes(unsigned int TotalSlots, float TotalBytes)
{
	char Buffer[32] = "";

	this->TotalBytesWritten = 0;
	this->TotalBytes = TotalBytes;
	this->TotalSlots = TotalSlots;

	// Reset progress bar position
	SendMessage(this->TotalProgress, PBM_SETPOS, 0, 0);

	// Update total status label
	wsprintfA(Buffer, _c("Uploading %d slot%s..."), this->TotalSlots, (this->TotalSlots == 1) ? "" : "s");
	SetWindowTextA(this->TotalStatusLabel, Buffer);
}

void StatusDialogManager::UpdateProgress(float BytesWritten)
{
	// Increment bytes written for current slot and total
	this->SlotBytesWritten += BytesWritten;
	this->TotalBytesWritten += BytesWritten;

	// Calculate percentages
	float SlotPercentage = (this->SlotBytesWritten / this->TotalSlotBytes) * 100;
	float TotalPercentage = (this->TotalBytesWritten / this->TotalBytes) * 100;

	// Set progress bar positions
	SendMessage(this->CurrentSlotProgress, PBM_SETPOS, (WPARAM)SlotPercentage, 0);
	SendMessage(this->TotalProgress, PBM_SETPOS, (WPARAM)TotalPercentage, 0);
}

INT_PTR StatusDialogManager::OnCtlColorDlg(WPARAM wParam, LPARAM lParam)
{
	HDC hdc = (HDC)wParam;

	SetBkColor(hdc, RGB(0x25, 0x38, 0x3c));  // Set the background color to black

	return (INT_PTR)(CreateSolidBrush(RGB(0x25, 0x38, 0x3c)));
}

INT_PTR StatusDialogManager::OnCtlColorStatic(WPARAM wParam, LPARAM lParam)
{
	HDC hdc = (HDC)wParam;

	SetBkColor(hdc, RGB(0x25, 0x38, 0x3c));  // Set the background color to black
	SetTextColor(hdc, WHITE);

	return (INT_PTR)(CreateSolidBrush(RGB(0x25, 0x38, 0x3c)));
}

INT_PTR CALLBACK StatusDialogManager::HandleMessage(UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message) {
	case WM_INITDIALOG:
		this->CurrentSlotProgress = GetDlgItem(this->Handle, PROGRESS_CURRENT_SLOT);
		this->TotalProgress = GetDlgItem(this->Handle, PROGRESS_TOTAL);
		this->SlotStatusLabel = GetDlgItem(this->Handle, LABEL_STATUS_SLOT);
		this->TotalStatusLabel = GetDlgItem(this->Handle, LABEL_STATUS_TOTAL);
		return TRUE;
	case WM_CTLCOLORDLG:
		return OnCtlColorDlg(wParam, lParam);
	case WM_CTLCOLORSTATIC:
		return OnCtlColorStatic(wParam, lParam);
	}

	return FALSE;
}