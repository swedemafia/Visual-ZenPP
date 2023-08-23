#ifndef PLUGIN_API
#define PLUGIN_API
#pragma once

namespace PluginAPI
{
	// User functions
	typedef BOOL(WINAPI* PluginInitProc)(struct PluginInformation* lpInterfaceInfo);
	typedef BOOL(WINAPI* CommandHookProc)(LPCSTR ConsoleCommand, LPARAM lParam);
	typedef void(WINAPI* ConnectionHookProc)(DWORD EventCode, LPARAM lParam);
	typedef void(WINAPI* DeviceHookProc)(DWORD EventCode, LPARAM lParam);
	typedef void(WINAPI* MessageHookProc)(BYTE PacketID, WORD PayloadSize, BYTE* Payload, LPARAM lParam);
	typedef void(WINAPI* TerminateHookProc)(LPARAM lParam);

	// Prototypes of emulator API Functions

	// UI:
	typedef int(WINAPI* WriteOutputTimestampProc)(void);
	typedef void(WINAPI* WriteOutputStringProc)(UINT Color, LPCSTR FormattedText);

	// Actions:
	typedef BOOL(WINAPI* EventPressProc)(BYTE Identifier);
	typedef BOOL(WINAPI* EventReleaseProc)(BYTE Identifier);

	// Controller:
	typedef BYTE(WINAPI* GetBatteryValueProc)(void);
	typedef char(WINAPI* GetInputValueProc)(BYTE Identifier);
	typedef char(WINAPI* GetLastInputValueProc)(BYTE Identifier);
	typedef char(WINAPI* GetOutputValueProc)(BYTE Identifier);
	typedef DWORD(WINAPI* GetPressTimeProc)(BYTE Identifier);
	typedef DWORD(WINAPI* GetReleaseTimeProc)(BYTE Identifier);
	typedef BYTE(WINAPI* GetRumbleProc)(CronusZen::RumbleValue Rumble);
	typedef int(WINAPI* GetTraceValueProc)(CronusZen::TraceValue Value);
	typedef BYTE(WINAPI* GetPs5AdtDataProc)(CronusZen::Ps5AdtValue Value);
	typedef void(WINAPI* SetValProc)(BYTE Identifier, BYTE Value);

	// Device:
	typedef void(WINAPI* EnterApiModeProc)(void);
	typedef void(WINAPI* ExitApiModeProc)(void);
	typedef void(WINAPI* ForwardAllInputsProc)(void);
	typedef CronusZen::ConsoleType(WINAPI* GetConnectedConsoleProc)(void);
	typedef CronusZen::ControllerType(WINAPI* GetConnectedControllerProc)(void);
	typedef WORD(WINAPI* GetCpuLoadValueProc)(void);
	typedef BYTE(WINAPI* GetLedStateProc)(BYTE Index);
	typedef BYTE(WINAPI* GetSlotValueProc)(void);
	typedef BYTE(WINAPI* GetVmSpeedValueProc)(void);
	typedef void(WINAPI* SetIoStatusProc)(CronusZen::StreamIoStatusMask StatusMask);

	// Information used by plugins for read/write addresses
	struct PluginInformation {
		DWORD Size;

		// Zen++ API functions
		// UI:
		WriteOutputTimestampProc	WriteOutputTimestamp;
		WriteOutputStringProc		WriteOutputString;

		// Actions:
		EventPressProc				EventPress;
		EventReleaseProc			EventRelease;

		// Controller:
		GetBatteryValueProc			GetBatteryValue;
		GetInputValueProc			GetInputValue;
		GetLastInputValueProc		GetLastInputValue;
		GetOutputValueProc			GetOutputValue;
		GetPressTimeProc			GetPressTime;
		GetReleaseTimeProc			GetReleaseTime;
		GetRumbleProc				GetRumble;
		GetPs5AdtDataProc			GetPs5AdtData;
		SetValProc					SetVal;

		// Device:
		EnterApiModeProc			EnterApiMode;
		ExitApiModeProc				ExitApiMode;
		ForwardAllInputsProc		ForwardAllInputs;
		GetConnectedConsoleProc		GetConnectedConsole;
		GetConnectedControllerProc	GetConnectedController;
		GetCpuLoadValueProc			GetCpuLoadValue;
		GetLedStateProc				GetLedState;
		GetSlotValueProc			GetSlotValue;
		GetTraceValueProc			GetTraceValue;
		GetVmSpeedValueProc			GetVmSpeedValue;
		SetIoStatusProc				SetIoStatus;

		// Hooks
		ConnectionHookProc			ConnectionHook;
		CommandHookProc				CommandHook;
		DeviceHookProc				DeviceHook;
		MessageHookProc				MessageHook;
		TerminateHookProc			TerminateHook;
		LPARAM						HookParam;
	};

	// Loaded plugin information
	struct LoadedPlugin {
		HMODULE Handle;
		PluginInformation Information;
		char Name[MAX_PATH];
		size_t MenuID;
		HMENU PopupMenu;
	};

	// User functions:
	BOOL LoadPlugin(LPCSTR File);
	BOOL UnloadPlugin(void);

	// API functions

	// Actions:
	BOOL WINAPI EventPress(BYTE Identifier);
	BOOL WINAPI EventRelease(BYTE Identifier);

	// Controller:
	BYTE WINAPI GetBatteryValue(void);
	char WINAPI GetInputValue(BYTE Identifier);
	char WINAPI GetLastInputValue(BYTE Identifier);
	char WINAPI GetOutputValue(BYTE Identifier);
	DWORD WINAPI GetPressTime(BYTE Identifier);
	DWORD WINAPI GetReleaseTime(BYTE Identifier);
	BYTE WINAPI GetRumble(CronusZen::RumbleValue Rumble);
	BYTE WINAPI GetPs5AdtData(CronusZen::Ps5AdtValue Value);
	void WINAPI SetVal(BYTE Identifier, BYTE Value);

	// Device:
	void WINAPI EnterApiMode(void);
	void WINAPI ExitApiMode(void);
	void WINAPI ForwardAllInputs(void);
	CronusZen::ConsoleType WINAPI GetConnectedConsole(void);
	CronusZen::ControllerType WINAPI GetConnectedController(void);
	WORD WINAPI GetCpuLoadValue(void);
	BYTE WINAPI GetLedState(BYTE Index);
	BYTE WINAPI GetSlotValue(void);
	int WINAPI GetTraceValue(CronusZen::TraceValue Value);
	BYTE WINAPI GetVmSpeedValue(void);
	void WINAPI SetIoStatus(CronusZen::StreamIoStatusMask StatusMask);
}

#endif