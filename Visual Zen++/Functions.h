#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#pragma once

enum MenuListIDs {
    MenuList_EmulatorOutputProtocol,
    MenuList_Operational,
    MenuList_RemoteSlotChange
};

const unsigned MenuIDs[][7] = {
    { 6, MENU_DEVICE_EMULATOR_AUTO, MENU_DEVICE_EMULATOR_NINTENDOSWITCH, MENU_DEVICE_EMULATOR_PLAYSTATION3, MENU_DEVICE_EMULATOR_PLAYSTATION45, MENU_DEVICE_EMULATOR_XBOX360, MENU_DEVICE_EMULATOR_XBOXONE },
    { 3, MENU_DEVICE_OPERATIONAL_STANDARD, MENU_DEVICE_OPERATIONAL_TOURNAMENT, MENU_DEVICE_OPERATIONAL_WHEEL },
    { 3, MENU_DEVICE_REMOTESLOT_DISABLED, MENU_DEVICE_REMOTESLOT_LEFTSTICK, MENU_DEVICE_REMOTESLOT_SELECT }
};

// Global functions:
void ChangeDS4LightbarBrightness(unsigned char Value);
void ChangeEmulatorOutputProtocol(MainDialogManager::EmulatorOutputProtocolIDs Protocol);
void ChangeOperationalMode(MainDialogManager::OperationalModeIDs Mode);
void ChangePS4Speciality(unsigned char Value);
void ChangeRemotePlay(unsigned char Value);
void ChangeRemoteSlotChange(MainDialogManager::RemoteSlotChangeIDs Type);
void ChangeVMSpeedValue(unsigned char Value);

// Menu convienence:
void UncheckSubMenuItems(MenuListIDs ListID, int MenuToCheck);

#endif