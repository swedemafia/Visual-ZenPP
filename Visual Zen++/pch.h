#ifndef PCH_H
#define PCH_H
#pragma once

// Additional dependencies
#pragma comment (lib, "hid.lib")
#pragma comment (lib, "cfgmgr32.lib")
#pragma comment (lib, "setupapi.lib")

#include <list>
#include <string>
#include <queue>
#include <tchar.h>
#include <Windows.h>
#include <cfgmgr32.h>
#include <SetupAPI.h>
#include <hidsdi.h>
#include <Richedit.h>

#include "resource.h"
#include "Crypt.h"
#include "ParseBuffer.h"
#include "StoreBuffer.h"
#include "Communication.h"
#include "CronusZen.h"
#include "Emulator.h"
#include "DialogManager.h"
#include "MainDialog.h"
#include "AppSettings.h"
#include "Functions.h"

#undef DEFINE_DEVPROPKEY
#define DEFINE_DEVPROPKEY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8, pid) EXTERN_C const DEVPROPKEY DECLSPEC_SELECTANY name = { { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }, pid }
DEFINE_DEVPROPKEY(DEVPKEY_Device_BusReportedDeviceDesc, 0x540b947e, 0x8b40, 0x45bc, 0xa8, 0xa2, 0x6a, 0x0b, 0x89, 0x4c, 0xbd, 0xa2, 4);


// Global classes
extern class MainDialogManager* MainDialog;

// Global structures
extern struct Communication::ConnectionInformation Connection;
extern struct CronusZen::CronusZenInformation Cronus;
extern struct Emulator::ConsoleInformation Console;
extern struct Emulator::DeviceInformation Device;

#endif
