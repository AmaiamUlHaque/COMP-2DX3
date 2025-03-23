// COMPENG 2DX3 Studio 5 Code Template

//  Written by Ama Simons
//  January 30, 2020
//  Last Update: Feb 8, 2024 by Dr. Shahrukh Athar, 
//	This code provides a base upon which you should develop the two experiments in Studio 5

#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "Systick.h"
#include "PLL.h"
#include <math.h>

uint8_t scan_value_8 = 0x00; //stores the 8-bit value for milestone 1
uint8_t scan_value_4 = 0b000; //stores the 8-bit value for milestone 2


void PortE_Init(void){	
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;		              // Activate the clock for Port E
	while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R4) == 0){};	      // Allow time for clock to stabilize
  
	GPIO_PORTE_DIR_R = 0b00001111;														// Enable PE0 - PE4 as outputs
	GPIO_PORTE_DEN_R = 0b00001111;                        		// Enable PE0 - PE4 as digital pins
	
	return;
	}

// For input
void PortM_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R11;                 // Activate the clock for Port M
	while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R11) == 0){};      // Allow time for clock to stabilize
		
	GPIO_PORTM_DIR_R = 0b00000000;       								      // Enable PM0 and PM1 as inputs 
  GPIO_PORTM_DEN_R = 0b00001111;														// Enable PM0 - PM4 as digital pins
	GPIO_PORTM_PUR_R = 0x0F;   //Pull Up (when not pressing it) Resistor Active Low
	return;
}

// Enable LED D1, D2. Remember D1 is connected to PN1 and D2 is connected to PN0
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


int main(void){
	PortE_Init();
	PortM_Init();
	PortN_Init();
	PortF_Init();

	while(1){// Keep checking if a button is pressed 
		
		GPIO_PORTF_DATA_R = 0b00000000;
		GPIO_PORTN_DATA_R = 0b00000000;
		
		// Code order: PM3 PM2 PM1 PM0 PE3 PE2 PE1 PE0 
		
		GPIO_PORTE_DATA_R = 0b11111110;
		
		// disable bottom two lines for continous ON instead of momentary
		int scan_value_4 = 0b0000;
		int scan_value_8 = 0x00;
		
		for(int i = 0; i < 4; i++){
			GPIO_PORTE_DATA_R = ~(1 << i);
			
			scan_value_8  = GPIO_PORTM_DATA_R & 0b00001111;
			scan_value_8 <<= 4; //shift to get 4 empty spots
			scan_value_8 |= (GPIO_PORTE_DATA_R & 0b00001111); 
			
			if((GPIO_PORTM_DATA_R & 0b1111) != 0b1111){
				
				if(scan_value_8 == 0b10111110) scan_value_4 = 0b0000; //0//
				if(scan_value_8 == 0b01110111) scan_value_4 = 0b0001; //1//
				if(scan_value_8 == 0b10110111) scan_value_4 = 0b0010; //2
				if(scan_value_8 == 0b11010111) scan_value_4 = 0b0011; //3
				if(scan_value_8 == 0b01111011) scan_value_4 = 0b0100; //4
				if(scan_value_8 == 0b10111011) scan_value_4 = 0b0101; //5
				if(scan_value_8 == 0b11011011) scan_value_4 = 0b0110; //6
				if(scan_value_8 == 0b01111101) scan_value_4 = 0b0111; //7
				if(scan_value_8 == 0b10111101) scan_value_4 = 0b1000; //8
				if(scan_value_8 == 0b11011101) scan_value_4 = 0b1001; //9
				if(scan_value_8 == 0b11100111) scan_value_4 = 0b1010; //A
				if(scan_value_8 == 0b11101011) scan_value_4 = 0b1011; //B
				if(scan_value_8 == 0b11101101) scan_value_4 = 0b1100; //C
				if(scan_value_8 == 0b11101110) scan_value_4 = 0b1101; //D
				if(scan_value_8 == 0b01111110) scan_value_4 = 0b1110; //*
				if(scan_value_8 == 0b11011110) scan_value_4 = 0b1111; //#
				
				
				GPIO_PORTF_DATA_R = 0b00000000;
				GPIO_PORTN_DATA_R = 0b00000000;
				
				if ((scan_value_4 & 0b0001) != 0) {
					GPIO_PORTN_DATA_R ^= 0b00000010; 
				}
				if ((scan_value_4 & 0b0010) != 0) {
					GPIO_PORTN_DATA_R ^= 0b00000001; 
				}
				if ((scan_value_4 & 0b0100) != 0) {
					GPIO_PORTF_DATA_R ^= 0b00010000; 
				}
				if ((scan_value_4 & 0b1000) != 0) {
					GPIO_PORTF_DATA_R ^= 0b00000001; 
				}
				
			}
		}
	}
}
