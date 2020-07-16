#include "StructuresInit.h"

//============================================================================
//----------------------------------------------------------------------------
//                              StructuresInit.c
//----------------------------------------------------------------------------
//============================================================================


#include "AnimCursor.h"
#include "ByteIO.h"
#include "DynamicMaps.h"
#include "Dynamics.h"
#include "GameOver.h"
#include "GliderDefines.h"
#include "Grease.h"
#include "Input.h"
#include "Macintosh.h"
#include "MainWindow.h"
#include "ObjectEdit.h"
#include "Objects.h"
#include "Play.h"
#include "Player.h"
#include "RectUtils.h"
#include "ResourceIDs.h"
#include "Room.h"
#include "RoomGraphics.h"
#include "RoomInfo.h"
#include "RubberBands.h"
#include "Scoreboard.h"
#include "SelectHouse.h"
#include "StructIO.h"
#include "Utilities.h"
#include "WinAPI.h"


void InitScoreboardMap (void);
void InitGliderMap (void);
void InitBlowers (void);
void InitFurniture (void);
void InitPrizes (void);
void InitTransports (void);
void InitSwitches (void);
void InitLights (void);
void InitAppliances (void);
void InitEnemies (void);
void InitClutter (void);
void InitSupport (void);
void InitAngel (void);


//==============================================================  Functions
//--------------------------------------------------------------  InitScoreboardMap
// Any graphics and structures relating to the scoreboard that appears…
// across the top of the game are initialized and loaded up here.

void InitScoreboardMap (void)
{
	Rect		bounds;
	HBITMAP		thePicture;
	BITMAP		bmInfo;
	LOGFONT		lfScoreboard;
	OSErr		theErr;
	SInt16		hOffset;

	lfScoreboard.lfHeight = -12;
	lfScoreboard.lfWidth = 0;
	lfScoreboard.lfEscapement = 0;
	lfScoreboard.lfOrientation = 0;
	lfScoreboard.lfWeight = FW_BOLD;
	lfScoreboard.lfItalic = FALSE;
	lfScoreboard.lfUnderline = FALSE;
	lfScoreboard.lfStrikeOut = FALSE;
	lfScoreboard.lfCharSet = DEFAULT_CHARSET;
	lfScoreboard.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lfScoreboard.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lfScoreboard.lfQuality = DEFAULT_QUALITY;
	lfScoreboard.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	StringCchCopy(lfScoreboard.lfFaceName, ARRAYSIZE(lfScoreboard.lfFaceName), L"Tahoma");

	wasScoreboardMode = kScoreboardHigh;
	boardSrcRect = houseRect;
	ZeroRectCorner(&boardSrcRect);
	boardSrcRect.bottom = kScoreboardTall;
	theErr = CreateOffScreenGWorld(&boardSrcMap, &boardSrcRect, kPreferredDepth);

	if (boardSrcRect.right >= 640)
		hOffset = (RectWide(&boardSrcRect) - kMaxViewWidth) / 2;
	else
		hOffset = -576;
	thePicture = GetPicture(kScoreboardPictID);
	if (thePicture == NULL)
		RedAlert(kErrFailedGraphicLoad);
	GetObject(thePicture, sizeof(bmInfo), &bmInfo);
	QSetRect(&bounds, 0, 0, (SInt16)bmInfo.bmWidth, (SInt16)bmInfo.bmHeight);
	QOffsetRect(&bounds, hOffset, 0);
	Mac_DrawPicture(boardSrcMap, thePicture, &bounds);
	DeleteObject(thePicture);

	QSetRect(&badgeSrcRect, 0, 0, 32, 66);				// 2144 pixels
	theErr = CreateOffScreenGWorld(&badgeSrcMap, &badgeSrcRect, kPreferredDepth);
	LoadGraphic(badgeSrcMap, kBadgePictID);

	boardDestRect = boardSrcRect;
	QOffsetRect(&boardDestRect, 0, -kScoreboardTall);

	hOffset = (RectWide(&houseRect) - 640) / 2;
	if (hOffset < 0)
		hOffset = -128;

	QSetRect(&boardTSrcRect, 0, 0, 256, 12);			// room title
	theErr = CreateOffScreenGWorld(&boardTSrcMap, &boardTSrcRect, kPreferredDepth);
	boardTDestRect = boardTSrcRect;
	QOffsetRect(&boardTDestRect, 137 + hOffset, 5);
	SelectObject(boardTSrcMap, CreateFontIndirect(&lfScoreboard));

	QSetRect(&boardGSrcRect, 0, 0, 20, 10);				// # gliders
	theErr = CreateOffScreenGWorld(&boardGSrcMap, &boardGSrcRect, kPreferredDepth);
	boardGDestRect = boardGSrcRect;
	QOffsetRect(&boardGDestRect, 526 + hOffset, 5);
	SelectObject(boardGSrcMap, CreateFontIndirect(&lfScoreboard));

	QSetRect(&boardPSrcRect, 0, 0, 64, 10);				// points
	theErr = CreateOffScreenGWorld(&boardPSrcMap, &boardPSrcRect, kPreferredDepth);
	boardPDestRect = boardPSrcRect;
	QOffsetRect(&boardPDestRect, 570 + hOffset, 5);		// total = 6396 pixels
	boardPQDestRect = boardPDestRect;
	QOffsetRect(&boardPQDestRect, 0, -kScoreboardTall);
	boardGQDestRect = boardGDestRect;
	QOffsetRect(&boardGQDestRect, 0, -kScoreboardTall);
	SelectObject(boardPSrcMap, CreateFontIndirect(&lfScoreboard));

	QSetRect(&badgesBlankRects[0], 0, 0, 16, 16);		// foil
	QOffsetRect(&badgesBlankRects[0], 0, 0);
	QSetRect(&badgesBlankRects[1], 0, 0, 16, 16);		// rubber bands
	QOffsetRect(&badgesBlankRects[1], 0, 16);
	QSetRect(&badgesBlankRects[2], 0, 0, 16, 17);		// battery
	QOffsetRect(&badgesBlankRects[2], 0, 32);
	QSetRect(&badgesBlankRects[3], 0, 0, 16, 17);		// helium
	QOffsetRect(&badgesBlankRects[3], 0, 49);

	QSetRect(&badgesBadgesRects[0], 0, 0, 16, 16);		// foil
	QOffsetRect(&badgesBadgesRects[0], 16, 0);
	QSetRect(&badgesBadgesRects[1], 0, 0, 16, 16);		// rubber bands
	QOffsetRect(&badgesBadgesRects[1], 16, 16);
	QSetRect(&badgesBadgesRects[2], 0, 0, 16, 17);		// battery
	QOffsetRect(&badgesBadgesRects[2], 16, 32);
	QSetRect(&badgesBadgesRects[3], 0, 0, 16, 17);		// helium
	QOffsetRect(&badgesBadgesRects[3], 16, 49);

	QSetRect(&badgesDestRects[0], 0, 0, 16, 16);		// foil
	QOffsetRect(&badgesDestRects[0], 432 + hOffset, 2 - kScoreboardTall);
	QSetRect(&badgesDestRects[1], 0, 0, 16, 16);		// rubber bands
	QOffsetRect(&badgesDestRects[1], 449 + hOffset, 2 - kScoreboardTall);
	QSetRect(&badgesDestRects[2], 0, 0, 16, 17);		// battery
	QOffsetRect(&badgesDestRects[2], 467 + hOffset, 1 - kScoreboardTall);
	QSetRect(&badgesDestRects[3], 0, 0, 16, 17);		// helium
	QOffsetRect(&badgesDestRects[3], 467 + hOffset, 1 - kScoreboardTall);
}

