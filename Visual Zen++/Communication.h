#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#pragma once

namespace Communication
{
	enum CompletionKey {
		Completion_Connect,
		Completion_Disconnect,
		Completion_Send,
		Completion_Receive
	};

	enum ConnectionState {
		Connection_Disconnected,
		Connection_Connecting,
		Connection_Connected,
		Connection_ClearBtCommand,
		Connection_CycleSlot,
		Connection_DeviceCleanup,
		Connection_FactoryReset,
		Connection_FlashConfig,
		Connection_FlashGamepack,
		Connection_PS4Speciality,
		Connection_RefreshDevice,
		Connection_RemotePlay,
		Connection_ResetDevice,
		Connection_RunScript,
		Connection_UnloadGpc
	};

	struct OverlappedEx {
		OVERLAPPED Overlapped;
		CompletionKey Key;
	};

	struct Overlapped {
		OverlappedEx OnConnect;
		OverlappedEx OnSendData;
		OverlappedEx OnRecvData;
	};

	struct ConnectionInformation {
		BYTE RecvBuffer[65];

		ConnectionState State;

		DWORD ReadInputLength;
		DWORD WriteOutputLength;

		HANDLE CompletionPort;
		HANDLE CompletionThread;
		HANDLE Handle;

		Overlapped Operation;

		std::queue<BYTE*> SendQueue;
		StoreBuffer* Store;

		WORD BytesRemaining;
		WORD TotalRecvBytes;
	};

	// Communication functions:
	BOOL AssociateWithCompletionPort(void);
	DWORD CompletionThreadProc(LPVOID Parameter);
	BOOL Connect(void);
	void Disconnect(void);
	BOOL HandleCompletionEvent(DWORD NumberOfBytes, OVERLAPPED* Overlapped);
	void PerformNextRead(void);
	void SendPacket(BYTE* Data);
	void QueuePacket(BYTE* Data, WORD Length);
	void UpdateConnectionState(ConnectionState EventCode);

	// Events:
	BOOL OnConnect(OVERLAPPED* Overlapped);
	BOOL OnRecvData(OVERLAPPED* Overlapped, DWORD BytesReceived);
	BOOL OnSendData(OVERLAPPED* Overlapped, DWORD BytesSent);

	// Class objects:
	class OutgoingPacket : public StoreBuffer {
	public:

		OutgoingPacket(BYTE PacketID) {

			FinalPacket = new StoreBuffer;

			// Insert null byte followed by the Packet ID
			if (FinalPacket) {
				FinalPacket->InsertByte(0);
				FinalPacket->InsertByte(PacketID);
			}
		}

		// Finalizes an outgoing packet
		void Finalize(DWORD TotalSize, BYTE SequenceID) {

			// Insert the rest of the packet header followed by the payload
			FinalPacket->InsertShort((USHORT)TotalSize);
			FinalPacket->InsertByte(SequenceID);
			FinalPacket->InsertData(Buffer(), Size());

			// Queue the packet
			QueuePacket(FinalPacket->Buffer(), (WORD)FinalPacket->Size());

			delete FinalPacket;
		}

	private:
		StoreBuffer* FinalPacket;
	};

}

#endif