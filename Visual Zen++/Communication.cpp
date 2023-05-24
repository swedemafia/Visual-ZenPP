#include "PCH.h"

BOOL Communication::AssociateWithCompletionPort(void)
{
	if ((Connection.CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1)) == NULL)
		return FALSE;

	if (!CreateIoCompletionPort(Connection.Handle, Connection.CompletionPort, NULL, 0))
		return FALSE;

	return TRUE;
}

DWORD Communication::CompletionThreadProc(LPVOID Parameter)
{
	DWORD NumberOfBytes;
	ULONG_PTR CompletionKey;
	OVERLAPPED* Overlapped;

	while (Connection.CompletionThread != INVALID_HANDLE_VALUE) {

		if (GetQueuedCompletionStatus(Connection.CompletionPort, &NumberOfBytes, &CompletionKey, &Overlapped, INFINITE)) {

			// Check completion event
			if (NumberOfBytes || (CompletionKey == Completion_Connect))
				HandleCompletionEvent(NumberOfBytes, Overlapped);
			else
				Disconnect();

		}

	}

	return 0;
}

BOOL Communication::Connect(void)
{
	// Make sure emulator is not connected already
	if (Connection.Handle != INVALID_HANDLE_VALUE) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, _c("A connection is already established!\r\n"));
		return FALSE;
	}

	// Prompt user status
	MainDialog->Timestamp();
	MainDialog->InsertFormattedText(GRAY, _c("Connecting to %s...\r\n"), Emulator::FromUnicode(Device.Path));

	// Attempt to open connection to the device
	if ((Connection.Handle = CreateFileW(Device.Path, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL)) == INVALID_HANDLE_VALUE) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, _c("Failed to establish a connection!\r\n"));
		return FALSE;
	}

	// Attempt to associate with an I/O completion port
	if (!AssociateWithCompletionPort()) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, _c("Failed to associate with an I/O completion port!\r\n"));
		Disconnect();
		return FALSE;
	}

	// Resume completion thread
	ResumeThread(Connection.CompletionThread);

	// Post status to I/O completion port
	PostQueuedCompletionStatus(Connection.CompletionPort, 0, Completion_Connect, &Connection.Operation.OnConnect.Overlapped);

	return TRUE;
}

void Communication::Disconnect(void)
{
	// Make sure emulator is not connected already
	if (Connection.Handle == INVALID_HANDLE_VALUE) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, _c("No connection is established!\r\n"));
		return;
	}

	// Only send these if the user is disconnecting from the device
	if ((Connection.State != Connection_ResetDevice) && (Connection.State != Connection_Disconnected) && (Device.State == Emulator::Device_Found)) {
		CronusZen::StreamIoStatus(CronusZen::Off);
		CronusZen::UnloadGpc();
	}

	// Close connection and suspend thread
	CloseHandle(Connection.CompletionPort);
	CloseHandle(Connection.Handle);
	SuspendThread(Connection.CompletionThread);

	// Only notify user if the device is not going through a console-commanded reset cycle
	if (Connection.State != Connection_ResetDevice) {
		MainDialog->Timestamp();
		MainDialog->InsertFormattedText(RED, _c("Disconnected from the device!\r\n"));

		// Set connection state
		UpdateConnectionState(Connection_Disconnected);
	}

	// Reset handle value
	Connection.CompletionPort = INVALID_HANDLE_VALUE;
	Connection.Handle = INVALID_HANDLE_VALUE;

	// Free memory
	delete[] Cronus.FirmwareVersion;
	delete[] Cronus.SerialNumber;
}

BOOL Communication::HandleCompletionEvent(DWORD NumberOfBytes, OVERLAPPED* Overlapped)
{
	OverlappedEx* Operation = CONTAINING_RECORD(Overlapped, OverlappedEx, Overlapped);

	// Handle asynchronous I/O operation
	switch (Operation->Key) {
	case Completion_Connect:
		if (OnConnect(Overlapped))
			PerformNextRead();
		break;
	case Completion_Receive:
		if (OnRecvData(Overlapped, NumberOfBytes))
			PerformNextRead();
		break;
	case Completion_Send:
		OnSendData(Overlapped, NumberOfBytes);
		break;
	}

	return TRUE;
}

