#ifndef STOREBUFFER_H
#define STOREBUFFER_H

struct OutOfMemoryError {};

class StoreBuffer {

public:

	explicit StoreBuffer(SIZE_T SizeHint = 128) : m_Size(SizeHint), m_Position(0), m_ValidSize(0) {
		m_Buffer = (PUCHAR)malloc(SizeHint);

		if (!m_Buffer)
			throw OutOfMemoryError();
	}

	~StoreBuffer() {
		free(m_Buffer);
	}

	VOID InsertByte(UCHAR Data) {
		InternalExpand(1);
		m_Buffer[m_Position] = Data;
		m_Position++;
		if (m_Position > m_ValidSize)
			m_ValidSize = m_Position;
	}

	VOID InsertShort(SIZE_T Data) {
		InternalExpand(2);
		*(PSIZE_T)&m_Buffer[m_Position] = Data;
		m_Position += 2;
		if (m_Position > m_ValidSize)
			m_ValidSize = m_Position;
	}

	VOID InsertLong(ULONG Data) {
		InternalExpand(4);
		*(PULONG)&m_Buffer[m_Position] = Data;
		m_Position += 4;
		if (m_Position > m_ValidSize)
			m_ValidSize = m_Position;
	}

	VOID InsertLongLong(ULONGLONG Data) {
		InternalExpand(8);
		*(PULONGLONG)&m_Buffer[m_Position] = Data;
		m_Position += 8;
		if (m_Position > m_ValidSize)
			m_ValidSize = m_Position;
	}

	VOID InsertData(CONST VOID* Data, SIZE_T Size) {
		InternalExpand(Size);
		memcpy(&m_Buffer[m_Position], Data, Size);
		m_Position += Size;
		if (m_Position > m_ValidSize)
			m_ValidSize = m_Position;
	}

	VOID InsertStringA(PCSTR Data) {
		SIZE_T Length = strlen(Data) + 1;
		InternalExpand(Length);
		memcpy(&m_Buffer[m_Position], Data, Length);
		m_Position += Length;
		if (m_Position > m_ValidSize)
			m_ValidSize = m_Position;
	}

	VOID InsertStringA(CONST std::string& Data) {
		SIZE_T Length = Data.size() + 1;
		InternalExpand(Length);
		memcpy(&m_Buffer[m_Position], Data.c_str(), Length);
		m_Position += Length;
		if (m_Position > m_ValidSize)
			m_ValidSize = m_Position;
	}

	VOID InsertStringW(PCWSTR Data) {
		SIZE_T Length = (wcslen(Data) << 1) + 2;
		InternalExpand(Length);
		memcpy(&m_Buffer[m_Position], Data, Length);
		m_Position += Length;
		if (m_Position > m_ValidSize)
			m_ValidSize = m_Position;
	}

	VOID InsertStringW(CONST std::wstring& Data) {
		SIZE_T Length = (Data.size() << 1) + 2;
		InternalExpand(Length);
		memcpy(&m_Buffer[m_Position], Data.c_str(), Length);
		m_Position += Length;
		if (m_Position > m_ValidSize)
			m_ValidSize = m_Position;
	}

	// Return the current size.
	SIZE_T Size(VOID) const {
		return m_ValidSize;
	}

	// Return the total allocated space.
	SIZE_T AlloctedSize(VOID) const {
		return m_Size;
	}

	// Return the current position.
	SIZE_T Position(VOID) const {
		return m_Position;
	}

	// Return the buffer pointer.
	CONST UCHAR* Buffer(VOID) const {
		return m_Buffer;
	}

	// Return the buffer pointer.
	UCHAR* Buffer(VOID) {
		return m_Buffer;
	}

	VOID Resize(SIZE_T NewSize) {
		PUCHAR NewBuffer = (PUCHAR)realloc(m_Buffer, NewSize);

		if (!NewBuffer)
			throw OutOfMemoryError();

		m_Buffer = NewBuffer;
	}

	VOID SetPosition(SIZE_T Position) {
		if (Position > m_Size)
			InternalExpand(Position - m_Size);

		m_Position = Position;

		if (m_Position > m_ValidSize)
			m_ValidSize = m_Position;
	}

	// Determine if it's safe to advance a certain number of bytes.
	VOID CheckAdvance(SIZE_T Bytes) const {
		if (m_Position + Bytes > m_Size)
			throw OverrunError();
	}

	// Determine if it's safe to advance a certain number of bytes.  Returns FALSEif an overrun would occur.
	BOOL CanAdvance(SIZE_T Bytes) {
		return m_Position + Bytes <= m_Size;
	}

	VOID Advance(SIZE_T Bytes) const {
		CheckAdvance(Bytes);
		m_Position += Bytes;
	}

private:
	VOID InternalExpand(SIZE_T RequiredBytes) {
		if (m_Position + RequiredBytes > m_Size)
			Resize((m_Size << 1) > m_Position + RequiredBytes ? m_Size << 1 : m_Position + RequiredBytes + (m_Size << 1));
	}

	PUCHAR m_Buffer;
	SIZE_T m_ValidSize, m_Size;
	mutable SIZE_T m_Position;

};


#endif