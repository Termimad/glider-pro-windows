
//============================================================================
//----------------------------------------------------------------------------
//								  ObjectInfo.c
//----------------------------------------------------------------------------
//============================================================================


#include "DialogUtils.h"
#include "Externs.h"
#include "Macintosh.h"
#include "ObjectEdit.h"
#include "RectUtils.h"
#include "ResourceIDs.h"


#define kBlowerInitialState     1006
#define kForceCheckbox          1007
#define kBlowerArrow            1008
#define kDirectionText          1009
#define kBlowerUpButton         1011
#define kBlowerRightButton      1012
#define kBlowerDownButton       1013
#define kBlowerLeftButton       1014
#define kBlowerLinkedFrom       1015
#define kLeftFacingRadio        1016
#define kRightFacingRadio       1017

#define kFurnitureLinkedFrom    1006

#define kCustPictIDItem         1007

#define kToggleRadio            1006
#define kForceOnRadio           1007
#define kForceOffRadio          1008
#define kLinkSwitchButton       1009
#define kSwitchGotoButton       1014
#define kSwitchLinkedFrom       1015

#define kTriggerDelayItem       1006
#define kLinkTriggerButton      1009
#define kTriggerGotoButton      1014
#define kTriggerLinkedFrom      1015

#define kLightInitialState      1006
#define kLightLinkedFrom        1008

#define kInitialStateCheckbox		6
#define kDelay3Item					6
#define kDelayItem					8
#define kDelayLabelItem				9
#define k100PtRadio					6
#define k300PtRadio					7
#define k500PtRadio					8
#define kGreaseItem					6
#define kLinkTransButton			6
#define kInitialStateCheckbox3		13
#define kTransRoomText				8
#define kTransObjectText			9
#define kKillBandsCheckbox			8
#define kKillBatteryCheckbox		9
#define kKillFoilCheckbox			10
#define kDelay2Item					7
#define kDelay2LabelItem			8
#define kDelay2LabelItem2			9
#define kInitialStateCheckbox2		10
#define kRadioFlower1				6
#define kRadioFlower6				11
#define kFlowerCancel				12
#define kGotoButton1				11