//--------------------------------------------------------------  InitGliderMap
// Graphics and structures relating to the little paper glider (the…
// player) are cretaed, loaded up and initialized here.

void InitGliderMap (void)
{
	OSErr		theErr;
	SInt16		i;

	QSetRect(&glidSrcRect, 0, 0, kGliderWide, 668);	// 32112 pixels
	theErr = CreateOffScreenGWorld(&glidSrcMap, &glidSrcRect, kPreferredDepth);
	LoadGraphic(glidSrcMap, kGliderPictID);

	theErr = CreateOffScreenGWorld(&glid2SrcMap, &glidSrcRect, kPreferredDepth);
	LoadGraphic(glid2SrcMap, kGlider2PictID);

	theErr = CreateOffScreenGWorld(&glidMaskMap, &glidSrcRect, 1);
	LoadGraphic(glidMaskMap, kGliderMaskID);

	for (i = 0; i <= 20; i++)
	{
		QSetRect(&gliderSrc[i], 0, 0, kGliderWide, kGliderHigh);
		QOffsetRect(&gliderSrc[i], 0, kGliderHigh * i);
	}
	for (i = 21; i <= 28; i++)
	{
		QSetRect(&gliderSrc[i], 0, 0, kGliderWide, kGliderBurningHigh);
		QOffsetRect(&gliderSrc[i], 0, 420 + (kGliderBurningHigh * (i - 21)));
	}

	QSetRect(&gliderSrc[29], 0, 0, kGliderWide, kGliderHigh);
	QOffsetRect(&gliderSrc[29], 0, 628);
	QSetRect(&gliderSrc[30], 0, 0, kGliderWide, kGliderHigh);
	QOffsetRect(&gliderSrc[30], 0, 648);

	QSetRect(&shadowSrcRect, 0, 0, kGliderWide, kShadowHigh * kNumShadowSrcRects);
	theErr = CreateOffScreenGWorld(&shadowSrcMap, &shadowSrcRect, kPreferredDepth);
	LoadGraphic(shadowSrcMap, kShadowPictID);

	theErr = CreateOffScreenGWorld(&shadowMaskMap, &shadowSrcRect, 1);
	LoadGraphic(shadowMaskMap, kShadowMaskID);

	for (i = 0; i < kNumShadowSrcRects; i++)
	{
		QSetRect(&shadowSrc[i], 0, 0, kGliderWide, kShadowHigh);
		QOffsetRect(&shadowSrc[i], 0, kShadowHigh * i);
	}

	QSetRect(&bandsSrcRect, 0, 0, 16, 18);		// 304 pixels
	theErr = CreateOffScreenGWorld(&bandsSrcMap, &bandsSrcRect, kPreferredDepth);
	LoadGraphic(bandsSrcMap, kRubberBandsPictID);

	theErr = CreateOffScreenGWorld(&bandsMaskMap, &bandsSrcRect, 1);
	LoadGraphic(bandsMaskMap, kRubberBandsMaskID);

	for (i = 0; i < 3; i++)
	{
		QSetRect(&bandRects[i], 0, 0, 16, 6);
		QOffsetRect(&bandRects[i], 0, 6 * i);
	}
}

//--------------------------------------------------------------  InitBlowers
// All blower graphics and structures are loaded up and initialized here.
// Blowers include vents, ducts, candles, fans, etc.

void InitBlowers (void)
{
	SInt16		i;
	OSErr		theErr;

	QSetRect(&blowerSrcRect, 0, 0, 48, 402);	// 19344 pixels
	theErr = CreateOffScreenGWorld(&blowerSrcMap, &blowerSrcRect, kPreferredDepth);
	LoadGraphic(blowerSrcMap, kBlowerPictID);

	theErr = CreateOffScreenGWorld(&blowerMaskMap, &blowerSrcRect, 1);
	LoadGraphic(blowerMaskMap, kBlowerMaskID);

	for (i = 0; i < kNumCandleFlames; i++)
	{
		QSetRect(&flame[i], 0, 0, 16, 15);
		QOffsetRect(&flame[i], 32, 179 + (i * 15));
	}

	for (i = 0; i < kNumTikiFlames; i++)
	{
		QSetRect(&tikiFlame[i], 0, 0, 8, 10);
		QOffsetRect(&tikiFlame[i], 40, 69 + (i * 10));
	}

	for (i = 0; i < kNumBBQCoals; i++)
	{
		QSetRect(&coals[i], 0, 0, 32, 9);
		QOffsetRect(&coals[i], 0, 304 + (i * 9));
	}

	QSetRect(&leftStartGliderSrc, 0, 0, 48, 16);
	QOffsetRect(&leftStartGliderSrc, 0, 358);

	QSetRect(&rightStartGliderSrc, 0, 0, 48, 16);
	QOffsetRect(&rightStartGliderSrc, 0, 374);
}

//--------------------------------------------------------------  InitFurniture
// Structures and graphics relating to the furniture is loaded up.
// Furniture includes tables, cabinets, shelves, etc.

