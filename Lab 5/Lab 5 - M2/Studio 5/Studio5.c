// COMPENG 2DX3 Studio 5 Code for Exercise 2

//  Written by Ama Simons
//  January 30, 2020
//  Last Update: Feb 8, 2024 by Dr. Shahrukh Athar, 
//	This code provides the solution for Studio 5 Exercise 2: 2x2 Keypad (through Push Buttons)

// EDITED STUDIO 5 EX2 SOLUTION FOR LAB 5 MILESTONE 2

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



int main(void){
	PortE_Init();
	PortM_Init();

	uint32_t binaryKey = 0b00000000; //holds binary value of 8bit PM[3:0]PE[3:0]

	
	while(1){// Keep checking if a button is pressed 
		
		
		//drive PE0 (row 0) low for scanning
		GPIO_PORTE_DATA_R =  0b11111110;
		
		if ((GPIO_PORTM_DATA_R & 0b00000001)==0) 			//D
			binaryKey = 0b00001101;
		else if ((GPIO_PORTM_DATA_R & 0b00000010)==0) //#
			binaryKey = 0b00001111;
		else if ((GPIO_PORTM_DATA_R & 0b00000100)==0) //0
			binaryKey = 0b00000000;
		else if ((GPIO_PORTM_DATA_R & 0b00001000)==0) //*
			binaryKey = 0b00001110;
		
		
		
		//drive PE1 (row 1) low for scanning
		GPIO_PORTE_DATA_R =  0b11111101;
		
		if ((GPIO_PORTM_DATA_R & 0b00000001)==0) 			//C
			binaryKey = 0b00001100;
		else if ((GPIO_PORTM_DATA_R & 0b00000010)==0) //9
			binaryKey = 0b00001001; 
		else if ((GPIO_PORTM_DATA_R & 0b00000100)==0) //8
			binaryKey = 0b00001000; 
		else if ((GPIO_PORTM_DATA_R & 0b00001000)==0) //7
			binaryKey = 0b00000111;
		
		
		
		//drive PE2 (row 2) low for scanning
		GPIO_PORTE_DATA_R =  0b11111011;
		
		if ((GPIO_PORTM_DATA_R & 0b00000001)==0) 			//B
			binaryKey = 0b00001011;
		else if ((GPIO_PORTM_DATA_R & 0b00000010)==0) //6
			binaryKey = 0b00000110; 
		else if ((GPIO_PORTM_DATA_R & 0b00000100)==0) //5
			binaryKey = 0b00000101; 
		else if ((GPIO_PORTM_DATA_R & 0b00001000)==0) //4
			binaryKey = 0b00000100;
		
		
		
		//drive PE3 (row 3) low for scanning
		GPIO_PORTE_DATA_R =  0b11110111;
		
		if ((GPIO_PORTM_DATA_R & 0b00000001)==0) 			//A
			binaryKey = 0b00001010;
		else if ((GPIO_PORTM_DATA_R & 0b00000010)==0) //3
			binaryKey = 0b00000011; 
		else if ((GPIO_PORTM_DATA_R & 0b00000100)==0) //2
			binaryKey = 0b00000010; 
		else if ((GPIO_PORTM_DATA_R & 0b00001000)==0) //1
			binaryKey = 0b00000001;
		
		
		
	}
}