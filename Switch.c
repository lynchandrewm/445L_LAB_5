// Switch.c
// Runs on TMC4C123
// Use GPIO in edge time mode to request interrupts on any
// edge of PF4 and start Timer0B. In Timer0B one-shot
// interrupts, record the state of the switch once it has stopped
// bouncing.
// Daniel Valvano
// May 3, 2015

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
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

// PF4 connected to a negative logic switch using internal pull-up (trigger on both edges)
#include <stdint.h>
#include <stdbool.h>
#include "../inc/tm4c123gh6pm.h"
#include "Switch.h"
#include "SysTick.h"
#include "../driverlib/gpio.h"
#include "../driverlib/pin_map.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/interrupt.h"

#define PB3                     (*((volatile uint32_t *)0x40005020))
#define PC6                     (*((volatile uint32_t *)0x40006100))
#define PF4                     (*((volatile uint32_t *)0x40025040))
#define PORTB_ADDRESS 0x40005000
#define PORTC_ADDRESS 0x40006000
#define PORTF_ADDRESS 0x40025000
#define PIN_3 0x8;
#define PIN_4 0x10;
#define PIN_6 0x40;
#define GPIOB_PRI_MASK 0xFFFF00FF
#define GPIOC_PRI_MASK 0xFF00FFFF
#define GPIOF_PRI_MASK 0xFF00FFFF
#define GPIOB_PRI_5 0x0000A000
#define GPIOC_PRI_5 0x00A00000
#define GPIOF_PRI_5 0x00A00000
#define GPIOB_NVIC_INT 0x00000002
#define GPIOC_NVIC_INT 0x00000004
#define GPIOF_NVIC_INT 0x40000000
#define GPIOB_SYSCTL 0x00000002
#define GPIOC_SYSCTL 0x00000004
#define GPIOF_SYSCTL 0x00000020
#define PIN3_CONFIG 0x0000F000
#define PIN6_CONFIG 0x0F000000
#define PIN4_CONFIG 0x000F0000
#define GPIO_DISABLE_ANALOG 0

#define SWITCH_UNPRESSED 1

#define DEBOUNCE_TIME 2 // 2ms


/*
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
*/


volatile static unsigned long PreviousPB3;
volatile static unsigned long PreviousPC6;
volatile static unsigned long PreviousPF4;
void (*TouchTaskPB3)(void);
void (*TouchTaskPC6)(void);
void (*TouchTaskPF4)(void);
void (*ReleaseTaskPB3)(void);
void (*ReleaseTaskPC6)(void);
void (*ReleaseTaskPF4)(void);

// This does nothing. All switches are initially set to this function
void static NullTask(void){
  return;
}

static void GPIOPB3Arm(void){
  GPIO_PORTB_ICR_R = PIN_3;      // (e) clear flag3
  GPIO_PORTB_IM_R |= PIN_3;      // (f) arm interrupt on PB3 *** No IME bit as mentioned in Book ***
  NVIC_PRI0_R = (NVIC_PRI0_R&GPIOB_PRI_MASK)|GPIOB_PRI_5; // (g) priority 5
  NVIC_EN0_R = GPIOB_NVIC_INT;      // (h) enable interrupt 1 in NVIC  
}

static void GPIOPC6Arm(void){
  GPIO_PORTC_ICR_R = PIN_6;      // (e) clear flag6
  GPIO_PORTC_IM_R |= PIN_6;      // (f) arm interrupt on PC6 *** No IME bit as mentioned in Book ***
  NVIC_PRI0_R = (NVIC_PRI0_R&GPIOC_PRI_MASK)|GPIOC_PRI_5; // (g) priority 5
  NVIC_EN0_R = GPIOC_NVIC_INT;      // (h) enable interrupt 2 in NVIC  
}

static void GPIOPF4Arm(void){
  GPIO_PORTF_ICR_R = PIN_4;      // (e) clear flag4
  GPIO_PORTF_IM_R |= PIN_4;      // (f) arm interrupt on PF4 *** No IME bit as mentioned in Book ***
  NVIC_PRI7_R = (NVIC_PRI7_R&GPIOF_PRI_MASK)|GPIOF_PRI_5; // (g) priority 5
  NVIC_EN0_R = GPIOF_NVIC_INT;      // (h) enable interrupt 30 in NVIC  
}

