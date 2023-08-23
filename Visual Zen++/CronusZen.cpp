#include "PCH.h"

BOOL CronusZen::CheckBitmask(BYTE Value, BYTE ShiftBits)
{
	return ((Value & 1) << ShiftBits) != 0;
}

void CronusZen::DisplaySlotsCfg(void)
{
	unsigned char UsedSlots = 0;
	unsigned long TotalBytes = 0;

	for (auto i = 0; i < 8; i++) {
		// Only notify user if the slot has a file size, will be 0 if there is no GPC in this slot
		if (Cronus.Slot[i].FileSize) {
			UsedSlots++;
			TotalBytes += Cronus.Slot[i].FileSize;
			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(YELLOW, "Slot #%d: %s (%d bytes).\r\n", i + 1, Cronus.Slot[i].FileName, Cronus.Slot[i].FileSize);
		}
	}

	// Notify user if no slots are configured
	if (!TotalBytes) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(YELLOW, "No slot configuration data returned.\r\n");
	}
	else {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(YELLOW, "%d slot%sconfigured using %d bytes of flash memory.\r\n", UsedSlots, UsedSlots == 1 ? " is " : "s ", TotalBytes);
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(YELLOW, "%d%% flash memory used with %d bytes available.\r\n", (unsigned int)((float)((float)TotalBytes / (float)262140) * 100), 262140 - TotalBytes);
	}
}

void CronusZen::WriteFlashConfig(void)
{

	if (MainDialog->StatusDialog != nullptr && !MainDialog->StatusDialog->IsVisible()) {
		MainDialog->StatusDialog->DisplayWindow();
	}

	// Only proceed if there are slots left to write
	if (Cronus.NumberOfPendingSlots) {

		// Attempt to send flash config command
		if (!FlashConfig()) {

			if (Cronus.NumberOfPendingSlots - 1) {
				MainDialog->Timestamp();
				MainDialog->InsertFormattedText(YELLOW, "Advancing all pending slots forward a slot...\r\n");
			}

			// Move to next slot, increase number of errors, and reduce number of pending slots
			Cronus.SlotToWrite++;
			Cronus.NumberOfErrorSlots++;
			Cronus.NumberOfPendingSlots--;
			WriteFlashConfig();
			return;
		}

		// Reduce how many slots are left to write
		Cronus.NumberOfPendingSlots--;

	}
	else {
		// Close status dialog
		if (MainDialog->StatusDialog != nullptr) {
			MainDialog->StatusDialog->HideWindow();
		}

		MainDialog->EnableDialog();

		// Reset our static variable keeping track of which slot to write
		Cronus.NumberOfErrorSlots = 0;
		Cronus.SlotToWrite = 0;

		// Update connection status
		Communication::UpdateConnectionState(Communication::Connection_Connected);

		// Request to resume data stream
		CronusZen::StreamIoStatus(CronusZen::InputReport | CronusZen::OutputReport | ((Cronus.InputReport.ConnectedController == Controller_Ps5) ? CronusZen::Ps5Adt : 0));

		// Notify user
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(GRAY, "Requesting slots configuration...\r\n");

		// Required delay
		Sleep(3000);

		// Send checksum result to force save the slots
		CronusZen::Cl();

		// Request slots config
		CronusZen::ReadSlotsCfg();
	}
}

void CronusZen::OnInputReport(void)
{

	if (Connection.State == Communication::Connection_DeviceCleanup) {

		// Notify user of success
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(GREEN, "Successfully erased slots on the device!\r\n");

		// Reset connection state
		Communication::UpdateConnectionState(Communication::Connection_Connected);

	}
	else if (Connection.State == Communication::Connection_ClearBtCommand) {

		// Notify user of success
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(GREEN, "Successfully cleared registered Bluetooth devices!\r\n");

		// Reset connection state
		Communication::UpdateConnectionState(Communication::Connection_Connected);

	}
	else {

		//memcpy(&Cronus.LastInputReport, &Cronus.InputReport, sizeof(InputReportInformation));

		// Process CPU load
		WORD CPULoad = Parse->ExtractShort();

		if (Cronus.InputReport.CpuLoadValue != CPULoad) {
			char CPULoadDisplay[24] = { 0 };
			wsprintfA(CPULoadDisplay, "CPU: %d%% @ %dms", CPULoad, Cronus.InputReport.VmSpeedValue);
			SetWindowTextA(MainDialog->CPUUsageLabel, CPULoadDisplay);
		}

		Cronus.InputReport.CpuLoadValue = CPULoad;

		// Extract current slot
		Cronus.InputReport.SlotValue = Parse->ExtractByte();

		if (Cronus.CurrentSlot != Cronus.InputReport.SlotValue) {

			// Update display label
			char Buffer[32] = "";

			if (Cronus.InputReport.SlotValue)
				wsprintfA(Buffer, "Current slot: %d", Cronus.InputReport.SlotValue);
			else
				wsprintfA(Buffer, "No slot loaded");

			SetWindowTextA(MainDialog->CurrentSlotLabel, (LPCSTR)Buffer);

			if (!Cronus.InputReport.SlotValue) {

				// See if user issued the command
				if (Connection.State == Communication::Connection_CycleSlot) {

					// Notify user
					if (Cronus.CurrentSlot != 10) {
						MainDialog->Timestamp();
						MainDialog->InsertFormattedText(GREEN, "Successfully cycled slots and unloaded %s!\r\n", Cronus.Slot[Cronus.CurrentSlot - 1].FileName);
					} 
					else {
						MainDialog->Timestamp();
						MainDialog->InsertFormattedText(GREEN, "Successfully stopped and unloaded %s!\r\n", Cronus.Slot[Cronus.CurrentSlot - 1].FileName);
					}

					// Update connection status
					Communication::UpdateConnectionState(Communication::Connection_Connected);

				}
				else if (Connection.State == Communication::Connection_UnloadGpc) {

					// Notify user
					if (Cronus.CurrentSlot != 10) {
						MainDialog->Timestamp();
						MainDialog->InsertFormattedText(GREEN, "Successfully unloaded %s (slot #%d)!\r\n", Cronus.Slot[Cronus.CurrentSlot - 1].FileName, Cronus.CurrentSlot);
					} 
					else {
						MainDialog->Timestamp();
						MainDialog->InsertFormattedText(GREEN, "Successfully stopped and unloaded %s!\r\n", Cronus.Slot[Cronus.CurrentSlot - 1].FileName);
					}

					// Update connection status
					Communication::UpdateConnectionState(Communication::Connection_Connected);
				}
				else {
					// GPC unloaded
					if (Cronus.CurrentSlot != 10) {
						MainDialog->Timestamp();
						MainDialog->InsertFormattedText(YELLOW, "%s from slot #%d has been unloaded!\r\n", Cronus.Slot[Cronus.CurrentSlot - 1].FileName, Cronus.CurrentSlot);
					}
					else {

						// Sometimes the device will report back on bootup so we must check to make sure it's worth reporting
						if (strlen((const char*)Cronus.Slot[Cronus.CurrentSlot - 1].FileName)) {
							MainDialog->Timestamp();
							MainDialog->InsertFormattedText(GREEN, "Successfully terminated the execution of %s!\r\n", Cronus.Slot[Cronus.CurrentSlot - 1].FileName);
						}

					}

					// Reset connection state
					Communication::UpdateConnectionState(Communication::Connection_Connected);

				}

			}
			else {

				if (Cronus.InputReport.SlotValue == 10) {

					if (strlen((const char*)Cronus.Slot[9].FileName)) {
						// Notify user
						MainDialog->Timestamp();
						MainDialog->InsertFormattedText(GREEN, "Successfully loaded and executed %s!\r\n", Cronus.Slot[9].FileName);

						if (Connection.State == Communication::Connection_RunScript) {
							// Reset connection state
							Communication::UpdateConnectionState(Communication::Connection_Connected);
						}
					}

				}
				else {

					// See if user issued the command
					if (Connection.State == Communication::Connection_CycleSlot) {

						// Notify user
						MainDialog->Timestamp();
						MainDialog->InsertFormattedText(GREEN, "Successfully loaded %s from slot #%d!\r\n", Cronus.Slot[Cronus.CurrentSlot].FileName, Cronus.CurrentSlot + 1);

						// Update connection status
						Communication::UpdateConnectionState(Communication::Connection_Connected);
					}
					else {
						MainDialog->Timestamp();
						MainDialog->InsertFormattedText(YELLOW, "%s from slot #%d is now loaded!\r\n", Cronus.Slot[Cronus.CurrentSlot].FileName, Cronus.CurrentSlot + 1);
					}

				}
			}

			// Set current slot
			Cronus.CurrentSlot = Cronus.InputReport.SlotValue;
		}

		// Extract remaining data
		Cronus.InputReport.ConnectedController = Parse->ExtractByte();
		Cronus.InputReport.ConnectedConsole = Parse->ExtractByte();
		Cronus.InputReport.LedState[0] = Parse->ExtractByte();
		Cronus.InputReport.LedState[1] = Parse->ExtractByte();
		Cronus.InputReport.LedState[2] = Parse->ExtractByte();
		Cronus.InputReport.LedState[3] = Parse->ExtractByte();
		Cronus.InputReport.Rumble[RumbleA] = Parse->ExtractByte();
		Cronus.InputReport.Rumble[RumbleB] = Parse->ExtractByte();
		Cronus.InputReport.Rumble[RumbleRT] = Parse->ExtractByte();
		Cronus.InputReport.Rumble[RumbleLT] = Parse->ExtractByte();
		Cronus.InputReport.BatteryValue = Parse->ExtractByte();

		// Get input values
		for (unsigned i = 0; i < 38; i++) {
			Cronus.LastInputReport.Input[i] = Cronus.InputReport.Input[i];
			Cronus.InputReport.Input[i] = (char)Parse->ExtractByte();

			// Check if button is pressed, reset PressTime
			if (!Cronus.LastInputReport.Input[i] && Cronus.InputReport.Input[i]) {
				Cronus.InputReport.PressTime[i] = Cronus.InputReport.VmSpeedValue;
			}
			else if (Cronus.LastInputReport.Input[i] && !Cronus.InputReport.Input[i]) {
				Cronus.InputReport.ReleaseTime[i] = Cronus.InputReport.VmSpeedValue;
			}
			else {
				Cronus.InputReport.ReleaseTime[i] += Cronus.InputReport.VmSpeedValue;
				Cronus.InputReport.PressTime[i] += Cronus.InputReport.VmSpeedValue;
			}
		}

		Parse->Advance(4);

		BYTE ReportType = Parse->ExtractByte();

		Cronus.InputReport.IsJoypad = CheckBitmask(ReportType, 0);
		Cronus.InputReport.IsConsole = CheckBitmask(ReportType, 1);
		Cronus.InputReport.IsMouse = CheckBitmask(ReportType, 2);
		Cronus.InputReport.IsKeyboard = CheckBitmask(ReportType, 3);
		Cronus.InputReport.IsNavcon = CheckBitmask(ReportType, 4);
		Cronus.InputReport.IsG13 = CheckBitmask(ReportType, 5);
		Cronus.InputReport.IsMirror = CheckBitmask(ReportType, 7);

		Cronus.InputReport.VmSpeedValue = Parse->ExtractByte();
		Cronus.InputReport.TimestampCounter = Parse->ExtractShort();

		if (Cronus.InputReport.VmSpeedValue != Cronus.VMSpeedValue) {

			if (Cronus.InputReport.VmSpeedValue) {
				char VMSpeedDisplay[24] = { 0 };
				wsprintfA(VMSpeedDisplay, "CPU: %d%% @ %dms", CPULoad, Cronus.InputReport.VmSpeedValue);
				SetWindowTextA(MainDialog->CPUUsageLabel, VMSpeedDisplay);
				ChangeVMSpeedValue(Cronus.InputReport.VmSpeedValue);
			}

			Cronus.VMSpeedValue = Cronus.InputReport.VmSpeedValue;
		}
	}
}

