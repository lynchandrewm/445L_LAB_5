// NewMusic.h
// Andrew Lynch

#ifndef __MUSIC__
#define __MUSIC__

#define DESIRED_TEMPO 120
#define MAX_SECONDS_PER_PART 60
#define MAX_MUSIC_LENGTH 7200 // 120 * 60

struct MyMusic { //Playable
  int cyclesPerStep;
  int numberOfSteps;
  int Envelope;
};
struct RawMusic { //Readable
  int value;
  int duration;
  int property;
};

enum{ //Note Values
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
enum{ //Note Durations
  Quarter,
  Half,
  Whole,
  Eigth,
  Triplet,
  Sixteenth
};
enum{ //Note Properties
  None,
  Staccato,
  Slurred
};
enum{ //Envelope Types
  FullEnvelope,
  FrontEnvelope,
  BackEnvelope,
  NoEnvelope
};



// Converts readable music into playable music. 
// This could be a critical section. So don't call in an ISR. 
// It takes time, so that'd be a dumb idea anyways. Just saying.
void Part_Init(const struct RawMusic part[], int len, struct MyMusic dst[], int dst_len, int resolution);

#endif //__MUSIC__