void InitFurniture (void)
{
	OSErr		theErr;

	QSetRect(&furnitureSrcRect, 0, 0, 64, 278);		// 17856 pixels
	theErr = CreateOffScreenGWorld(&furnitureSrcMap, &furnitureSrcRect, kPreferredDepth);
	LoadGraphic(furnitureSrcMap, kFurniturePictID);

	theErr = CreateOffScreenGWorld(&furnitureMaskMap, &furnitureSrcRect, 1);
	LoadGraphic(furnitureMaskMap, kFurnitureMaskID);

	QSetRect(&tableSrc, 0, 0, 64, 22);
	QOffsetRect(&tableSrc, 0, 0);

	QSetRect(&shelfSrc, 0, 0, 16, 21);
	QOffsetRect(&shelfSrc, 0, 22);

	QSetRect(&hingeSrc, 0, 0, 4, 16);
	QOffsetRect(&hingeSrc, 16, 22);

	QSetRect(&handleSrc, 0, 0, 4, 21);
	QOffsetRect(&handleSrc, 20, 22);

	QSetRect(&knobSrc, 0, 0, 8, 8);
	QOffsetRect(&knobSrc, 24, 22);

	QSetRect(&leftFootSrc, 0, 0, 16, 16);
	QOffsetRect(&leftFootSrc, 32, 22);

	QSetRect(&rightFootSrc, 0, 0, 16, 16);
	QOffsetRect(&rightFootSrc, 48, 22);

	QSetRect(&deckSrc, 0, 0, 64, 21);
	QOffsetRect(&deckSrc, 0, 162);
}

//--------------------------------------------------------------  InitPrizes
// Structures and graphics relating to the prizes (bonuses) are loaded up.
// Prizes includes clocks, rubber bands, extra gliders, etc.

void InitPrizes (void)
{
	SInt16		i;
	OSErr		theErr;

	QSetRect(&bonusSrcRect, 0, 0, 88, 378);		// 33264 pixels
	theErr = CreateOffScreenGWorld(&bonusSrcMap, &bonusSrcRect, kPreferredDepth);
	LoadGraphic(bonusSrcMap, kBonusPictID);

	theErr = CreateOffScreenGWorld(&bonusMaskMap, &bonusSrcRect, 1);
	LoadGraphic(bonusMaskMap, kBonusMaskID);

	for (i = 0; i < 11; i++)
	{
		QSetRect(&digits[i], 0, 0, 4, 6);
		QOffsetRect(&digits[i], 28, i * 6);
	}

	for (i = 0; i < 3; i++)
	{
		QSetRect(&pendulumSrc[i], 0, 0, 32, 28);
		QOffsetRect(&pendulumSrc[i], 56, 186 + (i * 28));
	}

	QSetRect(&greaseSrcRt[0], 0, 0, 32, 27);
	QOffsetRect(&greaseSrcRt[0], 0, 243);
	QSetRect(&greaseSrcRt[1], 0, 0, 32, 27);
	QOffsetRect(&greaseSrcRt[1], 0, 270);
	QSetRect(&greaseSrcRt[2], 0, 0, 32, 27);
	QOffsetRect(&greaseSrcRt[2], 0, 297);
	QSetRect(&greaseSrcRt[3], 0, 0, 32, 27);
	QOffsetRect(&greaseSrcRt[3], 32, 297);

	QSetRect(&greaseSrcLf[0], 0, 0, 32, 27);
	QOffsetRect(&greaseSrcLf[0], 0, 324);
	QSetRect(&greaseSrcLf[1], 0, 0, 32, 27);
	QOffsetRect(&greaseSrcLf[1], 32, 324);
	QSetRect(&greaseSrcLf[2], 0, 0, 32, 27);
	QOffsetRect(&greaseSrcLf[2], 0, 351);
	QSetRect(&greaseSrcLf[3], 0, 0, 32, 27);
	QOffsetRect(&greaseSrcLf[3], 32, 351);

	for (i = 0; i < 6; i++)
	{
		QSetRect(&starSrc[i], 0, 0, 32, 31);
		QOffsetRect(&starSrc[i], 48, i * 31);
	}

	for (i = 0; i < 3; i++)
	{
		QSetRect(&sparkleSrc[i + 2], 0, 0, 20, 19);
		QOffsetRect(&sparkleSrc[i + 2], 0, 70 + (i * 19));
	}
	sparkleSrc[0] = sparkleSrc[4];
	sparkleSrc[1] = sparkleSrc[3];

	QSetRect(&pointsSrcRect, 0, 0, 24, 120);		// 2880 pixels
	theErr = CreateOffScreenGWorld(&pointsSrcMap, &pointsSrcRect, kPreferredDepth);
	LoadGraphic(pointsSrcMap, kPointsPictID);

	theErr = CreateOffScreenGWorld(&pointsMaskMap, &pointsSrcRect, 1);
	LoadGraphic(pointsMaskMap, kPointsMaskID);

	for (i = 0; i < 15; i++)
	{
		QSetRect(&pointsSrc[i], 0, 0, 24, 8);
		QOffsetRect(&pointsSrc[i], 0, i * 8);
	}
}

//--------------------------------------------------------------  InitTransports
// Structures and graphics relating to the transports is loaded up.
// Transports includes transport ducts, mailboxes, etc.

void InitTransports (void)
{
	OSErr		theErr;

	QSetRect(&transSrcRect, 0, 0, 56, 32);	// 1848 pixels
	theErr = CreateOffScreenGWorld(&transSrcMap, &transSrcRect, kPreferredDepth);
	LoadGraphic(transSrcMap, kTransportPictID);

	theErr = CreateOffScreenGWorld(&transMaskMap, &transSrcRect, 1);
	LoadGraphic(transMaskMap, kTransportMaskID);
}

//--------------------------------------------------------------  InitSwitches
// Structures and graphics relating to switches are loaded up.
// Switches includes triggers, light switches, etc.

