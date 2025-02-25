//============================================================================
//----------------------------------------------------------------------------
//                                FileError.h
//----------------------------------------------------------------------------
//============================================================================

#ifndef FILE_ERROR_H_
#define FILE_ERROR_H_

#include "MacTypes.h"
#include "WinAPI.h"

Boolean CheckFileError (HWND ownerWindow, HRESULT resultCode, PCWSTR fileName);

#endif
