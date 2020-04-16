
//============================================================================
//----------------------------------------------------------------------------
//									 Render.c
//----------------------------------------------------------------------------
//============================================================================


#include "Externs.h"
#include "Environ.h"
#include "MainWindow.h"
#include "Objects.h"
#include "Play.h"
#include "Player.h"
#include "RectUtils.h"
#include "Room.h"
#include "RubberBands.h"


#define kMaxGarbageRects		48


void DrawReflection (gliderPtr, Boolean);
void RenderFlames (void);
void RenderPendulums (void);
void RenderFlyingPoints (void);
void RenderSparkles (void);
void RenderStars (void);
void RenderBands (void);
void RenderShreds (void);
void CopyRectsQD (void);
void CopyRectsAssm (void);


Rect		work2MainRects[kMaxGarbageRects];
Rect		back2WorkRects[kMaxGarbageRects];
Rect		shieldRect;
RgnHandle	mirrorRgn;
Point		shieldPt;
SInt32		nextFrame;
SInt16		numWork2Main, numBack2Work;
Boolean		hasMirror;

extern	bandPtr		bands;
extern	sparklePtr	sparkles;
extern	flyingPtPtr	flyingPoints;
extern	flamePtr	flames, tikiFlames, bbqCoals;
extern	pendulumPtr	pendulums;
extern	starPtr		theStars;
extern	shredPtr	shreds;
extern	Rect		sparkleSrc[];
extern	Rect		pointsSrc[];
extern	Rect		bandRects[];
extern	savedType	savedMaps[];
extern	Rect		shadowSrc[], justRoomsRect, movieRect;
extern	SInt16		numBands, numStars, numShredded;
extern	SInt16		numSparkles, numFlyingPts, numPendulums, clockFrame;
extern	SInt16		numFlames, numSavedMaps, numTikiFlames, numCoals;
extern	Boolean		evenFrame, shadowVisible, twoPlayerGame, tvOn;


//==============================================================  Functions
//--------------------------------------------------------------  AddRectToWorkRects

void AddRectToWorkRects (Rect *theRect)
{
	if (numWork2Main < (kMaxGarbageRects - 1))
	{
		work2MainRects[numWork2Main] = *theRect;
		if (work2MainRects[numWork2Main].left < justRoomsRect.left)
			work2MainRects[numWork2Main].left = justRoomsRect.left;
		else if (work2MainRects[numWork2Main].right > justRoomsRect.right)
			work2MainRects[numWork2Main].right = justRoomsRect.right;
		if (work2MainRects[numWork2Main].top < justRoomsRect.top)
			work2MainRects[numWork2Main].top = justRoomsRect.top;
		else if (work2MainRects[numWork2Main].bottom > justRoomsRect.bottom)
			work2MainRects[numWork2Main].bottom = justRoomsRect.bottom;
		numWork2Main++;
	}
}

//--------------------------------------------------------------  AddRectToBackRects

void AddRectToBackRects (Rect *theRect)
{
	if (numBack2Work < (kMaxGarbageRects - 1))
	{
		back2WorkRects[numBack2Work] = *theRect;
		if (back2WorkRects[numBack2Work].left < 0)
			back2WorkRects[numBack2Work].left = 0;
		else if (back2WorkRects[numBack2Work].right > workSrcRect.right)
			back2WorkRects[numBack2Work].right = workSrcRect.right;
		if (back2WorkRects[numBack2Work].top < 0)
			back2WorkRects[numBack2Work].top = 0;
		else if (back2WorkRects[numBack2Work].bottom > workSrcRect.bottom)
			back2WorkRects[numBack2Work].bottom = workSrcRect.bottom;
		numBack2Work++;
	}
}

//--------------------------------------------------------------  AddRectToWorkRectsWhole

