#include "pch.h"

void MainDialogManager::ClearVisibleSlots(void)
{
    for (unsigned i = 0; i < 8; i++)
        SendDlgItemMessageA(this->Handle, LABEL_SLOT_1 + i, WM_SETTEXT, 0, (LPARAM)"");
}

void MainDialogManager::DisableDialog(void)
{
    // Clear window text
    SetWindowTextA(this->CurrentSlotLabel, "");
    SetWindowTextA(this->CPUUsageLabel, "");

    // Disable menus
    // 
    // Connection:
    EnableMenuItem(this->MainMenu, MENU_CONNECTION_DISCONNECT, MF_DISABLED);

    // Program:
    EnableMenuItem(this->MainMenu, MENU_PROGRAM_LOAD_AND_RUN, MF_DISABLED);
    EnableMenuItem(this->MainMenu, MENU_PROGRAM_PROGRAM_DEVICE, MF_DISABLED);

    // Device:
    EnableMenuItem(this->MainMenu, MENU_DEVICE_SERIAL_NUMBER, MF_DISABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_CLEAR_BLUETOOTH, MF_DISABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_CYCLE_SLOTS, MF_DISABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_ERASE_MEMORY_SLOTS, MF_DISABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_FACTORY_RESET, MF_DISABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_RESET, MF_DISABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_PS4SPECIALITY, MF_DISABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_REMOTE_PLAY, MF_DISABLED);
    CheckMenuItem(this->MainMenu, MENU_DEVICE_PS4SPECIALITY, MF_UNCHECKED);
    CheckMenuItem(this->MainMenu, MENU_DEVICE_REMOTE_PLAY, MF_UNCHECKED);

    // Device->Emulator Output Protocol
    EnableMenuItem(this->MainMenu, MENU_DEVICE_EMULATOR_AUTO, MF_DISABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_EMULATOR_NINTENDOSWITCH, MF_DISABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_EMULATOR_PLAYSTATION3, MF_DISABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_EMULATOR_PLAYSTATION45, MF_DISABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_EMULATOR_XBOX360, MF_DISABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_EMULATOR_XBOXONE, MF_DISABLED);
    // Device->Operational Mode
    EnableMenuItem(this->MainMenu, MENU_DEVICE_OPERATIONAL_TOURNAMENT, MF_DISABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_OPERATIONAL_STANDARD, MF_DISABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_OPERATIONAL_WHEEL, MF_DISABLED);
    // Device->Remote Slot Change
    EnableMenuItem(this->MainMenu, MENU_DEVICE_REMOTESLOT_DISABLED, MF_DISABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_REMOTESLOT_LEFTSTICK, MF_DISABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_REMOTESLOT_SELECT, MF_DISABLED);

    // Disable dialog objects
    //
    // Buttons:
    EnableWindow(this->ClearBluetoothButton, FALSE);
    EnableWindow(this->CycleSlotsButton, FALSE);
    EnableWindow(this->EraseMemorySlotsButton, FALSE);
    EnableWindow(this->FactoryResetButton, FALSE);
    EnableWindow(this->ResetButton, FALSE);

    // Combo boxes:
    EnableWindow(this->EmulatorOutputComboBox, FALSE);
    EnableWindow(this->OperationalModeComboBox, FALSE);
    EnableWindow(this->RemoteSlotChangeComboBox, FALSE);

    // Radio buttons:
    EnableWindow(this->PS4SpecialityRadio, FALSE);
    EnableWindow(this->RemotePlayRadio, FALSE);

    // Sliders:
    EnableWindow(this->DS4LightbarBrightnessSlider, FALSE);
    EnableWindow(this->VMSpeedSlider, FALSE);

    // Clear labels
    SendDlgItemMessageA(this->Handle, LABEL_VM_SPEED_VALUE, WM_SETTEXT, 0, (LPARAM)"");
    SendDlgItemMessageA(this->Handle, LABEL_DS4_BRIGHTNESS_VALUE, WM_SETTEXT, 0, (LPARAM)"");
    SendDlgItemMessageA(this->Handle, LABEL_FIRMWARE_VERSION, WM_SETTEXT, 0, (LPARAM)"");
}