void InitSwitches (void)
{
	OSErr		theErr;

	QSetRect(&switchSrcRect, 0, 0, 32, 104);	// 3360 pixels
	theErr = CreateOffScreenGWorld(&switchSrcMap, &switchSrcRect, kPreferredDepth);
	LoadGraphic(switchSrcMap, kSwitchPictID);

	QSetRect(&lightSwitchSrc[0], 0, 0, 15, 24);
	QOffsetRect(&lightSwitchSrc[0], 0, 0);
	QSetRect(&lightSwitchSrc[1], 0, 0, 15, 24);
	QOffsetRect(&lightSwitchSrc[1], 16, 0);

	QSetRect(&machineSwitchSrc[0], 0, 0, 16, 24);
	QOffsetRect(&machineSwitchSrc[0], 0, 24);
	QSetRect(&machineSwitchSrc[1], 0, 0, 16, 24);
	QOffsetRect(&machineSwitchSrc[1], 16, 24);

	QSetRect(&thermostatSrc[0], 0, 0, 15, 24);
	QOffsetRect(&thermostatSrc[0], 0, 48);
	QSetRect(&thermostatSrc[1], 0, 0, 15, 24);
	QOffsetRect(&thermostatSrc[1], 16, 48);

	QSetRect(&powerSrc[0], 0, 0, 8, 8);
	QOffsetRect(&powerSrc[0], 0, 72);
	QSetRect(&powerSrc[1], 0, 0, 8, 8);
	QOffsetRect(&powerSrc[1], 8, 72);

	QSetRect(&knifeSwitchSrc[0], 0, 0, 16, 24);
	QOffsetRect(&knifeSwitchSrc[0], 0, 80);
	QSetRect(&knifeSwitchSrc[1], 0, 0, 16, 24);
	QOffsetRect(&knifeSwitchSrc[1], 16, 80);
}

//--------------------------------------------------------------  InitLights
// Structures and graphics relating to lights are loaded up.
// Lights includes table lamps, flourescent lights, track lights, etc.

void InitLights (void)
{
	SInt16		i;
	OSErr		theErr;

	QSetRect(&lightSrcRect, 0, 0, 72, 126);		// 9144 pixels
	theErr = CreateOffScreenGWorld(&lightSrcMap, &lightSrcRect, kPreferredDepth);
	LoadGraphic(lightSrcMap, kLightPictID);

	theErr = CreateOffScreenGWorld(&lightMaskMap, &lightSrcRect, 1);
	LoadGraphic(lightMaskMap, kLightMaskID);

	QSetRect(&flourescentSrc1, 0, 0, 16, 12);
	QOffsetRect(&flourescentSrc1, 0, 78);

	QSetRect(&flourescentSrc2, 0, 0, 16, 12);
	QOffsetRect(&flourescentSrc2, 0, 90);

	for (i = 0; i < kNumTrackLights; i++)
	{
		QSetRect(&trackLightSrc[i], 0, 0, 24, 24);
		QOffsetRect(&trackLightSrc[i], 24 * i, 102);
	}
}

//--------------------------------------------------------------  InitAppliances
// Structures and graphics relating to appliances are loaded up.
// Appliances includes toasters, T.V.s, etc.

void InitAppliances (void)
{
	SInt16		i;
	OSErr		theErr;

	QSetRect(&applianceSrcRect, 0, 0, 80, 269);		// 21600 pixels
	theErr = CreateOffScreenGWorld(&applianceSrcMap, &applianceSrcRect, kPreferredDepth);
	LoadGraphic(applianceSrcMap, kAppliancePictID);

	theErr = CreateOffScreenGWorld(&applianceMaskMap, &applianceSrcRect, 1);
	LoadGraphic(applianceMaskMap, kApplianceMaskID);

	QSetRect(&toastSrcRect, 0, 0, 32, 174);			// 5600 pixels
	theErr = CreateOffScreenGWorld(&toastSrcMap, &toastSrcRect, kPreferredDepth);
	LoadGraphic(toastSrcMap, kToastPictID);

	theErr = CreateOffScreenGWorld(&toastMaskMap, &toastSrcRect, 1);
	LoadGraphic(toastMaskMap, kToastMaskID);

	QSetRect(&shredSrcRect, 0, 0, 40, 35);			// 1440 pixels
	theErr = CreateOffScreenGWorld(&shredSrcMap, &shredSrcRect, kPreferredDepth);
	LoadGraphic(shredSrcMap, kShreddedPictID);

	theErr = CreateOffScreenGWorld(&shredMaskMap, &shredSrcRect, 1);
	LoadGraphic(shredMaskMap, kShreddedMaskID);

	QSetRect(&plusScreen1, 0, 0, 32, 22);
	QOffsetRect(&plusScreen1, 48, 127);
	QSetRect(&plusScreen2, 0, 0, 32, 22);
	QOffsetRect(&plusScreen2, 48, 149);

	QSetRect(&tvScreen1, 0, 0, 64, 49);
	QOffsetRect(&tvScreen1, 0, 171);
	QSetRect(&tvScreen2, 0, 0, 64, 49);
	QOffsetRect(&tvScreen2, 0, 220);

	QSetRect(&coffeeLight1, 0, 0, 8, 4);
	QOffsetRect(&coffeeLight1, 72, 171);
	QSetRect(&coffeeLight2, 0, 0, 8, 4);
	QOffsetRect(&coffeeLight2, 72, 175);

	for (i = 0; i < kNumOutletPicts; i++)
	{
		QSetRect(&outletSrc[i], 0, 0, 16, 24);
		QOffsetRect(&outletSrc[i], 64, 22 + (i * 24));
	}

	for (i = 0; i < kNumBreadPicts; i++)
	{
		QSetRect(&breadSrc[i], 0, 0, 32, 29);
		QOffsetRect(&breadSrc[i], 0, i * 29);
	}

	QSetRect(&vcrTime1, 0, 0, 16, 4);
	QOffsetRect(&vcrTime1, 64, 179);
	QSetRect(&vcrTime2, 0, 0, 16, 4);
	QOffsetRect(&vcrTime2, 64, 183);

	QSetRect(&stereoLight1, 0, 0, 4, 1);
	QOffsetRect(&stereoLight1, 68, 171);
	QSetRect(&stereoLight2, 0, 0, 4, 1);
	QOffsetRect(&stereoLight2, 68, 172);

	QSetRect(&microOn, 0, 0, 16, 35);
	QOffsetRect(&microOn, 64, 222);
	QSetRect(&microOff, 0, 0, 16, 35);
	QOffsetRect(&microOff, 64, 187);
}

//--------------------------------------------------------------  InitEnemies
// Structures and graphics relating to enemies are loaded up.
// Enemies includes darts, balloons, fish, etc.

