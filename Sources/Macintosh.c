#include "Macintosh.h"
#include "WinAPI.h"
#include <inttypes.h>
#include <string.h>
#include <strsafe.h>


//--------------------------------------------------------------  CopyBits
// Copy some portion of a bitmap from one graphics port to another.

void Mac_CopyBits(
	HDC srcBits,
	HDC dstBits,
	const Rect* srcRect,
	const Rect* dstRect,
	SInt16 mode,
	HRGN maskRgn)
{
	INT xSrc, ySrc, wSrc, hSrc;
	INT xDst, yDst, wDst, hDst;

	if (srcRect->left >= srcRect->right || srcRect->top >= srcRect->bottom)
		return;
	if (dstRect->left >= dstRect->right || dstRect->top >= dstRect->bottom)
		return;
	if (mode != srcCopy && mode != srcXor && mode != transparent)
		return;

	xSrc = srcRect->left;
	ySrc = srcRect->top;
	wSrc = srcRect->right - srcRect->left;
	hSrc = srcRect->bottom - srcRect->top;
	xDst = dstRect->left;
	yDst = dstRect->top;
	wDst = dstRect->right - dstRect->left;
	hDst = dstRect->bottom - dstRect->top;

	SaveDC(dstBits);
	if (maskRgn != NULL)
		SelectClipRgn(dstBits, maskRgn);

	switch (mode)
	{
		case srcCopy:
		StretchBlt(dstBits, xDst, yDst, wDst, hDst,
				srcBits, xSrc, ySrc, wSrc, hSrc, SRCCOPY);
		break;

		case srcXor:
		// invert the destination where the source is black
		// and keep the destination where the source is white
		BitBlt(dstBits, xDst, yDst, wDst, hDst, NULL, 0, 0, DSTINVERT);
		SetBkColor(dstBits, RGB(0xFF, 0xFF, 0xFF));
		SetTextColor(dstBits, RGB(0x00, 0x00, 0x00));
		StretchBlt(dstBits, xDst, yDst, wDst, hDst,
				srcBits, xSrc, ySrc, wSrc, hSrc, SRCINVERT);
		break;

		case transparent:
		TransparentBlt(dstBits, xDst, yDst, wDst, hDst,
				srcBits, xSrc, ySrc, wSrc, hSrc, GetBkColor(dstBits));
		break;
	}

	RestoreDC(dstBits, -1);
}

//--------------------------------------------------------------  CopyBits
// Copy some portion of a bitmap from one graphics port to another,
// with a mask to specify how much of each pixel is copied over.

void Mac_CopyMask(
	HDC srcBits,
	HDC maskBits,
	HDC dstBits,
	const Rect* srcRect,
	const Rect* maskRect,
	const Rect* dstRect)
{
	// TODO: add support for masks where pixels are between black and white
	HDC newSrcBits;
	HBITMAP newSrcBitmap, hbmPrev;
	INT xSrc, ySrc, wSrc, hSrc;
	INT xMask, yMask, wMask, hMask;
	INT xDst, yDst, wDst, hDst;

	if (srcRect->left >= srcRect->right || srcRect->top >= srcRect->bottom)
		return;
	if (maskRect->left >= maskRect->right || maskRect->top >= maskRect->bottom)
		return;
	if (dstRect->left >= dstRect->right || dstRect->top >= dstRect->bottom)
		return;

	xSrc = srcRect->left;
	ySrc = srcRect->top;
	wSrc = srcRect->right - srcRect->left;
	hSrc = srcRect->bottom - srcRect->top;
	xMask = maskRect->left;
	yMask = maskRect->top;
	wMask = maskRect->right - maskRect->left;
	hMask = maskRect->bottom - maskRect->top;
	xDst = dstRect->left;
	yDst = dstRect->top;
	wDst = dstRect->right - dstRect->left;
	hDst = dstRect->bottom - dstRect->top;

	SaveDC(dstBits);
	// make sure that monochrome masks are colorized correctly
	SetTextColor(dstBits, RGB(0x00, 0x00, 0x00));
	SetBkColor(dstBits, RGB(0xFF, 0xFF, 0xFF));
	// apply the mask to the destination
	StretchBlt(dstBits, xDst, yDst, wDst, hDst,
		maskBits, xMask, yMask, wMask, hMask, SRCAND); // DSa
	// create temporary drawing surface to adjust the source bits
	newSrcBits = CreateCompatibleDC(srcBits);
	SetTextColor(newSrcBits, RGB(0x00, 0x00, 0x00));
	SetBkColor(newSrcBits, RGB(0xFF, 0xFF, 0xFF));
	newSrcBitmap = CreateCompatibleBitmap(srcBits, wSrc, hSrc);
	hbmPrev = SelectObject(newSrcBits, newSrcBitmap);
	BitBlt(newSrcBits, 0, 0, wSrc, hSrc, srcBits, xSrc, ySrc, SRCCOPY);
	// black out unwanted source bits with the inverted mask
	StretchBlt(newSrcBits, 0, 0, wSrc, hSrc,
		maskBits, xMask, yMask, wMask, hMask, 0x00220326); // DSna
	// apply adjusted source bits to the destination
	StretchBlt(dstBits, xDst, yDst, wDst, hDst,
		newSrcBits, 0, 0, wSrc, hSrc, SRCPAINT); // DSo
	// clean up
	RestoreDC(dstBits, -1);
	SelectObject(newSrcBits, hbmPrev);
	DeleteObject(newSrcBitmap);
	DeleteDC(newSrcBits);
}

