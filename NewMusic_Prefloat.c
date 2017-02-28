// NewMusic.c
// Andrew Lynch

#include <stdint.h>
#include "..//inc//tm4c123gh6pm.h"
#include "NewMusic.h"

#define NORMAL_NOTE_LENGTH 9 // 0.9
#define STACCATO_NOTE_LENGTH 8 // 0.8

uint32_t static clk_speed = 80000000;
uint32_t static const ftempo = DESIRED_TEMPO / 60;
int previousProperty;

int static freqFinder(int src);
int static durationFinder(int src);
void static MakeMusic(const struct RawMusic src, struct MyMusic* dst, int* index_ptr);

void TestTask(const struct RawMusic part[], int len, struct MyMusic dst[]){
  return;
}


void Part_Init(const struct RawMusic part[], int len, struct MyMusic dst[]){  
  int part_index = 0;
  int music_index = 0;      
  while(part_index<len){       
    if(music_index > MAX_MUSIC_LENGTH){  
      break;   
    }          
    MakeMusic(part[part_index], dst, &music_index); 
    part_index++; music_index++;          
  }                                       
}                                         

void static MakeMusic(const struct RawMusic src, struct MyMusic* dst, int* index_ptr){
  int value = src.value;
  int enum_duration = src.duration;
  int property = src.property;
  int fnote = freqFinder(value);
  int duration = durationFinder(enum_duration);
  int actualDuration;
  if(value == Rest){
    dst[*index_ptr].cyclesPerStep = ( (duration * clk_speed) / ftempo ) / 100;
    dst[*index_ptr].numberOfSteps = 1;
    return;
  }
  else{
    fnote = freqFinder(value);
    dst[*index_ptr].cyclesPerStep = clk_speed / (SOUND_WAVE_RES * fnote);
  }
  switch(property){
    case None:
      actualDuration = (duration * NORMAL_NOTE_LENGTH)/10;
      dst[*index_ptr].numberOfSteps = ( fnote * actualDuration * SOUND_WAVE_RES / ftempo ) / 100;
      (*index_ptr)++;
      dst[*index_ptr].cyclesPerStep = ( (duration-actualDuration) * clk_speed / ftempo ) / 100;
      dst[*index_ptr].numberOfSteps = 1;
      switch(previousProperty){
        case None:
        case Staccato:
          dst[*index_ptr].Envelope = FullEnvelope;
          break;
        case Slurred:
          dst[*index_ptr].Envelope = BackEnvelope;
          break;
      }
      break;
    case Staccato:
      actualDuration = (duration * STACCATO_NOTE_LENGTH)/10;
      dst[*index_ptr].numberOfSteps = ( fnote * actualDuration * SOUND_WAVE_RES / ftempo ) / 100;
      (*index_ptr)++;
      dst[*index_ptr].cyclesPerStep = ( (duration-actualDuration) * clk_speed / ftempo ) / 100;
      dst[*index_ptr].numberOfSteps = 1;
      switch(previousProperty){
        case None:
        case Staccato:
          dst[*index_ptr].Envelope = FullEnvelope;
          break;
        case Slurred:
          dst[*index_ptr].Envelope = BackEnvelope;
          break;
      }
      break;
    case Slurred:
      actualDuration = duration;
      dst[*index_ptr].numberOfSteps = ( fnote * actualDuration * SOUND_WAVE_RES / ftempo ) / 100;
      switch(previousProperty){
        case None:
        case Staccato:
          dst[*index_ptr].Envelope = FrontEnvelope;
          break;
        case Slurred:
          dst[*index_ptr].Envelope = NoEnvelope;
          break;
      }
      break;
  }
  previousProperty = property;
}

