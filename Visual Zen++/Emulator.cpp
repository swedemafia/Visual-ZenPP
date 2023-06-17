#include "pch.h"

DWORD Emulator::DeviceThreadProc(LPVOID Parameter)
{
	BOOL DisplayUpdate = FALSE;

	while (Console.InputThread != INVALID_HANDLE_VALUE) {

		// Attempt to locate device
		BOOL Result = LocateDevice(_cw(L"Cronus Bridge"));

		// Check for a status change
		if (Result != Device.State) {
			UpdateDeviceState((Emulator::DeviceState)Result);
			DisplayUpdate = TRUE;
		}

		// Prompt user, if necessary
		if (DisplayUpdate) {

			// Determine which prompt to display to the user
			if (!Result) {

				if (Connection.State != Communication::Connection_ResetDevice) {
					MainDialog->Timestamp();
					MainDialog->InsertFormattedText(RED, _c("Unable to locate a compatible USB device!\r\n"));
				}

				// Clear stored device information
				memset(&Device, 0, sizeof(Device));

				if (Connection.State != Communication::Connection_Disconnected)
					Communication::Disconnect();

			}
			else {

				if (Connection.State != Communication::Connection_ResetDevice) {
					MainDialog->Timestamp();
					MainDialog->InsertFormattedText(GREEN, _c("Successfully located a compatible USB device!\r\n"));
					SleepEx(100, FALSE);
					LocateDevicePath();
					Communication::Connect();
				}
				else {
					MainDialog->Timestamp();
					MainDialog->InsertFormattedText(GREEN, _c("Successfully reset the device!\r\n"));
					SleepEx(100, FALSE);
					LocateDevicePath();
					Communication::Connect();
				}
			}

			// Disable display update
			DisplayUpdate = FALSE;
		}
	}

	return FALSE;
}

const char* Emulator::FromUnicode(LPCWSTR String)
{
	int Length = WideCharToMultiByte(CP_ACP, 0, String, -1, NULL, 0, NULL, NULL);

	// Allocate the buffer for the ANSI string
	char* AnsiString = new char[Length + 1];

	// Convert the Unicode string to ANSI
	WideCharToMultiByte(CP_ACP, 0, String, -1, AnsiString, Length, NULL, NULL);

	return AnsiString;
}

BOOL Emulator::LocateDevice(LPCWSTR DeviceName)
{
	// Query installed USB devices that are present to the system
	HDEVINFO DevInfo = SetupDiGetClassDevsW(NULL, L"USB", NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);

	if (DevInfo == INVALID_HANDLE_VALUE) {
		// TODO: Do something here
		return FALSE;
	}

	BOOL Found = FALSE;
	DWORD MemberIndex = 0;
	SP_DEVINFO_DATA DeviceInfoData;

	DeviceInfoData.cbSize = sizeof(DeviceInfoData);

	// Enumerate list of installed USB devices present to the system
	while (SetupDiEnumDeviceInfo(DevInfo, MemberIndex++, &DeviceInfoData)) {

		DWORD RequiredSize;
		DEVPROPTYPE PropertyType;
		TCHAR PropertyBuffer[256] = { NULL };

		// Query device properties
		if (!SetupDiGetDevicePropertyW(DevInfo, &DeviceInfoData, &DEVPKEY_Device_BusReportedDeviceDesc, &PropertyType, (PBYTE)PropertyBuffer, sizeof(PropertyBuffer), &RequiredSize, 0))
			continue;

		// Check if device matches
		if (!_wcsnicmp(PropertyBuffer, DeviceName, wcslen(DeviceName))) {

			// Device found
			Found = TRUE;

			// Only proceed if we do not have the device information
			if (!Device.BusReportedDescription) {

				// Allocate space to store the bus reported device description
				Device.BusReportedDescription = new TCHAR[RequiredSize]{ NULL };

				// Store bus reported device description so it can be retrieved later
				wcscpy_s(Device.BusReportedDescription, RequiredSize, PropertyBuffer);

				// Query device descriptors
				if (!SetupDiGetDeviceRegistryPropertyW(DevInfo, &DeviceInfoData, SPDRP_DEVICEDESC, NULL, (PBYTE)PropertyBuffer, sizeof(PropertyBuffer), &RequiredSize))
					break;

				// Allocate space to store the device description
				Device.Description = new TCHAR[RequiredSize]{ NULL };

				// Store device description so it can be retrieved later
				wcscpy_s(Device.Description, RequiredSize, PropertyBuffer);

				// Query device hardware IDs (product ID, vendor ID)
				if (!SetupDiGetDeviceRegistryPropertyW(DevInfo, &DeviceInfoData, SPDRP_HARDWAREID, NULL, (PBYTE)PropertyBuffer, sizeof(PropertyBuffer), &RequiredSize))
					break;

				// Allocate space to store the hardware id
				Device.HardwareID = new TCHAR[RequiredSize]{ NULL };

				// Store device description so it can be retrieved later
				wcscpy_s(Device.HardwareID, RequiredSize, PropertyBuffer);

				// Determine the Vendor and Product IDs
				TCHAR* NextToken = NULL;
				TCHAR* Token = _tcstok_s(PropertyBuffer, L"\\#&", &NextToken);

				// Loop through each portion of the hardware ID
				while (Token) {

					// Extract Vendor ID and Product IDs
					if (!_wcsnicmp(Token, L"VID_", 4))
						wcscpy_s(Device.VendorID, Token);
					else if (!_wcsnicmp(Token, L"PID_", 4))
						wcscpy_s(Device.ProductID, Token);

					// Move to next token
					Token = _tcstok_s(NULL, L"\\#&", &NextToken);
				}

				// Device only considered to be found if both the Product and Vendor IDs are available
				Found = (Device.ProductID && Device.VendorID);
			}
		}
	}

	// Destroy device info list
	SetupDiDestroyDeviceInfoList(DevInfo);

	return Found;
}