BOOL Communication::OnConnect(OVERLAPPED* Overlapped)
{
	PHIDP_PREPARSED_DATA PreparsedData = NULL;

	if (!HidD_GetPreparsedData(Connection.Handle, &PreparsedData)) {
		Disconnect();
		return FALSE;
	}

	HIDP_CAPS HidPCaps = { NULL };

	if (HidP_GetCaps(PreparsedData, &HidPCaps) != HIDP_STATUS_SUCCESS) {
		HidD_FreePreparsedData(PreparsedData);
		Disconnect();
		return FALSE;
	}

	// Set read/write attributes
	Connection.ReadInputLength = HidPCaps.InputReportByteLength;
	Connection.WriteOutputLength = HidPCaps.OutputReportByteLength;

	HidD_FreePreparsedData(PreparsedData);

	// Notify user of success
	MainDialog->Timestamp();
	MainDialog->InsertFormattedText(GREEN, _c("Successfully established a connection to the device!\r\n"));

	// Update state
	UpdateConnectionState(Connection_Connecting);

	// Send initial communication
	CronusZen::StreamIoStatus(CronusZen::Off);
	CronusZen::ExitApiMode();
	CronusZen::UnloadGpc();
	CronusZen::CircleTest(0, 0, 0);
	CronusZen::GetFw();

	return TRUE;
}

BOOL Communication::OnRecvData(OVERLAPPED* Overlapped, DWORD BytesReceived)
{
	if (!BytesReceived || (BytesReceived != Connection.ReadInputLength)) {
		Disconnect();
		return FALSE;
	}

	// First packet will contain the size of the entire payload, we extract that here
	if (!Connection.TotalRecvBytes) {
		Connection.Store = new StoreBuffer;
		Connection.TotalRecvBytes = *(WORD*)(Connection.RecvBuffer + 2);
	}

	// Get the payload size from the current packet
	Connection.BytesRemaining = *(WORD*)(Connection.RecvBuffer + 2);

	BYTE Byte1 = *(BYTE*)(Connection.RecvBuffer + 1);
	BYTE Byte2 = *(BYTE*)(Connection.RecvBuffer + 2);
	BYTE Byte3 = *(BYTE*)(Connection.RecvBuffer + 3);
	BYTE Byte4 = *(BYTE*)(Connection.RecvBuffer + 4);

	if (Byte1 == 1 && Byte2 == 0x2e && Byte3 == 0 && Byte4 == 1) {
		Connection.TotalRecvBytes = 60;
		Connection.BytesRemaining = 60;
	}
	else if (Byte1 == 2 && Byte2 == 0x24 && Byte3 == 0 && Byte4 == 1) {
		Connection.TotalRecvBytes = 60;
		Connection.BytesRemaining = 60;
	}
	else if ((Byte1 == 3 || Byte1 == 4 || Byte1 == 5) && (Byte2 == 12 || Byte2 == 25 || Byte2 == 20) && (Byte3 == 0) && (Byte4 == 1)) {
		Connection.TotalRecvBytes = 60;
		Connection.BytesRemaining = 60;
	}
	else if (Byte1 == 8 && Byte2 == 22 && Byte3 == 0 && Byte4 == 1) {
		Connection.TotalRecvBytes = 60;
		Connection.BytesRemaining = 60;
	}

	// Determine how to store the received data depending how it is received
	if (!Connection.Store->Size()) {

		// May need to double check this, but so far no issues
		if ((BytesReceived - 5) >= Connection.BytesRemaining) {
			Connection.Store->InsertData(Connection.RecvBuffer + 1, Connection.BytesRemaining + 4);
			Connection.BytesRemaining = 0;
		}
		else {
			Connection.Store->InsertData(Connection.RecvBuffer + 1, BytesReceived - 1);
			Connection.BytesRemaining -= (WORD)BytesReceived - 1;
		}

	}
	else {
		Connection.Store->InsertData(Connection.RecvBuffer + 5, BytesReceived - 5);
		Connection.BytesRemaining -= (WORD)BytesReceived;
	}

	// Determine if the full packet has been received
	if ((Connection.Store->Size() - 4) >= Connection.TotalRecvBytes) {

		Connection.TotalRecvBytes = 0;
		CronusZen::PreparsePacket(Connection.Store->Buffer(), (WORD)Connection.Store->Size());

		// Delete StoreBuffer as we are now done with it
		delete Connection.Store;

	}

	return TRUE;
}

BOOL Communication::OnSendData(OVERLAPPED* Overlapped, DWORD BytesSent)
{
	if (!BytesSent || (Connection.WriteOutputLength != BytesSent)) {
		Disconnect();
		return FALSE;
	}

	// Check the outgoing message queue
	if (!Connection.SendQueue.empty()) {

		// Pop front element from the queue
		Connection.SendQueue.pop();

		// Check if there are any remaining packets in the queue to send
		if (!Connection.SendQueue.empty()) {
			SendPacket(Connection.SendQueue.front());
		}
	}

	return TRUE;
}

