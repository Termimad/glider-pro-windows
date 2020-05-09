
//============================================================================
//----------------------------------------------------------------------------
//								  HighScores.c
//----------------------------------------------------------------------------
//============================================================================


//#include <Folders.h>
//#include <NumberFormatting.h>
//#include <Script.h>
//#include <Sound.h>
//#include <StringCompare.h>
#define _CRT_SECURE_NO_WARNINGS
#include "Macintosh.h"
#include "ByteIO.h"
#include "DialogUtils.h"
#include "Externs.h"
#include "Environ.h"
#include "House.h"
#include "MainWindow.h"
#include "RectUtils.h"
#include "ResourceIDs.h"
#include "Utilities.h"


#define kHighScoresPictID		1994
#define kHighScoresMaskID		1998
#define kHighNameItem			2
#define kNameNCharsItem			5
#define kHighBannerItem			2
#define kBannerScoreNCharsItem	5


void DrawHighScores (void);
void UpdateNameDialog (DialogPtr);
Boolean NameFilter (DialogPtr, EventRecord *, SInt16 *);
void GetHighScoreName (SInt16);
void UpdateBannerDialog (DialogPtr);
Boolean BannerFilter (DialogPtr, EventRecord *, SInt16 *);
void GetHighScoreBanner (void);
Boolean FindHighScoresFolder (LPWSTR, DWORD);
Boolean GetHighScoresFilePath (LPWSTR, DWORD, StringPtr);


Str31		highBanner;
Str15		highName;
SInt16		lastHighScore;
Boolean		keyStroke;

extern	SInt16		splashOriginH, splashOriginV;
extern	Boolean		quickerTransitions, resumedSavedGame;


//==============================================================  Functions
//--------------------------------------------------------------  DoHighScores
// Handles fading in and cleaning up the high scores screen.

void DoHighScores (void)
{
	Rect		tempRect;

	SpinCursor(3);
	//SetPort((GrafPtr)workSrcMap);
	Mac_PaintRect(workSrcMap, &workSrcRect, GetStockObject(BLACK_BRUSH));
	QSetRect(&tempRect, 0, 0, 640, 480);
	QOffsetRect(&tempRect, splashOriginH, splashOriginV);
	LoadScaledGraphic(workSrcMap, kStarPictID, &tempRect);
	if (quickerTransitions)
		DissBitsChunky(&workSrcRect);
	else
		DissBits(&workSrcRect);
	SpinCursor(3);
	//SetPort((GrafPtr)workSrcMap);
	DrawHighScores();
	SpinCursor(3);
	if (quickerTransitions)
		DissBitsChunky(&workSrcRect);
	else
		DissBits(&workSrcRect);
	//InitCursor();
	DelayTicks(60);
	WaitForInputEvent(30);

	RedrawSplashScreen();
}

//--------------------------------------------------------------  DrawHighScores
// Draws the actual scores on the screen.

#define kScoreSpacing			18
#define kScoreWide				352
#define kKimsLifted				4

