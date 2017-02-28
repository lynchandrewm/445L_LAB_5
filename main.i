#line 1 "main.c"




















 






#line 1 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdint.h"
 
 





 










#line 26 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdint.h"







 

     

     
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef   signed       __int64 int64_t;

     
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned       __int64 uint64_t;

     

     
     
typedef   signed          char int_least8_t;
typedef   signed short     int int_least16_t;
typedef   signed           int int_least32_t;
typedef   signed       __int64 int_least64_t;

     
typedef unsigned          char uint_least8_t;
typedef unsigned short     int uint_least16_t;
typedef unsigned           int uint_least32_t;
typedef unsigned       __int64 uint_least64_t;

     

     
typedef   signed           int int_fast8_t;
typedef   signed           int int_fast16_t;
typedef   signed           int int_fast32_t;
typedef   signed       __int64 int_fast64_t;

     
typedef unsigned           int uint_fast8_t;
typedef unsigned           int uint_fast16_t;
typedef unsigned           int uint_fast32_t;
typedef unsigned       __int64 uint_fast64_t;

     
typedef   signed           int intptr_t;
typedef unsigned           int uintptr_t;

     
typedef   signed       __int64 intmax_t;
typedef unsigned       __int64 uintmax_t;




     

     





     





     





     

     





     





     





     

     





     





     





     

     


     


     


     

     


     


     


     

     



     



     


     
    
 



#line 197 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdint.h"

     







     










     











#line 261 "C:\\Keil\\ARM\\ARMCC\\bin\\..\\include\\stdint.h"



 



#line 29 "main.c"
#line 1 "DAC.h"




















 











void DAC_Init(uint16_t data);





void DAC_Out(uint16_t code);
#line 30 "main.c"
#line 1 "SysTick.h"






























 


void SysTick_Init(void);



void SysTick_Wait(uint32_t delay);



void SysTick_Wait10ms(uint32_t delay);
#line 31 "main.c"
#line 1 "PLL.h"





















 







void PLL_Init(uint32_t freq);
#line 163 "PLL.h"




#line 32 "main.c"
#line 1 "NewMusic.h"










struct MyMusic {
  int cyclesPerStep;
  int numberOfSteps;
};

struct RawMusic {
  int value;
  int duration;
  int property;
};

enum{ 
  Rest,
  C_0,
  CSharp_0,
  DFlat_0,
  D_0,
  DSharp_0,
  EFlat_0,
  E_0,
  F_0,
  FSharp_0,
  GFlat_0,
  G_0,
  GSharp_0,
  AFlat_0,
  A_0,
  ASharp_0,
  BFlat_0,
  B_0,
  C_1,
  CSharp_1,
  DFlat_1,
  D_1,
  DSharp_1,
  EFlat_1,
  E_1,
  F_1,
  FSharp_1,
  GFlat_1,
  G_1,
  GSharp_1,
  AFlat_1,
  A_1,
  ASharp_1,
  BFlat_1,
  B_1,
  C_2,
  CSharp_2,
  DFlat_2,
  D_2,
  DSharp_2,
  EFlat_2,
  E_2,
  F_2,
  FSharp_2,
  GFlat_2,
  G_2,
  GSharp_2,
  AFlat_2,
  A_2,
  ASharp_2,
  BFlat_2,
  B_2,
  C_3,
  CSharp_3,
  DFlat_3,
  D_3,
  DSharp_3,
  EFlat_3,
  E_3,
  F_3,
  FSharp_3,
  GFlat_3,
  G_3,
  GSharp_3,
  AFlat_3,
  A_3,
  ASharp_3,
  BFlat_3,
  B_3,
  C_4,
  CSharp_4,
  DFlat_4,
  D_4,
  DSharp_4,
  EFlat_4,
  E_4,
  F_4,
  FSharp_4,
  GFlat_4,
  G_4,
  GSharp_4,
  AFlat_4,
  A_4,
  ASharp_4,
  BFlat_4,
  B_4,
  C_5,
  CSharp_5,
  DFlat_5,
  D_5,
  DSharp_5,
  EFlat_5,
  E_5,
  F_5,
  FSharp_5,
  GFlat_5,
  G_5,
  GSharp_5,
  AFlat_5,
  A_5,
  ASharp_5,
  BFlat_5,
  B_5,
  C_6,
  CSharp_6,
  DFlat_6,
  D_6,
  DSharp_6,
  EFlat_6,
  E_6,
  F_6,
  FSharp_6,
  GFlat_6,
  G_6,
  GSharp_6,
  AFlat_6,
  A_6,
  ASharp_6,
  BFlat_6,
  B_6,
  C_7,
  CSharp_7,
  DFlat_7,
  D_7,
  DSharp_7,
  EFlat_7,
  E_7,
  F_7,
  FSharp_7,
  GFlat_7,
  G_7,
  GSharp_7,
  AFlat_7,
  A_7,
  ASharp_7,
  BFlat_7,
  B_7,
  C_8,
  CSharp_8,
  DFlat_8,
  D_8,
  DSharp_8,
  EFlat_8,
  E_8,
  F_8,
  FSharp_8,
  GFlat_8,
  G_8,
  GSharp_8,
  AFlat_8,
  A_8,
  ASharp_8,
  BFlat_8,
  B_8
};

enum{ 
  Quarter,
  Half,
  Whole,
  Eigth,
  Triplet
};
enum{ 
  None,
  Staccato,
  Slurred
};






void Music_Init(void);
void Part_Init(const struct RawMusic part[], int len, struct MyMusic dst[]);
void TestTask(const struct RawMusic part[], int len, struct MyMusic dst[]);
#line 33 "main.c"
#line 1 "Player.h"



void Player_Init(void);

void Player_Go(void);
#line 34 "main.c"

int MelodyInstrument;

struct MyMusic tester[5];








const uint16_t horn1[64] = {
  1024 * 2, 1024 * 2, 1014 * 2, 1008 * 2, 1022 * 2, 1065 * 2, 1093 * 2, 1006 * 2, 858 * 2, 711 * 2, 612 * 2, 596 * 2, 672 * 2, 806 * 2, 952 * 2, 1074 * 2,
  1154 * 2, 1191 * 2, 1202 * 2, 1216 * 2, 1236 * 2, 1255 * 2, 1272 * 2, 1302 * 2, 1318 * 2, 1299 * 2, 1238 * 2, 1140 * 2, 1022 * 2, 910 * 2, 827 * 2,
  779 * 2, 758 * 2, 757 * 2, 782 * 2, 856 * 2, 972 * 2, 1088 * 2, 1177 * 2, 1226 * 2, 1232 * 2, 1203 * 2, 1157 * 2, 1110 * 2, 1067 * 2, 1028 * 2, 993 * 2,
  958 * 2, 929 * 2, 905 * 2, 892 * 2, 900 * 2, 940 * 2, 1022 * 2, 1125 * 2, 1157 * 2, 1087 * 2, 965 * 2, 836 * 2, 783 * 2, 816 * 2, 895 * 2, 971 * 2, 1017 * 2
};

struct MyMusic test[5];
void testing(void){
  test[5].cyclesPerStep = 0;
}

int main(void){struct MyMusic Melody[10];
  PLL_Init(24);
  
  Player_Init();
  testing();
  uint32_t i=0;
  DAC_Init(4095);                  
  SysTick_Init();
  while(1){
    DAC_Out(horn1[i]);
    i = (i + 1)&0x3F;
    
    
    
    






    SysTick_Wait(1136);              

  }
}

