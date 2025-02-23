;Template - Defining an Assembly File 
;
; Original: Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
;
; Modified by: Yaser M. Haddara
; January 21, 2023
; Program for studio 1A and 1B
; 1A: switch on D3
; 1B: use a pushbutton to control a user LED
;
; McMaster University, Comp Eng 2DX3, Winter 2023
; Further resources: Valvano textbook, Chs. 3 & 4
;

;ADDRESS SETUP
;Define your I/O Port Addresses Here

SYSCTL_RCGCGPIO_R             EQU     0x400FE608         ;Step 1: GPIO Run Mode Clock Gating Control Register Address

GPIO_PORTN_DIR_R              EQU     0x40064400         ;Step 3: GPIO Port F DIR Register Address
GPIO_PORTN_DEN_R              EQU     0x4006451C         ;Step 4: GPIO Port F DEN Register Address
GPIO_PORTN_DATA_R             EQU     0x400643FC         ;Step 5: GPIO Port F DATA Register Address

                              



        AREA    |.text|, CODE, READONLY, ALIGN=2
        THUMB
        EXPORT Start

;Function PortL_Init
;Enable PortF and set bit PF4 for digital output
PortN_Init 
		;STEP 1 Activate clock (Set bit 5 in RCGCGPIO; In C pseudcode: SYSCTL_RCGCGPIO_R |= #0x20)
		 LDR R1, =SYSCTL_RCGCGPIO_R		;Stores the address of the RCGCGPIO register in R1
		 LDR R0, [R1]					;Dereferences R1 to put the contents of the RCGCGPIO register in R0
		 ORR R0,R0, #0x1000				;Modifies the contents of R0 to set bit 12 without changing other bits
		 STR R0, [R1]					;Stores the new value into the RCGCGPIO register
		
		;STEP 2: Wait for Peripheral Ready
		 NOP
		 NOP
		 
		
		;STEP 3: Set Port Direction (set bit 0 to 1 for output)
		LDR R1, =GPIO_PORTN_DIR_R		;Load the memory address of the GPIODIR Port F Register into R1 (pointer)
		LDR R0, [R1]					;Load the contents from the memory address of GPIODIR Port F Register into R0
		ORR R0, R0, #0x01				;Modify the contents of R0 to clear bit 0 without changing other bits
		STR R0, [R1]					;Store what is in R0 into address pointed by R1 
		 
		;STEP 4: Enable Digital Functioning (set bit 0 to 1 for digital enable)
		LDR R1, =GPIO_PORTN_DEN_R		;Load the memory address of the GPIODEN Port F Register into R1 (pointer)
		LDR R0, [R1]					;Load the contents from the memory address of GPIODEN Port F Register into R0
		ORR R0, R0, #0x01				;Modify the contents of R0 to set bit 0 without changing other bits
		STR R0, [R1]					;Store what is in R0 into address pointed by R1 

        BX LR               ; return from function 

		
Start 
	    BL PortN_Init       ; call and execute PortN_Init
		
LedON ;STEP 5: Outputting a Logic High to Port N Pin 0 (to switch on D0)   
		LDR R1, =GPIO_PORTN_DATA_R		;Load the memory address of the GPIODATA Port N Register into R1 (pointer)
		LDR R0, [R1]					;Load the contents from the memory address of GPIODATA Port N Register into R0
		ORR R0, R0, #0x01				;Modify the contents of R0 to set bit 0 without changing other bits
		STR R0, [R1]
		
			
		;delay
		LDR R7, =1200000 ; 1.2 million cycles = 10ms
Debounce
		SUBS R7, #1
		BNE Debounce


LedOFF ;STEP 6: Outputting a Logic Low to Port N Pin 0 (to switch off D0)   
		LDR R1, =GPIO_PORTN_DATA_R		;Load the memory address of the GPIODATA Port N Register into R1 (pointer)
		LDR R0, [R1]					;Load the contents from the memory address of GPIODATA Port N Register into R0
		AND R0, R0, #0xFE				;Modify the contents of R0 to clear bit 0 without changing other bits
		STR R0, [R1]					;Store what is in R0 into address pointed by R1 
		
		
		LDR R7, =1200000 ; 1.2 million cycles = 10ms
Debounce1
		SUBS R7, #1
		BNE Debounce1
		
		B LedON
		
		
		ALIGN               ; directive for assembly			
        END                 ; End of function 