//============================================================================
//----------------------------------------------------------------------------
//									 Map.c
//----------------------------------------------------------------------------
//============================================================================


//#include <ControlDefinitions.h>
//#include <Resources.h>
#include "Macintosh.h"
#include "Externs.h"
#include "Environ.h"
#include "House.h"
#include "RectUtils.h"
#include "Utilities.h"


#define kMapRoomsHigh			9	// was 7
#define kMapRoomsWide			9	// was 7
#define kMapScrollBarWidth		16
#define kHScrollRef				5L
#define kVScrollRef				27L
#define kMapGroundValue			56
#define kNewRoomAlert			1004
#define kYesDoNewRoom			1
#define kThumbnailPictID		1010


void LoadGraphicPlus (SInt16, Rect *);
void RedrawMapContents (void);
void LiveHScrollAction (ControlHandle, SInt16);
void LiveVScrollAction (ControlHandle, SInt16);
Boolean QueryNewRoom (void);
void CreateNailOffscreen (void);
void KillNailOffscreen (void);

Rect			nailSrcRect, activeRoomRect, wasActiveRoomRect;
Rect			mapHScrollRect, mapVScrollRect, mapCenterRect;
Rect			mapWindowRect;
HDC				nailSrcMap;
WindowPtr		mapWindow;
ControlHandle	mapHScroll, mapVScroll;
SInt16			isMapH, isMapV, mapRoomsHigh, mapRoomsWide;
SInt16			mapLeftRoom, mapTopRoom;
Boolean			isMapOpen, doPrettyMap;

extern	Boolean		doBitchDialogs;


//==============================================================  Functions
//--------------------------------------------------------------  ThisRoomVisibleOnMap

#ifndef COMPILEDEMO
Boolean ThisRoomVisibleOnMap (void)
{
	SInt16		h, v;

	h = thisRoom->suite;
	v = kMapGroundValue - thisRoom->floor;

	if ((h < mapLeftRoom) || (v < mapTopRoom) ||
			(h >= (mapLeftRoom + mapRoomsWide)) ||
			(v >= (mapTopRoom + mapRoomsHigh)))
		return (false);
	else
		return (true);
}
#endif

//--------------------------------------------------------------  CenterMapOnRoom

#ifndef COMPILEDEMO
void CenterMapOnRoom (SInt16 h, SInt16 v)
{
	return;
#if 0
	if (mapWindow == nil)
		return;

	mapLeftRoom = h - (mapRoomsWide / 2);
	mapTopRoom = (kMapGroundValue - v) - (mapRoomsHigh / 2);

	if (mapLeftRoom < 0)
		mapLeftRoom = 0;
	else if (mapLeftRoom > (kMaxNumRoomsH - mapRoomsWide))
		mapLeftRoom = kMaxNumRoomsH - mapRoomsWide;

	if (mapTopRoom < 0)
		mapTopRoom = 0;
	else if (mapTopRoom > (kMaxNumRoomsV - mapRoomsHigh))
		mapTopRoom = kMaxNumRoomsV - mapRoomsHigh;

	if (mapWindow != nil)
	{
		SetControlValue(mapHScroll, mapLeftRoom);
		SetControlValue(mapVScroll, mapTopRoom);
	}
#endif
}
#endif

//--------------------------------------------------------------  FlagMapRoomsForUpdate

#ifndef COMPILEDEMO
void FlagMapRoomsForUpdate (void)
{
	return;
#if 0
	if (mapWindow == nil)
		return;

//	SetPortWindowPort(mapWindow);
	InvalWindowRect(mapWindow, &wasActiveRoomRect);
	InvalWindowRect(mapWindow, &activeRoomRect);
#endif
}
#endif

//--------------------------------------------------------------  FindNewActiveRoomRect