//--------------------------------------------------------------  GetDateTime
// Retrieve the number of seconds since midnight, January 1, 1904.
// The time difference is in terms of the local time zone.

void Mac_GetDateTime(UInt32 *secs)
{
	SYSTEMTIME localMacEpoch, utcMacEpoch;
	FILETIME epochTime, currentTime;
	ULARGE_INTEGER epochSecs, currentSecs;

	if (secs == NULL)
		return;
	*secs = 0;

	localMacEpoch.wYear = 1904;
	localMacEpoch.wMonth = 1;
	localMacEpoch.wDayOfWeek = 5; // Friday
	localMacEpoch.wDay = 1;
	localMacEpoch.wHour = 0;
	localMacEpoch.wMinute = 0;
	localMacEpoch.wSecond = 0;
	localMacEpoch.wMilliseconds = 0;
	if (!TzSpecificLocalTimeToSystemTime(NULL, &localMacEpoch, &utcMacEpoch))
		return;

	if (!SystemTimeToFileTime(&utcMacEpoch, &epochTime))
		return;
	epochSecs.LowPart = epochTime.dwLowDateTime;
	epochSecs.HighPart = epochTime.dwHighDateTime;
	epochSecs.QuadPart /= (10 * 1000 * 1000);

	GetSystemTimeAsFileTime(&currentTime);
	currentSecs.LowPart = currentTime.dwLowDateTime;
	currentSecs.HighPart = currentTime.dwHighDateTime;
	currentSecs.QuadPart /= (10 * 1000 * 1000);

	*secs = (UInt32)(currentSecs.QuadPart - epochSecs.QuadPart);
}

//--------------------------------------------------------------  InsetRect
// Shrink or expand the given rectangle. The rectangle's sides
// are moved inwards by the given deltas. To expand the rectangle,
// call this function with negative delta values. If either the width
// or the height of the resulting rectangle is less than 1, then the
// rectangle is set to the empty rectangle (0,0,0,0).

void Mac_InsetRect(Rect *r, SInt16 dh, SInt16 dv)
{
	if (r == NULL)
		return;
	r->left += dh;
	r->top += dv;
	r->right -= dh;
	r->bottom -= dv;
	if (r->left > r->right || r->top > r->bottom)
	{
		r->left = 0;
		r->top = 0;
		r->right = 0;
		r->bottom = 0;
	}
}

//--------------------------------------------------------------  NumToString
// Convert the given number to a decimal string representation.
// The string is written to the given output string.

void Mac_NumToString(SInt32 theNum, Str255 *theString)
{
	CHAR buffer[sizeof("-2147483648")];
	size_t length;
	HRESULT hr;

	if (theString == NULL)
		return;
	(*theString)[0] = 0; // return an empty string if an error occurs.
	hr = StringCchPrintfA(buffer, ARRAYSIZE(buffer), "%" PRId32, theNum);
	if (FAILED(hr))
		return;
	hr = StringCchLengthA(buffer, ARRAYSIZE(buffer), &length);
	if (FAILED(hr))
		return;
	(*theString)[0] = length;
	memcpy(&(*theString)[1], buffer, length);
}

