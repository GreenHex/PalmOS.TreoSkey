/*
 * Prefs.c
 */

#include "Global.h"
#include "Prefs.h"

/*
 * writePrefs
 */
void writePrefs(AppPreferences_t* pPrefs)
{
	PrefSetAppPreferences(appFileCreator, appPrefID, appPrefVersionNum, pPrefs, sizeof(AppPreferences_t), true);

} // writePrefs

/*
 * readPrefs
 */
void readPrefs(AppPreferences_t* pPrefs)
{	
	UInt16 prefSize = sizeof(AppPreferences_t);
	
	if (PrefGetAppPreferences(appFileCreator, appPrefID, pPrefs, &prefSize, true) == noPreferenceFound)
	{	
		pPrefs->bEnabled = PREFS_ENABLED;
		pPrefs->bEnableVolKeys = PREFS_ENABLE_VOLKEYS;
		pPrefs->bEnableSideKey = PREFS_ENABLE_SIDEKEY;

		writePrefs(pPrefs);
	}
} // readPrefs

/*
 * writeUSPrefs
 */
void writeUSPrefs(AppUSPrefs_t* pUSPrefs)
{
	PrefSetAppPreferences(appFileCreator, appPrefID, appPrefVersionNum, pUSPrefs, sizeof(AppUSPrefs_t), false);

} // writeUSPrefs

/*
 * readUSPrefs
 */
void readUSPrefs(AppUSPrefs_t* pUSPrefs)
{	
	UInt16 usPrefSize = sizeof(AppUSPrefs_t);
	
	if (PrefGetAppPreferences(appFileCreator, appPrefID, pUSPrefs, &usPrefSize, false) == noPreferenceFound)
	{	
		pUSPrefs->bActive = false;
		pUSPrefs->bOptionKeyPressed= false;

		writeUSPrefs(pUSPrefs);
	}
} // readPrefs

/*
 * Prefs.c
 */