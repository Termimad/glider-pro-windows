//============================================================================
//----------------------------------------------------------------------------
//                                FileError.c
//----------------------------------------------------------------------------
//============================================================================

#include "FileError.h"

#include "DialogUtils.h"
#include "ResourceIDs.h"

#include <strsafe.h>

static LPWSTR GetHResultErrorString (HRESULT hResult);

//==============================================================  Functions
//--------------------------------------------------------------  GetHResultErrorString
// Given an HRESULT code, returns an error string. Free the string
// with a call to LocalFree().

static LPWSTR GetHResultErrorString (HRESULT hResult)
{
	DWORD formatFlags;
	LPWSTR errorString;
	DWORD formatResult;
	size_t length;

	formatFlags = 0;
	formatFlags |= FORMAT_MESSAGE_ALLOCATE_BUFFER;
	formatFlags |= FORMAT_MESSAGE_FROM_SYSTEM;
	formatFlags |= FORMAT_MESSAGE_IGNORE_INSERTS;
	errorString = NULL;
	formatResult = FormatMessageW(
		formatFlags,
		NULL,
		(DWORD)hResult,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&errorString,
		0,
		NULL
	);
	if (formatResult == 0)
	{
		errorString = (LPWSTR)LocalAlloc(LPTR, sizeof(*errorString) * 32);
		if (errorString != NULL)
		{
			StringCchPrintfW(
				errorString,
				32,
				L"Unknown error 0x%08lX",
				(unsigned long)hResult
			);
		}
		return errorString;
	}
	length = wcslen(errorString);
	// Remove trailing \r\n pair if present
	if (length >= 2)
	{
		if (errorString[length - 1] == L'\n')
		{
			errorString[length - 1] = L'\0';
			if (errorString[length - 2] == L'\r')
			{
				errorString[length - 2] = L'\0';
			}
		}
	}
	return errorString;
}

//--------------------------------------------------------------  CheckFileError
// Given a result code (returned from a previous file operation) this
// function cheks to see if the result code is an error and, if it is
// a common error for which I have a string message, I bring up an
// alert with the error message.  If it is an unusual error, I still
// bring up an alert but with "Miscellaneous file error" and the
// error ID.

#define dirFulErr       (-33)   // directory full
#define dskFulErr       (-34)   // disk full
#define ioErr           (-36)   // I/O error
#define bdNamErr        (-37)   // bad file name
#define fnOpnErr        (-38)   // file not open
#define mFulErr         (-41)   // memory full (open) or file won't fit (load)
#define tmfoErr         (-42)   // too many files open
#define wPrErr          (-44)   // diskette is write protected
#define fLckdErr        (-45)   // file is locked
#define vLckdErr        (-46)   // volume is locked
#define fBsyErr         (-47)   // file is busy (delete)
#define dupFNErr        (-48)   // duplicate filename (rename)
#define opWrErr         (-49)   // file already open with write permission
#define volOffLinErr    (-53)   // volume not on line error (was ejected)
#define permErr         (-54)   // permissions error (on file open)
#define wrPermErr       (-61)   // write permissions error

Boolean CheckFileError (HWND ownerWindow, HRESULT resultCode, PCWSTR fileName)
{
	SInt16 dummyInt;
	DialogParams params = { 0 };
	LPWSTR errMessage;
	wchar_t errNumString[32];

	if (SUCCEEDED(resultCode))  // No problems?  Then cruise
		return(true);

	/*
	switch (resultCode)
	{
		case dirFulErr:
			stringIndex = 2;
			break;
		case dskFulErr:
			stringIndex = 3;
			break;
		case ioErr:
			stringIndex = 4;
			break;
		case bdNamErr:
			stringIndex = 5;
			break;
		case fnOpnErr:
			stringIndex = 6;
			break;
		case mFulErr:
			stringIndex = 7;
			break;
		case tmfoErr:
			stringIndex = 8;
			break;
		case wPrErr:
			stringIndex = 9;
			break;
		case fLckdErr:
			stringIndex = 10;
			break;
		case vLckdErr:
			stringIndex = 11;
			break;
		case fBsyErr:
			stringIndex = 12;
			break;
		case dupFNErr:
			stringIndex = 13;
			break;
		case opWrErr:
			stringIndex = 14;
			break;
		case volOffLinErr:
			stringIndex = 15;
			break;
		case permErr:
			stringIndex = 16;
			break;
		case wrPermErr:
			stringIndex = 17;
			break;
		default:
			stringIndex = 1;
			break;
	}
	*/

	errMessage = GetHResultErrorString(resultCode);
	StringCchPrintf(errNumString, ARRAYSIZE(errNumString), L"0x%08lX", (ULONG)resultCode);

	params.arg[0] = errMessage;
	params.arg[1] = errNumString;
	params.arg[2] = fileName;
	dummyInt = Alert(rFileErrorAlert, ownerWindow, &params);
	LocalFree(errMessage);
	return(false);
}
