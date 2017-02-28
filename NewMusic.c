// NewMusic.c
// Andrew Lynch

#include <stdint.h>
#include "..//inc//tm4c123gh6pm.h"
#include "NewMusic.h"

#define NORMAL_NOTE_LENGTH 0.95 // 0.9
#define STACCATO_NOTE_LENGTH 0.8 // 0.8

float static clk_speed = 80000000;
float static const ftempo = DESIRED_TEMPO / 60;
int previousProperty;
int SoundWaveResolution;

float static freqFinder(int src);
float static durationFinder(int src);
void static MakeMusic(const struct RawMusic src, struct MyMusic* dst, int* index_ptr);

void Part_Init(const struct RawMusic part[], int len, struct MyMusic dst[], int dst_len, int resolution){  
  int part_index = 0;
  int music_index = 0;
  SoundWaveResolution = resolution;                          // (0) Save local resolution
  while(part_index<len){                                     // (1) Index through "Pretty" music
    if(music_index > MAX_MUSIC_LENGTH){                      // (2) Check to make sure the music is too long
      break;   
    }          
    MakeMusic(part[part_index], dst, &music_index);          // (3) Convert "Pretty" to playable
    part_index++; music_index++;                             // (4) On to the next one
  }
  
  for(int i = music_index; i < dst_len; i++){                // (5) Make any left over space look invalid
    dst[i].cyclesPerStep = 0;
    dst[i].numberOfSteps = 1;    
  }
}                                         

void static MakeMusic(const struct RawMusic src, struct MyMusic* dst, int* index_ptr){
  int value = src.value;                                                                           // (0) Get local values from src
  int enum_duration = src.duration;
  int property = src.property;
  float fnote = freqFinder(value);                                                                 // (1) Get freqency of note
  float duration = durationFinder(enum_duration);                                                  // (2) Get duration of note
  float actualDuration;
  int ClksForDuration, ClksForNote;
  if(value == Rest){                                                                               // (3) Handle case for rest
    dst[*index_ptr].cyclesPerStep = (duration * clk_speed) / ftempo;
    dst[*index_ptr].numberOfSteps = 1;
    return;
  }
  else{                                                                                            // (4) Otherwise calculate timer periods
    dst[*index_ptr].cyclesPerStep = clk_speed / (SoundWaveResolution * fnote);
  }
  switch(property){                                                                                
    case None:                                                                                     // (5.0) For normal note, a gap is necessary between each note
      actualDuration = duration * NORMAL_NOTE_LENGTH;                                              //     which is essentially just a rest for 1/20 of the note duration
      dst[*index_ptr].numberOfSteps = fnote * actualDuration * SoundWaveResolution / ftempo;       //     but to keep consistent timing, we need to reduce the note's duration
      ClksForDuration = (duration * clk_speed) / ftempo;                                           //     19/20 of the full duration.
      ClksForNote = dst[*index_ptr].numberOfSteps * dst[*index_ptr].cyclesPerStep;
      (*index_ptr)++;
      dst[*index_ptr].cyclesPerStep = ClksForDuration - ClksForNote;
      dst[*index_ptr].numberOfSteps = 1;
      switch(previousProperty){                                                                    // (6.0) A normal note will tapper...
        case None:
        case Staccato:
          dst[*index_ptr].Envelope = FullEnvelope;                                                 // (6.1) Under most conditions it also starts quiet then loudens
          break;
        case Slurred:
          dst[*index_ptr].Envelope = BackEnvelope;                                                 // (6.2) but if preceeded by a slurred note, the note starts full
          break;
      }
      break;
    case Staccato:                                                                                 // (5.1) Similar to a normal note, staccato notes are just shorter
      actualDuration = duration * STACCATO_NOTE_LENGTH;
      dst[*index_ptr].numberOfSteps = fnote * actualDuration * SoundWaveResolution / ftempo;
      ClksForDuration = (duration * clk_speed) / ftempo;
      ClksForNote = dst[*index_ptr].numberOfSteps * dst[*index_ptr].cyclesPerStep;
      (*index_ptr)++;
      dst[*index_ptr].cyclesPerStep = ClksForDuration - ClksForNote;
      dst[*index_ptr].numberOfSteps = 1;
      switch(previousProperty){                                                                    // (6.0) This is exactly like a normal note
        case None:
        case Staccato:
          dst[*index_ptr].Envelope = FullEnvelope;
          break;
        case Slurred:
          dst[*index_ptr].Envelope = BackEnvelope;
          break;
      }
      break;
    case Slurred:                                                                                  // (5.2) Slurred notes connect to the following note,
      actualDuration = duration;                                                                   //       so no rest is necessary
      dst[*index_ptr].numberOfSteps = fnote * actualDuration * SoundWaveResolution / ftempo;
      switch(previousProperty){                                                                    // (6.3) Slurred notes DO NOT tapper. It's actually a pain in the ass...
        case None:
        case Staccato:
          dst[*index_ptr].Envelope = FrontEnvelope;                                                // (6.4) See they need two seperate envelopes for the first slurred,
          break;                                                                                   //       which still builds like a normal note
        case Slurred:
          dst[*index_ptr].Envelope = NoEnvelope;                                                   // (6.5) But when proceeded by a slurred note, a slurred note has no envelope
          break;
      }
      break;
  }
  previousProperty = property;                                                                     // (6.6) To deal with this envelope madness, we need to save the last note's property
}

