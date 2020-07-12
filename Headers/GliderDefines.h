#ifndef GLIDER_DEFINES_H_
#define GLIDER_DEFINES_H_

//============================================================================
//----------------------------------------------------------------------------
//                              GliderDefines.h
//----------------------------------------------------------------------------
//============================================================================


//==============================================================  Defines

//#define CREATEDEMODATA
//#define COMPILEDEMO
//#define CAREFULDEBUG
//#define COMPILEQT
#define BUILD_ARCADE_VERSION		0

#define kYellowUnaccounted			1
#define kYellowFailedResOpen		2
#define kYellowFailedResAdd			3
#define kYellowFailedResCreate		4
#define kYellowNoHouses				5
#define kYellowNewerVersion			6
#define kYellowNoBackground			7
#define kYellowIllegalRoomNum		8
#define kYellowNoBoundsRes			9
#define kYellowScrapError			10
#define kYellowNoMemory				11
#define kYellowFailedWrite			12
#define kYellowNoMusic				13
#define kYellowFailedSound			14
#define kYellowAppleEventErr		15
#define kYellowOpenedOldHouse		16
#define kYellowLostAllHouses		17
#define kYellowFailedSaveGame		18
#define kYellowSavedTimeWrong		19
#define kYellowSavedVersWrong		20
#define kYellowSavedRoomsWrong		21
#define kYellowQTMovieNotLoaded		22
#define kYellowNoRooms				23
#define kYellowCantOrderLinks		24

#define kSwitchIfNeeded				0
#define kSwitchTo256Colors			1
#define kSwitchTo16Grays			2

#define kProdGameScoreMode			-4
#define kKickGameScoreMode			-3
#define kPlayGameScoreMode			-2
#define kPlayWholeScoreMode			-1
#define kPlayChorus					4
#define kPlayRefrainSparse1			5
#define kPlayRefrainSparse2			6

#define kHitWallSound				0			//   ••••••
#define kFadeInSound				1			//  ••
#define kFadeOutSound				2			//   ••••••
#define kBeepsSound					3			//        ••
#define kBuzzerSound				4			//   ••••••
#define kDingSound					5			//
#define kEnergizeSound				6			//   ••••••
#define kFollowSound				7			//  ••    ••
#define kMicrowavedSound			8			//  ••    ••
#define kSwitchSound				9			//  ••    ••
#define kBirdSound					10			//   ••••••
#define kCuckooSound				11			//
#define kTikSound					12			//  ••    ••
#define kTokSound					13			//  ••    ••
#define kBlowerOn					14			//  ••    ••
#define kBlowerOff					15			//  ••    ••
#define kCaughtFireSound			16			//   ••••••
#define kScoreTikSound				17			//
#define kThrustSound				18			//  •••   ••
#define kFizzleSound				19			//  ••••  ••
#define kFireBandSound				20			//  •• •• ••
#define kBandReboundSound			21			//  ••  ••••
#define kGreaseSpillSound			22			//  ••   •••
#define kChordSound					23			//
#define kVCRSound					24			//  •••••••
#define kFoilHitSound				25			//  ••    ••
#define kShredSound					26			//  ••    ••
#define kToastLaunchSound			27			//  ••    ••
#define kToastLandSound				28			//  •••••••
#define kMacOnSound					29			//
#define kMacBeepSound				30			//
#define kMacOffSound				31			//
#define kTVOnSound					32			//
#define kTVOffSound					33			//   ••••••
#define kCoffeeSound				34			//  ••
#define kMysticSound				35			//   ••••••
#define kZapSound					36			//        ••
#define kPopSound					37			//   ••••••
#define kEnemyInSound				38			//
#define kEnemyOutSound				39			//   ••••••
#define kPaperCrunchSound			40			//  ••    ••
#define kBounceSound				41			//  ••    ••
#define kDripSound					42			//  ••    ••
#define kDropSound					43			//   ••••••
#define kFishOutSound				44			//
#define kFishInSound				45			//  ••    ••
#define kDontExitSound				46			//  ••    ••
#define kSizzleSound				47			//  ••    ••
#define kPaper1Sound				48			//  ••    ••
#define kPaper2Sound				49			//   ••••••
#define kPaper3Sound				50			//
#define kPaper4Sound				51			//  •••   ••
#define kTypingSound				52			//  ••••  ••
#define kCarriageSound				53			//  •• •• ••
#define kChord2Sound				54			//  ••  ••••
#define kPhoneRingSound				55			//  ••   •••
#define kChime1Sound				56			//
#define kChime2Sound				57			//  •••••••
#define kWebTwangSound				58			//  ••    ••
#define kTransOutSound				59			//  ••    ••
#define kTransInSound				60			//  ••    ••
#define kBonusSound					61			//  •••••••
#define kHissSound					62			//
#define kTriggerSound				63