void CronusZen::OnOutputReport(void)
{
	// Extract controller button values
	for (unsigned i = 0; i < 38; i++) {
		Cronus.LastOutputReport.Output[i] = Cronus.OutputReport.Output[i];
		Cronus.OutputReport.Output[i] = Parse->ExtractByte();
	}

	// Extract TRACE values
	// This can probably just be done by extracting 32-bit/16-bit values...
	Cronus.OutputReport.Trace1 = (Parse->ExtractByte() | (Parse->ExtractByte() << 8) | (Parse->ExtractByte() << 16) | (Parse->ExtractByte() << 24));
	Cronus.OutputReport.Trace2 = (Parse->ExtractByte() | (Parse->ExtractByte() << 8) | (Parse->ExtractByte() << 16) | (Parse->ExtractByte() << 24));
	Cronus.OutputReport.Trace3 = (Parse->ExtractByte() | (Parse->ExtractByte() << 8) | (Parse->ExtractByte() << 16) | (Parse->ExtractByte() << 24));
	Cronus.OutputReport.Trace4 = (Parse->ExtractByte() | (Parse->ExtractByte() << 8));
	Cronus.OutputReport.Trace5 = (Parse->ExtractByte() | (Parse->ExtractByte() << 8));
	Cronus.OutputReport.Trace6 = (Parse->ExtractByte() | (Parse->ExtractByte() << 8));

	Parse->Advance(1);

	Cronus.OutputReport.CfgState = Parse->ExtractByte();

	// Check for API mode
	if (Cronus.ApiMode) {
		CronusZen::ApiMode(); // Send API mode data
	}
}

void CronusZen::OnPs5AdtReport(void)
{
	for (unsigned i = 0; i < sizeof(Cronus.Ps5Adt); i++)
		Cronus.Ps5Adt[i] = Parse->ExtractByte();
}

void CronusZen::OnGetStatus(void)
{
	BYTE LastPacketID;
	WORD BytesWritten;

	Parse->Advance(1); // Skip unknown
	LastPacketID = Parse->ExtractByte();
	Parse->Advance(6); // Skip unknown
	BytesWritten = Parse->ExtractShort();

	switch (LastPacketID) {
	case PKT_REQUESTMKFILE:

		// Continue with initial connection process
		if (Connection.State == Communication::Connection_Connecting)
			RequestAttachedDevices();

		break;

	case PKT_FLASHCONFIG:

		if (BytesWritten) {

			// Notify user of success
			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(GREEN, "Successfully flashed config for %s to slot #%d!\r\n", Cronus.Slot[Cronus.SlotToWrite].FileName, Cronus.SlotToWrite - Cronus.NumberOfErrorSlots + 1);

			// Attempt to flash gamepack, if not, move to next file
			if (!FlashGamepack())
				WriteFlashConfig();
		}
		else {
			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(RED, "Failed flashing config for %s to slot #%d!\r\n", Cronus.Slot[Cronus.SlotToWrite].FileName, Cronus.SlotToWrite - Cronus.NumberOfErrorSlots + 1);
			WriteFlashConfig();
		}

		break;

	case PKT_FLASHGAMEPACK:

		if (BytesWritten) {

			// Notify user of success
			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(GREEN, "Successfully flashed %s to slot #%d!\r\n", Cronus.Slot[Cronus.SlotToWrite].FileName, Cronus.SlotToWrite - Cronus.NumberOfErrorSlots + 1);

		}
		else {

			// Notify user of failure
			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(RED, "Failed flashing %s to slot #%d!\r\n", Cronus.Slot[Cronus.SlotToWrite].FileName, Cronus.SlotToWrite - Cronus.NumberOfErrorSlots + 1);
		}

		// Move to next slot
		Cronus.SlotToWrite++;

		// Move to next slot
		WriteFlashConfig();

		break;

	}
}

void CronusZen::OnExclusionListRead(void)
{
	// Continue with initial connection process
	if (Connection.State == Communication::Connection_Connecting)
		FragmentRead();
}

void CronusZen::OnRequestAttachedDevices(void)
{
	// Continue with initial connection process
	if (Connection.State == Communication::Connection_Connecting) {

		// Notify user
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(GRAY, "Requesting slots configuration...\r\n");

		ReadSlotsCfg();
	}
}

void CronusZen::OnRequestMkFile(void)
{
	// Continue with initial connection process
	if (Connection.State == Communication::Connection_Connecting)
		GetStatus();
	else if (Connection.State == Communication::Connection_FactoryReset) {

		// Notify user of success
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(GREEN, "Successfully factory reset the device!\r\n");

		// Reset connection state
		Communication::UpdateConnectionState(Communication::Connection_Connected);

		// Resume streaming input/output
		StreamIoStatus(InputReport | OutputReport | ((Cronus.InputReport.ConnectedController == Controller_Ps5) ? Ps5Adt : 0));
	}
}

void CronusZen::OnGetFw(void)
{
	WORD FirmwareLength = (WORD)Parse->Size() - 9;

	// Get firmware version
	Cronus.FirmwareVersion = new BYTE[FirmwareLength + 1]{ 0 };
	memcpy(Cronus.FirmwareVersion, Parse->ExtractStringA(), FirmwareLength);

	// Get initial checksum values
	Cronus.Checksum[0] = Parse->ExtractByte();
	Cronus.Checksum[1] = Parse->ExtractByte();
	Cronus.Checksum[2] = Parse->ExtractByte();
	Cronus.Checksum[3] = Parse->ExtractByte();

	// Notify user of firmware
	if (!_stricmp((const char*)Cronus.FirmwareVersion, "2.1.0-beta.68")) {
		Cronus.CommunicationVersion = CronusZen::Communication_New;
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(GREEN, "Using latest supported firmware: %s.\r\n", Cronus.FirmwareVersion);
	}
	else if (!_stricmp((const char*)Cronus.FirmwareVersion, "2.1.0-beta.50")) {
		Cronus.CommunicationVersion = CronusZen::Communication_Old;
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(GREEN, "Using supported firmware: %s.\r\n", Cronus.FirmwareVersion);
	}
	else {
		Cronus.CommunicationVersion = CronusZen::Communication_Old;
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(PURPLE, "Using unsupported firmware: %s.\r\n", Cronus.FirmwareVersion);
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Please upgrade your device to firmware 2.1.0-beta.50 or 2.1.0-beta.68!\r\n");
	}

	// Request serial number (only if we are in the initial connection process)
	if (Connection.State == Communication::Connection_Connecting)
		GetSerial();

}

