#include "pch.h"

void ChangeDS4LightbarBrightness(unsigned char Value)
{
    if (Connection.State == Communication::Connection_Connected && Value == Cronus.Ds4LightbarBrightness)
        return;

    if (Connection.State == Communication::Connection_Connected || Connection.State == Communication::Connection_Connecting) {
        MainDialog->Timestamp();
        MainDialog->InsertFormattedText(YELLOW, "DS4 lightbar brightness is now set to %d%%.\t\n", Value);

        // Set slider value
        SendMessage(MainDialog->DS4LightbarBrightnessSlider, TBM_SETPOS, TRUE, (LPARAM)Value);
    }
    else if (Connection.State == Communication::Connection_Disconnected) {
        MainDialog->Timestamp();
        MainDialog->InsertFormattedText(RED, "You must be connected to a compatible device before attempting to change your DS4 controller lightbar brightness!\r\n");
    }
    else {
        MainDialog->Timestamp();
        MainDialog->InsertFormattedText(RED, "You must wait for the current operation to complete before attempting to change your DS4 controller lightbar brightness!\r\n");
    }
}

void ChangeEmulatorOutputProtocol(MainDialogManager::EmulatorOutputProtocolIDs Protocol)
{
    if (Connection.State == Communication::Connection_Connected && Protocol == Cronus.EmulatorOutputProtocol)
        return;

    if (Connection.State == Communication::Connection_Connected || Connection.State == Communication::Connection_Connecting) {

        switch (Protocol) {
        case MainDialogManager::Emulator_Auto:
            UncheckSubMenuItems(MenuList_EmulatorOutputProtocol, MENU_DEVICE_EMULATOR_AUTO);
            CronusZen::SendSingleFragment(CronusZen::EmulatorOutputProtocol_Auto);
            MainDialog->Timestamp();
            MainDialog->InsertFormattedText(YELLOW, _c("Emulator output protocol is set to automatic.\r\n"));
            break;

        case MainDialogManager::Emulator_NintendoSwitch:
            UncheckSubMenuItems(MenuList_EmulatorOutputProtocol, MENU_DEVICE_EMULATOR_NINTENDOSWITCH);
            CronusZen::SendSingleFragment(CronusZen::EmulatorOutputProtocol_NintendoSwitch);
            MainDialog->Timestamp();
            MainDialog->InsertFormattedText(YELLOW, _c("Emulator output protocol is set to Nintendo Switch.\r\n"));
            break;

        case MainDialogManager::Emulator_PlayStation3:
            UncheckSubMenuItems(MenuList_EmulatorOutputProtocol, MENU_DEVICE_EMULATOR_PLAYSTATION3);
            CronusZen::SendSingleFragment(CronusZen::EmulatorOutputProtocol_PlayStation3);
            MainDialog->Timestamp();
            MainDialog->InsertFormattedText(YELLOW, _c("Emulator output protocol is set to PlayStation 3.\r\n"));
            break;

        case MainDialogManager::Emulator_PlayStation45:
            UncheckSubMenuItems(MenuList_EmulatorOutputProtocol, MENU_DEVICE_EMULATOR_PLAYSTATION45);
            CronusZen::SendSingleFragment(CronusZen::EmulatorOutputProtocol_PlayStation45);
            MainDialog->Timestamp();
            MainDialog->InsertFormattedText(YELLOW, _c("Emulator output protocol is set to PlayStation 4/5.\r\n"));
            break;

        case MainDialogManager::Emulator_Xbox360:
            UncheckSubMenuItems(MenuList_EmulatorOutputProtocol, MENU_DEVICE_EMULATOR_XBOX360);
            CronusZen::SendSingleFragment(CronusZen::EmulatorOutputProtocol_Xbox360);
            MainDialog->Timestamp();
            MainDialog->InsertFormattedText(YELLOW, _c("Emulator output protocol is set to Xbox 360.\r\n"));
            break;

        case MainDialogManager::Emulator_XboxOne:
            UncheckSubMenuItems(MenuList_EmulatorOutputProtocol, MENU_DEVICE_EMULATOR_XBOXONE);
            CronusZen::SendSingleFragment(CronusZen::EmulatorOutputProtocol_XboxOne);
            MainDialog->Timestamp();
            MainDialog->InsertFormattedText(YELLOW, _c("Emulator output protocol is set to Xbox One.\r\n"));
            break;

        }

        // Update emulator protocol
        Cronus.EmulatorOutputProtocol = Protocol;

        // Set combo box index
        SendMessage(MainDialog->EmulatorOutputComboBox, CB_SETCURSEL, Protocol, 0);

    }
    else if (Connection.State == Communication::Connection_Disconnected) {
        MainDialog->Timestamp();
        MainDialog->InsertFormattedText(RED, "You must be connected to a compatible device before attempting to change the emulator output protocol!\r\n");
    }
    else {
        MainDialog->Timestamp();
        MainDialog->InsertFormattedText(RED, "You must wait for the current operation to complete before attempting to change the emulator output protocol!\r\n");
    }
}