void MainDialogManager::EnableDialog(void)
{
    // Enable menus
    // 
    // Connection:
    EnableMenuItem(this->MainMenu, MENU_CONNECTION_DISCONNECT, MF_ENABLED);

    // Program:
    EnableMenuItem(this->MainMenu, MENU_PROGRAM_LOAD_AND_RUN, MF_ENABLED);
    EnableMenuItem(this->MainMenu, MENU_PROGRAM_PROGRAM_DEVICE, MF_ENABLED);

    // Device:
    EnableMenuItem(this->MainMenu, MENU_DEVICE_SERIAL_NUMBER, MF_ENABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_CLEAR_BLUETOOTH, MF_ENABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_CYCLE_SLOTS, MF_ENABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_ERASE_MEMORY_SLOTS, MF_ENABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_FACTORY_RESET, MF_ENABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_RESET, MF_ENABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_PS4SPECIALITY, MF_ENABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_REMOTE_PLAY, MF_ENABLED);

    // Device->Emulator Output Protocol
    EnableMenuItem(this->MainMenu, MENU_DEVICE_EMULATOR_AUTO, MF_ENABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_EMULATOR_NINTENDOSWITCH, MF_ENABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_EMULATOR_PLAYSTATION3, MF_ENABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_EMULATOR_PLAYSTATION45, MF_ENABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_EMULATOR_XBOX360, MF_ENABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_EMULATOR_XBOXONE, MF_ENABLED);
    // Device->Operational Mode
    EnableMenuItem(this->MainMenu, MENU_DEVICE_OPERATIONAL_TOURNAMENT, MF_ENABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_OPERATIONAL_STANDARD, MF_ENABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_OPERATIONAL_WHEEL, MF_ENABLED);
    // Device->Remote Slot Change
    EnableMenuItem(this->MainMenu, MENU_DEVICE_REMOTESLOT_DISABLED, MF_ENABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_REMOTESLOT_LEFTSTICK, MF_ENABLED);
    EnableMenuItem(this->MainMenu, MENU_DEVICE_REMOTESLOT_SELECT, MF_ENABLED);

    // Enable dialog objects
    // 
    // Buttons:
    EnableWindow(this->ClearBluetoothButton, TRUE);
    EnableWindow(this->CycleSlotsButton, TRUE);
    EnableWindow(this->EraseMemorySlotsButton, TRUE);
    EnableWindow(this->FactoryResetButton, TRUE);
    EnableWindow(this->ResetButton, TRUE);

    // Combo boxes:
    EnableWindow(this->EmulatorOutputComboBox, TRUE);
    EnableWindow(this->OperationalModeComboBox, TRUE);
    EnableWindow(this->RemoteSlotChangeComboBox, TRUE);

    // Radio buttons:
    EnableWindow(this->PS4SpecialityRadio, TRUE);
    EnableWindow(this->RemotePlayRadio, TRUE);

    // Sliders:
    EnableWindow(this->DS4LightbarBrightnessSlider, TRUE);
    EnableWindow(this->VMSpeedSlider, TRUE);
}

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
    case WM_LBUTTONDOWN:
        return OnLButtonDown(wParam, lParam);
    case WM_RBUTTONDOWN:
        return OnRButtonDown(wParam, lParam);
    }

	return FALSE; // Default message processing
}

