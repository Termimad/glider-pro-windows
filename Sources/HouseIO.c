//============================================================================
//----------------------------------------------------------------------------
//                                 HouseIO.c
//----------------------------------------------------------------------------
//============================================================================

#include "HouseIO.h"

#include "Banner.h"
#include "DialogUtils.h"
#include "FileError.h"
#include "HighScores.h"
#include "House.h"
#include "HouseLegal.h"
#include "Macintosh.h"
#include "Main.h"
#include "MainWindow.h"
#include "Menu.h"
#include "ObjectEdit.h"
#include "Objects.h"
#include "Play.h"
#include "ResourceIDs.h"
#include "ResourceLoader.h"
#include "Room.h"
#include "RoomGraphics.h"
#include "SelectHouse.h"
#include "StringUtils.h"

#include <strsafe.h>

#include <stdlib.h>

#define kSaveChanges			IDYES
#define kDiscardChanges			IDNO

void LoopMovie (void);
void OpenHouseMovie (void);
void CloseHouseMovie (void);

Movie g_theMovie;
Rect g_movieRect;
SInt16 g_wasHouseVersion;
Boolean g_houseOpen;
Boolean g_fileDirty;
Boolean g_gameDirty;
Gp_HouseFile *g_theHouseFile;
Boolean g_changeLockStateOfHouse;
Boolean g_saveHouseLocked;
Boolean g_houseIsReadOnly;
Boolean g_hasMovie;
Boolean g_tvInRoom;

//==============================================================  Functions
//--------------------------------------------------------------  LoopMovie

void LoopMovie (void)
{
	return;
#if 0
	Handle		theLoop;
	UserData	theUserData;
	SInt16		theCount;

	theLoop = NewHandle(sizeof(SInt32));
	(** (SInt32 **) theLoop) = 0;
	theUserData = GetMovieUserData(g_theMovie);
	theCount = CountUserDataType(theUserData, 'LOOP');
	while (theCount--)
	{
		RemoveUserData(theUserData, 'LOOP', 1);
	}
	AddUserData(theUserData, theLoop, 'LOOP');
#endif
}

//--------------------------------------------------------------  OpenHouseMovie

void OpenHouseMovie (void)
{
#ifdef COMPILEQT
	TimeBase	theTime;
	houseSpec	theSpec;
	FInfo		finderInfo;
	Handle		spaceSaver;
	OSErr		theErr;
	SInt16		movieRefNum;
	Boolean		dataRefWasChanged;

	if (g_thisMac.hasQT)
	{
		theSpec = g_theHousesSpecs[g_thisHouseIndex];
		PasStringConcat(theSpec.name, "\p.mov");

		theErr = FSpGetFInfo(&theSpec, &finderInfo);
		if (theErr != noErr)
			return;

		theErr = OpenMovieFile(&theSpec, &movieRefNum, fsCurPerm);
		if (theErr != noErr)
		{
			YellowAlert(kYellowQTMovieNotLoaded, theErr);
			return;
		}

		theErr = NewMovieFromFile(&g_theMovie, movieRefNum, nil, theSpec.name,
				newMovieActive, &dataRefWasChanged);
		if (theErr != noErr)
		{
			YellowAlert(kYellowQTMovieNotLoaded, theErr);
			theErr = CloseMovieFile(movieRefNum);
			return;
		}
		theErr = CloseMovieFile(movieRefNum);

		spaceSaver = NewHandle(307200L);
		if (spaceSaver == nil)
		{
			YellowAlert(kYellowQTMovieNotLoaded, 749);
			CloseHouseMovie();
			return;
		}

		GoToBeginningOfMovie(g_theMovie);
		theErr = LoadMovieIntoRam(g_theMovie,
				GetMovieTime(g_theMovie, 0L), GetMovieDuration(g_theMovie), 0);
		if (theErr != noErr)
		{
			YellowAlert(kYellowQTMovieNotLoaded, theErr);
			DisposeHandle(spaceSaver);
			CloseHouseMovie();
			return;
		}
		DisposeHandle(spaceSaver);

		theErr = PrerollMovie(g_theMovie, 0, 0x000F0000);
		if (theErr != noErr)
		{
			YellowAlert(kYellowQTMovieNotLoaded, theErr);
			CloseHouseMovie();
			return;
		}

		theTime = GetMovieTimeBase(g_theMovie);
		SetTimeBaseFlags(theTime, loopTimeBase);
		SetMovieMasterTimeBase(g_theMovie, theTime, nil);
		LoopMovie();

		GetMovieBox(g_theMovie, &g_movieRect);

		g_theHousesSpecs[g_thisHouseIndex].g_hasMovie = true;
		g_hasMovie = true;
	}
#endif
}