void ChangeOperationalMode(MainDialogManager::OperationalModeIDs Mode)
{
    if (Connection.State == Communication::Connection_Connected && Mode == Cronus.DeviceMode)
        return;

    if (Connection.State == Communication::Connection_Connected || Connection.State == Communication::Connection_Connecting) {

        switch (Mode) {
        case MainDialogManager::Operational_Standard:
            UncheckSubMenuItems(MenuList_Operational, MENU_DEVICE_OPERATIONAL_STANDARD);
            CronusZen::SendSingleFragment(CronusZen::DeviceMode_Normal);
            MainDialog->Timestamp();
            MainDialog->InsertFormattedText(YELLOW, _c("Opertional mode is set to standard.\r\n"));

            break;

        case MainDialogManager::Operational_Tournament:
            UncheckSubMenuItems(MenuList_Operational, MENU_DEVICE_OPERATIONAL_TOURNAMENT);
            CronusZen::SendSingleFragment(CronusZen::DeviceMode_Tournament);
            MainDialog->Timestamp();
            MainDialog->InsertFormattedText(YELLOW, _c("Operational mode is set to tournament mode.\r\n"));

            break;

        case MainDialogManager::Operational_Wheel:
            UncheckSubMenuItems(MenuList_Operational, MENU_DEVICE_OPERATIONAL_WHEEL);
            CronusZen::SendSingleFragment(CronusZen::DeviceMode_Wheel);
            MainDialog->Timestamp();
            MainDialog->InsertFormattedText(YELLOW, _c("Operational mode is set to wheel mode.\r\n"));

            break;
        }

        // Update operational mode
        Cronus.DeviceMode = Mode;

        // Set combo box index
        SendMessage(MainDialog->OperationalModeComboBox, CB_SETCURSEL, Mode, 0);
    }
    else if(Connection.State == Communication::Connection_Disconnected) {
        MainDialog->Timestamp();
        MainDialog->InsertFormattedText(RED, "You must be connected to a compatible device before attempting to change the operational mode!\r\n");
    }
    else {
        MainDialog->Timestamp();
        MainDialog->InsertFormattedText(RED, "You must wait for the current operation to complete before attempting to change the operational mode!\r\n");
    }
}

void ChangePS4Speciality(unsigned char Value)
{
    if (Connection.State == Communication::Connection_Connected || Connection.State == Communication::Connection_Connecting) {
        
        if (Cronus.Thread == INVALID_HANDLE_VALUE) {

            // Notify user
            MainDialog->Timestamp();
            MainDialog->InsertFormattedText(GRAY, _c("Attempting to %s PS4 Speciality...\r\n"), (Value) ? _c("enable") : _c("disable"));

            // Create factory reset thread
            Cronus.Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CronusZen::ThreadProc, (LPVOID)Communication::Connection_PS4Speciality, 0, NULL);

            // Check for success
            if (Cronus.Thread == INVALID_HANDLE_VALUE) {
                Cronus.Ps4Speciality = ~Cronus.Ps4Speciality;
                MainDialog->Timestamp();
                MainDialog->InsertFormattedText(RED, _c("Failed to initiate PS4 Speciality thread (error: %d)!\r\n"), GetLastError());
            }

        }
        else {
            MainDialog->Timestamp();
            MainDialog->InsertFormattedText(RED, _c("You must wait for the operation to complete before attempting to %s PS4 Speciality!\r\n"), (Value == 1) ? _c("enable") : _c("disable"));
        }

    }
    else if (Connection.State == Communication::Connection_Disconnected) {
        MainDialog->Timestamp();
        MainDialog->InsertFormattedText(RED, "You must be connected to a compatible device before attempting to %s PS4 Speciality!\r\n", (Value == 1) ? _c("enable") : _c("disable"));
    }
    else {
        MainDialog->Timestamp();
        MainDialog->InsertFormattedText(RED, "You must wait for the current operation to complete before attempting to %s PS4 Speciality!\r\n", (Value == 1) ? _c("enable") : _c("disable"));
    }
}

