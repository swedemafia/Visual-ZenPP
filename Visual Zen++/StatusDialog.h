#ifndef STATUSDIALOG_H
#define STATUSDIALOG_H
#pragma once

class StatusDialogManager : public DialogManager::DialogHandler {
public:
	explicit StatusDialogManager(HINSTANCE Instance, int ResourceID) {
		this->Instance = Instance;
		this->ResourceID = ResourceID;
		this->Handle = NULL;
	}

	// User functions:
	void DisplayWindow(void);
	void HideWindow(void);
	void SetSlotBytes(unsigned int CurrentSlot, float SlotBytes);
	void SetTotalBytes(unsigned int TotalSlots, float TotalBytes);
	void UpdateProgress(float BytesWritten);

protected:
	// Callback functions:
	INT_PTR CALLBACK HandleMessage(UINT Message, WPARAM wParam, LPARAM lParam);

private:

	// Message handlers:
	INT_PTR OnCtlColorDlg(WPARAM wParam, LPARAM lParam);
	INT_PTR OnCtlColorStatic(WPARAM wParam, LPARAM lParam);
	
	// Progress bars:
	HWND CurrentSlotProgress;
	HWND TotalProgress;
	HWND SlotStatusLabel;
	HWND TotalStatusLabel;

	// Progress:
	float SlotBytesWritten;
	float TotalBytesWritten;
	float TotalBytes;
	float TotalSlotBytes;
	unsigned int TotalSlots;
};

#endif