void Switch_Init(){
  SYSCTL_RCGCGPIO_R |= GPIOB_SYSCTL; // (a) activate clock for port B
  while((SYSCTL_PRGPIO_R & GPIOB_SYSCTL) == 0){};
  GPIO_PORTB_DIR_R &= ~PIN_3;    // (c) make PB3 in (built-in button)
  GPIO_PORTB_AFSEL_R &= ~PIN_3;  //     disable alt funct on PB3
  GPIO_PORTB_DEN_R |= PIN_3;     //     enable digital I/O on PB3   
  GPIO_PORTB_PCTL_R &= ~PIN3_CONFIG; // configure PB3 as GPIO
  GPIO_PORTB_AMSEL_R = GPIO_DISABLE_ANALOG;       //     disable analog functionality on PB
  GPIO_PORTB_PUR_R |= PIN_3;     //     enable weak pull-up on PB3
  GPIO_PORTB_IS_R &= ~PIN_3;     // (d) PB3 is edge-sensitive
  GPIO_PORTB_IBE_R |= PIN_3;     //     PB3 is both edges
  
  SYSCTL_RCGCGPIO_R |= GPIOC_SYSCTL; // (a) activate clock for port C
  while((SYSCTL_PRGPIO_R & GPIOC_SYSCTL) == 0){};
  GPIO_PORTC_DIR_R &= ~PIN_6;    // (c) make PC6 in (built-in button)
  GPIO_PORTC_AFSEL_R &= ~PIN_6;  //     disable alt funct on PC6
  GPIO_PORTC_DEN_R |= PIN_6;     //     enable digital I/O on PC6   
  GPIO_PORTC_PCTL_R &= ~PIN6_CONFIG; // configure PC6 as GPIO
  GPIO_PORTC_AMSEL_R = GPIO_DISABLE_ANALOG;       //     disable analog functionality on PC
  GPIO_PORTC_PUR_R |= PIN_6;     //     enable weak pull-up on PC6
  GPIO_PORTC_IS_R &= ~PIN_6;     // (d) PC6 is edge-sensitive
  GPIO_PORTC_IBE_R |= PIN_6;     //     PC6 is both edges
  
  SYSCTL_RCGCGPIO_R |= GPIOF_SYSCTL; // (a) activate clock for port F
  while((SYSCTL_PRGPIO_R & GPIOF_SYSCTL) == 0){};
  GPIO_PORTF_DIR_R &= ~PIN_4;    // (c) make PF4 in (built-in button)
  GPIO_PORTF_AFSEL_R &= ~PIN_4;  //     disable alt funct on PF4
  GPIO_PORTF_DEN_R |= PIN_4;     //     enable digital I/O on PF4   
  GPIO_PORTF_PCTL_R &= ~PIN4_CONFIG; // configure PF4 as GPIO
  GPIO_PORTF_AMSEL_R = GPIO_DISABLE_ANALOG;       //     disable analog functionality on PF
  GPIO_PORTF_PUR_R |= PIN_4;     //     enable weak pull-up on PF4
  GPIO_PORTF_IS_R &= ~PIN_4;     // (d) PF4 is edge-sensitive
  GPIO_PORTF_IBE_R |= PIN_4;     //     PF4 is both edges

  TouchTaskPB3 = NullTask;      // Set hooks to a null task, just in case
  TouchTaskPC6 = NullTask;
  TouchTaskPF4 = NullTask;
  ReleaseTaskPB3 = NullTask;
  ReleaseTaskPC6 = NullTask;
  ReleaseTaskPF4 = NullTask;
  PreviousPB3 = SWITCH_UNPRESSED;    // Assume no one is holding the button down
  PreviousPC6 = SWITCH_UNPRESSED;
  PreviousPF4 = SWITCH_UNPRESSED;
    
  GPIOPB3Arm();                 // Arm the switches
  GPIOPC6Arm();
  GPIOPF4Arm();
}

// Assign task to the left, middle, or right switch. Can assign Press or Release
void Switch_AssignTask(uint8_t switchNumber, uint8_t PressOrRelease, void(*task)(void)){
  switch(switchNumber){               
    case LeftSwitch:
      if(PressOrRelease == Press){
        TouchTaskPB3 = task;
      }
      else{
        ReleaseTaskPB3 = task;
      }
      break;
    case MiddleSwitch:
      if(PressOrRelease == Press){
        TouchTaskPC6 = task;
      }
      else{
        ReleaseTaskPC6 = task;
      }
      break;
    case RightSwitch:
      if(PressOrRelease == Press){
        TouchTaskPF4 = task;
      }
      else{
        ReleaseTaskPF4 = task;
      }
      break;
  }
}
  
//interrupts on edge triggered switch 
void GPIOPortB_Handler(void){
  GPIO_PORTB_ICR_R = PIN_3;      // (e) clear flag3
  GPIO_PORTB_IM_R &= ~PIN_3;     // disarm interrupt on PF4 
  if(PreviousPB3){             // Unpressed is 1
    (*TouchTaskPB3)();
  } 
  else{
    (*ReleaseTaskPB3)();
  }
  SysTick_Wait10ms(DEBOUNCE_TIME); //wait ~20ms 
  PreviousPB3 = PB3;
  GPIOPB3Arm();
}

//interrupts on edge triggered switch 
void GPIOPortC_Handler(void){
  GPIO_PORTC_ICR_R = PIN_6;      // (e) clear flag3
  GPIO_PORTC_IM_R &= ~PIN_6;     // disarm interrupt on PF4 
  if(PreviousPC6){              // Unpressed is 1
    (*TouchTaskPC6)();
  }
  else{
    (*ReleaseTaskPC6)();
  }
  SysTick_Wait10ms(DEBOUNCE_TIME); //wait ~20ms 
  PreviousPC6 = PC6;
  GPIOPC6Arm();
}

//interrupts on edge triggered switch 
void GPIOPortF_Handler(void){
  GPIO_PORTF_ICR_R = PIN_4;      // (e) clear flag3
  GPIO_PORTF_IM_R &= ~PIN_4;     // disarm interrupt on PF4 
  if(PreviousPF4){              // Unpressed is 1
    (*TouchTaskPF4)();
  }
  else{
    (*ReleaseTaskPF4)();
  }
  SysTick_Wait10ms(DEBOUNCE_TIME); //wait ~20ms 
  PreviousPF4 = PF4;
  GPIOPF4Arm();
}
  