void CronusZen::OnGetSerial(void)
{
	WORD SerialLength = (WORD)(Parse->Size() - 4) >> 1;

	// Get serial number
	Cronus.SerialNumber = new BYTE[SerialLength + 1]{ 0 };
	memcpy(Cronus.SerialNumber, Emulator::FromUnicode(Parse->ExtractStringW()), SerialLength);

	// Do not process/update the checksum values if we are already connected
	if (Connection.State == Communication::Connection_Connected)
		return;

	// Extract and update the checksum values
	if (Cronus.CommunicationVersion == CronusZen::Communication_New) {
		// Newer firmware
		Cronus.Checksum[0] ^= *(BYTE*)(Cronus.SerialNumber + 4);
		Cronus.Checksum[1] ^= *(BYTE*)(Cronus.SerialNumber);
		Cronus.Checksum[2] ^= *(BYTE*)(Cronus.SerialNumber + 2);
		Cronus.Checksum[3] ^= *(BYTE*)(Cronus.SerialNumber + 6);
	}
	else {
		// Older firmware
		Cronus.Checksum[0] ^= *(BYTE*)(Cronus.SerialNumber + 2);
		Cronus.Checksum[1] ^= *(BYTE*)(Cronus.SerialNumber + 4);
		Cronus.Checksum[2] ^= *(BYTE*)(Cronus.SerialNumber + 6);
		Cronus.Checksum[3] ^= *(BYTE*)(Cronus.SerialNumber);
	}

	// Continue with initial connection process
	if (Connection.State == Communication::Connection_Connecting)
		ExclusionListRead();
}

void CronusZen::OnFragmentRead(void)
{
	// Continue with initial connection process
	if (Connection.State == Communication::Connection_Connecting)
		RequestMkFile();

	// Retrieve DS4 lightbar brightness
	Parse->Advance(2);
	Cronus.Ds4LightbarBrightness = Parse->ExtractByte();
	Parse->Advance(1);

	// Retrieve remaining values
	Cronus.DeviceMode = (DeviceModeType)((Parse->ExtractLong() >> 16) & 0xff);
	Cronus.EmulatorOutputProtocol = (EmulatorOutputProtocolType)((Parse->ExtractLong() >> 16) & 0xff);
	Cronus.RemoteSlotChange = (RemoteSlotChangeType)((Parse->ExtractLong() >> 16) & 0xff);
	Cronus.Ps4Speciality = (Ps4SpecialityType)((Parse->ExtractLong() >> 16) & 0xff);
	Cronus.RemotePlay = (RemotePlayType)((Parse->ExtractLong() >> 16) & 0xff);

	// Notify user
	// Print DS4 lightbar brightness
	if ((Cronus.Ds4LightbarBrightness >= 0) && (Cronus.Ds4LightbarBrightness <= 100)) {
		ChangeDS4LightbarBrightness(Cronus.Ds4LightbarBrightness);
	}
	else {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Got unexpected DS4 lightbar brightness value: %d (expected 0 to 100)!\r\n", Cronus.Ds4LightbarBrightness);
	}

	// Print device operational mode
	if ((Cronus.DeviceMode >= 0) && (Cronus.DeviceMode <= 2)) {
		ChangeOperationalMode((MainDialogManager::OperationalModeIDs)Cronus.DeviceMode);
	}
	else {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Got unexpected operational mode value: %d (expected 0 to 2)!\r\n", Cronus.DeviceMode);
	}

	// Print device emulator output protocol
	if ((Cronus.EmulatorOutputProtocol >= 0) && (Cronus.EmulatorOutputProtocol <= 5)) {
		ChangeEmulatorOutputProtocol((MainDialogManager::EmulatorOutputProtocolIDs)Cronus.EmulatorOutputProtocol);
	}
	else {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Got unexpected emulator output protocol value: %d (expected 0 to 5)!\r\n", Cronus.EmulatorOutputProtocol);
	}

	// Print remote slot change configuration
	if ((Cronus.RemoteSlotChange >= 0) && (Cronus.RemoteSlotChange <= 2)) {
		ChangeRemoteSlotChange((MainDialogManager::RemoteSlotChangeIDs)Cronus.RemoteSlotChange);
	}
	else {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Got unexpected remote slot change value: %d (expected 0 to 2)!\r\n", Cronus.RemoteSlotChange);
	}

	// Print PS4 Speciality status
	if ((Cronus.Ps4Speciality >= 0) && (Cronus.Ps4Speciality <= 1)) {

		// Only prompt user if it is enabled
		if (Cronus.Ps4Speciality) {
			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(GREEN, "Device is using PS4 Speciality!\r\n");
		}

		SendMessage(MainDialog->PS4SpecialityRadio, BM_SETCHECK, (Cronus.Ps4Speciality) ? BST_CHECKED : BST_UNCHECKED, 0);
		CheckMenuItem(MainDialog->MainMenu, MENU_DEVICE_PS4SPECIALITY, (Cronus.Ps4Speciality) ? MF_CHECKED : MF_UNCHECKED);
	}
	else {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Got unexpected PS4 Speciality value: %d (expected 0 or 1)!\r\n", Cronus.Ps4Speciality);
		SendMessage(MainDialog->PS4SpecialityRadio, BM_SETCHECK, BST_UNCHECKED, 0);
		CheckMenuItem(MainDialog->MainMenu, MENU_DEVICE_PS4SPECIALITY, MF_UNCHECKED);
	}

	// Print PlayStation Remote Play status
	if ((Cronus.RemotePlay >= 0) && (Cronus.RemotePlay <= 1)) {

		// Only prompt user if it is enabled
		if (Cronus.RemotePlay) {
			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(GREEN, "Device is using PlayStation Remote Play!\r\n");
		}

		SendMessage(MainDialog->RemotePlayRadio, BM_SETCHECK, (Cronus.RemotePlay) ? BST_CHECKED : BST_UNCHECKED, 0);
		CheckMenuItem(MainDialog->MainMenu, MENU_DEVICE_REMOTE_PLAY, (Cronus.RemotePlay) ? MF_CHECKED : MF_UNCHECKED);
	}
	else {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Got unexpected Remote Play value: %d (expected 0 or 1)!\r\n", Cronus.RemotePlay);
		SendMessage(MainDialog->RemotePlayRadio, BM_SETCHECK, BST_CHECKED, 0);
		CheckMenuItem(MainDialog->MainMenu, MENU_DEVICE_REMOTE_PLAY, MF_UNCHECKED);
	}

}