void UpdateBlowerInfo (HWND, HDC);
void UpdateApplianceInfo (DialogPtr);
void UpdateMicrowaveInfo (DialogPtr);
void UpdateGreaseInfo (DialogPtr);
void UpdateInvisBonusInfo (DialogPtr);
void UpdateTransInfo (DialogPtr);
void UpdateEnemyInfo (DialogPtr);
void UpdateFlowerInfo (DialogPtr);
INT_PTR CALLBACK BlowerFilter (HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK FurnitureFilter (HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK CustPictFilter (HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK SwitchFilter (HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK TriggerFilter (HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK LightFilter (HWND, UINT, WPARAM, LPARAM);
Boolean ApplianceFilter (DialogPtr, EventRecord *, SInt16 *);
Boolean MicrowaveFilter (DialogPtr, EventRecord *, SInt16 *);
Boolean GreaseFilter (DialogPtr, EventRecord *, SInt16 *);
Boolean InvisBonusFilter (DialogPtr, EventRecord *, SInt16 *);
Boolean TransFilter (DialogPtr, EventRecord *, SInt16 *);
Boolean EnemyFilter (DialogPtr, EventRecord *, SInt16 *);
Boolean FlowerFilter (DialogPtr, EventRecord *, SInt16 *);
void DoBlowerObjectInfo (HWND);
void DoFurnitureObjectInfo (HWND);
void DoCustPictObjectInfo (HWND);
void DoSwitchObjectInfo (HWND);
void DoTriggerObjectInfo (HWND);
void DoLightObjectInfo (HWND);
void DoApplianceObjectInfo (HWND, SInt16);
void DoMicrowaveObjectInfo (HWND);
void DoGreaseObjectInfo (HWND);
void DoInvisBonusObjectInfo (HWND);
void DoTransObjectInfo (HWND, SInt16);
void DoEnemyObjectInfo (HWND, SInt16);
void DoFlowerObjectInfo (HWND);


SInt16		newPoint;

extern	retroLink	retroLinkList[];
extern	SInt16		linkRoom, linkType, wasFlower;
extern	Byte		linkObject;
extern	Boolean		linkerIsSwitch;


#ifndef COMPILEDEMO

//==============================================================  Functions
//--------------------------------------------------------------  GetObjectName

void GetObjectName (wchar_t *buffer, int size, SInt16 objectType)
{
	UINT strID;
	LPWSTR strPtr;
	int strLen;

	if (buffer == NULL || size < 1)
		return;
	buffer[0] = L'\0';

	strID = (UINT)(kObjectNameStringsBase + objectType);
	strLen = LoadString(HINST_THISCOMPONENT, strID, (LPWSTR)&strPtr, 0);
	if (strLen == 0 || strPtr == NULL)
		return;
	StringCchCopyN(buffer, size, strPtr, strLen);
}

//--------------------------------------------------------------  UpdateBlowerInfo

#define kArrowheadLength 4

void UpdateBlowerInfo (HWND hDlg, HDC hdc)
{
	RECT bounds;
	LONG centerHori, centerVert;
	HPEN hPen, oldPen;
	HWND focusedWindow;

	if ((thisRoom->objects[objActive].what != kLeftFan) &&
		(thisRoom->objects[objActive].what != kRightFan))
	{
		GetWindowRect(GetDlgItem(hDlg, kBlowerArrow), &bounds);
		MapWindowPoints(HWND_DESKTOP, hDlg, (POINT *)&bounds, 2);

		FillRect(hdc, &bounds, GetSysColorBrush(COLOR_BTNFACE));

		// The bounds are deflated here to allow a focus rectangle to be drawn
		// later, when it is appropriate to do so, without the focus rectangle
		// overlapping the arrow.
		InflateRect(&bounds, -1, -1);
		centerHori = bounds.left + (bounds.right - bounds.left) / 2;
		centerVert = bounds.top + (bounds.bottom - bounds.top) / 2;

		hPen = CreatePen(PS_SOLID, 2, GetSysColor(COLOR_WINDOWTEXT));
		oldPen = SelectObject(hdc, hPen);

		// Draw an arrow pointing in the currently selected direction
		if (IsDlgButtonChecked(hDlg, kBlowerUpButton))
		{
			MoveToEx(hdc, centerHori, bounds.top, NULL);
			LineTo(hdc, centerHori, bounds.bottom - 1);
			MoveToEx(hdc, centerHori, bounds.top + 1, NULL);
			LineTo(hdc, centerHori + kArrowheadLength, bounds.top + 1 + kArrowheadLength);
			MoveToEx(hdc, centerHori - 1, bounds.top + 1, NULL);
			LineTo(hdc, centerHori - 1 - kArrowheadLength, bounds.top + 1 + kArrowheadLength);
		}
		else if (IsDlgButtonChecked(hDlg, kBlowerRightButton))
		{
			MoveToEx(hdc, bounds.right - 1, centerVert, NULL);
			LineTo(hdc, bounds.left, centerVert);
			MoveToEx(hdc, bounds.right - 2, centerVert, NULL);
			LineTo(hdc, bounds.right - 2 - kArrowheadLength, centerVert + kArrowheadLength);
			MoveToEx(hdc, bounds.right - 1, centerVert, NULL);
			LineTo(hdc, bounds.right - 2 - kArrowheadLength, centerVert - 1 - kArrowheadLength);
		}
		else if (IsDlgButtonChecked(hDlg, kBlowerDownButton))
		{
			MoveToEx(hdc, centerHori, bounds.top, NULL);
			LineTo(hdc, centerHori, bounds.bottom - 1);
			MoveToEx(hdc, centerHori, bounds.bottom - 2, NULL);
			LineTo(hdc, centerHori + kArrowheadLength, bounds.bottom - 2 - kArrowheadLength);
			MoveToEx(hdc, centerHori - 1, bounds.bottom - 2, NULL);
			LineTo(hdc, centerHori - 1 - kArrowheadLength, bounds.bottom - 2 - kArrowheadLength);
		}
		else if (IsDlgButtonChecked(hDlg, kBlowerLeftButton))
		{
			MoveToEx(hdc, bounds.left, centerVert, NULL);
			LineTo(hdc, bounds.right - 1, centerVert);
			MoveToEx(hdc, bounds.left, centerVert, NULL);
			LineTo(hdc, bounds.left + 1 + kArrowheadLength, centerVert - 1 - kArrowheadLength);
			MoveToEx(hdc, bounds.left + 1, centerVert, NULL);
			LineTo(hdc, bounds.left + 1 + kArrowheadLength, centerVert + kArrowheadLength);
		}

		SelectObject(hdc, oldPen);
		DeleteObject(hPen);

		// Draw the focus rectangle around the custom-drawn arrow, so that
		// the direction option buttons look like they are part of a single
		// control.
		InflateRect(&bounds, 1, 1);
		focusedWindow = GetFocus();
		if ((GetDlgItem(hDlg, kBlowerUpButton) == focusedWindow) ||
				(GetDlgItem(hDlg, kBlowerRightButton) == focusedWindow) ||
				(GetDlgItem(hDlg, kBlowerDownButton) == focusedWindow) ||
				(GetDlgItem(hDlg, kBlowerLeftButton) == focusedWindow))
		{
			if ((SendMessage(hDlg, WM_QUERYUISTATE, 0, 0) & UISF_HIDEFOCUS) == 0)
			{
				DrawFocusRect(hdc, &bounds);
			}
		}
	}
}

//--------------------------------------------------------------  UpdateApplianceInfo

void UpdateApplianceInfo (DialogPtr theDialog)
{
	return;
#if 0
	DrawDialog(theDialog);
	DrawDefaultButton(theDialog);
	FrameDialogItemC(theDialog, 5, kRedOrangeColor8);
#endif
}

//--------------------------------------------------------------  UpdateMicrowaveInfo

void UpdateMicrowaveInfo (DialogPtr theDialog)
{
	return;
#if 0
	DrawDialog(theDialog);
	DrawDefaultButton(theDialog);
	FrameDialogItemC(theDialog, 5, kRedOrangeColor8);
#endif
}

//--------------------------------------------------------------  UpdateGreaseInfo

void UpdateGreaseInfo (DialogPtr theDialog)
{
	return;
#if 0
	DrawDialog(theDialog);
	DrawDefaultButton(theDialog);
	FrameDialogItemC(theDialog, 5, kRedOrangeColor8);
#endif
}

//--------------------------------------------------------------  UpdateInvisBonusInfo

void UpdateInvisBonusInfo (DialogPtr theDialog)
{
	return;
#if 0
	DrawDialog(theDialog);
	DrawDefaultButton(theDialog);
	SelectFromRadioGroup(theDialog, newPoint + k100PtRadio,
			k100PtRadio, k500PtRadio);
	FrameDialogItemC(theDialog, 4, kRedOrangeColor8);
#endif
}

//--------------------------------------------------------------  UpdateTransInfo

void UpdateTransInfo (DialogPtr theDialog)
{
	return;
#if 0
	DrawDialog(theDialog);
	DrawDefaultButton(theDialog);
	FrameDialogItemC(theDialog, 4, kRedOrangeColor8);
	FrameDialogItemC(theDialog, 10, kRedOrangeColor8);
#endif
}

//--------------------------------------------------------------  UpdateEnemyInfo

void UpdateEnemyInfo (DialogPtr theDialog)
{
	return;
#if 0
	DrawDialog(theDialog);
	DrawDefaultButton(theDialog);
	FrameDialogItemC(theDialog, 4, kRedOrangeColor8);
#endif
}

//--------------------------------------------------------------  UpdateFlowerInfo

void UpdateFlowerInfo (DialogPtr theDialog)
{
	return;
#if 0
	DrawDialog(theDialog);
	DrawDefaultButton(theDialog);
	FrameDialogItemC(theDialog, 4, kRedOrangeColor8);
#endif
}

//--------------------------------------------------------------  BlowerFilter

INT_PTR CALLBACK BlowerFilter (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SInt16 what = thisRoom->objects[objActive].what;

		if (thisRoom->objects[objActive].data.a.initial)
			CheckDlgButton(hDlg, kBlowerInitialState, BST_CHECKED);
		else
			CheckDlgButton(hDlg, kBlowerInitialState, BST_UNCHECKED);

		if ((what == kTaper) || (what == kCandle) || (what == kStubby) ||
				(what == kTiki) || (what == kBBQ))
		{
			ShowWindow(GetDlgItem(hDlg, kBlowerInitialState), SW_HIDE);
		}

		if ((what == kLeftFan) || (what == kRightFan))
		{
			if (what == kLeftFan)
			{
				CheckRadioButton(hDlg, kLeftFacingRadio, kRightFacingRadio,
						kLeftFacingRadio);
			}
			else
			{
				CheckRadioButton(hDlg, kLeftFacingRadio, kRightFacingRadio,
						kRightFacingRadio);
			}
			ShowWindow(GetDlgItem(hDlg, kDirectionText), SW_HIDE);
		}
		else
		{
			ShowWindow(GetDlgItem(hDlg, kLeftFacingRadio), SW_HIDE);
			ShowWindow(GetDlgItem(hDlg, kRightFacingRadio), SW_HIDE);
		}

		if ((thisRoom->objects[objActive].data.a.vector & 0x01) == 0x01)
		{
			CheckRadioButton(hDlg, kBlowerUpButton, kBlowerLeftButton,
					kBlowerUpButton);
		}
		else if ((thisRoom->objects[objActive].data.a.vector & 0x02) == 0x02)
		{
			CheckRadioButton(hDlg, kBlowerUpButton, kBlowerLeftButton,
					kBlowerRightButton);
		}
		else if ((thisRoom->objects[objActive].data.a.vector & 0x04) == 0x04)
		{
			CheckRadioButton(hDlg, kBlowerUpButton, kBlowerLeftButton,
					kBlowerDownButton);
		}
		else if ((thisRoom->objects[objActive].data.a.vector & 0x08) == 0x08)
		{
			CheckRadioButton(hDlg, kBlowerUpButton, kBlowerLeftButton,
					kBlowerLeftButton);
		}
		if ((what != kInvisBlower) && (what != kLiftArea))
		{
			ShowWindow(GetDlgItem(hDlg, kBlowerUpButton), SW_HIDE);
			ShowWindow(GetDlgItem(hDlg, kBlowerRightButton), SW_HIDE);
			ShowWindow(GetDlgItem(hDlg, kBlowerDownButton), SW_HIDE);
			ShowWindow(GetDlgItem(hDlg, kBlowerLeftButton), SW_HIDE);
		}

		if (retroLinkList[objActive].room == -1)
			ShowWindow(GetDlgItem(hDlg, kBlowerLinkedFrom), SW_HIDE);

		CenterOverOwner(hDlg);
		ParamDialogText(hDlg, (const DialogParams *)lParam);
		return TRUE;
	}

	case WM_COMMAND:
	{
		HDC hdc;

		switch (LOWORD(wParam))
		{
		case IDOK:
		case kBlowerLinkedFrom:
		{
			SInt16 what = thisRoom->objects[objActive].what;

			if (IsDlgButtonChecked(hDlg, kBlowerInitialState))
				thisRoom->objects[objActive].data.a.initial = true;
			else
				thisRoom->objects[objActive].data.a.initial = false;

			if ((what == kInvisBlower) || (what == kLiftArea))
			{
				if (IsDlgButtonChecked(hDlg, kBlowerUpButton))
					thisRoom->objects[objActive].data.a.vector = 0x01;
				else if (IsDlgButtonChecked(hDlg, kBlowerRightButton))
					thisRoom->objects[objActive].data.a.vector = 0x02;
				else if (IsDlgButtonChecked(hDlg, kBlowerDownButton))
					thisRoom->objects[objActive].data.a.vector = 0x04;
				else if (IsDlgButtonChecked(hDlg, kBlowerLeftButton))
					thisRoom->objects[objActive].data.a.vector = 0x08;
			}

			if ((what == kLeftFan) || (what == kRightFan))
			{
				if (IsDlgButtonChecked(hDlg, kLeftFacingRadio))
					thisRoom->objects[objActive].what = kLeftFan;
				else
					thisRoom->objects[objActive].what = kRightFan;
				if (KeepObjectLegal())
				{
				}
				Mac_InvalWindowRect(mainWindow, &mainWindowRect);
				GetThisRoomsObjRects();
				ReadyBackground(thisRoom->background, thisRoom->tiles);
				DrawThisRoomsObjects();
			}
			fileDirty = true;
			UpdateMenus(false);
			EndDialog(hDlg, LOWORD(wParam));
			break;
		}

		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			break;

		case kBlowerUpButton:
		case kBlowerRightButton:
		case kBlowerDownButton:
		case kBlowerLeftButton:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			case BN_SETFOCUS:
			case BN_KILLFOCUS:
				hdc = GetDC(hDlg);
				UpdateBlowerInfo(hDlg, hdc);
				ReleaseDC(hDlg, hdc);
				break;
			}
			break;
		}
		return TRUE;
	}

	case WM_UPDATEUISTATE:
		InvalidateRect(hDlg, NULL, TRUE);
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hDlg, &ps);
		UpdateBlowerInfo(hDlg, ps.hdc);
		EndPaint(hDlg, &ps);
		return TRUE;
	}
	}
	return FALSE;
}

//--------------------------------------------------------------  FurnitureFilter

INT_PTR CALLBACK FurnitureFilter (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		if ((objActive < 0) || (retroLinkList[objActive].room == -1))
			ShowWindow(GetDlgItem(hDlg, kFurnitureLinkedFrom), SW_HIDE);

		CenterOverOwner(hDlg);
		ParamDialogText(hDlg, (const DialogParams *)lParam);
		FocusDefaultButton(hDlg);
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
		case kFurnitureLinkedFrom:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		}
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------  CustPictFilter

