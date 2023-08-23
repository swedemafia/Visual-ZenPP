#include "PCH.h"

BOOL PluginAPI::LoadPlugin(LPCSTR File)
{
	memset(&Plugin, 0, sizeof(Plugin));
	Plugin.Handle = LoadLibraryA(File);

	// Attempt to load the plugin
	if (Plugin.Handle) {

		PluginInitProc SetInterfaceInformation = (PluginInitProc)GetProcAddress(Plugin.Handle, "SetInterfaceInformation");

		if (!SetInterfaceInformation) {
			FreeLibrary(Plugin.Handle);
			return FALSE;
		}

		// Prepare plugin information
		Plugin.Information.Size = sizeof(PluginInformation);

		// Set API addresses:
		// UI:
		Plugin.Information.WriteOutputTimestamp = Emulator::WriteOutputTimestamp;
		Plugin.Information.WriteOutputString = Emulator::WriteFormattedOutput;

		// Actions:
		Plugin.Information.EventPress = EventPress;
		Plugin.Information.EventRelease = EventRelease;

		// Controller:
		Plugin.Information.GetBatteryValue = GetBatteryValue;
		Plugin.Information.GetInputValue = GetInputValue;
		Plugin.Information.GetLastInputValue = GetLastInputValue;
		Plugin.Information.GetOutputValue = GetOutputValue;
		Plugin.Information.GetPressTime = GetPressTime;
		Plugin.Information.GetReleaseTime = GetReleaseTime;
		Plugin.Information.GetRumble = GetRumble;
		Plugin.Information.GetPs5AdtData = GetPs5AdtData;
		Plugin.Information.SetVal = SetVal;

		// Device:
		Plugin.Information.EnterApiMode = EnterApiMode;
		Plugin.Information.ExitApiMode = ExitApiMode;
		Plugin.Information.ForwardAllInputs = ForwardAllInputs;
		Plugin.Information.GetConnectedConsole = GetConnectedConsole;
		Plugin.Information.GetConnectedController = GetConnectedController;
		Plugin.Information.GetCpuLoadValue = GetCpuLoadValue;
		Plugin.Information.GetLedState = GetLedState;
		Plugin.Information.GetSlotValue = GetSlotValue;
		Plugin.Information.GetTraceValue = GetTraceValue;
		Plugin.Information.GetVmSpeedValue = GetVmSpeedValue;
		Plugin.Information.SetIoStatus = SetIoStatus;

		// Attempt to set plugin interface information
		if (!SetInterfaceInformation(&Plugin.Information)) {
			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(RED, "Failed to set interface information for plugin!\r\n");
			return FALSE;
		}

		// Set loaded plugin information
		memcpy(Plugin.Name, File, MAX_PATH);

		// Add to list and notify user
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(GREEN, "Successfully loaded plugin %s!\r\n", PathFindFileNameA(File));
		MainDialog->UpdatePluginMenu();

		return TRUE;
	}

	// Unable to load library
	MainDialog->Timestamp();
	MainDialog->InsertFormattedText(RED, "Failed to load plugin %s!\r\n", PathFindFileNameA(File));

	return FALSE;
}

BOOL PluginAPI::UnloadPlugin(void)
{

	Plugin.Information.TerminateHook(Plugin.Information.HookParam);

	// Attempt to unload the plugin
	if (!FreeLibrary(Plugin.Handle)) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Failed to unload plugin %s!\r\n", PathFindFileNameA(Plugin.Name));
		return FALSE;
	}

	Plugin.Handle = 0;

	// Remove from list and notify user
	MainDialog->Timestamp();
	MainDialog->InsertFormattedText(GREEN, "Successfully unload plugin %s!\r\n", PathFindFileNameA(Plugin.Name));
	MainDialog->UpdatePluginMenu();

	// Update handle
	memset(&Plugin, 0, sizeof(Plugin));

	return TRUE;
}

// Actions:
BOOL WINAPI PluginAPI::EventPress(BYTE Identifier)
{
	if ((Identifier >= 0) && (Identifier <= 37)) {
		if (Cronus.InputReport.Input[Identifier] && !Cronus.LastInputReport.Input[Identifier]) {
			Cronus.LastInputReport.Input[Identifier] = Cronus.InputReport.Input[Identifier];
			return TRUE;
		}
	}

	return FALSE;
}

BOOL WINAPI PluginAPI::EventRelease(BYTE Identifier)
{
	if ((Identifier >= 0) && (Identifier <= 37)) {
		if (!Cronus.InputReport.Input[Identifier] && Cronus.LastInputReport.Input[Identifier]) {
			Cronus.LastInputReport.Input[Identifier] = 0;
			return TRUE;
		}
	}

	return FALSE;
}