void AddRectToWorkRectsWhole (Rect *theRect)
{
	if (numWork2Main < (kMaxGarbageRects - 1))
	{
		if ((theRect->right <= workSrcRect.left) ||
				(theRect->bottom <= workSrcRect.top) ||
				(theRect->left >= workSrcRect.right) ||
				(theRect->top >= workSrcRect.bottom))
			return;

		work2MainRects[numWork2Main] = *theRect;

		if (work2MainRects[numWork2Main].left < workSrcRect.left)
			work2MainRects[numWork2Main].left = workSrcRect.left;
		else if (work2MainRects[numWork2Main].right > workSrcRect.right)
			work2MainRects[numWork2Main].right = workSrcRect.right;
		if (work2MainRects[numWork2Main].top < workSrcRect.top)
			work2MainRects[numWork2Main].top = workSrcRect.top;
		else if (work2MainRects[numWork2Main].bottom > workSrcRect.bottom)
			work2MainRects[numWork2Main].bottom = workSrcRect.bottom;

		if ((work2MainRects[numWork2Main].right ==
				work2MainRects[numWork2Main].left) ||
				(work2MainRects[numWork2Main].top ==
				work2MainRects[numWork2Main].bottom))
			return;

		numWork2Main++;
	}
}

//--------------------------------------------------------------  DrawReflection

void DrawReflection (gliderPtr thisGlider, Boolean oneOrTwo)
{
	return;
#if 0
	RgnHandle	wasClip;
	Rect		src, dest;
	short		which;

	if (thisGlider->dontDraw)
		return;

	if (thisGlider->facing == kFaceRight)
		which = 0;
	else
		which = 1;

	dest = thisGlider->dest;
	QOffsetRect(&dest, playOriginH - 20, playOriginV - 16);

	wasClip = NewRgn();
	if (wasClip == nil)
		return;

	SetPort((GrafPtr)workSrcMap);
	GetClip(wasClip);
	SetClip(mirrorRgn);

	if (oneOrTwo)
	{
		if (showFoil)
			CopyMask((BitMap *)*GetGWorldPixMap(glid2SrcMap),
					(BitMap *)*GetGWorldPixMap(glidMaskMap),
					(BitMap *)*GetGWorldPixMap(workSrcMap),
					&thisGlider->src, &thisGlider->mask, &dest);
		else
			CopyMask((BitMap *)*GetGWorldPixMap(glidSrcMap),
					(BitMap *)*GetGWorldPixMap(glidMaskMap),
					(BitMap *)*GetGWorldPixMap(workSrcMap),
					&thisGlider->src, &thisGlider->mask, &dest);
	}
	else
	{
		CopyMask((BitMap *)*GetGWorldPixMap(glid2SrcMap),
				(BitMap *)*GetGWorldPixMap(glidMaskMap),
				(BitMap *)*GetGWorldPixMap(workSrcMap),
				&thisGlider->src, &thisGlider->mask, &dest);
	}

	SetClip(wasClip);
	DisposeRgn(wasClip);

	src =thisGlider->whole;
	QOffsetRect(&src, playOriginH - 20, playOriginV - 16);
	AddRectToWorkRects(&src);
	AddRectToBackRects(&dest);
#endif
}

//--------------------------------------------------------------  RenderFlames

