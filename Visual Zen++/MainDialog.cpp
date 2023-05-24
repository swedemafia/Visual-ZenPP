#include "pch.h"

INT_PTR CALLBACK MainDialogManager::HandleMessage(UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch (Message) {
    case WM_CLOSE:
        return OnClose(wParam, lParam);
    case WM_COMMAND:
        return OnCommand(wParam, lParam);
    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSTATIC:
        return OnCtlColorDlg(wParam, lParam);
    case WM_CTLCOLORLISTBOX:
        return OnCtlColorListBox(wParam, lParam);
    case WM_HSCROLL:
        return OnHScroll(wParam, lParam);
    case WM_INITDIALOG:
        return OnInitDialog(wParam, lParam);
    case WM_NOTIFY:
        return OnNotify(wParam, lParam);
    case WM_SHOWWINDOW:
        return OnShowWindow(wParam, lParam);
    }

	return FALSE; // Default message processing
}

INT_PTR MainDialogManager::OnClose(WPARAM wParam, LPARAM lParam)
{
    Communication::Disconnect();
    EndDialog(this->Handle, 0);
    PostQuitMessage(0);
	return TRUE;
}

INT_PTR MainDialogManager::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // Determine which dialog object was clicked
    switch (LOWORD(wParam)) {

    case COMBO_EMULATOR_OUTPUT_PROTOCOL:
        if ((HIWORD(wParam) == CBN_SELCHANGE)) {
            ChangeEmulatorOutputProtocol((MainDialogManager::EmulatorOutputProtocolIDs)SendMessage(this->EmulatorOutputComboBox, CB_GETCURSEL, 0, 0));
        }

        break;

    case COMBO_OPERATIONAL_MODE:
        
        if ((HIWORD(wParam) == CBN_SELCHANGE)) {
            ChangeOperationalMode((MainDialogManager::OperationalModeIDs)SendMessage(this->OperationalModeComboBox, CB_GETCURSEL, 0, 0));
        }

        break;

    case COMBO_REMOTE_SLOT_CHANGE:

        if ((HIWORD(wParam) == CBN_SELCHANGE)) {
            ChangeRemoteSlotChange((MainDialogManager::RemoteSlotChangeIDs)SendMessage(this->RemoteSlotChangeComboBox, CB_GETCURSEL, 0, 0));
        }

        break;

    // Emulator output protocol:
    case MENU_DEVICE_EMULATOR_AUTO:
        ChangeEmulatorOutputProtocol(Emulator_Auto);
        break;

    case MENU_DEVICE_EMULATOR_NINTENDOSWITCH:
        ChangeEmulatorOutputProtocol(Emulator_NintendoSwitch);
        break;

    case MENU_DEVICE_EMULATOR_PLAYSTATION3:
        ChangeEmulatorOutputProtocol(Emulator_PlayStation3);
        break;

    case MENU_DEVICE_EMULATOR_PLAYSTATION45:
        ChangeEmulatorOutputProtocol(Emulator_PlayStation45);
        break;

    case MENU_DEVICE_EMULATOR_XBOX360:
        ChangeEmulatorOutputProtocol(Emulator_Xbox360);
        break;

    case MENU_DEVICE_EMULATOR_XBOXONE:
        ChangeEmulatorOutputProtocol(Emulator_XboxOne);
        break;

    // Device operational mode:
    case MENU_DEVICE_OPERATIONAL_STANDARD:
        ChangeOperationalMode(Operational_Standard);
        break;
    case MENU_DEVICE_OPERATIONAL_TOURNAMENT:
        ChangeOperationalMode(Operational_Tournament);
        break;
    case MENU_DEVICE_OPERATIONAL_WHEEL:
        ChangeOperationalMode(Operational_Wheel);
        break;
            
    // Remote slot change:
    case MENU_DEVICE_REMOTESLOT_DISABLED:
        ChangeRemoteSlotChange(RemoteSlot_Disabled);
        break;
    case MENU_DEVICE_REMOTESLOT_LEFTSTICK:
        ChangeRemoteSlotChange(RemoteSlot_L3LS);
        break;
    case MENU_DEVICE_REMOTESLOT_SELECT:
        ChangeRemoteSlotChange(RemoteSlot_ShareView);
        break;

    // Clear Bluetooth devices:
    case BUTTON_CLEAR_BLUETOOTH:
    case MENU_DEVICE_CLEAR_BLUETOOTH:

        if (Connection.State != Communication::Connection_Disconnected) {

            if (Cronus.Thread == INVALID_HANDLE_VALUE) {

                // Notify user
                Timestamp();
                InsertFormattedText(GRAY, _c("Attempting to clear registered Bluetooth devices...\r\n"));

                // Create factory reset thread
                Cronus.Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CronusZen::ThreadProc, (LPVOID)Communication::Connection_ClearBtCommand, 0, NULL);

                // Check for success
                if (Cronus.Thread == INVALID_HANDLE_VALUE) {
                    InsertFormattedText(RED, _c("Failed to initiate clear registered Bluetooth devices thread (error: %d)!\r\n"), GetLastError());
                }

            }
            else {
                Timestamp();
                InsertFormattedText(RED, _c("You must wait for the operation to complete before attempting to clear the registered Bluetooth devices!\r\n"));
            }

        }
        else {
            Timestamp();
            InsertFormattedText(RED, _c("You must connect to the device before attempting to clear the registered Bluetooth devices!\r\n"));
        }

        break;

    // Device cleanup:
    case BUTTON_ERASE_MEMORY_SLOTS:
    case MENU_DEVICE_ERASE_MEMORY_SLOTS:

        if (Connection.State != Communication::Connection_Disconnected) {

            if (Cronus.Thread == INVALID_HANDLE_VALUE) {

                // Notify user
                Timestamp();
                InsertFormattedText(GRAY, _c("Attempting to erase slots...\r\n"));

                // Create erase slots thread
                Cronus.Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CronusZen::ThreadProc, (LPVOID)Communication::Connection_DeviceCleanup, 0, NULL);

                // Check for success
                if (Cronus.Thread == INVALID_HANDLE_VALUE) {
                    Timestamp();
                    InsertFormattedText(RED, _c("Failed to initiate erase slot thread (error: %d)!\r\n"), GetLastError());
                }

            }
            else {
                Timestamp();
                InsertFormattedText(RED, _c("You must wait for the operation to complete before attempting to erase slots!\r\n"));
            }

        }
        else {
            Timestamp();
            InsertFormattedText(RED, _c("You must connect to the device before attempting to erase the slots!\r\n"));
        }

        break;

    // Factory reset:
    case BUTTON_FACTORY_RESET:
    case MENU_DEVICE_FACTORY_RESET:

        if (Connection.State != Communication::Connection_Disconnected) {

            if (Cronus.Thread == INVALID_HANDLE_VALUE) {

                // Notify user
                Timestamp();
                InsertFormattedText(GRAY, _c("Attempting to factory reset the device...\r\n"));

                // Create factory reset thread
                Cronus.Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CronusZen::ThreadProc, (LPVOID)Communication::Connection_FactoryReset, 0, NULL);

                // Check for success
                if (Cronus.Thread == INVALID_HANDLE_VALUE) {
                    Timestamp();
                    InsertFormattedText(RED, _c("Failed to initiate factory reset thread (error: %d)!\r\n"), GetLastError());
                }

            }
            else {
                Timestamp();
                InsertFormattedText(RED, _c("You must wait for the operation to complete before attempting to factory reset!\r\n"));
            }

        }
        else {
            Timestamp();
            InsertFormattedText(RED, _c("You must connect to the device before attempting to factory reset!\r\n"));
        }

        break;

    // Reset device:
    case BUTTON_RESET_DEVICE:
    case MENU_DEVICE_RESET:

        if (Connection.State != Communication::Connection_Disconnected) {

            if (Cronus.Thread == INVALID_HANDLE_VALUE) {

                // Notify user
                Timestamp();
                InsertFormattedText(GRAY, _c("Attempting to reset the device...\r\n"));

                // Create factory reset thread
                Cronus.Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CronusZen::ThreadProc, (LPVOID)Communication::Connection_ResetDevice, 0, NULL);

                // Check for success
                if (Cronus.Thread == INVALID_HANDLE_VALUE) {
                    Timestamp();
                    InsertFormattedText(RED, _c("Failed to initiate reset thread (error: %d)!\r\n"), GetLastError());
                }

            }
            else {
                Timestamp();
                InsertFormattedText(RED, _c("You must wait for the operation to complete before attempting to factory reset!\r\n"));
            }

        }
        else {
            Timestamp();
            InsertFormattedText(RED, _c("You must connect to the device before attempting to reset the device!\r\n"));
        }

        break;

    // Serial number:
    case MENU_DEVICE_SERIAL_NUMBER:
        // Notify user of serial number
        MainDialog->Timestamp();
        MainDialog->InsertFormattedText(GRAY, _c("Device serial number: %s.\r\n"), Cronus.SerialNumber);
        break;

    // PS4 Speciality:
    case MENU_DEVICE_PS4SPECIALITY:
    case RADIO_PS4_SPECIALITY:

        if (HIWORD(wParam) == BN_CLICKED) {

            // Unselect it
            if (Cronus.Ps4Speciality && !Cronus.RemotePlay) {
                if (Connection.State == Communication::Connection_Connected) {
                    SendMessage(this->PS4SpecialityRadio, BM_SETCHECK, BST_UNCHECKED, 0);
                    Cronus.Ps4Speciality = 0;
                }
                ChangePS4Speciality(0);
            }
            else if(!Cronus.Ps4Speciality) {
                if (Connection.State != Communication::Connection_RemotePlay) {
                    if (Connection.State != Communication::Connection_PS4Speciality) {
                        Cronus.Ps4Speciality = 1;
                        CheckMenuItem(MainDialog->MainMenu, MENU_DEVICE_REMOTE_PLAY, MF_UNCHECKED);
                        CheckMenuItem(MainDialog->MainMenu, MENU_DEVICE_PS4SPECIALITY, MF_CHECKED);
                        ChangePS4Speciality(1);
                    }
                    else {
                        ChangePS4Speciality(0);
                    }
                }
                else {
                    ChangePS4Speciality(1);
                }
                
            }

        }
        break;

    // Remote Play:
    case MENU_DEVICE_REMOTE_PLAY:
    case RADIO_REMOTE_PLAY:

        if (HIWORD(wParam) == BN_CLICKED) {

            // Unselect it
            if (Cronus.RemotePlay && !Cronus.Ps4Speciality) {
                if (Connection.State == Communication::Connection_Connected) {
                    SendMessage(this->RemotePlayRadio, BM_SETCHECK, BST_UNCHECKED, 0);
                    Cronus.RemotePlay = 0;
                }
                ChangeRemotePlay(0);
            }
            else if (!Cronus.RemotePlay) {
                if (Connection.State != Communication::Connection_PS4Speciality) {
                    if (Connection.State != Communication::Connection_RemotePlay) {
                        Cronus.RemotePlay = 1;
                        CheckMenuItem(MainDialog->MainMenu, MENU_DEVICE_REMOTE_PLAY, MF_CHECKED);
                        CheckMenuItem(MainDialog->MainMenu, MENU_DEVICE_PS4SPECIALITY, MF_UNCHECKED);
                        ChangeRemotePlay(1);
                    }
                    else {
                        ChangeRemotePlay(0);
                    }
                }
                else {
                    ChangeRemotePlay(1);
                }
            }

        }
        break;

    // Disconnect:
    case MENU_CONNECTION_DISCONNECT:
        Communication::Disconnect();
        break;
    
    // Reconnect:
    case MENU_CONNECTION_RECONNECT:

        // Disconnect if we are currently connected
        if (Connection.State != Communication::Connection_Disconnected)
            Communication::Disconnect();
           
        // Begin connection process
        Communication::Connect();

        break;

    case MENU_FILE_EXIT:
        return OnClose(wParam, lParam);

    // Clear output:
    case MENU_FILE_CLEAR_OUTPUT:
        SetWindowText(this->RichEditOutput, (LPCWSTR)L"");
        break;

    }


    return TRUE;
}

