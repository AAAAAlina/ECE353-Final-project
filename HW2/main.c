// Copyright (c) 2015-16, Joe Krachey
// All rights reserved.
//
// Redistribution and use in source or binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in source form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <stdlib.h>

#include "main.h"


#define goLeft 0xbff
#define goRight 0x3ff
#define PWM0GENA 0x0000008C
#define PWM0GENB 0x0000080C

char individual[] = "Team 32";
char * msg0; //for printing text

// parameters used to read the joystick
volatile uint16_t x_PS2; 
volatile uint16_t y_PS2;

volatile uint8_t up_count = 80;	//going up counter

// current position of the rabbit
uint16_t x_pos = 80;	//place at the middle first
uint16_t y_pos = 5;

// previsou position of the rabbit
// in order to make it not blink when it is not moving
uint16_t x_pre;
uint16_t y_pre;

// 2D array to save the grass position
// 6 grass in total, and their x, y position
uint16_t grass[6][2];

// parameters to record the score
uint8_t totalPoint = 0;
uint8_t highest = 0;
// address that saving the history highest score
const uint16_t address = 0xFFFF;

uint16_t width = 10; // save the last grass position

bool make_sound = true;  // used for configue normal and quiet mode

// used for alert the interrupt
volatile bool ALERT_ADC_UPDATE;
volatile bool ALERT_DEBOUNCE_UPDATE;

// parameters used for debounce the SW1 button
volatile bool SW1_pressed = false;
volatile uint8_t debounce_count;


//*****************************************************************************
// 
//*****************************************************************************
void TIMER0A_Handler()
{
	ALERT_DEBOUNCE_UPDATE = true;
  TIMER0->ICR |= TIMER_ICR_TATOCINT;
}
//*****************************************************************************
// 
//*****************************************************************************
void TIMER0B_Handler()
{
	ALERT_ADC_UPDATE = true;
	TIMER0->ICR |= TIMER_ICR_TBTOCINT;
	ADC0->PSSI = ADC_PSSI_SS2;		

}
//*****************************************************************************
// 
//*****************************************************************************
void ADC0SS2_Handler(void)
{	
	y_PS2 = ADC0->SSFIFO2 & 0xFFF;
	x_PS2 = ADC0->SSFIFO2 & 0xFFF;
  ADC0->ISC  |= ADC_ISC_IN2;          // Ack the conversion
}
//*****************************************************************************
// 
//*****************************************************************************
void DisableInterrupts(void)
{
  __asm {
         CPSID  I
  }
}

//*****************************************************************************
// 
//*****************************************************************************
void EnableInterrupts(void)
{
  __asm {
    CPSIE  I
  }
}


//*****************************************************************************
//*****************************************************************************
void initializeBoard(void)
{
  DisableInterrupts();
	init_serial_debug(true, true);
  gp_timer_config_32(TIMER0_BASE, TIMER_TAMR_TAMR_PERIOD, false, true); 
  ft6x06_init();
	lp_io_init();	
	ps2_initialize();
	lcd_config_gpio();
	lcd_config_screen();
  lcd_clear_screen(LCD_COLOR_BLUE2);
	gpio_config_port_control(GPIOF_BASE, GPIO_PCTL_PF2_M, GPIO_PCTL_M1PWM6);
	gpio_config_alternate_function(GPIOF_BASE,PF2);
	pwmConfig(PWM0GENA, PWM0GENB);
	eeprom_init();
  EnableInterrupts();
}