void Communication::PerformNextRead(void)
{
	memset(Connection.RecvBuffer, 0, Connection.ReadInputLength);
	memset(&Connection.Operation.OnRecvData.Overlapped, 0, sizeof(Connection.Operation.OnRecvData.Overlapped));

	// Perform read request
	if (!ReadFile(Connection.Handle, Connection.RecvBuffer, Connection.ReadInputLength, NULL, &Connection.Operation.OnRecvData.Overlapped)) {

		// ERROR_IO_PENDING is success, anything else is a failure
		if (GetLastError() != ERROR_IO_PENDING) {

			MainDialog->Timestamp();
			MainDialog->InsertFormattedText(RED, _c("Error: ReadFile failed (error: %d)!\r\n"), GetLastError());

			Disconnect();
		}
	}
}

void Communication::SendPacket(BYTE* Data)
{
	memset(&Connection.Operation.OnSendData.Overlapped, 0, sizeof(Connection.Operation.OnSendData.Overlapped));

	// Perform the write request
	if (!WriteFile(Connection.Handle, Data, Connection.WriteOutputLength, NULL, &Connection.Operation.OnSendData.Overlapped)) {

		// ERROR_IO_PENDING is success, anything else is a failure
		if (GetLastError() != ERROR_IO_PENDING) {

			if (Connection.State != Connection_ResetDevice) {
				MainDialog->Timestamp();
				MainDialog->InsertFormattedText(RED, _c("Error: WriteFile failed (error: %d)!\r\n"), GetLastError());
			}

			Disconnect();
		}
	}

	// Sleep while the operation finishes (device may not be quick enough to process)
	SleepEx(1, FALSE);
}

void Communication::QueuePacket(BYTE* Data, WORD Length)
{
	BYTE QueuedMessage[65] = { 0 };

	// Copy packet contents to a buffer
	memcpy(QueuedMessage, Data, Length);

	if (Connection.SendQueue.empty()) {
		SendPacket(QueuedMessage);
		return;
	}

	// Either queue or immediate send the packet
	Connection.SendQueue.push(QueuedMessage);
}

