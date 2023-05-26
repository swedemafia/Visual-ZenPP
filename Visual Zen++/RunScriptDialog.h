#ifndef RUNSCRIPTDIALOG_H
#define RUNSCRIPTDIALOG_H
#pragma once

class RunScriptDialogManager : public DialogManager::DialogHandler {
public:
	explicit RunScriptDialogManager(HINSTANCE Instance, int ResourceID) {
		this->Instance = Instance;
		this->ResourceID = ResourceID;
		this->Handle = NULL;
	}

	// User functions:
	void DisplayWindow(void);
	void HideWindow(void);
	void SetTotalBytes(float TotalBytes);
	void UpdateProgress(float BytesWritten);

protected:
	// Callback functions:
	INT_PTR CALLBACK HandleMessage(UINT Message, WPARAM wParam, LPARAM lParam);

private:

	// Message handlers:
	INT_PTR OnCtlColorDlg(WPARAM wParam, LPARAM lParam);
	INT_PTR OnCtlColorStatic(WPARAM wParam, LPARAM lParam);

	// Progress bars:
	HWND TotalProgress;
	HWND TotalProgressLabel;
	HWND TotalStatusLabel;
	HWND TotalPercentageLabel;

	// Progress:
	float TotalBytesWritten;
	float TotalBytes;
};

#endif