void DrawHighScores (void)
{
	HDC			tempMap, tempMask;
	OSErr		theErr;
	Rect		tempRect, tempRect2;
	Str255		tempStr;
	SInt16		scoreLeft, bannerWidth, i, dropIt;
	INT			hOffset, vOffset;
	LOGFONT		theLogFont;
	HFONT		theFont;
	COLORREF	wasColor;

	theLogFont.lfHeight = 0;
	theLogFont.lfWidth = 0;
	theLogFont.lfEscapement = 0;
	theLogFont.lfOrientation = 0;
	theLogFont.lfWeight = FW_NORMAL;
	theLogFont.lfItalic = FALSE;
	theLogFont.lfUnderline = FALSE;
	theLogFont.lfStrikeOut = FALSE;
	theLogFont.lfCharSet = DEFAULT_CHARSET;
	theLogFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	theLogFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	theLogFont.lfQuality = DEFAULT_QUALITY;
	theLogFont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	wcscpy(theLogFont.lfFaceName, L"Tahoma");

	scoreLeft = ((thisMac.screen.right - thisMac.screen.left) - kScoreWide) / 2;
	dropIt = 129 + splashOriginV;

	QSetRect(&tempRect, 0, 0, 332, 30);
	theErr = CreateOffScreenGWorld(&tempMap, &tempRect, kPreferredDepth);
	LoadGraphic(tempMap, kHighScoresPictID);

	theErr = CreateOffScreenGWorld(&tempMask, &tempRect, 1);
	LoadGraphic(tempMask, kHighScoresMaskID);

	tempRect2 = tempRect;
	QOffsetRect(&tempRect2, scoreLeft + (kScoreWide - 332) / 2, dropIt - 60);

	Mac_CopyMask(tempMap, tempMask, workSrcMap,
			&tempRect, &tempRect, &tempRect2);

	DisposeGWorld(tempMap);
	DisposeGWorld(tempMask);

	theLogFont.lfHeight = -14;
	theLogFont.lfWeight = FW_BOLD;
	theFont = CreateFontIndirect(&theLogFont);

	SaveDC(workSrcMap);
	SelectObject(workSrcMap, theFont);
	PasStringCopyC("\xA5 ", tempStr); // "• "
	PasStringConcat(tempStr, thisHouseName);
	PasStringConcatC(tempStr, " \xA5"); // " •"
	hOffset = scoreLeft + ((kScoreWide - Mac_StringWidth(workSrcMap, tempStr)) / 2);
	vOffset = dropIt - 65;
	MoveToEx(workSrcMap, hOffset - 1, vOffset - 1, NULL);
	SetTextColor(workSrcMap, blackColor);
	Mac_DrawString(workSrcMap, tempStr);
	MoveToEx(workSrcMap, hOffset, vOffset, NULL);
	SetTextColor(workSrcMap, cyanColor);
	Mac_DrawString(workSrcMap, tempStr);
	RestoreDC(workSrcMap, -1);
	DeleteObject(theFont);

	theLogFont.lfHeight = -12;
	theLogFont.lfWeight = FW_BOLD;
	theFont = CreateFontIndirect(&theLogFont);
	SaveDC(workSrcMap);
	SelectObject(workSrcMap, theFont);
													// message for score #1
	PasStringCopy(thisHouse->highScores.banner, tempStr);
	bannerWidth = Mac_StringWidth(workSrcMap, tempStr);
	hOffset = scoreLeft + (kScoreWide - bannerWidth) / 2;
	vOffset = dropIt - kKimsLifted;
	SetTextColor(workSrcMap, blackColor);
	MoveToEx(workSrcMap, hOffset, vOffset, NULL);
	Mac_DrawString(workSrcMap, tempStr);
	SetTextColor(workSrcMap, yellowColor);
	MoveToEx(workSrcMap, hOffset, vOffset - 1, NULL);
	Mac_DrawString(workSrcMap, tempStr);

	QSetRect(&tempRect, 0, 0, bannerWidth + 8, kScoreSpacing);
	QOffsetRect(&tempRect, scoreLeft - 3 + (kScoreWide - bannerWidth) / 2,
			dropIt + 5 - kScoreSpacing - kKimsLifted);
	wasColor = SetDCBrushColor(workSrcMap, blackColor);
	Mac_FrameRect(workSrcMap, &tempRect, GetStockObject(DC_BRUSH), 1, 1);
	QOffsetRect(&tempRect, -1, -1);
	SetDCBrushColor(workSrcMap, yellowColor);
	Mac_FrameRect(workSrcMap, &tempRect, GetStockObject(DC_BRUSH), 1, 1);
	SetDCBrushColor(workSrcMap, wasColor);

	for (i = 0; i < kMaxScores; i++)
	{
		if (thisHouse->highScores.scores[i] > 0L)
		{
			SpinCursor(1);
			Mac_NumToString(i + 1L, tempStr);		// draw placing number
			SetTextColor(workSrcMap, blackColor);
			if (i == 0)
				MoveToEx(workSrcMap, scoreLeft + 1, dropIt - kScoreSpacing - kKimsLifted, NULL);
			else
				MoveToEx(workSrcMap, scoreLeft + 1, dropIt + (i * kScoreSpacing), NULL);
			Mac_DrawString(workSrcMap, tempStr);
			if (i == lastHighScore)
				SetTextColor(workSrcMap, whiteColor);
			else
				SetTextColor(workSrcMap, cyanColor);
			if (i == 0)
				MoveToEx(workSrcMap, scoreLeft + 0, dropIt - 1 - kScoreSpacing - kKimsLifted, NULL);
			else
				MoveToEx(workSrcMap, scoreLeft + 0, dropIt - 1 + (i * kScoreSpacing), NULL);
			Mac_DrawString(workSrcMap, tempStr);
													// draw high score name
			PasStringCopy(thisHouse->highScores.names[i], tempStr);
			SetTextColor(workSrcMap, blackColor);
			if (i == 0)
				MoveToEx(workSrcMap, scoreLeft + 31, dropIt - kScoreSpacing - kKimsLifted, NULL);
			else
				MoveToEx(workSrcMap, scoreLeft + 31, dropIt + (i * kScoreSpacing), NULL);
			Mac_DrawString(workSrcMap, tempStr);
			if (i == lastHighScore)
				SetTextColor(workSrcMap, whiteColor);
			else
				SetTextColor(workSrcMap, yellowColor);
			if (i == 0)
				MoveToEx(workSrcMap, scoreLeft + 30, dropIt - 1 - kScoreSpacing - kKimsLifted, NULL);
			else
				MoveToEx(workSrcMap, scoreLeft + 30, dropIt - 1 + (i * kScoreSpacing), NULL);
			Mac_DrawString(workSrcMap, tempStr);
													// draw level number
			Mac_NumToString(thisHouse->highScores.levels[i], tempStr);
			SetTextColor(workSrcMap, blackColor);
			if (i == 0)
				MoveToEx(workSrcMap, scoreLeft + 161, dropIt - kScoreSpacing - kKimsLifted, NULL);
			else
				MoveToEx(workSrcMap, scoreLeft + 161, dropIt + (i * kScoreSpacing), NULL);
			Mac_DrawString(workSrcMap, tempStr);
			if (i == lastHighScore)
				SetTextColor(workSrcMap, whiteColor);
			else
				SetTextColor(workSrcMap, yellowColor);
			if (i == 0)
				MoveToEx(workSrcMap, scoreLeft + 160, dropIt - 1 - kScoreSpacing - kKimsLifted, NULL);
			else
				MoveToEx(workSrcMap, scoreLeft + 160, dropIt - 1 + (i * kScoreSpacing), NULL);
			Mac_DrawString(workSrcMap, tempStr);
													// draw word "rooms"
			if (thisHouse->highScores.levels[i] == 1)
				GetLocalizedString(6, tempStr);
			else
				GetLocalizedString(7, tempStr);
			SetTextColor(workSrcMap, blackColor);
			if (i == 0)
				MoveToEx(workSrcMap, scoreLeft + 193, dropIt - kScoreSpacing - kKimsLifted, NULL);
			else
				MoveToEx(workSrcMap, scoreLeft + 193, dropIt + (i * kScoreSpacing), NULL);
			Mac_DrawString(workSrcMap, tempStr);
			SetTextColor(workSrcMap, cyanColor);
			if (i == 0)
				MoveToEx(workSrcMap, scoreLeft + 192, dropIt - 1 - kScoreSpacing - kKimsLifted, NULL);
			else
				MoveToEx(workSrcMap, scoreLeft + 192, dropIt - 1 + (i * kScoreSpacing), NULL);
			Mac_DrawString(workSrcMap, tempStr);
													// draw high score points
			Mac_NumToString(thisHouse->highScores.scores[i], tempStr);
			SetTextColor(workSrcMap, blackColor);
			if (i == 0)
				MoveToEx(workSrcMap, scoreLeft + 291, dropIt - kScoreSpacing - kKimsLifted, NULL);
			else
				MoveToEx(workSrcMap, scoreLeft + 291, dropIt + (i * kScoreSpacing), NULL);
			Mac_DrawString(workSrcMap, tempStr);
			if (i == lastHighScore)
				SetTextColor(workSrcMap, whiteColor);
			else
				SetTextColor(workSrcMap, yellowColor);
			if (i == 0)
				MoveToEx(workSrcMap, scoreLeft + 290, dropIt - 1 - kScoreSpacing - kKimsLifted, NULL);
			else
				MoveToEx(workSrcMap, scoreLeft + 290, dropIt - 1 + (i * kScoreSpacing), NULL);
			Mac_DrawString(workSrcMap, tempStr);
		}
	}

	RestoreDC(workSrcMap, -1);
	DeleteObject(theFont);

	theLogFont.lfHeight = -9;
	theLogFont.lfWeight = FW_BOLD;
	theFont = CreateFontIndirect(&theLogFont);
	SaveDC(workSrcMap);
	SelectObject(workSrcMap, theFont);
	SetTextColor(workSrcMap, blueColor);
	MoveToEx(workSrcMap, scoreLeft + 80, dropIt - 1 + (10 * kScoreSpacing), NULL);
	GetLocalizedString(8, tempStr);
	Mac_DrawString(workSrcMap, tempStr);
	RestoreDC(workSrcMap, -1);
	DeleteObject(theFont);
}

