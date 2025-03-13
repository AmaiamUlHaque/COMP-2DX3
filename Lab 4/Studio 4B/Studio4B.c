// COMPENG 2DX3
// This program illustrates the interfacing of the Stepper Motor with the microcontroller

//  Written by Ama Simons
//  January 18, 2020
// 	Last Update by Dr. Shahrukh Athar on February 2, 2025

#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"


void PortM_Init(void){
	//Use PortM pins (PM0-PM3) for output
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R11;				// activate clock for Port M
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R11) == 0){};	// allow time for clock to stabilize
	GPIO_PORTM_DIR_R |= 0x0F;        								// configure Port M pins (PM0-PM3) as output
  GPIO_PORTM_AFSEL_R &= ~0x0F;     								// disable alt funct on Port M pins (PM0-PM3)
  GPIO_PORTM_DEN_R |= 0x0F;        								// enable digital I/O on Port M pins (PM0-PM3)
																									// configure Port M as GPIO
  GPIO_PORTM_AMSEL_R &= ~0x0F;     								// disable analog functionality on Port M	pins (PM0-PM3)	
	return;
}


void spin(){																			// Complete function spin to implement the Full-step Stepping Method
	uint32_t delay = 1;															// Does your motor spin clockwise or counter-clockwise?
	
	for(int i=0; i<512; i++){												// What should the upper-bound of i be for one complete rotation of the motor shaft?
		GPIO_PORTM_DATA_R = 0b00000011;
		SysTick_WaitMinimum(delay);											// What if we want to reduce the delay between steps to be less than 10 ms?
		GPIO_PORTM_DATA_R = 0b00000110;													// Complete the missing code.
		SysTick_WaitMinimum(delay);
		GPIO_PORTM_DATA_R = 0b00001100;													// Complete the missing code.
		SysTick_WaitMinimum(delay);
		GPIO_PORTM_DATA_R = 0b00001001;													// Complete the missing code.
		SysTick_WaitMinimum(delay);
	}
}

void spinCW(){
	uint32_t delay = 1;
	
	GPIO_PORTM_DATA_R = 0b00000011;
	SysTick_WaitMinimum(delay);
	
	GPIO_PORTM_DATA_R = 0b00000110;													// Complete the missing code.
	SysTick_WaitMinimum(delay);
	
	GPIO_PORTM_DATA_R = 0b00001100;													// Complete the missing code.
	SysTick_WaitMinimum(delay);
	
	GPIO_PORTM_DATA_R = 0b00001001;													// Complete the missing code.
	SysTick_WaitMinimum(delay);
}

void spinCCW(){
	uint32_t delay = 1;
	
	GPIO_PORTM_DATA_R = 0b00001001;
	SysTick_WaitMinimum(delay);
	
	GPIO_PORTM_DATA_R = 0b00001100;													// Complete the missing code.
	SysTick_WaitMinimum(delay);
	
	GPIO_PORTM_DATA_R = 0b00000110;													// Complete the missing code.
	SysTick_WaitMinimum(delay);
	
	GPIO_PORTM_DATA_R = 0b00000011;													// Complete the missing code.
	SysTick_WaitMinimum(delay);
}


void rotate(uint32_t steps, uint32_t dir){
	if (dir == 1){
		for (int i=0; i<steps; i++){
			spinCW();
		}
	}
	else if(dir == 0){
		for (int i=0; i<steps; i++){
			spinCCW();
		}
	}
}




int main(void){
	PLL_Init();																			// Default Set System Clock to 120MHz
	SysTick_Init();																	// Initialize SysTick configuration
	PortM_Init();																		// Initialize Port M
	
	
	uint32_t rev = 512; //512 steps = 1 revolution
	
	rotate(2*rev,1); //CW  2 revs
	rotate(1*rev,0); //CCW 1 rev
	return 0;
}