// COMPENG 2DX3 Studio 5 Code for Exercise 2

//  Written by Ama Simons
//  January 30, 2020
//  Last Update: Feb 8, 2024 by Dr. Shahrukh Athar, 
//	This code provides the solution for Studio 5 Exercise 2: 2x2 Keypad (through Push Buttons)

// EDITED STUDIO 5 EX2 SOLUTION FOR LAB 5 MILESTONE 3

#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "Systick.h"
#include "PLL.h"

void PortE_Init(void){	
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;		              // Activate the clock for Port E
	while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R4) == 0){};	      // Allow time for clock to stabilize
  
	GPIO_PORTE_DIR_R = 0b00001111;														// Enable PE0-PE3 as outputs
	GPIO_PORTE_DEN_R = 0b00001111;                        		// Enable PE0-PE3 as digital pins
	return;
	}

void PortM_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R11;                 // Activate the clock for Port M
	while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R11) == 0){};      // Allow time for clock to stabilize
		
	GPIO_PORTM_DIR_R = 0b00000000;       								      // Enable PM0-PM3 as inputs 
  GPIO_PORTM_DEN_R = 0b00001111;														// Enable PM0-PM3 as digital pins
	return;
}

//Enable LED D1, D2. Remember D1 is connected to PN1 and D2 is connected to PN0
void PortN_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;                 // Activate the clock for Port N
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R12) == 0){};				// Allow time for clock to stabilize
		
	GPIO_PORTN_DIR_R=0b00000011;															// Enable PN0 and PN1 as outputs													
	GPIO_PORTN_DEN_R=0b00000011;															// Enable PN0 and PN1 as digital pins
	return;
}

//Enable LED D3, D4. Remember D3 is connected to PF4 and D4 is connected to PF0
void PortF_Init(void){
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;                 	// Activate the clock for Port F
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R5) == 0){};					// Allow time for clock to stabilize
		
	GPIO_PORTF_DIR_R=0b00010001;															// Enable PF0 and PF4 as outputs
	GPIO_PORTF_DEN_R=0b00010001;															// Enable PF0 and PF4 as digital pins
	return;
}


void LEDsOn (uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4){
	
	// LED: 	
	//      D1  D2  D3  D4
	//      PN1 PN0 PF4 PF0
	
	//D1
	if (d1)
		GPIO_PORTN_DATA_R = 0b00000010;
	else 
		GPIO_PORTN_DATA_R = 0b00000000;
	
	//D2
	if (d2)
		GPIO_PORTN_DATA_R = 0b00000001;
	else
		GPIO_PORTN_DATA_R = 0b00000000;
	
	//D3
	if (d3)
		GPIO_PORTF_DATA_R = 0b00010000;
	else
		GPIO_PORTF_DATA_R = 0b00000000;
	
	//D4
	if (d1)
		GPIO_PORTN_DATA_R = 0b00000001;
	else
		GPIO_PORTF_DATA_R = 0b00000000;
	
	return;
}

int main(void){
	PortE_Init();
	PortM_Init();
	PortN_Init();
	PortF_Init();
	
	
	
	
	

	while(1){// Keep checking if a button is pressed 
		
		//drive PE0 (row 0) low for scanning
		GPIO_PORTE_DATA_R =  0b11111110;
		
		while ((GPIO_PORTM_DATA_R & 0b00000001)==0) //D
			LEDsOn(1,1,0,1);
		while ((GPIO_PORTM_DATA_R & 0b00000010)==0) //#
			LEDsOn(1,1,1,1);
		while ((GPIO_PORTM_DATA_R & 0b00000100)==0) //0
			LEDsOn(0,0,0,0);
		while ((GPIO_PORTM_DATA_R & 0b00001000)==0) //*
			LEDsOn(1,1,1,0);
		
		
		
		//drive PE1 (row 1) low for scanning
		GPIO_PORTE_DATA_R =  0b11111101;
		
		while ((GPIO_PORTM_DATA_R & 0b00000001)==0) //C
			LEDsOn(1,1,0,0);
		while ((GPIO_PORTM_DATA_R & 0b00000010)==0) //9
			LEDsOn(1,0,0,1);
		while ((GPIO_PORTM_DATA_R & 0b00000100)==0) //8
			LEDsOn(1,0,0,0);
		while ((GPIO_PORTM_DATA_R & 0b00001000)==0) //7
			LEDsOn(0,1,0,1);
		
		
		
		//drive PE2 (row 2) low for scanning
		GPIO_PORTE_DATA_R =  0b11111011;
		
		while ((GPIO_PORTM_DATA_R & 0b00000001)==0) //B
			LEDsOn(1,0,1,1);
		while ((GPIO_PORTM_DATA_R & 0b00000010)==0) //6
			LEDsOn(0,1,1,0);
		while ((GPIO_PORTM_DATA_R & 0b00000100)==0) //5
			LEDsOn(0,1,0,1);
		while ((GPIO_PORTM_DATA_R & 0b00001000)==0) //4
			LEDsOn(0,1,0,0);
		
		
		
		//drive PE3 (row 3) low for scanning
		GPIO_PORTE_DATA_R =  0b11110111;
		
		while ((GPIO_PORTM_DATA_R & 0b00000001)==0) //A
			LEDsOn(1,0,1,0);
		while ((GPIO_PORTM_DATA_R & 0b00000010)==0) //3
			LEDsOn(0,0,1,1);
		while ((GPIO_PORTM_DATA_R & 0b00000100)==0) //2
			LEDsOn(0,0,1,0);
		while ((GPIO_PORTM_DATA_R & 0b00001000)==0) //1
			LEDsOn(0,0,0,1);
		
	}
}