void InitEnemies (void)
{
	SInt16		i;
	OSErr		theErr;

	QSetRect(&balloonSrcRect, 0, 0, 24, 30 * kNumBalloonFrames);
	theErr = CreateOffScreenGWorld(&balloonSrcMap, &balloonSrcRect, kPreferredDepth);
	LoadGraphic(balloonSrcMap, kBalloonPictID);

	theErr = CreateOffScreenGWorld(&balloonMaskMap, &balloonSrcRect, 1);
	LoadGraphic(balloonMaskMap, kBalloonMaskID);

	QSetRect(&copterSrcRect, 0, 0, 32, 30 * kNumCopterFrames);
	theErr = CreateOffScreenGWorld(&copterSrcMap, &copterSrcRect, kPreferredDepth);
	LoadGraphic(copterSrcMap, kCopterPictID);

	theErr = CreateOffScreenGWorld(&copterMaskMap, &copterSrcRect, 1);
	LoadGraphic(copterMaskMap, kCopterMaskID);

	QSetRect(&dartSrcRect, 0, 0, 64, 19 * kNumDartFrames);
	theErr = CreateOffScreenGWorld(&dartSrcMap, &dartSrcRect, kPreferredDepth);
	LoadGraphic(dartSrcMap, kDartPictID);

	theErr = CreateOffScreenGWorld(&dartMaskMap, &dartSrcRect, 1);
	LoadGraphic(dartMaskMap, kDartMaskID);

	QSetRect(&ballSrcRect, 0, 0, 32, 32 * kNumBallFrames);
	theErr = CreateOffScreenGWorld(&ballSrcMap, &ballSrcRect, kPreferredDepth);
	LoadGraphic(ballSrcMap, kBallPictID);

	theErr = CreateOffScreenGWorld(&ballMaskMap, &ballSrcRect, 1);
	LoadGraphic(ballMaskMap, kBallMaskID);

	QSetRect(&dripSrcRect, 0, 0, 16, 12 * kNumDripFrames);
	theErr = CreateOffScreenGWorld(&dripSrcMap, &dripSrcRect, kPreferredDepth);
	LoadGraphic(dripSrcMap, kDripPictID);

	theErr = CreateOffScreenGWorld(&dripMaskMap, &dripSrcRect, 1);
	LoadGraphic(dripMaskMap, kDripMaskID);

	QSetRect(&enemySrcRect, 0, 0, 36, 33);
	theErr = CreateOffScreenGWorld(&enemySrcMap, &enemySrcRect, kPreferredDepth);
	LoadGraphic(enemySrcMap, kEnemyPictID);

	theErr = CreateOffScreenGWorld(&enemyMaskMap, &enemySrcRect, 1);
	LoadGraphic(enemyMaskMap, kEnemyMaskID);

	QSetRect(&fishSrcRect, 0, 0, 16, 16 * kNumFishFrames);
	theErr = CreateOffScreenGWorld(&fishSrcMap, &fishSrcRect, kPreferredDepth);
	LoadGraphic(fishSrcMap, kFishPictID);

	theErr = CreateOffScreenGWorld(&fishMaskMap, &fishSrcRect, 1);
	LoadGraphic(fishMaskMap, kFishMaskID);

	for (i = 0; i < kNumBalloonFrames; i++)
	{
		QSetRect(&balloonSrc[i], 0, 0, 24, 30);
		QOffsetRect(&balloonSrc[i], 0, 30 * i);
	}

	for (i = 0; i < kNumCopterFrames; i++)
	{
		QSetRect(&copterSrc[i], 0, 0, 32, 30);
		QOffsetRect(&copterSrc[i], 0, 30 * i);
	}

	for (i = 0; i < kNumDartFrames; i++)
	{
		QSetRect(&dartSrc[i], 0, 0, 64, 19);
		QOffsetRect(&dartSrc[i], 0, 19 * i);
	}

	for (i = 0; i < kNumBallFrames; i++)
	{
		QSetRect(&ballSrc[i], 0, 0, 32, 32);
		QOffsetRect(&ballSrc[i], 0, 32 * i);
	}

	for (i = 0; i < kNumDripFrames; i++)
	{
		QSetRect(&dripSrc[i], 0, 0, 16, 12);
		QOffsetRect(&dripSrc[i], 0, 12 * i);
	}

	for (i = 0; i < kNumFishFrames; i++)
	{
		QSetRect(&fishSrc[i], 0, 0, 16, 16);
		QOffsetRect(&fishSrc[i], 0, 16 * i);
	}
}

//--------------------------------------------------------------  InitClutter
// Structures and graphics relating to clutter are loaded up.
// Clutter includes mirrors, teddy bears, fireplaces, calendars, etc.

void InitClutter (void)
{
	OSErr		theErr;

	QSetRect(&clutterSrcRect, 0, 0, 128, 69);
	theErr = CreateOffScreenGWorld(&clutterSrcMap, &clutterSrcRect, kPreferredDepth);
	LoadGraphic(clutterSrcMap, kClutterPictID);

	theErr = CreateOffScreenGWorld(&clutterMaskMap, &clutterSrcRect, 1);
	LoadGraphic(clutterMaskMap, kClutterMaskID);

	QSetRect(&flowerSrc[0], 0, 0, 10, 28);
	QOffsetRect(&flowerSrc[0], 0, 23);

	QSetRect(&flowerSrc[1], 0, 0, 24, 35);
	QOffsetRect(&flowerSrc[1], 10, 16);

	QSetRect(&flowerSrc[2], 0, 0, 34, 35);
	QOffsetRect(&flowerSrc[2], 34, 16);

	QSetRect(&flowerSrc[3], 0, 0, 27, 23);
	QOffsetRect(&flowerSrc[3], 68, 14);

	QSetRect(&flowerSrc[4], 0, 0, 27, 14);
	QOffsetRect(&flowerSrc[4], 68, 37);

	QSetRect(&flowerSrc[5], 0, 0, 32, 51);
	QOffsetRect(&flowerSrc[5], 95, 0);
}

//--------------------------------------------------------------  InitSupport
// The floor support grphic is loaded up.  It is only visible when…
// playing in 9-room mode.  It is the horizontal wooden beam that…
// seperates floors from one another.

void InitSupport (void)
{
	OSErr		theErr;

	QSetRect(&suppSrcRect, 0, 0, kRoomWide, kFloorSupportTall);		// 44
	theErr = CreateOffScreenGWorld(&suppSrcMap, &suppSrcRect, kPreferredDepth);
	LoadGraphic(suppSrcMap, kSupportPictID);
}

//--------------------------------------------------------------  InitAngel
// This loads the graphic of the girl riding the glider.  It is seen…
// only upon completing a house.  She flies across the screen dropping…
// stars below.

void InitAngel (void)
{
	OSErr		theErr;

	QSetRect(&angelSrcRect, 0, 0, 96, 44);
	theErr = CreateOffScreenGWorld(&angelSrcMap, &angelSrcRect, kPreferredDepth);
	LoadGraphic(angelSrcMap, kAngelPictID);

	theErr = CreateOffScreenGWorld(&angelMaskMap, &angelSrcRect, 1);
	LoadGraphic(angelMaskMap, kAngelMaskID);
}