#define kHitWallPriority			100			//   ••••••
#define kScoreTikPriority			101			//  ••
#define kBandReboundPriority		102			//   ••••••
#define kDontExitPriority			103			//        ••
#define kTikPriority				200			//   ••••••
#define kTokPriority				201			//
#define kMysticPriority				202			//   ••••••
#define kChime1Priority				203			//  ••    ••
#define kChime2Priority				204			//  ••    ••
#define kThrustPriority				300			//  ••    ••
#define kFireBandPriority			301			//   ••••••
#define kChordPriority				302			//
#define kVCRPriority				303			//  ••    ••
#define kToastLaunchPriority		304			//  ••    ••
#define kToastLandPriority			305			//  ••    ••
#define kCoffeePriority				306			//  ••    ••
#define kBouncePriority				307			//   ••••••
#define kDripPriority				308			//
#define kDropPriority				309			//  •••   ••
#define kWebTwangPriority			310			//  ••••  ••
#define kHissPriority				311			//  •• •• ••
#define kFoilHitPriority			400			//  ••  ••••
#define kMacOnPriority				401			//  ••   •••
#define kMacOffPriority				402			//
#define kMacBeepPriority			403			//  •••••••
#define kTVOnPriority				404			//  ••    ••
#define kTVOffPriority				405			//  ••    ••
#define kZapPriority				406			//  ••    ••
#define kPopPriority				407			//  •••••••
#define kEnemyInPriority			408			//
#define kEnemyOutPriority			409			//
#define kPaperCrunchPriority		410			//
#define kFishOutPriority			411			//
#define kFishInPriority				412			//
#define kSizzlePriority				413
#define kPhoneRingPriority			500
#define kSwitchPriority				700
#define kBlowerOnPriority			701
#define kBlowerOffPriority			702
#define kFizzlePriority				703
#define kBeepsPriority				800
#define kBuzzerPriority				801
#define kDingPriority				802
#define kEnergizePriority			803
#define kBirdPriority				804
#define kCuckooPriority				805
#define kGreaseSpillPriority		806
#define kPapersPriority				807
#define kTypingPriority				808
#define kCarriagePriority			809
#define kChord2Priority				810
#define kMicrowavedPriority			811
#define kBonusPriority				812
#define kFadeInPriority				900
#define kFadeOutPriority			901
#define kCaughtFirePriority			902
#define kShredPriority				903
#define kFollowPriority				904
#define kTransInPriority			905
#define kTransOutPriority			906
#define kTriggerPriority			999

#define kSplashMode					0
#define kEditMode					1
#define kPlayMode					2

#define kIdleSplashTicks			7200L		// 2 minutes

#define kRoomAbove					1
#define kRoomBelow					2
#define kRoomToRight				3
#define kRoomToLeft					4

#define kBumpUp						1
#define kBumpDown					2
#define kBumpRight					3
#define kBumpLeft					4

#define kAbove						1
#define kToRight					2
#define kBelow						3
#define kToLeft						4
#define kBottomCorner				5
#define kTopCorner					6

#define kCentralRoom				0
#define kNorthRoom					1
#define kNorthEastRoom				2
#define kEastRoom					3
#define kSouthEastRoom				4
#define kSouthRoom					5
#define kSouthWestRoom				6
#define kWestRoom					7
#define kNorthWestRoom				8

