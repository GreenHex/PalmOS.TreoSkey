/*
 * AppMain.c
 */
 
#include "Global.h"
#include "AppResources.h"
#include "PhoneUtils.h"
#include "Prefs.h"
#include "Panl.h"
#include "NotifyReg.h"
#include "WinDisp.h"

// Prototypes
static void 				InitializeMainForm(FormType* pForm, AppPreferences_t* pPrefs);
static Boolean 				MainFormHandleEvent(EventType* pEvent);
static Boolean 				AppHandleEvent(EventType* pEvent);
static void 				AppEventLoop(void);
static Err 					AppStart(void);
static void 				AppStop(void);

UInt32 						PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags);

/*
 * InitializeMainForm
 */
static void InitializeMainForm(FormType* pForm, AppPreferences_t* pPrefs)
{	
	ControlType*    		pCtl;
	MemHandle				verStrH = NULL;
	Char*					verStrP = NULL;
	Char*					verStr = NULL;
			
	pCtl = FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, MAIN_ENABLE_CHECKBOX));
	CtlSetValue(pCtl, pPrefs->bEnabled);
	
	pCtl = FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, MAIN_ENABLE_VOLKEYS_CHECKBOX));
	CtlSetValue(pCtl, pPrefs->bEnableVolKeys);
	
	pCtl = FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, MAIN_ENABLE_SIDEKEY_CHECKBOX));
	CtlSetValue(pCtl, pPrefs->bEnableSideKey);
	
	verStrH = DmGet1Resource('tver', 1000);
	if (verStrH)
	{
		verStrP = MemHandleLock(verStrH);
		
		if (verStrP)
		{
			pCtl = FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, MAIN_VERSION_LABEL));
			
			verStr = (Char*) CtlGetLabel(pCtl);
		
			StrCopy(verStr, "v");
			StrCat(verStr, verStrP);
						
			CtlSetLabel(pCtl, verStr);
			
			MemHandleUnlock(verStrH);
		}
	   	
	   	DmReleaseResource(verStrH);
	}

	FrmDrawForm(pForm);
	
} // InitializeMainForm

/*
 * MainFormHandleEvent
 */
static Boolean MainFormHandleEvent(EventType* pEvent)
{
	Boolean 				handled = false;
	FormType* 				pForm = NULL;
	ControlType*    		pCtl = NULL;
	ListType*				pList = NULL;
	FieldType*				pField = NULL;
	
	AppPreferences_t		prefs;
	
	EventType 				newEvent;

	MemSet(&prefs, sizeof(AppPreferences_t), 0);
	readPrefs(&prefs);
	
	pForm = FrmGetActiveForm(); // THE CAUSE OF SO MANY CRASHES!!!
	
	switch (pEvent->eType)
	{
		case frmOpenEvent:
			
			InitializeMainForm(pForm, &prefs);
			FrmDrawForm(pForm);
			
			handled = true;

			break;
			
		case frmCloseEvent:

			PanelFormClose(pForm);
			
			RegisterForNotifications(prefs.bEnabled);

			handled = true;
									
			break;
			
		case ctlSelectEvent:
			switch (pEvent->data.ctlSelect.controlID)
			{
				case MAIN_ENABLE_CHECKBOX:
				
					prefs.bEnabled = (CtlGetValue(pEvent->data.ctlSelect.pControl) == 1);
					
					writePrefs(&prefs);
					
					RegisterForNotifications(prefs.bEnabled);
					
					handled = true;
					
					break;
					
				case MAIN_ENABLE_VOLKEYS_CHECKBOX:
				
					prefs.bEnableVolKeys = (CtlGetValue(pEvent->data.ctlSelect.pControl) == 1);
				
					writePrefs(&prefs);
					
					handled = true;
					
					break;
					
				case MAIN_ENABLE_SIDEKEY_CHECKBOX:
				
					prefs.bEnableSideKey = (CtlGetValue(pEvent->data.ctlSelect.pControl) == 1);
				
					writePrefs(&prefs);
					
					handled = true;
					
					break;
					
				case MAIN_DONE_BUTTON:
					
					pCtl = FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, MAIN_ENABLE_CHECKBOX));
					prefs.bEnabled = (CtlGetValue(pCtl) == 1);
					
					pCtl = FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, MAIN_ENABLE_VOLKEYS_CHECKBOX));
					prefs.bEnableVolKeys = (CtlGetValue(pCtl) == 1);
					
					pCtl = FrmGetObjectPtr(pForm, FrmGetObjectIndex(pForm, MAIN_ENABLE_SIDEKEY_CHECKBOX));
					prefs.bEnableSideKey = (CtlGetValue(pCtl) == 1);
					
					writePrefs(&prefs);
					
					RegisterForNotifications(prefs.bEnabled);

					if (pEvent->data.ctlSelect.controlID == MAIN_DONE_BUTTON)
					{
						EventType 		newEvent;
						
						newEvent.eType = appStopEvent;
						EvtAddEventToQueue(&newEvent);
					}
					
					handled = true;
					
					break;
					
				default:
					break;
			}
			break;
			
		default:
			break;
	}
		
	return handled;
}

/*
 * AppHandleEvent
 */
static Boolean AppHandleEvent(EventType* pEvent)
{
	UInt16 		formID;
	FormType* 	pForm;
	Boolean		handled = false;

	if (pEvent->eType == frmLoadEvent)
	{
		formID = pEvent->data.frmLoad.formID;
		
		pForm = FrmInitForm(formID);
		FrmSetActiveForm(pForm);

		if (formID == MAIN_FORM)
			FrmSetEventHandler(pForm, &MainFormHandleEvent);
			
		handled = true;
	}
	
	return handled;
}