//--------------------------------------------------------------  SortHighScores
// This does a simple sort of the high scores.

void SortHighScores (void)
{
	scoresType	tempScores;
	SInt32		greatest;
	SInt16		i, h, which;

	for (h = 0; h < kMaxScores; h++)
	{
		greatest = -1L;
		which = -1;
		for (i = 0; i < kMaxScores; i++)
		{
			if (thisHouse->highScores.scores[i] > greatest)
			{
				greatest = thisHouse->highScores.scores[i];
				which = i;
			}
		}
		if (which != -1)
		{
			PasStringCopy(thisHouse->highScores.names[which], tempScores.names[h]);
			tempScores.scores[h] = thisHouse->highScores.scores[which];
			tempScores.timeStamps[h] = thisHouse->highScores.timeStamps[which];
			tempScores.levels[h] = thisHouse->highScores.levels[which];
			thisHouse->highScores.scores[which] = -1L;
		}
	}
	PasStringCopy(thisHouse->highScores.banner, tempScores.banner);
	thisHouse->highScores = tempScores;
}

//--------------------------------------------------------------  ZeroHighScores
// This funciton goes through and resets or "zeros" all high scores.

void ZeroHighScores (void)
{
	SInt16		i;

	PasStringCopy(thisHouseName, thisHouse->highScores.banner);
	for (i = 0; i < kMaxScores; i++)
	{
		PasStringCopyC("--------------", thisHouse->highScores.names[i]);
		thisHouse->highScores.scores[i] = 0L;
		thisHouse->highScores.timeStamps[i] = 0L;
		thisHouse->highScores.levels[i] = 0;
	}
}