INT_PTR CALLBACK CustPictFilter (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	INT wasPict;
	HWND hwndPictID;

	switch (message)
	{
	case WM_INITDIALOG:
		if (thisRoom->objects[objActive].what == kCustomPict)
			wasPict = thisRoom->objects[objActive].data.g.height;
		else
			wasPict = thisRoom->objects[objActive].data.e.where;
		SetDlgItemInt(hDlg, kCustPictIDItem, (UINT)wasPict, TRUE);

		CenterOverOwner(hDlg);
		ParamDialogText(hDlg, (const DialogParams *)lParam);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			wasPict = (INT)GetDlgItemInt(hDlg, kCustPictIDItem, NULL, TRUE);
			if (thisRoom->objects[objActive].what == kCustomPict)
			{
				if ((wasPict < 10000) || (wasPict > 32767))
				{
					MessageBeep(MB_ICONWARNING);
					wasPict = thisRoom->objects[objActive].data.g.height;
					SetDlgItemInt(hDlg, kCustPictIDItem, (UINT)wasPict, TRUE);
					hwndPictID = GetDlgItem(hDlg, kCustPictIDItem);
					SendMessage(hDlg, WM_NEXTDLGCTL, (WPARAM)hwndPictID, TRUE);
				}
				else
				{
					thisRoom->objects[objActive].data.g.height = (SInt16)wasPict;
					if (KeepObjectLegal())
					{
					}
					fileDirty = true;
					UpdateMenus(false);
					Mac_InvalWindowRect(mainWindow, &mainWindowRect);
					GetThisRoomsObjRects();
					ReadyBackground(thisRoom->background, thisRoom->tiles);
					DrawThisRoomsObjects();
					EndDialog(hDlg, IDOK);
				}
			}
			else
			{
				if ((wasPict < 3000) || (wasPict > 32767))
				{
					MessageBeep(MB_ICONWARNING);
					wasPict = thisRoom->objects[objActive].data.e.where;
					SetDlgItemInt(hDlg, kCustPictIDItem, (UINT)wasPict, TRUE);
					hwndPictID = GetDlgItem(hDlg, kCustPictIDItem);
					SendMessage(hDlg, WM_NEXTDLGCTL, (WPARAM)hwndPictID, TRUE);
				}
				else
				{
					thisRoom->objects[objActive].data.e.where = (SInt16)wasPict;
					fileDirty = true;
					UpdateMenus(false);
					Mac_InvalWindowRect(mainWindow, &mainWindowRect);
					GetThisRoomsObjRects();
					ReadyBackground(thisRoom->background, thisRoom->tiles);
					DrawThisRoomsObjects();
					EndDialog(hDlg, IDOK);
				}
			}
			break;

		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			break;
		}
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------  SwitchFilter

INT_PTR CALLBACK SwitchFilter (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		HWND hwndInitialFocus = NULL;

		if (thisRoom->objects[objActive].data.e.type == kToggle)
		{
			hwndInitialFocus = GetDlgItem(hDlg, kToggleRadio);
			CheckRadioButton(hDlg, kToggleRadio, kForceOffRadio, kToggleRadio);
		}
		else if (thisRoom->objects[objActive].data.e.type == kForceOn)
		{
			hwndInitialFocus = GetDlgItem(hDlg, kForceOnRadio);
			CheckRadioButton(hDlg, kToggleRadio, kForceOffRadio, kForceOnRadio);
		}
		else if (thisRoom->objects[objActive].data.e.type == kForceOff)
		{
			hwndInitialFocus = GetDlgItem(hDlg, kForceOffRadio);
			CheckRadioButton(hDlg, kToggleRadio, kForceOffRadio, kForceOffRadio);
		}
		if (hwndInitialFocus == NULL)
			hwndInitialFocus = GetDlgItem(hDlg, kToggleRadio);

		if (thisRoom->objects[objActive].data.e.who == 255)
			EnableWindow(GetDlgItem(hDlg, kSwitchGotoButton), FALSE);

		if (retroLinkList[objActive].room == -1)
			ShowWindow(GetDlgItem(hDlg, kSwitchLinkedFrom), SW_HIDE);

		CenterOverOwner(hDlg);
		ParamDialogText(hDlg, (const DialogParams *)lParam);
		SendMessage(hDlg, WM_NEXTDLGCTL, (WPARAM)hwndInitialFocus, TRUE);
		return FALSE;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case kLinkSwitchButton:
		case kSwitchGotoButton:
		case kSwitchLinkedFrom:
			if (IsDlgButtonChecked(hDlg, kToggleRadio))
				thisRoom->objects[objActive].data.e.type = kToggle;
			else if (IsDlgButtonChecked(hDlg, kForceOnRadio))
				thisRoom->objects[objActive].data.e.type = kForceOn;
			else if (IsDlgButtonChecked(hDlg, kForceOffRadio))
				thisRoom->objects[objActive].data.e.type = kForceOff;
			fileDirty = true;
			UpdateMenus(false);
			EndDialog(hDlg, LOWORD(wParam));
			break;

		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			break;
		}
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------  TriggerFilter

INT_PTR CALLBACK TriggerFilter (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	INT delayIs;
	HWND hwndFocus;

	switch (message)
	{
	case WM_INITDIALOG:
		if (retroLinkList[objActive].room == -1)
			ShowWindow(GetDlgItem(hDlg, kTriggerLinkedFrom), SW_HIDE);

		if (thisRoom->objects[objActive].data.e.who == 255)
			EnableWindow(GetDlgItem(hDlg, kTriggerGotoButton), FALSE);

		delayIs = thisRoom->objects[objActive].data.e.delay;
		SetDlgItemInt(hDlg, kTriggerDelayItem, (UINT)delayIs, TRUE);

		CenterOverOwner(hDlg);
		ParamDialogText(hDlg, (const DialogParams *)lParam);
		hwndFocus = GetDlgItem(hDlg, kTriggerDelayItem);
		SendMessage(hDlg, WM_NEXTDLGCTL, (WPARAM)hwndFocus, TRUE);
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case kLinkTriggerButton:
		case kTriggerGotoButton:
		case kTriggerLinkedFrom:
			delayIs = GetDlgItemInt(hDlg, kTriggerDelayItem, NULL, TRUE);
			if ((delayIs < 0) || (delayIs > 32767))
			{
				MessageBeep(MB_ICONWARNING);
				delayIs = thisRoom->objects[objActive].data.e.delay;
				SetDlgItemInt(hDlg, kTriggerDelayItem, (UINT)delayIs, TRUE);
				hwndFocus = GetDlgItem(hDlg, kTriggerDelayItem);
				SendMessage(hDlg, WM_NEXTDLGCTL, (WPARAM)hwndFocus, TRUE);
			}
			else
			{
				thisRoom->objects[objActive].data.e.delay = (SInt16)delayIs;
				fileDirty = true;
				UpdateMenus(false);
				EndDialog(hDlg, LOWORD(wParam));
			}
			break;

		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			break;
		}
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------  LightFilter

INT_PTR CALLBACK LightFilter (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		if (thisRoom->objects[objActive].data.f.initial)
			CheckDlgButton(hDlg, kLightInitialState, BST_CHECKED);
		else
			CheckDlgButton(hDlg, kLightInitialState, BST_UNCHECKED);

		if (retroLinkList[objActive].room == -1)
			ShowWindow(GetDlgItem(hDlg, kLightLinkedFrom), SW_HIDE);

		CenterOverOwner(hDlg);
		ParamDialogText(hDlg, (const DialogParams *)lParam);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case kLightLinkedFrom:
			if (IsDlgButtonChecked(hDlg, kLightInitialState))
				thisRoom->objects[objActive].data.f.initial = true;
			else
				thisRoom->objects[objActive].data.f.initial = false;

			ReadyBackground(thisRoom->background, thisRoom->tiles);
			DrawThisRoomsObjects();
			Mac_InvalWindowRect(mainWindow, &mainWindowRect);
			fileDirty = true;
			UpdateMenus(false);
			EndDialog(hDlg, LOWORD(wParam));
			break;

		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			break;
		}
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------  ApplianceFilter

Boolean ApplianceFilter (DialogPtr dial, EventRecord *event, SInt16 *item)
{
	return false;
#if 0
	switch (event->what)
	{
		case keyDown:
		switch ((event->message) & charCodeMask)
		{
			case kReturnKeyASCII:
			case kEnterKeyASCII:
			FlashDialogButton(dial, kOkayButton);
			*item = kOkayButton;
			return(true);
			break;

			case kEscapeKeyASCII:
			FlashDialogButton(dial, kCancelButton);
			*item = kCancelButton;
			return(true);
			break;

			case kTabKeyASCII:
			SelectDialogItemText(dial, kDelayItem, 0, 1024);
			return(true);
			break;

			default:
			return(false);
		}
		break;

		case mouseDown:
		return(false);
		break;

		case mouseUp:
		return(false);
		break;

		case updateEvt:
		SetPort((GrafPtr)dial);
		BeginUpdate(GetDialogWindow(dial));
		UpdateApplianceInfo(dial);
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

//--------------------------------------------------------------  MicrowaveFilter

Boolean MicrowaveFilter (DialogPtr dial, EventRecord *event, SInt16 *item)
{
	return false;
#if 0
	switch (event->what)
	{
		case keyDown:
		switch ((event->message) & charCodeMask)
		{
			case kReturnKeyASCII:
			case kEnterKeyASCII:
			FlashDialogButton(dial, kOkayButton);
			*item = kOkayButton;
			return(true);
			break;

			case kEscapeKeyASCII:
			FlashDialogButton(dial, kCancelButton);
			*item = kCancelButton;
			return(true);
			break;

			default:
			return(false);
		}
		break;

		case mouseDown:
		return(false);
		break;

		case mouseUp:
		return(false);
		break;

		case updateEvt:
		SetPort((GrafPtr)dial);
		BeginUpdate(GetDialogWindow(dial));
		UpdateMicrowaveInfo(dial);
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

//--------------------------------------------------------------  GreaseFilter

Boolean GreaseFilter (DialogPtr dial, EventRecord *event, SInt16 *item)
{
	return false;
#if 0
	switch (event->what)
	{
		case keyDown:
		switch ((event->message) & charCodeMask)
		{
			case kReturnKeyASCII:
			case kEnterKeyASCII:
			FlashDialogButton(dial, kOkayButton);
			*item = kOkayButton;
			return(true);
			break;

			case kEscapeKeyASCII:
			FlashDialogButton(dial, kCancelButton);
			*item = kCancelButton;
			return(true);
			break;

			default:
			return(false);
		}
		break;

		case updateEvt:
		SetPort((GrafPtr)dial);
		BeginUpdate(GetDialogWindow(dial));
		UpdateGreaseInfo(dial);
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

//--------------------------------------------------------------  InvisBonusFilter

Boolean InvisBonusFilter (DialogPtr dial, EventRecord *event, SInt16 *item)
{
	return false;
#if 0
	switch (event->what)
	{
		case keyDown:
		switch ((event->message) & charCodeMask)
		{
			case kReturnKeyASCII:
			case kEnterKeyASCII:
			FlashDialogButton(dial, kOkayButton);
			*item = kOkayButton;
			return(true);
			break;

			default:
			return(false);
		}
		break;

		case updateEvt:
		SetPort((GrafPtr)dial);
		BeginUpdate(GetDialogWindow(dial));
		UpdateInvisBonusInfo(dial);
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

//--------------------------------------------------------------  TransFilter

Boolean TransFilter (DialogPtr dial, EventRecord *event, SInt16 *item)
{
	return false;
#if 0
	switch (event->what)
	{
		case keyDown:
		switch ((event->message) & charCodeMask)
		{
			case kReturnKeyASCII:
			case kEnterKeyASCII:
			FlashDialogButton(dial, kOkayButton);
			*item = kOkayButton;
			return(true);
			break;

			case kEscapeKeyASCII:
			FlashDialogButton(dial, kCancelButton);
			*item = kCancelButton;
			return(true);
			break;

			default:
			return(false);
		}
		break;

		case updateEvt:
		SetPort((GrafPtr)dial);
		BeginUpdate(GetDialogWindow(dial));
		UpdateTransInfo(dial);
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

//--------------------------------------------------------------  EnemyFilter

Boolean EnemyFilter (DialogPtr dial, EventRecord *event, SInt16 *item)
{
	return false;
#if 0
	switch (event->what)
	{
		case keyDown:
		switch ((event->message) & charCodeMask)
		{
			case kReturnKeyASCII:
			case kEnterKeyASCII:
			FlashDialogButton(dial, kOkayButton);
			*item = kOkayButton;
			return(true);
			break;

			case kEscapeKeyASCII:
			FlashDialogButton(dial, kCancelButton);
			*item = kCancelButton;
			return(true);
			break;

			case kTabKeyASCII:
			SelectDialogItemText(dial, kDelay2Item, 0, 1024);
			return(true);
			break;

			default:
			return(false);
		}
		break;

		case mouseDown:
		return(false);
		break;

		case mouseUp:
		return(false);
		break;

		case updateEvt:
		SetPort((GrafPtr)dial);
		BeginUpdate(GetDialogWindow(dial));
		UpdateEnemyInfo(dial);
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

//--------------------------------------------------------------  EnemyFilter

Boolean FlowerFilter (DialogPtr dial, EventRecord *event, SInt16 *item)
{
	return false;
#if 0
	switch (event->what)
	{
		case keyDown:
		switch ((event->message) & charCodeMask)
		{
			case kReturnKeyASCII:
			case kEnterKeyASCII:
			FlashDialogButton(dial, kOkayButton);
			*item = kOkayButton;
			return(true);
			break;

			case kEscapeKeyASCII:
			FlashDialogButton(dial, kCancelButton);
			*item = kCancelButton;
			return(true);
			break;

			default:
			return(false);
		}
		break;

		case mouseDown:
		return(false);
		break;

		case mouseUp:
		return(false);
		break;

		case updateEvt:
		SetPort((GrafPtr)dial);
		BeginUpdate(GetDialogWindow(dial));
		UpdateFlowerInfo(dial);
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

//--------------------------------------------------------------  DoBlowerObjectInfo

void DoBlowerObjectInfo (HWND hwndOwner)
{
	DialogParams params = { 0 };
	wchar_t numberStr[32];
	wchar_t kindStr[256];
	wchar_t distStr[32];
	INT_PTR result;

	StringCchPrintf(numberStr, ARRAYSIZE(numberStr), L"%ld", (long)(objActive + 1));
	GetObjectName(kindStr, ARRAYSIZE(kindStr), thisRoom->objects[objActive].what);
	StringCchPrintf(distStr, ARRAYSIZE(distStr), L"%ld",
			(long)thisRoom->objects[objActive].data.a.distance);

	params.arg[0] = numberStr;
	params.arg[1] = kindStr;
	params.arg[2] = distStr;
	result = DialogBoxParam(HINST_THISCOMPONENT,
			MAKEINTRESOURCE(kBlowerInfoDialogID),
			hwndOwner, BlowerFilter, (LPARAM)&params);

	if (result == kBlowerLinkedFrom)
	{
		GoToObjectInRoomNum(retroLinkList[objActive].object,
				retroLinkList[objActive].room);
	}
}

//--------------------------------------------------------------  DoFurnitureObjectInfo

void DoFurnitureObjectInfo (HWND hwndOwner)
{
	DialogParams params = { 0 };
	INT_PTR result;
	wchar_t numberStr[32];
	wchar_t kindStr[256];

	if (objActive == kInitialGliderSelected)
	{
		StringCchCopy(numberStr, ARRAYSIZE(numberStr), L"-");
		StringCchCopy(kindStr, ARRAYSIZE(kindStr), L"Glider Begins");
	}
	else if (objActive == kLeftGliderSelected)
	{
		StringCchCopy(numberStr, ARRAYSIZE(numberStr), L"-");
		StringCchCopy(kindStr, ARRAYSIZE(kindStr), L"New Glider (left)");
	}
	else if (objActive == kRightGliderSelected)
	{
		StringCchCopy(numberStr, ARRAYSIZE(numberStr), L"-");
		StringCchCopy(kindStr, ARRAYSIZE(kindStr), L"New Glider (right)");
	}
	else
	{
		StringCchPrintf(numberStr, ARRAYSIZE(numberStr), L"%ld", (long)(objActive + 1));
		GetObjectName(kindStr, ARRAYSIZE(kindStr), thisRoom->objects[objActive].what);
	}

	params.arg[0] = numberStr;
	params.arg[1] = kindStr;
	result = DialogBoxParam(HINST_THISCOMPONENT,
			MAKEINTRESOURCE(kFurnitureInfoDialogID),
			hwndOwner, FurnitureFilter, (LPARAM)&params);

	if (result == kFurnitureLinkedFrom)
	{
		GoToObjectInRoomNum(retroLinkList[objActive].object,
				retroLinkList[objActive].room);
	}
}

//--------------------------------------------------------------  DoCustPictObjectInfo

void DoCustPictObjectInfo (HWND hwndOwner)
{
	DialogParams params = { 0 };
	wchar_t numberStr[32];
	wchar_t kindStr[256];

	StringCchPrintf(numberStr, ARRAYSIZE(numberStr), L"%ld", (long)(objActive + 1));
	GetObjectName(kindStr, ARRAYSIZE(kindStr), thisRoom->objects[objActive].what);
	if (thisRoom->objects[objActive].what == kCustomPict)
	{
		params.arg[0] = numberStr;
		params.arg[1] = kindStr;
		params.arg[2] = L"PICT";
		params.arg[3] = L"10000";
	}
	else
	{
		params.arg[0] = numberStr;
		params.arg[1] = kindStr;
		params.arg[2] = L"Sound";
		params.arg[3] = L"3000";
	}

	DialogBoxParam(HINST_THISCOMPONENT,
			MAKEINTRESOURCE(kCustPictInfoDialogID),
			hwndOwner, CustPictFilter, (LPARAM)&params);
}

//--------------------------------------------------------------  DoSwitchObjectInfo

void DoSwitchObjectInfo (HWND hwndOwner)
{
	DialogParams params = { 0 };
	wchar_t numberStr[16];
	wchar_t kindStr[256];
	wchar_t roomStr[32];
	wchar_t objStr[16];
	SInt16 floor, suite;
	INT_PTR result;

	StringCchPrintf(numberStr, ARRAYSIZE(numberStr), L"%d", (int)(objActive + 1));
	GetObjectName(kindStr, ARRAYSIZE(kindStr), thisRoom->objects[objActive].what);
	if (thisRoom->objects[objActive].data.e.where == -1)
	{
		StringCchCopy(roomStr, ARRAYSIZE(roomStr), L"none");
	}
	else
	{
		ExtractFloorSuite(thisRoom->objects[objActive].data.e.where, &floor, &suite);
		StringCchPrintf(roomStr, ARRAYSIZE(roomStr), L"%d / %d", (int)floor, (int)suite);
	}

	if (thisRoom->objects[objActive].data.e.who == 255)
	{
		StringCchCopy(objStr, ARRAYSIZE(objStr), L"none");
	}
	else
	{
		StringCchPrintf(objStr, ARRAYSIZE(objStr), L"%d",
			(int)(thisRoom->objects[objActive].data.e.who + 1));
	}

	params.arg[0] = numberStr;
	params.arg[1] = kindStr;
	params.arg[2] = roomStr;
	params.arg[3] = objStr;
	result = DialogBoxParam(HINST_THISCOMPONENT,
			MAKEINTRESOURCE(kSwitchInfoDialogID),
			hwndOwner, SwitchFilter, (LPARAM)&params);

	if (result == kLinkSwitchButton)
	{
		linkType = kSwitchLinkOnly;
		linkerIsSwitch = true;
		OpenLinkWindow();
		linkRoom = thisRoomNumber;
		linkObject = (Byte)objActive;
		DeselectObject();
	}
	else if (result == kSwitchGotoButton)
	{
		GoToObjectInRoom(thisRoom->objects[objActive].data.e.who, floor, suite);
	}
	else if (result == kSwitchLinkedFrom)
	{
		GoToObjectInRoomNum(retroLinkList[objActive].object,
			retroLinkList[objActive].room);
	}
}

//--------------------------------------------------------------  DoTriggerObjectInfo

void DoTriggerObjectInfo (HWND hwndOwner)
{
	DialogParams params = { 0 };
	wchar_t numberStr[16];
	wchar_t kindStr[256];
	wchar_t roomStr[32];
	wchar_t objStr[32];
	SInt16 floor, suite;
	INT_PTR result;

	StringCchPrintf(numberStr, ARRAYSIZE(numberStr), L"%d", (int)(objActive + 1));
	GetObjectName(kindStr, ARRAYSIZE(kindStr), thisRoom->objects[objActive].what);
	if (thisRoom->objects[objActive].data.e.where == -1)
	{
		StringCchCopy(roomStr, ARRAYSIZE(roomStr), L"none");
	}
	else
	{
		ExtractFloorSuite(thisRoom->objects[objActive].data.e.where, &floor, &suite);
		StringCchPrintf(roomStr, ARRAYSIZE(roomStr), L"%d / %d", (int)floor, (int)suite);
	}

	if (thisRoom->objects[objActive].data.e.who == 255)
	{
		StringCchCopy(objStr, ARRAYSIZE(objStr), L"none");
	}
	else
	{
		StringCchPrintf(objStr, ARRAYSIZE(objStr), L"%d",
			(int)(thisRoom->objects[objActive].data.e.who + 1));
	}

	params.arg[0] = numberStr;
	params.arg[1] = kindStr;
	params.arg[2] = roomStr;
	params.arg[3] = objStr;
	result = DialogBoxParam(HINST_THISCOMPONENT,
			MAKEINTRESOURCE(kTriggerInfoDialogID),
			hwndOwner, TriggerFilter, (LPARAM)&params);

	if (result == kLinkTriggerButton)
	{
		linkType = kTriggerLinkOnly;
		linkerIsSwitch = true;
		OpenLinkWindow();
		linkRoom = thisRoomNumber;
		linkObject = (Byte)objActive;
		DeselectObject();
	}
	else if (result == kTriggerGotoButton)
	{
		GoToObjectInRoom(thisRoom->objects[objActive].data.e.who, floor, suite);
	}
	else if (result == kTriggerLinkedFrom)
	{
		GoToObjectInRoomNum(retroLinkList[objActive].object,
			retroLinkList[objActive].room);
	}
}

//--------------------------------------------------------------  DoLightObjectInfo

void DoLightObjectInfo (HWND hwndOwner)
{
	DialogParams params = { 0 };
	wchar_t numberStr[16];
	wchar_t kindStr[256];
	INT_PTR result;

	StringCchPrintf(numberStr, ARRAYSIZE(numberStr), L"%d", (int)(objActive + 1));
	GetObjectName(kindStr, ARRAYSIZE(kindStr), thisRoom->objects[objActive].what);

	params.arg[0] = numberStr;
	params.arg[1] = kindStr;
	result = DialogBoxParam(HINST_THISCOMPONENT,
			MAKEINTRESOURCE(kLightInfoDialogID),
			hwndOwner, LightFilter, (LPARAM)&params);

	if (result == kLightLinkedFrom)
	{
		GoToObjectInRoomNum(retroLinkList[objActive].object,
				retroLinkList[objActive].room);
	}
}

//--------------------------------------------------------------  DoApplianceObjectInfo

void DoApplianceObjectInfo (HWND hwndOwner, SInt16 what)
{
	MessageBox(hwndOwner, L"DoApplianceObjectInfo()", NULL, MB_ICONHAND);
	return;
#if 0
	DialogPtr		infoDial;
	Str255			numberStr, kindStr;
	long			delay;
	short			item, initial;
	Boolean			leaving, doReturn;
	ModalFilterUPP	applianceFilterUPP;

	applianceFilterUPP = NewModalFilterUPP(ApplianceFilter);

	NumToString(objActive + 1, numberStr);
	GetIndString(kindStr, kObjectNameStrings, thisRoom->objects[objActive].what);
	ParamText(numberStr, kindStr, "\p", "\p");

	BringUpDialog(&infoDial, kApplianceInfoDialogID);

	if (retroLinkList[objActive].room == -1)
		HideDialogItem(infoDial, 10);

	if (thisRoom->objects[objActive].data.g.initial)
		SetDialogItemValue(infoDial, kInitialStateCheckbox, 1);
	else
		SetDialogItemValue(infoDial, kInitialStateCheckbox, 0);

	if ((what == kShredder) || (what == kMacPlus) || (what == kTV) ||
			(what == kCoffee) || (what == kVCR) || (what == kMicrowave))
	{
		HideDialogItem(infoDial, kDelayItem);
		HideDialogItem(infoDial, kDelayLabelItem);
	}

	delay = thisRoom->objects[objActive].data.g.delay;
	SetDialogNumToStr(infoDial, kDelayItem, (long)delay);
	SelectDialogItemText(infoDial, kDelayItem, 0, 1024);

	leaving = false;
	doReturn = false;

	while (!leaving)
	{
		ModalDialog(applianceFilterUPP, &item);

		if (item == kOkayButton)
		{
			GetDialogNumFromStr(infoDial, kDelayItem, &delay);
			if ((delay < 0L) || (delay > 255L))
			{
				SysBeep(0);
				delay = thisRoom->objects[objActive].data.g.delay;
				SetDialogNumToStr(infoDial, kDelayItem, (long)delay);
				SelectDialogItemText(infoDial, kDelayItem, 0, 1024);
			}
			else
			{
				thisRoom->objects[objActive].data.g.delay = (Byte)delay;
				GetDialogItemValue(infoDial, kInitialStateCheckbox, &initial);
				if (initial == 1)
					thisRoom->objects[objActive].data.g.initial = true;
				else
					thisRoom->objects[objActive].data.g.initial = false;
				fileDirty = true;
				UpdateMenus(false);
				InvalWindowRect(mainWindow, &mainWindowRect);
				GetThisRoomsObjRects();
				ReadyBackground(thisRoom->background, thisRoom->tiles);
				DrawThisRoomsObjects();
				leaving = true;
			}
		}
		else if (item == kCancelButton)
			leaving = true;
		else if (item == kInitialStateCheckbox)
			ToggleDialogItemValue(infoDial, kInitialStateCheckbox);
		else if (item == 10)			// Linked From? button.
		{
			GetDialogNumFromStr(infoDial, kDelayItem, &delay);
			if ((delay < 0L) || (delay > 255L))
			{
				SysBeep(0);
				delay = thisRoom->objects[objActive].data.g.delay;
				SetDialogNumToStr(infoDial, kDelayItem, (long)delay);
				SelectDialogItemText(infoDial, kDelayItem, 0, 1024);
			}
			else
			{
				thisRoom->objects[objActive].data.g.delay = (Byte)delay;
				GetDialogItemValue(infoDial, kInitialStateCheckbox, &initial);
				if (initial == 1)
					thisRoom->objects[objActive].data.g.initial = true;
				else
					thisRoom->objects[objActive].data.g.initial = false;
				fileDirty = true;
				UpdateMenus(false);
				InvalWindowRect(mainWindow, &mainWindowRect);
				GetThisRoomsObjRects();
				ReadyBackground(thisRoom->background, thisRoom->tiles);
				DrawThisRoomsObjects();
				leaving = true;
				doReturn = true;
			}
		}
	}

	DisposeDialog(infoDial);
	DisposeModalFilterUPP(applianceFilterUPP);

	if (doReturn)
	{
		GoToObjectInRoomNum(retroLinkList[objActive].object,
				retroLinkList[objActive].room);
	}
#endif
}

//--------------------------------------------------------------  DoMicrowaveObjectInfo

void DoMicrowaveObjectInfo (HWND hwndOwner)
{
	MessageBox(hwndOwner, L"DoMicrowaveObjectInfo()", NULL, MB_ICONHAND);
	return;
#if 0
	DialogPtr		infoDial;
	Str255			numberStr, kindStr;
	short			item, initial, kills;
	Boolean			leaving, doReturn;
	ModalFilterUPP	microwaveFilterUPP;

	microwaveFilterUPP = NewModalFilterUPP(MicrowaveFilter);

	NumToString(objActive + 1, numberStr);
	GetIndString(kindStr, kObjectNameStrings, thisRoom->objects[objActive].what);
	ParamText(numberStr, kindStr, "\p", "\p");

	BringUpDialog(&infoDial, kMicrowaveInfoDialogID);

	if (retroLinkList[objActive].room == -1)
		HideDialogItem(infoDial, 11);

	if (thisRoom->objects[objActive].data.g.initial)
		SetDialogItemValue(infoDial, kInitialStateCheckbox, 1);
	else
		SetDialogItemValue(infoDial, kInitialStateCheckbox, 0);

	kills = (short)thisRoom->objects[objActive].data.g.byte0;
	if ((kills & 0x0001) == 0x0001)
		SetDialogItemValue(infoDial, kKillBandsCheckbox, 1);
	else
		SetDialogItemValue(infoDial, kKillBandsCheckbox, 0);
	if ((kills & 0x0002) == 0x0002)
		SetDialogItemValue(infoDial, kKillBatteryCheckbox, 1);
	else
		SetDialogItemValue(infoDial, kKillBatteryCheckbox, 0);
	if ((kills & 0x0004) == 0x0004)
		SetDialogItemValue(infoDial, kKillFoilCheckbox, 1);
	else
		SetDialogItemValue(infoDial, kKillFoilCheckbox, 0);

	leaving = false;
	doReturn = false;

	while (!leaving)
	{
		ModalDialog(microwaveFilterUPP, &item);

		if (item == kOkayButton)
		{
			GetDialogItemValue(infoDial, kInitialStateCheckbox, &initial);
			if (initial == 1)
				thisRoom->objects[objActive].data.g.initial = true;
			else
				thisRoom->objects[objActive].data.g.initial = false;
			kills = 0;
			GetDialogItemValue(infoDial, kKillBandsCheckbox, &initial);
			if (initial == 1)
				kills += 1;
			GetDialogItemValue(infoDial, kKillBatteryCheckbox, &initial);
			if (initial == 1)
				kills += 2;
			GetDialogItemValue(infoDial, kKillFoilCheckbox, &initial);
			if (initial == 1)
				kills += 4;
			thisRoom->objects[objActive].data.g.byte0 = (Byte)kills;

			fileDirty = true;
			UpdateMenus(false);
			InvalWindowRect(mainWindow, &mainWindowRect);
			GetThisRoomsObjRects();
			ReadyBackground(thisRoom->background, thisRoom->tiles);
			DrawThisRoomsObjects();
			leaving = true;
		}
		else if (item == kCancelButton)
			leaving = true;
		else if (item == kInitialStateCheckbox)
			ToggleDialogItemValue(infoDial, kInitialStateCheckbox);
		else if (item == kKillBandsCheckbox)
			ToggleDialogItemValue(infoDial, kKillBandsCheckbox);
		else if (item == kKillBatteryCheckbox)
			ToggleDialogItemValue(infoDial, kKillBatteryCheckbox);
		else if (item == kKillFoilCheckbox)
			ToggleDialogItemValue(infoDial, kKillFoilCheckbox);
		else if (item == 11)			// Linked From? button.
		{
			GetDialogItemValue(infoDial, kInitialStateCheckbox, &initial);
			if (initial == 1)
				thisRoom->objects[objActive].data.g.initial = true;
			else
				thisRoom->objects[objActive].data.g.initial = false;
			kills = 0;
			GetDialogItemValue(infoDial, kKillBandsCheckbox, &initial);
			if (initial == 1)
				kills += 1;
			GetDialogItemValue(infoDial, kKillBatteryCheckbox, &initial);
			if (initial == 1)
				kills += 2;
			GetDialogItemValue(infoDial, kKillFoilCheckbox, &initial);
			if (initial == 1)
				kills += 4;
			thisRoom->objects[objActive].data.g.byte0 = (Byte)kills;

			fileDirty = true;
			UpdateMenus(false);
			InvalWindowRect(mainWindow, &mainWindowRect);
			GetThisRoomsObjRects();
			ReadyBackground(thisRoom->background, thisRoom->tiles);
			DrawThisRoomsObjects();
			leaving = true;
			doReturn = true;
		}
	}

	DisposeDialog(infoDial);
	DisposeModalFilterUPP(microwaveFilterUPP);

	if (doReturn)
	{
		GoToObjectInRoomNum(retroLinkList[objActive].object,
				retroLinkList[objActive].room);
	}
#endif
}

//--------------------------------------------------------------  DoGreaseObjectInfo

void DoGreaseObjectInfo (HWND hwndOwner)
{
	MessageBox(hwndOwner, L"DoGreaseObjectInfo()", NULL, MB_ICONHAND);
	return;
#if 0
	DialogPtr		infoDial;
	Str255			numberStr, kindStr;
	short			item;
	Boolean			leaving, wasSpilled, doReturn;
	ModalFilterUPP	greaseFilterUPP;

	greaseFilterUPP = NewModalFilterUPP(GreaseFilter);

	NumToString(objActive + 1, numberStr);
	GetIndString(kindStr, kObjectNameStrings, thisRoom->objects[objActive].what);
	ParamText(numberStr, kindStr, "\p", "\p");

	BringUpDialog(&infoDial, kGreaseInfoDialogID);

	if (retroLinkList[objActive].room == -1)
		HideDialogItem(infoDial, 8);

	wasSpilled = !(thisRoom->objects[objActive].data.c.initial);
	SetDialogItemValue(infoDial, kGreaseItem, (short)wasSpilled);
	leaving = false;
	doReturn = false;

	while (!leaving)
	{
		ModalDialog(greaseFilterUPP, &item);

		if (item == kOkayButton)
		{
			thisRoom->objects[objActive].data.c.initial = !wasSpilled;
			fileDirty = true;
			UpdateMenus(false);
			InvalWindowRect(mainWindow, &mainWindowRect);
			GetThisRoomsObjRects();
			ReadyBackground(thisRoom->background, thisRoom->tiles);
			DrawThisRoomsObjects();
			leaving = true;
		}
		else if (item == kCancelButton)
		{
			leaving = true;
		}
		else if (item == kGreaseItem)
		{
			wasSpilled = !wasSpilled;
			SetDialogItemValue(infoDial, kGreaseItem, (short)wasSpilled);
		}
		else if (item == 8)				// Linked From? button.
		{
			thisRoom->objects[objActive].data.c.initial = !wasSpilled;
			fileDirty = true;
			UpdateMenus(false);
			InvalWindowRect(mainWindow, &mainWindowRect);
			GetThisRoomsObjRects();
			ReadyBackground(thisRoom->background, thisRoom->tiles);
			DrawThisRoomsObjects();
			leaving = true;
			doReturn = true;
		}
	}

	DisposeDialog(infoDial);
	DisposeModalFilterUPP(greaseFilterUPP);

	if (doReturn)
	{
		GoToObjectInRoomNum(retroLinkList[objActive].object,
				retroLinkList[objActive].room);
	}
#endif
}

//--------------------------------------------------------------  DoInvisBonusObjectInfo

void DoInvisBonusObjectInfo (HWND hwndOwner)
{
	MessageBox(hwndOwner, L"DoInvisBonusObjectInfo()", NULL, MB_ICONHAND);
	return;
#if 0
	DialogPtr		infoDial;
	Str255			numberStr, kindStr;
	short			item;
	Boolean			leaving, doReturn;
	ModalFilterUPP	invisBonusFilterUPP;

	invisBonusFilterUPP = NewModalFilterUPP(InvisBonusFilter);

	NumToString(objActive + 1, numberStr);
	GetIndString(kindStr, kObjectNameStrings, thisRoom->objects[objActive].what);
	ParamText(numberStr, kindStr, "\p", "\p");

	switch (thisRoom->objects[objActive].data.c.points)
	{
		case 300:
		newPoint = 1;
		break;

		case 500:
		newPoint = 2;
		break;

		default:
		newPoint = 0;
		break;
	}

	BringUpDialog(&infoDial, kInvisBonusInfoDialogID);

	if (retroLinkList[objActive].room == -1)
		HideDialogItem(infoDial, 9);

	leaving = false;
	doReturn = false;

	while (!leaving)
	{
		ModalDialog(invisBonusFilterUPP, &item);

		if (item == kOkayButton)
		{
			switch (newPoint)
			{
				case 0:
				thisRoom->objects[objActive].data.c.points = 100;
				break;

				case 1:
				thisRoom->objects[objActive].data.c.points = 300;
				break;

				case 2:
				thisRoom->objects[objActive].data.c.points = 500;
				break;
			}
			fileDirty = true;
			UpdateMenus(false);
			leaving = true;
		}
		else if (item == kCancelButton)
			leaving = true;
		else if (item == k100PtRadio)
		{
			SelectFromRadioGroup(infoDial, item, k100PtRadio, k500PtRadio);
			newPoint = 0;
		}
		else if (item == k300PtRadio)
		{
			SelectFromRadioGroup(infoDial, item, k100PtRadio, k500PtRadio);
			newPoint = 1;
		}
		else if (item == k500PtRadio)
		{
			SelectFromRadioGroup(infoDial, item, k100PtRadio, k500PtRadio);
			newPoint = 2;
		}
		else if (item == 9)				// Linked From? button.
		{
			switch (newPoint)
			{
				case 0:
				thisRoom->objects[objActive].data.c.points = 100;
				break;

				case 1:
				thisRoom->objects[objActive].data.c.points = 300;
				break;

				case 2:
				thisRoom->objects[objActive].data.c.points = 500;
				break;
			}
			fileDirty = true;
			UpdateMenus(false);
			leaving = true;
			doReturn = true;
		}
	}

	DisposeDialog(infoDial);
	DisposeModalFilterUPP(invisBonusFilterUPP);

	if (doReturn)
	{
		GoToObjectInRoomNum(retroLinkList[objActive].object,
				retroLinkList[objActive].room);
	}
#endif
}

//--------------------------------------------------------------  DoTransObjectInfo

void DoTransObjectInfo (HWND hwndOwner, SInt16 what)
{
	MessageBox(hwndOwner, L"DoTransObjectInfo()", NULL, MB_ICONHAND);
	return;
#if 0
	DialogPtr		infoDial;
	Str255			numberStr, kindStr, roomStr, tempStr, objStr;
	short			item, floor, suite;
	Boolean			leaving, doLink, doGoTo, doReturn, wasState;
	ModalFilterUPP	transFilterUPP;

	transFilterUPP = NewModalFilterUPP(TransFilter);

	NumToString(objActive + 1, numberStr);
	GetIndString(kindStr, kObjectNameStrings, thisRoom->objects[objActive].what);
	if (thisRoom->objects[objActive].data.d.where == -1)
		PasStringCopy("\pnone", roomStr);
	else
	{
		ExtractFloorSuite(thisRoom->objects[objActive].data.d.where, &floor, &suite);
		NumToString((long)floor, roomStr);
		PasStringConcat(roomStr, "\p / ");
		NumToString((long)suite, tempStr);
		PasStringConcat(roomStr, tempStr);
	}

	if (thisRoom->objects[objActive].data.d.who == 255)
		PasStringCopy("\pnone", objStr);
	else
		NumToString((long)thisRoom->objects[objActive].data.d.who + 1, objStr);

	ParamText(numberStr, kindStr, roomStr, objStr);

	BringUpDialog(&infoDial, kTransInfoDialogID);

	if (retroLinkList[objActive].room == -1)
		HideDialogItem(infoDial, 12);
	if (what != kDeluxeTrans)
		HideDialogItem(infoDial, kInitialStateCheckbox3);
	else
	{
		wasState = (thisRoom->objects[objActive].data.d.wide & 0xF0) >> 4;
		SetDialogItemValue(infoDial, kInitialStateCheckbox3, (short)wasState);
	}

	leaving = false;
	doLink = false;
	doGoTo = false;
	doReturn = false;

	if (thisRoom->objects[objActive].data.d.who == 255)
		MyDisableControl(infoDial, kGotoButton1);

	while (!leaving)
	{
		ModalDialog(transFilterUPP, &item);

		if (item == kOkayButton)
		{
			if (what == kDeluxeTrans)
				thisRoom->objects[objActive].data.d.wide = wasState << 4;
			fileDirty = true;
			UpdateMenus(false);
			leaving = true;
		}
		else if (item == kCancelButton)
			leaving = true;
		else if (item == kLinkTransButton)
		{
			if (what == kDeluxeTrans)
				thisRoom->objects[objActive].data.d.wide = wasState << 4;
			fileDirty = true;
			UpdateMenus(false);
			leaving = true;
			doLink = true;
		}
		else if (item == kGotoButton1)
		{
			if (what == kDeluxeTrans)
				thisRoom->objects[objActive].data.d.wide = wasState << 4;
			fileDirty = true;
			UpdateMenus(false);
			leaving = true;
			doGoTo = true;
		}
		else if (item == 12)			// Linked From? button.
		{
			if (what == kDeluxeTrans)
				thisRoom->objects[objActive].data.d.wide = wasState << 4;
			fileDirty = true;
			UpdateMenus(false);
			leaving = true;
			doReturn = true;
		}
		else if (item == kInitialStateCheckbox3)
		{
			wasState = !wasState;
			SetDialogItemValue(infoDial, kInitialStateCheckbox3, (short)wasState);
		}
	}

	DisposeDialog(infoDial);
	DisposeModalFilterUPP(transFilterUPP);

	if (doLink)
	{
		linkType = kTransportLinkOnly;
		linkerIsSwitch = false;
		OpenLinkWindow();
		linkRoom = thisRoomNumber;
		linkObject = (Byte)objActive;
		DeselectObject();
	}
	else if (doGoTo)
	{
		GoToObjectInRoom((short)thisRoom->objects[objActive].data.d.who, floor, suite);
	}
	else if (doReturn)
	{
		GoToObjectInRoomNum(retroLinkList[objActive].object,
				retroLinkList[objActive].room);
	}
#endif
}

//--------------------------------------------------------------  DoEnemyObjectInfo

void DoEnemyObjectInfo (HWND hwndOwner, SInt16 what)
{
	MessageBox(hwndOwner, L"DoEnemyObjectInfo()", NULL, MB_ICONHAND);
	return;
#if 0
	DialogPtr		infoDial;
	Str255			numberStr, kindStr;
	long			delay;
	short			item, initial;
	Boolean			leaving, doReturn;
	ModalFilterUPP	enemyFilterUPP;

	enemyFilterUPP = NewModalFilterUPP(EnemyFilter);

	NumToString(objActive + 1, numberStr);
	GetIndString(kindStr, kObjectNameStrings, thisRoom->objects[objActive].what);
	ParamText(numberStr, kindStr, "\p", "\p");

	BringUpDialog(&infoDial, kEnemyInfoDialogID);

	if (retroLinkList[objActive].room == -1)
		HideDialogItem(infoDial, 11);

	delay = thisRoom->objects[objActive].data.h.delay;
	SetDialogNumToStr(infoDial, kDelay2Item, (long)delay);
	SelectDialogItemText(infoDial, kDelay2Item, 0, 1024);

	if (thisRoom->objects[objActive].data.h.initial)
		SetDialogItemValue(infoDial, kInitialStateCheckbox2, 1);
	else
		SetDialogItemValue(infoDial, kInitialStateCheckbox2, 0);

	if (what == kBall)
	{
		HideDialogItem(infoDial, kDelay2Item);
		HideDialogItem(infoDial, 8);
		HideDialogItem(infoDial, 9);
	}

	leaving = false;
	doReturn = false;

	while (!leaving)
	{
		ModalDialog(enemyFilterUPP, &item);

		if (item == kOkayButton)
		{
			GetDialogNumFromStr(infoDial, kDelay2Item, &delay);
			if (((delay < 0L) || (delay > 255L)) && (what != kBall))
			{
				SysBeep(0);
				delay = thisRoom->objects[objActive].data.h.delay;
				SetDialogNumToStr(infoDial, kDelay2Item, (long)delay);
				SelectDialogItemText(infoDial, kDelay2Item, 0, 1024);
			}
			else
			{
				GetDialogItemValue(infoDial, kInitialStateCheckbox2, &initial);
				if (initial == 1)
					thisRoom->objects[objActive].data.h.initial = true;
				else
					thisRoom->objects[objActive].data.h.initial = false;
				if (what != kBall)
					thisRoom->objects[objActive].data.h.delay = (Byte)delay;
				fileDirty = true;
				UpdateMenus(false);
				leaving = true;
			}
		}
		else if (item == kCancelButton)
			leaving = true;
		else if (item == kInitialStateCheckbox2)
			ToggleDialogItemValue(infoDial, kInitialStateCheckbox2);
		else if (item == 11)			// Linked From? button.
		{
			GetDialogNumFromStr(infoDial, kDelay2Item, &delay);
			if (((delay < 0L) || (delay > 255L)) && (what != kBall))
			{
				SysBeep(0);
				delay = thisRoom->objects[objActive].data.h.delay;
				SetDialogNumToStr(infoDial, kDelay2Item, (long)delay);
				SelectDialogItemText(infoDial, kDelay2Item, 0, 1024);
			}
			else
			{
				GetDialogItemValue(infoDial, kInitialStateCheckbox2, &initial);
				if (initial == 1)
					thisRoom->objects[objActive].data.h.initial = true;
				else
					thisRoom->objects[objActive].data.h.initial = false;
				if (what != kBall)
					thisRoom->objects[objActive].data.h.delay = (Byte)delay;
				fileDirty = true;
				UpdateMenus(false);
				leaving = true;
				doReturn = true;
			}
		}
	}

	DisposeDialog(infoDial);
	DisposeModalFilterUPP(enemyFilterUPP);

	if (doReturn)
	{
		GoToObjectInRoomNum(retroLinkList[objActive].object,
				retroLinkList[objActive].room);
	}
#endif
}

//--------------------------------------------------------------  DoFlowerObjectInfo

void DoFlowerObjectInfo (HWND hwndOwner)
{
	MessageBox(hwndOwner, L"DoFlowerObjectInfo()", NULL, MB_ICONHAND);
	return;
#if 0
	DialogPtr		infoDial;
	Str255			numberStr, kindStr;
	short			item, flower;
	Boolean			leaving, doReturn;
	ModalFilterUPP	flowerFilterUPP;

	flowerFilterUPP = NewModalFilterUPP(FlowerFilter);

	NumToString(objActive + 1, numberStr);
	GetIndString(kindStr, kObjectNameStrings, thisRoom->objects[objActive].what);
	ParamText(numberStr, kindStr, "\p", "\p");

	BringUpDialog(&infoDial, kFlowerInfoDialogID);

	if (retroLinkList[objActive].room == -1)
		HideDialogItem(infoDial, 13);

	flower = thisRoom->objects[objActive].data.i.pict + kRadioFlower1;
	SelectFromRadioGroup(infoDial, flower, kRadioFlower1, kRadioFlower6);

	leaving = false;
	doReturn = false;

	while (!leaving)
	{
		ModalDialog(flowerFilterUPP, &item);

		if (item == kOkayButton)
		{
			flower -= kRadioFlower1;
			if (flower != thisRoom->objects[objActive].data.i.pict)
			{
				InvalWindowRect(mainWindow, &thisRoom->objects[objActive].data.i.bounds);
				thisRoom->objects[objActive].data.i.bounds.right =
						thisRoom->objects[objActive].data.i.bounds.left +
						RectWide(&flowerSrc[flower]);
				thisRoom->objects[objActive].data.i.bounds.top =
						thisRoom->objects[objActive].data.i.bounds.bottom -
						RectTall(&flowerSrc[flower]);
				thisRoom->objects[objActive].data.i.pict = flower;
				InvalWindowRect(mainWindow, &thisRoom->objects[objActive].data.i.bounds);
				GetThisRoomsObjRects();
				ReadyBackground(thisRoom->background, thisRoom->tiles);
				DrawThisRoomsObjects();
				fileDirty = true;
				UpdateMenus(false);
				wasFlower = flower;
			}
			leaving = true;
		}
		else if ((item >= kRadioFlower1) && (item <= kRadioFlower6))
		{
			flower = item;
			SelectFromRadioGroup(infoDial, flower, kRadioFlower1, kRadioFlower6);
		}
		else if (item == kFlowerCancel)
		{
			leaving = true;
		}
		else if (item == 13)			// Linked From? button.
		{
			flower -= kRadioFlower1;
			if (flower != thisRoom->objects[objActive].data.i.pict)
			{
				InvalWindowRect(mainWindow, &thisRoom->objects[objActive].data.i.bounds);
				thisRoom->objects[objActive].data.i.bounds.right =
						thisRoom->objects[objActive].data.i.bounds.left +
						RectWide(&flowerSrc[flower]);
				thisRoom->objects[objActive].data.i.bounds.top =
						thisRoom->objects[objActive].data.i.bounds.bottom -
						RectTall(&flowerSrc[flower]);
				thisRoom->objects[objActive].data.i.pict = flower;
				InvalWindowRect(mainWindow, &thisRoom->objects[objActive].data.i.bounds);
				GetThisRoomsObjRects();
				ReadyBackground(thisRoom->background, thisRoom->tiles);
				DrawThisRoomsObjects();
				fileDirty = true;
				UpdateMenus(false);
				wasFlower = flower;
			}
			leaving = true;
			doReturn = true;
		}
	}

	DisposeDialog(infoDial);
	DisposeModalFilterUPP(flowerFilterUPP);

	if (doReturn)
	{
		GoToObjectInRoomNum(retroLinkList[objActive].object,
				retroLinkList[objActive].room);
	}
#endif
}

//--------------------------------------------------------------  DoObjectInfo

void DoObjectInfo (HWND hwndOwner)
{
	if ((objActive == kInitialGliderSelected) ||
			(objActive == kLeftGliderSelected) ||
			(objActive == kRightGliderSelected))
	{
		DoFurnitureObjectInfo(hwndOwner);
		return;
	}

	switch (thisRoom->objects[objActive].what)
	{
		case kFloorVent:
		case kCeilingVent:
		case kFloorBlower:
		case kCeilingBlower:
		case kSewerGrate:
		case kLeftFan:
		case kRightFan:
		case kTaper:
		case kCandle:
		case kStubby:
		case kTiki:
		case kBBQ:
		case kInvisBlower:
		case kGrecoVent:
		case kSewerBlower:
		case kLiftArea:
		DoBlowerObjectInfo(hwndOwner);
		break;

		case kTable:
		case kShelf:
		case kCabinet:
		case kFilingCabinet:
		case kWasteBasket:
		case kMilkCrate:
		case kCounter:
		case kDresser:
		case kDeckTable:
		case kStool:
		case kTrunk:
		case kInvisObstacle:
		case kManhole:
		case kBooks:
		case kInvisBounce:
		case kRedClock:
		case kBlueClock:
		case kYellowClock:
		case kCuckoo:
		case kPaper:
		case kBattery:
		case kBands:
		case kFoil:
		case kStar:
		case kSparkle:
		case kHelium:
		case kSlider:
		case kUpStairs:
		case kDownStairs:
		case kDoorInLf:
		case kDoorInRt:
		case kDoorExRt:
		case kDoorExLf:
		case kWindowInLf:
		case kWindowInRt:
		case kWindowExRt:
		case kWindowExLf:
		case kCinderBlock:
		case kFlowerBox:
		case kCDs:
		case kGuitar:
		case kStereo:
		case kCobweb:
		case kOzma:
		case kMirror:
		case kMousehole:
		case kFireplace:
		case kWallWindow:
		case kBear:
		case kCalendar:
		case kVase1:
		case kVase2:
		case kBulletin:
		case kCloud:
		case kFaucet:
		case kRug:
		case kChimes:
		DoFurnitureObjectInfo(hwndOwner);
		break;

		case kGreaseRt:
		case kGreaseLf:
		DoGreaseObjectInfo(hwndOwner);
		break;

		case kInvisBonus:
		DoInvisBonusObjectInfo(hwndOwner);
		break;

		case kMailboxLf:
		case kMailboxRt:
		case kFloorTrans:
		case kCeilingTrans:
		case kInvisTrans:
		case kDeluxeTrans:
		DoTransObjectInfo(hwndOwner, thisRoom->objects[objActive].what);
		break;

		case kLightSwitch:
		case kMachineSwitch:
		case kThermostat:
		case kPowerSwitch:
		case kKnifeSwitch:
		case kInvisSwitch:
		DoSwitchObjectInfo(hwndOwner);
		break;

		case kTrigger:
		case kLgTrigger:
		DoTriggerObjectInfo(hwndOwner);
		break;

		case kCeilingLight:
		case kLightBulb:
		case kTableLamp:
		case kHipLamp:
		case kDecoLamp:
		case kFlourescent:
		case kTrackLight:
		case kInvisLight:
		DoLightObjectInfo(hwndOwner);
		break;

		case kShredder:
		case kToaster:
		case kMacPlus:
		case kTV:
		case kCoffee:
		case kOutlet:
		case kVCR:
		DoApplianceObjectInfo(hwndOwner, thisRoom->objects[objActive].what);
		break;

		case kMicrowave:
		DoMicrowaveObjectInfo(hwndOwner);
		break;

		case kBalloon:
		case kCopterLf:
		case kCopterRt:
		case kDartLf:
		case kDartRt:
		case kBall:
		case kDrip:
		case kFish:
		DoEnemyObjectInfo(hwndOwner, thisRoom->objects[objActive].what);
		break;

		case kFlower:
		DoFlowerObjectInfo(hwndOwner);
		break;

		case kSoundTrigger:
		case kCustomPict:
		DoCustPictObjectInfo(hwndOwner);
		break;

		default:
		MessageBeep(MB_ICONWARNING);
		break;
	}
}

#endif

