#ifndef __ECE353_HW2_FONTS_H__
#define __ECE353_HW2_FONTS_H__

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"

#define       FONT_HEIGHT     16
#define       FONT_WIDTH      17

#define       JR_WIDTH   165
#define       JR_HEIGHT   64

#define				MENU_CLOUD_HEIGHT   118
#define				MENU_CLOUD_WIDTH    230

#define				BEGIN_HEIGHT   21
#define				BEGIN_WIDTH    198

#define				MENURABBIT_HEIGHT   54
#define				MENURABBIT_WIDTH    49

#define				MENUGRASS_HEIGHT   55
#define				MENUGRASS_WIDTH    235

#define				MAINMENU_HEIGHT   25
#define				MAINMENU_WIDTH    166

#define				NORMAL_HEIGHT   24
#define				NORMAL_WIDTH    205

#define				QUIET_HEIGHT   31
#define				QUIET_WIDTH    187

#define				RABBIT_HEIGHT   15
#define				RABBIT_WIDTH    20

#define				GRASS_HEIGHT   10
#define				GRASS_WIDTH    15

/* Font data for Sitka Small 12pt */
extern const uint8_t sitkaSmall_12ptBitmaps[];
extern const uint8_t cloudBitmaps[];
extern const uint8_t jumpingRabbitBitmaps[];
extern const uint8_t beginBitmaps[];
extern const uint8_t menuRabbitBitmaps[];
extern const uint8_t menugrassBitmaps[];
extern const uint8_t mainMenuBitmaps [];
extern const uint8_t normalModeBitmaps[];
extern const uint8_t quietModeBitmaps[];
extern const uint8_t rabbitCopyBitmaps[];
extern const uint8_t grassBitmaps[];
/**********************************************************
* Function Name: lcd_print_stringXY
**********************************************************
* Summary: prints a string to the LCD screen at a specified
* XY location in specified foreground and background colors
* X will specify the number of characters across with X=0
* being the left most character position, and X=13 being
* the right most.  Y will specify the number of characters
* down the screen with Y=0 being the top of the screen and
* Y=19 being the bottom row of characters.  NOTE this is
* opposite of how columns and rows are specified for the
* IL9341.
* Returns:
*  Nothing
**********************************************************/
void lcd_print_stringXY(
    char *msg, 
    int8_t X,
		int8_t Y,
    uint16_t fg_color, 
    uint16_t bg_color
);
		
void lcd_print_jrXY(
    int16_t X,
		int16_t Y,
    uint16_t fg_color, 
    uint16_t bg_color
);
		
void lcd_print_cloudXY(
    int16_t X,
		int16_t Y,
    uint16_t fg_color, 
    uint16_t bg_color
);	

void lcd_print_beginXY(
    int16_t X,
		int16_t Y,
    uint16_t fg_color, 
    uint16_t bg_color
);

void lcd_print_menuRabbitXY(
    int16_t X,
		int16_t Y,
    uint16_t fg_color, 
    uint16_t bg_color
);

void lcd_print_menugrassXY(
    int16_t X,
		int16_t Y,
    uint16_t fg_color, 
    uint16_t bg_color
);

void lcd_print_mainMenuXY(
    int16_t X,
		int16_t Y,
    uint16_t fg_color, 
    uint16_t bg_color
);

void lcd_print_normalModeXY(
    int16_t X,
		int16_t Y,
    uint16_t fg_color, 
    uint16_t bg_color
);

void lcd_print_quietModeXY(
    int16_t X,
		int16_t Y,
    uint16_t fg_color, 
    uint16_t bg_color
);

void lcd_print_rabbitXY(
    int16_t X,
		int16_t Y,
    uint16_t fg_color, 
    uint16_t bg_color
);

void lcd_print_grassXY(
    int16_t X,
		int16_t Y,
    uint16_t fg_color, 
    uint16_t bg_color
);
    
#endif