//--------------------------------------------------------------  ZeroAllButHighestScore
// Like the above, but this function preserves the highest score.

void ZeroAllButHighestScore (void)
{
	SInt16		i;

	for (i = 1; i < kMaxScores; i++)
	{
		PasStringCopyC("--------------", thisHouse->highScores.names[i]);
		thisHouse->highScores.scores[i] = 0L;
		thisHouse->highScores.timeStamps[i] = 0L;
		thisHouse->highScores.levels[i] = 0;
	}
}

//--------------------------------------------------------------  TestHighScore
// This function is called after a game ends in order to test the…
// current high score against the high score list.  It returns true…
// if the player is on the high score list now.

Boolean TestHighScore (void)
{
	SInt16		placing, i;

	if (resumedSavedGame)
		return (false);

	lastHighScore = -1;
	placing = -1;

	for (i = 0; i < kMaxScores; i++)
	{
		if (theScore > thisHouse->highScores.scores[i])
		{
			placing = i;
			lastHighScore = i;
			break;
		}
	}

	if (placing != -1)
	{
		//FlushEvents(everyEvent, 0);
		GetHighScoreName(placing + 1);
		PasStringCopy(highName, thisHouse->highScores.names[kMaxScores - 1]);
		if (placing == 0)
		{
			GetHighScoreBanner();
			PasStringCopy(highBanner, thisHouse->highScores.banner);
		}
		thisHouse->highScores.scores[kMaxScores - 1] = theScore;
		Mac_GetDateTime(&thisHouse->highScores.timeStamps[kMaxScores - 1]);
		thisHouse->highScores.levels[kMaxScores - 1] = CountRoomsVisited();
		SortHighScores();
		gameDirty = true;
	}

	if (placing != -1)
	{
		DoHighScores();
		return (true);
	}
	else
		return (false);
}