//--------------------------------------------------------------  PtInRect
// Check if the given point is enclosed within the given rectangle.
// Return nonzero (true) if the point is within the rectangle; otherwise
// return zero (false). The point is considered within the rectangle
// if it is within all four sides, or if it lies on the left or top side.
// The point is not considered within the rectangle if it lies on the
// right or bottom side (because its corresponding pixel would be
// outside the rectangle).

Boolean Mac_PtInRect(Point pt, const Rect *r)
{
	if (r == NULL)
		return false;
	return (pt.h >= r->left) && (pt.h < r->right) &&
			(pt.v >= r->top) && (pt.v < r->bottom);
}

//--------------------------------------------------------------  Global Data

// The eight basic QuickDraw colors
const RGBColor QDColors[] = {
	{ 0x0000, 0x0000, 0x0000 }, // black
	{ 0xFC00, 0xF37D, 0x052F }, // yellow
	{ 0xF2D7, 0x0856, 0x84EC }, // magenta
	{ 0xDD6B, 0x08C2, 0x06A2 }, // red
	{ 0x0241, 0xAB54, 0xEAFF }, // cyan
	{ 0x0000, 0x8000, 0x11B0 }, // green
	{ 0x0000, 0x0000, 0xD400 }, // blue
	{ 0xFFFF, 0xFFFF, 0xFFFF }, // white
};

// The default 4-bit Macintosh color palette
const RGBColor MacColor4[] = {
	{ 0xFFFF, 0xFFFF, 0xFFFF }, // white
	{ 0xFFFF, 0xFFFF, 0x0000 }, // yellow
	{ 0xFFFF, 0x6666, 0x0000 }, // orange
	{ 0xDDDD, 0x0000, 0x0000 }, // red
	{ 0xFFFF, 0x0000, 0x9999 }, // magenta
	{ 0x3333, 0x0000, 0x9999 }, // purple
	{ 0x0000, 0x0000, 0xCCCC }, // blue
	{ 0x0000, 0x9999, 0xFFFF }, // cyan
	{ 0x0000, 0xAAAA, 0x0000 }, // green
	{ 0x0000, 0x6666, 0x0000 }, // dark green
	{ 0x6666, 0x3333, 0x0000 }, // brown
	{ 0x9999, 0x6666, 0x3333 }, // tan
	{ 0xBBBB, 0xBBBB, 0xBBBB }, // light gray
	{ 0x8888, 0x8888, 0x8888 }, // medium gray
	{ 0x4444, 0x4444, 0x4444 }, // dark gray
	{ 0x0000, 0x0000, 0x0000 }, // black
};

