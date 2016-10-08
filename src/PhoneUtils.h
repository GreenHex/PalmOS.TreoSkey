/*
 * PhoneUtils.h
 */
 
#include <PalmTypes.h>
#include <Hs.h>
#include <HsPhone.h>
#include <HsCreators.h>
#include <HsNav.h>
#include <HsExt.h>
#include <palmOneResources.h>

// beeps, etc...
#define SND1_FREQUENCY					900 // 1760
#define SND1_DURATION					80
#define SND2_FREQUENCY					900 // 1760
#define SND2_DURATION					80
#define PAUSE_FREQUENCY					1 // 0 doesn't play any sound // we don't use this anyway
#define PAUSE_DURATION					80

#define MIN_VERSION  					sysMakeROMVersion(5, 0, 0, sysROMStageRelease, 0)
#define LAUNCH_FLAGS 					(sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)

extern void 							DelayTask(UInt32 DelaySeconds);
extern void 							Alert(Char* MsgType, Char* Msg, Err err);
extern void 							beep(UInt8 numBeeps);
extern Err 								RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags);

/*
 * PhoneUtils.h
 */