void ChangeRemotePlay(unsigned char Value)
{
    if (Connection.State == Communication::Connection_Connected || Connection.State == Communication::Connection_Connecting) {

        if (Cronus.Thread == INVALID_HANDLE_VALUE) {

            // Notify user
            MainDialog->Timestamp();
            MainDialog->InsertFormattedText(GRAY, _c("Attempting to %s Remote Play...\r\n"), (Value) ? _c("enable") : _c("disable"));

            // Create factory reset thread
            Cronus.Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CronusZen::ThreadProc, (LPVOID)Communication::Connection_RemotePlay, 0, NULL);

            // Check for success
            if (Cronus.Thread == INVALID_HANDLE_VALUE) {
                Cronus.RemotePlay = ~Cronus.RemotePlay;
                MainDialog->Timestamp();
                MainDialog->InsertFormattedText(RED, _c("Failed to initiate Remote Play thread (error: %d)!\r\n"), GetLastError());
            }

        }
        else {
            MainDialog->Timestamp();
            MainDialog->InsertFormattedText(RED, _c("You must wait for the operation to complete before attempting to change Remote Play!\r\n"));
        }

    }
    else if (Connection.State == Communication::Connection_Disconnected) {
        MainDialog->Timestamp();
        MainDialog->InsertFormattedText(RED, "You must be connected to a compatible device before attempting to %s Remote Play!\r\n", (Value == 1) ? _c("enable") : _c("disable"));
    }
    else {
        MainDialog->Timestamp();
        MainDialog->InsertFormattedText(RED, "You must wait for the current operation to complete before attempting to %s Remote Play!\r\n", (Value == 1) ? _c("enable") : _c("disable"));
    }
}

void ChangeRemoteSlotChange(MainDialogManager::RemoteSlotChangeIDs Type)
{
    if (Connection.State == Communication::Connection_Connected && Type == Cronus.RemoteSlotChange)
        return;

    if (Connection.State == Communication::Connection_Connected || Connection.State == Communication::Connection_Connecting) {

        switch (Type) {
        case MainDialogManager::RemoteSlot_Disabled:
            UncheckSubMenuItems(MenuList_RemoteSlotChange, MENU_DEVICE_REMOTESLOT_DISABLED);
            CronusZen::SendSingleFragment(CronusZen::RemoteSlot_Disable);
            MainDialog->Timestamp();
            MainDialog->InsertFormattedText(YELLOW, _c("Remote slot change is disabled.\r\n"));
            break;

        case MainDialogManager::RemoteSlot_L3LS:
            UncheckSubMenuItems(MenuList_RemoteSlotChange, MENU_DEVICE_REMOTESLOT_LEFTSTICK);
            CronusZen::SendSingleFragment(CronusZen::RemoteSlot_PSL3);
            MainDialog->Timestamp();
            MainDialog->InsertFormattedText(YELLOW, _c("Remote slot change is set to PS/Xbox + L3/LS.\r\n"));
            break;

        case MainDialogManager::RemoteSlot_ShareView:
            UncheckSubMenuItems(MenuList_RemoteSlotChange, MENU_DEVICE_REMOTESLOT_SELECT);
            CronusZen::SendSingleFragment(CronusZen::RemoteSlot_PSSHARE);
            MainDialog->Timestamp();
            MainDialog->InsertFormattedText(YELLOW, _c("Remote slot change is set to PS/Xbox + Share/View.\r\n"));
            break;

        }

        // Update remote slot change
        Cronus.RemoteSlotChange = Type;

        // Set combo box index
        SendMessage(MainDialog->RemoteSlotChangeComboBox, CB_SETCURSEL, Type, 0);

    }
    else if (Connection.State == Communication::Connection_Disconnected) {
        MainDialog->Timestamp();
        MainDialog->InsertFormattedText(RED, "You must be connected to a compatible device before attempting to change the remote slot change type!\r\n");
    }
    else {
        MainDialog->Timestamp();
        MainDialog->InsertFormattedText(RED, "You must wait for the current operation to complete before attempting to change the remote slot change type!\r\n");
    }
}

void ChangeVMSpeedValue(unsigned char Value)
{
    if (Connection.State == Communication::Connection_Connected && Value == Cronus.VMSpeedValue)
        return;

    if (Connection.State == Communication::Connection_Connected || Connection.State == Communication::Connection_Connecting) {
        MainDialog->Timestamp();
        MainDialog->InsertFormattedText(YELLOW, "Virtual machine (VM) speed is now %dms.\r\n", Value);

        // Set slider value
        SendMessage(MainDialog->VMSpeedSlider, TBM_SETPOS, TRUE, (LPARAM)Value);
    }
    else if (Connection.State == Communication::Connection_Disconnected) {
        MainDialog->Timestamp();
        MainDialog->InsertFormattedText(RED, "You must be connected to a compatible device before attempting to change the virtual machine speed!\r\n");
    }
    else {
        MainDialog->Timestamp();
        MainDialog->InsertFormattedText(RED, "You must wait for the current operation to complete before attempting to change virtual machine speed!\r\n");
    }
}

void UncheckSubMenuItems(MenuListIDs ListID, int MenuToCheck)
{
    // Loop through and uncheck each one
    for (unsigned i = 1; i <= MenuIDs[ListID][0]; i++) {
        // Uncheck the sub menu
        CheckMenuItem(MainDialog->MainMenu, MenuIDs[ListID][i], MF_UNCHECKED);
    }

    // Check the menu we want to be checked
    CheckMenuItem(MainDialog->MainMenu, MenuToCheck, MF_CHECKED);
}