#ifndef COMPILEDEMO
void FindNewActiveRoomRect (void)
{
	Rect		aRoom;
	SInt16		h, i;
	SInt16		floor, suite, whoCares;
	Boolean		activeRoomVisible;

	if (mapWindow == nil)
		return;

	activeRoomVisible = false;

	for (i = 0; i < mapRoomsHigh; i++)
	{
		for (h = 0; h < mapRoomsWide; h++)
		{
			QSetRect(&aRoom, 0, 0, kMapRoomWidth, kMapRoomHeight);
			QOffsetRect(&aRoom, kMapRoomWidth * h, kMapRoomHeight * i);

			suite = h + mapLeftRoom;
			floor = kMapGroundValue - (i + mapTopRoom);
			if ((RoomExists(suite, floor, &whoCares)) && (houseUnlocked))
			{
				if (whoCares == thisRoomNumber)
				{
					wasActiveRoomRect = activeRoomRect;
					activeRoomRect = aRoom;
					activeRoomVisible = true;
				}
			}
		}
	}

	if (activeRoomVisible)
	{
		activeRoomRect.right++;
		activeRoomRect.bottom++;
		Mac_InsetRect(&activeRoomRect, -1, -1);
	}
}
#endif

//--------------------------------------------------------------  LoadGraphicPlus

void LoadGraphicPlus (SInt16 resID, Rect *theRect)
{
	return;
#if 0
	PicHandle	thePicture;

	thePicture = GetPicture(resID);
	if (thePicture == nil)
	{
		thePicture = (PicHandle)GetResource('Date', resID);
		if (thePicture == nil)
		{
			return;
		}
	}
	DrawPicture(thePicture, theRect);
	ReleaseResource((Handle)thePicture);
#endif
}

//--------------------------------------------------------------  RedrawMapContents

#ifndef COMPILEDEMO
void RedrawMapContents (void)
{
	return;
#if 0
	Rect		newClip, aRoom, src;
	RgnHandle	wasClip;
	short		h, i, groundLevel;
	short		floor, suite, whoCares, type;
	char		wasState;
	Boolean		activeRoomVisible;

	if (mapWindow == nil)
		return;

	activeRoomVisible = false;
	groundLevel = kMapGroundValue - mapTopRoom;

	newClip.left = mapWindowRect.left;
	newClip.top = mapWindowRect.top;
	newClip.right = mapWindowRect.right + 2 - kMapScrollBarWidth;
	newClip.bottom = mapWindowRect.bottom + 2 - kMapScrollBarWidth;

	SetPort((GrafPtr)mapWindow);
	wasClip = NewRgn();
	if (wasClip != nil)
	{
		GetClip(wasClip);
		ClipRect(&newClip);
	}

	wasState = HGetState((Handle)thisHouse);
	HLock((Handle)thisHouse);

	for (i = 0; i < mapRoomsHigh; i++)
	{
		for (h = 0; h < mapRoomsWide; h++)
		{
			QSetRect(&aRoom, 0, 0, kMapRoomWidth, kMapRoomHeight);
			QOffsetRect(&aRoom, kMapRoomWidth * h, kMapRoomHeight * i);

			suite = h + mapLeftRoom;
			floor = kMapGroundValue - (i + mapTopRoom);
			if ((RoomExists(suite, floor, &whoCares)) && (houseUnlocked))
			{
				PenNormal();
				type = (*thisHouse)->rooms[whoCares].background - kBaseBackgroundID;
				if (type > kNumBackgrounds)
				{
					if (!doPrettyMap)
						type = kNumBackgrounds;	// Draw "?" thumbnail.
				}
				ForeColor(blackColor);
				if (type > kNumBackgrounds)		// Do a "pretty" thumbnail.
				{
					LoadGraphicPlus(type + kBaseBackgroundID, &aRoom);
				}
				else
				{
					QSetRect(&src, 0, 0, kMapRoomWidth, kMapRoomHeight);
					QOffsetRect(&src, 0, type * kMapRoomHeight);
					CopyBits((BitMap *)*GetGWorldPixMap(nailSrcMap),
							GetPortBitMapForCopyBits(GetWindowPort(mapWindow)),
							&src, &aRoom, srcCopy, nil);
				}

				if (whoCares == thisRoomNumber)
				{
					activeRoomRect = aRoom;
					activeRoomVisible = true;
				}
			}
			else
			{
				Pattern		dummyPat;

				PenPat(GetQDGlobalsGray(&dummyPat));
				if (i >= groundLevel)
					ForeColor(greenColor);
				else
					ForeColor(blueColor);
				PaintRect(&aRoom);
			}
		}
	}

	HSetState((Handle)thisHouse, wasState);

	ForeColor(blackColor);
	PenNormal();

	for (i = 1; i < mapRoomsWide; i++)
	{
		MoveTo(i * kMapRoomWidth, 0);
		Line(0, mapRoomsHigh * kMapRoomHeight);
	}

	for (i = 1; i < mapRoomsHigh; i++)
	{
		MoveTo(0, i * kMapRoomHeight);
		Line(mapRoomsWide * kMapRoomWidth, 0);
	}

	if (activeRoomVisible)
	{
		ForeColor(redColor);
		activeRoomRect.right++;
		activeRoomRect.bottom++;
		FrameRect(&activeRoomRect);
		InsetRect(&activeRoomRect, 1, 1);
		FrameRect(&activeRoomRect);
		ForeColor(blackColor);
		InsetRect(&activeRoomRect, -1, -1);
	}

	if (wasClip != nil)
	{
		SetClip(wasClip);
		DisposeRgn(wasClip);
	}
#endif
}
#endif

