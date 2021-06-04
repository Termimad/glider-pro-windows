//============================================================================
//----------------------------------------------------------------------------
//                               SelectHouse.h
//----------------------------------------------------------------------------
//============================================================================

#ifndef SELECT_HOUSE_H_
#define SELECT_HOUSE_H_

#include "GliderStructs.h"
#include "MacTypes.h"
#include "WinAPI.h"

typedef struct houseSpec
{
	WCHAR path[MAX_PATH];
	WCHAR houseName[33];
	Str32 name;
	int iconIndex;
	Boolean hasMovie;
} houseSpec, *houseSpecPtr;

extern houseSpecPtr g_theHousesSpecs;
extern SInt16 g_housesFound;
extern SInt16 g_thisHouseIndex;
extern SInt16 g_maxFiles;
extern SInt16 g_willMaxFiles;
extern SInt16 g_demoHouseIndex;

void DoLoadHouse (HWND ownerWindow);
BOOL GetHouseFolderPath (LPWSTR buffer, DWORD cch);
void BuildHouseList (HWND ownerWindow);
void AddExtraHouse (const houseSpec *newHouse);

#endif