INT_PTR MainDialogManager::OnCtlColorDlg(WPARAM wParam, LPARAM lParam)
{
    HDC hdc = (HDC)wParam;

    SetBkColor(hdc, RGB(0x25, 0x38, 0x3c));  // Set the background color to black
    SetTextColor(hdc, WHITE); // Set the foreground text to white

    return (INT_PTR)(CreateSolidBrush(RGB(0x25, 0x38, 0x3c)));
}

INT_PTR MainDialogManager::OnCtlColorListBox(WPARAM wParam, LPARAM lParam)
{
    HDC hdc = (HDC)wParam;

    SetBkColor(hdc, BLACK);  // Set the background color to black
    SetTextColor(hdc, WHITE); // Set the foreground text to white

    return (INT_PTR)(GetStockObject(BLACK_BRUSH));
}

INT_PTR MainDialogManager::OnHScroll(WPARAM wParam, LPARAM lParam)
{
    if (LOWORD(wParam) == TB_ENDTRACK) {
        if ((HWND)lParam == this->DS4LightbarBrightnessSlider) {

            // Retrieve the slider value when the user releases the slider
            int SliderValue = SendMessage(this->DS4LightbarBrightnessSlider, TBM_GETPOS, 0, 0);

            // Display the slider value (optional)
            char FormattedValue[8];
            sprintf_s(FormattedValue, "%d%%", SliderValue);
            SendDlgItemMessageA(this->Handle, LABEL_DS4_BRIGHTNESS_VALUE, WM_SETTEXT, 0, (LPARAM)FormattedValue);

            // Submit update if we are connected
            if (Connection.State == Communication::Connection_Connected) {
                ChangeDS4LightbarBrightness(SliderValue);
                Cronus.Ds4LightbarBrightness = SliderValue;
                CronusZen::SendSingleFragment(0xff001001 | (SliderValue << 16));
            }

        }
        else if ((HWND)lParam == this->VMSpeedSlider) {

            // Retrieve the slider value when the user releases the slider
            int SliderValue = SendMessage(this->VMSpeedSlider, TBM_GETPOS, 0, 0);

            // Display the slider value (optional)
            char FormattedValue[8];
            sprintf_s(FormattedValue, "%dms", SliderValue);
            SendDlgItemMessageA(this->Handle, LABEL_VM_SPEED_VALUE, WM_SETTEXT, 0, (LPARAM)FormattedValue);

            // Submit update if we are connected
            if (Connection.State == Communication::Connection_Connected) {
                CronusZen::StreamIoStatus(CronusZen::Off);
                Timestamp();
                InsertFormattedText(GRAY, _c("Attempting to change the virtual machine speed to %dms...\r\n"), SliderValue);
                CronusZen::SetVmCtrl(SliderValue);
                CronusZen::StreamIoStatus(CronusZen::InputReport | CronusZen::OutputReport | CronusZen::Ps5Adt);
            }
        }
    }

    return TRUE;
}

