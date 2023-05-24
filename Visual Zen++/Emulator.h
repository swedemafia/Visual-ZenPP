#ifndef EMULATOR_H
#define EMULATOR_H
#pragma once

namespace Emulator
{
	enum DeviceState {
		Device_Init = -1,
		Device_NotFound,
		Device_Found
	};

	// Structures:
	struct ConsoleInformation {
		HANDLE InputHandle;
		HANDLE InputThread;
		HANDLE OutputHandle;
		INPUT_RECORD InputBuffer[128];
		char InputCharacters[256];
		unsigned int InputLength;
	};

	struct DeviceInformation {
		DeviceState State;
		TCHAR* BusReportedDescription;
		TCHAR* Description;
		TCHAR* Path;
		TCHAR* HardwareID;
		TCHAR ProductID[9];
		TCHAR VendorID[9];
	};

	// User functions:
	DWORD DeviceThreadProc(LPVOID Parameter);
	const char* FromUnicode(LPCWSTR String);
	BOOL LocateDevice(LPCWSTR DeviceName);
	BOOL LocateDevicePath(void);
	void UpdateDeviceState(DeviceState EventCode);

	// Display functions:
	//void WriteFormattedOutput(UINT Color, LPCSTR Format, ...);
	int WINAPI WriteOutputTimestamp(void);
	//int WINAPI WriteOutputString(LPCSTR String);
}

#endif