BOOL Emulator::LocateDevicePath(void)
{
	GUID Guid;

	// Prepare GUID for Human Interface Devices
	HidD_GetHidGuid(&Guid);

	// Query present Human Interface Devices
	HDEVINFO DevInfo = SetupDiGetClassDevs(&Guid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);

	// Check for success
	if (DevInfo == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD MemberIndex = 0;
	SP_DEVICE_INTERFACE_DATA InterfaceData = { NULL };
	TCHAR CompareString[26];

	// Prepare interface data
	InterfaceData.cbSize = sizeof(InterfaceData);

	// Create comparison string
	swprintf_s(CompareString, 26, TEXT("\\\\?\\hid#%s&%s"), Device.VendorID, Device.ProductID);

	// Enumerate list of installed Human Interface Devices present to the system
	while (SetupDiEnumDeviceInterfaces(DevInfo, NULL, &Guid, MemberIndex++, &InterfaceData)) {

		DWORD RequiredSize;

		// Get the size of the interface data
		if (!SetupDiGetDeviceInterfaceDetailW(DevInfo, &InterfaceData, NULL, 0, &RequiredSize, NULL)) {

			// Check for success (insufficient buffer is a success, so far)
			if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
				return FALSE;

			PSP_DEVICE_INTERFACE_DETAIL_DATA_W InterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA_W)malloc(RequiredSize);

			// Zero memory
			memset(InterfaceDetailData, 0, sizeof(InterfaceDetailData));

			if (!InterfaceDetailData)
				return FALSE;

			// Size depends on whether or not app is 32 or 64-bit
			InterfaceDetailData->cbSize = sizeof(int*) == 4 ? 6 : 8;

			// Perform the actual query
			if (!SetupDiGetDeviceInterfaceDetailW(DevInfo, &InterfaceData, InterfaceDetailData, RequiredSize, &RequiredSize, NULL)) {
				free(InterfaceDetailData);
				return FALSE;
			}

			// Check if the device path contains the Product and Vendor IDs
			if (!_tcsnicmp(InterfaceDetailData->DevicePath, CompareString, 25)) {

				// Allocate space to store the device path
				Device.Path = new TCHAR[RequiredSize]{ NULL };

				// Store device description so it can be retrieved later
				wcscpy_s(Device.Path, RequiredSize, InterfaceDetailData->DevicePath);
			}

			// Free up memory
			free(InterfaceDetailData);

		}
	}

	return Device.Path != NULL;
}

void Emulator::UpdateDeviceState(DeviceState EventCode)
{
	// Change state
	Device.State = EventCode;

	// Notify plugins
	if (Plugin.Handle) {
		Plugin.Information.DeviceHook(EventCode, Plugin.Information.HookParam);
	}

}

void WINAPI Emulator::WriteFormattedOutput(UINT Color, LPCSTR FormattedText)
{
	MainDialog->InsertFormattedText(Color, FormattedText);
}

int WINAPI Emulator::WriteOutputTimestamp(void)
{
	MainDialog->Timestamp();
	return 0;
}