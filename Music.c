// Music.c
// Andrew Lynch

#include <stdint.h>
#include "..//inc//tm4c123gh6pm.h"
#include "Music.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void static SetTimer0A(uint32_t period);
void static SetTimer0B(uint32_t period);
void static SetTimer1A(uint32_t period);
void static SetTimer1B(uint32_t period);
void static SetTimer2A(uint32_t period);
void static SetTimer2B(uint32_t period);

#define MAX_INSTRUMENT_INDEX 63
#define MAX_ALLEGRO_INDEX 93
#define MAX_STOCCATO1_INDEX 24
#define MAX_STOCCATO2_INDEX 24
#define MAX_STOCCATO3_INDEX 24
#define MAX_MELODY_INDEX 53
#define MAX_COUNTERMELODY_INDEX 55

typedef struct MusicStruct {
  uint32_t cyclesPerStep;
  uint32_t numberOfSteps;
} music;

uint32_t Timer0A_Count;
uint32_t Timer0B_Count;
uint32_t Timer1A_Count;
uint32_t Timer1B_Count;
uint32_t Timer2A_Count;
uint32_t Timer2B_Count;
uint32_t Allegro_InstrumentIndex;
uint32_t Stoccato1_InstrumentIndex;
uint32_t Stoccato2_InstrumentIndex;
uint32_t Stoccato3_InstrumentIndex;
uint32_t Melody_InstrumentIndex;
uint32_t CounterMelody_InstrumentIndex;

uint8_t AllegroIndex;
uint8_t Stoccato1Index;
uint8_t Stoccato2Index;
uint8_t Stoccato3Index;
uint8_t MelodyIndex;
uint8_t CounterMelodyIndex;

const uint16_t flute[64] = {
  1007 * 2 * 2, 1252 * 2 * 2, 1374 * 2, 1548 * 2, 1698 * 2, 1797 * 2, 1825 * 2, 1797 * 2, 1675 * 2, 1562 * 2, 1383 * 2, 1219 * 2, 1092 * 2, 
  1007 * 2, 913 * 2, 890 * 2, 833 * 2, 847 * 2, 810 * 2, 777 * 2, 744 * 2, 674 * 2, 598 * 2, 551 * 2, 509 * 2, 476 * 2, 495 * 2, 533 * 2, 589 * 2, 
  659 * 2, 758 * 2, 876 * 2, 1007 * 2, 1252 * 2, 1374 * 2, 1548 * 2, 1698 * 2, 1797 * 2, 1825 * 2, 1797 * 2, 1675 * 2, 1562 * 2, 1383 * 2, 
  1219 * 2, 1092 * 2, 1007 * 2, 913 * 2, 890 * 2, 833 * 2, 847 * 2, 810 * 2, 777 * 2, 744 * 2, 674 * 2, 598 * 2, 551 * 2, 509 * 2, 476 * 2, 495 * 2, 
  533 * 2, 589 * 2, 659 * 2, 758 * 2, 876 * 2
};
  
const uint16_t horn[64] = {
  1063 * 2, 1082 * 2, 1119 * 2, 1275 * 2, 1678 * 2, 1748 * 2, 1275 * 2, 755 * 2, 661 * 2, 661 * 2, 703 * 2, 731 * 2, 769 * 2, 845 * 2, 
  1039 * 2, 1134 * 2, 1209 * 2, 1332 * 2, 1465 * 2, 1545 * 2, 1427 * 2, 1588 * 2, 1370 * 2, 1086 * 2, 708 * 2, 519 * 2, 448 * 2, 490 * 2, 
  566 * 2, 684 * 2, 802 * 2, 992 * 2, 1063 * 2, 1082 * 2, 1119 * 2, 1275 * 2, 1678 * 2, 1748 * 2, 1275 * 2, 755 * 2, 661 * 2, 661 * 2, 703 * 2, 
  731 * 2, 769 * 2, 845 * 2, 1039 * 2, 1134 * 2, 1209 * 2, 1332 * 2, 1465 * 2, 1545 * 2, 1427 * 2, 1588 * 2, 1370 * 2, 1086 * 2, 708 * 2, 
  519 * 2, 448 * 2, 490 * 2, 566 * 2, 684 * 2, 802 * 2, 992 * 2
};

