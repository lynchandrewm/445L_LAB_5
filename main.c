// main.c
// Runs on LM4F120/TM4C123
// Test the functions provided in DAC.c 
// Andrew Lynch
// February 21, 2017

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// SSI0Clk (SCLK, pin 4) connected to PA2
// SSI0Fss (!CS, pin 2) connected to PA3
// SSI0Tx (DIN, pin 3) connected to PA5
// see Figure 7.19 for complete schematic

#include <stdint.h>
#include "SysTick.h"
#include "PLL.h"
//#include "NewMusic.h"
#include "Player.h"
#include "Switch.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


int main(void){
  PLL_Init(Bus80MHz);
  DisableInterrupts();
  Player_Init();
  SysTick_Init();
  Switch_Init(); // SysTick Init here
  Switch_AssignTask(LeftSwitch, Press, Player_Restart);
  Switch_AssignTask(MiddleSwitch, Press, Player_Pause);
  Switch_AssignTask(RightSwitch, Press, Player_ChangeVolume);
  SysTick_Wait10ms(2);
  Player_Restart(); // Interrupts enabled here
  while(1){
    WaitForInterrupt();
  }
}
