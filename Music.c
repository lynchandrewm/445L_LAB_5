// Music.c
// Andrew Lynch

#include <stdint.h>
#include "..//inc//tm4c123gh6pm.h"
#include "Music.h"

#define NORMAL_NOTE_LENGTH 9 // 0.9
#define STACCATO_NOTE_LENGTH 5 // 0.5

int static freqFinder(int src);
int static durationFinder(int src);
void static MakeMusic(const struct RawMusic src, struct Music* dst, int* index_ptr);

uint32_t static clk_speed;
uint32_t static const ftempo = DESIRED_TEMPO / 60;

void Part_Init(const struct RawMusic part[], int len, struct Music dst[]){  \
  int freq, num, part_index, music_index;      \
  while(part_index<len){       \
    if(music_index > MAX_MUSIC_LENGTH){  \
      break;   \
    }          \
    MakeMusic(part[part_index], dst, &music_index); \
    part_index++; music_index++;          \
  }                                       \
}                                         \

void static MakeMusic(const struct RawMusic src, struct Music* dst, int* index_ptr){
  int value = src.value;
  int enum_duration = src.duration;
  int property = src.property;
  int fnote = freqFinder(value);
  int duration = durationFinder(enum_duration);
  int actualDuration;
  if(value == Rest){
    dst[*index_ptr].cyclesPerStep = duration * clk_speed / ftempo;
    dst[*index_ptr].numberOfSteps = 1;
    return;
  }
  else{
    fnote = freqFinder(value);
    dst[*index_ptr].cyclesPerStep = clk_speed / (64 * fnote);
  }
  switch(property){
    case None:
      actualDuration = (duration * NORMAL_NOTE_LENGTH)/10;
      dst[*index_ptr].numberOfSteps = fnote * actualDuration * 64 / ftempo;
      (*index_ptr)++;
      dst[*index_ptr].cyclesPerStep = (duration-actualDuration) * clk_speed / ftempo;
      dst[*index_ptr].numberOfSteps = 1;
      break;
    case Staccato:
      actualDuration = (duration * STACCATO_NOTE_LENGTH)/10;
      dst[*index_ptr].numberOfSteps = fnote * actualDuration * 64 / ftempo;
      (*index_ptr)++;
      dst[*index_ptr].cyclesPerStep = (duration-actualDuration) * clk_speed / ftempo;
      dst[*index_ptr].numberOfSteps = 1;
      break;
    case Slurred:
      actualDuration = duration;
      dst[*index_ptr].numberOfSteps = fnote * actualDuration * 64 / ftempo;
      break;
  }
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
  }
  return -1;
}

const uint16_t flute[64] = {
  1007 * 2, 1252 * 2, 1374 * 2, 1548 * 2, 1698 * 2, 1797 * 2, 1825 * 2, 1797 * 2, 1675 * 2, 1562 * 2, 1383 * 2, 1219 * 2, 1092 * 2, 
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