const uint16_t trumpet[64] = {
  987 * 2, 1049 * 2, 1090 * 2, 1110 * 2, 1134 * 2, 1160 * 2, 1139 * 2, 1092 * 2, 1070 * 2, 1042 * 2, 1035 * 2, 1029 * 2, 1008 * 2, 1066 * 2,
  1150 * 2, 1170 * 2, 1087 * 2, 915 * 2, 679 * 2, 372 * 2, 151 * 2, 558 * 2, 1014 * 2, 1245 * 2, 1260 * 2, 1145 * 2, 1063 * 2, 984 * 2, 934 * 2,
  960 * 2, 1027 * 2, 1077 * 2, 1081 * 2, 1074 * 2, 1064 * 2, 1042 * 2, 1010 * 2, 974 * 2, 968 * 2, 974 * 2, 994 * 2, 1039 * 2, 1094 * 2, 1129 * 2,
  1125 * 2, 1092 * 2, 1056 * 2, 1056 * 2, 1082 * 2, 1059 * 2, 1046 * 2, 1058 * 2, 1061 * 2, 1045 * 2, 1034 * 2, 1050 * 2, 1094 * 2, 1112 * 2,
  1092 * 2, 1063 * 2, 1053 * 2, 1065 * 2, 1052 * 2, 992 * 2
};

const uint16_t basson[64] = {
  1068 * 2, 1169 * 2, 1175 * 2, 1161 * 2, 1130 * 2, 1113 * 2, 1102 * 2, 1076 * 2, 1032 * 2, 985 * 2, 963 * 2, 987 * 2, 1082 * 2, 1343 * 2,
  1737 * 2, 1863 * 2, 1575 * 2, 1031 * 2, 538 * 2, 309 * 2, 330 * 2, 472 * 2, 626 * 2, 807 * 2, 1038 * 2, 1270 * 2, 1420 * 2, 1461 * 2, 1375 * 2,
  1201 * 2, 1005 * 2, 819 * 2, 658 * 2, 532 * 2, 496 * 2, 594 * 2, 804 * 2, 1055 * 2, 1248 * 2, 1323 * 2, 1233 * 2, 1049 * 2, 895 * 2, 826 * 2,
  826 * 2, 850 * 2, 862 * 2, 861 * 2, 899 * 2, 961 * 2, 1006 * 2, 1023 * 2, 1046 * 2, 1092 * 2, 1177 * 2, 1224 * 2, 1186 * 2, 1133 * 2, 1098 * 2,
  1102 * 2, 1109 * 2, 1076 * 2, 1027 * 2, 1003 * 2
};

const uint16_t oboe[64] = {
  1024 * 2, 1024 * 2, 1014 * 2, 1008 * 2, 1022 * 2, 1065 * 2, 1093 * 2, 1006 * 2, 858 * 2, 711 * 2, 612 * 2, 596 * 2, 672 * 2, 806 * 2, 952 * 2, 1074 * 2,
  1154 * 2, 1191 * 2, 1202 * 2, 1216 * 2, 1236 * 2, 1255 * 2, 1272 * 2, 1302 * 2, 1318 * 2, 1299 * 2, 1238 * 2, 1140 * 2, 1022 * 2, 910 * 2, 827 * 2,
  779 * 2, 758 * 2, 757 * 2, 782 * 2, 856 * 2, 972 * 2, 1088 * 2, 1177 * 2, 1226 * 2, 1232 * 2, 1203 * 2, 1157 * 2, 1110 * 2, 1067 * 2, 1028 * 2, 993 * 2,
  958 * 2, 929 * 2, 905 * 2, 892 * 2, 900 * 2, 940 * 2, 1022 * 2, 1125 * 2, 1157 * 2, 1087 * 2, 965 * 2, 836 * 2, 783 * 2, 816 * 2, 895 * 2, 971 * 2, 1017 * 2
};