void CronusZen::OnReadSlotsCfg(void)
{

	// Continue with initial connection process
	if ((Connection.State != Communication::Connection_Connected) && (Connection.State != Communication::Connection_FlashConfig)) {
		StreamIoStatus(InputReport | OutputReport | ((Cronus.InputReport.ConnectedController == Controller_Ps5) ? Ps5Adt : 0));

		// Update connection state
		Communication::UpdateConnectionState(Communication::Connection_Connected);

		// Update current slot label
		SetWindowTextA(MainDialog->CurrentSlotLabel, "No slot loaded");
	}

	// Reset current slot configuration
	if (Connection.State != Communication::Connection_FlashConfig) {
		memset(&Cronus.Slot, 0, sizeof(Cronus.Slot));
	}

	// Clear slot labels
	MainDialog->ClearVisibleSlots();

	// Loop through and extract each slot configuration data
	for (auto i = 0; i < 8; i++) {

		if (Connection.State != Communication::Connection_FlashConfig) {
			Parse->Advance(7); // Unknown
			Parse->Advance(1); // Slot number (BYTE SlotNumber = Parse->ExtractByte() & ~0x30;)
			Parse->Advance(44); // Unknown
			memcpy(Cronus.Slot[i].FileName, Parse->ExtractData(Cronus.Slot[i].FileName, 62), 62);
			memcpy(Cronus.Slot[i].FilePath, Cronus.Slot[i].FileName, sizeof(Cronus.Slot[i].FilePath));
			Parse->Advance(2);
			Cronus.Slot[i].FileSize = Parse->ExtractShort();
			Parse->Advance(2);
			Parse->Advance(6);
		}
		else {
			Parse->Advance(126);
		}

		Parse->Advance(382);

		BYTE OutputData[508] = { 0 };
		memcpy(&OutputData, Parse->Buffer() + 4 + (i * 508), sizeof(OutputData));

		// File size is always one less for whatever reason
		if (Cronus.Slot[i].FileSize)
			Cronus.Slot[i].FileSize--;

		// Set the slot information
		if (Cronus.Slot[i].FileSize) {

			// Append .bin to the end if it's not there
			if (*(WORD*)(Parse->Buffer() + 4 + (i * 508)) == 0xffff) {

				int FileNameLength = strlen((const char*)Cronus.Slot[i].FileName);

				if (FileNameLength > 58) {
					*(DWORD*)(Cronus.Slot[i].FileName + 59) = 'nib.';
				}
				else {
					if (*(DWORD*)(Cronus.Slot[i].FileName + FileNameLength - 4) == 'cpg.')
						*(DWORD*)(Cronus.Slot[i].FileName + FileNameLength - 4) = 'nib.';
				}

			}


			DWORD fileAttributes = GetFileAttributesA((LPCSTR)Cronus.Slot[i].FileName);

			if (!(fileAttributes != INVALID_FILE_ATTRIBUTES && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY))) {
				// Read byte code
				Cronus.ReadByteCode[i] = TRUE;
			}

			// Populate slot labels
			SetWindowTextA(GetDlgItem(MainDialog->Handle, LABEL_SLOT_1 + i), (LPCSTR)Cronus.Slot[i].FileName);

			char SlotConfigFileName[MAX_PATH] = "";
			wsprintfA(SlotConfigFileName, "SlotConfig//%i.dat", i);

			// Set SlotConfig file
			HANDLE SlotConfigFile = CreateFileA(SlotConfigFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);

			if (SlotConfigFile != INVALID_HANDLE_VALUE) {

				DWORD BytesWritten;

				// File found, set path to that file
				//memcpy(&Cronus.Slot[i].FilePath, Cronus.Slot[i].FileName, sizeof(Cronus.Slot[i].FilePath));

				// Attempt to dump data
				if (!WriteFile(SlotConfigFile, OutputData, sizeof(OutputData), &BytesWritten, NULL)) {
					MainDialog->Timestamp();
					MainDialog->InsertFormattedText(RED, "Failed to write slot config file for %s! (error: %d)\r\n", Cronus.Slot[i].FileName, GetLastError());
				}
				else {
					if (BytesWritten != sizeof(OutputData)) {
						MainDialog->Timestamp();
						MainDialog->InsertFormattedText(RED, "Writing slot config file for %s returned bytes written mismatch!\r\n", Cronus.Slot[i].FileName);
					}
				}

				// Close SlotConfig file
				CloseHandle(SlotConfigFile);
			}
			else {
				MainDialog->Timestamp();
				MainDialog->InsertFormattedText(RED, "Failed to create slot config file for %s! (error: %d)\r\n", Cronus.Slot[i].FileName, GetLastError());
			}
		}
	}

	// Only notify and request byte code if user is not flashing
	if (Connection.State != Communication::Connection_FlashConfig) {
		DisplaySlotsCfg();
		ReadByteCodes();
	}
	else {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(GRAY, "Cleaning up device slots; please wait...\r\n");
		CronusZen::DeviceCleanup();
		Sleep(5500);

		// Calculate total bytes
		float TotalBytes;
		unsigned int TotalSlots;

		// Initialize values
		TotalBytes = 0;
		TotalSlots = 0;

		// Calculate total number of slots
		for (unsigned i = 0; i < 8; i++) {
			TotalBytes += (float)Cronus.Slot[i].FileSize + 1;
			TotalSlots += (Cronus.Slot[i].FileSize > 0) ? 1 : 0;
		}

		if (TotalBytes >= 262140) {

			// Notify user
			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(GRAY, "Warning: not all scripts will be able to be written to the device!  Maximum flash memory usage exceeded!\r\n");

			unsigned int ByteCounter = 0;

			for (unsigned i = 0; i < 8; i++) {

				ByteCounter += Cronus.Slot[i].FileSize;

				// Notify user
				if (ByteCounter >= 262140) {

					if (Cronus.Slot[i].FileSize) {
						MainDialog->Timestamp();
						MainDialog->InsertFormattedText(RED, "Unable to write %s to slot #%d!\r\n", Cronus.Slot[i].FileName, i + 1);
						TotalBytes -= Cronus.Slot[i].FileSize;
						TotalSlots--;
						Cronus.NumberOfPendingSlots--;
						memset(&Cronus.Slot[i], 0, sizeof(Cronus.Slot[i]));
						SendDlgItemMessage(MainDialog->Handle, LABEL_SLOT_1 + i, WM_SETTEXT, 0, (LPARAM)"");
					}
				}
			}

		}

		// Set progress bar
		MainDialog->StatusDialog->SetTotalBytes(TotalSlots, TotalBytes);

		WriteFlashConfig();
	}
}

void CronusZen::ReadByteCodes(void)
{
	if (Cronus.SlotToRead != -1) {
		
		// See if slot needs to be read
		if (Cronus.ReadByteCode[Cronus.SlotToRead]) {
			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(GRAY, "Requesting byte code for %s...\r\n", Cronus.Slot[Cronus.SlotToRead].FileName);
			CronusZen::ReadByteCode(Cronus.SlotToRead);
		}
		else {
			// Move to the next slot
			if (Cronus.SlotToRead + 1 < 8) {
				Cronus.SlotToRead++;
				ReadByteCodes();
			}
			else {
				Cronus.SlotToRead = 0;
			}
		}
	}
	else {
		Cronus.SlotToRead = 0;
	}
}

void CronusZen::OnReadByteCode(void)
{

	if (Parse->Size() >= Cronus.Slot[Cronus.SlotToRead].FileSize) {
		HANDLE FileHandle = CreateFileA((LPCSTR)Cronus.Slot[Cronus.SlotToRead].FileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW, 0, NULL);

		if (FileHandle == INVALID_HANDLE_VALUE) {
			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(RED, "Downloaded byte code but failed to create new file %s! (error: %d)\r\n", Cronus.Slot[Cronus.SlotToRead].FileName, GetLastError());
		}
		else {

			DWORD BytesWritten;

			// Attempt to write the file
			if (!WriteFile(FileHandle, Parse->Buffer() + 4, Cronus.Slot[Cronus.SlotToRead].FileSize, &BytesWritten, NULL)) {
				MainDialog->Timestamp();
				MainDialog->InsertFormattedText(RED, "Downloaded byte code but failed to write byte code to file %s! (error: %d)\r\n", Cronus.Slot[Cronus.SlotToRead].FileName, GetLastError());
			}

			// Check bytes written
			if (BytesWritten == Cronus.Slot[Cronus.SlotToRead].FileSize) {
				// Notify user
				MainDialog->Timestamp();
				MainDialog->InsertFormattedText(GREEN, "Successfully downloaded slot #%d and saved as %s!\r\n", Cronus.SlotToRead + 1, Cronus.Slot[Cronus.SlotToRead].FileName);
			}

			// Close file
			CloseHandle(FileHandle);
		}

		// Disable it from being read again
		Cronus.ReadByteCode[Cronus.SlotToRead] = FALSE;

		// Move to the next slot
		if (Cronus.SlotToRead + 1 < 8) {
			Cronus.SlotToRead++;
			ReadByteCodes();
		}
		else {
			Cronus.SlotToRead = 0;
		}
	}
	else {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Gamepacks are not able to be downloaded from the device!\r\n");
	}
}

void CronusZen::DispatchMessageHook(BYTE PacketID, WORD PayloadSize, BYTE* Payload)
{
	// Notify plugins
	if (Plugin.Handle) {
		//Plugin.Information.MessageHook(PacketID, PayloadSize, Payload, Plugin.Information.HookParam);
	}
}

void CronusZen::ParsePacket(BYTE PacketID, WORD BytesReceived, WORD ExpectedBytes, PacketProcessorProc CompletionRoutine) {
	if (BytesReceived < ExpectedBytes) {
		//OnMismatchPacketSize(PacketID, ExpectedBytes);
	}
	else
		CompletionRoutine();
}

void CronusZen::PreparsePacket(BYTE* Data, WORD Length)
{
	// Create new parse buffer
	Parse = new ParseBuffer(Data, Length);

	// Get Packet ID and skip the rest
	BYTE PacketID = Parse->ExtractByte();
	WORD PayloadSize = Parse->ExtractShort();

	// Do not care about the sequence ID because we only process all the data
	Parse->Advance(1);

	// Dispatch MessageHook for plugins
	DispatchMessageHook(PacketID, PayloadSize, Data + 4);

	// Extract Packet ID, we really do not care about the rest since it is combined into one total packet
	switch (PacketID) {
	case PKT_INPUTREPORT:
		ParsePacket(PKT_INPUTREPORT, Length, 0, &CronusZen::OnInputReport);
		break;

	case PKT_OUTPUTREPORT:
		ParsePacket(PKT_OUTPUTREPORT, Length, 0, &CronusZen::OnOutputReport);
		break;

	case PKT_PS5ADTDATA:
		ParsePacket(PKT_PS5ADTDATA, Length, 0, &CronusZen::OnPs5AdtReport);
		break;

	case PKT_GETSTATUS:
		ParsePacket(PKT_GETSTATUS, Length, 14, &CronusZen::OnGetStatus);
		break;

	case PKT_EXCLUSIONLISTREAD:
		ParsePacket(PKT_EXCLUSIONLISTREAD, Length, 60, &CronusZen::OnExclusionListRead);
		break;

	case PKT_REQUESTATTACHEDDEVICES:
		ParsePacket(PKT_REQUESTATTACHEDDEVICES, Length, 100, &CronusZen::OnRequestAttachedDevices);
		break;

	case PKT_REQUESTMKFILE:
		ParsePacket(PKT_REQUESTMKFILE, Length, 0, &CronusZen::OnRequestMkFile);
		break;

	case PKT_GETFW:
		ParsePacket(PKT_GETFW, Length, 9, &CronusZen::OnGetFw);
		break;

	case PKT_GETSERIAL:
		ParsePacket(PKT_GETSERIAL, Length, 20, &CronusZen::OnGetSerial);
		break;

	case PKT_FRAGMENTREAD:
		ParsePacket(PKT_FRAGMENTREAD, Length, 0, &CronusZen::OnFragmentRead);
		break;

	case PKT_READSLOTSCFG:
		ParsePacket(PKT_READSLOTSCFG, Length, 4068, &CronusZen::OnReadSlotsCfg);
		break;

	case PKT_READBYTECODE:
		ParsePacket(PKT_READBYTECODE, Length, 0, &CronusZen::OnReadByteCode);
		break;

	}

	delete Parse;
}

