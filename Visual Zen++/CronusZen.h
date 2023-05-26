#ifndef CRONUSZEN_H
#define CRONUSZEN_H
#pragma once

namespace CronusZen
{
	enum {
		PKT_INPUTREPORT = 0x01,
		PKT_OUTPUTREPORT = 0x02,
		PKT_REQUESTIOSTATUS = 0x02,
		PKT_RUNSCRIPT = 0x03,
		PKT_APIMODE = 0x04,
		PKT_RESETDEVICE = 0x06,
		PKT_ENTERAPIMODE = 0x07,
		PKT_EXITAPIMODE = 0x08,
		PKT_PS5ADTDATA = 0x08,
		PKT_UNLOADGPC = 0x09,
		PKT_CHANGESLOTA = 0x0a,
		PKT_CHANGESLOTB = 0x0b,
		PKT_TURNOFFCONTROLLER = 0x0c,
		PKT_CLEARBTCOMMAND = 0x0d,
		PKT_GETSTATUS = 0x20,
		PKT_STREAMIOSTATUS = 0xab,
		PKT_EXCLUSIONLISTREAD = 0xb0,
		PKT_EXCLUSIONLISTWRITE = 0xb1,
		PKT_CL = 0xb2,
		PKT_TOGGLEVMRUNS = 0xb3,
		PKT_GETZENETDATA = 0xc0,
		PKT_WRITEZENETDATA = 0xc1,
		PKT_DFUPREPAREDEVICE = 0xde,
		PKT_DFUUPDATEFIRMWARE = 0xdf,
		PKT_DEVICECLEANUP = 0xe2,
		PKT_GETCONFIG = 0xe3,
		PKT_FACTORYRESET = 0xe4,
		PKT_REQUESTATTACHEDDEVICES = 0xe5,
		PKT_REQUESTMKFILE = 0xe6,
		PKT_SENDSINGLEFRAGMENT = 0xe9,
		PKT_REFRESHEEPROM = 0xee,
		PKT_GETFW = 0xf0,
		PKT_GETSERIAL = 0xf1,
		PKT_FLASHGAMEPACK = 0xf2,
		PKT_FLASHCONFIG = 0xf3,
		PKT_SENDMKFILE = 0xf4,
		PKT_SAVEMKFINALIZECONFIG = 0xf6,
		PKT_FINALIZEMKFILE = 0xf7,
		PKT_CIRCLETEST = 0xf8,
		PKT_FRAGMENTSWRITE = 0xf9,
		PKT_FRAGMENTREAD = 0xfa,
		PKT_READSLOTSCFG = 0xfb,
		PKT_SETVMCTRL = 0xfc,
		PKT_READBYTECODE = 0xfd
	};

	enum XboxOneController {
		XB1_XBOX,
		XB1_VIEW,
		XB1_MENU,
		XB1_RB,
		XB1_RT,
		XB1_RS,
		XB1_LB,
		XB1_LT,
		XB1_LS,
		XB1_RX,
		XB1_RY,
		XB1_LX,
		XB1_LY,
		XB1_UP,
		XB1_DOWN,
		XB1_LEFT,
		XB1_RIGHT,
		XB1_Y,
		XB1_B,
		XB1_A,
		XB1_X,
		XB1_SHARE,
		XB1_SYNC = 27,
		XB1_PR1,
		XB1_PR2,
		XB1_PL1,
		XB1_PL2
	};

	enum StreamIoStatusMask {
		Off,
		InputReport,
		OutputReport,
		Mouse = 4,
		Navcon = 16,
		G13 = 32,
		Debug = 64,
		Ps5Adt = 128
	};

	enum CfgState {
		Cfg_HIP,
		Cfg_ADS,
		Cfg_AUX1,
		Cfg_AUX2,
		Cfg_AUX3,
		Cfg_AUX4
	};

	enum ConsoleType {
		Console_None,
		Console_Ps3,
		Console_Xb360,
		Console_Ps4,
		Console_Xb1,
		Console_Switch,
		Console_Ps5,
		Console_Wheel = 8,
	};