void Communication::UpdateConnectionState(ConnectionState EventCode)
{
	// Change state
	Connection.State = EventCode;

	switch (EventCode) {
	case Connection_Disconnected:
		MainDialog->UpdateCaption("Not connected to any device");

		// Disable menus
		// 
		// Connection:
		EnableMenuItem(MainDialog->MainMenu, MENU_CONNECTION_DISCONNECT, MF_DISABLED);
		
		// Program:
		EnableMenuItem(MainDialog->MainMenu, MENU_PROGRAM_LOAD_AND_RUN, MF_DISABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_PROGRAM_PROGRAM_DEVICE, MF_DISABLED);

		// Device:
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_SERIAL_NUMBER, MF_DISABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_CLEAR_BLUETOOTH, MF_DISABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_ERASE_MEMORY_SLOTS, MF_DISABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_FACTORY_RESET, MF_DISABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_RESET, MF_DISABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_PS4SPECIALITY, MF_DISABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_REMOTE_PLAY, MF_DISABLED);
		CheckMenuItem(MainDialog->MainMenu, MENU_DEVICE_PS4SPECIALITY, MF_UNCHECKED);
		CheckMenuItem(MainDialog->MainMenu, MENU_DEVICE_REMOTE_PLAY, MF_UNCHECKED);

		// Device->Emulator Output Protocol
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_EMULATOR_AUTO, MF_DISABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_EMULATOR_NINTENDOSWITCH, MF_DISABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_EMULATOR_PLAYSTATION3, MF_DISABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_EMULATOR_PLAYSTATION45, MF_DISABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_EMULATOR_XBOX360, MF_DISABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_EMULATOR_XBOXONE, MF_DISABLED);
		// Device->Operational Mode
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_OPERATIONAL_TOURNAMENT, MF_DISABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_OPERATIONAL_STANDARD, MF_DISABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_OPERATIONAL_WHEEL, MF_DISABLED);
		// Device->Remote Slot Change
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_REMOTESLOT_DISABLED, MF_DISABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_REMOTESLOT_LEFTSTICK, MF_DISABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_REMOTESLOT_SELECT, MF_DISABLED);
		
		// Disable dialog objects
		//
		// Buttons:
		EnableWindow(MainDialog->ClearBluetoothButton, FALSE);
		EnableWindow(MainDialog->EraseMemorySlotsButton, FALSE);
		EnableWindow(MainDialog->FactoryResetButton, FALSE);
		EnableWindow(MainDialog->ResetButton, FALSE);
		
		// Combo boxes:
		EnableWindow(MainDialog->EmulatorOutputComboBox, FALSE);
		EnableWindow(MainDialog->OperationalModeComboBox, FALSE);
		EnableWindow(MainDialog->RemoteSlotChangeComboBox, FALSE);

		// Radio buttons:
		EnableWindow(MainDialog->PS4SpecialityRadio, FALSE);
		EnableWindow(MainDialog->RemotePlayRadio, FALSE);

		// Sliders:
		EnableWindow(MainDialog->DS4LightbarBrightnessSlider, FALSE);
		EnableWindow(MainDialog->VMSpeedSlider, FALSE);

		// Clear labels
		SendDlgItemMessageA(MainDialog->Handle, LABEL_VM_SPEED_VALUE, WM_SETTEXT, 0, (LPARAM)"");
		SendDlgItemMessageA(MainDialog->Handle, LABEL_DS4_BRIGHTNESS_VALUE, WM_SETTEXT, 0, (LPARAM)"");
		SendDlgItemMessageA(MainDialog->Handle, LABEL_FIRMWARE_VERSION, WM_SETTEXT, 0, (LPARAM)"");
		break;

	case Connection_Connecting:
		MainDialog->UpdateCaption("Connecting to device");
		break;

	case Connection_Connected:
		MainDialog->UpdateCaption("Connected to device");

		// Enable menus
		// 
		// Connection:
		EnableMenuItem(MainDialog->MainMenu, MENU_CONNECTION_DISCONNECT, MF_ENABLED);

		// Program:
		EnableMenuItem(MainDialog->MainMenu, MENU_PROGRAM_LOAD_AND_RUN, MF_ENABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_PROGRAM_PROGRAM_DEVICE, MF_ENABLED);

		// Device:
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_SERIAL_NUMBER, MF_ENABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_CLEAR_BLUETOOTH, MF_ENABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_ERASE_MEMORY_SLOTS, MF_ENABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_FACTORY_RESET, MF_ENABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_RESET, MF_ENABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_PS4SPECIALITY, MF_ENABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_REMOTE_PLAY, MF_ENABLED);

		// Device->Emulator Output Protocol
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_EMULATOR_AUTO, MF_ENABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_EMULATOR_NINTENDOSWITCH, MF_ENABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_EMULATOR_PLAYSTATION3, MF_ENABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_EMULATOR_PLAYSTATION45, MF_ENABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_EMULATOR_XBOX360, MF_ENABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_EMULATOR_XBOXONE, MF_ENABLED);
		// Device->Operational Mode
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_OPERATIONAL_TOURNAMENT, MF_ENABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_OPERATIONAL_STANDARD, MF_ENABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_OPERATIONAL_WHEEL, MF_ENABLED);
		// Device->Remote Slot Change
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_REMOTESLOT_DISABLED, MF_ENABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_REMOTESLOT_LEFTSTICK, MF_ENABLED);
		EnableMenuItem(MainDialog->MainMenu, MENU_DEVICE_REMOTESLOT_SELECT, MF_ENABLED);

		// Enable dialog objects
		// 
		// Buttons:
		EnableWindow(MainDialog->ClearBluetoothButton, TRUE);
		EnableWindow(MainDialog->EraseMemorySlotsButton, TRUE);
		EnableWindow(MainDialog->FactoryResetButton, TRUE);
		EnableWindow(MainDialog->ResetButton, TRUE);

		// Combo boxes:
		EnableWindow(MainDialog->EmulatorOutputComboBox, TRUE);
		EnableWindow(MainDialog->OperationalModeComboBox, TRUE);
		EnableWindow(MainDialog->RemoteSlotChangeComboBox, TRUE);

		// Radio buttons:
		EnableWindow(MainDialog->PS4SpecialityRadio, TRUE);
		EnableWindow(MainDialog->RemotePlayRadio, TRUE);

		// Sliders:
		EnableWindow(MainDialog->DS4LightbarBrightnessSlider, TRUE);
		EnableWindow(MainDialog->VMSpeedSlider, TRUE);
		break;

	case Connection_PS4Speciality:
		MainDialog->UpdateCaption("Connected to device; enabling PS4 Speciality...");
		EnableWindow(MainDialog->PS4SpecialityRadio, FALSE);
		EnableWindow(MainDialog->RemotePlayRadio, FALSE);
		break;

	case Connection_RemotePlay:
		MainDialog->UpdateCaption("Connected to device; enabling Remote Play...");
		EnableWindow(MainDialog->PS4SpecialityRadio, FALSE);
		EnableWindow(MainDialog->RemotePlayRadio, FALSE);
		break;

	}

	// Notify plugins
	//for (auto iterator = PluginList.begin(); iterator != PluginList.end(); ++iterator) {
	//	PluginAPI::LoadedPlugin Plugin = *iterator;
	//	Plugin.Information.ConnectionHook(EventCode, Plugin.Information.HookParam);
	//}
}