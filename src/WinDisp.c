/*
 * WinDisp.c
 */

#include "WinDisp.h"
#include "PhoneUtils.h"

// Prototypes
static Err 				SaveRect(RectangleType* rectP, WinHandle* winHP, RectangleType* rectObscuredP);
static void 			RestoreRect(WinHandle* winHP, RectangleType* rectObscuredP);

/*
 * SaveRect
 */
static Err SaveRect(RectangleType* rectP, WinHandle* winHP, RectangleType* rectObscuredP)
{
	Err		error = errNone;
	
	if ((rectP) && (winHP)) 
	{
		*winHP = NULL; // preset
		
		WinGetFramesRectangle(dialogFrame, rectP, rectObscuredP);			
		*winHP = WinSaveBits(rectObscuredP, &error);
		
		if (error)
			*winHP = NULL;		
	}
	
	return (error);

} // SaveRect

/*
 * RestoreRect
 */
static void RestoreRect(WinHandle* winHP, RectangleType* rectObscuredP)
{
	if ((*winHP) && (rectObscuredP))
	{	
		WinRestoreBits(*winHP, rectObscuredP->topLeft.x, rectObscuredP->topLeft.y);
		*winHP = NULL;
	}

	return;

} // RestoreRect

/*
 * DisplayState
 */
void DisplayState(Char* str1, Char* str2)
{
	FormActiveStateType		activeState;
	//WinHandle				oldWin;
	//WinHandle				newWin;
	
	RectangleType			rect = { { (DISP_WIDTH - RECT_WIDTH_BIG) / 2, ((DISP_HEIGHT - RECT_HEIGHT_BIG) / 2)} , { RECT_WIDTH_BIG , RECT_HEIGHT_BIG} };	
							
	UInt16 					rectWidth = rect.extent.x;
	UInt16 					rectHeight = rect.extent.y;
	UInt16					rectLeft_X = (DISP_WIDTH - rectWidth) / 2;
	UInt16					rectTop_Y = ((DISP_HEIGHT - rectHeight) / 2); //  - 4;
	UInt16					lineWidth = rectWidth - (2 * LINE_GAP);
	
    RGBColorType 			rgb;
    IndexedColorType		colorWhite;
    IndexedColorType		colorBlack;
    IndexedColorType		colorGrey;
	IndexedColorType 		colorRed;
	IndexedColorType		colorGreen;
	
	UInt16					str1Width = 0;
	UInt16					str2Width = 0;
	UInt16					strHeight = 0;
	
	RectangleType			rectObscured;
	WinHandle				winH = NULL;
	
	WinDisplayToWindowPt(&rectLeft_X, &rectTop_Y);

	rect.topLeft.x = rectLeft_X;
	rect.topLeft.y = rectTop_Y;
	
    FrmSaveActiveState(&activeState); 
	
	//oldWin = WinGetActiveWindow();
	//newWin = WinGetDisplayWindow();
	
	//WinSetActiveWindow(newWin);
	//WinSetDrawWindow(newWin);
	
	WinPushDrawState();
	
	SaveRect(&rect, &winH, &rectObscured);
   	
	rgb.r=255; rgb.g=255; rgb.b=255; // white
	colorWhite = WinRGBToIndex(&rgb);	
	
	rgb.r=0; rgb.g=0; rgb.b=0; // black
	colorBlack = WinRGBToIndex(&rgb);
	
    rgb.r=150; rgb.g=150; rgb.b=150; // grey?
	colorGrey = WinRGBToIndex(&rgb);
	
	rgb.r=255; rgb.g=0; rgb.b=51; // red?
	colorRed = WinRGBToIndex(&rgb);
		
	rgb.r=0; rgb.g=153; rgb.b=51; // green?
	colorGreen = WinRGBToIndex(&rgb);	
		
	WinSetForeColor(colorGrey);
	WinSetBackColor(colorWhite);
		
	WinEraseRectangleFrame(dialogFrame, &rect); 
    WinEraseRectangle(&rect, 2); // 2nd arg is corner "diameter"!

	WinEraseRectangleFrame(dialogFrame, &rect);
	WinPaintRectangleFrame(dialogFrame, &rect);
	
	FntSetFont(stdFont);
	    
	str1Width = FntLineWidth(str1, StrLen(str1));
	
	WinSetTextColor(colorBlack);
	
    WinPaintChars(str1, StrLen(str1),
				rectLeft_X + (rectWidth - str1Width)/2,
				rectTop_Y + TXT_OFF_Y);
			
	if (str2)
	{
		Boolean					onFlag = (StrCompare(str2, STR_ON) != 0);
		
		strHeight = FntLineHeight();
		str2Width = FntLineWidth(str2, StrLen(str2));
		
		WinPaintLine(rectLeft_X + LINE_GAP, rectTop_Y + (rectHeight / 2), rectLeft_X + lineWidth, rectTop_Y + (rectHeight / 2));
		
		if (onFlag)
		{
			WinSetTextColor(colorRed);
		}
		else
		{
			WinSetTextColor(colorGreen);
		}
		
	    WinPaintChars(str2, StrLen(str2),
				rectLeft_X + (rectWidth - str2Width)/2,
				rectTop_Y + TXT_OFF_Y + strHeight + 4);	
	}
        
    DelayTask(2); // 2 seconds

	RestoreRect(&winH, &rectObscured);

 	WinPopDrawState();

//	WinSetActiveWindow(oldWin);
//	WinSetDrawWindow(oldWin);
	
	FrmRestoreActiveState(&activeState);
	
	return;
   
} // DisplayState

/*
 * WinDisp.c
 */