	enum ControllerType {
		Controller_None,
		Controller_Ps3 = 16,
		Controller_Xb360 = 32,
		Controller_Wii = 48,
		Controller_WiiN,
		Controller_WiiPro,
		Controller_SwitchPro,
		Controller_SwitchCon,
		Controller_WiiuPro,
		Controller_Ps4 = 64,
		Controller_Ps5,
		Controller_Xb1 = 80,
		Controller_Df = 100,
		Controller_DfPro = 104,
		Controller_Dfgt = 106,
		Controller_G25 = 105,
		Controller_G27 = 107,
		Controller_G29 = 112,
		Controller_G920H = 98,
		Controller_G920X = 128,
		Controller_KEYBMOUSE = 144
	};

	enum DeviceModeType {
		DeviceMode_Wheel = 0xff001002,
		DeviceMode_Normal = 0xff011002,
		DeviceMode_Tournament = 0xff021002,
	};

	enum EmulatorOutputProtocolType {
		EmulatorOutputProtocol_Auto = 0xff001003,
		EmulatorOutputProtocol_PlayStation3 = 0xff011003,
		EmulatorOutputProtocol_Xbox360 = 0xff021003,
		EmulatorOutputProtocol_PlayStation45 = 0xff031003,
		EmulatorOutputProtocol_XboxOne = 0xff041003,
		EmulatorOutputProtocol_NintendoSwitch = 0xff051003,
		EmulatorOutputProtocol_PCMobile = 0xff021003
	};

	enum RemoteSlotChangeType {
		RemoteSlot_Disable = 0xff001004,
		RemoteSlot_PSSHARE = 0xff011004,
		RemoteSlot_PSL3 = 0xff021004
	};

	enum Ps4SpecialityType {
		Ps4Speciality_Disabled = 0xff001005,
		Ps4Speciality_Enabled = 0xff011005,
	};

	enum RemotePlayType {
		RemotePlay_Disabled = 0xff001006,
		RemotePlay_Enabled = 0xff011006,
	};

	enum RumbleValue {
		RumbleA,
		RumbleB,
		RumbleRT,
		RumbleLT
	};

	enum PS5_ADT_MODES {
		PS5_ADT_NR,
		PS5_ADT_CR,
		PS5_ADT_SR,
		PS5_ADT_EF1 = 4,
		PS5_ADT_EF2 = 32,
		PS5_ADT_OFF = 5,
		PS5_ADT_NO_RES1 = 33,
		PS5_ADT_NO_RES2 = 37,
		PS5_ADT_HAS_RES1 = 34,
		PS5_ADT_HAS_RES2 = 38
	};

	enum Ps5AdtValue {
		RightModeValue,
		RightStartValue,
		RightForce1Value,
		RightForce2Value,
		RightStrengthLowValue,
		RightStrengthMidValue,
		RightStrengthHidValue,
		RightUnknown1Value,
		RightUnknown2Value,
		RightFreqValue,
		RightUnknown3Value,
		LeftModeValue,
		LeftStartValue,
		LeftForce1Value,
		LeftForce2Value,
		LeftStrengthLowValue,
		LeftStrengthMidValue,
		LeftStrengthHighValue,
		LeftUnknown1Value,
		LeftUnknown2Value,
		LeftFreqValue,
		LeftUnknown3Value
	};

	enum TraceValue {
		TRACE_1 = 0,
		TRACE_2,
		TRACE_3,
		TRACE_4,
		TRACE_5,
		TRACE_6
	};

	enum CommunicationVersion {
		Communication_Old,
		Communication_New
	};

	static const char* DeviceModeStrings[] = { "wheel mode", "normal mode", "tournament mode" };
	static const char* RemoteSlotChangeStrings[] = { "disabled", "PS/XBOX + View/Share", "PS/XBox + L3/LS" };
	static const char* EmulatorOutputProtocolStrings[] = { "auto", "PlayStation 3", "Xbox 360", "PlayStation 4/5", "Xbox One X/S", "Nintendo Switch", "PC/Mobile" };

	// Function prototypes
	typedef void(*PacketProcessorProc)(void);

	// Structures:
	struct SlotConfig {
		BYTE FilePath[MAX_PATH];
		BYTE FileName[MAX_PATH];
		DWORD FileSize;
	};