int static freqFinder(int src){
  switch(src){
    case Rest:
      return 0;
    case C_0:
      return 0;
    case CSharp_0:
    case DFlat_0:
      return 0;
    case D_0:
      return 0;
    case DSharp_0:
    case EFlat_0:
      return 0;
    case E_0:
      return 0;
    case F_0:
      return 0;
    case FSharp_0:
    case GFlat_0:
      return 0;
    case G_0:
      return 0;
    case GSharp_0:
    case AFlat_0:
      return 0;
    case A_0:
      return 0;
    case ASharp_0:
    case BFlat_0:
      return 0;
    case B_0:
      return 0;
    case C_1:
      return 0;
    case CSharp_1:
    case DFlat_1:
      return 0;
    case D_1:
      return 0;
    case DSharp_1:
    case EFlat_1:
      return 0;
    case E_1:
      return 0;
    case F_1:
      return 0;
    case FSharp_1:
    case GFlat_1:
      return 0;
    case G_1:
      return 0;
    case GSharp_1:
    case AFlat_1:
      return 0;
    case A_1:
      return 0;
    case ASharp_1:
    case BFlat_1:
      return 0;
    case B_1:
      return 0;
    case C_2:
      return 0;
    case CSharp_2:
    case DFlat_2:
      return 0;
    case D_2:
      return 0;
    case DSharp_2:
    case EFlat_2:
      return 0;
    case E_2:
      return 0;
    case F_2:
      return 0;
    case FSharp_2:
    case GFlat_2:
      return 0;
    case G_2:
      return 0;
    case GSharp_2:
    case AFlat_2:
      return 0;
    case A_2:
      return 0;
    case ASharp_2:
    case BFlat_2:
      return 0;
    case B_2:
      return 0;
    case C_3:
      return 131;
    case CSharp_3:
    case DFlat_3:
      return 139;
    case D_3:
      return 147;
    case DSharp_3:
    case EFlat_3:
      return 156;
    case E_3:
      return 166;
    case F_3:
      return 175;
    case FSharp_3:
    case GFlat_3:
      return 185;
    case G_3:
      return 196;
    case GSharp_3:
    case AFlat_3:
      return 208;
    case A_3:
      return 220;
    case ASharp_3:
    case BFlat_3:
      return 233;
    case B_3:
      return 247;
    case C_4:
      return 262;
    case CSharp_4:
    case DFlat_4:
      return 277;
    case D_4:
      return 294;
    case DSharp_4:
    case EFlat_4:
      return 311;
    case E_4:
      return 330;
    case F_4:
      return 349;
    case FSharp_4:
    case GFlat_4:
      return 370;
    case G_4:
      return 392;
    case GSharp_4:
    case AFlat_4:
      return 415;
    case A_4:
      return 440;
    case ASharp_4:
    case BFlat_4:
      return 466;
    case B_4:
      return 494;
    case C_5:
      return 523;
    case CSharp_5:
    case DFlat_5:
      return 554;
    case D_5:
      return 587;
    case DSharp_5:
    case EFlat_5:
      return 622;
    case E_5:
      return 659;
    case F_5:
      return 698;
    case FSharp_5:
    case GFlat_5:
      return 740;
    case G_5:
      return 784;
    case GSharp_5:
    case AFlat_5:
      return 831;
    case A_5:
      return 880;
    case ASharp_5:
    case BFlat_5:
      return 932;
    case B_5:
      return 988;
    case C_6:
      return 0;
    case CSharp_6:
    case DFlat_6:
      return 0;
    case D_6:
      return 0;
    case DSharp_6:
    case EFlat_6:
      return 0;
    case E_6:
      return 0;
    case F_6:
      return 0;
    case FSharp_6:
    case GFlat_6:
      return 0;
    case G_6:
      return 0;
    case GSharp_6:
    case AFlat_6:
      return 0;
    case A_6:
      return 0;
    case ASharp_6:
    case BFlat_6:
      return 0;
    case B_6:
      return 0;
    case C_7:
      return 0;
    case CSharp_7:
    case DFlat_7:
      return 0;
    case D_7:
      return 0;
    case DSharp_7:
    case EFlat_7:
      return 0;
    case E_7:
      return 0;
    case F_7:
      return 0;
    case FSharp_7:
    case GFlat_7:
      return 0;
    case G_7:
      return 0;
    case GSharp_7:
    case AFlat_7:
      return 0;
    case A_7:
      return 0;
    case ASharp_7:
    case BFlat_7:
      return 0;
    case B_7:
      return 0;
    case C_8:
      return 0;
    case CSharp_8:
    case DFlat_8:
      return 0;
    case D_8:
      return 0;
    case DSharp_8:
    case EFlat_8:
      return 0;
    case E_8:
      return 0;
    case F_8:
      return 0;
    case FSharp_8:
    case GFlat_8:
      return 0;
    case G_8:
      return 0;
    case GSharp_8:
    case AFlat_8:
      return 0;
    case A_8:
      return 0;
    case ASharp_8:
    case BFlat_8:
      return 0;
    case B_8:
      return 0;
  }
  return -1;
}

int static durationFinder(int src){
  switch(src){
    case Quarter:
      return 100;
    case Half:
      return 200;
    case Whole:  
      return 400;
    case Eigth:
      return 50;
    case Triplet:
      return 33;
    case Sixteenth:
      return 25;
  }
  return -1;
}