//--------------------------------------------------------------  UpdateMapWindow

void UpdateMapWindow (void)
{
	return;
#if 0
	#ifndef COMPILEDEMO
	if (mapWindow == nil)
		return;

	SetControlValue(mapHScroll, mapLeftRoom);
	SetControlValue(mapVScroll, mapTopRoom);

	SetPortWindowPort(mapWindow);
	DrawControls(mapWindow);
	DrawGrowIcon(mapWindow);
	RedrawMapContents();
	#endif
#endif
}

//--------------------------------------------------------------  ResizeMapWindow

void ResizeMapWindow (SInt16 newH, SInt16 newV)
{
	return;
#if 0
#ifndef COMPILEDEMO
	if ((newH == 0) && (newV == 0))
		return;

	SetPortWindowPort(mapWindow);
	mapRoomsWide = newH / kMapRoomWidth;
	if (mapRoomsWide < 3)
		mapRoomsWide = 3;
	mapRoomsHigh = newV / kMapRoomHeight;
	if (mapRoomsHigh < 3)
		mapRoomsHigh = 3;
	QSetRect(&mapWindowRect, 0, 0,
			mapRoomsWide * kMapRoomWidth + kMapScrollBarWidth - 2,
			mapRoomsHigh * kMapRoomHeight + kMapScrollBarWidth - 2);
	EraseRect(&mapWindowRect);
	SizeWindow(mapWindow, mapWindowRect.right, mapWindowRect.bottom, true);

	SetControlMaximum(mapHScroll, kMaxNumRoomsH - mapRoomsWide);
	MoveControl(mapHScroll, 0, mapWindowRect.bottom - kMapScrollBarWidth + 2);
	SizeControl(mapHScroll, mapWindowRect.right - kMapScrollBarWidth + 3,
			kMapScrollBarWidth);
	mapLeftRoom = GetControlValue(mapHScroll);

	SetControlMaximum(mapVScroll, kMaxNumRoomsV - mapRoomsHigh);
	MoveControl(mapVScroll, mapWindowRect.right - kMapScrollBarWidth + 2, 0);
	SizeControl(mapVScroll, kMapScrollBarWidth,
			mapWindowRect.bottom - kMapScrollBarWidth + 3);
	mapTopRoom = GetControlValue(mapVScroll);

	InvalWindowRect(mapWindow, &mapWindowRect);
#endif
#endif
}

//--------------------------------------------------------------  OpenMapWindow

