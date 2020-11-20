#define GP_USE_WINAPI_H

#include "Music.h"

//============================================================================
//----------------------------------------------------------------------------
//                                  Music.c
//----------------------------------------------------------------------------
//============================================================================


#include "Audio.h"
#include "GliderDefines.h"
#include "HouseIO.h"
#include "ResourceIDs.h"
#include "ResourceLoader.h"
#include "Utilities.h"
#include "WinAPI.h"

#include <stdlib.h>


#define kBaseBufferMusicID      2000
#define kMaxMusic               7
#define kLastMusicPiece         16
#define kLastGamePiece          6


void MusicCallBack (AudioChannel *channel, void *userdata);
OSErr LoadMusicSounds (void);
OSErr DumpMusicSounds (void);
OSErr OpenMusicChannel (void);
OSErr CloseMusicChannel (void);


Boolean isMusicOn;
Boolean isPlayMusicIdle;
Boolean isPlayMusicGame;
Boolean failedMusic;
Boolean dontLoadMusic;

static AudioChannel *musicChannel;
static CRITICAL_SECTION musicCriticalSection;
static WaveData theMusicData[kMaxMusic];
static SInt16 musicSoundID;
static SInt16 musicCursor;
static SInt16 musicScore[kLastMusicPiece];
static SInt16 gameScore[kLastGamePiece];
static SInt16 musicMode;


//==============================================================  Functions
//--------------------------------------------------------------  StartMusic

OSErr StartMusic (void)
{
	AudioEntry entry;
	SInt16 soundVolume;

	if (dontLoadMusic)
	{
		return noErr;
	}

	EnterCriticalSection(&musicCriticalSection);

	UnivGetSoundVolume(&soundVolume);
	if ((!isMusicOn) && (soundVolume != 0) && (!failedMusic))
	{
		AudioChannel_ClearAudio(musicChannel);

		entry.buffer = theMusicData[musicSoundID].dataBytes;
		entry.length = theMusicData[musicSoundID].dataLength;
		entry.endingCallback = NULL;
		entry.destroyCallback = NULL;
		entry.userdata = NULL;
		AudioChannel_QueueAudio(musicChannel, &entry);

		musicCursor++;
		if (musicCursor >= kLastMusicPiece)
		{
			musicCursor = 0;
		}
		musicSoundID = musicScore[musicCursor];

		entry.buffer = theMusicData[musicSoundID].dataBytes;
		entry.length = theMusicData[musicSoundID].dataLength;
		entry.endingCallback = MusicCallBack;
		entry.destroyCallback = NULL;
		entry.userdata = NULL;
		AudioChannel_QueueAudio(musicChannel, &entry);

		isMusicOn = true;
	}

	LeaveCriticalSection(&musicCriticalSection);

	return noErr;
}

//--------------------------------------------------------------  StopTheMusic

void StopTheMusic (void)
{
	if (dontLoadMusic)
	{
		return;
	}

	EnterCriticalSection(&musicCriticalSection);

	if ((isMusicOn) && (!failedMusic))
	{
		AudioChannel_ClearAudio(musicChannel);
		isMusicOn = false;
	}

	LeaveCriticalSection(&musicCriticalSection);
}

//--------------------------------------------------------------  ToggleMusicWhilePlaying

void ToggleMusicWhilePlaying (void)
{
	if (dontLoadMusic)
	{
		return;
	}

	if (isPlayMusicGame)
	{
		if (!isMusicOn)
		{
			StartMusic();
		}
	}
	else
	{
		if (isMusicOn)
		{
			StopTheMusic();
		}
	}
}

//--------------------------------------------------------------  SetMusicalPiece

void SetMusicalMode (SInt16 newMode)
{
	if (dontLoadMusic)
	{
		return;
	}

	EnterCriticalSection(&musicCriticalSection);

	switch (newMode)
	{
		case kKickGameScoreMode:
		musicCursor = 2;
		break;

		case kProdGameScoreMode:
		musicCursor = -1;
		break;

		default:
		musicMode = newMode;
		musicCursor = 0;
		break;
	}

	LeaveCriticalSection(&musicCriticalSection);
}

//--------------------------------------------------------------  MusicCallBack