//--------------------------------------------------------------  CreateOffscreens
// All "utility" or "work" offscreen pix/bit maps are created here.
// These would be offscreens that are reused throughout a game - they…
// are not static (mere repositories for graphics).  Most are used…
// to facilitate the animation when a game is in progress.

void CreateOffscreens (void)
{
	OSErr		theErr;

	justRoomsRect = houseRect;
	ZeroRectCorner(&justRoomsRect);

	workSrcRect = houseRect;			// Set up work map
	ZeroRectCorner(&workSrcRect);
	theErr = CreateOffScreenGWorld(&workSrcMap, &workSrcRect, kPreferredDepth);

	backSrcRect = houseRect;			// Set up background map
	ZeroRectCorner(&backSrcRect);
	theErr = CreateOffScreenGWorld(&backSrcMap, &backSrcRect, kPreferredDepth);

	InitScoreboardMap();	SpinCursor(1);
	InitGliderMap();		SpinCursor(1);
	InitBlowers();			SpinCursor(1);
	InitFurniture();		SpinCursor(1);
	InitPrizes();			SpinCursor(1);
	InitTransports();		SpinCursor(1);
	InitSwitches();			SpinCursor(1);
	InitLights();			SpinCursor(1);
	InitAppliances();		SpinCursor(1);
	InitEnemies();			SpinCursor(1);
	InitClutter();			SpinCursor(1);
	InitSupport();			SpinCursor(1);
	InitAngel();			SpinCursor(1);

	QSetRect(&tileSrcRect, 0, 0, 128, 80);
	tileSrcMap = NULL;
//	????
}

//--------------------------------------------------------------  CreatePointers
// This function allocates other large structures.  Pointers to hold…
// large arrays, etc.

void CreatePointers (void)
{
	DWORD demoResourceSize;
	LPVOID demoResourceData;
	byteio demoReader;
	HRESULT hr;
	size_t i;

	thisRoom = NULL;
	thisRoom = (roomPtr)malloc(sizeof(roomType));
	if (thisRoom == NULL)
		RedAlert(kErrNoMemory);

	hotSpots = NULL;
	hotSpots = (hotPtr)malloc(sizeof(hotObject) * kMaxHotSpots);
	if (hotSpots == NULL)
		RedAlert(kErrNoMemory);

	sparkles = NULL;
	sparkles = (sparklePtr)malloc(sizeof(sparkleType) * kMaxSparkles);
	if (sparkles == NULL)
		RedAlert(kErrNoMemory);

	flyingPoints = NULL;
	flyingPoints = (flyingPtPtr)malloc(sizeof(flyingPtType) * kMaxFlyingPts);
	if (flyingPoints == NULL)
		RedAlert(kErrNoMemory);

	flames = NULL;
	flames = (flamePtr)malloc(sizeof(flameType) * kMaxCandles);
	if (flames == NULL)
		RedAlert(kErrNoMemory);

	tikiFlames = NULL;
	tikiFlames = (flamePtr)malloc(sizeof(flameType) * kMaxTikis);
	if (tikiFlames == NULL)
		RedAlert(kErrNoMemory);

	bbqCoals = NULL;
	bbqCoals = (flamePtr)malloc(sizeof(flameType) * kMaxCoals);
	if (bbqCoals == NULL)
		RedAlert(kErrNoMemory);

	pendulums = NULL;
	pendulums = (pendulumPtr)malloc(sizeof(pendulumType) * kMaxPendulums);
	if (pendulums == NULL)
		RedAlert(kErrNoMemory);

	for (i = 0; i < kMaxSavedMaps; i++)
		savedMaps[i].map = NULL;

	bands = NULL;
	bands = (bandPtr)malloc(sizeof(bandType) * kMaxRubberBands);
	if (bands == NULL)
		RedAlert(kErrNoMemory);

	grease = NULL;
	grease = (greasePtr)malloc(sizeof(greaseType) * kMaxGrease);
	if (grease == NULL)
		RedAlert(kErrNoMemory);

	theStars = NULL;
	theStars = (starPtr)malloc(sizeof(starType) * kMaxStars);
	if (theStars == NULL)
		RedAlert(kErrNoMemory);

	shreds = NULL;
	shreds = (shredPtr)malloc(sizeof(shredType) * kMaxShredded);
	if (shreds == NULL)
		RedAlert(kErrNoMemory);

	dinahs = NULL;
	dinahs = (dynaPtr)malloc(sizeof(dynaType) * kMaxDynamicObs);
	if (dinahs == NULL)
		RedAlert(kErrNoMemory);

	masterObjects = NULL;
	masterObjects = (objDataPtr)malloc(sizeof(objDataType) * kMaxMasterObjects);
	if (masterObjects == NULL)
		RedAlert(kErrNoMemory);

	srcRects = NULL;
	srcRects = (Rect *)malloc(sizeof(Rect) * kNumSrcRects);
	if (srcRects == NULL)
		RedAlert(kErrNoMemory);

	theHousesSpecs = NULL;
	theHousesSpecs = (houseSpecPtr)malloc(sizeof(houseSpec) * maxFiles);
	if (theHousesSpecs == NULL)
		RedAlert(kErrNoMemory);

#ifdef CREATEDEMODATA
	demoData = NULL;
	demoData = (demoPtr)malloc(sizeof(demoType) * 2000);
	if (demoData == NULL)
		RedAlert(kErrNoMemory);
#else
	demoData = NULL;
	demoData = (demoPtr)malloc(sizeof(demoType) * (kDemoLength / demoTypeByteSize));
	if (demoData == NULL)
		RedAlert(kErrNoMemory);

	hr = LoadModuleResource(HINST_THISCOMPONENT,
		MAKEINTRESOURCE(IDR_DEMO), RT_DEMO,
		&demoResourceData, &demoResourceSize);
	if (FAILED(hr) || demoResourceSize != kDemoLength)
		RedAlert(kErrFailedResourceLoad);

	if (!byteio_init_memory_reader(&demoReader, demoResourceData, demoResourceSize))
		RedAlert(kErrNoMemory);
	for (i = 0; i < (kDemoLength / demoTypeByteSize); i++)
	{
		if (!ReadDemoType(&demoReader, &demoData[i]))
			RedAlert(kErrFailedResourceLoad);
	}
	byteio_close(&demoReader);
#endif
}

