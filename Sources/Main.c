#include "Main.h"

//============================================================================
//----------------------------------------------------------------------------
//                              Glider PRO 1.0.4
//                              by  john calhoun
//----------------------------------------------------------------------------
//============================================================================


#include "AnimCursor.h"
#include "Audio.h"
#include "Coordinates.h"
#include "Environ.h"
#include "Events.h"
#include "HighScores.h"
#include "House.h"
#include "HouseIO.h"
#include "HouseLegal.h"
#include "Input.h"
#include "InterfaceInit.h"
#include "Link.h"
#include "Macintosh.h"
#include "MainWindow.h"
#include "Map.h"
#include "Marquee.h"
#include "Menu.h"
#include "Music.h"
#include "Play.h"
#include "Player.h"
#include "Prefs.h"
#include "ResourceIDs.h"
#include "ResourceLoader.h"
#include "Room.h"
#include "RoomGraphics.h"
#include "SelectHouse.h"
#include "Settings.h"
#include "Sound.h"
#include "StringUtils.h"
#include "StructuresInit.h"
#include "Tools.h"
#include "Utilities.h"
#include "WinAPI.h"


void ReadInPrefs (HWND ownerWindow);
void WriteOutPrefs (HWND ownerWindow);


SInt16		isVolume, wasVolume;
SInt16		isDepthPref, numSMWarnings;
Boolean		quitting, doZooms, quickerTransitions, isUseSecondScreen;


//==============================================================  Functions
//--------------------------------------------------------------  ReadInPrefs

// Called only once when game launches - reads in the preferences saved…
// from the last time Glider PRO was launched.  If no prefs are found,…
// it assigns default settings.

void ReadInPrefs (HWND ownerWindow)
{
	prefsInfo	thePrefs;

	if (LoadPrefs(ownerWindow, &thePrefs, kPrefsVersion))
	{
#ifdef COMPILEDEMO
		PasStringCopyC("Demo House", thisHouseName);
#else
		PasStringCopy(thePrefs.wasDefaultName, thisHouseName);
#endif
		PasStringCopy(thePrefs.wasLeftName, leftName);
		PasStringCopy(thePrefs.wasRightName, rightName);
		PasStringCopy(thePrefs.wasBattName, batteryName);
		PasStringCopy(thePrefs.wasBandName, bandName);
		PasStringCopy(thePrefs.wasHighName, highName);
		PasStringCopy(thePrefs.wasHighBanner, highBanner);
		theGlider.leftKey = thePrefs.wasLeftMap;
		theGlider.rightKey = thePrefs.wasRightMap;
		theGlider.battKey = thePrefs.wasBattMap;
		theGlider.bandKey = thePrefs.wasBandMap;
		isVolume = thePrefs.wasVolume;
		isDepthPref = thePrefs.wasDepthPref;
		isMusicOn = thePrefs.wasMusicOn;
		doZooms = thePrefs.wasZooms;
		quickerTransitions = thePrefs.wasQuickTrans;
		isDoColorFade = thePrefs.wasDoColorFade;
		isPlayMusicIdle = thePrefs.wasIdleMusic;
		isPlayMusicGame = thePrefs.wasGameMusic;
		isHouseChecks = thePrefs.wasHouseChecks;
		maxFiles = thePrefs.wasMaxFiles;
		if ((maxFiles < 12) || (maxFiles > 500))
			maxFiles = 12;
		isEditH = thePrefs.wasEditH;
		isEditV = thePrefs.wasEditV;
		isMapH = thePrefs.wasMapH;
		isMapV = thePrefs.wasMapV;
		mapRoomsWide = thePrefs.wasMapWide;
		mapRoomsHigh = thePrefs.wasMapHigh;
		isToolsH = thePrefs.wasToolsH;
		isToolsV = thePrefs.wasToolsV;
		isLinkH = thePrefs.wasLinkH;
		isLinkV = thePrefs.wasLinkV;
		isCoordH = thePrefs.wasCoordH;
		isCoordV = thePrefs.wasCoordV;
		mapLeftRoom = thePrefs.isMapLeft;
		mapTopRoom = thePrefs.isMapTop;
		wasFloor = thePrefs.wasFloor;
		wasSuite = thePrefs.wasSuite;
		numSMWarnings = thePrefs.smWarnings;
		autoRoomEdit = thePrefs.wasAutoEdit;
		isMapOpen = thePrefs.wasMapOpen;
		isToolsOpen = thePrefs.wasToolsOpen;
		isCoordOpen = thePrefs.wasCoordOpen;
		numNeighbors = thePrefs.wasNumNeighbors;
		toolMode = thePrefs.wasToolGroup;
		doAutoDemo = thePrefs.wasDoAutoDemo;
		isEscPauseKey = thePrefs.wasEscPauseKey;
		// TODO: implement support for fullscreen display and monitor selection
		isUseSecondScreen = false;
		doBackground = thePrefs.wasDoBackground;
		doPrettyMap = thePrefs.wasPrettyMap;
		doBitchDialogs = thePrefs.wasBitchDialogs;
	}
	else
	{
#ifdef COMPILEDEMO
		PasStringCopyC("Demo House", thisHouseName);
#else
		PasStringCopyC("Slumberland", thisHouseName);
#endif
		PasStringCopyC("lf arrow", leftName);
		PasStringCopyC("rt arrow", rightName);
		PasStringCopyC("dn arrow", batteryName);
		PasStringCopyC("up arrow", bandName);
		PasStringCopyC("Your Name", highName);
		PasStringCopyC("Your Message Here", highBanner);
		theGlider.leftKey = VK_LEFT;
		theGlider.rightKey = VK_RIGHT;
		theGlider.battKey = VK_DOWN;
		theGlider.bandKey = VK_UP;

		UnivGetSoundVolume(&isVolume);
		if (isVolume < 1)
			isVolume = 1;
		else if (isVolume > 3)
			isVolume = 3;

		isDepthPref = kSwitchIfNeeded;
		isSoundOn = true;
		isMusicOn = true;
		isPlayMusicIdle = true;
		isPlayMusicGame = true;
		isHouseChecks = true;
		doZooms = true;
		quickerTransitions = false;
		numNeighbors = 9;
		isDoColorFade = true;
		maxFiles = 48;
		willMaxFiles = 48;
		isEditH = 3;
		isEditV = 41;
		isMapH = 3;
//		isMapV = qd.screenBits.bounds.bottom - 100;
		isMapV = 100;
		mapRoomsWide = 15;
		mapRoomsHigh = 4;
//		isToolsH = qd.screenBits.bounds.right - 120;
		isToolsH = 100;
		isToolsV = 35;
		isLinkH = 50;
		isLinkV = 80;
//		isCoordH = qd.screenBits.bounds.right - 55;
		isCoordH = 50;
		isCoordV = 204;
		mapLeftRoom = 60;
		mapTopRoom = 50;
		wasFloor = 0;
		wasSuite = 0;
		numSMWarnings = 0;
		autoRoomEdit = true;
		isMapOpen = true;
		isToolsOpen = true;
		isCoordOpen = false;
		toolMode = kBlowerMode;
		doAutoDemo = true;
		isEscPauseKey = false;
		isUseSecondScreen = false;
		doBackground = false;
		doPrettyMap = false;
		doBitchDialogs = true;
	}

	if ((numNeighbors > 1) && (thisMac.screen.right <= 512))
		numNeighbors = 1;

	UnivGetSoundVolume(&wasVolume);
	UnivSetSoundVolume(isVolume);

	if (isVolume == 0)
		isSoundOn = false;
	else
		isSoundOn = true;
}