void RenderFlames (void)
{
	SInt16		i;

	if ((numFlames == 0) && (numTikiFlames == 0) && (numCoals == 0))
		return;

	for (i = 0; i < numFlames; i++)
	{
		flames[i].mode++;
		flames[i].src.top += 15;
		flames[i].src.bottom += 15;
		if (flames[i].mode >= kNumCandleFlames)
		{
			flames[i].mode = 0;
			flames[i].src.top = 0;
			flames[i].src.bottom = 15;
		}

		Mac_CopyBits(savedMaps[flames[i].who].map, workSrcMap,
				&flames[i].src, &flames[i].dest, srcCopy, nil);

		AddRectToWorkRects(&flames[i].dest);
	}

	for (i = 0; i < numTikiFlames; i++)
	{
		tikiFlames[i].mode++;
		tikiFlames[i].src.top += 10;
		tikiFlames[i].src.bottom += 10;
		if (tikiFlames[i].mode >= kNumTikiFlames)
		{
			tikiFlames[i].mode = 0;
			tikiFlames[i].src.top = 0;
			tikiFlames[i].src.bottom = 10;
		}

		Mac_CopyBits(savedMaps[tikiFlames[i].who].map, workSrcMap,
				&tikiFlames[i].src, &tikiFlames[i].dest, srcCopy, nil);

		AddRectToWorkRects(&tikiFlames[i].dest);
	}

	for (i = 0; i < numCoals; i++)
	{
		bbqCoals[i].mode++;
		bbqCoals[i].src.top += 9;
		bbqCoals[i].src.bottom += 9;
		if (bbqCoals[i].mode >= kNumBBQCoals)
		{
			bbqCoals[i].mode = 0;
			bbqCoals[i].src.top = 0;
			bbqCoals[i].src.bottom = 9;
		}

		Mac_CopyBits(savedMaps[bbqCoals[i].who].map, workSrcMap,
				&bbqCoals[i].src, &bbqCoals[i].dest, srcCopy, nil);

		AddRectToWorkRects(&bbqCoals[i].dest);
	}
}

//--------------------------------------------------------------  RenderPendulums

void RenderPendulums (void)
{
	SInt16		i;
	Boolean		playedTikTok;

	playedTikTok = false;

	if (numPendulums == 0)
		return;

	clockFrame++;
	if ((clockFrame == 10) || (clockFrame == 15))
	{
		if (clockFrame >= 15)
			clockFrame = 0;

		for (i = 0; i < numPendulums; i++)
		{
			if (pendulums[i].active)
			{
				if (pendulums[i].toOrFro)
				{
					pendulums[i].mode++;
					pendulums[i].src.top += 28;
					pendulums[i].src.bottom += 28;

					if (pendulums[i].mode >= 2)
					{
						pendulums[i].toOrFro = !pendulums[i].toOrFro;
						if (!playedTikTok)
						{
							PlayPrioritySound(kTikSound, kTikPriority);
							playedTikTok = true;
						}
					}
				}
				else
				{
					pendulums[i].mode--;
					pendulums[i].src.top -= 28;
					pendulums[i].src.bottom -= 28;

					if (pendulums[i].mode <= 0)
					{
						pendulums[i].toOrFro = !pendulums[i].toOrFro;
						if (!playedTikTok)
						{
							PlayPrioritySound(kTokSound, kTokPriority);
							playedTikTok = true;
						}
					}
				}

				Mac_CopyBits(savedMaps[pendulums[i].who].map, workSrcMap,
						&pendulums[i].src, &pendulums[i].dest, srcCopy, nil);

				AddRectToWorkRects(&pendulums[i].dest);
			}
		}
	}
}

//--------------------------------------------------------------  RenderFlyingPoints

void RenderFlyingPoints (void)
{
	SInt16		i;

	if (numFlyingPts == 0)
		return;

	for (i = 0; i < kMaxFlyingPts; i++)
	{
		if (flyingPoints[i].mode != -1)
		{
			if (flyingPoints[i].mode > flyingPoints[i].stop)
			{
				flyingPoints[i].mode = flyingPoints[i].start;
				flyingPoints[i].loops++;
			}

			if (flyingPoints[i].loops >= kMaxFlyingPointsLoop)
			{
				AddRectToWorkRects(&flyingPoints[i].dest);
				flyingPoints[i].mode = -1;
				numFlyingPts--;
			}
			else
			{
				flyingPoints[i].dest.left += flyingPoints[i].hVel;
				flyingPoints[i].dest.right += flyingPoints[i].hVel;

				if (flyingPoints[i].hVel > 0)
					flyingPoints[i].whole.right = flyingPoints[i].dest.right;
				else
					flyingPoints[i].whole.left = flyingPoints[i].dest.left;

				flyingPoints[i].dest.top += flyingPoints[i].vVel;
				flyingPoints[i].dest.bottom += flyingPoints[i].vVel;

				if (flyingPoints[i].vVel > 0)
					flyingPoints[i].whole.bottom = flyingPoints[i].dest.bottom;
				else
					flyingPoints[i].whole.top = flyingPoints[i].dest.top;

				Mac_CopyMask(pointsSrcMap, pointsMaskMap, workSrcMap,
						&pointsSrc[flyingPoints[i].mode],
						&pointsSrc[flyingPoints[i].mode],
						&flyingPoints[i].dest);

				AddRectToWorkRects(&flyingPoints[i].whole);
				AddRectToBackRects(&flyingPoints[i].dest);
				flyingPoints[i].whole = flyingPoints[i].dest;
				flyingPoints[i].mode++;
			}
		}
	}
}