INT_PTR MainDialogManager::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
    if (Connection.State == Communication::Connection_Connected) {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        RECT rect;

        // Loop through each of the slots (labels)
        for (unsigned i = 0; i < 8; i++) {

            HWND SlotToCheck = GetDlgItem(this->Handle, LABEL_SLOT_1 + i);

            // Convert coordinates
            GetWindowRect(SlotToCheck, &rect);
            MapWindowPoints(NULL, this->Handle, reinterpret_cast<POINT*>(&rect), 2);

            if (PtInRect(&rect, { x, y })) {

                OPENFILENAMEA OpenFileName = { 0 };
                char ChosenFile[MAX_PATH] = "";

                // Open dialog only for *.bin files
                OpenFileName.lStructSize = sizeof(OPENFILENAMEA);
                OpenFileName.hwndOwner = this->Handle;
                OpenFileName.lpstrFile = ChosenFile;
                OpenFileName.nMaxFile = sizeof(ChosenFile);
                OpenFileName.lpstrFilter = "Compiled Scripts (*.bin)\0*.bin\0";
                OpenFileName.nFilterIndex = 1;
                OpenFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_DONTADDTORECENT | OFN_NOCHANGEDIR;

                // Open the dialog and set the label if the user selects a file
                if (GetOpenFileNameA(&OpenFileName) == TRUE) {

                    // Copy file path and file name
                    memcpy(&Cronus.Slot[i].FilePath, OpenFileName.lpstrFile, sizeof(Cronus.Slot[i].FilePath));
                    memcpy(&Cronus.Slot[i].FileName, PathFindFileNameA(OpenFileName.lpstrFile), sizeof(Cronus.Slot[i].FileName));
                    
                    // Attempt to open file
                    HANDLE FileHandle = CreateFileA((LPCSTR)Cronus.Slot[i].FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
                    
                    // Determine if file was able to be read
                    if (FileHandle == INVALID_HANDLE_VALUE) {
                        Timestamp();
                        InsertFormattedText(RED, "Error: failed to load file %s! (error: %d)\r\n", Cronus.Slot[i].FileName, GetLastError());
                    }
                    else {

                        // Obtain file size
                        Cronus.Slot[i].FileSize = GetFileSize(FileHandle, NULL);

                        if (Cronus.Slot[i].FileSize == INVALID_FILE_SIZE) {

                            // Prompt user of error querying file size
                            Timestamp();
                            InsertFormattedText(RED, "Error: failed to load file file %s, unable to query file size! (error: %d)\r\n", Cronus.Slot[i].FileName, GetLastError());

                            // Clear slot
                            memset(&Cronus.Slot[i], 0, sizeof(Cronus.Slot[i]));
                        }
                        else {
                            SetWindowTextA(SlotToCheck, (LPCSTR)Cronus.Slot[i].FileName);
                        }

                        // Close file
                        CloseHandle(FileHandle);
                    }
                }

                return TRUE;
            }

        }
    }

    return FALSE;
}

INT_PTR MainDialogManager::OnClose(WPARAM wParam, LPARAM lParam)
{
    CronusZen::ExitApiMode();
    CronusZen::StreamIoStatus(CronusZen::Off);
    Communication::Disconnect();
    PostQuitMessage(0);
    EndDialog(this->Handle, 0);
	return TRUE;
}

