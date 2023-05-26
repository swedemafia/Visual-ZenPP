#include "pch.h"

void RunScriptDialogManager::HideWindow(void)
{
	ShowWindow(this->Handle, SW_HIDE);
}

void RunScriptDialogManager::DisplayWindow(void)
{
	ShowWindow(this->Handle, SW_SHOW);
	SetWindowPos(this->Handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetForegroundWindow(this->Handle);

	// Set progress bar colors
	SendMessage(this->TotalProgress, PBM_SETBARCOLOR, 0, BLUEGREEN);
}

void RunScriptDialogManager::SetTotalBytes(float TotalBytes)
{
	this->TotalBytesWritten = 0;
	this->TotalBytes = TotalBytes;

	// Reset progress bar position
	SendMessage(this->TotalProgress, PBM_SETPOS, 0, 0);

	// Update total status label
	SetWindowTextA(this->TotalStatusLabel, _c("Loading game pack to device..."));
}

void RunScriptDialogManager::UpdateProgress(float BytesWritten)
{
	// Increment bytes written for current slot and total
	this->TotalBytesWritten += BytesWritten;

	// Calculate percentages
	float TotalPercentage = (this->TotalBytesWritten / this->TotalBytes) * 100;

	char Buffer[64] = "";

	// Calculate total percentage label
	sprintf_s(Buffer, _c("%.02f%%"), (this->TotalBytesWritten / this->TotalBytes) * 100);
	SetWindowTextA(this->TotalPercentageLabel, Buffer);

	// Generate total status label
	sprintf_s(Buffer, _c("Uploaded %.0f of %.0f bytes..."), this->TotalBytesWritten, this->TotalBytes);
	SetWindowTextA(this->TotalProgressLabel, Buffer);


	// Set progress bar positions
	SendMessage(this->TotalProgress, PBM_SETPOS, (WPARAM)TotalPercentage, 0);
}

INT_PTR RunScriptDialogManager::OnCtlColorDlg(WPARAM wParam, LPARAM lParam)
{
	HDC hdc = (HDC)wParam;

	SetBkColor(hdc, RGB(0x25, 0x38, 0x3c));  // Set the background color to black

	return (INT_PTR)(CreateSolidBrush(RGB(0x25, 0x38, 0x3c)));
}

INT_PTR RunScriptDialogManager::OnCtlColorStatic(WPARAM wParam, LPARAM lParam)
{
	HDC hdc = (HDC)wParam;

	SetBkColor(hdc, RGB(0x25, 0x38, 0x3c));  // Set the background color to black
	SetTextColor(hdc, WHITE);

	return (INT_PTR)(CreateSolidBrush(RGB(0x25, 0x38, 0x3c)));
}

INT_PTR CALLBACK RunScriptDialogManager::HandleMessage(UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message) {
	case WM_INITDIALOG:
		this->TotalProgress = GetDlgItem(this->Handle, PROGRESS_TOTAL);
		this->TotalPercentageLabel = GetDlgItem(this->Handle, LABEL_PERCENTAGE_TOTAL);
		this->TotalProgressLabel = GetDlgItem(this->Handle, LABEL_PROGRESS_TOTAL);
		this->TotalStatusLabel = GetDlgItem(this->Handle, LABEL_STATUS_TOTAL);
		return TRUE;
	case WM_CTLCOLORDLG:
		return OnCtlColorDlg(wParam, lParam);
	case WM_CTLCOLORSTATIC:
		return OnCtlColorStatic(wParam, lParam);
	}

	return FALSE;
}