void OpenMapWindow (void)
{
	return;
#if 0
#ifndef COMPILEDEMO
	Rect		src, dest;
	Point		globalMouse;

	if (mapWindow == nil)
	{
		CreateNailOffscreen();
		QSetRect(&mapWindowRect, 0, 0,
				mapRoomsWide * kMapRoomWidth + kMapScrollBarWidth - 2,
				mapRoomsHigh * kMapRoomHeight + kMapScrollBarWidth - 2);
		mapWindow = NewCWindow(nil, &mapWindowRect,
					"\pMap", false, kWindoidGrowWDEF, kPutInFront, true, 0L);

		if (mapWindow == nil)
			RedAlert(kErrNoMemory);

//		if (OptionKeyDown())
//		{
//			isMapH = 3;
//			isMapV = qd.screenBits.bounds.bottom - 100;
//		}
		MoveWindow(mapWindow, isMapH, isMapV, true);
		globalMouse = MyGetGlobalMouse();
		QSetRect(&wasActiveRoomRect, 0, 0, 1, 1);
		QSetRect(&activeRoomRect, 0, 0, 1, 1);
		QSetRect(&src, 0, 0, 1, 1);
		QOffsetRect(&src, globalMouse.h, globalMouse.v);
		GetWindowRect(mapWindow, &dest);
		BringToFront(mapWindow);
		ShowHide(mapWindow, true);
//		FlagWindowFloating(mapWindow);	TEMP - use flaoting windows
		HiliteAllWindows();

		SetPort((GrafPtr)mapWindow);
		SetOrigin(1, 1);
		QSetRect(&mapHScrollRect, -1, mapRoomsHigh * kMapRoomHeight,
				mapRoomsWide * kMapRoomWidth + 1,
				mapRoomsHigh * kMapRoomHeight + kMapScrollBarWidth);
		QSetRect(&mapVScrollRect, mapRoomsWide * kMapRoomWidth, -1,
				mapRoomsWide * kMapRoomWidth + kMapScrollBarWidth,
				mapRoomsHigh * kMapRoomHeight + 1);
		mapHScroll = NewControl(mapWindow, &mapHScrollRect, "\p", true, mapLeftRoom,
				0, kMaxNumRoomsH - mapRoomsWide, scrollBarProc, kHScrollRef);
		if (mapHScroll == nil)
			RedAlert(kErrNoMemory);

		mapVScroll = NewControl(mapWindow, &mapVScrollRect, "\p", true, mapTopRoom,
				0, kMaxNumRoomsV - mapRoomsHigh, scrollBarProc, kVScrollRef);
		if (mapVScroll == nil)
			RedAlert(kErrNoMemory);

		QSetRect(&mapCenterRect, -16, -16, 0, 0);
		QOffsetRect(&mapCenterRect, mapWindowRect.right + 2,
				mapWindowRect.bottom + 2);

		CenterMapOnRoom(thisRoom->suite, thisRoom->floor);
	}

	UpdateMapCheckmark(true);
#endif
#endif
}

//--------------------------------------------------------------  CloseMapWindow

void CloseMapWindow (void)
{
	return;
#if 0
#ifndef COMPILEDEMO
	CloseThisWindow(&mapWindow);
	UpdateMapCheckmark(false);
#endif
#endif
}

//--------------------------------------------------------------  ToggleMapWindow

void ToggleMapWindow (void)
{
	return;
#if 0
#ifndef COMPILEDEMO
	if (mapWindow == nil)
	{
		OpenMapWindow();
		isMapOpen = true;
	}
	else
	{
		CloseMapWindow();
		isMapOpen = false;
	}
#endif
#endif
}

//--------------------------------------------------------------  LiveHScrollAction
#ifndef COMPILEDEMO

void LiveHScrollAction (ControlHandle theControl, SInt16 thePart)
{
	return;
#if 0
	short		wasValue, newValue;

	switch (thePart)
	{
		case kControlUpButtonPart:
		wasValue = GetControlValue(theControl);
		SetControlValue(theControl, wasValue - 1);
		if (GetControlValue(theControl) != wasValue)
		{
			mapLeftRoom = GetControlValue(theControl);
			RedrawMapContents();
		}
		break;

		case kControlDownButtonPart:
		wasValue = GetControlValue(theControl);
		SetControlValue(theControl, wasValue + 1);
		if (GetControlValue(theControl) != wasValue)
		{
			mapLeftRoom = GetControlValue(theControl);
			RedrawMapContents();
		}
		break;

		case kControlPageUpPart:
		wasValue = GetControlValue(theControl);
		newValue = wasValue - (mapRoomsWide / 2);
		SetControlValue(theControl, newValue);
		if (GetControlValue(theControl) != wasValue)
		{
			mapLeftRoom = GetControlValue(theControl);
			RedrawMapContents();
		}
		break;

		case kControlPageDownPart:
		wasValue = GetControlValue(theControl);
		newValue = wasValue + (mapRoomsWide / 2);
		SetControlValue(theControl, newValue);
		if (GetControlValue(theControl) != wasValue)
		{
			mapLeftRoom = GetControlValue(theControl);
			RedrawMapContents();
		}
		break;

		case kControlIndicatorPart:
		break;
	}
#endif
}
#endif

