#include "Macintosh.h"
#include "WinAPI.h"
#include <inttypes.h>
#include <string.h>
#include <strsafe.h>


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
