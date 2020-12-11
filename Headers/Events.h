#ifndef EVENTS_H_
#define EVENTS_H_

//============================================================================
//----------------------------------------------------------------------------
//                                  Events.h
//----------------------------------------------------------------------------
//============================================================================

#include "MacTypes.h"
#include "WinAPI.h"

extern UInt32 incrementModeTime;
extern Boolean doAutoDemo;
extern Boolean switchedOut;
extern Boolean ignoreDoubleClick;
extern HACCEL splashAccelTable;
extern HACCEL editAccelTable;

void HandleEvent (void);
void IgnoreThisClick (void);

#endif
