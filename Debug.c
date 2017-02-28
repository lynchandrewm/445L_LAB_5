// Debug.c
// Andrew Lynch

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

void PortB_Init(void){ unsigned long volatile delay;
  SYSCTL_RCGC2_R |= 0x02;       // activate port B
  delay = SYSCTL_RCGC2_R;        
  delay = SYSCTL_RCGC2_R;         
  GPIO_PORTB_DIR_R |= 0x3C;    // make PB5-2 output heartbeats
  GPIO_PORTB_AFSEL_R &= ~0x3C;   // disable alt funct on PB5-2
  GPIO_PORTB_DEN_R |= 0x3C;     // enable digital I/O on PB5-2
  GPIO_PORTB_PCTL_R = ~0x00FFFF00;
  GPIO_PORTB_AMSEL_R &= ~0x3C;      // disable analog functionality on PB
}