INT_PTR MainDialogManager::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
    RECT DialogRect;

    // Get window rect
    GetWindowRect(this->Handle, &DialogRect);

    // Determine X/Y coordinates
    int DialogX = (GetSystemMetrics(SM_CXSCREEN) - (DialogRect.right - DialogRect.left)) >> 1;
    int DialogY = (GetSystemMetrics(SM_CYSCREEN) - (DialogRect.bottom - DialogRect.top)) >> 1;

    // Set dialog position
    SetWindowPos(this->Handle, NULL, DialogX, DialogY, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

    // Get dialog object handles
    //
    // RichEdit:
    this->RichEditOutput = GetDlgItem(this->Handle, RICHEDIT_OUTPUT);

    // Buttons:
    this->ClearBluetoothButton = GetDlgItem(this->Handle, BUTTON_CLEAR_BLUETOOTH);
    this->EraseMemorySlotsButton = GetDlgItem(this->Handle, BUTTON_ERASE_MEMORY_SLOTS);
    this->FactoryResetButton = GetDlgItem(this->Handle, BUTTON_FACTORY_RESET);
    this->ResetButton = GetDlgItem(this->Handle, BUTTON_RESET_DEVICE);
    
    // Combo boxes:
    this->EmulatorOutputComboBox = GetDlgItem(this->Handle, COMBO_EMULATOR_OUTPUT_PROTOCOL);
    this->RemoteSlotChangeComboBox = GetDlgItem(this->Handle, COMBO_REMOTE_SLOT_CHANGE);
    this->OperationalModeComboBox = GetDlgItem(this->Handle, COMBO_OPERATIONAL_MODE);

    // Menu:
    this->MainMenu = GetMenu(this->Handle);

    // Radio buttons:
    this->PS4SpecialityRadio = GetDlgItem(this->Handle, RADIO_PS4_SPECIALITY);
    this->RemotePlayRadio = GetDlgItem(this->Handle, RADIO_REMOTE_PLAY);

    // Sliders:
    this->DS4LightbarBrightnessSlider = GetDlgItem(this->Handle, SLIDER_DS4_LIGHTBAR);
    this->VMSpeedSlider = GetDlgItem(this->Handle, SLIDER_VM);

    // Prepare RichEdit output
    SendMessage(this->RichEditOutput, EM_SETBKGNDCOLOR, FALSE, BLACK);
    SendMessage(this->RichEditOutput, EM_SETEVENTMASK, 0, SendMessage(this->RichEditOutput, EM_GETEVENTMASK, 0, 0) | ENM_LINK);
    SendMessage(this->RichEditOutput, EM_AUTOURLDETECT, TRUE, 0);

    // Set paragraph formatting
    PARAFORMAT2 pf;
    pf.cbSize = sizeof(PARAFORMAT2);
    pf.dwMask = PFM_OFFSET;
    pf.dxOffset = 1440;
    SendMessage(this->RichEditOutput, EM_SETPARAFORMAT, 0, (LPARAM)&pf);

    // Set character formatting
    CHARFORMAT cf;
    cf.cbSize = sizeof(CHARFORMAT);
    cf.dwMask = CFM_SIZE | CFM_FACE;
    cf.yHeight = 210;
    wcscpy_s(cf.szFaceName, _cw(L"Microsoft Sans Serif"));
    SendMessage(this->RichEditOutput, EM_SETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cf);

    // Prepare combo drop list boxes
    // 
    // Emulator output protocol:
    SendMessage(this->EmulatorOutputComboBox, CB_ADDSTRING, 0, (LPARAM)_cw(L"Auto"));
    SendMessage(this->EmulatorOutputComboBox, CB_ADDSTRING, 0, (LPARAM)_cw(L"PlayStation 3"));
    SendMessage(this->EmulatorOutputComboBox, CB_ADDSTRING, 0, (LPARAM)_cw(L"Xbox 360"));
    SendMessage(this->EmulatorOutputComboBox, CB_ADDSTRING, 0, (LPARAM)_cw(L"PlayStation 4/5"));
    SendMessage(this->EmulatorOutputComboBox, CB_ADDSTRING, 0, (LPARAM)_cw(L"Xbox One X/S"));
    SendMessage(this->EmulatorOutputComboBox, CB_ADDSTRING, 0, (LPARAM)_cw(L"Nintendo Switch"));

    // Remote slot change:
    SendMessage(this->RemoteSlotChangeComboBox, CB_ADDSTRING, 0, (LPARAM)_cw(L"Disabled"));
    SendMessage(this->RemoteSlotChangeComboBox, CB_ADDSTRING, 0, (LPARAM)_cw(L"PS/Xbox + Share/View"));
    SendMessage(this->RemoteSlotChangeComboBox, CB_ADDSTRING, 0, (LPARAM)_cw(L"PS/Xbox + L3/LS"));

    // Device operational mode:
    SendMessage(this->OperationalModeComboBox, CB_ADDSTRING, 0, (LPARAM)_cw(L"Wheel Mode"));
    SendMessage(this->OperationalModeComboBox, CB_ADDSTRING, 0, (LPARAM)_cw(L"Standard Mode"));
    SendMessage(this->OperationalModeComboBox, CB_ADDSTRING, 0, (LPARAM)_cw(L"Tournament Mode"));

    // Prepare sliders
    // 
    // DS4 lightbar brightness:
    SendMessage(this->DS4LightbarBrightnessSlider, TBM_SETRANGE, TRUE, MAKELONG(0, 100));
    SendMessage(this->DS4LightbarBrightnessSlider, TBM_SETPAGESIZE, 0, 5);

    // VM speed:
    SendMessage(this->VMSpeedSlider, TBM_SETRANGE, TRUE, MAKELONG(1, 40));
    SendMessage(this->VMSpeedSlider, TBM_SETPAGESIZE, 0, 1);
    
    // Print greeting
    Timestamp();
    InsertFormattedText(GRAY, _c("Welcome to Zen++ - Copyright (C) 2023 Swedemafia - version %d.%02d.\r\n"), VERSION_MAJOR, VERSION_MINOR);
    Timestamp();
    InsertFormattedText(GRAY, _c("An Asgard production (https://discord.gg/tGH7QxtPam).\r\n"));

    // Disable URLs due to HID path
    SendMessage(this->RichEditOutput, EM_AUTOURLDETECT, FALSE, 0);

    // Device thread:
    Console.InputThread = INVALID_HANDLE_VALUE;

    // Communication:
    Connection.State = Communication::Connection_Disconnected;
    Connection.Handle = INVALID_HANDLE_VALUE;
    Connection.CompletionPort = INVALID_HANDLE_VALUE;
    Connection.CompletionThread = INVALID_HANDLE_VALUE;
    Connection.Operation.OnConnect.Key = Communication::Completion_Connect;
    Connection.Operation.OnRecvData.Key = Communication::Completion_Receive;
    Connection.Operation.OnSendData.Key = Communication::Completion_Send;

    // Cronus Zen:
    memset(&Cronus, 0, sizeof(Cronus));
    Cronus.Thread = INVALID_HANDLE_VALUE;

    // Device:
    Device.State = Emulator::Device_Init;

	return TRUE;
}