// The default 8-bit Macintosh color palette
const RGBColor MacColor8[] = {
	{ 0xFFFF, 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF, 0xCCCC }, { 0xFFFF, 0xFFFF, 0x9999 },
	{ 0xFFFF, 0xFFFF, 0x6666 }, { 0xFFFF, 0xFFFF, 0x3333 }, { 0xFFFF, 0xFFFF, 0x0000 },
	{ 0xFFFF, 0xCCCC, 0xFFFF }, { 0xFFFF, 0xCCCC, 0xCCCC }, { 0xFFFF, 0xCCCC, 0x9999 },
	{ 0xFFFF, 0xCCCC, 0x6666 }, { 0xFFFF, 0xCCCC, 0x3333 }, { 0xFFFF, 0xCCCC, 0x0000 },
	{ 0xFFFF, 0x9999, 0xFFFF }, { 0xFFFF, 0x9999, 0xCCCC }, { 0xFFFF, 0x9999, 0x9999 },
	{ 0xFFFF, 0x9999, 0x6666 }, { 0xFFFF, 0x9999, 0x3333 }, { 0xFFFF, 0x9999, 0x0000 },
	{ 0xFFFF, 0x6666, 0xFFFF }, { 0xFFFF, 0x6666, 0xCCCC }, { 0xFFFF, 0x6666, 0x9999 },
	{ 0xFFFF, 0x6666, 0x6666 }, { 0xFFFF, 0x6666, 0x3333 }, { 0xFFFF, 0x6666, 0x0000 },
	{ 0xFFFF, 0x3333, 0xFFFF }, { 0xFFFF, 0x3333, 0xCCCC }, { 0xFFFF, 0x3333, 0x9999 },
	{ 0xFFFF, 0x3333, 0x6666 }, { 0xFFFF, 0x3333, 0x3333 }, { 0xFFFF, 0x3333, 0x0000 },
	{ 0xFFFF, 0x0000, 0xFFFF }, { 0xFFFF, 0x0000, 0xCCCC }, { 0xFFFF, 0x0000, 0x9999 },
	{ 0xFFFF, 0x0000, 0x6666 }, { 0xFFFF, 0x0000, 0x3333 }, { 0xFFFF, 0x0000, 0x0000 },

	{ 0xCCCC, 0xFFFF, 0xFFFF }, { 0xCCCC, 0xFFFF, 0xCCCC }, { 0xCCCC, 0xFFFF, 0x9999 },
	{ 0xCCCC, 0xFFFF, 0x6666 }, { 0xCCCC, 0xFFFF, 0x3333 }, { 0xCCCC, 0xFFFF, 0x0000 },
	{ 0xCCCC, 0xCCCC, 0xFFFF }, { 0xCCCC, 0xCCCC, 0xCCCC }, { 0xCCCC, 0xCCCC, 0x9999 },
	{ 0xCCCC, 0xCCCC, 0x6666 }, { 0xCCCC, 0xCCCC, 0x3333 }, { 0xCCCC, 0xCCCC, 0x0000 },
	{ 0xCCCC, 0x9999, 0xFFFF }, { 0xCCCC, 0x9999, 0xCCCC }, { 0xCCCC, 0x9999, 0x9999 },
	{ 0xCCCC, 0x9999, 0x6666 }, { 0xCCCC, 0x9999, 0x3333 }, { 0xCCCC, 0x9999, 0x0000 },
	{ 0xCCCC, 0x6666, 0xFFFF }, { 0xCCCC, 0x6666, 0xCCCC }, { 0xCCCC, 0x6666, 0x9999 },
	{ 0xCCCC, 0x6666, 0x6666 }, { 0xCCCC, 0x6666, 0x3333 }, { 0xCCCC, 0x6666, 0x0000 },
	{ 0xCCCC, 0x3333, 0xFFFF }, { 0xCCCC, 0x3333, 0xCCCC }, { 0xCCCC, 0x3333, 0x9999 },
	{ 0xCCCC, 0x3333, 0x6666 }, { 0xCCCC, 0x3333, 0x3333 }, { 0xCCCC, 0x3333, 0x0000 },
	{ 0xCCCC, 0x0000, 0xFFFF }, { 0xCCCC, 0x0000, 0xCCCC }, { 0xCCCC, 0x0000, 0x9999 },
	{ 0xCCCC, 0x0000, 0x6666 }, { 0xCCCC, 0x0000, 0x3333 }, { 0xCCCC, 0x0000, 0x0000 },

	{ 0x9999, 0xFFFF, 0xFFFF }, { 0x9999, 0xFFFF, 0xCCCC }, { 0x9999, 0xFFFF, 0x9999 },
	{ 0x9999, 0xFFFF, 0x6666 }, { 0x9999, 0xFFFF, 0x3333 }, { 0x9999, 0xFFFF, 0x0000 },
	{ 0x9999, 0xCCCC, 0xFFFF }, { 0x9999, 0xCCCC, 0xCCCC }, { 0x9999, 0xCCCC, 0x9999 },
	{ 0x9999, 0xCCCC, 0x6666 }, { 0x9999, 0xCCCC, 0x3333 }, { 0x9999, 0xCCCC, 0x0000 },
	{ 0x9999, 0x9999, 0xFFFF }, { 0x9999, 0x9999, 0xCCCC }, { 0x9999, 0x9999, 0x9999 },
	{ 0x9999, 0x9999, 0x6666 }, { 0x9999, 0x9999, 0x3333 }, { 0x9999, 0x9999, 0x0000 },
	{ 0x9999, 0x6666, 0xFFFF }, { 0x9999, 0x6666, 0xCCCC }, { 0x9999, 0x6666, 0x9999 },
	{ 0x9999, 0x6666, 0x6666 }, { 0x9999, 0x6666, 0x3333 }, { 0x9999, 0x6666, 0x0000 },
	{ 0x9999, 0x3333, 0xFFFF }, { 0x9999, 0x3333, 0xCCCC }, { 0x9999, 0x3333, 0x9999 },
	{ 0x9999, 0x3333, 0x6666 }, { 0x9999, 0x3333, 0x3333 }, { 0x9999, 0x3333, 0x0000 },
	{ 0x9999, 0x0000, 0xFFFF }, { 0x9999, 0x0000, 0xCCCC }, { 0x9999, 0x0000, 0x9999 },
	{ 0x9999, 0x0000, 0x6666 }, { 0x9999, 0x0000, 0x3333 }, { 0x9999, 0x0000, 0x0000 },

	{ 0x6666, 0xFFFF, 0xFFFF }, { 0x6666, 0xFFFF, 0xCCCC }, { 0x6666, 0xFFFF, 0x9999 },
	{ 0x6666, 0xFFFF, 0x6666 }, { 0x6666, 0xFFFF, 0x3333 }, { 0x6666, 0xFFFF, 0x0000 },
	{ 0x6666, 0xCCCC, 0xFFFF }, { 0x6666, 0xCCCC, 0xCCCC }, { 0x6666, 0xCCCC, 0x9999 },
	{ 0x6666, 0xCCCC, 0x6666 }, { 0x6666, 0xCCCC, 0x3333 }, { 0x6666, 0xCCCC, 0x0000 },
	{ 0x6666, 0x9999, 0xFFFF }, { 0x6666, 0x9999, 0xCCCC }, { 0x6666, 0x9999, 0x9999 },
	{ 0x6666, 0x9999, 0x6666 }, { 0x6666, 0x9999, 0x3333 }, { 0x6666, 0x9999, 0x0000 },
	{ 0x6666, 0x6666, 0xFFFF }, { 0x6666, 0x6666, 0xCCCC }, { 0x6666, 0x6666, 0x9999 },
	{ 0x6666, 0x6666, 0x6666 }, { 0x6666, 0x6666, 0x3333 }, { 0x6666, 0x6666, 0x0000 },
	{ 0x6666, 0x3333, 0xFFFF }, { 0x6666, 0x3333, 0xCCCC }, { 0x6666, 0x3333, 0x9999 },
	{ 0x6666, 0x3333, 0x6666 }, { 0x6666, 0x3333, 0x3333 }, { 0x6666, 0x3333, 0x0000 },
	{ 0x6666, 0x0000, 0xFFFF }, { 0x6666, 0x0000, 0xCCCC }, { 0x6666, 0x0000, 0x9999 },
	{ 0x6666, 0x0000, 0x6666 }, { 0x6666, 0x0000, 0x3333 }, { 0x6666, 0x0000, 0x0000 },

	{ 0x3333, 0xFFFF, 0xFFFF }, { 0x3333, 0xFFFF, 0xCCCC }, { 0x3333, 0xFFFF, 0x9999 },
	{ 0x3333, 0xFFFF, 0x6666 }, { 0x3333, 0xFFFF, 0x3333 }, { 0x3333, 0xFFFF, 0x0000 },
	{ 0x3333, 0xCCCC, 0xFFFF }, { 0x3333, 0xCCCC, 0xCCCC }, { 0x3333, 0xCCCC, 0x9999 },
	{ 0x3333, 0xCCCC, 0x6666 }, { 0x3333, 0xCCCC, 0x3333 }, { 0x3333, 0xCCCC, 0x0000 },
	{ 0x3333, 0x9999, 0xFFFF }, { 0x3333, 0x9999, 0xCCCC }, { 0x3333, 0x9999, 0x9999 },
	{ 0x3333, 0x9999, 0x6666 }, { 0x3333, 0x9999, 0x3333 }, { 0x3333, 0x9999, 0x0000 },
	{ 0x3333, 0x6666, 0xFFFF }, { 0x3333, 0x6666, 0xCCCC }, { 0x3333, 0x6666, 0x9999 },
	{ 0x3333, 0x6666, 0x6666 }, { 0x3333, 0x6666, 0x3333 }, { 0x3333, 0x6666, 0x0000 },
	{ 0x3333, 0x3333, 0xFFFF }, { 0x3333, 0x3333, 0xCCCC }, { 0x3333, 0x3333, 0x9999 },
	{ 0x3333, 0x3333, 0x6666 }, { 0x3333, 0x3333, 0x3333 }, { 0x3333, 0x3333, 0x0000 },
	{ 0x3333, 0x0000, 0xFFFF }, { 0x3333, 0x0000, 0xCCCC }, { 0x3333, 0x0000, 0x9999 },
	{ 0x3333, 0x0000, 0x6666 }, { 0x3333, 0x0000, 0x3333 }, { 0x3333, 0x0000, 0x0000 },

	{ 0x0000, 0xFFFF, 0xFFFF }, { 0x0000, 0xFFFF, 0xCCCC }, { 0x0000, 0xFFFF, 0x9999 },
	{ 0x0000, 0xFFFF, 0x6666 }, { 0x0000, 0xFFFF, 0x3333 }, { 0x0000, 0xFFFF, 0x0000 },
	{ 0x0000, 0xCCCC, 0xFFFF }, { 0x0000, 0xCCCC, 0xCCCC }, { 0x0000, 0xCCCC, 0x9999 },
	{ 0x0000, 0xCCCC, 0x6666 }, { 0x0000, 0xCCCC, 0x3333 }, { 0x0000, 0xCCCC, 0x0000 },
	{ 0x0000, 0x9999, 0xFFFF }, { 0x0000, 0x9999, 0xCCCC }, { 0x0000, 0x9999, 0x9999 },
	{ 0x0000, 0x9999, 0x6666 }, { 0x0000, 0x9999, 0x3333 }, { 0x0000, 0x9999, 0x0000 },
	{ 0x0000, 0x6666, 0xFFFF }, { 0x0000, 0x6666, 0xCCCC }, { 0x0000, 0x6666, 0x9999 },
	{ 0x0000, 0x6666, 0x6666 }, { 0x0000, 0x6666, 0x3333 }, { 0x0000, 0x6666, 0x0000 },
	{ 0x0000, 0x3333, 0xFFFF }, { 0x0000, 0x3333, 0xCCCC }, { 0x0000, 0x3333, 0x9999 },
	{ 0x0000, 0x3333, 0x6666 }, { 0x0000, 0x3333, 0x3333 }, { 0x0000, 0x3333, 0x0000 },
	{ 0x0000, 0x0000, 0xFFFF }, { 0x0000, 0x0000, 0xCCCC }, { 0x0000, 0x0000, 0x9999 },
	{ 0x0000, 0x0000, 0x6666 }, { 0x0000, 0x0000, 0x3333 },

	{ 0xEEEE, 0x0000, 0x0000 }, { 0xDDDD, 0x0000, 0x0000 },
	{ 0xBBBB, 0x0000, 0x0000 }, { 0xAAAA, 0x0000, 0x0000 },
	{ 0x8888, 0x0000, 0x0000 }, { 0x7777, 0x0000, 0x0000 },
	{ 0x5555, 0x0000, 0x0000 }, { 0x4444, 0x0000, 0x0000 },
	{ 0x2222, 0x0000, 0x0000 }, { 0x1111, 0x0000, 0x0000 },

	{ 0x0000, 0xEEEE, 0x0000 }, { 0x0000, 0xDDDD, 0x0000 },
	{ 0x0000, 0xBBBB, 0x0000 }, { 0x0000, 0xAAAA, 0x0000 },
	{ 0x0000, 0x8888, 0x0000 }, { 0x0000, 0x7777, 0x0000 },
	{ 0x0000, 0x5555, 0x0000 }, { 0x0000, 0x4444, 0x0000 },
	{ 0x0000, 0x2222, 0x0000 }, { 0x0000, 0x1111, 0x0000 },

	{ 0x0000, 0x0000, 0xEEEE }, { 0x0000, 0x0000, 0xDDDD },
	{ 0x0000, 0x0000, 0xBBBB }, { 0x0000, 0x0000, 0xAAAA },
	{ 0x0000, 0x0000, 0x8888 }, { 0x0000, 0x0000, 0x7777 },
	{ 0x0000, 0x0000, 0x5555 }, { 0x0000, 0x0000, 0x4444 },
	{ 0x0000, 0x0000, 0x2222 }, { 0x0000, 0x0000, 0x1111 },

	{ 0xEEEE, 0xEEEE, 0xEEEE }, { 0xDDDD, 0xDDDD, 0xDDDD },
	{ 0xBBBB, 0xBBBB, 0xBBBB }, { 0xAAAA, 0xAAAA, 0xAAAA },
	{ 0x8888, 0x8888, 0x8888 }, { 0x7777, 0x7777, 0x7777 },
	{ 0x5555, 0x5555, 0x5555 }, { 0x4444, 0x4444, 0x4444 },
	{ 0x2222, 0x2222, 0x2222 }, { 0x1111, 0x1111, 0x1111 },
	{ 0x0000, 0x0000, 0x0000 },
};