//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
	 // used for reading the position of the touch event
	uint16_t x,y;
  uint8_t touch_event;
	char msg3[2];		// used for printing total score
	char msg4[2];		// used for printing highest point
	msg0 = malloc(20 * sizeof(char)); // used for printing message
	
	initializeBoard();
	
  put_string("\n\r");
  put_string("************************************\n\r");
  put_string("ECE353 - Spring 2017 Final project\n\r  ");
  put_string("\n\rName:");
  put_string(individual);
  put_string("\n\r");  
  put_string("************************************\n\r");
	
	
  // Reach infinite loop
  while(1){	
		// The welcome page
		msg0 = "Welcome to";
		lcd_print_stringXY(msg0,0,2,LCD_COLOR_GREEN2,LCD_COLOR_BLUE2);
		// print the cloud
		lcd_print_cloudXY(1,150,LCD_COLOR_WHITE,LCD_COLOR_BLUE2);
		// print the words "Jumping Rabbit"
		lcd_print_jrXY(35, 180,LCD_COLOR_MAGENTA,LCD_COLOR_WHITE );
		// print the words "PRESS TO BEGIN"
		lcd_print_beginXY(20, 80,LCD_COLOR_WHITE,LCD_COLOR_BLUE2 );
		// print the rabbit
		lcd_print_menuRabbitXY(20, 102,LCD_COLOR_WHITE,LCD_COLOR_BLUE2 );
		// print the grass
		lcd_print_menugrassXY(1, 0,LCD_COLOR_GREEN2,LCD_COLOR_BLUE2 );
		
	// Determine how many active touch events there are.  If there are more than
  // 0, then read the x and y coordinates and print them out to the serial debug
  // teriminal using printf.	
	do{
			gp_timerA_wait(TIMER0_BASE, 5000000);
			touch_event = ft6x06_read_td_status();
			if(touch_event > 0){
				x = ft6x06_read_x();
				y = ft6x06_read_y();
				printf("x= %d y= %d \n\r", x, y);
			}
	}while(!(y>=70 && y<=110));
	// if the words " PRESS TO BEGIN" is pressed
	// Reach the main menu
	if(y>=70 && y<= 110){
		// clear the screen
		lcd_clear_screen(LCD_COLOR_BLUE2);
		// print the cloud
		lcd_print_cloudXY(1,180,LCD_COLOR_WHITE,LCD_COLOR_BLUE2);
		// print the words "Main Menu"
		lcd_print_mainMenuXY(30,230,LCD_COLOR_MAGENTA,LCD_COLOR_WHITE);
		// print the words "Normal Mode" ( with sound )
		lcd_print_normalModeXY(15,120,LCD_COLOR_YELLOW,LCD_COLOR_BLUE2);
		// print the words "Quiet Mode" ( without sound
		lcd_print_quietModeXY(30,50,LCD_COLOR_GREEN2,LCD_COLOR_BLUE2);
	}
	y = 0; // clear the touch event
	do{		
			// wait for the player choose the mode
			gp_timerA_wait(TIMER0_BASE, 5000000);
			touch_event = ft6x06_read_td_status();
			if(touch_event > 0){
				x = ft6x06_read_x();
				y = ft6x06_read_y();
				printf("x= %d y= %d \n\r", x, y);
			}
	}while(!(y>=40 && y<=150));
	// if normal mode is chose
	if(y>=100 && y<= 150)
		make_sound = true;
	else // if quiet mode is chose
		make_sound = false;
	
		// initialize the game
		lcd_clear_screen(LCD_COLOR_BLUE2);
		grass_initial();
		// print the rabbit
		lcd_print_rabbitXY(x_pos,y_pos,LCD_COLOR_WHITE,LCD_COLOR_BLUE2);
		
		// start the game
		while(1){
			// save the rabbit previous position
			x_pre = x_pos;
			y_pre = y_pos;
			// debounce for the SW1 button
			if(ALERT_DEBOUNCE_UPDATE){
				if(!(lp_io_read_pin(SW1_BIT))){
					if(debounce_count < 6){
						debounce_count++;
					}
					if(debounce_count == 6){
						debounce_count = 7;
						SW1_pressed = true;
					}
				}
				ALERT_DEBOUNCE_UPDATE = false;
			}
			// ADC interrupt, update the rabbit position
			if(ALERT_ADC_UPDATE){
				// if the joystick goes left, rabbit goes left
				if(x_PS2>(goLeft)){
					if(x_pos < ROWS-RABBIT_WIDTH - 5){
						x_pos = x_pos + 5;
					}
				}
				// if the joystick goes right, rabbit goes right
				else if(x_PS2<(goRight)){
					if(x_pos > 5){
						x_pos = x_pos - 5;
					}
				}
				// wait until the SW1 button is pressed to let the rabbit jump
				if(SW1_pressed){
					if(y_pos > 4){
							y_pos = y_pos - 4; // if rabbit does not reach the bottom
					}
					else{		// if the rabbit reaches the bottom, game over
						// update the highest score and print message
						renew_highest();
						lcd_clear_screen(LCD_COLOR_RED);
						msg0 = "GAME OVER!";
						lcd_print_stringXY(msg0,2,7,LCD_COLOR_BLACK,LCD_COLOR_RED);
						
						msg0 = "YOUR SCORE";
						lcd_print_stringXY(msg0,2,9,LCD_COLOR_BLACK,LCD_COLOR_RED);				
						sprintf(msg3, "%d", totalPoint);
						// print your score
						lcd_print_stringXY(msg3,6,10,LCD_COLOR_BLACK,LCD_COLOR_RED);
						
						msg0 = "HIGHEST SCORE";
						sprintf(msg4, "%d", highest);
						// print highest score
						lcd_print_stringXY(msg0,0,12,LCD_COLOR_BLACK,LCD_COLOR_RED);
						lcd_print_stringXY(msg4,6,13,LCD_COLOR_BLACK,LCD_COLOR_RED);
						// free the malloc pointer
						free(msg0);
						// reach the infinite loop, press reset to start another game
						while(1){}
					}
					// if the rabbit is above approximately 1/4 of the screen, let the grass move down
					if(y_pos > (COLS-RABBIT_HEIGHT)*1/4){
							grass_move();
					}
					// if the rabbit reaches the top, let it falls
					if(y_pos >= (COLS-RABBIT_HEIGHT)- 6){
						up_count = 0;
					}		
					// else, let the rabbit fly until the up count is 0
					if(up_count>0){
						if(y_pos < COLS-RABBIT_HEIGHT - 6){
							y_pos = y_pos + 6;
						}
						up_count--;
					}
					
					// if the rabbit collides with the grass, increase the score and delete the grass
					if(getpointcheck()){
						uint16_t counting = 0;
						up_count = 45;
						totalPoint = totalPoint + 1;
						// if normal mode, making sound when the rabbit and grass collides
						if(make_sound){
							pwmEnable(500);
							while(counting < 60000){
								counting = counting +1;
							}
							pwmDisable();
						}
					}
				}
				ALERT_ADC_UPDATE = false;
				// if the rabbit did not move, do not redraw the rabbit
				if( x_pre != x_pos || y_pre != y_pos){
					lcd_print_rabbitXY(x_pre,y_pre,LCD_COLOR_BLUE2,LCD_COLOR_BLUE2);
					lcd_print_rabbitXY(x_pos,y_pos,LCD_COLOR_WHITE,LCD_COLOR_BLUE2);
				}
			}
			
		}
}
}