DWORD CronusZen::ThreadProc(Communication::ConnectionState Parameter)
{
	switch (Parameter) {
	case Communication::Connection_ClearBtCommand:

		// Update connection state
		Communication::UpdateConnectionState(Communication::Connection_ClearBtCommand);

		// Perform clear Bluetooth device sequence
		CronusZen::StreamIoStatus(CronusZen::Off);
		Sleep(15);
		CronusZen::TurnOffController();
		Sleep(30);
		CronusZen::StreamIoStatus(CronusZen::Off);
		Sleep(15);
		CronusZen::ClearBtCommand();
		Sleep(15);
		CronusZen::StreamIoStatus(CronusZen::InputReport | CronusZen::OutputReport | ((Cronus.InputReport.ConnectedController == Controller_Ps5) ? CronusZen::Ps5Adt : 0));

		// Wait 1 second
		Sleep(1000);

		// Check if the connection state updated
		if (Connection.State != Communication::Connection_Connected) {

			// Prompt user status
			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(RED, "Clearing registered Bluetooth devices failed; the device did not respond within the expected timeframe!\r\n");

			// Reset state back to connected
			Communication::UpdateConnectionState(Communication::Connection_Connected);
		}

		break;

	case Communication::Connection_DeviceCleanup:

		// Update connection state
		Communication::UpdateConnectionState(Communication::Connection_DeviceCleanup);

		// Perform erase slot sequence
		CronusZen::StreamIoStatus(CronusZen::Off);
		CronusZen::DeviceCleanup();

		// Wait 5 seconds
		Sleep(5000);

		// Request to resume data stream
		CronusZen::StreamIoStatus(CronusZen::InputReport | CronusZen::OutputReport | ((Cronus.InputReport.ConnectedController == Controller_Ps5) ? CronusZen::Ps5Adt : 0));

		// Check if we received a response back
		Sleep(1000);

		// Check if the connection state updated
		if (Connection.State != Communication::Connection_Connected) {

			// Prompt user status
			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(RED, "Erase slots failed; the device did not respond within the expected timeframe!\r\n");

			// Reset state back to connected
			Communication::UpdateConnectionState(Communication::Connection_Connected);
		}
		else {

			memset(&Cronus.Slot, 0, sizeof(Cronus.Slot));

			// Clear slot labels
			for (unsigned i = 0; i < 8; i++)
				SetWindowTextA(GetDlgItem(MainDialog->Handle, LABEL_SLOT_1 + i), (LPCSTR)"");

			// Refresh slot config
			CronusZen::ReadSlotsCfg();
		}

		break;

	case Communication::Connection_FactoryReset:

		// Update connection state
		Communication::UpdateConnectionState(Communication::Connection_FactoryReset);

		// Perform factory reset sequence
		CronusZen::StreamIoStatus(CronusZen::Off);
		CronusZen::FactoryReset();

		// Wait 8 seconds
		Sleep(8000);

		// RequestMkFile
		CronusZen::RequestMkFile();

		// Check if we received a response back
		Sleep(1000);

		// Check if the connection state updated
		if (Connection.State != Communication::Connection_Connected) {

			// Prompt user status
			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(RED, "Factory reset failed; the device did not respond within the expected timeframe!\r\n");

			// Reset state back to connected
			Communication::UpdateConnectionState(Communication::Connection_Connected);
		}

		break;


	case Communication::Connection_FlashConfig:

		// Update connection state
		Communication::UpdateConnectionState(Communication::Connection_FlashConfig);

		// Perform flash config sequence
		CronusZen::StreamIoStatus(CronusZen::Off);
		Sleep(100);

		// Disable dialog
		MainDialog->DisableDialog();
		
		// Read slots config and save it before we flash device
		CronusZen::ReadSlotsCfg();

		break;

	case Communication::Connection_PS4Speciality:

		// Update connection state
		Communication::UpdateConnectionState(Communication::Connection_PS4Speciality);

		// Perform remote play status change sequence
		CronusZen::SendSingleFragment((DWORD)((Cronus.Ps4Speciality == 1) ? CronusZen::Ps4Speciality_Enabled : CronusZen::Ps4Speciality_Disabled));
		Sleep(30);
		CronusZen::StreamIoStatus(CronusZen::Off);
		Sleep(15);

		// Check if Remote Play is enabled and disable it
		if (Cronus.RemotePlay) {
			CronusZen::SendSingleFragment((DWORD)CronusZen::RemotePlay_Disabled);
			Sleep(30);
			CronusZen::StreamIoStatus(CronusZen::Off);
			Sleep(15);
			Cronus.RemotePlay = 0;
		}

		
		CronusZen::SendSingleFragment((DWORD)CronusZen::EmulatorOutputProtocol_Auto);
		Sleep(15);
		CronusZen::StreamIoStatus(CronusZen::InputReport | CronusZen::OutputReport | ((Cronus.InputReport.ConnectedController == Controller_Ps5) ? CronusZen::Ps5Adt : 0));
		Sleep(5000);

		// Notify user of status
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(GREEN, "Successfully %s PS4 Speciality!\r\n", (Cronus.Ps4Speciality == CronusZen::Ps4Speciality_Enabled) ? "enabled" : "disabled");
		CronusZen::StreamIoStatus(CronusZen::Off);
		Sleep(15);

		// Notify user of status
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(GRAY, "Attempting to reset the device...\r\n");

		// Update connection state and perform reset
		Communication::UpdateConnectionState(Communication::Connection_ResetDevice);
		CronusZen::ResetDevice();

		// Clear visible slots
		MainDialog->ClearVisibleSlots();

		break;

	case Communication::Connection_RemotePlay:

		// Update connection state
		Communication::UpdateConnectionState(Communication::Connection_RemotePlay);

		// Perform remote play status change sequence
		CronusZen::SendSingleFragment((DWORD)((Cronus.RemotePlay == 1) ? CronusZen::RemotePlay_Enabled : CronusZen::RemotePlay_Disabled));
		Sleep(30);
		CronusZen::StreamIoStatus(CronusZen::Off);
		Sleep(15);
		CronusZen::SendSingleFragment((DWORD)((Cronus.RemotePlay == 1) ? CronusZen::EmulatorOutputProtocol_PlayStation45 : CronusZen::EmulatorOutputProtocol_Auto));
		Sleep(30);
		CronusZen::StreamIoStatus(CronusZen::Off);
		Sleep(15);

		// Check if PS4 Speciality is enabled and disable it
		if (Cronus.Ps4Speciality) {
			CronusZen::SendSingleFragment((DWORD)CronusZen::Ps4Speciality_Disabled);
			Sleep(30);
			CronusZen::StreamIoStatus(CronusZen::Off);
			Sleep(15);
			Cronus.Ps4Speciality = 0;
		}

		CronusZen::StreamIoStatus(CronusZen::InputReport | CronusZen::OutputReport | ((Cronus.InputReport.ConnectedController == Controller_Ps5) ? CronusZen::Ps5Adt : 0));
		Sleep(5000);

		// Notify user of status
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(GREEN, "Successfully %s Remote Play!\r\n", (Cronus.RemotePlay) ? "enabled" : "disabled");
		CronusZen::StreamIoStatus(CronusZen::Off);
		Sleep(15);

		// Notify user of status
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(GRAY, "Attempting to reset the device...\r\n");

		// Update connection state and perform reset
		Communication::UpdateConnectionState(Communication::Connection_ResetDevice);
		CronusZen::ResetDevice();

		// Clear visible slots
		MainDialog->ClearVisibleSlots();

		break;

	case Communication::Connection_ResetDevice:

		// Update connection state
		Communication::UpdateConnectionState(Communication::Connection_ResetDevice);

		// Perform reset sequence
		CronusZen::StreamIoStatus(CronusZen::Off);

		// Wait 5 seconds
		Sleep(5000);

		// Resume reset sequence
		CronusZen::ResetDevice();

		break;

	case Communication::Connection_RunScript:

		// Update connection state
		Communication::UpdateConnectionState(Communication::Connection_RunScript);

		// Disable streaming
		CronusZen::StreamIoStatus(CronusZen::Off);
		Sleep(100);

		CronusZen::RunScript((const char*)Cronus.Slot[9].FilePath);

		// Check if we received a response back
		Sleep(1000);

		// Check if the connection state updated
		if (Connection.State != Communication::Connection_Connected) {

			// Prompt user status
			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(RED, "Run script failed failed; the device did not respond within the expected timeframe!\r\n");

			// Reset state back to connected
			Communication::UpdateConnectionState(Communication::Connection_Connected);

			// Resume streaming
			CronusZen::StreamIoStatus(CronusZen::InputReport | CronusZen::OutputReport | ((Cronus.InputReport.ConnectedController == Controller_Ps5) ? CronusZen::Ps5Adt : 0));
		}

		break;

	}

	// Close thread handle
	CloseHandle(Cronus.Thread);

	// Reset thread handle
	Cronus.Thread = INVALID_HANDLE_VALUE;

	// Exit thread
	ExitThread(0);

}