#define kSimpleRoom                 2000
#define kPaneledRoom                2001
#define kBasement                   2002
#define kChildsRoom                 2003
#define kAsianRoom                  2004
#define kUnfinishedRoom             2005
#define kSwingersRoom               2006
#define kBathroom                   2007
#define kLibrary                    2008
#define kGarden                     2009
#define kSkywalk                    2010
#define kDirt                       2011
#define kMeadow                     2012
#define kField                      2013
#define kRoof                       2014
#define kSky                        2015
#define kStratosphere               2016
#define kStars                      2017

#define kMapRoomHeight				20
#define kMapRoomWidth				32

#define kMaxScores					10
#define kMaxRoomObs					24
#define kMaxSparkles				3
#define kNumSparkleModes			5
#define kMaxFlyingPts				3
#define kMaxFlyingPointsLoop		24
#define kMaxCandles					20
#define kMaxTikis					8
#define kMaxCoals					8
#define kMaxPendulums				8
#define kMaxHotSpots				56
#define kMaxSavedMaps				24
#define kMaxRubberBands				2
#define kMaxGrease					16
#define kMaxStars					4
#define kMaxShredded				4
#define kMaxDynamicObs				18
#define kMaxMasterObjects			216		// kMaxRoomObs * 9
#define kMaxViewWidth				1536
#define kMaxViewHeight				1026

#define kSelectTool					0

#define kBlowerMode					1
#define kFurnitureMode				2
#define kBonusMode					3
#define kTransportMode				4
#define kSwitchMode					5
#define kLightMode					6
#define kApplianceMode				7
#define kEnemyMode					8
#define kClutterMode				9

#define kIgnoreIt					0			//   ••••••
#define kLiftIt						1			//  ••    ••
#define kDropIt						2			//  ••••••••
#define kPushItLeft					3			//  ••    ••
#define kPushItRight				4			//  ••    ••
#define kDissolveIt					5			//
#define kRewardIt					6			//   ••••••
#define kMoveItUp					7			//  ••    ••
#define kMoveItDown					8			//  ••
#define kSwitchIt					9			//  ••    ••
#define kShredIt					10			//   ••••••
#define kStrumIt					11			//
#define kTriggerIt					12			//  ••••••••
#define kBurnIt						13			//     ••
#define kSlideIt					14			//     ••
#define kTransportIt				15			//     ••
#define kIgnoreLeftWall				16			//     ••
#define kIgnoreRightWall			17			//
#define kMailItLeft					18			//   ••••••
#define kMailItRight				19			//     ••
#define kDuctItDown					20			//     ••
#define kDuctItUp					21			//     ••
#define kMicrowaveIt				22			//   ••••••
#define kIgnoreGround				23			//
#define kBounceIt					24			//
#define kChimeIt					25			//     ••
#define kWebIt						26			//     ••
#define kSoundIt					27

#define kFloorVent					0x01		// Blowers
#define kCeilingVent				0x02
#define kFloorBlower				0x03
#define kCeilingBlower				0x04
#define kSewerGrate					0x05
#define kLeftFan					0x06
#define kRightFan					0x07
#define kTaper						0x08
#define kCandle						0x09
#define kStubby						0x0A
#define kTiki						0x0B
#define kBBQ						0x0C
#define kInvisBlower				0x0D
#define kGrecoVent					0x0E
#define kSewerBlower				0x0F
#define kLiftArea					0x10

#define kTable						0x11		// Furniture
#define kShelf						0x12
#define kCabinet					0x13
#define kFilingCabinet				0x14
#define kWasteBasket				0x15
#define kMilkCrate					0x16
#define kCounter					0x17
#define kDresser					0x18
#define kDeckTable					0x19
#define kStool						0x1A
#define kTrunk						0x1B
#define kInvisObstacle				0x1C
#define kManhole					0x1D
#define kBooks						0x1E
#define kInvisBounce				0x1F

#define kRedClock					0x21		// Prizes
#define kBlueClock					0x22
#define kYellowClock				0x23
#define kCuckoo						0x24
#define kPaper						0x25
#define kBattery					0x26
#define kBands						0x27
#define kGreaseRt					0x28
#define kGreaseLf					0x29
#define kFoil						0x2A
#define kInvisBonus					0x2B
#define kStar						0x2C
#define kSparkle					0x2D
#define kHelium						0x2E
#define kSlider						0x2F

