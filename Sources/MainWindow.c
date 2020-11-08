#define GP_USE_WINAPI_H

#include "MainWindow.h"

//============================================================================
//----------------------------------------------------------------------------
//                                MainWindow.c
//----------------------------------------------------------------------------
//============================================================================


#include "AnimCursor.h"
#include "ColorUtils.h"
#include "Coordinates.h"
#include "DialogUtils.h"
#include "Environ.h"
#include "Events.h"
#include "FrameTimer.h"
#include "House.h"
#include "HouseIO.h"
#include "Input.h"
#include "Link.h"
#include "Macintosh.h"
#include "Map.h"
#include "Marquee.h"
#include "Menu.h"
#include "Music.h"
#include "ObjectEdit.h"
#include "Play.h"
#include "RectUtils.h"
#include "Render.h"
#include "ResourceIDs.h"
#include "ResourceLoader.h"
#include "Room.h"
#include "Scoreboard.h"
#include "SelectHouse.h"
#include "StringUtils.h"
#include "Tools.h"
#include "Transitions.h"
#include "Utilities.h"

#include <mmsystem.h>
#include <strsafe.h>


#define WC_MAINWINDOW  TEXT("GliderMainWindow")


void DrawOnSplash (HDC hdc, SInt16 splashHouseIndex);
void PaintMainWindow (HDC hdc);
void AdjustMainWindowDC (HDC hdc);
void HandleMainClick (HWND hwnd, Point wherePt, Boolean isDoubleClick);
void SetPaletteToGrays (RGBQUAD *colors, UINT numColors, int saturation,
	int maxSaturation);
LRESULT CALLBACK MainWindowProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void MainWindow_OnActivateApp (HWND hwnd, BOOL fActivate);


HCURSOR handCursor;
HCURSOR vertCursor;
HCURSOR horiCursor;
HCURSOR diagBotCursor;
HCURSOR diagTopCursor;
Rect workSrcRect;
HDC workSrcMap;
Rect mainWindowRect;
HWND mainWindow;
SInt16 isEditH;
SInt16 isEditV;
SInt16 playOriginH;
SInt16 playOriginV;
SInt16 splashOriginH;
SInt16 splashOriginV;
SInt16 theMode;
Boolean fadeGraysOut;
Boolean isDoColorFade;
Boolean splashDrawn;
HDC splashSrcMap;
Rect splashSrcRect;

static HCURSOR mainWindowCursor;


//==============================================================  Functions
//--------------------------------------------------------------  RegisterMainWindowClass

void RegisterMainWindowClass (void)
{
	WNDCLASSEX wcx;

	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcx.lpfnWndProc = MainWindowProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = HINST_THISCOMPONENT;
	wcx.hIcon = LoadIcon(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDI_APPL));
	wcx.hCursor = NULL;
	wcx.hbrBackground = NULL;
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = WC_MAINWINDOW;
	wcx.hIconSm = NULL;
	RegisterClassEx(&wcx);
}

//--------------------------------------------------------------  DrawOnSplash
// Draws additional text on top of splash screen.
//
// The appropriate house name to display may not be the current house, so an index
// into 'theHousesSpecs' is passed in to specify which house to display on the
// splash screen. In most cases, the value should be 'thisHouseIndex', but during
// the demo, 'thisHouseIndex' refers to the demo house and not the house that the
// player had loaded up before. The DoDemoGame() function handles this subtlety.