// Looks up frequencies given the enum note name. It's big. 
float static freqFinder(int src){
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
      return 65.41;
    case CSharp_2:
    case DFlat_2:
      return 69.30;
    case D_2:
      return 73.42;
    case DSharp_2:
    case EFlat_2:
      return 77.78;
    case E_2:
      return 82.41;
    case F_2:
      return 87.31;
    case FSharp_2:
    case GFlat_2:
      return 92.50;
    case G_2:
      return 98.00;
    case GSharp_2:
    case AFlat_2:
      return 103.83;
    case A_2:
      return 110.00;
    case ASharp_2:
    case BFlat_2:
      return 116.54;
    case B_2:
      return 123.47;
    case C_3:
      return 130.81;
    case CSharp_3:
    case DFlat_3:
      return 138.59;
    case D_3:
      return 146.83;
    case DSharp_3:
    case EFlat_3:
      return 155.56;
    case E_3:
      return 164.81;
    case F_3:
      return 174.61;
    case FSharp_3:
    case GFlat_3:
      return 185.00;
    case G_3:
      return 196.00;
    case GSharp_3:
    case AFlat_3:
      return 207.65;
    case A_3:
      return 220.00;
    case ASharp_3:
    case BFlat_3:
      return 233.08;
    case B_3:
      return 246.94;
    case C_4:
      return 261.63;
    case CSharp_4:
    case DFlat_4:
      return 277.18;
    case D_4:
      return 293.66;
    case DSharp_4:
    case EFlat_4:
      return 311.13;
    case E_4:
      return 329.63;
    case F_4:
      return 349.23;
    case FSharp_4:
    case GFlat_4:
      return 369.99;
    case G_4:
      return 392.00;
    case GSharp_4:
    case AFlat_4:
      return 415.30;
    case A_4:
      return 440.00;
    case ASharp_4:
    case BFlat_4:
      return 466.16;
    case B_4:
      return 493.88;
    case C_5:
      return 523.25;
    case CSharp_5:
    case DFlat_5:
      return 554.37;
    case D_5:
      return 587.33;
    case DSharp_5:
    case EFlat_5:
      return 622.25;
    case E_5:
      return 659.25;
    case F_5:
      return 698.46;
    case FSharp_5:
    case GFlat_5:
      return 739.99;
    case G_5:
      return 783.99;
    case GSharp_5:
    case AFlat_5:
      return 830.61;
    case A_5:
      return 880.00;
    case ASharp_5:
    case BFlat_5:
      return 932.33;
    case B_5:
      return 987.77;
    case C_6:
      return 1046.50;
    case CSharp_6:
    case DFlat_6:
      return 1108.73;
    case D_6:
      return 1174.66;
    case DSharp_6:
    case EFlat_6:
      return 1244.51;
    case E_6:
      return 1318.51;
    case F_6:
      return 1396.91;
    case FSharp_6:
    case GFlat_6:
      return 1479.98;
    case G_6:
      return 1567.98;
    case GSharp_6:
    case AFlat_6:
      return 1661.22;
    case A_6:
      return 1760.00;
    case ASharp_6:
    case BFlat_6:
      return 1864.66;
    case B_6:
      return 1975.53;
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

// Looks up durations given the enum note name. It's not big. 
float static durationFinder(int src){
  switch(src){
    case Quarter:
      return 1;
    case Half:
      return 2;
    case Whole:  
      return 4;
    case Eigth:
      return 0.5;
    case Triplet:
      return 0.3333;               // This will eventually lead to dissonance between parts. 
    case Sixteenth:
      return 0.25;
  }
  return -1;
}