INT_PTR MainDialogManager::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
    // PS4 Speciality:
    if ((HWND)wParam == this->PS4SpecialityRadio) {

        if (HIWORD(wParam) == BN_CLICKED) {

            // Get if PS4 Speciality is already enabled
            LRESULT IsSelected = SendMessage(this->PS4SpecialityRadio, BM_GETCHECK, 0, 0);

            // Unselect it
            if (IsSelected)
                SendMessage(this->PS4SpecialityRadio, BM_SETCHECK, BST_UNCHECKED, 0);
        }
    }
    else if ((HWND)wParam == this->RemotePlayRadio) {

        if (HIWORD(wParam) == BN_CLICKED) {

            // Get if Remote Play is already enabled
            LRESULT IsSelected = SendMessage(this->RemotePlayRadio, BM_GETCHECK, 0, 0);

            // Unselect it
            if (IsSelected)
                SendMessage(this->RemotePlayRadio, BM_SETCHECK, BST_UNCHECKED, 0);
        }
    }

    return TRUE;
}

INT_PTR MainDialogManager::OnNotify(WPARAM wParam, LPARAM lParam)
{
    NMHDR* pnmhdr;
    pnmhdr = (NMHDR*)lParam;

    switch(pnmhdr->code) {
    case EN_LINK:
        ENLINK* pEnLink;
        pEnLink = (ENLINK*)lParam;
        if (pEnLink->msg == WM_LBUTTONUP) {
            InsertFormattedText(RED, "user clicked link!\r\n");
        }
        break;

    case NM_CUSTOMDRAW:
        if (reinterpret_cast<LPNMHDR>(lParam)->hwndFrom == this->DS4LightbarBrightnessSlider) {

            // Get slider value
            int SliderValue = SendMessage(this->DS4LightbarBrightnessSlider, TBM_GETPOS, 0, 0);

            // Build display buffer
            char Display[8] = "";
            sprintf_s(Display, "%d%%", SliderValue);

            // Set label text
            SendDlgItemMessageA(this->Handle, LABEL_DS4_BRIGHTNESS_VALUE, WM_SETTEXT, 0, (LPARAM)Display);

        }
        else if (reinterpret_cast<LPNMHDR>(lParam)->hwndFrom == this->VMSpeedSlider) {

            // Get slider value
            int SliderValue = SendMessage(this->VMSpeedSlider, TBM_GETPOS, 0, 0);

            // Build display buffer
            char Display[8] = "";
            sprintf_s(Display, "%dms", SliderValue);

            // Set label text
            SendDlgItemMessageA(this->Handle, LABEL_VM_SPEED_VALUE, WM_SETTEXT, 0, (LPARAM)Display);
        }
        break;

    }

    return TRUE;
}