const music Allegro[MAX_ALLEGRO_INDEX] = {
  { 40000000, 1 },
  { 4256, 4698 },
  { 18000000, 1 },
  { 3188, 6272 },
  { 18000000, 1 },
  { 4777, 4186 },
  { 18000000, 1 },
  { 3792, 5274 },
  { 18000000, 1 },
  { 2840, 7040 },
  { 18000000, 1 },
  { 2128, 9397 },
  { 18000000, 1 },
  { 4256, 4698 },
  { 18000000, 1 },
  { 3188, 6272 },
  { 18000000, 1 },
  { 4777, 4186 },
  { 18000000, 1 },
  { 3792, 5274 },
  { 18000000, 1 },
  { 2840, 7040 },
  { 18000000, 1 },
  { 2128, 9397 },
  { 18000000, 1 },
  { 4256, 4698 },
  { 18000000, 1 },
  { 3188, 6272 },
  { 18000000, 1 },
  { 4777, 4186 },
  { 18000000, 1 },
  { 3792, 5274 },
  { 18000000, 1 },
  { 2840, 7040 },
  { 18000000, 1 },
  { 2128, 9397 },
  { 18000000, 1 },
  { 4256, 4698 },
  { 18000000, 1 },
  { 3188, 6272 },
  { 18000000, 1 },
  { 4777, 4186 },
  { 18000000, 1 },
  { 3792, 5274 },
  { 18000000, 1 },
  { 2840, 7040 },
  { 18000000, 1 },
  { 2128, 9397 },
  { 18000000, 1 },
  { 4256, 4698 },
  { 18000000, 1 },
  { 3188, 6272 },
  { 18000000, 1 },
  { 4777, 4186 },
  { 18000000, 1 },
  { 3792, 5274 },
  { 18000000, 1 },
  { 2840, 7040 },
  { 18000000, 1 },
  { 2128, 9397 },
  { 18000000, 1 },
  { 4256, 4698 },
  { 18000000, 1 },
  { 3188, 6272 },
  { 18000000, 1 },
  { 4777, 4186 },
  { 18000000, 1 },
  { 3792, 5274 },
  { 18000000, 1 },
  { 2840, 7040 },
  { 18000000, 1 },
  { 2128, 9397 },
  { 18000000, 1 },
  { 4256, 4698 },
  { 18000000, 1 },
  { 3188, 6272 },
  { 18000000, 1 },
  { 4777, 4186 },
  { 18000000, 1 },
  { 3792, 5274 },
  { 18000000, 1 },
  { 2840, 7040 },
  { 18000000, 1 },
  { 2128, 9397 },
  { 18000000, 1 },
  { 4256, 4698 },
  { 18000000, 1 },
  { 3188, 6272 },
  { 18000000, 1 },
  { 4777, 4186 },
  { 18000000, 1 },
  { 3792, 5274 },
  { 18000000, 1 },
};

const music Stoccato3[MAX_STOCCATO3_INDEX] = {
  { 4256, 9397 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 4777, 8372 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 4256, 9397 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 4777, 8372 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 4256, 9397 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 4777, 8372 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 4256, 9397 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 4777, 8372 },
  { 36000000, 1 },
  { 80000000, 1 },
};

const music Stoccato2[MAX_STOCCATO2_INDEX] = {
  { 2530, 15804 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 2840, 14080 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 2530, 15804 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 2840, 14080 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 2530, 15804 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 2840, 14080 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 2530, 15804 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 2840, 14080 },
  { 36000000, 1 },
  { 80000000, 1 },
};

const music Stoccato1[MAX_STOCCATO1_INDEX] = {
  { 3188, 12544 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 3579, 11175 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 3188, 12544 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 3579, 11175 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 3188, 12544 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 3579, 11175 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 3188, 12544 },
  { 36000000, 1 },
  { 80000000, 1 },
  { 3579, 11175 },
  { 36000000, 1 },
  { 80000000, 1 },
};

const music Melody[MAX_MELODY_INDEX] = {
  { 3188, 12544 },
  { 36000000, 1 },
  { 4256, 9397 },
  { 36000000, 1 },
  { 3792, 10548 },
  { 36000000, 1 },
  { 3579, 11175 },
  { 36000000, 1 },
  { 3792, 10548 },
  { 36000000, 1 },
  { 4256, 9397 },
  { 36000000, 1 },
  { 3188, 12544 },
  { 36000000, 1 },
  { 4256, 9397 },
  { 36000000, 1 },
  { 3792, 10548 },
  { 36000000, 1 },
  { 2840, 14080 },
  { 36000000, 1 },
  { 2530, 15804 },
  { 36000000, 1 },
  { 4777, 8372 },
  { 36000000, 1 },
  { 3188, 12544 },
  { 36000000, 1 },
  { 4256, 9397 },
  { 36000000, 1 },
  { 3792, 10548 },
  { 36000000, 1 },
  { 3579, 5587 },
  { 18000000, 1 },
  { 3188, 6272 },
  { 18000000, 1 },
  { 2840, 14080 },
  { 36000000, 1 },
  { 3188, 3763 },
  { 2840, 4224 },
  { 3188, 3763 },
  { 3579, 11175 },
  { 36000000, 1 },
  { 3792, 3164 },
  { 3579, 3352 },
  { 3792, 3164 },
  { 4256, 9397 },
  { 36000000, 1 },
  { 2840, 4224 },
  { 3188, 3763 },
  { 2840, 4224 },
  { 2530, 15804 },
  { 36000000, 1 },
  { 4777, 8372 },
  { 36000000, 1 },
};