void DrawOnSplash (HDC hdc, SInt16 splashHouseIndex)
{
	Str255 houseLoadedStr;
	HFONT theFont;
	HGDIOBJ wasFont;
	houseSpec splashHouseSpec;

	if (splashHouseIndex < 0 || splashHouseIndex >= housesFound)
		return;

	splashHouseSpec = theHousesSpecs[splashHouseIndex];

	PasStringCopyC("House: ", houseLoadedStr);
	PasStringConcat(houseLoadedStr, splashHouseSpec.name);
	if ((thisMac.hasQT) && (splashHouseSpec.hasMovie))
		PasStringConcatC(houseLoadedStr, " (QT)");
	MoveToEx(hdc, splashOriginH + 436, splashOriginV + 314, NULL);
	theFont = CreateTahomaFont(-9, FW_BOLD);
	wasFont = SelectObject(hdc, theFont);
	if (splashHouseSpec.readOnly)
		ColorText(hdc, houseLoadedStr, 5L);
	else
		ColorText(hdc, houseLoadedStr, 28L);
	SelectObject(hdc, wasFont);
	DeleteObject(theFont);
}

//--------------------------------------------------------------  RedrawSplashScreen

void RedrawSplashScreen (SInt16 splashHouseIndex)
{
	Rect tempRect;

	Mac_PaintRect(workSrcMap, &workSrcRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
	tempRect = splashSrcRect;
	ZeroRectCorner(&tempRect);
	QOffsetRect(&tempRect, splashOriginH, splashOriginV);
	Mac_CopyBits(splashSrcMap, workSrcMap, &splashSrcRect, &tempRect, srcCopy, nil);
	DrawOnSplash(workSrcMap, splashHouseIndex);
	DissolveScreenOn(&workSrcRect);
	CopyRectMainToWork(&workSrcRect);
}

//--------------------------------------------------------------  PaintMainWindow

void PaintMainWindow (HDC hdc)
{
	RECT clientRect;
	HRGN unpaintedRgn;
	HRGN justPaintedRgn;
	Rect tempRect;

	GetClientRect(mainWindow, &clientRect);
	DPtoLP(hdc, (POINT *)&clientRect, 2);
	unpaintedRgn = CreateRectRgnIndirect(&clientRect);
	justPaintedRgn = CreateRectRgn(0, 0, 0, 0);

	if (theMode == kEditMode)
	{
		PauseMarquee();
		Mac_CopyBits(workSrcMap, hdc, &mainWindowRect, &mainWindowRect, srcCopy, nil);
		ResumeMarquee();

		SetRectRgn(
			justPaintedRgn,
			mainWindowRect.left,
			mainWindowRect.top,
			mainWindowRect.right,
			mainWindowRect.bottom
		);
		CombineRgn(unpaintedRgn, unpaintedRgn, justPaintedRgn, RGN_DIFF);
	}
	else if (theMode == kPlayMode)
	{
		Mac_CopyBits(workSrcMap, hdc, &justRoomsRect, &justRoomsRect, srcCopy, nil);
		RefreshScoreboard(kNormalTitleMode);

		SetRectRgn(
			justPaintedRgn,
			justRoomsRect.left,
			justRoomsRect.top,
			justRoomsRect.right,
			justRoomsRect.bottom
		);
		CombineRgn(unpaintedRgn, unpaintedRgn, justPaintedRgn, RGN_DIFF);
		SetRectRgn(
			justPaintedRgn,
			boardDestRect.left,
			boardDestRect.top,
			boardDestRect.right,
			boardDestRect.bottom
		);
		CombineRgn(unpaintedRgn, unpaintedRgn, justPaintedRgn, RGN_DIFF);
	}
	else if (theMode == kSplashMode)
	{
		Mac_PaintRect(workSrcMap, &workSrcRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
		tempRect = splashSrcRect;
		ZeroRectCorner(&tempRect);
		QOffsetRect(&tempRect, splashOriginH, splashOriginV);
		Mac_CopyBits(splashSrcMap, workSrcMap, &splashSrcRect, &tempRect, srcCopy, nil);
		Mac_CopyBits(workSrcMap, hdc, &workSrcRect, &workSrcRect, srcCopy, nil);
		DrawOnSplash(hdc, thisHouseIndex);

		SetRectRgn(
			justPaintedRgn,
			workSrcRect.left,
			workSrcRect.top,
			workSrcRect.right,
			workSrcRect.bottom
		);
		CombineRgn(unpaintedRgn, unpaintedRgn, justPaintedRgn, RGN_DIFF);
	}

	FillRgn(hdc, unpaintedRgn, (HBRUSH)GetStockObject(BLACK_BRUSH));
	DeleteObject(justPaintedRgn);
	DeleteObject(unpaintedRgn);

	splashDrawn = true;
}

//--------------------------------------------------------------  AdjustMainWindowDC

void AdjustMainWindowDC (HDC hdc)
{
	if (GetMenu(mainWindow) == NULL)
	{
		SetWindowOrgEx(hdc, 0, -kScoreboardTall, NULL);
	}
}

//--------------------------------------------------------------  UpdateMainWindow
// Redraws the main window (depends on mode were in - splash, editing, playing).

void UpdateMainWindow (void)
{
	UpdateWindow(mainWindow);
}

//--------------------------------------------------------------  OpenMainWindow
// Opens up the main window (how it does this depends on mode were in).

void OpenMainWindow (void)
{
	LPCWSTR titlePtr;
	int titleLen;
	WCHAR windowTitle[128];
	SInt16 whichRoom;
	RECT rcClient;
	LONG width, height;
	WINDOWPLACEMENT placement;
	DWORD windowStyle;
	Rect tempRect;

	titleLen = LoadString(HINST_THISCOMPONENT, IDS_APPLICATION_TITLE, (LPWSTR)&titlePtr, 0);
	if (titleLen <= 0)
	{
		titleLen = 0;
		titlePtr = L"";
	}
	StringCchCopyN(windowTitle, ARRAYSIZE(windowTitle), titlePtr, titleLen);

	if (mainWindow != NULL)
	{
		YellowAlert(mainWindow, kYellowUnaccounted, 6);
		return;
	}

	if (theMode == kEditMode)
	{
		QSetRect(&mainWindowRect, 0, 0, kRoomWide, kTileHigh);
		SetRect(&rcClient, 0, 0, kRoomWide, kTileHigh);
		windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		AdjustWindowRect(&rcClient, windowStyle, TRUE);
		width = rcClient.right - rcClient.left;
		height = rcClient.bottom - rcClient.top;
		mainWindow = CreateWindow(WC_MAINWINDOW, windowTitle,
				windowStyle, 0, 0, width, height,
				NULL, theMenuBar, HINST_THISCOMPONENT, NULL);
		if (mainWindow == NULL)
			RedAlert(kErrDialogDidntLoad);

		if (OptionKeyDown())
		{
			isEditH = 3;
			isEditV = 41;
		}

		placement.length = sizeof(placement);
		GetWindowPlacement(mainWindow, &placement);
		OffsetRect(&placement.rcNormalPosition,
				-placement.rcNormalPosition.left,
				-placement.rcNormalPosition.top);
		OffsetRect(&placement.rcNormalPosition, isEditH, isEditV);
		placement.showCmd = SW_SHOWNORMAL;
		SetWindowPlacement(mainWindow, &placement);

		whichRoom = GetFirstRoomNumber();
		CopyRoomToThisRoom(whichRoom);
		ReflectCurrentRoom(false);
	}
	else
	{
		rcClient.left = 0;
		rcClient.top = 0;
		rcClient.right = RectWide(&thisMac.screen);
		rcClient.bottom = RectTall(&thisMac.screen);
		windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		AdjustWindowRect(&rcClient, windowStyle, FALSE);
		width = rcClient.right - rcClient.left;
		height = rcClient.bottom - rcClient.top;
		mainWindow = CreateWindow(WC_MAINWINDOW, windowTitle,
				windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
				NULL, theMenuBar, HINST_THISCOMPONENT, NULL);
		if (mainWindow == NULL)
			RedAlert(kErrDialogDidntLoad);
		CenterWindowOverOwner(mainWindow);
		ShowWindow(mainWindow, SW_SHOWNORMAL);

		GetClientRect(mainWindow, &rcClient);
		mainWindowRect.left = 0;
		mainWindowRect.top = 0;
		mainWindowRect.right = (SInt16)rcClient.right;
		mainWindowRect.bottom = (SInt16)rcClient.bottom;

		{
			RECT clientRect;
			HDC clientHDC;

			GetClientRect(mainWindow, &clientRect);
			clientHDC = GetDC(mainWindow);
			PatBlt(clientHDC, 0, 0, clientRect.right, clientRect.bottom, BLACKNESS);
			ReleaseDC(mainWindow, clientHDC);
		}

		splashOriginH = (RectWide(&workSrcRect) - RectWide(&splashSrcRect)) / 2;
		if (splashOriginH < 0)
			splashOriginH = 0;
		splashOriginV = (RectTall(&workSrcRect) - RectTall(&splashSrcRect)) / 2;
		if (splashOriginV < 0)
			splashOriginV = 0;

		Mac_PaintRect(workSrcMap, &workSrcRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
		tempRect = splashSrcRect;
		ZeroRectCorner(&tempRect);
		Mac_CopyBits(splashSrcMap, workSrcMap, &splashSrcRect, &tempRect, srcCopy, nil);

		if ((fadeGraysOut) && (isDoColorFade))
		{
			fadeGraysOut = false;
		}
	}
}

//--------------------------------------------------------------  CloseMainWindow
// Closes the main window.

void CloseMainWindow (void)
{
	if (mainWindow != NULL)
		DestroyWindow(mainWindow);
	mainWindow = NULL;
}

//--------------------------------------------------------------  UpdateEditWindowTitle
// Handles changing the title across the top of the main window.  Only
// relevant when editing a house (room title displayed in window title).

#ifndef COMPILEDEMO
void UpdateEditWindowTitle (void)
{
	wchar_t houseName[64];
	wchar_t roomName[32];
	wchar_t newTitle[256];

	if (mainWindow == NULL)
	{
		return;
	}

	WinFromMacString(houseName, ARRAYSIZE(houseName), thisHouseName);
	if (noRoomAtAll)
	{
		StringCchPrintf(newTitle, ARRAYSIZE(newTitle), L"%s - No rooms", houseName);
	}
	else if (houseUnlocked)
	{
		WinFromMacString(roomName, ARRAYSIZE(roomName), thisRoom->name);
		StringCchPrintf(newTitle, ARRAYSIZE(newTitle), L"%s - %s (%d, %d)",
			houseName, roomName, (int)thisRoom->floor, (int)thisRoom->suite);
	}
	else
	{
		StringCchPrintf(newTitle, ARRAYSIZE(newTitle), L"%s - House Locked", houseName);
	}
	SetWindowText(mainWindow, newTitle);
}
#endif

//--------------------------------------------------------------  HandleMainClick
// Handle a mouse click in the main window (relevant only when editing).

void HandleMainClick (HWND hwnd, Point wherePt, Boolean isDoubleClick)
{
	if ((theMode != kEditMode) || (mainWindow == NULL) || (!houseUnlocked))
		return;

	if (toolSelected == kSelectTool)
		DoSelectionClick(hwnd, wherePt, isDoubleClick);
	else
		DoNewObjectClick(hwnd, wherePt);

	if (GetKeyState(VK_SHIFT) >= 0) // if shift key up
	{
		SelectTool(kSelectTool);
	}
}

//--------------------------------------------------------------  GetMainWindowDC

HDC GetMainWindowDC (void)
{
	HDC hdc;

	if (mainWindow != NULL)
	{
		hdc = GetDC(mainWindow);
		AdjustMainWindowDC(hdc);
		return hdc;
	}
	else
	{
		return NULL;
	}
}

//--------------------------------------------------------------  ReleaseMainWindowDC

void ReleaseMainWindowDC (HDC hdc)
{
	if (hdc != NULL)
	{
		ReleaseDC(mainWindow, hdc);
	}
}

//--------------------------------------------------------------  ShowMenuBarOld
// Displays the menu bar (after having been hidden).
/*
void ShowMenuBarOld (void)
{
	Rect			theRect;
	GrafPtr			wasPort, tempPort;
	RgnHandle		worldRgn, menuBarRgn;

	if (LMGetMBarHeight() == 0)
	{
		GetPort(&wasPort);
		tempPort = (GrafPtr)NewPtrClear(sizeof(GrafPort));
		OpenPort(tempPort);
		SetPort((GrafPtr)tempPort);

		LMSetMBarHeight(thisMac.menuHigh);

		theRect = (**GetGrayRgn()).rgnBBox;
		UnionRect(&theRect, &qd.screenBits.bounds, &theRect);
		worldRgn = NewRgn();
		OpenRgn();
		FrameRoundRect(&theRect, 16, 16);
		CloseRgn(worldRgn);

		theRect = qd.screenBits.bounds;
		theRect.bottom = theRect.top + thisMac.menuHigh;
		menuBarRgn = NewRgn();
		RectRgn(menuBarRgn, &theRect);

		SectRgn(worldRgn, menuBarRgn, menuBarRgn);	//	/------------------\
		DisposeRgn(worldRgn);						//	|__________________|

		UnionRgn(tempPort->visRgn, menuBarRgn, tempPort->visRgn);
		DiffRgn(tempPort->visRgn, menuBarRgn, tempPort->visRgn);
		DisposeRgn(menuBarRgn);

		ClosePort(tempPort);
		SetPort((GrafPtr)wasPort);

		DrawMenuBar();
	}
}
*/
//--------------------------------------------------------------  HideMenuBarOld
// Hides the menu bar - completely erasing it from the screen.
/*
void HideMenuBarOld (void)
{
	Rect			theRect;
	RgnHandle		worldRgn, menuBarRgn;
	GrafPtr			wasPort, tempPort;

	if (LMGetMBarHeight() != 0)
	{
		GetPort(&wasPort);
		tempPort = (GrafPtr)NewPtrClear(sizeof(GrafPort));
		OpenPort(tempPort);
		SetPort((GrafPtr)tempPort);

		LMSetMBarHeight(0);

		theRect = (**GetGrayRgn()).rgnBBox;
		UnionRect(&theRect, &qd.screenBits.bounds, &theRect);
		worldRgn = NewRgn();
		OpenRgn();
		FrameRoundRect(&theRect, 16, 16);
		CloseRgn(worldRgn);

		theRect = qd.screenBits.bounds;
		theRect.bottom = theRect.top + thisMac.menuHigh;
		menuBarRgn = NewRgn();
		RectRgn(menuBarRgn, &theRect);

		SectRgn(worldRgn, menuBarRgn, menuBarRgn);	//	/------------------\
		DisposeRgn(worldRgn);						//	|__________________|

		UnionRgn(tempPort->visRgn, menuBarRgn, tempPort->visRgn);
		DisposeRgn(menuBarRgn);

		PaintRect(&theRect);

		ClosePort(tempPort);
		SetPort((GrafPtr)wasPort);
	}
}
*/
//--------------------------------------------------------------  SetPaletteToGrays
// Sets up a gray palette corresponding in luminance to the given color
// palette. This is to facilitate the gray->color fade when the game comes up.
// The ratio given by the 'numer' and 'denom' parameters is the satuaration
// of the resulting palette, ranging from 0 (full gray) to 1 (original color).

void SetPaletteToGrays (RGBQUAD *colors, UINT numColors, int saturation,
	int maxSaturation)
{
	// The magic wand's color is kept the same.
	const COLORREF magicWandColor = RGB(0xFF, 0xFF, 0x00);
	int grayscale, maxGrayscale;
	int red, green, blue, gray;
	COLORREF thisColor;
	UINT i;

	grayscale = maxSaturation - saturation;
	maxGrayscale = maxSaturation;

	for (i = 0; i < numColors; i++)
	{
		red = colors[i].rgbRed;
		green = colors[i].rgbGreen;
		blue = colors[i].rgbBlue;
		thisColor = RGB(red, green, blue);
		if (thisColor != magicWandColor)
		{
			gray = ((red * 3) + (green * 6) + (blue * 1)) / 10;
			red -= MulDiv(red - gray, grayscale, maxGrayscale);
			green -= MulDiv(green - gray, grayscale, maxGrayscale);
			blue -= MulDiv(blue - gray, grayscale, maxGrayscale);
		}
		colors[i].rgbRed = (BYTE)red;
		colors[i].rgbGreen = (BYTE)green;
		colors[i].rgbBlue = (BYTE)blue;
	}
}

//--------------------------------------------------------------  WashColorIn
// Slowly walks the palette from its gray luminance state to the full color
// palette.  In this way, color appears to slowly wash in.

void WashColorIn (void)
{
	const int kGray2ColorSteps = 180;
	RGBQUAD wasColors[256];
	RGBQUAD newColors[256];
	UINT numColors;
	HBITMAP splashDIB;
	HDC splashDC;
	HDC hdc;
	RECT clientRect;
	BOOL fading;
	MSG msg;
	DWORD wasFPS;
	int i;
	int c;

	DisableMenuBar();

	splashDIB = Gp_LoadBuiltInImageAsDIB(kSplash8BitPICT);
	if (splashDIB == NULL)
		RedAlert(kErrFailedGraphicLoad);

	GetClientRect(mainWindow, &clientRect);

	splashDC = CreateCompatibleDC(NULL);
	SaveDC(splashDC);
	SelectObject(splashDC, splashDIB);

	numColors = GetDIBColorTable(splashDC, 0, ARRAYSIZE(wasColors), wasColors);
	if (numColors != ARRAYSIZE(wasColors))
		RedAlert(kErrUnnaccounted);

	wasFPS = GetFrameRate();
	SetFrameRate(60);

	fading = TRUE;
	for (i = 0; fading && (i < kGray2ColorSteps); i++)
	{
		while (PeekMessageOrWaitForFrame(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				PostQuitMessage((int)msg.wParam);
				fading = FALSE;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			switch (msg.message)
			{
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			case WM_LBUTTONDOWN:
				fading = FALSE;
				break;
			}
		}

		for (c = 0; c < ARRAYSIZE(newColors); c++)
		{
			newColors[c] = wasColors[c];
		}
		SetPaletteToGrays(newColors, ARRAYSIZE(newColors), i, kGray2ColorSteps);
		SetDIBColorTable(splashDC, 0, ARRAYSIZE(newColors), newColors);

		hdc = GetMainWindowDC();
		BitBlt(hdc, splashOriginH, splashOriginV, 640, 460, splashDC, 0, 0, SRCCOPY);
		ReleaseMainWindowDC(hdc);
		ValidateRect(mainWindow, NULL);
	}

	SetFrameRate(wasFPS);
	RestoreDC(splashDC, -1);
	DeleteDC(splashDC);
	DeleteObject(splashDIB);
	EnableMenuBar();
	InvalidateRect(mainWindow, NULL, TRUE);
}

//--------------------------------------------------------------  InitMainWindowCursor

void InitMainWindowCursor (void)
{
	SetMainWindowCursor(LoadCursor(NULL, IDC_ARROW));
}

//--------------------------------------------------------------  GetMainWindowCursor

HCURSOR GetMainWindowCursor (void)
{
	return mainWindowCursor;
}

//--------------------------------------------------------------  SetMainWindowCursor

void SetMainWindowCursor (HCURSOR hCursor)
{
	mainWindowCursor = hCursor;
}

//--------------------------------------------------------------  MainWindowProc

LRESULT CALLBACK MainWindowProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ACTIVATEAPP:
		MainWindow_OnActivateApp(hwnd, (BOOL)wParam);
		return 0;

	case WM_CLOSE:
		if (theMode == kPlayMode)
		{
			DoCommandKeyQuit();
		}
		DoMenuChoice(hwnd, ID_QUIT);
		return 0;

	case WM_COMMAND:
		DoMenuChoice(hwnd, LOWORD(wParam));
		return 0;

	case WM_CREATE:
		SetMainWindowCursor(LoadCursor(NULL, IDC_ARROW));
		return 0;

	case WM_DESTROY:
		// Remove the menu bar from the window so that it isn't destroyed
		// with the window. It will be reattached to the main window if
		// it is recreated.
		SetMenu(hwnd, NULL);
		return 0;

	case WM_ENABLE:
		if (mapWindow != NULL && IsWindow(mapWindow))
		{
			EnableWindow(mapWindow, !!wParam);
		}
		if (toolsWindow != NULL && IsWindow(toolsWindow))
		{
			EnableWindow(toolsWindow, !!wParam);
		}
		if (linkWindow != NULL && IsWindow(linkWindow))
		{
			EnableWindow(linkWindow, !!wParam);
		}
		if (coordWindow != NULL && IsWindow(coordWindow))
		{
			EnableWindow(coordWindow, !!wParam);
		}
		return 0;

	case WM_KEYDOWN:
		HandleKeyEvent(hwnd, (BYTE)wParam);
		return 0;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	{
		Point wherePt;
		wherePt.h = GET_X_LPARAM(lParam);
		wherePt.v = GET_Y_LPARAM(lParam);
		if (uMsg == WM_LBUTTONDBLCLK && !ignoreDoubleClick)
		{
			HandleMainClick(hwnd, wherePt, true);
		}
		else
		{
			HandleMainClick(hwnd, wherePt, false);
		}
		ignoreDoubleClick = false;
		return 0;
	}

	case WM_MOVE:
		if (theMode == kEditMode)
		{
			WINDOWPLACEMENT placement;
			placement.length = sizeof(placement);
			GetWindowPlacement(hwnd, &placement);
			isEditH = (SInt16)placement.rcNormalPosition.left;
			isEditV = (SInt16)placement.rcNormalPosition.top;
		}
		return 0;

	case WM_SETCURSOR:
		if (LOWORD(lParam) == HTCLIENT)
		{
			SetCursor(GetMainWindowCursor());
			return TRUE;
		}
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		if (BeginPaint(hwnd, &ps))
		{
			AdjustMainWindowDC(ps.hdc);
			PaintMainWindow(ps.hdc);
			EndPaint(hwnd, &ps);
		}
		return 0;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//--------------------------------------------------------------  MainWindow_OnActivateApp

void MainWindow_OnActivateApp (HWND hwnd, BOOL fActivate)
{
	if (theMode == kPlayMode)
	{
		if (fActivate)
		{
			switchedOut = false;
			if (isPlayMusicGame && !isMusicOn)
				StartMusic();
			//HideCursor();
		}
		else
		{
			InitCursor();
			switchedOut = true;
			if (isPlayMusicGame && isMusicOn)
				StopTheMusic();
		}
	}
	else
	{
		if (fActivate)
		{
			switchedOut = false;
			InitCursor();
			if ((isPlayMusicIdle) && (theMode != kEditMode))
			{
				OSErr theErr = StartMusic();
				if (theErr != noErr)
				{
					YellowAlert(hwnd, kYellowNoMusic, theErr);
					failedMusic = true;
				}
			}
			incrementModeTime = timeGetTime() + TicksToMillis(kIdleSplashTicks);

#ifndef COMPILEDEMO
//			if (theMode == kEditMode)
//				SeeIfValidScrapAvailable(true);
#endif
		}
		else
		{
			switchedOut = true;
			InitCursor();
			if ((isMusicOn) && (theMode != kEditMode))
				StopTheMusic();
		}
	}
}