INT_PTR MainDialogManager::OnShowWindow(WPARAM wParam, LPARAM lParam)
{
    // Create our main thread
    if (Connection.CompletionThread == INVALID_HANDLE_VALUE)
        Connection.CompletionThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Communication::CompletionThreadProc, NULL, CREATE_SUSPENDED, NULL);

    if (Console.InputThread == INVALID_HANDLE_VALUE)
        Console.InputThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Emulator::DeviceThreadProc, NULL, 0, NULL);

    return TRUE;
}

void MainDialogManager::OutputText(COLORREF Color, const char* Text)
{
    CHARFORMAT2 CharFormat;

    // Prepare character formatting structure
    memset(&CharFormat, 0, sizeof(CHARFORMAT2));

    // Set character formatting attributes
    CharFormat.cbSize = sizeof(CHARFORMAT2);
    CharFormat.dwMask = CFM_COLOR;
    CharFormat.dwEffects = 0;
    CharFormat.crTextColor = Color;
    CharFormat.bCharSet = DEFAULT_CHARSET;
    CharFormat.bPitchAndFamily = DEFAULT_PITCH;

    // Set the this attributes for the existing text
    SendMessage(this->RichEditOutput, EM_SETSEL, -1, -1);
    SendMessage(this->RichEditOutput, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&CharFormat);

    // Append the text
    SendMessageA(this->RichEditOutput, EM_REPLACESEL, FALSE, (LPARAM)Text);
    SendMessage(this->RichEditOutput, EM_SETSEL, -1, -1);
    SendMessage(this->RichEditOutput, EM_SCROLL, SB_BOTTOM, 0);
}

void MainDialogManager::InsertFormattedText(COLORREF Color, const char* Format, ...)
{

    char Output[512] = "";

	va_list argptr;
	va_start(argptr, Format);
	vsnprintf_s(Output, sizeof(Output), sizeof(Output) - 1, Format, argptr);
	va_end(argptr);

	OutputText(Color, Output);

}

void MainDialogManager::Timestamp(void) {
    SYSTEMTIME SystemTime;
    GetLocalTime(&SystemTime);
    InsertFormattedText(WHITE, _c("[%02i:%02i:%02i.%03i] "), SystemTime.wHour % 12, SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds);
}

void MainDialogManager::UpdateCaption(const char* Status)
{
    char Output[512] = "";

    wsprintfA(Output, _c("Zen++ - Copyright (C) 2023 Swedemafia - %s"), Status);

    // Set window caption
    SetWindowTextA(this->Handle, Output);
}