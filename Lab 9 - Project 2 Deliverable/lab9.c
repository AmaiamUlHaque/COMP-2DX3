// Amaiam Ul Haque
// March 31, 2025
// COMP ENG 2DX3 - Lab 9 - Project 2 Deliverable


//HEADER FILES ---------------------------------------------------------------------------------------------------------------
#include <stdint.h>
#include "tm4c1294ncpdt.h" 
#include "PLL.h"
#include "SysTick.h"

#include "uart.h"
#include "onboardLEDs.h"
#include "VL53L1X_api.h"

//GLOBAL CONSTANTS -----------------------------------------------------------------------------------------------------------
int32_t STEP = 16;


//GLOBAL VARIABLES -----------------------------------------------------------------------------------------------------------
uint32_t currentPos = 0;	//current position in steps
uint32_t direction = 1;		//direction toggle
//initial states of status outputs
uint32_t state0 = 0;
uint32_t state1 = 1; //default of toggle direction is CW
uint32_t state2 = 1; //default of toggle angle is 11.25
uint32_t state3 = 0;


//FUNCTION PROTOTYPES ---------------------------------------------------------------------------------------------------------
void PortN_Init(); //on board LEDs
void PortF_Init(); //on board LEDs
void PortJ_Init(); //on board push button
void PortH_Init(); //stepper motor
void PortG_init(); //ToF sensor

void spinCW();
void spinCCW();
void rotate(uint32_t steps, uint32_t dir);

void updateCurrPos(uint32_t stepsTaken);
void returnHome();

void statusOutput0 (uint32_t state);
void statusOutput1 (uint32_t state);
void statusOutput2 (uint32_t state);
void statusOutput3 (uint32_t state);
void clearAllStatusOutputs();

void toggleDirection();


//FUNCTION DEFINITIONS --------------------------------------------------------------------------------------------------------

//initialise port N for status output 0 & 1 (D1 = PN1 & D2 = PN0) as output
void PortN_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;                 // Activate the clock for Port N
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R12) == 0){};				// Allow time for clock to stabilize
		
	GPIO_PORTN_DIR_R=0b00000011;															// Enable PN0 and PN1 as outputs													
	GPIO_PORTN_DEN_R=0b00000011;															// Enable PN0 and PN1 as digital pins
	return;
}

//initialise port F for status output 2 & 3 (D3 = PF4 & D4 = PF0) as output
void PortF_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;                 	// Activate the clock for Port F
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R5) == 0){};					// Allow time for clock to stabilize
		
	GPIO_PORTF_DIR_R=0b00010001;															// Enable PF0 and PF4 as outputs
	GPIO_PORTF_DEN_R=0b00010001;															// Enable PF0 and PF4 as digital pins
	return;
}
	
//initialise port J for buttons 0 & 1 (B0 = PJ0 & B3 = PJ1) as input
void PortJ_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R8;                 // Activate the clock for Port M
	while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R8) == 0){};      // Allow time for clock to stabilize
		
	GPIO_PORTJ_DIR_R = 0b00000000;       								      // Enable PM0 and PM1 as inputs 
  GPIO_PORTJ_DEN_R = 0b00001111;														// Enable PM0 - PM4 as digital pins
	GPIO_PORTJ_PUR_R = 0x0F;   //Pull Up (when not pressing it) Resistor Active Low
	return;
}
	
//initialise port H for stepper motor (PH0-3) as output
void PortH_Init(void){
	//Use PortH pins (PH0-3) for output
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R7;						// activate clock for Port H
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R7) == 0){};		// allow time for clock to stabilize --> two clock cycles (same as x2 NOP)
	
	GPIO_PORTH_DIR_R |= 0x0F;        										// configure Port H pins (PH0-3) as output
  GPIO_PORTH_AFSEL_R &= ~0x0F;     										// disable alt funct on Port H pins (PH0-3)
  GPIO_PORTH_DEN_R |= 0x0F;        										// enable digital I/O on Port H pins (PH0-3)
																											// configure Port H as GPIO
  GPIO_PORTH_AMSEL_R &= ~0x0F;     										// disable analog functionality on Port H pins (PH0-3)
	return;
}




//motor spins CW taking 4 steps at a time
void spinCW(){
	uint32_t delay = 1;
	
	GPIO_PORTH_DATA_R = 0b00000011;
	SysTick_WaitMinimum(delay);
	
	GPIO_PORTH_DATA_R = 0b00000110; 
	SysTick_WaitMinimum(delay);
	
	GPIO_PORTH_DATA_R = 0b00001100; 
	SysTick_WaitMinimum(delay);
	
	GPIO_PORTH_DATA_R = 0b00001001; 
	SysTick_WaitMinimum(delay);
	
	updateCurrPos(4);
	return;
}