void CronusZen::RunScript(const char* Script)
{
	HANDLE ScriptFile = CreateFileA(Script, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	// Check if file was opened
	if (ScriptFile == INVALID_HANDLE_VALUE) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Failed to open script %s! (error: %d)\r\n", PathFindFileNameA(Script), GetLastError());
		return;
	}

	// Get filesize
	DWORD FileSize = GetFileSize(ScriptFile, NULL);

	// Check if file size was returned
	if (FileSize == INVALID_FILE_SIZE) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Failed to query file size of %s! (error: %d)\r\n", PathFindFileNameA(Script), GetLastError());
		CloseHandle(ScriptFile);
		return;
	}

	// Notify user of status
	MainDialog->Timestamp();
	MainDialog->InsertFormattedText(GREEN, "Successfully opened script %s!\r\n", PathFindFileNameA(Script));

	BYTE* FileData = new BYTE[FileSize]{ 0 };

	// Check if memory was successfully allocated
	if (!FileData) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Failed to allocate %d bytes of memory while reading the script file!\r\n", FileSize);
		CloseHandle(ScriptFile);
		return;
	}

	DWORD BytesRead;

	// Attempt to read file data
	if (!ReadFile(ScriptFile, FileData, FileSize, &BytesRead, NULL)) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Failed to read file data from %s! (error: %d)\r\n", PathFindFileNameA(Script), GetLastError());
		CloseHandle(ScriptFile);
		delete[] FileData;
		return;
	}

	// Check bytes read versus file size
	if (BytesRead != FileSize) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Failed to read file data from %s (read %d bytes, expected %d bytes)!\r\n", PathFindFileNameA(Script), BytesRead, FileSize);
		CloseHandle(ScriptFile);
		delete[] FileData;
		return;
	}

	// Store a copy of the script file data
	Cronus.Slot[9].FileSize = FileSize;

	// Notify user of the status
	MainDialog->Timestamp();
	MainDialog->InsertFormattedText(GRAY, "Initiating transfer of %d bytes of data...\r\n", FileSize);

	// Pause IoStream
	StreamIoStatus(Off);

	// Process file data
	BYTE* LastData = new BYTE[60]{ 0 };
	DWORD BytesSent = 0;

	MainDialog->RunScriptDialog->SetTotalBytes((float)FileSize);
	MainDialog->RunScriptDialog->DisplayWindow();

	unsigned short PreviousPercentage = 0;

	while (BytesSent != FileSize) {
		Communication::OutgoingPacket RunScript(PKT_RUNSCRIPT);

		if (FileSize - BytesSent < 60) {

			// Insert the remaining file data and pad it with the previous packet data
			RunScript.InsertData(FileData + BytesSent, FileSize - BytesSent);
			RunScript.InsertData(LastData + FileSize - BytesSent, 60 - (FileSize - BytesSent));
			RunScript.Finalize(FileSize, (!BytesSent) ? 1 : 0);

			// Increase bytes sent to keep track how much of the file data has been sent
			BytesSent += (FileSize - BytesSent);

			// Update progress bar
			MainDialog->RunScriptDialog->UpdateProgress((float)(FileSize - BytesSent));
		}
		else {

			// Copy the last data which is used to pad the final packet
			memcpy(LastData, FileData + BytesSent, 60);

			// Insert the next 60 bytes of file data
			RunScript.InsertData(FileData + BytesSent, 60);
			RunScript.Finalize(FileSize, (!BytesSent) ? 1 : 0);

			// Increase bytes sent to keep track how much of the file data has been sent
			BytesSent += 60;

			// Update progress bar
			MainDialog->RunScriptDialog->UpdateProgress(60);
		}
	}

	// Hide dialog
	MainDialog->RunScriptDialog->HideWindow();

	// Notify user
	MainDialog->Timestamp();
	MainDialog->InsertFormattedText(YELLOW, "Uploaded script to flash memory; waiting for response...\r\n", BytesSent);

	// Close file
	CloseHandle(ScriptFile);

	// Free memory
	delete[] LastData;

	// Submit IoStream request
	StreamIoStatus(InputReport | OutputReport | ((Cronus.InputReport.ConnectedController == Controller_Ps5) ? Ps5Adt : 0));
}

void CronusZen::ApiMode(void)
{
	Communication::OutgoingPacket ApiMode(PKT_APIMODE);

	// Insert data
	for (unsigned i = 0; i < 38; i++) {
		ApiMode.InsertByte(Cronus.Output[i]);
		Cronus.Output[i] = 0; // Reset value
	}

	ApiMode.Finalize(ApiMode.Size(), 1);
}

void CronusZen::ResetDevice(void)
{
	Communication::OutgoingPacket ResetDevice(PKT_RESETDEVICE);

	ResetDevice.Finalize(0, 1);
}

void CronusZen::EnterApiMode(void)
{
	Communication::OutgoingPacket EnterApiMode(PKT_ENTERAPIMODE);

	EnterApiMode.Finalize(0, 1);
}

void CronusZen::ExitApiMode(void)
{
	Communication::OutgoingPacket ExitApiMode(PKT_EXITAPIMODE);

	ExitApiMode.Finalize(0, 1);

	if (Connection.State != Communication::Connection_Connecting) {
		// Submit IoStream request
		StreamIoStatus(InputReport | OutputReport | ((Cronus.InputReport.ConnectedController == Controller_Ps5) ? Ps5Adt : 0));
	}
}

void CronusZen::UnloadGpc(void)
{
	Communication::OutgoingPacket UnloadGpc(PKT_UNLOADGPC);

	UnloadGpc.Finalize(0, 1);
}

void CronusZen::ChangeSlotA(void)
{
	Communication::OutgoingPacket ChangeSlotA(PKT_CHANGESLOTA);

	ChangeSlotA.Finalize(0, 1);
}

void CronusZen::ChangeSlotB(void)
{
	Communication::OutgoingPacket ChangeSlotB(PKT_CHANGESLOTB);

	ChangeSlotB.Finalize(0, 1);
}


void CronusZen::TurnOffController(void)
{
	Communication::OutgoingPacket TurnOffController(PKT_TURNOFFCONTROLLER);

	TurnOffController.Finalize(0, 1);
}

void CronusZen::ClearBtCommand(void)
{
	Communication::OutgoingPacket ClearBtCommand(PKT_CLEARBTCOMMAND);

	ClearBtCommand.Finalize(0, 1);
}

void CronusZen::GetStatus(void)
{
	Communication::OutgoingPacket GetStatus(PKT_GETSTATUS);

	GetStatus.Finalize(0, 1);
}

void CronusZen::StreamIoStatus(BYTE StatusMask)
{
	Communication::OutgoingPacket StreamIoStatus(PKT_STREAMIOSTATUS);

	StreamIoStatus.InsertByte(StatusMask);
	StreamIoStatus.Finalize(1, 1);
}

void CronusZen::Cl(void)
{
	Communication::OutgoingPacket Cl(PKT_CL);

	// Insert checksum
	Cl.InsertByte(Cronus.Checksum[0]);
	Cl.InsertByte(Cronus.Checksum[1]);
	Cl.InsertByte(Cronus.Checksum[2]);
	Cl.InsertByte(Cronus.Checksum[3]);

	// Insert random data
	for (unsigned char i = 0; i < 56; i++)
		Cl.InsertByte(rand() % 255);

	Cl.Finalize(Cl.Size(), 1);
}

void CronusZen::ExclusionListRead(void)
{
	Communication::OutgoingPacket ExclusionListRead(PKT_EXCLUSIONLISTREAD);

	ExclusionListRead.Finalize(0, 1);
}

void CronusZen::DeviceCleanup(void)
{
	Communication::OutgoingPacket DeviceCleanup(PKT_DEVICECLEANUP);

	DeviceCleanup.Finalize(0, 1);
}

void CronusZen::FactoryReset(void)
{
	Communication::OutgoingPacket FactoryReset(PKT_FACTORYRESET);

	FactoryReset.Finalize(0, 1);
}

void CronusZen::RequestAttachedDevices(void)
{
	Communication::OutgoingPacket RequestAttachedDevices(PKT_REQUESTATTACHEDDEVICES);

	// Notify user
	//MainDialog->InsertFormattedText(GRAY, "Requesting list of attached devices...\r\n");

	RequestAttachedDevices.Finalize(0, 1);
}

void CronusZen::SendSingleFragment(DWORD Value)
{
	if (Connection.State > Communication::Connection_Connecting) {
		Communication::OutgoingPacket SendSingleFragment(PKT_SENDSINGLEFRAGMENT);

		SendSingleFragment.InsertLong(Value);
		SendSingleFragment.Finalize(4, 1);
	}
}