//--------------------------------------------------------------  UpdateNameDialog
// Redraws the "Enter High Score Name" dialog.

void UpdateNameDialog (DialogPtr theDialog)
{
	return;
#if 0
	short		nChars;

	DrawDialog(theDialog);
	DrawDefaultButton(theDialog);

	nChars = GetDialogStringLen(theDialog, kHighNameItem);
	SetDialogNumToStr(theDialog, kNameNCharsItem, (long)nChars);
#endif
}

//--------------------------------------------------------------  NameFilter
// Dialog filter for the "Enter High Score Name" dialog.

Boolean NameFilter (DialogPtr dial, EventRecord *event, SInt16 *item)
{
	return (false);
#if 0
	short		nChars;

	if (keyStroke)
	{
		nChars = GetDialogStringLen(dial, kHighNameItem);
		SetDialogNumToStr(dial, kNameNCharsItem, (long)nChars);
		keyStroke = false;
	}

	switch (event->what)
	{
		case keyDown:
		keyStroke = true;
		switch ((event->message) & charCodeMask)
		{
			case kReturnKeyASCII:
			case kEnterKeyASCII:
			PlayPrioritySound(kCarriageSound, kCarriagePriority);
			FlashDialogButton(dial, kOkayButton);
			*item = kOkayButton;
			return(true);
			break;

			case kTabKeyASCII:
			SelectDialogItemText(dial, kHighNameItem, 0, 1024);
			return(false);
			break;

			default:
			PlayPrioritySound(kTypingSound, kTypingPriority);
			return(false);
		}
		break;

		case updateEvt:
		BeginUpdate(GetDialogWindow(dial));
		UpdateNameDialog(dial);
		EndUpdate(GetDialogWindow(dial));
		event->what = nullEvent;
		return(false);
		break;

		default:
		return(false);
		break;
	}
#endif
}

//--------------------------------------------------------------  GetHighScoreName
// Brings up a dialog to get player's name (due to a high score).

void GetHighScoreName (SInt16 place)
{
	MessageBox(mainWindow, L"GetHighScoreName()", NULL, MB_ICONHAND);
	PasStringCopyC("HighScoreName", highName);
	return;
#if 0
	DialogPtr		theDial;
	Str255			scoreStr, placeStr, tempStr;
	short			item;
	Boolean			leaving;
	ModalFilterUPP	nameFilterUPP;

	nameFilterUPP = NewModalFilterUPP(NameFilter);

	InitCursor();
	NumToString(theScore, scoreStr);
	NumToString((long)place, placeStr);
	ParamText(scoreStr, placeStr, thisHouseName, "\p");
	PlayPrioritySound(kEnergizeSound, kEnergizePriority);
	BringUpDialog(&theDial, kHighNameDialogID);
	FlushEvents(everyEvent, 0);
	SetDialogString(theDial, kHighNameItem, highName);
	SelectDialogItemText(theDial, kHighNameItem, 0, 1024);
	leaving = false;

	while (!leaving)
	{
		ModalDialog(nameFilterUPP, &item);

		if (item == kOkayButton)
		{
			GetDialogString(theDial, kHighNameItem, tempStr);
			PasStringCopyNum(tempStr, highName, 15);
			leaving = true;
		}
	}

	DisposeDialog(theDial);
	DisposeModalFilterUPP(nameFilterUPP);
#endif
}