//--------------------------------------------------------------  RenderSparkles

void RenderSparkles (void)
{
	SInt16		i;

	if (numSparkles == 0)
		return;

	for (i = 0; i < kMaxSparkles; i++)
	{
		if (sparkles[i].mode != -1)
		{
			if (sparkles[i].mode >= kNumSparkleModes)
			{
				AddRectToWorkRects(&sparkles[i].bounds);
				sparkles[i].mode = -1;
				numSparkles--;
			}
			else
			{
				Mac_CopyMask(bonusSrcMap, bonusMaskMap, workSrcMap,
						&sparkleSrc[sparkles[i].mode],
						&sparkleSrc[sparkles[i].mode],
						&sparkles[i].bounds);

				AddRectToWorkRects(&sparkles[i].bounds);
				AddRectToBackRects(&sparkles[i].bounds);
				sparkles[i].mode++;
			}
		}
	}
}

//--------------------------------------------------------------  RenderStars

void RenderStars (void)
{
	SInt16		i;

	if (numStars == 0)
		return;

	for (i = 0; i < numStars; i++)
	{
		if (theStars[i].mode != -1)
		{
			theStars[i].mode++;
			theStars[i].src.top += 31;
			theStars[i].src.bottom += 31;
			if (theStars[i].mode >= 6)
			{
				theStars[i].mode = 0;
				theStars[i].src.top = 0;
				theStars[i].src.bottom = 31;
			}

			Mac_CopyBits(savedMaps[theStars[i].who].map, workSrcMap,
					&theStars[i].src, &theStars[i].dest, srcCopy, nil);

			AddRectToWorkRects(&theStars[i].dest);
		}
	}
}

//--------------------------------------------------------------  RenderGlider

void RenderGlider (gliderPtr thisGlider, Boolean oneOrTwo)
{
	Rect		src, dest;
	SInt16		which;

	if (thisGlider->dontDraw)
		return;

	if (thisGlider->facing == kFaceRight)
		which = 0;
	else
		which = 1;

	if (shadowVisible)
	{
		dest = thisGlider->destShadow;
		QOffsetRect(&dest, playOriginH, playOriginV);

		if ((thisGlider->mode == kGliderComingUp) ||
				(thisGlider->mode == kGliderGoingDown))
		{
			src = shadowSrc[which];
			src.right = src.left + (dest.right - dest.left);

			Mac_CopyMask(shadowSrcMap, shadowMaskMap, workSrcMap,
					&src, &src, &dest);
		}
		else if (thisGlider->mode == kGliderComingDown)
		{
			src = shadowSrc[which];
			src.left = src.right - (dest.right - dest.left);

			Mac_CopyMask(shadowSrcMap, shadowMaskMap, workSrcMap,
					&src, &src, &dest);
		}
		else
			Mac_CopyMask(shadowSrcMap, shadowMaskMap, workSrcMap,
					&shadowSrc[which], &shadowSrc[which], &dest);
		src = thisGlider->wholeShadow;
		QOffsetRect(&src, playOriginH, playOriginV);
		AddRectToWorkRects(&src);
		AddRectToBackRects(&dest);
	}

	dest = thisGlider->dest;
	QOffsetRect(&dest, playOriginH, playOriginV);

	if (oneOrTwo)
	{
		if ((!twoPlayerGame) && (showFoil))
			Mac_CopyMask(glid2SrcMap, glidMaskMap, workSrcMap,
					&thisGlider->src, &thisGlider->mask, &dest);
		else
			Mac_CopyMask(glidSrcMap, glidMaskMap, workSrcMap,
					&thisGlider->src, &thisGlider->mask, &dest);
	}
	else
	{
		Mac_CopyMask(glid2SrcMap, glidMaskMap, workSrcMap,
				&thisGlider->src, &thisGlider->mask, &dest);
	}

	src = thisGlider->whole;
	QOffsetRect(&src, playOriginH, playOriginV);
	AddRectToWorkRects(&src);
	AddRectToBackRects(&dest);
}