//--------------------------------------------------------------  CloseHouseMovie

void CloseHouseMovie (void)
{
#ifdef COMPILEQT
	OSErr		theErr;

	if ((g_thisMac.hasQT) && (g_hasMovie))
	{
		theErr = LoadMovieIntoRam(g_theMovie,
				GetMovieTime(g_theMovie, 0L), GetMovieDuration(g_theMovie), flushFromRam);
		DisposeMovie(g_theMovie);
	}
#endif
	g_hasMovie = false;
}

//--------------------------------------------------------------  OpenHouse
// Opens a house (whatever current selection is).  Returns true if all went well.

Boolean OpenHouse (HWND ownerWindow)
{
	if (g_houseOpen)
	{
		if (!CloseHouse(ownerWindow))
		{
			return false;
		}
	}
	if ((g_housesFound < 1) || (g_thisHouseIndex == -1))
	{
		return false;
	}

	if (COMPILEDEMO)
	{
		if (lstrcmpi(g_theHousesSpecs[g_thisHouseIndex].houseName, L"Demo House") != 0)
		{
			return false;
		}
	}

	g_theHouseFile = Gp_LoadHouseFile(g_theHousesSpecs[g_thisHouseIndex].path);
	if (g_theHouseFile == NULL)
	{
		g_houseIsReadOnly = false;
		CheckFileError(ownerWindow, HRESULT_FROM_WIN32(ERROR_OPEN_FAILED),
			g_theHousesSpecs[g_thisHouseIndex].houseName);
		return false;
	}
	g_houseIsReadOnly = Gp_HouseFileReadOnly(g_theHouseFile);
	g_theHousesSpecs[g_thisHouseIndex].readOnly = g_houseIsReadOnly;

	g_houseOpen = true;

	g_hasMovie = false;
	g_theHousesSpecs[g_thisHouseIndex].hasMovie = false;
	g_tvInRoom = false;
	g_tvWithMovieNumber = -1;
	OpenHouseMovie();

	return true;
}

//--------------------------------------------------------------  OpenSpecificHouse
// Opens the specific house passed in.

Boolean OpenSpecificHouse (PCWSTR filename, HWND ownerWindow)
{
	SInt16 i;
	Boolean itOpened;

	if ((g_housesFound < 1) || (g_thisHouseIndex == -1))
		return (false);

	itOpened = false;

	for (i = 0; i < g_housesFound; i++)
	{
		if (wcscmp(g_theHousesSpecs[i].path, filename) == 0)
		{
			g_thisHouseIndex = i;
			PasStringCopy(g_theHousesSpecs[g_thisHouseIndex].name, g_thisHouseName);
			if (OpenHouse(ownerWindow))
				itOpened = ReadHouse(ownerWindow);
			else
				itOpened = false;
			break;
		}
	}

	return (itOpened);
}

//--------------------------------------------------------------  SaveHouseAs

Boolean SaveHouseAs (void)
{
	// TEMP - fix this later -- use NavServices (see House.c)
/*
	StandardFileReply	theReply;
	FSSpec				oldHouse;
	OSErr				theErr;
	Boolean				noProblems;
	Str255				tempStr;

	noProblems = true;

	GetLocalizedString_Pascal(15, tempStr);
	StandardPutFile(tempStr, g_thisHouseName, &theReply);
	if (theReply.sfGood)
	{
		oldHouse = g_theHousesSpecs[g_thisHouseIndex];

		CloseHouseResFork();						// close this house file
		theErr = FSClose(houseRefNum);
		if (theErr != noErr)
		{
			CheckFileError(theErr, "\pPreferences");
			return(false);
		}
													// create new house file
		theErr = FSpCreate(&theReply.sfFile, 'ozm5', 'gliH', theReply.sfScript);
		if (!CheckFileError(theErr, theReply.sfFile.name))
			return (false);
		HCreateResFile(theReply.sfFile.vRefNum, theReply.sfFile.parID,
				theReply.sfFile.name);
		if (ResError() != noErr)
			YellowAlert(kYellowFailedResCreate, ResError());
		PasStringCopy(theReply.sfFile.name, g_thisHouseName);
													// open new house data fork
		theErr = FSpOpenDF(&theReply.sfFile, fsRdWrPerm, &houseRefNum);
		if (!CheckFileError(theErr, g_thisHouseName))
			return (false);

		g_houseOpen = true;

		noProblems = WriteHouse(false);				// write out house data
		if (!noProblems)
			return(false);

		BuildHouseList();
		if (OpenSpecificHouse(&theReply.sfFile))	// open new house again
		{
		}
		else
		{
			if (OpenSpecificHouse(&oldHouse))
			{
				YellowAlert(kYellowOpenedOldHouse, 0);
			}
			else
			{
				YellowAlert(kYellowLostAllHouses, 0);
				noProblems = false;
			}
		}
	}

	return (noProblems);
	*/
	return false;
}