#define kUpStairs					0x31		// Transport
#define kDownStairs					0x32
#define kMailboxLf					0x33
#define kMailboxRt					0x34
#define kFloorTrans					0x35
#define kCeilingTrans				0x36
#define kDoorInLf					0x37
#define kDoorInRt					0x38
#define kDoorExRt					0x39
#define kDoorExLf					0x3A
#define kWindowInLf					0x3B
#define kWindowInRt					0x3C
#define kWindowExRt					0x3D
#define kWindowExLf					0x3E
#define kInvisTrans					0x3F
#define kDeluxeTrans				0x40

#define kLightSwitch				0x41		// Switches
#define kMachineSwitch				0x42
#define kThermostat					0x43
#define kPowerSwitch				0x44
#define kKnifeSwitch				0x45
#define kInvisSwitch				0x46
#define kTrigger					0x47
#define kLgTrigger					0x48
#define kSoundTrigger				0x49

#define kCeilingLight				0x51		// Lights
#define kLightBulb					0x52
#define kTableLamp					0x53
#define kHipLamp					0x54
#define kDecoLamp					0x55
#define kFlourescent				0x56
#define kTrackLight					0x57
#define kInvisLight					0x58

#define kShredder					0x61		// Appliances
#define kToaster					0x62
#define kMacPlus					0x63
#define kGuitar						0x64
#define kTV							0x65
#define kCoffee						0x66
#define kOutlet						0x67
#define kVCR						0x68
#define kStereo						0x69
#define kMicrowave					0x6A
#define kCinderBlock				0x6B
#define kFlowerBox					0x6C
#define kCDs						0x6D
#define kCustomPict					0x6E

#define kBalloon					0x71		// Enemies
#define kCopterLf					0x72
#define kCopterRt					0x73
#define kDartLf						0x74
#define kDartRt						0x75
#define kBall						0x76
#define kDrip						0x77
#define kFish						0x78
#define kCobweb						0x79

#define kOzma						0x81		// Clutter
#define kMirror						0x82
#define kMousehole					0x83
#define kFireplace					0x84
#define kFlower						0x85
#define kWallWindow					0x86
#define kBear						0x87
#define kCalendar					0x88
#define kVase1						0x89
#define kVase2						0x8A
#define kBulletin					0x8B
#define kCloud						0x8C
#define kFaucet						0x8D
#define kRug						0x8E
#define kChimes						0x8F

#define kNumSrcRects				0x90

#define kTableThick					8
#define kShelfThick					6
#define kToggle						0
#define kForceOn					1
#define kForceOff					2
#define kOneShot					3
#define kNumTrackLights				3
#define kNumOutletPicts				4
#define kNumCandleFlames			5
#define kNumTikiFlames				5
#define kNumBBQCoals				4
#define kNumPendulums				3
#define kNumBreadPicts				6
#define kNumBalloonFrames			8
#define kNumCopterFrames			10
#define kNumDartFrames				4
#define kNumBallFrames				2
#define kNumDripFrames				6
#define kNumFishFrames				8
#define kNumFlowers					6

#define	kNumMarqueePats				7

#define kSwitchLinkOnly				3
#define kTriggerLinkOnly			4
#define kTransportLinkOnly			5

#define kFloorVentTop				305
#define kCeilingVentTop				8
#define kFloorBlowerTop				304
#define kCeilingBlowerTop			5
#define kSewerGrateTop				303
#define kCeilingTransTop			6
#define kFloorTransTop				302
#define kStairsTop					28
#define kCounterBottom				304
#define kDresserBottom				293
#define kCeilingLightTop			4
#define kHipLampTop					23
#define kDecoLampTop				91
#define kFlourescentTop				12
#define kTrackLightTop				5

#define kDoorInTop					0
#define kDoorInLfLeft				0
#define kDoorInRtLeft				368
#define kDoorExTop					0
#define kDoorExLfLeft				0
#define kDoorExRtLeft				496
#define kWindowInTop				64
#define kWindowInLfLeft				0
#define kWindowInRtLeft				492
#define kWindowExTop				64
#define kWindowExLfLeft				0
#define kWindowExRtLeft				496

