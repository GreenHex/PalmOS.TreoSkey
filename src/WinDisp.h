/*
 * WinDisp.h
 */
 
#include <PalmOS.h>
#include <Window.h>
#include <Font.h>

#include "Global.h"

// Window defines
#define DISP_WIDTH				160
#define DISP_HEIGHT				DISP_WIDTH

#define RECT_WIDTH				140
#define RECT_HEIGHT				34

#define RECT_LEFT_X				(DISP_WIDTH - RECT_WIDTH) / 2
#define RECT_TOP_Y				((DISP_HEIGHT - RECT_HEIGHT) / 2) - 4 // additional 2 pixels

#define TXT_WIDTH				RECT_WIDTH - (2 * TXT_OFF_X)

#define LINE_GAP				2

#define RECT_WIDTH_BIG			110
#define RECT_HEIGHT_BIG			30

#define TXT_OFF_X				5
#define TXT_OFF_Y				2

#define STR_ON					"Active"
#define STR_OFF					"Disabled"

// Prototypes;
extern void 					DisplayState(Char* str1, Char* str2);
/*
 * WinDisp.h
 */