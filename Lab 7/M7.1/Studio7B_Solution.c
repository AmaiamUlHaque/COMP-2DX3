/* Studio 7B Code
		Sample code provided for studio to demonstrate periodic interrupt 
		Based upon interrupt textbook code by Valvano.

		This code will use an onboard 16/32-bit timer to trigger an interrupt.  The trigger
		will cause the timer ISR to execute. The timer ISR will
		flash an onboard LED on/off. However, you can adapt this code
		to perform any operations simply by changing the code in the ISR
		
		The program is written to flash the onboard LED at a frequency of 10 Hz

		Written by Dr. Tom Doyle
		Last Updated: February 22, 2024, by Dr. Shahrukh Athar
*/

#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"


// Flash LED D1 (D1 is connected to PN1)
void FlashLED1(int count) {
		while(count--) {
			GPIO_PORTN_DATA_R ^= 0b00000010; 			// hello world!
			SysTick_Wait10ms(10);							// .1s delay
			GPIO_PORTN_DATA_R ^= 0b00000010;			
		}
}

//Flash LED D2 (D2 is connected to PN0)
void FlashLED2(int count) {
		while(count--) {
			GPIO_PORTN_DATA_R ^= 0b00000001; 			// hello world!
			SysTick_Wait10ms(10);							// .1s delay
			GPIO_PORTN_DATA_R ^= 0b00000001;			
		}
}

// Initialize onboard LEDs
void PortN_Init(void){
	//Use PortN onboard LEDs
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;			// Activate clock for Port N
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R12) == 0){};		// Allow time for clock to stabilize
	GPIO_PORTN_DIR_R |= 0x03;        							// Make PN0 and PN1 out (PN0/PN1 built-in LEDs)
	GPIO_PORTN_AFSEL_R &= ~0x03;     						// Disable alt funct on PN[0:1]
  GPIO_PORTN_DEN_R |= 0x03;        							// Enable digital I/O on PN[0:1]
																									
  //GPIO_PORTN_PCTL_R = (GPIO_PORTN_PCTL_R&0xFFFFFF00)+0x00000000;
  GPIO_PORTN_AMSEL_R &= ~0x03;     							// Disable analog functionality on PN[0:1]
	FlashLED1(1);											// Flash LED D1 (Hello World)
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


// Assumes 120MHz bus, bus period = 1/(120*10^6) = 8.33 ns
// If bus clock = timer clock, then max interrupt period = 1/(120*10^6) * 2^32 = 35.8s
// Since we multiply the period by 120 to match the units of 1 us, the period will be in 1us units
// We want to set the interrupt period to be 0.25 sec. Calculate the value that must be placed in period to achieve this:
// 0.25s = 250000 us (Fill in the blank)
void Timer3_Init(void){

	uint32_t period = 250000;						// 32-bit value in 1us increments
	
	// Step 1: Activate timer
	SYSCTL_RCGCTIMER_R = 0x08;				// (Step 1)Activate timer 
	SysTick_Wait10ms(1);							// Wait for the timer module to turn on
	
	
	// Step 2: Arm and Configure Timer Module
	TIMER3_CTL_R = 0x0;						// (Step 2-1) Disable Timer3 during setup (Timer stops counting)
	TIMER3_CFG_R = 0x0;						// (Step 2-2) Configure for 32-bit timer mode   
	TIMER3_TAMR_R = 0x2;						// (Step 2-3) Configure for periodic mode   
	TIMER3_TAPR_R = 0x0;						// (Step 2-4) Set prescale value to 0; i.e. Timer3 works with Maximum Freq = bus clock freq (120MHz)  
	TIMER3_TAILR_R = (period*120)-1; 			// (Step 2-5) Reload value (we multiply the period by 120 to match the units of 1 us)  
	TIMER3_ICR_R = 0x1;							// (Step 2-6) Acknowledge the timeout interrupt (Clear timeout flag of Timer3)
	TIMER3_IMR_R = 0x1;						// (Step 2-7) Arm timeout interrupt   
	
	
	// Step 3: Enable Interrupt at Processor side
	NVIC_EN1_R = 0x00000008;					// Enable IRQ 35 in NVIC 
	NVIC_PRI8_R = 0x40000000;					// Set Interrupt Priority to 2 
																
	EnableInt();									// Global Interrupt Enable
	
	
	// Step 4: Enable the Timer to start counting
	TIMER3_CTL_R = 0x1;						// Enable Timer3
} 


// This is the Interrupt Service Routine (ISR). This must be included and match the
// interrupt naming convention in startup_msp432e401y_uvision.s
// (Note - not the same as Valvano textbook).
void TIMER3A_IRQHandler(void){ 
	
	TIMER3_ICR_R = 0x1;							// Acknowledge Timer3 timeout
	FlashLED2(1);								// Execute user task -- we simply flash LED D2
}


// The main program -- notice how we are only initializing the micro and nothing else.
// Our configured interrupts are being handled and tasks executed on an event driven basis.
int main(void){
	PLL_Init();               							// set system clock to 120 MHz
	SysTick_Init();								// Initialize SysTick
	PortN_Init();									// Initialize the onboard LEDs on port N
	Timer3_Init();								// Initalize and configure the Timer3 Interrupt
	
	while(1){									// Inside an infinite while loop,
		WaitForInt();								// Call WaitForInt() function to wait for interrupts
	}
}