//--------------------------------------------------------------  RenderBands

void RenderBands (void)
{
	Rect		dest;
	SInt16		i;

	if (numBands == 0)
		return;

	for (i = 0; i < numBands; i++)
	{
		dest = bands[i].dest;
		QOffsetRect(&dest, playOriginH, playOriginV);
		Mac_CopyMask(bandsSrcMap, bandsMaskMap, workSrcMap,
				&bandRects[bands[i].mode],
				&bandRects[bands[i].mode], &dest);

		AddRectToWorkRects(&dest);
		AddRectToBackRects(&dest);
	}
}

//--------------------------------------------------------------  RenderShreds

void RenderShreds (void)
{
	Rect		src, dest;
	SInt16		i, high;

	if (numShredded > 0)
	{
		for (i = 0; i < numShredded; i++)
		{
			if (shreds[i].frame == 0)
			{
				shreds[i].bounds.bottom += 1;
				high = shreds[i].bounds.bottom - shreds[i].bounds.top;
				if (high >= 35)
					shreds[i].frame = 1;
				src = shredSrcRect;
				src.top = src.bottom - high;
				dest = shreds[i].bounds;
				QOffsetRect(&dest, playOriginH, playOriginV);
				Mac_CopyMask(shredSrcMap, shredMaskMap, workSrcMap,
						&src, &src, &dest);
				AddRectToBackRects(&dest);
				dest.top--;
				AddRectToWorkRects(&dest);
				PlayPrioritySound(kShredSound, kShredPriority);
			}
			else if (shreds[i].frame < 20)
			{
				shreds[i].bounds.top += 4;
				shreds[i].bounds.bottom += 4;
				dest = shreds[i].bounds;
				QOffsetRect(&dest, playOriginH, playOriginV);
				shreds[i].frame++;
				if (shreds[i].frame < 20)
				{
					Mac_CopyMask(shredSrcMap, shredMaskMap, workSrcMap,
							&shredSrcRect, &shredSrcRect, &dest);
				}
				else
				{
					AddSparkle(&shreds[i].bounds);
					PlayPrioritySound(kFadeOutSound, kFadeOutPriority);
				}
				AddRectToBackRects(&dest);
				dest.top -= 4;
				AddRectToWorkRects(&dest);
			}
		}
	}
}

//--------------------------------------------------------------  CopyRectsQD

void CopyRectsQD (void)
{
	HDC			mainWindowDC;
	SInt16		i;

	mainWindowDC = GetMainWindowDC();
	for (i = 0; i < numWork2Main; i++)
	{
		Mac_CopyBits(workSrcMap, mainWindowDC,
				&work2MainRects[i], &work2MainRects[i],
				srcCopy, nil);
	}
	ReleaseMainWindowDC(mainWindowDC);

	for (i = 0; i < numBack2Work; i++)
	{
		Mac_CopyBits(backSrcMap, workSrcMap,
				&back2WorkRects[i], &back2WorkRects[i],
				srcCopy, nil);
	}
}

//--------------------------------------------------------------  RenderFrame