//--------------------------------------------------------------  LiveVScrollAction
#ifndef COMPILEDEMO

void LiveVScrollAction (ControlHandle theControl, SInt16 thePart)
{
	return;
#if 0
	short		wasValue, newValue;

	switch (thePart)
	{
		case kControlUpButtonPart:
		wasValue = GetControlValue(theControl);
		SetControlValue(theControl, wasValue - 1);
		if (GetControlValue(theControl) != wasValue)
		{
			mapTopRoom = GetControlValue(theControl);
			RedrawMapContents();
		}
		break;

		case kControlDownButtonPart:
		wasValue = GetControlValue(theControl);
		SetControlValue(theControl, wasValue + 1);
		if (GetControlValue(theControl) != wasValue)
		{
			mapTopRoom = GetControlValue(theControl);
			RedrawMapContents();
		}
		break;

		case kControlPageUpPart:
		wasValue = GetControlValue(theControl);
		newValue = wasValue - (mapRoomsHigh / 2);
		SetControlValue(theControl, newValue);
		if (GetControlValue(theControl) != wasValue)
		{
			mapTopRoom = GetControlValue(theControl);
			RedrawMapContents();
		}
		break;

		case kControlPageDownPart:
		wasValue = GetControlValue(theControl);
		newValue = wasValue + (mapRoomsHigh / 2);
		SetControlValue(theControl, newValue);
		if (GetControlValue(theControl) != wasValue)
		{
			mapTopRoom = GetControlValue(theControl);
			RedrawMapContents();
		}
		break;

		case kControlIndicatorPart:
		break;
	}
#endif
}
#endif

//--------------------------------------------------------------  HandleMapClick