#define kNumTiles					8
#define kTileWide					64
#define kTileHigh					322
#define kRoomWide					512			// kNumTiles * kTileWide
#define kFloorSupportTall			44
#define kVertLocalOffset			322			// kTileHigh - 39 (was 283, then 295)

#define kCeilingLimit				8
#define kFloorLimit					312
#define kRoofLimit					122
#define kLeftWallLimit				12
#define kNoLeftWallLimit			-24			// 0 - (kGliderWide / 2)
#define kRightWallLimit				500
#define kNoRightWallLimit			536			// kRoomWide + (kGliderWide / 2)
#define kNoCeilingLimit				-10
#define kNoFloorLimit				332

#define kScoreboardHigh				0
#define kScoreboardLow				1
#define kScoreboardTall				20

#define kHouseVersion				0x0200
#define kNewHouseVersion			0x0300
#define kBaseBackgroundID			2000
#define kFirstOutdoorBack			2009
#define kNumBackgrounds				18
#define kUserBackground				3000
#define kUserStructureRange			3300
#define kRoomIsEmpty				-1
#define kObjectIsEmpty				-1
#define kNoObjectSelected			-1
#define kInitialGliderSelected		-2
#define kLeftGliderSelected			-3
#define kRightGliderSelected		-4
#define kTicksPerFrame				2
#define kNumUndergroundFloors		8
#define kRoomVisitScore				100
#define kRedClockPoints				100
#define kBlueClockPoints			300
#define kYellowClockPoints			500
#define kCuckooClockPoints			1000
#define kStarPoints					5000
#define kRedOrangeColor8			23		// actually, 18
#define kMaxNumRoomsH				128
#define kMaxNumRoomsV				64
#define kStartSparkle				4
#define kLengthOfZap				30

#define kGliderWide					48
#define kGliderHigh					20
#define kHalfGliderWide				24
#define kGliderBurningHigh			26
#define kShadowHigh					9
#define kShadowTop					306
#define kFaceRight					true
#define kFaceLeft					false
#define kPlayer1					true
#define kPlayer2					false
#define kNumGliderSrcRects			31
#define kNumShadowSrcRects			2
#define kFirstAboutFaceFrame		18
#define kLastAboutFaceFrame			20
#define kWasBurning					2
#define kLeftFadeOffset				7
#define kLastFadeSequence			16
#define kGliderStartsDown			32

#define kGliderNormal				0			//  ••    ••
#define kGliderFadingIn				1			//  •••  •••
#define kGliderFadingOut			2			//  •• •• ••
#define kGliderGoingUp				3			//  ••    ••
#define kGliderComingUp				4			//  ••    ••
#define kGliderGoingDown			5			//
#define kGliderComingDown			6			//   ••••••
#define kGliderFaceLeft				7			//  ••    ••
#define kGliderFaceRight			8			//  ••    ••
#define kGliderBurning				9			//  ••    ••
#define kGliderTransporting			10			//   ••••••
#define kGliderDuctingDown			11			//
#define kGliderDuctingUp			12			//  •••••••
#define kGliderDuctingIn			13			//  ••    ••
#define kGliderMailInLeft			14			//  ••    ••
#define kGliderMailOutLeft			15			//  ••    ••
#define kGliderMailInRight			16			//  •••••••
#define kGliderMailOutRight			17			//
#define kGliderGoingFoil			18			//  ••••••••
#define kGliderLosingFoil			19			//  ••
#define kGliderShredding			20			//  ••••
#define kGliderInLimbo				21			//  ••
#define kGliderIdle					22			//  ••••••••
#define kGliderTransportingIn		23

#define kPlayerIsDeadForever		-69
#define kPlayerMailedOut			-12
#define kPlayerDuckedOut			-11
#define kPlayerTransportedOut		-10
#define kPlayerEscapingDownStairs	-9
#define kPlayerEscapingUpStairs		-8
#define kPlayerEscapedDownStairs	-7
#define kPlayerEscapedUpStairs		-6
#define kPlayerEscapedDown			-5
#define kPlayerEscapedUp			-4
#define kPlayerEscapedLeft			-3
#define kPlayerEscapedRight			-2
#define kNoOneEscaped				-1

