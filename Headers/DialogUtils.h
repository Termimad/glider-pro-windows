//============================================================================
//----------------------------------------------------------------------------
//								DialogUtils.h
//----------------------------------------------------------------------------
//============================================================================


//#include <Dialogs.h>
#include "Macintosh.h"


void BringUpDialog (DialogPtr *theDialog, SInt16 dialogID);
//void GetPutDialogCorner (Point *);
//void GetGetDialogCorner (Point *);
//void CenterDialog (SInt16);
void GetDialogRect (Rect *, SInt16);
//void TrueCenterDialog (SInt16);
//void CenterAlert (SInt16);
//void ZoomOutDialogRect (SInt16);
//void ZoomOutAlertRect (SInt16);
void FlashDialogButton (DialogPtr, SInt16);
void DrawDefaultButton (DialogPtr);
void GetDialogString (DialogPtr, SInt16, StringPtr);
void SetDialogString (DialogPtr, SInt16, StringPtr);
SInt16 GetDialogStringLen (DialogPtr, SInt16);
void GetDialogItemValue (DialogPtr, SInt16, SInt16 *);
void SetDialogItemValue (DialogPtr, SInt16, SInt16);
void ToggleDialogItemValue (DialogPtr, SInt16);
void SetDialogNumToStr (DialogPtr, SInt16, SInt32);
void GetDialogNumFromStr (DialogPtr, SInt16, SInt32 *);
void GetDialogItemRect (DialogPtr, SInt16, Rect *);
void SetDialogItemRect (DialogPtr, SInt16, Rect *);
void OffsetDialogItemRect (DialogPtr, SInt16, SInt16, SInt16);
void SelectFromRadioGroup (DialogPtr, SInt16, SInt16, SInt16);
//void AddMenuToPopUp (DialogPtr, SInt16, MenuHandle);
void GetPopUpMenuValue (DialogPtr, SInt16, SInt16 *);
void SetPopUpMenuValue (DialogPtr, SInt16, SInt16);
void MyEnableControl(DialogPtr, SInt16);
void MyDisableControl(DialogPtr, SInt16);
void DrawDialogUserText (DialogPtr, SInt16, StringPtr, Boolean);
void DrawDialogUserText2 (DialogPtr, SInt16, StringPtr);
void LoadDialogPICT (DialogPtr, SInt16, SInt16);
void FrameDialogItem (DialogPtr, SInt16);
void FrameDialogItemC (DialogPtr, SInt16, SInt32);
void FrameOvalDialogItem (DialogPtr, SInt16);
void BorderDialogItem (DialogPtr, SInt16, SInt16);
void ShadowDialogItem (DialogPtr, SInt16, SInt16);
void EraseDialogItem (DialogPtr, SInt16);