void CronusZen::RequestMkFile(void)
{
	Communication::OutgoingPacket RequestMkFile(PKT_REQUESTMKFILE);

	RequestMkFile.Finalize(0, 1);
}

void CronusZen::GetFw(void)
{
	Communication::OutgoingPacket GetFw(PKT_GETFW);

	GetFw.Finalize(0, 1);
}

void CronusZen::GetSerial(void)
{
	Communication::OutgoingPacket GetSerial(PKT_GETSERIAL);

	GetSerial.Finalize(0, 1);
}

BOOL CronusZen::FlashGamepack(void)
{
	HANDLE FileHandle = CreateFileA((LPCSTR)Cronus.Slot[Cronus.SlotToWrite].FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	unsigned char SlotToFlash = Cronus.SlotToWrite - Cronus.NumberOfErrorSlots;

	// Validate the file is able to be read
	if (FileHandle == INVALID_HANDLE_VALUE) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Unable to open %s for flashing gamepack on slot #%d! (error: %d)\r\n", Cronus.Slot[Cronus.SlotToWrite].FileName, SlotToFlash + 1, GetLastError());
		return FALSE;
	}

	DWORD FileSize = GetFileSize(FileHandle, NULL);

	// Validate the file size
	if (FileSize == INVALID_FILE_SIZE) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Unable to query file size from %s for flashing gamepack on slot #%d! (error: %d)\r\n", Cronus.Slot[Cronus.SlotToWrite].FileName, SlotToFlash + 1, GetLastError());
		CloseHandle(FileHandle);
		return FALSE;
	}

	DWORD BytesRead;
	BYTE* FileData = new BYTE[FileSize];

	if (FileData == nullptr) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Failed to allocate a read buffer when flashing gamepack on slot #%d!\r\n", SlotToFlash + 1);
		delete[] FileData;
		return FALSE;
	}

	if (!ReadFile(FileHandle, FileData, FileSize, &BytesRead, NULL)) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Unable to read file data from %s for flashing gamepack on slot #%d! (error: %d)\r\n", Cronus.Slot[Cronus.SlotToWrite].FileName, SlotToFlash + 1, GetLastError());
		CloseHandle(FileHandle);
		delete[] FileData;
		return FALSE;
	}

	// Close file
	CloseHandle(FileHandle);

	if (BytesRead != FileSize) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Unexpected number of bytes read when flashing gamepack on slot #%d (got %d bytes, expected %d bytes)!\r\n", SlotToFlash + 1, BytesRead, FileSize);
		delete[] FileData;
		return FALSE;
	}

	BYTE* LastData = new BYTE[60]{ 0 };

	if (LastData == nullptr) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Failed to allocate a buffer for last sent data when flashing gamepack on slot #%d!\r\n", SlotToFlash + 1);
		delete[] FileData;
		return FALSE;
	}

	DWORD BytesSent = 0;
	unsigned short PreviousPercentage = 0;

	MainDialog->Timestamp();
	MainDialog->InsertFormattedText(GRAY, "Attempting to write %s to slot #%d...\r\n", Cronus.Slot[Cronus.SlotToWrite].FileName, SlotToFlash + 1);
	MainDialog->StatusDialog->SetSlotBytes(Cronus.SlotToWrite + 1, (float)FileSize);

	while (BytesSent != FileSize) {
		Communication::OutgoingPacket FlashGamepack(PKT_FLASHGAMEPACK);

		if (!BytesSent) {

			// Set slot number
			FlashGamepack.InsertByte(SlotToFlash + 48);

			if (FileSize >= 59) {

				// Insert file data
				FlashGamepack.InsertData(FileData, 59);

				// Increase number of bytes sent
				BytesSent += 59;

				// Copy the last sent data which will be used to pad the final packet
				*(BYTE*)(LastData) = (SlotToFlash + 48);
				memcpy(LastData + 1, FileData, 59);

				// Update progress bars
				MainDialog->StatusDialog->UpdateProgress(59);
			}
			else {
				FlashGamepack.InsertData(FileData, FileSize);
				BytesSent += FileSize;

				// Update progress bars
				MainDialog->StatusDialog->UpdateProgress((float)FileSize);
			}
		}
		else {
			if (FileSize - BytesSent < 60) {

				// Insert the remaining file data and pad it with the last data
				FlashGamepack.InsertData(FileData + BytesSent, FileSize - BytesSent);
				FlashGamepack.InsertData(LastData + FileSize - BytesSent, 60 - (FileSize - BytesSent));
				BytesSent += (FileSize - BytesSent);

				// Update progress bars
				MainDialog->StatusDialog->UpdateProgress((float)(FileSize - BytesSent));
			}
			else {

				// Copy the last sent data which will be used to pad the final packet
				memcpy(LastData, FileData + BytesSent, 60);

				// Insert the next 60 bytes of the file data
				FlashGamepack.InsertData(FileData + BytesSent, 60);
				BytesSent += 60;

				// Update progress bars
				MainDialog->StatusDialog->UpdateProgress(60);
			}
		}

		// The packet is ready to be sent
		FlashGamepack.Finalize(FileSize + 1, (BytesSent <= 60) ? 1 : 0);
	}

	// Verify game pack was written
	Sleep(100);
	GetStatus();

	// Deallocate used memoroy
	delete[] LastData;
	delete[] FileData;


	return TRUE;
}