/*
 * AppEventLoop
 */
static void AppEventLoop(void)
{
	// Err			error;
	EventType	event;

	do {
		EvtGetEvent(&event, evtWaitForever);

		if (SysHandleEvent(&event))
			continue;
					
		if (AppHandleEvent(&event))
			continue;

		FrmDispatchEvent(&event);

	} while (event.eType != appStopEvent);
}

/*
 * AppStart
 */
static Err AppStart(void)
{
	FrmGotoForm(MAIN_FORM);
	return errNone;

} // AppStart

/*
 * AppStop
 */
static void AppStop(void)
{	
	FrmCloseAllForms();	
    
} // AppStop						

/*
 * PilotMain
 */
UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	Err 						error = errNone;
	AppPreferences_t			prefs;
	
	MemSet(&prefs, sizeof(AppPreferences_t), 0);
	readPrefs(&prefs);
	
	switch (cmd)
	{
		case sysAppLaunchCmdNormalLaunch:
	    case sysAppLaunchCmdPanelCalledFromApp:
	    case sysAppLaunchCmdReturnFromPanel:

			// Check device
			if ((error = RomVersionCompatible(MIN_VERSION, launchFlags)))
			{
				Alert("Error ", "Incompatible Device", error);
				return error;
			}
			
			if ((error = AppStart()) == 0)
			{
				AppEventLoop();
				AppStop();				
			}
			
			break;
			
		case sysAppLaunchCmdSystemReset:

			if (!(error = RomVersionCompatible(MIN_VERSION, launchFlags)))
			{
				RegisterForNotifications(prefs.bEnabled);
			}		
			break;
			
		case sysAppLaunchCmdNotify:
			/*
			if (((SysNotifyParamType*)cmdPBP)->notifyType == sysNotifyAppLaunchingEvent)
			{
				
				
			}
			else if (((SysNotifyParamType*)cmdPBP)->notifyType == sysNotifyAppQuittingEvent)
			{
				
				
			}
			else */ if (((SysNotifyParamType*)cmdPBP)->notifyType == sysNotifyVirtualCharHandlingEvent)
			{
				SysNotifyParamType* 	notifyParam = (SysNotifyParamType *)cmdPBP;
				UInt16					keyMask = ((SysNotifyVirtualCharHandlingType *)notifyParam->notifyDetailsP)->keyDown.modifiers;
				UInt16					keyCode = ((SysNotifyVirtualCharHandlingType *)notifyParam->notifyDetailsP)->keyDown.keyCode;
				
				if (keyMask & commandKeyMask)
				{
					Boolean			handled = false;
					AppUSPrefs_t	usPrefs;
					WChar			key;
					
					MemSet(&usPrefs, sizeof(AppUSPrefs_t), 0);
					readUSPrefs(&usPrefs);						
					
					if ( keyMask & shiftKeyMask )
					{
						break;
					}
					
					if ((keyCode == keyLeftAlt)
								/* || (keyCode == keyLeftShift)
								|| (keyCode == keyRightShift) */)
					{
						usPrefs.bOptionKeyPressed = !(usPrefs.bOptionKeyPressed);	
					}
					else if (keyCode != keySide)
					{	
						usPrefs.bOptionKeyPressed = false;
					}
					
					writeUSPrefs(&usPrefs);
					
					if ((prefs.bEnableVolKeys) && (keyCode == keyVolumeUp) && (usPrefs.bActive))
					{
						key = vchrRockerUp;
						usPrefs.bOptionKeyPressed = false;
						handled = true;
					}
					else if ((prefs.bEnableVolKeys) && (keyCode == keyVolumeDown) && (usPrefs.bActive))
					{
						key = vchrRockerDown;
						usPrefs.bOptionKeyPressed = false;
						handled = true;
					}
					else if ((keyCode == keySide))
					{
						if ((usPrefs.bOptionKeyPressed) /* || (keyMask & shiftKeyMask) */)
						{
							usPrefs.bActive = ~usPrefs.bActive;
							usPrefs.bOptionKeyPressed = false;
							writeUSPrefs(&usPrefs);
							
							if (usPrefs.bActive)
							{
								DisplayState(APP_NAME, STR_ON);
							}
							else
							{
								DisplayState(APP_NAME, STR_OFF);
							}	

							handled = true;							
						}
						else if ((prefs.bEnableSideKey) && (usPrefs.bActive))
						{
							key = vchrRockerCenter;
							usPrefs.bOptionKeyPressed = false;
							handled = true;
						}
					}

					writeUSPrefs(&usPrefs);
/*					
					if (keyMask & autoRepeatKeyMask )
					{
						DisplayWindow("TreoTest", "AutoRepeat");
					}
					if (keyMask & doubleTapKeyMask )
					{
						DisplayWindow("TreoTest", "DoubleTap");
					}
										
*/
/*
					if (keyCode == vchrRockerUp)
					{
						Alert("up", " ", keyCode);
					}
*/					
					if ((handled) && (usPrefs.bActive))
					{
						EvtResetAutoOffTimer();
						
						((SysNotifyParamType*)cmdPBP)->handled = handled;
						
						EvtEnqueueKey(key, (UInt16) key, commandKeyMask);
					}
				}					
			}
		
			break;
	
		default:
			break;
	}

	return error;
	
} // PilotMain

/*
 * AppMain.c */