void HandleMapClick (EventRecord *theEvent)
{
	return;
#if 0
#ifndef COMPILEDEMO
	Rect				aRoom;
	ControlHandle		whichControl;
	Point				wherePt, globalWhere;
	long				controlRef;
	short				whichPart, localH, localV;
	short				roomH, roomV, itsNumber;
	ControlActionUPP	scrollHActionUPP, scrollVActionUPP;

	wherePt = theEvent->where;

	scrollHActionUPP = NewControlActionUPP(LiveHScrollAction);
	scrollVActionUPP = NewControlActionUPP(LiveVScrollAction);

	if (mapWindow == nil)
		return;

	SetPortWindowPort(mapWindow);
	globalWhere = wherePt;
	GlobalToLocal(&wherePt);
	wherePt.h -= 1;
	wherePt.v -= 1;

	whichPart = FindControl(wherePt, mapWindow, &whichControl);
	if (whichPart == 0)				// User clicked in map content area.
	{
		localH = wherePt.h / kMapRoomWidth;
		localV = wherePt.v / kMapRoomHeight;

		if ((localH >= mapRoomsWide) || (localV >= mapRoomsHigh))
			return;

		roomH = localH + mapLeftRoom;
		roomV = kMapGroundValue - (localV + mapTopRoom);

		if (RoomExists(roomH, roomV, &itsNumber))
		{
			CopyRoomToThisRoom(itsNumber);
			DeselectObject();
			ReflectCurrentRoom(false);

			if (thisMac.hasDrag)
			{
				SetPortWindowPort(mainWindow);
				QSetRect(&aRoom, 0, 0, kMapRoomWidth, kMapRoomHeight);
				CenterRectOnPoint(&aRoom, globalWhere);
//				if (DragRoom(theEvent, &aRoom, itsNumber))
//				{		// TEMP disabled.
//				}
			}
		}
		else
		{
			if (doBitchDialogs)
			{
				if (QueryNewRoom())
				{
					if (!CreateNewRoom(roomH, roomV))
					{
						YellowAlert(kYellowUnaccounted, 11);
						return;
					}
					else
					{
						DeselectObject();
						ReflectCurrentRoom(false);
					}
				}
				else
					return;
			}
			else
			{
				if (!CreateNewRoom(roomH, roomV))
				{
					YellowAlert(kYellowUnaccounted, 11);
					return;
				}
				else
				{
					DeselectObject();
					ReflectCurrentRoom(false);
				}
			}
		}
	}
	else
	{
		controlRef = GetControlReference(whichControl);
		if (controlRef == kHScrollRef)
		{
			switch (whichPart)
			{
				case kControlUpButtonPart:
				case kControlDownButtonPart:
				case kControlPageUpPart:
				case kControlPageDownPart:
				if (TrackControl(whichControl, wherePt, scrollHActionUPP))
				{

				}
				break;

				case kControlIndicatorPart:
				if (TrackControl(whichControl, wherePt, nil))
				{
					mapLeftRoom = GetControlValue(whichControl);
					RedrawMapContents();
				}
				break;
			}
		}
		else if (controlRef == kVScrollRef)
		{
			switch (whichPart)
			{
				case kControlUpButtonPart:
				case kControlDownButtonPart:
				case kControlPageUpPart:
				case kControlPageDownPart:
				if (TrackControl(whichControl, wherePt, scrollVActionUPP))
				{

				}
				break;

				case kControlIndicatorPart:
				if (TrackControl(whichControl, wherePt, nil))
				{
					mapTopRoom = GetControlValue(whichControl);
					RedrawMapContents();
				}
				break;
			}
		}
	}

	DisposeControlActionUPP(scrollHActionUPP);
	DisposeControlActionUPP(scrollVActionUPP);
#endif
#endif
}

//--------------------------------------------------------------  QueryNewRoom

#ifndef COMPILEDEMO
Boolean QueryNewRoom (void)
{
	return true;
#if 0
	short		hitWhat;

//	CenterAlert(kNewRoomAlert);
	hitWhat = Alert(kNewRoomAlert, nil);
	if (hitWhat == kYesDoNewRoom)
		return (true);
	else
		return (false);
#endif
}
#endif

//--------------------------------------------------------------  CreateNailOffscreen

#ifndef COMPILEDEMO
void CreateNailOffscreen (void)
{
	OSErr		theErr;

	if (nailSrcMap == NULL)
	{
		QSetRect(&nailSrcRect, 0, 0, kMapRoomWidth, kMapRoomHeight * (kNumBackgrounds + 1));
		theErr = CreateOffScreenGWorld(&nailSrcMap, &nailSrcRect, kPreferredDepth);
		LoadGraphic(nailSrcMap, kThumbnailPictID);
	}
}
#endif

//--------------------------------------------------------------  KillNailOffscreen

#ifndef COMPILEDEMO
void KillNailOffscreen (void)
{
	if (nailSrcMap != NULL)
	{
		DisposeGWorld(nailSrcMap);
		nailSrcMap = NULL;
	}
}
#endif

//--------------------------------------------------------------  MoveRoom

void MoveRoom (Point wherePt)
{
	SInt16		localH, localV;
	SInt16		roomH, roomV, itsNumber;

	localH = wherePt.h / kMapRoomWidth;
	localV = wherePt.v / kMapRoomHeight;

	if ((localH >= mapRoomsWide) || (localV >= mapRoomsHigh))
		return;

	roomH = localH + mapLeftRoom;
	roomV = kMapGroundValue - (localV + mapTopRoom);

	if (RoomExists(roomH, roomV, &itsNumber))
	{

	}
	else
	{
		thisRoom->floor = roomV;
		thisRoom->suite = roomH;
		fileDirty = true;
		UpdateMenus(false);
		RedrawMapContents();
	}
}