//--------------------------------------------------------------  WriteOutPrefs

// Called just before Glider PRO quits.  This function writes out…
// the user preferences to disk.

void WriteOutPrefs (HWND ownerWindow)
{
	prefsInfo	thePrefs;

	UnivGetSoundVolume(&isVolume);

#ifdef COMPILEDEMO
	PasStringCopyC("Demo House", thePrefs.wasDefaultName);
#else
	PasStringCopy(thisHouseName, thePrefs.wasDefaultName);
#endif
	PasStringCopy(leftName, thePrefs.wasLeftName);
	PasStringCopy(rightName, thePrefs.wasRightName);
	PasStringCopy(batteryName, thePrefs.wasBattName);
	PasStringCopy(bandName, thePrefs.wasBandName);
	PasStringCopy(highName, thePrefs.wasHighName);
	PasStringCopy(highBanner, thePrefs.wasHighBanner);
	thePrefs.wasLeftMap = theGlider.leftKey;
	thePrefs.wasRightMap = theGlider.rightKey;
	thePrefs.wasBattMap = theGlider.battKey;
	thePrefs.wasBandMap = theGlider.bandKey;
	thePrefs.wasVolume = isVolume;
	thePrefs.wasDepthPref = isDepthPref;
	thePrefs.wasMusicOn = isMusicOn;
	thePrefs.wasZooms = doZooms;
	thePrefs.wasQuickTrans = quickerTransitions;
	thePrefs.wasDoColorFade = isDoColorFade;
	thePrefs.wasIdleMusic = isPlayMusicIdle;
	thePrefs.wasGameMusic = isPlayMusicGame;
	thePrefs.wasHouseChecks = isHouseChecks;
	thePrefs.wasMaxFiles = willMaxFiles;
	thePrefs.wasEditH = isEditH;
	thePrefs.wasEditV = isEditV;
	thePrefs.wasMapH = isMapH;
	thePrefs.wasMapV = isMapV;
	thePrefs.wasMapWide = mapRoomsWide;
	thePrefs.wasMapHigh = mapRoomsHigh;
	thePrefs.wasToolsH = isToolsH;
	thePrefs.wasToolsV = isToolsV;
	thePrefs.isMapLeft = mapLeftRoom;
	thePrefs.isMapTop = mapTopRoom;
	thePrefs.wasFloor = wasFloor;
	thePrefs.wasSuite = wasSuite;
	thePrefs.wasLinkH = isLinkH;
	thePrefs.wasLinkV = isLinkV;
	thePrefs.wasCoordH = isCoordH;
	thePrefs.wasCoordV = isCoordV;
	thePrefs.smWarnings = numSMWarnings;
	thePrefs.wasAutoEdit = autoRoomEdit;
	thePrefs.wasMapOpen = isMapOpen;
	thePrefs.wasToolsOpen = isToolsOpen;
	thePrefs.wasCoordOpen = isCoordOpen;
	thePrefs.wasNumNeighbors = numNeighbors;
	thePrefs.wasToolGroup = toolMode;
	thePrefs.wasDoAutoDemo = doAutoDemo;
	thePrefs.wasEscPauseKey = isEscPauseKey;
	thePrefs.wasScreen2 = isUseSecondScreen;
	thePrefs.wasDoBackground = doBackground;
	thePrefs.wasPrettyMap = doPrettyMap;
	thePrefs.wasBitchDialogs = doBitchDialogs;

	if (!SavePrefs(ownerWindow, &thePrefs, kPrefsVersion))
		MessageBeep(MB_ICONWARNING);

	UnivSetSoundVolume(wasVolume);
}