void RenderFrame (void)
{
	return;
#if 0
	if (hasMirror)
	{
		DrawReflection(&theGlider, true);
		if (twoPlayerGame)
			DrawReflection(&theGlider2, false);
	}
	HandleGrease();
	RenderPendulums();
	if (evenFrame)
		RenderFlames();
	else
		RenderStars();
	RenderDynamics();
	RenderFlyingPoints();
	RenderSparkles();
	RenderGlider(&theGlider, true);
	if (twoPlayerGame)
		RenderGlider(&theGlider2, false);
	RenderShreds();
	RenderBands();

	while (TickCount() < nextFrame)
	{
	}
	nextFrame = TickCount() + kTicksPerFrame;

	CopyRectsQD();

	numWork2Main = 0;
	numBack2Work = 0;
#endif
}

//--------------------------------------------------------------  InitGarbageRects

void InitGarbageRects (void)
{
	SInt16		i;

	numWork2Main = 0;
	numBack2Work = 0;

	numSparkles = 0;
	for (i = 0; i < kMaxSparkles; i++)
		sparkles[i].mode = -1;

	numFlyingPts = 0;
	for (i = 0; i < kMaxFlyingPts; i++)
		flyingPoints[i].mode = -1;

	nextFrame = MillisToTicks(GetTickCount()) + kTicksPerFrame;
}

//--------------------------------------------------------------  CopyRectBackToWork

void CopyRectBackToWork (Rect *theRect)
{
	Mac_CopyBits(backSrcMap, workSrcMap,
			theRect, theRect, srcCopy, nil);
}

//--------------------------------------------------------------  CopyRectWorkToBack

void CopyRectWorkToBack (Rect *theRect)
{
	Mac_CopyBits(workSrcMap, backSrcMap,
			theRect, theRect, srcCopy, nil);
}

//--------------------------------------------------------------  CopyRectWorkToMain

void CopyRectWorkToMain (Rect *theRect)
{
	HDC			mainWindowDC;

	mainWindowDC = GetMainWindowDC();
	Mac_CopyBits(workSrcMap, mainWindowDC,
			theRect, theRect, srcCopy, nil);
	ReleaseMainWindowDC(mainWindowDC);
}

//--------------------------------------------------------------  CopyRectMainToWork

void CopyRectMainToWork (Rect *theRect)
{
	HDC			mainWindowDC;

	mainWindowDC = GetMainWindowDC();
	Mac_CopyBits(mainWindowDC, workSrcMap,
			theRect, theRect, srcCopy, nil);
	ReleaseMainWindowDC(mainWindowDC);
}

//--------------------------------------------------------------  CopyRectMainToBack

void CopyRectMainToBack (Rect *theRect)
{
	return;
#if 0
	CopyBits(GetPortBitMapForCopyBits(GetWindowPort(mainWindow)),
			(BitMap *)*GetGWorldPixMap(backSrcMap),
			theRect, theRect, srcCopy, nil);
#endif
}

//--------------------------------------------------------------  AddToMirrorRegion

void AddToMirrorRegion (Rect *theRect)
{
	return;
#if 0
	RgnHandle	tempRgn;

	if (mirrorRgn == nil)
	{
		mirrorRgn = NewRgn();
		if (mirrorRgn != nil)
			RectRgn(mirrorRgn, theRect);
	}
	else
	{
		tempRgn = NewRgn();
		if (tempRgn != nil)
		{
			RectRgn(tempRgn, theRect);
			UnionRgn(mirrorRgn, tempRgn, mirrorRgn);
			DisposeRgn(tempRgn);
		}
	}
	hasMirror = true;
#endif
}

//--------------------------------------------------------------  ZeroMirrorRegion

void ZeroMirrorRegion (void)
{
	return;
#if 0
	if (mirrorRgn != nil)
		DisposeRgn(mirrorRgn);
	mirrorRgn = nil;
	hasMirror = false;
#endif
}

