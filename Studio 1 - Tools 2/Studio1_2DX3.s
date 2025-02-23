;Template - Defining an Assembly File 
;
; Original: Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu

; McMaster University, Comp Eng 2DX3, Winter 2023
; Further resources: Valvano textbook, Chs. 3 & 4

;ADDRESS SETUP
;Define your I/O Port Addresses Here

SYSCTL_RCGCGPIO_R             EQU     0x400FE608         ;Step 1: GPIO Run Mode Clock Gating Control Register Address
GPIO_PORTF_DIR_R              EQU                        ;Step 3: GPIO Port F DIR Register Address
GPIO_PORTF_DEN_R              EQU                        ;Step 4: GPIO Port F DEN Register Address
GPIO_PORTF_DATA_R             EQU     0x4005D3FC         ;Step 5: GPIO Port F DATA Register Address

                              -


        AREA    |.text|, CODE, READONLY, ALIGN=2
        THUMB
        EXPORT Start

;Function PortF_Init
;Enable PortF and set bit PF4 for digital output
PortF_Init 
		;STEP 1 Activate clock (Set bit 5 in RCGCGPIO; In C pseudcode: SYSCTL_RCGCGPIO_R |= #0x20)
		 LDR R1, =SYSCTL_RCGCGPIO_R		;Stores the address of the RCGCGPIO register in R1
		 LDR R0, [R1]					;Dereferences R1 to put the contents of the RCGCGPIO register in R0
		 ORR R0,R0, #0x20				;Modifies the contents of R0 to set bit 5 without changing other bits
		 STR R0, [R1]					;Stores the new value into the RCGCGPIO register
		
		;STEP 2: Wait for Peripheral Ready
		 NOP
		 NOP
		 
		
		;STEP 3: Set Port Direction (set bit 4 to 1 for output)
					;Load the memory address of the GPIODIR Port F Register into R1 (pointer)
					;Load the contents from the memory address of GPIODIR Port F Register into R0
					;Modify the contents of R0 to set bit 4 without changing other bits
					;Store what is in R0 into address pointed by R1 
		 
		;STEP 4: Enable Digital Functioning (set bit 4 to 1 for digital enable)
					;Load the memory address of the GPIODEN Port F Register into R1 (pointer)
					;Load the contents from the memory address of GPIODEN Port F Register into R0
					;Modify the contents of R0 to set bit 4 without changing other bits
					;Store what is in R0 into address pointed by R1 

        BX LR               ; return from function 
       
Start 
	    BL PortF_Init       ; call and execute PortF_Init
		
		;STEP 5: Outputting a Logic High to Port F Pin 4 (to switch on D3)   
					;Load the memory address of the GPIODATA Port F Register into R1 (pointer)
					;Load the contents from the memory address of GPIODATA Port F Register into R0
					;Modify the contents of R0 to set bit 4 without changing other bits
					;Store what is in R0 into address pointed by R1 
		 
		ALIGN               ; directive for assembly			
        END                 ; End of function 