//--------------------------------------------------------------  UpdateBannerDialog
// Redraws the "Enter Message" dialog.

void UpdateBannerDialog (DialogPtr theDialog)
{
	return;
#if 0
	short		nChars;

	DrawDialog(theDialog);
	DrawDefaultButton(theDialog);

	nChars = GetDialogStringLen(theDialog, kHighBannerItem);
	SetDialogNumToStr(theDialog, kBannerScoreNCharsItem, (long)nChars);
#endif
}

//--------------------------------------------------------------  BannerFilter
// Dialog filter for the "Enter Message" dialog.

Boolean BannerFilter (DialogPtr dial, EventRecord *event, SInt16 *item)
{
	return (false);
#if 0
	short		nChars;

	if (keyStroke)
	{
		nChars = GetDialogStringLen(dial, kHighBannerItem);
		SetDialogNumToStr(dial, kBannerScoreNCharsItem, (long)nChars);
		keyStroke = false;
	}

	switch (event->what)
	{

		case keyDown:
		keyStroke = true;
		switch ((event->message) & charCodeMask)
		{
			case kReturnKeyASCII:
			case kEnterKeyASCII:
			PlayPrioritySound(kCarriageSound, kCarriagePriority);
			FlashDialogButton(dial, kOkayButton);
			*item = kOkayButton;
			return(true);
			break;

			case kTabKeyASCII:
			SelectDialogItemText(dial, kHighBannerItem, 0, 1024);
			return(false);
			break;

			default:
			PlayPrioritySound(kTypingSound, kTypingPriority);
			return(false);
		}
		break;

		case updateEvt:
		BeginUpdate(GetDialogWindow(dial));
		UpdateBannerDialog(dial);
		EndUpdate(GetDialogWindow(dial));
		event->what = nullEvent;
		return(false);
		break;

		default:
		return(false);
		break;
	}
#endif
}

//--------------------------------------------------------------  GetHighScoreBanner
// A player who gets the #1 slot gets to enter a short message (that…
// appears across the top of the high scores list).  This dialog…
// gets that message.

void GetHighScoreBanner (void)
{
	MessageBox(mainWindow, L"GetHighScoreBanner()", NULL, MB_ICONHAND);
	PasStringCopyC("HighScoreBanner", highBanner);
	return;
#if 0
	DialogPtr		theDial;
	Str255			tempStr;
	short			item;
	Boolean			leaving;
	ModalFilterUPP	bannerFilterUPP;

	bannerFilterUPP = NewModalFilterUPP(BannerFilter);

	PlayPrioritySound(kEnergizeSound, kEnergizePriority);
	BringUpDialog(&theDial, kHighBannerDialogID);
	SetDialogString(theDial, kHighBannerItem, highBanner);
	SelectDialogItemText(theDial, kHighBannerItem, 0, 1024);
	leaving = false;

	while (!leaving)
	{
		ModalDialog(bannerFilterUPP, &item);

		if (item == kOkayButton)
		{
			GetDialogString(theDial, kHighBannerItem, tempStr);
			PasStringCopyNum(tempStr, highBanner, 31);
			leaving = true;
		}
	}

	DisposeDialog(theDial);
	DisposeModalFilterUPP(bannerFilterUPP);
#endif
}

//--------------------------------------------------------------  FindHighScoresFolder