//--------------------------------------------------------------  main
// Here is main().  The first function called when Glider PRO comes up.

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPWSTR lpCmdLine, int nShowCmd)
{
//	SInt32		theErr;
	Boolean		whoCares, audioInitialized;
	HRESULT		hr;

	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nShowCmd);

	hr = Audio_InitDevice();
	audioInitialized = SUCCEEDED(hr);
	if (FAILED(hr))
	{
		dontLoadSounds = true;
		dontLoadMusic = true;
	}

	ToolBoxInit();
	CheckOurEnvirons();
	if (FAILED(Gp_LoadBuiltInAssets()))
		RedAlert(kErrFailedResourceLoad);
	LoadCursors();
	ReadInPrefs(NULL);

	VariableInit();						SpinCursor(2);
	CheckMemorySize(NULL);
	GetExtraCursors();					SpinCursor(2);
	InitMarquee();
	CreatePointers();					SpinCursor(2);
	InitSrcRects();
	CreateOffscreens();					SpinCursor(2);
	InitSound(NULL);					SpinCursor(2);
	InitMusic(NULL);					SpinCursor(2);
	OpenMainWindow();

#ifdef COMPILEQT
	if (thisMac.hasQT)
	{
		theErr = EnterMovies();
		if (theErr != noErr)
			thisMac.hasQT = false;
	}
#endif

	BuildHouseList(mainWindow);
	if (OpenHouse(mainWindow))
		whoCares = ReadHouse(mainWindow);

	PlayPrioritySound(kBirdSound, kBirdPriority);
	DelayTicks(6);
	InitializeMenus();					InitCursor();

#if BUILD_ARCADE_VERSION
//	HideMenuBarOld();
#endif

	if (isDoColorFade)
	{
		WashColorIn();
	}

	incrementModeTime = GetTickCount() + TicksToMillis(kIdleSplashTicks);
	while (!quitting)		// this is the main loop
		HandleEvent();
/*
#if BUILD_ARCADE_VERSION
	ShowMenuBarOld();
#endif
*/
	CloseMainWindow();
	KillMusic();
	KillSound();
	if (houseOpen)
	{
		if (!CloseHouse(NULL))
		{
			Gp_UnloadHouseFile();
			houseOpen = false;
		}
	}

	// Detach all submenus from the menu bar, and then destroy all menus.
	RemoveMenu(theMenuBar, kAppleMenuID, MF_BYCOMMAND);
	RemoveMenu(theMenuBar, kGameMenuID, MF_BYCOMMAND);
	RemoveMenu(theMenuBar, kOptionsMenuID, MF_BYCOMMAND);
	RemoveMenu(theMenuBar, kHouseMenuID, MF_BYCOMMAND);
	DestroyMenu(appleMenu);
	DestroyMenu(gameMenu);
	DestroyMenu(optionsMenu);
	DestroyMenu(houseMenu);
	DestroyMenu(theMenuBar);

	WriteOutPrefs(NULL);
	Gp_UnloadBuiltInAssets();
//	theErr = LoadScrap();

	if (audioInitialized)
	{
		Audio_KillDevice();
		audioInitialized = false;
	}

	return 0;
}