void MusicCallBack (AudioChannel *channel, void *userdata)
{
	AudioEntry entry;

	(void)userdata;

	EnterCriticalSection(&musicCriticalSection);

	switch (musicMode)
	{
		case kPlayGameScoreMode:
		musicCursor++;
		if (musicCursor >= kLastGamePiece)
		{
			musicCursor = 1;
		}
		musicSoundID = gameScore[musicCursor];
		if (musicSoundID < 0)
		{
			musicCursor += musicSoundID;
			musicSoundID = gameScore[musicCursor];
		}
		break;

		case kPlayWholeScoreMode:
		musicCursor++;
		if (musicCursor >= kLastMusicPiece - 1)
		{
			musicCursor = 0;
		}
		musicSoundID = musicScore[musicCursor];
		break;

		default:
		musicSoundID = musicMode;
		break;
	}

	entry.buffer = theMusicData[musicSoundID].dataBytes;
	entry.length = theMusicData[musicSoundID].dataLength;
	entry.endingCallback = MusicCallBack;
	entry.destroyCallback = NULL;
	entry.userdata = NULL;
	AudioChannel_QueueAudio(channel, &entry);

	LeaveCriticalSection(&musicCriticalSection);
}

//--------------------------------------------------------------  LoadMusicSounds

OSErr LoadMusicSounds (void)
{
	SInt16 i;
	HRESULT hr;

	for (i = 0; i < kMaxMusic; i++)
	{
		theMusicData[i].dataLength = 0;
		theMusicData[i].dataBytes = NULL;
	}

	for (i = 0; i < kMaxMusic; i++)
	{
		hr = Gp_LoadBuiltInSound(i + kBaseBufferMusicID, &theMusicData[i]);
		if (FAILED(hr))
		{
			return -1;
		}
	}

	// Test to make sure that all music resources have the same format
	for (i = 1; i < kMaxMusic; i++)
	{
		if (theMusicData[i].format.channels != theMusicData[0].format.channels)
		{
			return -2;
		}
		if (theMusicData[i].format.bitsPerSample != theMusicData[0].format.bitsPerSample)
		{
			return -2;
		}
		if (theMusicData[i].format.samplesPerSec != theMusicData[0].format.samplesPerSec)
		{
			return -2;
		}
	}

	return noErr;
}

//--------------------------------------------------------------  DumpMusicSounds

OSErr DumpMusicSounds (void)
{
	SInt16 i;

	for (i = 0; i < kMaxMusic; i++)
	{
		free((void *)theMusicData[i].dataBytes);
		theMusicData[i].dataBytes = NULL;
		theMusicData[i].dataLength = 0;
	}
	return noErr;
}

//--------------------------------------------------------------  OpenMusicChannel

OSErr OpenMusicChannel (void)
{
	musicChannel = AudioChannel_Open(&theMusicData[0].format);
	if (musicChannel == NULL)
	{
		return -1;
	}
	return noErr;
}

//--------------------------------------------------------------  CloseMusicChannel

OSErr CloseMusicChannel (void)
{
	if (musicChannel != NULL)
	{
		AudioChannel_Close(musicChannel);
		musicChannel = NULL;
	}
	return noErr;
}

//--------------------------------------------------------------  InitMusic

void InitMusic (HWND ownerWindow)
{
	OSErr		theErr;

	if (dontLoadMusic)
	{
		return;
	}

	InitializeCriticalSection(&musicCriticalSection);

	musicChannel = NULL;

	failedMusic = false;
	isMusicOn = false;
	theErr = LoadMusicSounds();
	if (theErr != noErr)
	{
		YellowAlert(ownerWindow, kYellowNoMusic, theErr);
		failedMusic = true;
		return;
	}
	theErr = OpenMusicChannel();
	if (theErr != noErr)
	{
		YellowAlert(ownerWindow, kYellowNoMusic, theErr);
		failedMusic = true;
		return;
	}

	musicScore[0] = 0;
	musicScore[1] = 1;
	musicScore[2] = 2;
	musicScore[3] = 3;
	musicScore[4] = 4;
	musicScore[5] = 4;
	musicScore[6] = 0;
	musicScore[7] = 1;
	musicScore[8] = 2;
	musicScore[9] = 3;
	musicScore[10] = kPlayChorus;
	musicScore[11] = kPlayChorus;
	musicScore[12] = kPlayRefrainSparse1;
	musicScore[13] = kPlayRefrainSparse2;
	musicScore[14] = kPlayChorus;
	musicScore[15] = kPlayChorus;

	gameScore[0] = kPlayRefrainSparse2;
	gameScore[1] = kPlayRefrainSparse1;
	gameScore[2] = -1;
	gameScore[3] = kPlayRefrainSparse2;
	gameScore[4] = kPlayChorus;
	gameScore[5] = kPlayChorus;

	musicCursor = 0;
	musicSoundID = musicScore[musicCursor];
	musicMode = kPlayWholeScoreMode;
}

//--------------------------------------------------------------  KillMusic

void KillMusic (void)
{
	if (dontLoadMusic)
	{
		return;
	}

	StopTheMusic();
	CloseMusicChannel();
	DumpMusicSounds();
	DeleteCriticalSection(&musicCriticalSection);
}