#define kLinkedToOther				0
#define kLinkedToLeftMailbox		1
#define kLinkedToRightMailbox		2
#define kLinkedToCeilingDuct		3
#define kLinkedToFloorDuct			4

#define kResumeGameMode				0
#define kNewGameMode				1

#define kNormalTitleMode			0
#define kEscapedTitleMode			1
#define kSavingTitleMode			2

#define kDemoLength					6702

#define kPreferredDepth				8

#define kHomeKeyASCII				0x01
#define kEnterKeyASCII				0x03
#define kEndKeyASCII				0x04
#define kHelpKeyASCII				0x05
#define	kDeleteKeyASCII				0x08
#define kTabKeyASCII				0x09
#define kPageUpKeyASCII				0x0B
#define kPageDownKeyASCII			0x0C
#define	kReturnKeyASCII				0x0D
#define	kFunctionKeyASCII			0x10
#define	kClearKeyASCII				0x1A
#define	kEscapeKeyASCII				0x1B
#define	kLeftArrowKeyASCII			0x1C
#define kRightArrowKeyASCII			0x1D
#define	kUpArrowKeyASCII			0x1E
#define kDownArrowKeyASCII			0x1F
#define kSpaceBarASCII				0x20
#define kExclamationASCII			0x21
#define kPlusKeyASCII				0x2B
#define kMinusKeyASCII				0x2D
#define k0KeyASCII					0x30
#define k1KeyASCII					0x31
#define k2KeyASCII					0x32
#define k3KeyASCII					0x33
#define k4KeyASCII					0x34
#define k5KeyASCII					0x35
#define k6KeyASCII					0x36
#define k7KeyASCII					0x37
#define k8KeyASCII					0x38
#define k9KeyASCII					0x39

#define kCapAKeyASCII				0x41
#define kCapBKeyASCII				0x42
#define kCapCKeyASCII				0x43
#define kCapDKeyASCII				0x44
#define kCapEKeyASCII				0x45
#define kCapFKeyASCII				0x46
#define kCapGKeyASCII				0x47
#define kCapHKeyASCII				0x48
#define kCapIKeyASCII				0x49
#define kCapJKeyASCII				0x4A
#define kCapKKeyASCII				0x4B
#define kCapLKeyASCII				0x4C
#define kCapMKeyASCII				0x4D
#define kCapNKeyASCII				0x4E
#define kCapOKeyASCII				0x4F
#define kCapPKeyASCII				0x50
#define kCapQKeyASCII				0x51
#define kCapRKeyASCII				0x52
#define kCapSKeyASCII				0x53
#define kCapTKeyASCII				0x54
#define kCapUKeyASCII				0x55
#define kCapVKeyASCII				0x56
#define kCapWKeyASCII				0x57
#define kCapXKeyASCII				0x58
#define kCapYKeyASCII				0x59
#define kCapZKeyASCII				0x5A

#define kAKeyASCII					0x61
#define kBKeyASCII					0x62
#define kCKeyASCII					0x63
#define kDKeyASCII					0x64
#define kEKeyASCII					0x65
#define kFKeyASCII					0x66
#define kGKeyASCII					0x67
#define kHKeyASCII					0x68
#define kIKeyASCII					0x69
#define kJKeyASCII					0x6A
#define kKKeyASCII					0x6B
#define kLKeyASCII					0x6C
#define kMKeyASCII					0x6D
#define kNKeyASCII					0x6E
#define kOKeyASCII					0x6F
#define kPKeyASCII					0x70
#define kQKeyASCII					0x71
#define kRKeyASCII					0x72
#define kSKeyASCII					0x73
#define kTKeyASCII					0x74
#define kUKeyASCII					0x75
#define kVKeyASCII					0x76
#define kWKeyASCII					0x77
#define kXKeyASCII					0x78
#define kYKeyASCII					0x79
#define kZKeyASCII					0x7A
#define kForwardDeleteASCII			0x7F