// Controller:
BYTE WINAPI PluginAPI::GetBatteryValue(void)
{
	return Cronus.InputReport.BatteryValue;
}

char WINAPI PluginAPI::GetInputValue(BYTE Identifier)
{
	if ((Identifier >= 0) && (Identifier <= 37))
		return Cronus.InputReport.Input[Identifier];
	else
		return 0;
}

char WINAPI PluginAPI::GetLastInputValue(BYTE Identifier)
{
	if ((Identifier >= 0) && (Identifier <= 37))
		return Cronus.LastInputReport.Input[Identifier];
	else
		return 0;
}


char WINAPI PluginAPI::GetOutputValue(BYTE Identifier)
{
	if ((Identifier >= 0) && (Identifier <= 37))
		return Cronus.OutputReport.Output[Identifier];
	else
		return 0;
}

DWORD WINAPI PluginAPI::GetPressTime(BYTE Identifier)
{
	if ((Identifier >= 0) && (Identifier <= 37))
		return Cronus.InputReport.PressTime[Identifier];
	else
		return 0;
}

DWORD WINAPI PluginAPI::GetReleaseTime(BYTE Identifier)
{
	if ((Identifier >= 0) && (Identifier <= 37))
		return Cronus.InputReport.ReleaseTime[Identifier];
	else
		return 0;
}

BYTE WINAPI PluginAPI::GetRumble(CronusZen::RumbleValue Rumble)
{
	if ((Rumble >= CronusZen::RumbleA) && (Rumble <= CronusZen::RumbleLT))
		return Cronus.InputReport.Rumble[Rumble];
	else
		return 0;
}

BYTE WINAPI PluginAPI::GetPs5AdtData(CronusZen::Ps5AdtValue Value)
{
	if (Value >= 0 && Value <= 21) {
		return Cronus.Ps5Adt[Value];
	}
	else {
		return 0;
	}
}

void WINAPI PluginAPI::SetVal(BYTE Identifier, BYTE Value)
{
	if ((Identifier >= 0) && (Identifier <= 37))
		Cronus.Output[Identifier] = Value;
}

// Device:
void WINAPI PluginAPI::EnterApiMode(void)
{
	CronusZen::EnterApiMode();
	Cronus.ApiMode = TRUE; // Enable API mode
}

void WINAPI PluginAPI::ExitApiMode(void)
{
	Cronus.ApiMode = FALSE; // Disable API mode
	CronusZen::ExitApiMode();
}

void WINAPI PluginAPI::ForwardAllInputs(void)
{
	for (unsigned i = 0; i < 38; i++) {
		Cronus.Output[i] = Cronus.InputReport.Input[i];
	}
}

CronusZen::ConsoleType WINAPI PluginAPI::GetConnectedConsole(void)
{
	return (CronusZen::ConsoleType)Cronus.InputReport.ConnectedConsole;
}

CronusZen::ControllerType WINAPI PluginAPI::GetConnectedController(void)
{
	return (CronusZen::ControllerType)Cronus.InputReport.ConnectedController;
}

WORD WINAPI PluginAPI::GetCpuLoadValue(void)
{
	return Cronus.InputReport.CpuLoadValue;
}

BYTE WINAPI PluginAPI::GetLedState(BYTE Index)
{
	return Cronus.InputReport.LedState[Index];
}

BYTE WINAPI PluginAPI::GetSlotValue(void)
{
	return Cronus.InputReport.SlotValue;
}

int WINAPI PluginAPI::GetTraceValue(CronusZen::TraceValue Value)
{
	switch (Value) {
	case CronusZen::TRACE_1:
		return Cronus.OutputReport.Trace1;
	case CronusZen::TRACE_2:
		return Cronus.OutputReport.Trace2;
	case CronusZen::TRACE_3:
		return Cronus.OutputReport.Trace3;
	case CronusZen::TRACE_4:
		return Cronus.OutputReport.Trace4;
	case CronusZen::TRACE_5:
		return Cronus.OutputReport.Trace5;
	case CronusZen::TRACE_6:
		return Cronus.OutputReport.Trace6;
	default:
		return 0;
	}
}

BYTE WINAPI PluginAPI::GetVmSpeedValue(void)
{
	return Cronus.InputReport.VmSpeedValue;
}

void WINAPI PluginAPI::SetIoStatus(CronusZen::StreamIoStatusMask StatusMask)
{
	CronusZen::StreamIoStatus((BYTE)StatusMask);
}