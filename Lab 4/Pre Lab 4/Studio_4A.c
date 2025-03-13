// 2DX4_Knowledge_Thread_3_Session 0
// This program illustrates the use of SysTick in the C language.
// Note the library headers asscoaited are PLL.h and SysTick.h,
// which define functions and variables used in PLL.c and SysTick.c.
//

//
//  Written by Ama Simons
//  January 18, 2020
//  Last Update: January 18, 2020


#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"


void PortN_Init(void){
	//Use PortN onboard LED	
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;				// activate clock for Port N
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R12) == 0){};	// allow time for clock to stabilize
	GPIO_PORTN_DIR_R |= 0x05;        								// make PN0 out (PN0 built-in LED1)
  GPIO_PORTN_AFSEL_R &= ~0x05;     								// disable alt funct on PN0
  GPIO_PORTN_DEN_R |= 0x05;        								// enable digital I/O on PN0
																									// configure PN1 as GPIO
  //GPIO_PORTN_PCTL_R = (GPIO_PORTN_PCTL_R&0xFFFFFF0F)+0x00000000;
  GPIO_PORTN_AMSEL_R &= ~0x05;     								// disable analog functionality on PN0		
	
	GPIO_PORTN_DATA_R ^= 0b00000001; 								//hello world!
	SysTick_Wait10ms(10);														//.1s delay
	GPIO_PORTN_DATA_R ^= 0b00000001;	
	return;
}


void DutyCycle_Percent(uint8_t duty){
		//a period for this cycle is 1s --> 1000*1ms = 1000ms = 1s 
		float percent;	
		percent = ((float)duty*1000)/(255);
		int percent_int;
		percent_int = (int)percent;
		GPIO_PORTN_DATA_R ^= 0b00000100;
		SysTick_Wait1ms(percent_int);  //Note the use of the new function 1ms
		GPIO_PORTN_DATA_R ^= 0b00000100;
		SysTick_Wait1ms(1000-percent_int); //waits for the remaining time
}

int main(void){
	
	PLL_Init();								// Default Set System Clock to 120MHz
	SysTick_Init();						// Initialize SysTick configuration
	PortN_Init();							// Initialize Port N 

															//25 = 10 duty cycle
	uint8_t duty = 5*25; 				//5*25 is 50% duty cycle
	while(1){
		DutyCycle_Percent(duty);
	}
}