#define	kPlusKeypadMap				66		// key map offset for + on keypad
#define	kMinusKeypadMap				73		// key map offset for - on keypad
#define	kTimesKeypadMap				68		// key map offset for * on keypad
#define	k0KeypadMap					85		// key map offset for 0 on keypad
#define	k1KeypadMap					84		// key map offset for 1 on keypad
#define	k2KeypadMap					83		// key map offset for 2 on keypad
#define	k3KeypadMap					82		// key map offset for 3 on keypad
#define k4KeypadMap					81		// key map offset for 4 on keypad
#define k5KeypadMap					80		// key map offset for 5 on keypad
#define k6KeypadMap					95		// key map offset for 6 on keypad
#define k7KeypadMap					94		// key map offset for 7 on keypad
#define k8KeypadMap					92		// key map offset for 8 on keypad
#define k9KeypadMap					91		// key map offset for 9 on keypad

#define kUpArrowKeyMap				121		// key map offset for up arrow
#define kDownArrowKeyMap			122		// key map offset for down arrow
#define kRightArrowKeyMap			123		// key map offset for right arrow
#define kLeftArrowKeyMap			124		// key map offset for left arrow

#define kAKeyMap					7
#define	kBKeyMap					12
#define	kCKeyMap					15
#define	kDKeyMap					5
#define	kEKeyMap					9
#define	kFKeyMap					4
#define	kGKeyMap					2
#define	kHKeyMap					3
#define	kMKeyMap					41
#define	kNKeyMap					42
#define	kOKeyMap					24
#define	kPKeyMap					36
#define	kQKeyMap					11
#define	kRKeyMap					8
#define kSKeyMap					6
#define kTKeyMap					22
#define kVKeyMap					14
#define kWKeyMap					10
#define kXKeyMap					0
#define kZKeyMap					1
#define kPeriodKeyMap				40
#define	kCommandKeyMap				48
#define	kEscKeyMap					50
#define	kDeleteKeyMap				52
#define kSpaceBarMap				54
#define	kTabKeyMap					55
#define	kControlKeyMap				60
#define	kOptionKeyMap				61
#define	kCapsLockKeyMap				62
#define	kShiftKeyMap				63

#define kTabRawKey					0x30	// key map offset for Tab key
#define kClearRawKey				0x47	// key map offset for Clear key
#define kF5RawKey					0x60	// key map offset for F5
#define kF6RawKey					0x61	// key map offset for F6
#define kF7RawKey					0x62	// key map offset for F7
#define kF3RawKey					0x63	// key map offset for F3
#define kF8RawKey					0x64	// key map offset for F8
#define kF9RawKey					0x65	// key map offset for F9
#define kF11RawKey					0x67	// key map offset for F11
#define kF13RawKey					0x69	// key map offset for F13
#define kF14RawKey					0x6B	// key map offset for F14
#define kF10RawKey					0x6D	// key map offset for F10
#define kF12RawKey					0x6F	// key map offset for F12
#define kF15RawKey					0x71	// key map offset for F15
#define kF4RawKey					0x76	// key map offset for F4
#define kF2RawKey					0x78	// key map offset for F2
#define kF1RawKey					0x7A	// key map offset for F1

#define	kErrUnnaccounted			1
#define	kErrNoMemory				2
#define kErrDialogDidntLoad			3
#define kErrFailedResourceLoad		4
#define kErrFailedGraphicLoad		5
#define kErrFailedOurDirect			6
#define kErrFailedValidation		7
#define kErrNeedSystem7				8
#define kErrFailedGetDevice			9
#define kErrFailedMemoryOperation	10
#define kErrFailedCatSearch			11
#define kErrNeedColorQD				12
#define kErrNeed16Or256Colors		13

#define iAbout					1
#define iNewGame				1
#define iTwoPlayer				2
#define iOpenSavedGame			3
#define iLoadHouse				5
#define iQuit					7
#define iEditor					1
#define iHighScores				3
#define iPrefs					4
#define iHelp					5
#define iNewHouse				1
#define iSave					2
#define iHouse					4
#define iRoom					5
#define iObject					6
#define iCut					8
#define iCopy					9
#define iPaste					10
#define iClear					11
#define iDuplicate				12
#define iBringForward			14
#define iSendBack				15
#define iGoToRoom				17
#define iMapWindow				19
#define iObjectWindow			20
#define iCoordinateWindow		21

#endif
