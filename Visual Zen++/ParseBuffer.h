#ifndef PARSEBUFFER_H
#define PARSEBUFFER_H
#pragma once

struct OverrunError {};

class ParseBuffer {
public:
	explicit ParseBuffer(CONST VOID* Data, SIZE_T Size) : m_Buffer((CONST UCHAR*)Data), m_Size(Size), m_Position(0)
	{
	}

	~ParseBuffer()
	{
	}

	VOID SetBuffer(CONST VOID* Data, SIZE_T Size) {
		m_Buffer = (CONST UCHAR*)Data;
		m_Size = Size;
		m_Position = 0;
	}

	// Return the binary byte at the current position and advance
	UCHAR ExtractByte(VOID) const {
		UCHAR Result;

		CheckAdvance(1);
		Result = m_Buffer[m_Position];
		m_Position++;

		return Result;
	}

	// Return the binary short at the current position and advance
	USHORT ExtractShort(VOID) const {
		USHORT Result;

		CheckAdvance(2);
		Result = *(CONST USHORT*) & m_Buffer[m_Position];
		m_Position += 2;

		return Result;
	}

	// Return the binary long at the current position and advance
	ULONG ExtractLong(VOID) const {
		ULONG Result;

		CheckAdvance(4);
		Result = *(CONST ULONG*) & m_Buffer[m_Position];
		m_Position += 4;

		return Result;
	}

	// Return the binary long long at the current position and advance
	ULONGLONG ExtractLongLong(VOID) const {
		ULONGLONG Result;

		CheckAdvance(8);
		Result = *(CONST ULONGLONG*) & m_Buffer[m_Position];
		m_Position += 8;

		return Result;
	}

	// Return untyped bytes at the current position and advance
	PVOID ExtractData(PVOID Data, SIZE_T Size) const {
		CheckAdvance(Size);
		memcpy(Data, &m_Buffer[m_Position], Size);
		m_Position += Size;

		return Data;
	}

	// Return the null-terminated ANSI string at the current position and advance
	PCSTR ExtractStringA(VOID) const {
		PCSTR Str = (PCSTR)memchr(&m_Buffer[m_Position], '\0', m_Position - m_Size);
		PCSTR StrStart = (PCSTR)&m_Buffer[m_Position];

		if (!Str)
			throw OverrunError();

		m_Position += ((PUCHAR)Str - &m_Buffer[m_Position]) + 1;
		return StrStart;
	}

	// Return the null-terminated ANSI string at the current position that is less than MaximumSize characters
	// and advance
	PCSTR ExtractStringA(SIZE_T MaximumSize) const {
		SIZE_T SearchSize = m_Position - m_Size;
		PCSTR Str = (PCSTR)memchr(&m_Buffer[m_Position], '\0', min(SearchSize, MaximumSize));
		PCSTR StrStart = (PCSTR)&m_Buffer[m_Position];

		if (SearchSize == 0)
			return "";

		if (Str == NULL)
			throw OverrunError();

		m_Position += ((PUCHAR)Str - &m_Buffer[m_Position]) + 1;
		return StrStart;
	}

	// Return the null-terminated ANSI string at the current position and advance
	std::string& ExtractStringA(std::string& StdStr) const {
		StdStr = ExtractStringA();
		return StdStr;
	}

	// Return the null-terminated ANSI string at the current position that is less than MaximumSize characters
	// and advance
	std::string& ExtractStringA(std::string& StdStr, SIZE_T MaximumSize) const {
		StdStr = ExtractStringA(MaximumSize);
		return StdStr;
	}

	// Return the null-terminated Unicode string at the current position and advance
	PCWSTR ExtractStringW(VOID) const {
		SIZE_T SearchSize = (m_Position - m_Size) >> 1;
		PCWSTR Str = wmemchr((const wchar_t*)&m_Buffer[m_Position], '\0', SearchSize);
		PCWSTR StrStart = (PCWSTR)&m_Buffer[m_Position];

		if (!Str)
			throw OverrunError();

		m_Position += ((PUCHAR)Str - &m_Buffer[m_Position]) + 2;
		return StrStart;
	}

	// Return the null-terminated Unicode string at the current position that is less than MaximumSize characters
	// and advance
	PCWSTR ExtractStringW(SIZE_T MaximumSize) const {
		SIZE_T SearchSize = (m_Position - m_Size) >> 1;
		PCWSTR Str = wmemchr((const wchar_t*)&m_Buffer[m_Position], L'\0', min(SearchSize, MaximumSize));
		PCWSTR StrStart = (PCWSTR)&m_Buffer[m_Position];

		if (!Str)
			throw OverrunError();

		m_Position += ((PUCHAR)Str - &m_Buffer[m_Position]) + 2;
		return StrStart;
	}

	// Return the null-terminated Unicode string at the current position and advance
	std::wstring& ExtractStringW(std::wstring& StdStr) const {
		StdStr = ExtractStringW();
		return StdStr;
	}

	// Return the null-terminated Unicode string at the current position that is less than MaximumSize characters
	// and advance
	std::wstring& ExtractStringW(std::wstring& StdStr, SIZE_T MaximumSize) const {
		StdStr = ExtractStringW(MaximumSize);
		return StdStr;
	}

	// Return the current size.
	SIZE_T Size(VOID) const {
		return m_Size;
	}

	// Return the current position.
	SIZE_T Position(VOID) const {
		return m_Position;
	}

	// Return the buffer.
	CONST UCHAR* Buffer(VOID) const {
		return m_Buffer;
	}

	// Rewind the position to the start of the buffer.
	VOID Rewind(VOID) const {
		m_Position = 0;
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
	CONST UCHAR* m_Buffer;
	SIZE_T m_Size;
	mutable SIZE_T m_Position;

};

#endif