INT_PTR MainDialogManager::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // Determine which dialog object was clicked
    switch (LOWORD(wParam)) {
    // Load Plugin
    case MENU_PLUGINS_LOAD_PLUGIN:
        OPENFILENAMEA PluginFileName;
        char ChosenPlugin[MAX_PATH];

        memset(&PluginFileName, 0, sizeof(OPENFILENAMEA));
        memset(&ChosenPlugin, 0, MAX_PATH);

        // Open dialog only for *.bin files
        PluginFileName.lStructSize = sizeof(OPENFILENAMEA);
        PluginFileName.hwndOwner = this->Handle;
        PluginFileName.lpstrFile = ChosenPlugin;
        PluginFileName.nMaxFile = sizeof(ChosenPlugin);
        PluginFileName.lpstrFilter = "Zen++ Plugins (*.zpl)\0*.zpl\0";
        PluginFileName.nFilterIndex = 1;
        PluginFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_DONTADDTORECENT | OFN_NOCHANGEDIR;

        // Open the dialog and set the label if the user selects a file
        if (GetOpenFileNameA(&PluginFileName) == TRUE) {
            PluginAPI::LoadPlugin(PluginFileName.lpstrFile);
        }


        break;
    // Unload plugin:
    case MENU_PLUGINS_UNLOAD_PLUGIN:
        PluginAPI::UnloadPlugin();
        break;

    // About
    case MENU_ABOUT_ABOUT:
        Timestamp();
        InsertFormattedText(LIGHTBLUE, "Zen++ - version %d.%02d - an Asgard production by Swedemafia.\r\n", VERSION_MAJOR, VERSION_MINOR);
        Timestamp();
        InsertFormattedText(LIGHTBLUE, "Zen++ was developed to protect script developers.\r\n");

        // Enable URL detection
        SendMessage(this->RichEditOutput, EM_AUTOURLDETECT, TRUE, 0);

        Timestamp();
        InsertFormattedText(LIGHTBLUE, "Join Asgard (Discord server) for support: https://discord.gg/tGH7QxtPam.\r\n");
        Timestamp();
        InsertFormattedText(PURPLE, "Zen++ is not an official product of Collective Minds Gaming Co. Ltd.\r\n");
        Timestamp();
        InsertFormattedText(PURPLE, "The developer is not affiliated with Collective Minds Gaming Co. Ltd.\r\n");

        // Disable URL detection
        SendMessage(this->RichEditOutput, EM_AUTOURLDETECT, FALSE, 0);

        break;

    // Support
    case MENU_ABOUT_SUPPORT:

        // Enable URL detection
        SendMessage(this->RichEditOutput, EM_AUTOURLDETECT, TRUE, 0);

        // Prompt user
        Timestamp();
        InsertFormattedText(GRAY, "For support, join the Discord: https://discord.gg/tGH7QxtPam.\r\n");

        // Disable URL detection
        SendMessage(this->RichEditOutput, EM_AUTOURLDETECT, FALSE, 0);

        break;
    // Remove popup
    case MENU_REMOVE:
        char Buffer[128];

        // Initialize buffer
        memset(&Buffer, 0, sizeof(Buffer));
        wsprintfA(Buffer, "Are you sure you want to remove %s from slot #%d?", Cronus.Slot[PopupSlot].FileName, PopupSlot + 1);

        // Verify user wants to remove script
        if (MessageBoxA(this->Handle, Buffer, "Remove from Slot?", MB_YESNO | MB_ICONQUESTION) == IDYES) {
            memset(&Cronus.Slot[PopupSlot], 0, sizeof(Cronus.Slot[PopupSlot]));
            SetWindowTextA(GetDlgItem(this->Handle, LABEL_SLOT_1 + PopupSlot), "");
        }

        break;

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
                InsertFormattedText(GRAY, "Attempting to clear registered Bluetooth devices...\r\n");

                // Create factory reset thread
                Cronus.Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CronusZen::ThreadProc, (LPVOID)Communication::Connection_ClearBtCommand, 0, NULL);

                // Check for success
                if (Cronus.Thread == INVALID_HANDLE_VALUE) {
                    InsertFormattedText(RED, "Failed to initiate clear registered Bluetooth devices thread (error: %d)!\r\n", GetLastError());
                }

            }
            else {
                Timestamp();
                InsertFormattedText(RED, "You must wait for the operation to complete before attempting to clear the registered Bluetooth devices!\r\n");
            }

        }
        else {
            Timestamp();
            InsertFormattedText(RED, "You must connect to the device before attempting to clear the registered Bluetooth devices!\r\n");
        }

        break;

    // Device cleanup:
    case BUTTON_ERASE_MEMORY_SLOTS:
    case MENU_DEVICE_ERASE_MEMORY_SLOTS:

        if (Connection.State != Communication::Connection_Disconnected) {

            if (Cronus.Thread == INVALID_HANDLE_VALUE) {

                // Notify user
                Timestamp();
                InsertFormattedText(GRAY, "Attempting to erase slots...\r\n");

                // Create erase slots thread
                Cronus.Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CronusZen::ThreadProc, (LPVOID)Communication::Connection_DeviceCleanup, 0, NULL);

                // Check for success
                if (Cronus.Thread == INVALID_HANDLE_VALUE) {
                    Timestamp();
                    InsertFormattedText(RED, "Failed to initiate erase slot thread (error: %d)!\r\n", GetLastError());
                }

            }
            else {
                Timestamp();
                InsertFormattedText(RED, "You must wait for the operation to complete before attempting to erase slots!\r\n");
            }

        }
        else {
            Timestamp();
            InsertFormattedText(RED, "You must connect to the device before attempting to erase the slots!\r\n");
        }

        break;

    // Factory reset:
    case BUTTON_FACTORY_RESET:
    case MENU_DEVICE_FACTORY_RESET:

        if (Connection.State != Communication::Connection_Disconnected) {

            if (Cronus.Thread == INVALID_HANDLE_VALUE) {

                // Notify user
                Timestamp();
                InsertFormattedText(GRAY, "Attempting to factory reset the device...\r\n");

                // Create factory reset thread
                Cronus.Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CronusZen::ThreadProc, (LPVOID)Communication::Connection_FactoryReset, 0, NULL);

                // Check for success
                if (Cronus.Thread == INVALID_HANDLE_VALUE) {
                    Timestamp();
                    InsertFormattedText(RED, "Failed to initiate factory reset thread (error: %d)!\r\n", GetLastError());
                }

            }
            else {
                Timestamp();
                InsertFormattedText(RED, "You must wait for the operation to complete before attempting to factory reset!\r\n");
            }

        }
        else {
            Timestamp();
            InsertFormattedText(RED, "You must connect to the device before attempting to factory reset!\r\n");
        }

        break;

    // Reset device:
    case BUTTON_RESET_DEVICE:
    case MENU_DEVICE_RESET:

        if (Connection.State != Communication::Connection_Disconnected) {

            if (Cronus.Thread == INVALID_HANDLE_VALUE) {

                // Notify user
                Timestamp();
                InsertFormattedText(GRAY, "Attempting to reset the device...\r\n");

                // Create factory reset thread
                Cronus.Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CronusZen::ThreadProc, (LPVOID)Communication::Connection_ResetDevice, 0, NULL);

                // Check for success
                if (Cronus.Thread == INVALID_HANDLE_VALUE) {
                    Timestamp();
                    InsertFormattedText(RED, "Failed to initiate reset thread (error: %d)!\r\n", GetLastError());
                }

            }
            else {
                Timestamp();
                InsertFormattedText(RED, "You must wait for the operation to complete before attempting to factory reset!\r\n");
            }

        }
        else {
            Timestamp();
            InsertFormattedText(RED, "You must connect to the device before attempting to reset the device!\r\n");
        }

        break;

    // Cycle slots:
    case MENU_DEVICE_CYCLE_SLOTS:
    case BUTTON_CYCLE_SLOTS:

        if (Connection.State == Communication::Connection_Connected) {

            // May only proceed if a completion thread is not running
            if (Cronus.Thread == INVALID_HANDLE_VALUE) {

                unsigned char NumberOfSlots;

                NumberOfSlots = 0;

                for (unsigned i = 0; i < 8; i++)
                    if (Cronus.Slot[i].FileSize)
                        NumberOfSlots++;

                if (NumberOfSlots) {
                    // Notify user
                    Timestamp();
                    InsertFormattedText(GRAY, "Attempting to cycle slots...\r\n");

                    // Update connection status
                    Communication::UpdateConnectionState(Communication::Connection_CycleSlot);

                    // Perform cycle sequence
                    CronusZen::StreamIoStatus(CronusZen::Off);
                    CronusZen::ChangeSlotA();
                    CronusZen::StreamIoStatus(CronusZen::Off);
                    CronusZen::ChangeSlotB();
                    CronusZen::StreamIoStatus(CronusZen::InputReport | CronusZen::OutputReport);
                }
                else {
                    Timestamp();
                    InsertFormattedText(RED, "There are no slots to cycle!\r\n");
                }
            }
            else {
                Timestamp();
                InsertFormattedText(RED, "You must wait for the operation to complete prior to attempting to cycle to the next GPC!\r\n");
            }

        }
        else {
            Timestamp();
            InsertFormattedText(RED, "You must connect to the device before attempting to cycle to the next GPC!\r\n");
        }

        break;

    // Serial number:
    case MENU_DEVICE_SERIAL_NUMBER:
        // Notify user of serial number
        this->Timestamp();
        this->InsertFormattedText(GRAY, "Device serial number: %s.\r\n", Cronus.SerialNumber);
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
                        CheckMenuItem(this->MainMenu, MENU_DEVICE_REMOTE_PLAY, MF_UNCHECKED);
                        CheckMenuItem(this->MainMenu, MENU_DEVICE_PS4SPECIALITY, MF_CHECKED);
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
                        CheckMenuItem(this->MainMenu, MENU_DEVICE_REMOTE_PLAY, MF_CHECKED);
                        CheckMenuItem(this->MainMenu, MENU_DEVICE_PS4SPECIALITY, MF_UNCHECKED);
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

    // Load and run
    case MENU_PROGRAM_LOAD_AND_RUN:

        OPENFILENAMEA OpenFileName;
        char ChosenFile[MAX_PATH];

        memset(&ChosenFile, 0, sizeof(ChosenFile));
        memset(&OpenFileName, 0, sizeof(OPENFILENAMEA));

        // Open dialog only for *.bin files
        OpenFileName.lStructSize = sizeof(OPENFILENAMEA);
        OpenFileName.hwndOwner = this->Handle;
        OpenFileName.lpstrFile = ChosenFile;
        OpenFileName.nMaxFile = sizeof(ChosenFile);
        OpenFileName.lpstrFilter = "Compiled Scripts (*.bin)\0*.bin\0";
        OpenFileName.nFilterIndex = 1;
        OpenFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_DONTADDTORECENT | OFN_NOCHANGEDIR;

        // Open the dialog and set the label if the user selects a file
        if (GetOpenFileNameA(&OpenFileName) == TRUE) {

            PathFindFileNameA(OpenFileName.lpstrFile);

            if (Connection.State == Communication::Connection_Connected)
            {
                if (Cronus.Thread == INVALID_HANDLE_VALUE) {

                    // Find the position of the last backslash character
                    memcpy(Cronus.Slot[9].FilePath, OpenFileName.lpstrFile, sizeof(Cronus.Slot[9].FilePath));
                    memcpy(Cronus.Slot[9].FileName, PathFindFileNameA(OpenFileName.lpstrFile), 64);

                    // Notify user
                    Timestamp();
                    InsertFormattedText(GRAY, "Attempting to run script %s...\r\n", Cronus.Slot[9].FileName);

                    // Create factory reset thread
                    Cronus.Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CronusZen::ThreadProc, (LPVOID)Communication::Connection_RunScript, 0, NULL);

                    // Check for success
                    if (Cronus.Thread == INVALID_HANDLE_VALUE) {
                        Timestamp();
                        InsertFormattedText(RED, "Failed to initiate run script thread (error: %d)!\r\n", GetLastError());
                    }

                }
                else {
                    Timestamp();
                    InsertFormattedText(RED, "You must wait for the operation to complete before attempting to run a script!\r\n");
                }

            }
            else {
                Timestamp();
                InsertFormattedText(RED, "You must connect to the device before attempting to run a script!\r\n");
            }
        }
        else {
            Timestamp();
            InsertFormattedText(RED, "Failed to open, load and run %s!\r\n", PathFindFileNameA(OpenFileName.lpstrFile));
        }


        break;

    // Program device
    case MENU_PROGRAM_PROGRAM_DEVICE:
        if (Connection.State == Communication::Connection_Connected) {

            for (auto i = 0; i < 8; i++)
                if (strlen((const char*)Cronus.Slot[i].FileName))
                    Cronus.NumberOfPendingSlots++;

            // Validate that there are slots pending to write
            if (Cronus.NumberOfPendingSlots) {

                // Consolidate the slots
                for (auto i = 0, j = 1; j < 8; j++) {
                    if (!strlen((const char*)Cronus.Slot[i].FileName)) {
                        if (strlen((const char*)Cronus.Slot[j].FileName)) {
                            memcpy(&Cronus.Slot[i], &Cronus.Slot[j], sizeof(Cronus.Slot[j]));
                            memset(&Cronus.Slot[j], 0, sizeof(Cronus.Slot[j]));
                            i++;
                        }
                    }
                    else {
                        i++;
                    }
                }

                // Create factory reset thread
                Cronus.Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CronusZen::ThreadProc, (LPVOID)Communication::Connection_FlashConfig, 0, NULL);

                // Check for success
                if (Cronus.Thread == INVALID_HANDLE_VALUE) {
                    Timestamp();
                    InsertFormattedText(RED, "Failed to initiate flash config thread (error: %d)!\r\n", GetLastError());
                }

            }
            else {
                Timestamp();
                InsertFormattedText(RED, "You must set some slots to load scripts before attempting to program the slots!\r\n");
            }

        }
        else {
            Timestamp();
            InsertFormattedText(RED, "You must connect to the device before attempting to program the slots!\r\n");
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
          
        if (Device.Path != nullptr) {
            // Begin connection process
            Communication::Connect();
        }
        else {
            this->Timestamp();
            this->InsertFormattedText(RED, "Cannot connect; unable to locate a compatible USB device!\r\n");
        }

        break;

    case MENU_FILE_EXIT:
        return OnClose(wParam, lParam);

    // Clear output:
    case MENU_FILE_CLEAR_OUTPUT:
        SetWindowText(this->RichEditOutput, NULL);
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
                InsertFormattedText(GRAY, "Attempting to change the virtual machine speed to %dms...\r\n", SliderValue);
                CronusZen::SetVmCtrl(SliderValue);
                CronusZen::StreamIoStatus(CronusZen::InputReport | CronusZen::OutputReport);
            }
        }
    }

    return TRUE;
}

