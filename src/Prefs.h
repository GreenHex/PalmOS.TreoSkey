/*
 * Prefs.h
 */

#include <PalmTypes.h>

// Structures and defines
typedef struct { 
	Boolean									bEnabled; // 1
	Boolean									bEnableVolKeys; // 1
	Boolean									bEnableSideKey; // 1
} AppPreferences_t;

typedef struct {
	Boolean									bActive; // 1
	Boolean									bOptionKeyPressed; // 1
} AppUSPrefs_t;

#define PREFS_ENABLED						false
#define PREFS_ENABLE_VOLKEYS				true
#define PREFS_ENABLE_SIDEKEY				true
 
// Prototypes
extern void 								writePrefs(AppPreferences_t* pPrefs);
extern void 								readPrefs(AppPreferences_t* pPrefs);
extern void 								writeUSPrefs(AppUSPrefs_t* pUSPrefs);
extern void 								readUSPrefs(AppUSPrefs_t* pUSPrefs);

/*
 * Prefs.h
 */