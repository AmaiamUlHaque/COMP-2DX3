//Lab 4 - Milestone 2 --> Section 6.4.2 of lab manual


#include <stdint.h>
#define NVIC_ST_CTRL_R          (*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile uint32_t *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile uint32_t *)0xE000E018))
#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value


// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = NVIC_ST_RELOAD_M;  // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
                                        // enable SysTick with core clock
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC;
}
// Time delay using busy wait.
// The delay parameter is in units of the core clock. (units of 8.333 nsec for 120 MHz clock)


void SysTick_Wait(uint32_t delay){
  volatile uint32_t elapsedTime;
  uint32_t startTime = NVIC_ST_CURRENT_R;
  do{
    elapsedTime = (startTime-NVIC_ST_CURRENT_R)&0x00FFFFFF;
  }
  while(elapsedTime <= delay);
}



// Time delay using busy wait.
// This assumes 120 MHz system clock.
void SysTick_Wait10ms(uint32_t delay){
  uint32_t i;
  for(i=0; i<delay; i++){
    SysTick_Wait(1200000);  // wait 10ms (assumes 120 MHz clock)
  }
}



//kept experimenting with numbers
void SysTick_WaitMinimum(uint32_t delay){
	uint32_t i;
  for(i=0; i<delay; i++){
    //SysTick_Wait(1200000);  // wait 10ms (assumes 120 MHz clock)
		//1,200,000
		//1'000,000
		//0'750'000
		//0,500,000
		//0,250,000
		//0'175'000
		//0'172'500 --> works
		//0'172'000 --> doesnt work
		SysTick_Wait(250000);  // wait 1.4375ms (assumes 120 MHz clock)
	}
	return;
}


//TO CALCULATE DELAY
//Delay = Number of ticks × Time per tick
// = (int)delay / 120MHz
// = 172500 / 120'000'000
// = 0.0014375s
// = 1.4375ms