//--------------------------------------------------------------  InitSrcRects
// This is a nasty, ugly function that initializes all global rectangles…
// used in Glider PRO.

void InitSrcRects (void)
{
	QSetRect(&srcRects[kFloorVent], 0, 0, 48, 11);		// Blowers
	QOffsetRect(&srcRects[kFloorVent], 0, 0);
	QSetRect(&srcRects[kCeilingVent], 0, 0, 48, 11);
	QOffsetRect(&srcRects[kCeilingVent], 0, 11);
	QSetRect(&srcRects[kFloorBlower], 0, 0, 48, 15);
	QOffsetRect(&srcRects[kFloorBlower], 0, 22);
	QSetRect(&srcRects[kCeilingBlower], 0, 0, 48, 15);
	QOffsetRect(&srcRects[kCeilingBlower], 0, 37);
	QSetRect(&srcRects[kSewerGrate], 0, 0, 48, 17);
	QOffsetRect(&srcRects[kSewerGrate], 0, 52);
	QSetRect(&srcRects[kLeftFan], 0, 0, 40, 55);
	QOffsetRect(&srcRects[kLeftFan], 0, 69);
	QSetRect(&srcRects[kRightFan], 0, 0, 40, 55);
	QOffsetRect(&srcRects[kRightFan], 0, 124);
	QSetRect(&srcRects[kTaper], 0, 0, 20, 59);
	QOffsetRect(&srcRects[kTaper], 0, 209);
	QSetRect(&srcRects[kCandle], 0, 0, 32, 30);
	QOffsetRect(&srcRects[kCandle], 0, 179);
	QSetRect(&srcRects[kStubby], 0, 0, 20, 36);
	QOffsetRect(&srcRects[kStubby], 0, 268);
	QSetRect(&srcRects[kTiki], 0, 0, 27, 28);
	QOffsetRect(&srcRects[kTiki], 21, 268);
	QSetRect(&srcRects[kBBQ], 0, 0, 64, 33);
	QSetRect(&srcRects[kInvisBlower], 0, 0, 24, 24);
	QSetRect(&srcRects[kGrecoVent], 0, 0, 48, 18);
	QOffsetRect(&srcRects[kGrecoVent], 0, 340);
	QSetRect(&srcRects[kSewerBlower], 0, 0, 32, 12);
	QOffsetRect(&srcRects[kSewerBlower], 0, 390);
	QSetRect(&srcRects[kLiftArea], 0, 0, 64, 32);

	QSetRect(&srcRects[kTable], 0, 0, 64, kTableThick);		// Furniture
	QSetRect(&srcRects[kShelf], 0, 0, 64, kShelfThick);
	QSetRect(&srcRects[kCabinet], 0, 0, 64, 64);
	QSetRect(&srcRects[kFilingCabinet], 0, 0, 74, 107);
	QSetRect(&srcRects[kWasteBasket], 0, 0, 64, 61);
	QOffsetRect(&srcRects[kWasteBasket], 0, 43);
	QSetRect(&srcRects[kMilkCrate], 0, 0, 64, 58);
	QOffsetRect(&srcRects[kMilkCrate], 0, 104);
	QSetRect(&srcRects[kCounter], 0, 0, 128, 64);
	QSetRect(&srcRects[kDresser], 0, 0, 128, 64);
	QSetRect(&srcRects[kDeckTable], 0, 0, 64, kTableThick);
	QSetRect(&srcRects[kStool], 0, 0, 48, 38);
	QOffsetRect(&srcRects[kStool], 0, 183);
	QSetRect(&srcRects[kTrunk], 0, 0, 144, 80);
	QSetRect(&srcRects[kInvisObstacle], 0, 0, 64, 64);
	QSetRect(&srcRects[kManhole], 0, 0, 123, 22);
	QSetRect(&srcRects[kBooks], 0, 0, 64, 51);
	QSetRect(&srcRects[kInvisBounce], 0, 0, 64, 64);

	QSetRect(&srcRects[kRedClock], 0, 0, 28, 17);			// Prizes
	QSetRect(&srcRects[kBlueClock], 0, 0, 28, 25);
	QOffsetRect(&srcRects[kBlueClock], 0, 17);
	QSetRect(&srcRects[kYellowClock], 0, 0, 28, 28);
	QOffsetRect(&srcRects[kYellowClock], 0, 42);
	QSetRect(&srcRects[kCuckoo], 0, 0, 40, 80);
	QOffsetRect(&srcRects[kCuckoo], 0, 148);
	QSetRect(&srcRects[kPaper], 0, 0, 48, 21);
	QOffsetRect(&srcRects[kPaper], 0, 127);
	QSetRect(&srcRects[kBattery], 0, 0, 16, 25);
	QOffsetRect(&srcRects[kBattery], 32, 0);
	QSetRect(&srcRects[kBands], 0, 0, 28, 23);
	QOffsetRect(&srcRects[kBands], 20, 70);
	QSetRect(&srcRects[kGreaseRt], 0, 0, 32, 27);
	QOffsetRect(&srcRects[kGreaseRt], 0, 243);
	QSetRect(&srcRects[kGreaseLf], 0, 0, 32, 27);
	QOffsetRect(&srcRects[kGreaseLf], 0, 324);
	QSetRect(&srcRects[kFoil], 0, 0, 55, 15);
	QOffsetRect(&srcRects[kFoil], 0, 228);
	QSetRect(&srcRects[kInvisBonus], 0, 0, 24, 24);
	QSetRect(&srcRects[kStar], 0, 0, 32, 31);
	QOffsetRect(&srcRects[kStar], 48, 0);
	QSetRect(&srcRects[kSparkle], 0, 0, 20, 19);
	QOffsetRect(&srcRects[kSparkle], 0, 70);
	QSetRect(&srcRects[kHelium], 0, 0, 56, 16);
	QOffsetRect(&srcRects[kHelium], 32, 270);
	QSetRect(&srcRects[kSlider], 0, 0, 64, 16);

	QSetRect(&srcRects[kUpStairs], 0, 0, 160, 267);			// Transport
	QSetRect(&srcRects[kDownStairs], 0, 0, 160, 267);
	QSetRect(&srcRects[kMailboxLf], 0, 0, 94, 80);
	QSetRect(&srcRects[kMailboxRt], 0, 0, 94, 80);
	QSetRect(&srcRects[kFloorTrans], 0, 0, 56, 15);
	QOffsetRect(&srcRects[kFloorTrans], 0, 1);
	QSetRect(&srcRects[kCeilingTrans], 0, 0, 56, 15);
	QOffsetRect(&srcRects[kCeilingTrans], 0, 16);
	QSetRect(&srcRects[kDoorInLf], 0, 0, 144, 322);
	QSetRect(&srcRects[kDoorInRt], 0, 0, 144, 322);
	QSetRect(&srcRects[kDoorExRt], 0, 0, 16, 322);
	QSetRect(&srcRects[kDoorExLf], 0, 0, 16, 322);
	QSetRect(&srcRects[kWindowInLf], 0, 0, 20, 170);
	QSetRect(&srcRects[kWindowInRt], 0, 0, 20, 170);
	QSetRect(&srcRects[kWindowExRt], 0, 0, 16, 170);
	QSetRect(&srcRects[kWindowExLf], 0, 0, 16, 170);
	QSetRect(&srcRects[kInvisTrans], 0, 0, 64, 32);
	QSetRect(&srcRects[kDeluxeTrans], 0, 0, 64, 64);

	QSetRect(&srcRects[kLightSwitch], 0, 0, 15, 24);		// Switch
	QSetRect(&srcRects[kMachineSwitch], 0, 0, 16, 24);
	QOffsetRect(&srcRects[kMachineSwitch], 0, 48);
	QSetRect(&srcRects[kThermostat], 0, 0, 15, 24);
	QOffsetRect(&srcRects[kThermostat], 0, 48);
	QSetRect(&srcRects[kPowerSwitch], 0, 0, 8, 8);
	QOffsetRect(&srcRects[kPowerSwitch], 0, 72);
	QSetRect(&srcRects[kKnifeSwitch], 0, 0, 16, 24);
	QOffsetRect(&srcRects[kKnifeSwitch], 0, 80);
	QSetRect(&srcRects[kInvisSwitch], 0, 0, 12, 12);
	QSetRect(&srcRects[kTrigger], 0, 0, 12, 12);
	QSetRect(&srcRects[kLgTrigger], 0, 0, 48, 48);
	QSetRect(&srcRects[kSoundTrigger], 0, 0, 32, 32);

	QSetRect(&srcRects[kCeilingLight], 0, 0, 64, 20);		// Lights
	QOffsetRect(&srcRects[kCeilingLight], 0, 0);
	QSetRect(&srcRects[kLightBulb], 0, 0, 16, 28);
	QOffsetRect(&srcRects[kLightBulb], 0, 20);
	QSetRect(&srcRects[kTableLamp], 0, 0, 48, 70);
	QOffsetRect(&srcRects[kTableLamp], 16, 20);
	QSetRect(&srcRects[kHipLamp], 0, 0, 72, 276);
	QSetRect(&srcRects[kDecoLamp], 0, 0, 64, 212);
	QSetRect(&srcRects[kFlourescent], 0, 0, 64, 12);
	QSetRect(&srcRects[kTrackLight], 0, 0, 64, 24);
	QSetRect(&srcRects[kInvisLight], 0, 0, 16, 16);

	QSetRect(&srcRects[kShredder], 0, 0, 73, 22);			// Appliances
	QSetRect(&srcRects[kToaster], 0, 0, 48, 27);
	QOffsetRect(&srcRects[kToaster], 0, 22);
	QSetRect(&srcRects[kMacPlus], 0, 0, 48, 58);
	QOffsetRect(&srcRects[kMacPlus], 0, 49);
	QSetRect(&srcRects[kGuitar], 0, 0, 64, 172);
	QSetRect(&srcRects[kTV], 0, 0, 92, 77);
	QSetRect(&srcRects[kCoffee], 0, 0, 43, 64);
	QOffsetRect(&srcRects[kCoffee], 0, 107);
	QSetRect(&srcRects[kOutlet], 0, 0, 16, 24);
	QOffsetRect(&srcRects[kOutlet], 64, 22);
	QSetRect(&srcRects[kVCR], 0, 0, 96, 22);
	QSetRect(&srcRects[kStereo], 0, 0, 128, 53);
	QSetRect(&srcRects[kMicrowave], 0, 0, 92, 59);
	QSetRect(&srcRects[kCinderBlock], 0, 0, 40, 62);
	QSetRect(&srcRects[kFlowerBox], 0, 0, 80, 32);
	QSetRect(&srcRects[kCDs], 0, 0, 16, 30);
	QOffsetRect(&srcRects[kCDs], 48, 22);
	QSetRect(&srcRects[kCustomPict], 0, 0, 72, 34);

	QSetRect(&srcRects[kBalloon], 0, 0, 24, 30);			// Enemies
	QSetRect(&srcRects[kCopterLf], 0, 0, 32, 30);
	QSetRect(&srcRects[kCopterRt], 0, 0, 32, 30);
	QSetRect(&srcRects[kDartLf], 0, 0, 64, 19);
	QSetRect(&srcRects[kDartRt], 0, 0, 64, 19);
	QSetRect(&srcRects[kBall], 0, 0, 32, 32);
	QSetRect(&srcRects[kDrip], 0, 0, 16, 12);
	QSetRect(&srcRects[kFish], 0, 0, 36, 33);
	QSetRect(&srcRects[kCobweb], 0, 0, 54, 45);

	QSetRect(&srcRects[kOzma], 0, 0, 102, 92);				// Clutter
	QSetRect(&srcRects[kMirror], 0, 0, 64, 64);
	QSetRect(&srcRects[kMousehole], 0, 0, 10, 11);
	QSetRect(&srcRects[kFireplace], 0, 0, 180, 142);
	QSetRect(&srcRects[kWallWindow], 0, 0, 64, 80);
	QSetRect(&srcRects[kBear], 0, 0, 56, 58);
	QSetRect(&srcRects[kCalendar], 0, 0, 63, 92);
	QSetRect(&srcRects[kVase1], 0, 0, 36, 45);
	QSetRect(&srcRects[kVase2], 0, 0, 35, 57);
	QSetRect(&srcRects[kBulletin], 0, 0, 80, 58);
	QSetRect(&srcRects[kCloud], 0, 0, 128, 30);
	QSetRect(&srcRects[kFaucet], 0, 0, 56, 18);
	QOffsetRect(&srcRects[kFaucet], 0, 51);
	QSetRect(&srcRects[kRug], 0, 0, 144, 18);
	QSetRect(&srcRects[kChimes], 0, 0, 28, 74);
}