//--------------------------------------------------------------  ReadHouse
// With a house open, this function reads in the actual bits of data
// into memory.

Boolean ReadHouse (HWND ownerWindow)
{
	uint64_t byteCount;
	SInt16 whichRoom;
	HRESULT hr;

	if (!g_houseOpen)
	{
		YellowAlert(ownerWindow, kYellowUnaccounted, 2);
		return false;
	}

	if (g_gameDirty || g_fileDirty)
	{
		if (g_houseIsReadOnly)
		{
			if (!WriteScoresToDisk(ownerWindow))
			{
				YellowAlert(ownerWindow, kYellowFailedWrite, 0);
				return false;
			}
		}
		else if (!WriteHouse(ownerWindow, false))
		{
			return false;
		}
	}

	byteCount = Gp_HouseFileDataSize(g_theHouseFile);

	if (COMPILEDEMO)
	{
		if (byteCount != 16526)
		{
			return false;
		}
	}

	free(g_thisHouse.rooms);
	ZeroMemory(&g_thisHouse, sizeof(g_thisHouse));

	hr = Gp_ReadHouseData(g_theHouseFile, &g_thisHouse);
	if (FAILED(hr))
	{
		ZeroMemory(&g_thisHouse, sizeof(g_thisHouse));
		g_thisHouse.nRooms = 0;
		g_noRoomAtAll = true;
		YellowAlert(ownerWindow, kYellowNoRooms, 0);
		return false;
	}

	if (COMPILEDEMO)
	{
		if (g_thisHouse.nRooms != 45)
		{
			return false;
		}
	}
	if ((g_thisHouse.nRooms < 1) || (byteCount == 0))
	{
		g_thisHouse.nRooms = 0;
		g_noRoomAtAll = true;
		YellowAlert(ownerWindow, kYellowNoRooms, 0);
		return false;
	}

	g_wasHouseVersion = g_thisHouse.version;
	if (g_wasHouseVersion >= kNewHouseVersion)
	{
		YellowAlert(ownerWindow, kYellowNewerVersion, 0);
		return false;
	}

	g_houseUnlocked = ((g_thisHouse.timeStamp & 0x00000001) == 0);
	if (COMPILEDEMO)
	{
		if (g_houseUnlocked)
		{
			return false;
		}
	}
	g_changeLockStateOfHouse = false;
	g_saveHouseLocked = false;

	whichRoom = g_thisHouse.firstRoom;
	if (COMPILEDEMO)
	{
		if (whichRoom != 0)
		{
			return false;
		}
	}

	g_wardBitSet = ((g_thisHouse.flags & 0x00000001) == 0x00000001);
	g_phoneBitSet = ((g_thisHouse.flags & 0x00000002) == 0x00000002);
	g_bannerStarCountOn = ((g_thisHouse.flags & 0x00000004) == 0x00000000);

	g_noRoomAtAll = (RealRoomNumberCount() == 0);
	g_thisRoomNumber = -1;
	g_previousRoom = -1;
	if (!g_noRoomAtAll)
	{
		CopyRoomToThisRoom(whichRoom);
	}

	if (g_houseIsReadOnly)
	{
		g_houseUnlocked = false;
		if (ReadScoresFromDisk(ownerWindow))
		{
		}
	}

	g_objActive = kNoObjectSelected;
	ReflectCurrentRoom(true);
	g_gameDirty = false;
	g_fileDirty = false;
	UpdateMenus(false);

	return true;
}

//--------------------------------------------------------------  WriteHouse
// This function writes out the house data to disk.