INT_PTR MainDialogManager::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
    RECT DialogRect;

    SendMessage(this->Handle, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ICO_ICON)));

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
    this->CycleSlotsButton = GetDlgItem(this->Handle, BUTTON_CYCLE_SLOTS);
    this->EraseMemorySlotsButton = GetDlgItem(this->Handle, BUTTON_ERASE_MEMORY_SLOTS);
    this->FactoryResetButton = GetDlgItem(this->Handle, BUTTON_FACTORY_RESET);
    this->ResetButton = GetDlgItem(this->Handle, BUTTON_RESET_DEVICE);
    
    // Combo boxes:
    this->EmulatorOutputComboBox = GetDlgItem(this->Handle, COMBO_EMULATOR_OUTPUT_PROTOCOL);
    this->RemoteSlotChangeComboBox = GetDlgItem(this->Handle, COMBO_REMOTE_SLOT_CHANGE);
    this->OperationalModeComboBox = GetDlgItem(this->Handle, COMBO_OPERATIONAL_MODE);

    // Menu:
    this->MainMenu = GetMenu(this->Handle);

    // Labels:
    this->CurrentSlotLabel = GetDlgItem(this->Handle, LABEL_CURRENT_SLOT);
    this->CPUUsageLabel = GetDlgItem(this->Handle, LABEL_CPU_USAGE);

    // Listbox:
    this->ScriptsListBox = GetDlgItem(this->Handle, LISTBOX_SCRIPTS);

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
    wcscpy_s(cf.szFaceName, L"Microsoft Sans Serif");
    SendMessage(this->RichEditOutput, EM_SETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cf);

    // Prepare combo drop list boxes
    // 
    // Emulator output protocol:
    SendMessageA(this->EmulatorOutputComboBox, CB_ADDSTRING, 0, (LPARAM)"Auto");
    SendMessageA(this->EmulatorOutputComboBox, CB_ADDSTRING, 0, (LPARAM)"PlayStation 3");
    SendMessageA(this->EmulatorOutputComboBox, CB_ADDSTRING, 0, (LPARAM)"PC/Mobile | Xbox 360");
    SendMessageA(this->EmulatorOutputComboBox, CB_ADDSTRING, 0, (LPARAM)"PlayStation 4/5");
    SendMessageA(this->EmulatorOutputComboBox, CB_ADDSTRING, 0, (LPARAM)"Xbox One X/S");
    SendMessageA(this->EmulatorOutputComboBox, CB_ADDSTRING, 0, (LPARAM)"Nintendo Switch");

    // Remote slot change:
    SendMessageA(this->RemoteSlotChangeComboBox, CB_ADDSTRING, 0, (LPARAM)"Disabled");
    SendMessageA(this->RemoteSlotChangeComboBox, CB_ADDSTRING, 0, (LPARAM)"PS/Xbox + Share/View");
    SendMessageA(this->RemoteSlotChangeComboBox, CB_ADDSTRING, 0, (LPARAM)"PS/Xbox + L3/LS");

    // Device operational mode:
    SendMessageA(this->OperationalModeComboBox, CB_ADDSTRING, 0, (LPARAM)"Wheel Mode");
    SendMessageA(this->OperationalModeComboBox, CB_ADDSTRING, 0, (LPARAM)"Standard Mode");
    SendMessageA(this->OperationalModeComboBox, CB_ADDSTRING, 0, (LPARAM)"Tournament Mode");

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
    InsertFormattedText(GRAY, "Welcome to Zen++ - Copyright (C) 2023 Swedemafia - version %d.%02d.\r\n", VERSION_MAJOR, VERSION_MINOR);
    Timestamp();
    InsertFormattedText(GRAY, "An Asgard production (https://discord.gg/tGH7QxtPam).\r\n");

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

    // Populate file list
    char CurrentDirectory[MAX_PATH] = "";
    
    if (GetCurrentDirectoryA(MAX_PATH, CurrentDirectory)) {

        // Format directory with type of files we are looking for
        wsprintfA(CurrentDirectory, "%s\\*.bin", CurrentDirectory);

        WIN32_FIND_DATAA FindData;
        HANDLE FindFile = FindFirstFileA(CurrentDirectory, &FindData);

        // Loop through each file and add it to the listbox
        if (FindFile != INVALID_HANDLE_VALUE) {
            do {
                if (!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                    SendMessageA(this->ScriptsListBox, LB_ADDSTRING, 0, (LPARAM)FindData.cFileName);
            } while (FindNextFileA(FindFile, &FindData));
        }
    }
    else {
        Timestamp();
        InsertFormattedText(RED, "Failed to query list of files in the current directory!\r\n");
    }

    StatusDialog = new StatusDialogManager(this->Instance, DLG_STATUS);
    StatusDialog->MakeDialog();

    RunScriptDialog = new RunScriptDialogManager(this->Instance, DLG_RUNSCRIPT);
    RunScriptDialog->MakeDialog();

    // Create SlotConfig directory
    CreateDirectoryA("SlotConfig", NULL);

    // Set window caption
    SetWindowTextA(this->Handle, "Zen++ - Copyright (C) 2023 - Swedemafia");

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

            TCHAR LinkClicked[64];
            TEXTRANGE tr;

            tr.chrg = pEnLink->chrg;
            tr.lpstrText = LinkClicked;

            // Get the actual link
            SendMessage(pnmhdr->hwndFrom, EM_GETTEXTRANGE, 0, (LPARAM)&tr);

            // Open it
            ShellExecute(this->Handle, L"open", LinkClicked, NULL, NULL, SW_SHOWNORMAL);
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

INT_PTR MainDialogManager::OnRButtonDown(WPARAM wParam, LPARAM lParam)
{
    int x = LOWORD(lParam);
    int y = HIWORD(lParam);
    RECT rect;

    // Loop through each of the slots (labels)
    for (unsigned i = 0; i < 8; i++) {

        HWND SlotToCheck = GetDlgItem(this->Handle, LABEL_SLOT_1 + i);

        // Convert coordinates
        GetWindowRect(SlotToCheck, &rect);
        MapWindowPoints(NULL, this->Handle, reinterpret_cast<POINT*>(&rect), 2);

        if (PtInRect(&rect, { x, y })) {

            if (Cronus.Slot[i].FileSize) {

                POINT CursorPosition;
                HMENU PopupMenu = LoadMenu(this->Instance, MAKEINTRESOURCE(MENU_POPUP_REMOVE));

                // Get cursor position
                GetCursorPos(&CursorPosition);

                // Display the popup menu
                TrackPopupMenu(GetSubMenu(LoadMenu(this->Instance, MAKEINTRESOURCE(MENU_POPUP_REMOVE)), 0), TPM_LEFTALIGN | TPM_TOPALIGN, CursorPosition.x, CursorPosition.y, 0, this->Handle, NULL);

                // Destroy the menu
                DestroyMenu(PopupMenu);

                this->PopupSlot = i;
            }

            break;
        }

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
    InsertFormattedText(WHITE, "[%02i:%02i:%02i.%03i] ", SystemTime.wHour % 12, SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds);
}

void MainDialogManager::UpdateCaption(const char* Status)
{
    char Output[512] = "";

    wsprintfA(Output, "Zen++ - Copyright (C) 2023 Swedemafia - %s", Status);

    // Set window caption
    SetWindowTextA(this->Handle, Output);
}

void MainDialogManager::UpdatePluginMenu(void)
{
    if (Plugin.Handle) {
        EnableMenuItem(MainDialog->MainMenu, MENU_PLUGINS_LOAD_PLUGIN, MF_DISABLED);
        EnableMenuItem(MainDialog->MainMenu, MENU_PLUGINS_UNLOAD_PLUGIN, MF_ENABLED);
    }
    else {
        EnableMenuItem(MainDialog->MainMenu, MENU_PLUGINS_LOAD_PLUGIN, MF_ENABLED);
        EnableMenuItem(MainDialog->MainMenu, MENU_PLUGINS_UNLOAD_PLUGIN, MF_DISABLED);
    }
}