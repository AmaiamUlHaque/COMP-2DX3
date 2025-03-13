//Lab 4 - Milestone 1 --> Section 6.4.1 of lab manual

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
	//f = 100Hz
	//T = 1/100 = 0.01s = 10ms
	
	float percent;	
	percent = ((float)duty*100)/(255); //eg. if duty=25 --> 25*100/255 = 10 ish = percent as an int
	int percent_int;
	percent_int = (int)percent;
	GPIO_PORTN_DATA_R ^= 0b00000100;
	SysTick_Wait100micros(percent_int);  //Note the use of the new function 1ms
	GPIO_PORTN_DATA_R ^= 0b00000100;
	SysTick_Wait100micros(100-percent_int); //waits for the remaining time
}

void IntensitySteps(void){
	
	for (int i=0; i<=10; i++){ //duty cycle 0%->100%
		for (int j=0; j<10; j++){ //repeats each cycle 10 times
			DutyCycle_Percent(i*25);
		}
	}
	
	for (int i=10; i>=0; i--){ //duty cycle 100%->0%
		for (int j=0; j<10; j++){ //repeats each cycle 10 times
			DutyCycle_Percent(i*25);
		}
	}
	return;
}

int main(void){
	
	PLL_Init();								// Default Set System Clock to 120MHz
	SysTick_Init();						// Initialize SysTick configuration
	PortN_Init();							// Initialize Port N 
	
	
	while(1){
		IntensitySteps();
	}
	
}





