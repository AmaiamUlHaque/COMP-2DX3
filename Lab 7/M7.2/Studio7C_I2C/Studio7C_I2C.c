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


// Enable interrupts
void EnableInt(void)
{    __asm("    cpsie   i\n");
}

// Disable interrupts
void DisableInt(void)
{    __asm("    cpsid   i\n");
}

// Low power wait
void WaitForInt(void)
{    __asm("    wfi\n");
}

// Global variable visible in Watch window of debugger
// Increments at least once per button press
volatile unsigned long FallingEdges = 0;

// Give clock to Port J and initalize PJ1 as Digital Input GPIO
void PortJ_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R8;					// Activate clock for Port J
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R8) == 0){};	// Allow time for clock to stabilize
  GPIO_PORTJ_DIR_R &= ~0x02;    										// Make PJ1 input 
  GPIO_PORTJ_DEN_R |= 0x02;     										// Enable digital I/O on PJ1
	
	GPIO_PORTJ_PCTL_R &= ~0x000000F0;	 								//  Configure PJ1 as GPIO 
	GPIO_PORTJ_AMSEL_R &= ~0x02;											//  Disable analog functionality on PJ1		
	GPIO_PORTJ_PUR_R |= 0x02;													//	Enable weak pull up resistor on PJ1
}


// Interrupt initialization for GPIO Port J IRQ# 51
void PortJ_Interrupt_Init(void){
	
		FallingEdges = 0;             		// Initialize counter

		GPIO_PORTJ_IS_R =  0;  						// (Step 1) PJ1 is Edge-sensitive 
		GPIO_PORTJ_IBE_R = 0;  						//     			PJ1 is not triggered by both edges 
		GPIO_PORTJ_IEV_R = 0;  						//     			PJ1 is falling edge event 
		GPIO_PORTJ_ICR_R = 0x02; 					// 					Clear interrupt flag by setting proper bit in ICR register
		GPIO_PORTJ_IM_R = 0x02;  					// 					Arm interrupt on PJ1 by setting proper bit in IM register
    
		NVIC_EN1_R =   0x00080000;  			// (Step 2) Enable interrupt 51 in NVIC (which is in Register EN1) =irq# % 32 = 51 % 32 = 19
	
		NVIC_PRI12_R =  0xA0000000;							// (Step 4) Set interrupt priority to 5 --> = irq# / 4 = 12

		EnableInt();  										// (Step 3) Enable Global Interrupt. lets go!
	
		//staring bit = 5+8*(irq# / 4) = 5+8*(51%4) = 5+8*3= 29
}


//	(Step 5) IRQ Handler (Interrupt Service Routine).  
//  				This must be included and match interrupt naming convention in startup_msp432e401y_uvision.s 
//					(Note - not the same as Valvano textbook).
void GPIOJ_IRQHandler(void){
  FallingEdges = FallingEdges + 1;	// Increase the global counter variable ;Observe in Debug Watch Window
	FlashLED2(1);											// Flash the LED D2 once
	
	
	FlashLED2(1);		
	I2C_Send1(0x3B, 42);		//address, data 
	SysTick_Wait10ms(50);
	
	GPIO_PORTJ_ICR_R = 0x02; 					// Acknowledge flag by setting proper bit in ICR register
																		//use the same number as before before
	
}




int main(void) {
	PLL_Init();								// set system clock to 120 MHz
	SysTick_Init();						// initialize SysTick
	I2C_Init();								// initialize I2C
	PortN_Init();							// initialize Port N
	PortJ_Init();									// Initialize the onboard push button on PJ1
	PortJ_Interrupt_Init();				// Initalize and configure the Interrupt on Port J
	
	
//	SysTick_Wait10ms(100); // let peripherals boot
	
	while(1) {
		WaitForInt();	
//		FlashLED2(1);		
//		I2C_Send1(0x3B, 42);		//address, data 
//		SysTick_Wait10ms(50);
	}
}