	struct InputReportInformation {
		WORD CpuLoadValue;
		BYTE SlotValue;
		BYTE ConnectedController;
		BYTE ConnectedConsole;
		BYTE LedState[4];
		BYTE Rumble[4];
		BYTE BatteryValue;
		char Input[38];
		DWORD PressTime[38];
		DWORD ReleaseTime[38];

		BOOL IsJoypad;
		BOOL IsConsole;
		BOOL IsMouse;
		BOOL IsKeyboard;
		BOOL IsNavcon;
		BOOL IsG13;
		BOOL IsMirror;

		BYTE VmSpeedValue;
		WORD TimestampCounter;
	};

	struct OutputReportInformation {
		char Output[38];
		DWORD Trace1;
		DWORD Trace2;
		DWORD Trace3;
		WORD Trace4;
		WORD Trace5;
		WORD Trace6;
		BYTE Unknown;
		BYTE CfgState;
	};

	struct CronusZenInformation {
		BYTE Checksum[4];
		BYTE CurrentSlot;
		BYTE ReadByteCode[8];
		BYTE* FirmwareVersion;
		BYTE* SerialNumber;

		unsigned char NumberOfErrorSlots;
		unsigned char NumberOfPendingSlots;
		unsigned char SlotToWrite;
		unsigned char SlotToRead;
		unsigned char VMSpeedValue;

		CommunicationVersion CommunicationVersion;

		// Controller input/output report information
		InputReportInformation InputReport;
		OutputReportInformation OutputReport;

		// Last controller input/output report information
		InputReportInformation LastInputReport;
		OutputReportInformation LastOutputReport;

		// PlayStatation5 Adaptive Trigger information
		BYTE Ps5Adt[22];

		BYTE Ds4LightbarBrightness;
		BYTE DeviceMode;
		BYTE EmulatorOutputProtocol;
		BYTE RemoteSlotChange;
		BYTE Ps4Speciality;
		BYTE RemotePlay;

		SlotConfig Slot[10];

		HANDLE Thread;
	};

	// Variables:
	static class ParseBuffer* Parse;

	// Handlers:
	void DispatchMessageHook(BYTE PacketID, WORD PayloadSize, BYTE* Payload);
	void ParsePacket(BYTE PacketID, WORD BytesReceived, WORD ExpectedBytes, PacketProcessorProc CompletionRoutine);
	void PreparsePacket(BYTE* Data, WORD Length);
	DWORD ThreadProc(Communication::ConnectionState Parameter);

	// User functions:
	BOOL CheckBitmask(BYTE Value, BYTE ShiftBits);
	void DisplaySlotsCfg(void);
	void ReadByteCodes(void);
	void WriteFlashConfig(void);

	// Recv:
	void OnInputReport(void);
	void OnOutputReport(void);
	void OnPs5AdtReport(void);
	void OnGetStatus(void);
	void OnExclusionListRead(void);
	void OnRequestAttachedDevices(void);
	void OnRequestMkFile(void);
	void OnGetFw(void);
	void OnGetSerial(void);
	void OnFragmentRead(void);
	void OnReadSlotsCfg(void);
	void OnReadByteCode(void);

	// Send:
	void RunScript(const char* Script);
	void ResetDevice(void);
	void ExitApiMode(void);
	void UnloadGpc(void);
	void ChangeSlotA(void);
	void ChangeSlotB(void);
	void TurnOffController(void);
	void ClearBtCommand(void);
	void GetStatus(void);
	void StreamIoStatus(BYTE StatusMask);
	void ExclusionListRead(void);
	void Cl(void);
	void DeviceCleanup(void);
	void FactoryReset(void);
	void RequestAttachedDevices(void);
	void SendSingleFragment(DWORD Value);
	void RequestMkFile(void);
	void GetFw(void);
	void GetSerial(void);
	BOOL FlashGamepack(void);
	BOOL FlashConfig(void);
	void CircleTest(BYTE x, BYTE y, WORD Speed);
	void FragmentRead(void);
	void ReadSlotsCfg(void);
	void SetVmCtrl(unsigned int Speed);
	void ReadByteCode(unsigned char Slot);
}

#endif