#ifndef MAINDIALOG_H
#define MAINDIALOG_H
#pragma once

class MainDialogManager : public DialogManager::DialogHandler
{
public:
	explicit MainDialogManager(HINSTANCE Instance, int ResourceID) {
		this->Instance = Instance;
		this->ResourceID = ResourceID;
		this->Handle = NULL;
		this->RichEditOutput = NULL;
	}

	// RichEdit functions:
	void InsertFormattedText(COLORREF Color, const char* Format, ...);
	void Timestamp(void);
	
	// Window functions:
	void UpdateCaption(const char* Status);

	// Dialog objects:
	// 
	// Menu:
	HMENU MainMenu;

	// RichEdit:
	HWND RichEditOutput;

	// Buttons:
	HWND ClearBluetoothButton;
	HWND EraseMemorySlotsButton;
	HWND FactoryResetButton;
	HWND ResetButton;

	// Combo boxes:
	HWND EmulatorOutputComboBox;
	HWND OperationalModeComboBox;
	HWND RemoteSlotChangeComboBox;

	// Radio buttons:
	HWND PS4SpecialityRadio;
	HWND RemotePlayRadio;

	// Sliders:
	HWND DS4LightbarBrightnessSlider;
	HWND VMSpeedSlider;

	// Dialog object IDs:
	enum EmulatorOutputProtocolIDs {
		Emulator_Auto,
		Emulator_PlayStation3,
		Emulator_Xbox360,
		Emulator_PlayStation45,
		Emulator_XboxOne,
		Emulator_NintendoSwitch,
	};

	enum OperationalModeIDs {
		Operational_Wheel,
		Operational_Standard,
		Operational_Tournament
	};

	enum RemoteSlotChangeIDs {
		RemoteSlot_Disabled,
		RemoteSlot_ShareView,
		RemoteSlot_L3LS,
	};

private:

	// Message handlers:
	INT_PTR OnClose(WPARAM wParam, LPARAM lParam);
	INT_PTR OnCommand(WPARAM wParam, LPARAM lParam);
	INT_PTR OnCtlColorDlg(WPARAM wParam, LPARAM lParam);
	INT_PTR OnCtlColorListBox(WPARAM wParam, LPARAM lParam);
	INT_PTR OnHScroll(WPARAM wParam, LPARAM lParam);
	INT_PTR OnInitDialog(WPARAM wParam, LPARAM lParam);
	INT_PTR OnLButtonDown(WPARAM wParam, LPARAM lParam);
	INT_PTR OnNotify(WPARAM wParam, LPARAM lParam);
	INT_PTR OnShowWindow(WPARAM wParam, LPARAM lParam);


	// User functions:
	void OutputText(COLORREF Color, const char* Text);

protected:
	// Callback functions:
	INT_PTR CALLBACK HandleMessage(UINT Message, WPARAM wParam, LPARAM lParam);

};

#endif