const music CounterMelody[MAX_COUNTERMELODY_INDEX] = {
  { 40000000, 1 },
  { 2840, 7040 },
  { 18000000, 1 },
  { 2530, 7902 },
  { 18000000, 1 },
  { 3188, 18816 },
  { 54000000, 1 },
  { 2530, 7902 },
  { 18000000, 1 },
  { 2840, 28160 },
  { 72000000, 1 },
  { 40000000, 1 },
  { 2840, 4688 },
  { 2530, 5262 },
  { 3188, 4177 },
  { 11988000, 1 },
  { 2840, 7040 },
  { 18000000, 1 },
  { 2530, 7902 },
  { 18000000, 1 },
  { 4777, 8372 },
  { 36000000, 1 },
  { 2530, 15804 },
  { 36000000, 1 },
  { 2840, 14080 },
  { 36000000, 1 },
  { 40000000, 1 },
  { 2840, 7040 },
  { 18000000, 1 },
  { 2530, 7902 },
  { 18000000, 1 },
  { 3188, 18816 },
  { 54000000, 1 },
  { 2530, 7902 },
  { 18000000, 1 },
  { 2840, 28160 },
  { 72000000, 1 },
  { 2840, 7040 },
  { 18000000, 1 },
  { 2530, 7902 },
  { 18000000, 1 },
  { 3188, 6272 },
  { 18000000, 1 },
  { 2840, 7040 },
  { 18000000, 1 },
  { 2530, 7902 },
  { 18000000, 1 },
  { 3188, 6272 },
  { 18000000, 1 },
  { 4777, 8372 },
  { 36000000, 1 },
  { 2530, 15804 },
  { 36000000, 1 },
  { 2840, 14080 },
  { 36000000, 1 }
};


void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge Timer0A_Handler timeout
  if(Allegro[AllegroIndex].numberOfSteps > Timer0A_Count){ // not ready for next note, but increment step
    Allegro_InstrumentIndex++;
    if(Allegro_InstrumentIndex > MAX_INSTRUMENT_INDEX){
      Allegro_InstrumentIndex = 0;
    }
  }
  else { // ready for next note
    AllegroIndex++;
    if(AllegroIndex > MAX_ALLEGRO_INDEX){
      AllegroIndex = 0;
    }
    SetTimer0A(Allegro[AllegroIndex].cyclesPerStep);
  }
  Timer0A_Count++;
}

void Timer0B_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TBTOCINT;// acknowledge Timer0B_Handler timeout
  if(Stoccato3[Stoccato3Index].numberOfSteps > Timer0B_Count){ // not ready for next note, but increment step
    Stoccato3_InstrumentIndex++;
    if(Stoccato3_InstrumentIndex > MAX_INSTRUMENT_INDEX){
      Stoccato3_InstrumentIndex = 0;
    }
  }
  else { // ready for next note
    Stoccato3Index++;
    if(Stoccato3Index > MAX_STOCCATO3_INDEX){
      Stoccato3Index = 0;
    }
    SetTimer0B(Stoccato3[Stoccato3Index].cyclesPerStep);
  }
  Timer0B_Count++;
}

void Timer1A_Handler(void){
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge Timer1A_Handler timeout
  if(Stoccato2[Stoccato2Index].numberOfSteps > Timer1A_Count){ // not ready for next note, but increment step
    Stoccato2_InstrumentIndex++;
    if(Stoccato2_InstrumentIndex > MAX_INSTRUMENT_INDEX){
      Stoccato2_InstrumentIndex = 0;
    }
  }
  else { // ready for next note
    Stoccato2Index++;
    if(Stoccato2Index > MAX_STOCCATO2_INDEX){
      Stoccato2Index = 0;
    }
    SetTimer1A(Stoccato2[Stoccato2Index].cyclesPerStep);
  }
  Timer1A_Count++;
}