//motor spins CCW taking 4 steps at a time
void spinCCW(){
	uint32_t delay = 1;
	
	GPIO_PORTH_DATA_R = 0b00001001;
	SysTick_WaitMinimum(delay);
	
	GPIO_PORTH_DATA_R = 0b00001100; 
	SysTick_WaitMinimum(delay);
	
	GPIO_PORTH_DATA_R = 0b00000110; 
	SysTick_WaitMinimum(delay);
	
	GPIO_PORTH_DATA_R = 0b00000011;
	SysTick_WaitMinimum(delay);
	
	updateCurrPos(-4);
	return;
}



//actual STEP = 4*steps
//if dir == 1 --> CW
//if dir == 0 --> CCw
void rotate(uint32_t steps, uint32_t dir){ 
	//updates currentPos automatically
	if (dir == 1){
		for (uint32_t i=0; i<steps; i++){
			spinCW();
		}
		statusOutput3(1);
		SysTick_Wait10ms(1);
		statusOutput3(0);
	}
	else if(dir == 0){
		for (uint32_t i=0; i<steps; i++){
			spinCCW();
		}
		statusOutput3(1);
		SysTick_Wait10ms(1);
		statusOutput3(0);
	}
}



//FOR ALL STATUS OUTPUT FUNCTION CALLS 
// if state == 1 --> status output set
// else --> status output clear
void statusOutput0 (uint32_t state) {
	if (state == 1)
		GPIO_PORTN_DATA_R |= 0b00000010; //D1 on 
	else
		GPIO_PORTN_DATA_R &= 0b11111101; //D1 off
	return;
}

void statusOutput1 (uint32_t state) {
	if (state == 1)
		GPIO_PORTN_DATA_R |= 0b00000001; //D2 on
	else 
		GPIO_PORTN_DATA_R &= 0b11111110; //D2 off
	return;
}

void statusOutput2 (uint32_t state) {
	if (state == 1)
		GPIO_PORTF_DATA_R |= 0b00010000; //D3 on
	else
		GPIO_PORTF_DATA_R &= 0b11101111; //D3 off
	return;
}

void statusOutput3 (uint32_t state) {
	if (state == 1)
		GPIO_PORTF_DATA_R |= 0b00000001; //D4
	else
		GPIO_PORTF_DATA_R &= 0b11111110; //D3
	
	return;
}

void clearAllStatusOutputs(){
	GPIO_PORTF_DATA_R = 0b00000000;
	GPIO_PORTN_DATA_R = 0b00000000;
	return;
}





void toggleDirection(){
	state1 ^= 1;
	if (direction == 1)
		direction = 0;
	else
		direction = 1;
	return;
}
	

//updates currentPos based on stepsTaken
void updateCurrPos(uint32_t stepsTaken){
	currentPos = (currentPos + stepsTaken) % 2048; //mod operator for if currentPos goes over 2048 CW direction
	
	if (currentPos < 0){ //for if currentPos goes below 0 from CCW direction
		currentPos += 2048;
	}
	return;
}

void returnHome(){
	if (currentPos >= 1024){
		rotate((2048-currentPos)/4, 1);
	}
	else {
		rotate((currentPos)/4, 0);
	}
	SysTick_Wait10ms(100);
	//currentPos automatically reset back to 0 within spinCW and spinCCW functions
	return;
}


// MAIN ---------------------------------------------------------------------------------------------------------------------------

int main(void){
	PLL_Init();				// Default Set System Clock to 120MHz
	SysTick_Init();		// Initialize SysTick configuration
	PortN_Init();			// Initialize Port N
	PortF_Init();			// Initialize Port F
	PortJ_Init();			// Initialize Port J
	PortH_Init();			// Initialize Port H
	
	clearAllStatusOutputs();
	
	//rotate(STEP,direction);
	//statusOutput0(1);
	//SysTick_Wait10ms(1);
	//clearAllStatusOutputs();
	
	
	
	
	while (1){ 
		
		//INITIALLY OFF
		while (state0 == 0){ 
			clearAllStatusOutputs();
			//B0 - PJ0 - TOGGLE POWER
			if ((GPIO_PORTJ_DATA_R &= 0b00000001) == 0){ //once B0 pressed, turn ON
				state0 = 1;
				SysTick_Wait10ms(100); //wait 1s
			}
		}
			
		
		//B0 - PJ0 - TOGGLE POWER
		if ((GPIO_PORTJ_DATA_R &= 0b00000001) == 0) //once B0 pressed, turn OFF
				state0 ^= 1;
		
		//B1 - PJ1 - TOGGLE DIRECTION
		if ((GPIO_PORTJ_DATA_R &= 0b00000010) == 0) {
			toggleDirection();
		}
		
		
		//at this point, all statuses are checked
		//so adjust statusOutputs, then spin accordingly
		statusOutput0(state0);
		statusOutput1(state1);
		statusOutput2(state2);
		statusOutput3(state3);
		
		rotate(STEP, direction);
		
	}
	
	return 0;
}







