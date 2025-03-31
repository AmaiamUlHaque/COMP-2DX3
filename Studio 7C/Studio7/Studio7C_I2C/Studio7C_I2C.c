/* Studio 7C Project Code
		Sample code provided for studio to demonstrate I2C function and debugging 
		Uses modified I2C code by Valvano.
		
		Don't forget to connect Gnd to Gnd

		Written by Dr. Tom Doyle
		Last Updated: March 4, 2020
*/

#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"
#include "i2c0.h" //modified version of Valvano i2c0.c  


// Flash LED D1
void FlashLED1(int count) {
		while(count--) {
			GPIO_PORTN_DATA_R ^= 0b00000010; 								//hello world!
			SysTick_Wait10ms(10);														//.1s delay
			GPIO_PORTN_DATA_R ^= 0b00000010;			
		}
}

// Flash LED D2
void FlashLED2(int count) {
		while(count--) {
			GPIO_PORTN_DATA_R ^= 0b00000001; 								//hello world!
			SysTick_Wait10ms(10);														//.1s delay
			GPIO_PORTN_DATA_R ^= 0b00000001;			
		}
}

// Initialize onboard LEDs
void PortN_Init(void){
	//Use PortN onboard LED	
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;				// activate clock for Port N
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R12) == 0){};	// allow time for clock to stabilize
		GPIO_PORTN_DIR_R |= 0x03;        							// make PN1:0 output (PN1:0 built-in LEDs)
		GPIO_PORTN_AFSEL_R &= ~0x03;     							// disable alt funct on PN1:0
		GPIO_PORTN_DEN_R |= 0x03;        							// enable digital I/O on PN1:0
																									// configure PN1:0 as GPIO
  //GPIO_PORTN_PCTL_R = (GPIO_PORTN_PCTL_R&0xFFFFFF00)+0x00000000;
		GPIO_PORTN_AMSEL_R &= ~0x03;     							// disable analog functionality on PN1:0		
		FlashLED1(1);
		return;
}

int main(void) {
	PLL_Init();								// set system clock to 120 MHz
	SysTick_Init();						// initialize SysTick
	I2C_Init();								// initialize I2C
	PortN_Init();							// initialize Port N
	
//	SysTick_Wait10ms(100); // let peripherals boot
	
	while(1) {
		FlashLED2(1);		
		I2C_Send1(0x3B, 42);		//address, data 
		SysTick_Wait10ms(50);
	}
}