BOOL CronusZen::FlashConfig(void)
{
	unsigned char SlotToFlash = Cronus.SlotToWrite - Cronus.NumberOfErrorSlots;
	DWORD FileSize;

	HANDLE FileHandle = CreateFileA((LPCSTR)Cronus.Slot[Cronus.SlotToWrite].FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	// Validate the file is able to be read
	if (FileHandle == INVALID_HANDLE_VALUE) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Unable to open %s for flashing slot config on slot #%d! (error: %d)\r\n", Cronus.Slot[Cronus.SlotToWrite].FileName, SlotToFlash + 1, GetLastError());
		return FALSE;
	}

	FileSize = GetFileSize(FileHandle, NULL);

	// Validate the file size
	if (FileSize == INVALID_FILE_SIZE) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, "Unable to query file size from %s for flashing slot config on slot #%d! (error: %d)\r\n", Cronus.Slot[Cronus.SlotToWrite].FileName, SlotToFlash + 1, GetLastError());
		CloseHandle(FileHandle);
		return FALSE;
	}

	// Close the file, we just need the file size
	CloseHandle(FileHandle);

	// Notify user
	MainDialog->Timestamp();
	MainDialog->InsertFormattedText(GRAY, "Attempting to flash config for slot #%d...\r\n", SlotToFlash + 1);

	// Set file size for the pending write
	Cronus.Slot[Cronus.SlotToWrite].FileSize = FileSize;

	// Read SlotConfig file
	char SlotConfigFileName[MAX_PATH] = "";
	wsprintfA(SlotConfigFileName, "SlotConfig//%d.dat", Cronus.SlotToWrite);

	// Set SlotConfig file
	HANDLE SlotConfigFile = CreateFileA(SlotConfigFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	BOOLEAN FailedToRead = FALSE;

	if (SlotConfigFile == INVALID_HANDLE_VALUE) {

		// No config file found
		FailedToRead = TRUE;

	}
	else {

		DWORD FileSize = GetFileSize(SlotConfigFile, NULL);

		if (FileSize == INVALID_FILE_SIZE) {

			// Notify user
			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(RED, "Error querying file size for slot config file %d.dat! (error: %d)\r\n", Cronus.SlotToWrite, GetLastError());
			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(GRAY, "Sending blank configuration data...\r\n");

			FailedToRead = TRUE;

		}
		else {

			// Attempt to read the slot config
			BYTE SlotConfigData[508] = { 0 };
			DWORD BytesRead;

			if (!ReadFile(SlotConfigFile, SlotConfigData, sizeof(SlotConfigData), &BytesRead, NULL)) {

				// Notify user
				MainDialog->Timestamp();
				MainDialog->InsertFormattedText(RED, "Error reading slot config file %d.dat! (error: %d)\r\n", Cronus.SlotToWrite, GetLastError());
				MainDialog->Timestamp();
				MainDialog->InsertFormattedText(GRAY, "Sending blank configuration data...\r\n");

				FailedToRead = TRUE;

			}
			else {
				
				// Compare bytes read versus what was expected
				if (BytesRead != sizeof(SlotConfigData)) {

					// Notify user
					MainDialog->Timestamp();
					MainDialog->InsertFormattedText(RED, "Read slot config file %d.dat but the file size is a mismatch!\r\n", Cronus.SlotToWrite);
					MainDialog->Timestamp();
					MainDialog->InsertFormattedText(GRAY, "Sending blank configuration data...\r\n");

					FailedToRead = TRUE;

				}
				else {
					BYTE CopyFileName[62] = { 0 };
					BYTE ReportedFileName[62] = { 0 };

					memcpy(&ReportedFileName, SlotConfigData + 52, sizeof(ReportedFileName));
					memcpy(&CopyFileName, Cronus.Slot[Cronus.SlotToWrite].FileName, sizeof(CopyFileName));

					// Compare file names to see if we found config data for it
					if (!memcmp(CopyFileName, ReportedFileName, sizeof(CopyFileName))) {
						DWORD BytesSent = 0;
						BYTE CopyData[32] = { 0 };

						MainDialog->Timestamp();
						MainDialog->InsertFormattedText(YELLOW, "Found slot configuration data; uploading...\r\n");

						for (unsigned i = 0; i < 10; i++) {

							// Build our outgoing packet (#1)
							Communication::OutgoingPacket FlashConfig(PKT_FLASHCONFIG);

								if (i < 9) {

									// We read the data, now let us send it
									FlashConfig.InsertData(SlotConfigData + (i * 60), 60);
									FlashConfig.Finalize(508, (!i) ? 1 : 0);

									// Check for the copy data to pad the last packet
									if (i == 8) {
										memcpy(&CopyData, SlotConfigData + (i * 60) + BytesSent + 28, 32);
									}

								}
								else {

									// Send the last packet which includes the copy data for padding
									FlashConfig.InsertData(SlotConfigData + (i * 60) + BytesSent, 28);
									FlashConfig.InsertData(CopyData, 32);
									FlashConfig.Finalize(508, 0);
								}

							BytesSent += 60;
						}
					}
					else {
						FailedToRead = TRUE;

					}
				}
			}

			CloseHandle(SlotConfigFile);

		}
	}

	if (FailedToRead) {

		// Build our outgoing packet (#1)
		Communication::OutgoingPacket FlashConfig(PKT_FLASHCONFIG);

		// We must build the config data
		FlashConfig.InsertShort(0xffff);
		FlashConfig.InsertByte(0);
		FlashConfig.InsertByte(0);
		FlashConfig.InsertByte(1 + (32 | 2));
		FlashConfig.InsertByte(0);
		FlashConfig.InsertByte(0);
		FlashConfig.InsertByte(SlotToFlash + 48);

		// Insert non-sense data
		for (auto i = 8; i < 52; i++)
			FlashConfig.InsertByte((UCHAR)(i - 8));

		// Insert first 8 bytes of file name (or null values)
		FlashConfig.InsertData(Cronus.Slot[Cronus.SlotToWrite].FileName, 8);
		FlashConfig.Finalize(508, 1);

		// Build our going packet (#2)
		Communication::OutgoingPacket FlashConfig2(PKT_FLASHCONFIG);

		// Fill the rest of the data
		FlashConfig2.InsertData(Cronus.Slot[Cronus.SlotToWrite].FileName + 8, 56);
		FlashConfig2.InsertShort((SIZE_T)FileSize + 1);
		FlashConfig2.InsertShort(0);
		FlashConfig2.Finalize(508, 0);

		// Send blank SPVAR data
		for (unsigned i = 0; i < 7; i++) {
			Communication::OutgoingPacket FlashConfig3(PKT_FLASHCONFIG);
			BYTE EmptyData[60] = { 0 };
			FlashConfig3.InsertData(EmptyData, 60);
			FlashConfig3.Finalize(508, 0);
		}
	}

	/*
	FlashConfig.InsertShort(0xffff);
	FlashConfig.InsertByte(0);
	FlashConfig.InsertByte(0);
	FlashConfig.InsertByte(1 + (32 | 2));
	FlashConfig.InsertByte(0);
	FlashConfig.InsertByte(0);
	FlashConfig.InsertByte(SlotToFlash + 48);

	// Insert non-sense data
	for (auto i = 8; i < 52; i++)
		FlashConfig.InsertByte((UCHAR)(i - 8));

	// Insert first 8 bytes of file name (or null values)
	FlashConfig.InsertData(Cronus.Slot[Cronus.SlotToWrite].FileName, 8);
	FlashConfig.Finalize(508, 1);

	// Build our going packet (#2)
	Communication::OutgoingPacket FlashConfig2(PKT_FLASHCONFIG);

	// Fill the rest of the data
	FlashConfig2.InsertData(Cronus.Slot[Cronus.SlotToWrite].FileName + 8, 56);
	FlashConfig2.InsertShort((SIZE_T)FileSize + 1);
	FlashConfig2.InsertShort(0);
	FlashConfig2.Finalize(508, 0);

	// Read SlotConfig file
	char SlotConfigFileName[MAX_PATH] = "";
	wsprintfA(SlotConfigFileName, "SlotConfig//%i.dat", Cronus.SlotToWrite);

	// Set SlotConfig file
	HANDLE SlotConfigFile = CreateFileA(SlotConfigFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	BOOLEAN FailedToRead = FALSE;

	if (SlotConfigFile != INVALID_HANDLE_VALUE) {

		DWORD BytesRead;
		BYTE InputData[444] = { 0 };

		// Attempt to dump data
		if (!ReadFile(SlotConfigFile, InputData, sizeof(InputData), &BytesRead, NULL)) {
			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(RED, "Failed read slot config file for %s!\r\n", Cronus.Slot[Cronus.SlotToWrite].FileName);
			FailedToRead = TRUE;
		}
		else {
			if (BytesRead != sizeof(InputData)) {
				MainDialog->Timestamp();
				MainDialog->InsertFormattedText(RED, "Reading slot config file for %s returned bytes read mismatch!\r\n", Cronus.Slot[Cronus.SlotToWrite].FileName);
				FailedToRead = TRUE;
			}
			else {
				if (!_strnicmp((const char*)InputData, (const char*)Cronus.Slot[Cronus.SlotToWrite].FileName, 62)) {
					
					unsigned short BytesSent = 0;
					BYTE CopyData[32] = { 0 };

					for (unsigned i = 0; i < 7; i++) {
						Communication::OutgoingPacket FlashConfig3(PKT_FLASHCONFIG);

						if (i != 6) {

							// Copy trailing data for final packet
							if (i == 5) {
								memcpy(&CopyData, InputData + 62 + BytesSent + 28, 32);
							}

							if (!i) {
								FlashConfig3.InsertByte(0);
								FlashConfig3.InsertByte(0);
								FlashConfig3.InsertByte(0);
								FlashConfig3.InsertByte(0);
								FlashConfig3.InsertByte(0);
								FlashConfig3.InsertByte(0);
								FlashConfig3.InsertData(InputData + 62 + BytesSent, 54);
								FlashConfig3.Finalize(508, 0);
								BytesSent += 54;

							}
							else {
								FlashConfig3.InsertData(InputData + 62 + BytesSent, 60);
								FlashConfig3.Finalize(508, 0);
								BytesSent += 60;
							}
						}
						else {
							FlashConfig3.InsertData(InputData + 62 + BytesSent, 28);
							FlashConfig3.InsertData(CopyData, 32);
							FlashConfig3.Finalize(508, 0);
						}
					}
				}
				else {
					FailedToRead = TRUE;
				}
			}
		}

		// Close SlotConfig file
		CloseHandle(SlotConfigFile);
	}
	else {
		FailedToRead = TRUE;
	}

	if (FailedToRead) {
		// No config file, send blank data
		for (unsigned i = 0; i < 7; i++) {
			Communication::OutgoingPacket FlashConfig3(PKT_FLASHCONFIG);
			BYTE EmptyData[60] = { 0 };
			FlashConfig3.InsertData(EmptyData, 60);
			FlashConfig3.Finalize(508, 0);
		}
	}*/

	Sleep(100);
	GetStatus();

	return TRUE;
}

void CronusZen::CircleTest(BYTE x, BYTE y, WORD Speed)
{
	Communication::OutgoingPacket CircleTest(PKT_CIRCLETEST);

	CircleTest.InsertByte(x);
	CircleTest.InsertByte(y);
	CircleTest.InsertShort(Speed);
	CircleTest.Finalize(4, 1);
}

void CronusZen::FragmentRead(void)
{
	Communication::OutgoingPacket FragmentRead(PKT_FRAGMENTREAD);

	MainDialog->Timestamp();
	MainDialog->InsertFormattedText(GRAY, "Requesting device state...\r\n");

	FragmentRead.Finalize(0, 1);
}

void CronusZen::ReadSlotsCfg(void)
{
	Communication::OutgoingPacket ReadSlotsCfg(PKT_READSLOTSCFG);

	ReadSlotsCfg.Finalize(0, 1);
}

void CronusZen::SetVmCtrl(unsigned int Speed)
{
	Communication::OutgoingPacket SetVmCtrl(PKT_SETVMCTRL);

	if (Speed > 10) {
		SetVmCtrl.InsertByte(Speed - 10);
	}
	else if (Speed < 10) {
		SetVmCtrl.InsertByte(256 - (10 - Speed));
		SetVmCtrl.InsertByte(0xff);
	}
	else {
		SetVmCtrl.InsertByte(0);
	}

	SetVmCtrl.Finalize(SetVmCtrl.Size(), 1);
}

void CronusZen::ReadByteCode(unsigned char Slot)
{
	Communication::OutgoingPacket ReadByteCode(PKT_READBYTECODE);

	ReadByteCode.InsertByte(0x30 + Slot + 1);
	ReadByteCode.Finalize(ReadByteCode.Size(), 1);

}