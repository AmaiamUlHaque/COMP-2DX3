/* Studio 7D Project Code   
		Sample code provided for studio to demonstrate I2C function and debugging 
		Uses modified I2C code by Valvano.

		Written by Dr. Tom Doyle
		
		Last Updated: March 7, 2024 by Dr. Shahrukh Athar
		Integrated I2C and UART to identify ToF sensor and display to RealTerm
*/

#include <stdint.h>
#include <stdio.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"
#include "i2c0.h" //modified version of Valvano i2c0.c  
#include "uart.h"

//Flash D1
void FlashLED1(int count) {
		while(count--) {
			GPIO_PORTN_DATA_R ^= 0b00000010; 								//hello world!
			SysTick_Wait10ms(10);														//.1s delay
			GPIO_PORTN_DATA_R ^= 0b00000010;			
		}
}

//Flash D2
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
		GPIO_PORTN_DIR_R |= 0x03;        								// make PN0:1 out (PN0 on-board LED D2; PN1 on-board LED D1)
		GPIO_PORTN_AFSEL_R &= ~0x03;     								// disable alt funct on PN0:1
		GPIO_PORTN_DEN_R |= 0x03;        								// enable digital I/O on PN0:1
    //GPIO_PORTN_PCTL_R = (GPIO_PORTN_PCTL_R&0xFFFFFF00)+0x00000000;
		GPIO_PORTN_AMSEL_R &= ~0x03;     								// disable analog functionality on PN0:1	
	  FlashLED1(1);																		// initialization check
	return;
}

int main(void) {

	uint8_t modelID, modelType;
	PLL_Init();
	SysTick_Init();
	I2C_Init();
	UART_Init();
	PortN_Init();
	
	FlashLED2(1);
		
	UART_printf("Studio 7D - Identify ToF Sensor:\r\n");
	
	I2C_Send2(0x29, 0x01, 0x0F);		// ToF Model ID Register
	SysTick_Wait10ms(50);
	modelID = I2C_Recv(0x29);				// Expected value 0xEA
	SysTick_Wait10ms(50);
	I2C_Send2(0x29, 0x01, 0x10);		// ToF Model Type Register
	SysTick_Wait10ms(50);
	modelType = I2C_Recv(0x29);				// Expected value 0xCC
	SysTick_Wait10ms(50);
	
	sprintf(printf_buffer, "Model ID = 0x%x, Model Type = 0x%x\r\n", modelID, modelType);
	UART_printf(printf_buffer);
}