// initialize the grass images when the game starts
void grass_initial(){
	uint8_t i;
	uint16_t height = 60;
	for(i = 0; i < (sizeof(grass)/sizeof(grass[0])) ; i++){
		grass[i][0] = (width + 100) %(ROWS-GRASS_WIDTH);  
		width = grass[i][0];
		grass[i][1] = height + i*50;	
		lcd_print_grassXY(grass[i][0],grass[i][1],LCD_COLOR_GREEN2,LCD_COLOR_BLUE2);
	}
}

// let the grass move
void grass_move(){
	uint8_t i;
	for(i = 0; i < (sizeof(grass)/sizeof(grass[0])) ; i++){
		lcd_print_grassXY(grass[i][0],grass[i][1],LCD_COLOR_BLUE2,LCD_COLOR_BLUE2);
		grass[i][1] = grass[i][1] - 2;
		if(grass[i][1] < RABBIT_HEIGHT){
				grass[i][0] = (width + 120) %(ROWS-GRASS_WIDTH);
				width = grass[i][0];
				grass[i][1] = COLS-GRASS_HEIGHT;
		}
		if(grass[i][0] != 300){
			lcd_print_grassXY(grass[i][0],grass[i][1],LCD_COLOR_GREEN2,LCD_COLOR_BLUE2);
		}
	}
}

// chech whether the rabbit collides with the grass
bool getpointcheck(){
	uint16_t rbt_x, rbt_y, i;
	for(rbt_x = x_pos; rbt_x < x_pos + RABBIT_WIDTH; rbt_x++){
		for(i = 0; i < (sizeof(grass)/sizeof(grass[0])) ; i++){
			if(rbt_x>grass[i][0] && rbt_x < (grass[i][0] + GRASS_WIDTH)){
				for(rbt_y = y_pos; rbt_y < y_pos + RABBIT_HEIGHT; rbt_y++){
					if(rbt_y>grass[i][1] && rbt_y < (grass[i][1] + GRASS_HEIGHT)){
						lcd_print_grassXY(grass[i][0],grass[i][1],LCD_COLOR_BLUE2,LCD_COLOR_BLUE2);
						grass[i][0] = 300;
						return true;
					}
				}
			}
		}
	}
	return false;
}

//*****************************************************************************
//EEPROM
//*****************************************************************************

// update the highest score
void renew_highest(void){
	//eeprom_byte_write(I2C1_BASE, address, highest); //first set for highest score

	eeprom_byte_read(I2C1_BASE, address, &highest);
	if(totalPoint > highest){
		eeprom_byte_write(I2C1_BASE, address, totalPoint);
	  highest = totalPoint;
	}
		
}