void Timer1B_Handler(void){
  TIMER1_ICR_R = TIMER_ICR_TBTOCINT;// acknowledge Timer1B_Handler timeout
  if(Stoccato1[Stoccato1Index].numberOfSteps > Timer1B_Count){ // not ready for next note, but increment step
    Stoccato1_InstrumentIndex++;
    if(Stoccato1_InstrumentIndex > MAX_INSTRUMENT_INDEX){
      Stoccato1_InstrumentIndex = 0;
    }
  }
  else { // ready for next note
    Stoccato1Index++;
    if(Stoccato1Index > MAX_STOCCATO1_INDEX){
      Stoccato1Index = 0;
    }
    SetTimer1B(Stoccato1[Stoccato1Index].cyclesPerStep);
  }
  Timer1B_Count++;
}

void Timer2A_Handler(void){
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge Timer2A_Handler timeout
  if(Melody[MelodyIndex].numberOfSteps > Timer2A_Count){ // not ready for next note, but increment step
    Melody_InstrumentIndex++;
    if(Melody_InstrumentIndex > MAX_INSTRUMENT_INDEX){
      Melody_InstrumentIndex = 0;
    }
  }
  else { // ready for next note
    MelodyIndex++;
    if(MelodyIndex > MAX_MELODY_INDEX){
      MelodyIndex = 0;
    }
    SetTimer2A(Melody[MelodyIndex].cyclesPerStep);
  }
  Timer2A_Count++;
}

void Timer2B_Handler(void){
  TIMER2_ICR_R = TIMER_ICR_TBTOCINT;// acknowledge Timer2B_Handler timeout
  if(CounterMelody[CounterMelodyIndex].numberOfSteps > Timer2B_Count){ // not ready for next note, but increment step
    CounterMelody_InstrumentIndex++;
    if(CounterMelody_InstrumentIndex > MAX_INSTRUMENT_INDEX){
      CounterMelody_InstrumentIndex = 0;
    }
  }
  else { // ready for next note
    CounterMelodyIndex++;
    if(CounterMelodyIndex > MAX_COUNTERMELODY_INDEX){
      CounterMelodyIndex = 0;
    }
    SetTimer2B(CounterMelody[CounterMelodyIndex].cyclesPerStep);
  }
  Timer2B_Count++;
}

void Timers_init(void){long sr;
  sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x07;   // 0) activate TIMER0-2
  
  TIMER0_CTL_R = 0x00000000;    // 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER0_TAPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<19;           // 9) enable IRQ 19 in NVIC
  TIMER0_CTL_R = 0x00000001;    // 10) enable TIMER0A
  
  TIMER0_CTL_R = 0x00000000;    // 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TBMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER0_TBPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFFFF00)|0x00000080; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<20;           // 9) enable IRQ 19 in NVIC
  TIMER0_CTL_R = 0x00000001;    // 10) enable TIMER0A
  
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00008000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 37, interrupt number 21
  NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC
  TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A  
  
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFF00FFFF)|0x00800000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 37, interrupt number 21
  NVIC_EN0_R = 1<<22;           // 9) enable IRQ 21 in NVIC
  TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A  
  
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
  
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI6_R = (NVIC_PRI6_R&0xFFFFFF00)|0x00000080; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<24;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
  
  EndCritical(sr);
}

void static SetTimer0A(uint32_t period){
  TIMER0_TAILR_R = period-1;    // 4) reload value
}

void static SetTimer0B(uint32_t period){
  TIMER0_TBILR_R = period-1;    // 4) reload value
}

void static SetTimer1A(uint32_t period){
  TIMER1_TAILR_R = period-1;    // 4) reload value
}

void static SetTimer1B(uint32_t period){
  TIMER1_TBILR_R = period-1;    // 4) reload value
}

void static SetTimer2A(uint32_t period){
  TIMER2_TAILR_R = period-1;    // 4) reload value
}

void static SetTimer2B(uint32_t period){
  TIMER2_TBILR_R = period-1;    // 4) reload value
}