Boolean WriteHouse (HWND ownerWindow, Boolean checkIt)
{
	UInt32 timeStamp;
	HRESULT hr;

	if (!g_houseOpen)
	{
		YellowAlert(ownerWindow, kYellowUnaccounted, 4);
		return false;
	}

	CopyThisRoomToRoom();

	if (checkIt)
	{
		CheckHouseForProblems();
	}

	if (g_fileDirty)
	{
		timeStamp = Mac_GetDateTime();
		timeStamp &= 0x7FFFFFFF;

		if (g_changeLockStateOfHouse)
		{
			g_houseUnlocked = !g_saveHouseLocked;
		}

		if (g_houseUnlocked)
		{
			timeStamp &= 0x7FFFFFFE;
		}
		else
		{
			timeStamp |= 0x00000001;
		}
		g_thisHouse.timeStamp = (SInt32)timeStamp;
		g_thisHouse.version = g_wasHouseVersion;
	}

	hr = Gp_WriteHouseData(g_theHouseFile, &g_thisHouse);
	if (FAILED(hr))
	{
		CheckFileError(ownerWindow, hr, g_theHousesSpecs[g_thisHouseIndex].houseName);
		return false;
	}

	if (g_changeLockStateOfHouse)
	{
		g_changeLockStateOfHouse = false;
		ReflectCurrentRoom(true);
	}

	g_gameDirty = false;
	g_fileDirty = false;
	UpdateMenus(false);
	return true;
}

//--------------------------------------------------------------  CloseHouse
// This function closes the current house that is open.

Boolean CloseHouse (HWND ownerWindow)
{
	if (!g_houseOpen)
	{
		return (true);
	}

	if (g_gameDirty)
	{
		if (g_houseIsReadOnly)
		{
			if (!WriteScoresToDisk(ownerWindow))
			{
				YellowAlert(ownerWindow, kYellowFailedWrite, 0);
			}
		}
		else if (!WriteHouse(ownerWindow, g_theMode == kEditMode))
		{
			YellowAlert(ownerWindow, kYellowFailedWrite, 0);
		}
	}
	else if (g_fileDirty)
	{
		if (!COMPILEDEMO)
		{
			if (!QuerySaveChanges(ownerWindow)) // false signifies user canceled
			{
				return(false);
			}
		}
	}

	CloseHouseMovie();
	Gp_UnloadHouseFile(g_theHouseFile);
	g_theHouseFile = NULL;

	g_houseOpen = false;

	g_gameDirty = false;
	g_fileDirty = false;
	return (true);
}

//--------------------------------------------------------------  QuerySaveChanges
// If changes were made, this function will present the user with a
// dialog asking them if they would like to save the changes.

Boolean QuerySaveChanges (HWND ownerWindow)
{
	DialogParams	params = { 0 };
	wchar_t			houseStr[64];
	SInt16			hitWhat;
	Boolean			whoCares;

	if (!g_fileDirty)
		return(true);

	WinFromMacString(houseStr, ARRAYSIZE(houseStr), g_thisHouseName);
	params.arg[0] = houseStr;
	hitWhat = Alert(kSaveChangesAlert, ownerWindow, &params);
	if (hitWhat == kSaveChanges)
	{
		if (g_wasHouseVersion < kHouseVersion)
			ConvertHouseVer1To2();
		g_wasHouseVersion = kHouseVersion;
		if (WriteHouse(ownerWindow, true))
			return (true);
		else
			return (false);
	}
	else if (hitWhat == kDiscardChanges)
	{
		g_fileDirty = false;
		if (!g_quitting)
		{
			whoCares = CloseHouse(ownerWindow);
			if (OpenHouse(ownerWindow))
				whoCares = ReadHouse(ownerWindow);
		}
		UpdateMenus(false);
		return (true);
	}
	else
		return (false);
}

//--------------------------------------------------------------  YellowAlert
// This is a dialog used to present an error code and explanation
// to the user when a non-lethal error has occurred.  Ideally, of
// course, this never is called.

void YellowAlert (HWND ownerWindow, SInt16 whichAlert, SInt16 identifier)
{
	DialogParams	params = { 0 };
	wchar_t			errStr[256];
	wchar_t			errNumStr[16];
	INT				result;
	SInt16			whoCares;

	result = LoadString(HINST_THISCOMPONENT, kYellowAlertStringBase + whichAlert,
			errStr, ARRAYSIZE(errStr));
	if (result <= 0)
		errStr[0] = L'\0';
	StringCchPrintf(errNumStr, ARRAYSIZE(errNumStr), L"%d", (int)identifier);

	params.arg[0] = errStr;
	params.arg[1] = errNumStr;
	whoCares = Alert(kYellowAlert, ownerWindow, &params);
}
