/*
 * NotifyReg.c
 */
 
#include "Global.h"
#include "NotifyReg.h"

/*
 * RegisterForNotifications
 */
void RegisterForNotifications(Boolean bRegister)
{
	Err					error = errNone;
	
	Int32				NotifyPriority = 96; // sysNotifyNormalPriority = 0

	// UInt16 				PhoneLibRefNum;
	
	UInt16 				cardNo; 
	LocalID  			dbID;
/*	
	if (HsGetPhoneLibrary(&PhoneLibRefNum))
		return;
		
	if (PhnLibOpen(PhoneLibRefNum))
		return;
*/
	SysCurAppDatabase(&cardNo, &dbID);
	
	if (bRegister)
	{
		// error = PhnLibRegister(PhoneLibRefNum, appFileCreator, phnServiceVoice);
		// SysNotifyRegister(cardNo, dbID, sysNotifyAppLaunchingEvent, NULL, NotifyPriority, NULL);
		// SysNotifyRegister(cardNo, dbID, sysNotifyAppQuittingEvent, NULL, NotifyPriority, NULL);
		SysNotifyRegister(cardNo, dbID, sysNotifyVirtualCharHandlingEvent, NULL, NotifyPriority, NULL);
		// SysNotifyRegister(cardNo, dbID, csDeferredNotifyEvent, NULL, NotifyPriority, NULL);
	}
	else
	{
		// error = PhnLibRegister(PhoneLibRefNum, appFileCreator, 0);
		// SysNotifyUnregister(cardNo, dbID, sysNotifyAppLaunchingEvent, NotifyPriority);
		// SysNotifyUnregister(cardNo, dbID, sysNotifyAppQuittingEvent, NotifyPriority);
		SysNotifyUnregister(cardNo, dbID, sysNotifyVirtualCharHandlingEvent, NotifyPriority);
		// SysNotifyUnregister(cardNo, dbID, csDeferredNotifyEvent, NotifyPriority);
	}
			
	// PhnLibClose(PhoneLibRefNum);
	
} // RegisterForNotifications

/*
 * NotifyReg.c
 */