Boolean FindHighScoresFolder (LPWSTR scoresDirPath, DWORD cchDirPath)
{
	// This used to search the system preferences folder for a folder
	// named "G-PRO Scores ƒ". Now it just uses the "Scores" folder under
	// "%APPDATA%\glider-pro-windows" or next to the executable (depending
	// on whether "portable.dat" exists next to the executable or not.
	// (See the GetDataFolderPath function.)

	WCHAR pathBuffer[MAX_PATH];
	HRESULT hr;

	if (!GetDataFolderPath(pathBuffer, ARRAYSIZE(pathBuffer)))
		return false;
	hr = StringCchCat(pathBuffer, ARRAYSIZE(pathBuffer), L"\\Scores");
	if (FAILED(hr))
		return false;
	if (!CreateDirectory(pathBuffer, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
	{
		Str255 label;
		PasStringCopyC("Prefs Folder", label);
		CheckFileError(GetLastError(), label);
		return false;
	}

	hr = StringCchCopy(scoresDirPath, cchDirPath, pathBuffer);
	return SUCCEEDED(hr);
}

//--------------------------------------------------------------  GetHighScoresFilePath

Boolean GetHighScoresFilePath (LPWSTR lpPath, DWORD cchPath, StringPtr baseName)
{
	WCHAR pathBuffer[MAX_PATH];
	WCHAR wideBaseName[256];

	if (!WinFromMacString(wideBaseName, ARRAYSIZE(wideBaseName), baseName))
		return false;
	if (!FindHighScoresFolder(pathBuffer, ARRAYSIZE(pathBuffer)))
		return false;
	if (FAILED(StringCchCat(pathBuffer, ARRAYSIZE(pathBuffer), L"\\")))
		return false;
	if (FAILED(StringCchCat(pathBuffer, ARRAYSIZE(pathBuffer), wideBaseName)))
		return false;
	if (FAILED(StringCchCat(pathBuffer, ARRAYSIZE(pathBuffer), L".gls")))
		return false;

	return SUCCEEDED(StringCchCopy(lpPath, cchPath, pathBuffer));
}

//--------------------------------------------------------------  WriteScoresToDisk

Boolean WriteScoresToDisk (void)
{
	Str255		fileLabel;
	WCHAR		pathBuffer[MAX_PATH];
	HANDLE		scoresFileHandle;
	byteio		byteWriter;
	Boolean		writeSucceeded;
	DWORD		lastError;

	PasStringCopyC("High Scores File", fileLabel);

	if (!GetHighScoresFilePath(pathBuffer, ARRAYSIZE(pathBuffer), thisHouseName))
		return false;

	scoresFileHandle = CreateFile(pathBuffer, GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (scoresFileHandle == INVALID_HANDLE_VALUE)
	{
		CheckFileError(GetLastError(), fileLabel);
		return false;
	}

	if (!byteio_init_handle_writer(&byteWriter, scoresFileHandle))
	{
		CloseHandle(scoresFileHandle);
		return false;
	}
	writeSucceeded = WriteScoresType(&byteWriter, &thisHouse->highScores);
	lastError = GetLastError();
	byteio_close(&byteWriter);
	CloseHandle(scoresFileHandle);
	if (!writeSucceeded)
	{
		CheckFileError(lastError, fileLabel);
		return false;
	}

	return true;
}

//--------------------------------------------------------------  ReadScoresFromDisk

Boolean ReadScoresFromDisk (void)
{
	scoresType	tempScores;
	Str255		fileLabel;
	WCHAR		pathBuffer[MAX_PATH];
	HANDLE		scoresFileHandle;
	byteio		byteReader;
	Boolean		readSucceeded;
	DWORD		lastError;

	PasStringCopyC("High Scores File", fileLabel);

	if (!GetHighScoresFilePath(pathBuffer, ARRAYSIZE(pathBuffer), thisHouseName))
		return false;

	scoresFileHandle = CreateFile(pathBuffer, GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	lastError = GetLastError();
	if (scoresFileHandle == INVALID_HANDLE_VALUE)
	{
		if (lastError != ERROR_FILE_NOT_FOUND)
			CheckFileError(lastError, fileLabel);
		return false;
	}

	if (!byteio_init_handle_reader(&byteReader, scoresFileHandle))
	{
		CloseHandle(scoresFileHandle);
		return false;
	}
	readSucceeded = ReadScoresType(&byteReader, &tempScores);
	lastError = GetLastError();
	byteio_close(&byteReader);
	CloseHandle(scoresFileHandle);
	if (!readSucceeded)
	{
		CheckFileError(lastError, fileLabel);
		return false;
	}

	thisHouse->highScores = tempScores;
	return true;
}

