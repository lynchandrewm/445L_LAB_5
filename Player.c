// Player.c
// Andrew Lynch

#include <stdint.h>
#include "NewMusic.h"
#include "DAC.h"
#include "../inc/tm4c123gh6pm.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

#define ON 1
#define OFF 0
#define ENVELOPE_ON ON
#define NUMBER_OF_PARTS 1

#define DEBUG ON
#if DEBUG
#include "Debug.h"
#endif //DEBUG

enum{ //Instruments
  Flute,
  Horn,
  Trumpet,
  Bassoon,
  Oboe,
  Sine
};

// Music Parts
const int Bassoon_Part_Length = 125;
const struct RawMusic Bassoon_Part[Bassoon_Part_Length] = {
  {G_3, Quarter, None},
  {D_3, Quarter, None},
  {E_3, Quarter, None},
  {F_3, Quarter, None},
  {E_3, Quarter, None},
  {D_3, Quarter, None},
  {G_3, Quarter, None},
  {D_3, Quarter, None},
  {E_3, Quarter, None},
  {A_2, Quarter, None},
  {B_2, Quarter, None},
  {C_3, Quarter, None},
  {G_3, Quarter, None},
  {D_3, Quarter, None},
  {E_3, Quarter, None},
  {F_3, Eigth, None},
  {G_3, Eigth, None},
  {A_3, Quarter, None},
  {G_3, Triplet, Slurred},
  {A_3, Triplet, Slurred},
  {G_3, Triplet, Slurred},
  {F_3, Quarter, None},
  {E_3, Triplet, Slurred},
  {F_3, Triplet, Slurred},
  {E_3, Triplet, Slurred},
  {D_3, Quarter, None},
  {A_2, Triplet, Slurred},
  {G_2, Triplet, Slurred},
  {A_2, Triplet, Slurred},
  {B_2, Quarter, None},
  {C_3, Quarter, None},
  
  {G_3, Quarter, None},
  {Rest, Half, None},
  {F_3, Quarter, None},
  {Rest, Half, None},
  {B_2, Quarter, None},
  {Rest, Half, None},
  {A_2, Quarter, None},
  {Rest, Half, None},
  {D_3, Quarter, None},
  {Rest, Half, None},
  {C_3, Quarter, None},
  {Rest, Half, None},
  {G_3, Quarter, None},
  {Rest, Half, None},
  {F_3, Quarter, None},
  {Rest, Half, None},
  
  {G_3, Quarter, None},
  {Rest, Half, None},
  {F_3, Quarter, None},
  {Rest, Half, None},
  {B_2, Quarter, None},
  {Rest, Half, None},
  {A_2, Quarter, None},
  {Rest, Half, None},
  {D_3, Quarter, None},
  {Rest, Half, None},
  {C_3, Quarter, None},
  {Rest, Half, None},
  {G_3, Quarter, None},
  {Rest, Half, None},
  {F_3, Quarter, None},
  {Rest, Half, None},
  
  {G_3, Quarter, None},
  {D_3, Quarter, None},
  {E_3, Quarter, None},
  {F_3, Quarter, None},
  {E_3, Quarter, None},
  {D_3, Quarter, None},
  {G_3, Quarter, None},
  {D_3, Quarter, None},
  {E_3, Quarter, None},
  {A_2, Quarter, None},
  {B_2, Quarter, None},
  {C_3, Quarter, None},
  {G_3, Quarter, None},
  {D_3, Quarter, None},
  {E_3, Quarter, None},
  {F_3, Eigth, None},
  {G_3, Eigth, None},
  {A_3, Quarter, None},
  {G_3, Triplet, Slurred},
  {A_3, Triplet, Slurred},
  {G_3, Triplet, Slurred},
  {F_3, Quarter, None},
  {E_3, Triplet, Slurred},
  {F_3, Triplet, Slurred},
  {E_3, Triplet, Slurred},
  {D_3, Quarter, None},
  {A_2, Triplet, Slurred},
  {G_2, Triplet, Slurred},
  {A_2, Triplet, Slurred},
  {B_2, Quarter, None},
  {C_3, Quarter, None},

  {G_3, Quarter, None},
  {D_3, Quarter, None},
  {E_3, Quarter, None},
  {F_3, Quarter, None},
  {E_3, Quarter, None},
  {D_3, Quarter, None},
  {G_3, Quarter, None},
  {D_3, Quarter, None},
  {E_3, Quarter, None},
  {A_2, Quarter, None},
  {B_2, Quarter, None},
  {C_3, Quarter, None},
  {G_3, Quarter, None},
  {D_3, Quarter, None},
  {E_3, Quarter, None},
  {F_3, Eigth, None},
  {G_3, Eigth, None},
  {A_3, Quarter, None},
  {G_3, Triplet, Slurred},
  {A_3, Triplet, Slurred},
  {G_3, Triplet, Slurred},
  {F_3, Quarter, None},
  {E_3, Triplet, Slurred},
  {F_3, Triplet, Slurred},
  {E_3, Triplet, Slurred},
  {D_3, Quarter, None},
  {A_2, Triplet, Slurred},
  {G_2, Triplet, Slurred},
  {A_2, Triplet, Slurred},
  {B_2, Quarter, None},
  {C_3, Quarter, None}
};

const int Trumpet_Part_Length = 139;
const struct RawMusic Trumpet_Part[Trumpet_Part_Length] = {
  {Rest, Whole, None},
  {Rest, Whole, None},
  {Rest, Whole, None},
  {Rest, Whole, None},
  {Rest, Whole, None},
  {Rest, Whole, None},
  
  {G_4, Quarter, None},
  {D_4, Quarter, None},
  {E_4, Quarter, None},
  {F_4, Quarter, None},
  {E_4, Quarter, None},
  {D_4, Quarter, None},
  {G_4, Quarter, None},
  {D_4, Quarter, None},
  {E_4, Quarter, None},
  {A_3, Quarter, None},
  {B_3, Quarter, None},
  {C_4, Quarter, None},
  {G_4, Quarter, None},
  {D_4, Quarter, None},
  {E_4, Quarter, None},
  {F_4, Eigth, None},
  {G_4, Eigth, None},
  {A_4, Quarter, None},
  {G_4, Triplet, Slurred},
  {A_4, Triplet, Slurred},
  {G_4, Triplet, Slurred},
  {F_4, Quarter, None},
  {E_4, Triplet, Slurred},
  {F_4, Triplet, Slurred},
  {E_4, Triplet, Slurred},
  {D_4, Quarter, None},
  {A_3, Triplet, Slurred},
  {G_3, Triplet, Slurred},
  {A_3, Triplet, Slurred},
  {B_3, Quarter, None},
  {C_4, Quarter, None},
  
  {Rest, Quarter, None},
  {A_4, Eigth, None},
  {B_4, Eigth, None},
  {G_4, Quarter, Slurred},
  {G_4, Eigth, None},
  {B_4, Eigth, None},
  {A_4, Half, None},
  {Rest, Quarter, None},
  {A_4, Triplet, Slurred},
  {B_4, Triplet, Slurred},
  {G_4, Triplet, Slurred},
  {A_4, Eigth, Slurred},
  {A_4, Sixteenth, None},
  {B_4, Sixteenth, None},
  {C_5, Quarter, None},
  {B_4, Quarter, None},
  {A_4, Quarter, None},
  {Rest, Quarter, None},
  {A_4, Eigth, None},
  {B_4, Eigth, None},
  {G_4, Quarter, Slurred},
  {G_4, Eigth, None},
  {B_4, Eigth, None},
  {A_4, Half, None},
  {A_4, Eigth, None},
  {B_4, Eigth, None},
  {G_4, Eigth, None},
  {A_4, Eigth, None},
  {B_4, Eigth, Slurred},
  {B_4, Sixteenth, None},
  {G_4, Sixteenth, None},
  {C_5, Quarter, None},
  {B_4, Quarter, None},
  {A_4, Quarter, None},
  
  {Rest, Quarter, None},
  {A_4, Eigth, None},
  {B_4, Eigth, None},
  {G_4, Quarter, Slurred},
  {G_4, Eigth, None},
  {B_4, Eigth, None},
  {A_4, Half, None},
  {Rest, Quarter, None},
  {A_4, Triplet, Slurred},
  {B_4, Triplet, Slurred},
  {G_4, Triplet, Slurred},
  {A_4, Eigth, Slurred},
  {A_4, Sixteenth, None},
  {B_4, Sixteenth, None},
  {C_5, Quarter, None},
  {B_4, Quarter, None},
  {A_4, Quarter, None},
  {Rest, Quarter, None},
  {A_4, Eigth, None},
  {B_4, Eigth, None},
  {G_4, Quarter, Slurred},
  {G_4, Eigth, None},
  {B_4, Eigth, None},
  {A_4, Half, None},
  {A_4, Eigth, None},
  {B_4, Eigth, None},
  {G_4, Eigth, None},
  {A_4, Eigth, None},
  {B_4, Eigth, Slurred},
  {B_4, Sixteenth, None},
  {G_4, Sixteenth, None},
  {C_5, Quarter, None},
  {B_4, Quarter, None},
  {A_4, Quarter, None},

  {Rest, Quarter, None},
  {A_4, Eigth, None},
  {B_4, Eigth, None},
  {G_4, Quarter, Slurred},
  {G_4, Eigth, None},
  {B_4, Eigth, None},
  {A_4, Half, None},
  {Rest, Quarter, None},
  {A_4, Triplet, Slurred},
  {B_4, Triplet, Slurred},
  {G_4, Triplet, Slurred},
  {A_4, Eigth, Slurred},
  {A_4, Sixteenth, None},
  {B_4, Sixteenth, None},
  {C_5, Quarter, None},
  {B_4, Quarter, None},
  {A_4, Quarter, None},
  {Rest, Quarter, None},
  {A_4, Eigth, None},
  {B_4, Eigth, None},
  {G_4, Quarter, Slurred},
  {G_4, Eigth, None},
  {B_4, Eigth, None},
  {A_4, Half, None},
  {A_4, Eigth, None},
  {B_4, Eigth, None},
  {G_4, Eigth, None},
  {A_4, Eigth, None},
  {B_4, Eigth, Slurred},
  {B_4, Sixteenth, None},
  {G_4, Sixteenth, None},
  {C_5, Quarter, None},
  {B_4, Quarter, None},
  {A_4, Quarter, None}
};

const int Oboe_Part_Length = 106;
struct RawMusic Oboe_Part[Oboe_Part_Length] = {
  {Rest, Whole, None},
  {Rest, Whole, None},
  {Rest, Whole, None},
  {Rest, Whole, None},
  {Rest, Whole, None},
  {Rest, Whole, None},

  {Rest, Whole, None},
  {Rest, Whole, None},
  {Rest, Whole, None},
  {Rest, Whole, None},
  {Rest, Whole, None},
  {Rest, Whole, None},
  
  {G_5, Quarter, None},
  {D_5, Quarter, None},
  {E_5, Quarter, None},
  {F_5, Quarter, None},
  {E_5, Quarter, None},
  {D_5, Quarter, None},
  {G_5, Quarter, None},
  {D_5, Quarter, None},
  {E_5, Quarter, None},
  {A_4, Quarter, None},
  {B_4, Quarter, None},
  {C_5, Quarter, None},
  {G_5, Quarter, None},
  {D_5, Quarter, None},
  {E_5, Quarter, None},
  {F_5, Eigth, None},
  {G_5, Eigth, None},
  {A_5, Quarter, None},
  {G_5, Triplet, Slurred},
  {A_5, Triplet, Slurred},
  {G_5, Triplet, Slurred},
  {F_5, Quarter, None},
  {E_5, Triplet, Slurred},
  {F_5, Triplet, Slurred},
  {E_5, Triplet, Slurred},
  {D_5, Quarter, None},
  {A_4, Triplet, Slurred},
  {G_4, Triplet, Slurred},
  {A_4, Triplet, Slurred},
  {B_4, Quarter, None},
  {C_5, Quarter, None},

  {G_5, Quarter, None},
  {Rest, Half, None},
  {F_5, Quarter, None},
  {Rest, Half, None},
  {B_5, Quarter, None},
  {Rest, Half, None},
  {A_5, Quarter, None},
  {Rest, Half, None},
  {D_5, Quarter, None},
  {Rest, Half, None},
  {C_5, Quarter, None},
  {Rest, Half, None},
  {G_5, Quarter, None},
  {Rest, Half, None},
  {F_5, Quarter, None},
  {Rest, Half, None},
  
  {Rest,Quarter,None},
  {D_5, Eigth, None},
  {G_5, Eigth, None},
  {C_5, Eigth, None},
  {E_5, Eigth, None},
  {A_5, Eigth, None},
  {D_6, Eigth, None},
  {D_5, Eigth, None},
  {G_5, Eigth, None},
  {C_5, Eigth, None},
  {E_5, Eigth, None},
  {A_5, Eigth, None},
  {D_6, Eigth, None},
  {D_5, Eigth, None},
  {G_5, Eigth, None},
  {C_5, Eigth, None},
  {E_5, Eigth, None},
  {A_5, Eigth, None},
  {D_6, Eigth, None},
  {D_5, Eigth, None},
  {G_5, Eigth, None},
  {C_5, Eigth, None},
  {E_5, Eigth, None},
  {A_5, Eigth, None},
  {D_6, Eigth, None},
  {D_5, Eigth, None},
  {G_5, Eigth, None},
  {C_5, Eigth, None},
  {E_5, Eigth, None},
  {A_5, Eigth, None},
  {D_6, Eigth, None},
  {D_5, Eigth, None},
  {G_5, Eigth, None},
  {C_5, Eigth, None},
  {E_5, Eigth, None},
  {A_5, Eigth, None},
  {D_6, Eigth, None},
  {D_5, Eigth, None},
  {G_5, Eigth, None},
  {C_5, Eigth, None},
  {E_5, Eigth, None},
  {A_5, Eigth, None},
  {D_6, Eigth, None},
  {D_5, Eigth, None},
  {G_5, Eigth, None},
  {C_5, Eigth, None},
  {E_5, Eigth, None}
};




// Instruments
const uint16_t trumpetArray[168] = {	
988	,
1009	,
1034	,
1059	,
1077	,
1088	,
1096	,
1103	,
1112	,
1122	,
1130	,
1139	,
1151	,
1160	,
1163	,
1152	,
1134	,
1112	,
1099	,
1081	,
1076	,
1070	,
1061	,
1050	,
1038	,
1032	,
1034	,
1039	,
1038	,
1028	,
1013	,
1007	,
1010	,
1029	,
1062	,
1097	,
1131	,
1157	,
1174	,
1177	,
1160	,
1130	,
1088	,
1031	,
961	,
888	,
800	,
706	,
601	,
490	,
355	,
215	,
136	,
166	,
314	,
532	,
741	,
911	,
1049	,
1154	,
1229	,
1273	,
1285	,
1260	,
1214	,
1168	,
1131	,
1103	,
1071	,
1040	,
1007	,
981	,
956	,
939	,
929	,
939	,
957	,
982	,
1007	,
1033	,
1055	,
1074	,
1083	,
1084	,
1081	,
1074	,
1076	,
1074	,
1072	,
1067	,
1057	,
1050	,
1041	,
1032	,
1018	,
1003	,
986	,
975	,
969	,
966	,
969	,
969	,
974	,
976	,
984	,
994	,
1010	,
1028	,
1047	,
1069	,
1090	,
1109	,
1123	,
1130	,
1131	,
1128	,
1122	,
1112	,
1094	,
1079	,
1064	,
1054	,
1050	,
1052	,
1062	,
1076	,
1082	,
1077	,
1067	,
1054	,
1048	,
1046	,
1048	,
1055	,
1059	,
1063	,
1063	,
1060	,
1054	,
1047	,
1040	,
1034	,
1034	,
1036	,
1045	,
1058	,
1075	,
1094	,
1106	,
1113	,
1111	,
1106	,
1094	,
1085	,
1074	,
1062	,
1055	,
1052	,
1055	,
1060	,
1065	,
1069	,
1061	,
1049	,
1025	,
1000	,
979	,
973	
};	
const uint16_t bassoonArray[171] = {	
1005	,
	
	
1041	,
	
	
1092	,
	
	
1148	,
	
	
1172	,
	
	
1175	,
	
	
1176	,
	
	
1176	,
	
	
1173	,
	
	
1167	,
	
	
1157	,
	
	
1144	,
	
	
1131	,
	
	
1121	,
	
	
1116	,
	
	
1112	,
	
	
1110	,
	
	
1105	,
	
	
1099	,
	
	
1090	,
	
	
1079	,
	
	
1065	,
	
	
1048	,
	
	
1029	,
	
	
1009	,
	
	
992	,
	
	
977	,
	
	
967	,
	
	
960	,
	
	
963	,
	
	
972	,
	
	
987	,
	
	
1010	,
	
	
1047	,
	
	
1108	,
	
	
1197	,
	
	
1313	,
	
	
1456	,
	
	
1619	,
	
	
1773	,
	
	
1876	,
	
	
1891	,
	
	
1849	,
	
	
1754	,
	
	
1616	,
	
	
1430	,
	
	
1212	,
	
	
991	,
	
	
786	,
	
	
605	,
	
	
457	,
	
	
357	,
	
	
305	,
	
	
286	,
	
	
295	,
	
	
332	,
	
	
384	,
	
	
442	,
	
	
501	,
	
	
558	,
	
	
615	,
	
	
676	,
	
	
743	,
	
	
816	,
	
	
900	,
	
	
990	,
	
	
1086	,
	
	
1179	,
	
	
1261	,
	
	
1331	,
	
	
1388	,
	
	
1431	,
	
	
1459	,
	
	
1469	,
	
	
1460	,
	
	
1433	,
	
	
1388	,
	
	
1328	,
	
	
1260	,
	
	
1190	,
	
	
1116	,
	
	
1042	,
	
	
967	,
	
	
896	,
	
	
829	,
	
	
766	,
	
	
706	,
	
	
647	,
	
	
594	,
	
	
547	,
	
	
512	,
	
	
491	,
	
	
488	,
	
	
508	,
	
	
545	,
	
	
599	,
	
	
670	,
	
	
756	,
	
	
850	,
	
	
949	,
	
	
1043	,
	
	
1127	,
	
	
1199	,
	
	
1262	,
	
	
1307	,
	
	
1329	,
	
	
1326	,
	
	
1297	,
	
	
1248	,
	
	
1182	,
	
	
1108	,
	
	
1035	,
	
	
971	,
	
	
915	,
	
	
870	,
	
	
841	,
	
	
823	,
	
	
820	,
	
	
823	,
	
	
826	,
	
	
833	,
	
	
844	,
	
	
856	,
	
	
862	,
	
	
863	,
	
	
862	,
	
	
859	,
	
	
859	,
	
	
869	,
	
	
888	,
	
	
910	,
	
	
934	,
	
	
958	,
	
	
980	,
	
	
996	,
	
	
1008	,
	
	
1017	,
	
	
1020	,
	
	
1025	,
	
	
1034	,
	
	
1043	,
	
	
1056	,
	
	
1071	,
	
	
1093	,
	
	
1125	,
	
	
1161	,
	
	
1196	,
	
	
1219	,
	
	
1229	,
	
	
1223	,
	
	
1205	,
	
	
1182	,
	
	
1160	,
	
	
1141	,
	
	
1123	,
	
	
1108	,
	
	
1097	,
	
	
1093	,
	
	
1095	,
	
	
1103	,
	
	
1111	,
	
	
1112	,
	
	
1108	,
	
	
1095	,
	
	
1078	,
	
	
1061	,
	
	
1044	,
	
	
1021	,
	
	
1005	,
	
	
1001	,
	
	
1005	,
};	
const uint16_t oboeArray[128] = {	
1024	,
1024	,
1024	,
1024	,
1017	,
1011	,
1007	,
1008	,
1014	,
1029	,
1052	,
1078	,
1098	,
1088	,
1038	,
973	,
898	,
818	,
743	,
678	,
627	,
597	,
588	,
604	,
643	,
700	,
769	,
843	,
917	,
986	,
1048	,
1099	,
1139	,
1169	,
1187	,
1195	,
1200	,
1204	,
1211	,
1220	,
1231	,
1241	,
1251	,
1259	,
1266	,
1278	,
1295	,
1309	,
1318	,
1318	,
1309	,
1289	,
1258	,
1217	,
1167	,
1112	,
1052	,
992	,
935	,
885	,
843	,
810	,
786	,
771	,
761	,
755	,
755	,
759	,
771	,
793	,
830	,
881	,
940	,
1003	,
1062	,
1114	,
1158	,
1195	,
1219	,
1233	,
1236	,
1228	,
1213	,
1192	,
1169	,
1145	,
1121	,
1098	,
1077	,
1057	,
1037	,
1019	,
1002	,
984	,
966	,
950	,
935	,
922	,
910	,
900	,
893	,
891	,
895	,
905	,
925	,
955	,
996	,
1048	,
1102	,
1148	,
1165	,
1149	,
1113	,
1060	,
997	,
932	,
866	,
806	,
784	,
782	,
799	,
832	,
873	,
916	,
955	,
987	,
1010	,
1024	
};	

const uint8_t FullEnvelopeArray[20] = { 32,80,104,116,122,128,126,122,120,116,112,104,98,90,80,70,58,46,38,32 };
const uint8_t FrontEnvelopeArray[20] = { 32,80,104,116,122,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128 };
const uint8_t BackEnvelopeArray[20] = { 128,128,128,128,128,128,126,122,120,116,112,104,98,90,80,70,58,46,38,32 };



// Music Globals
uint8_t Parts_Ready = 0;
const int Part_Length = 256; // 2^8
int8_t IsPaused;
const int8_t Incrementer = 1;
enum{
  Volume1,
  Volume2,
  Volume3,
  Volume4,
  Volume5,
  Volume6,
  Volume7,
  Volume8,
  Volume9,
  Volume10,
};
uint8_t Volume = Volume10;
// Part 0
const int part0_length = Part_Length;
struct MyMusic part0[Part_Length];
int part0_instrument = Bassoon;
const int part0_instrumentResolution = 171;
int part0_instrumentIndex;
int part0_index;
int part0_stepsRemaining;
int part0_OutValue;
int part0_StepsPerEnvelope;
int part0_EnvelopeStepCount;
int part0_EnvelopeIndex;
// Part 1
const int part1_length = Part_Length;
struct MyMusic part1[Part_Length];
int part1_instrument = Trumpet;
const int part1_instrumentResolution = 168;
int part1_instrumentIndex;
int part1_index;
int part1_stepsRemaining;
int part1_OutValue;
int part1_StepsPerEnvelope;
int part1_EnvelopeStepCount;
int part1_EnvelopeIndex;
// Part 2
const int part2_length = Part_Length;
struct MyMusic part2[Part_Length];
int part2_instrument = Oboe;
const int part2_instrumentResolution = 128;
int part2_instrumentIndex;
int part2_index;
int part2_stepsRemaining;
int part2_OutValue;
int part2_StepsPerEnvelope;
int part2_EnvelopeStepCount;
int part2_EnvelopeIndex;
// Prototypes
void static Timer0A_Init(uint32_t period);
void static Timer1A_Init(uint32_t period);
void static Timer2A_Init(uint32_t period);

void Player_Restart(void){
  DisableInterrupts();                                       // (0) This is a critical section
  
  part0_OutValue = 0;                                        // (1) Clear output
  part0_index = 0;                                           // (2) Start Music over
  part0_stepsRemaining = part0[part0_index].numberOfSteps;   // (3) Load First Note
  part0_EnvelopeIndex = 0;                                   // (4) Start envelope over
  part0_EnvelopeStepCount = 0;                               // (4) Start envelope over
  
  part1_OutValue = 0;                                        // (1) Clear output
  part1_index = 0;                                           // (2) Start Music over 
  part1_stepsRemaining = part1[part1_index].numberOfSteps;   // (3) Load First Note
  part1_EnvelopeIndex = 0;                                   // (4) Start envelope over
  part1_EnvelopeStepCount = 0;                               // (4) Start envelope over
  
  part2_OutValue = 0;                                        // (1) Clear output
  part2_index = 0;                                           // (2) Start Music over  
  part2_stepsRemaining = part2[part2_index].numberOfSteps;   // (3) Load First Note
  part2_EnvelopeIndex = 0;                                   // (4) Start envelope over
  part2_EnvelopeStepCount = 0;                               // (4) Start envelope over
  
  TIMER0_TAILR_R = part0[part0_index].cyclesPerStep - 1;     // (5) Load First Duration
  TIMER1_TAILR_R = part1[part2_index].cyclesPerStep - 1;     // (5) Load First Duration
  TIMER2_TAILR_R = part1[part2_index].cyclesPerStep - 1;     // (5) Load First Duration
  
  EnableInterrupts();                                        // (6) Can't be nested

}

void Player_Pause(void){
  if(IsPaused){                   // Already paused, start the timers
    TIMER0_CTL_R = 0x00000001;    
    TIMER1_CTL_R = 0x00000001;    
    TIMER2_CTL_R = 0x00000001;    
    IsPaused = 0;                 // Unpause
  }
  else{                           // Not paused, stop the timers
    TIMER0_CTL_R = 0x00000000; 
    TIMER1_CTL_R = 0x00000000; 
    TIMER2_CTL_R = 0x00000000; 
    IsPaused = 1;                 // Pause
  }
}

// Loops from loudest to softest. Rolls over to Loudest again.
void Player_ChangeVolume(void){
  switch(Volume){
    case Volume10:
      Volume = Volume9;
      break;
    case Volume9:
      Volume = Volume8;
      break;
    case Volume8:
      Volume = Volume7;
      break;
    case Volume7:
      Volume = Volume6;
      break;
    case Volume6:
      Volume = Volume5;
      break;
    case Volume5:
      Volume = Volume4;
      break;
    case Volume4:
      Volume = Volume3;
      break;
    case Volume3:
      Volume = Volume2;
      break;
    case Volume2:
      Volume = Volume1;
      break;
    case Volume1:
      Volume = Volume10;
      break;
  }
}

void Player_Init(void){
#if DEBUG
  PortB_Init();
#endif //DEBUG
  
  NVIC_SYS_PRI3_R |= 7<<21;        // (0) Set PendSV to lowest priority
  DAC_Init(4095);                  // (1) Initialize with command: Vout = Vref
  
  part0_OutValue = 0;              // (2) Ensure no weird compiler numbers
  part1_OutValue = 0; 
  part2_OutValue = 0;
  
  // init parts
  Part_Init(Bassoon_Part, Bassoon_Part_Length, part0, part0_length, part0_instrumentResolution);  // (3) Convert pretty music, to playable music
  part0_index = 0; part0_instrumentIndex = 0;                                                     // (4) Start at begining of Music, and instrument
  part0_stepsRemaining = part0[part0_index].numberOfSteps;                                        // (5) Load first duration
  Part_Init(Trumpet_Part, Trumpet_Part_Length, part1, part1_length, part1_instrumentResolution);  // (3)
  part1_index = 0; part1_instrumentIndex = 0;                                                     // (4)
  part1_stepsRemaining = part1[part1_index].numberOfSteps;                                        // (5)
  Part_Init(Oboe_Part, Oboe_Part_Length, part2, part2_length, part2_instrumentResolution);        // (3)
  part2_index = 0; part2_instrumentIndex = 0;                                                     // (4)
  part2_stepsRemaining = part2[part2_index].numberOfSteps;                                        // (5)

  // start timers
  Timer0A_Init(part0[part0_index].cyclesPerStep);                                                 // (6) Initialize timers with first note
  Timer1A_Init(part1[part1_index].cyclesPerStep);                                                 // (6)
  Timer2A_Init(part2[part2_index].cyclesPerStep);                                                 // (6)
  IsPaused = 0; Player_Pause();                                                                   // (7) Begin paused
  
}

// Returns the array address to the instrument inputed
const uint16_t* FindInstrumentArray(int instrument){
  switch(instrument){
    case Trumpet:
      return trumpetArray;
    case Bassoon:
      return bassoonArray;
    case Oboe:
      return oboeArray;
  }
  return 0;
}

void PendSV_Handler(void){int output;
  output =  0;                       // (0) Start at level  | MAX = 0
  output += part0_OutValue>>2;       // (1) Add a part/2    | MAX = 2048
  output += part1_OutValue>>2;       // (1) Add a part/2    | MAX = 4096
  output += part2_OutValue>>2;       // (1) Add a part/2    | MAX = 6144  !!!
  switch(Volume){
    case Volume10:
      output *= 10;                  // (3) Add multiplier  | MAX = 61440 !!!
      break;
    case Volume9:
      output *= 9;                   // (3) Add multiplier  | MAX = 55296 !!!
      break;
    case Volume8:
      output *= 8;                   // (3) Add multiplier  | MAX = 49152 !!!
      break;
    case Volume7:
      output *= 7;                   // (3) Add multiplier  | MAX = 43008 !!!
      break;
    case Volume6:
      output *= 6;                   // (3) Add multiplier  | MAX = 36864 !!!
      break;
    case Volume5:
      output *= 5;                   // (3) Add multiplier  | MAX = 30720 !!!
      break;
    case Volume4:
      output *= 4;                   // (3) Add multiplier  | MAX = 24576 !!!
      break;
    case Volume3:
      output *= 3;                   // (3) Add multiplier  | MAX = 18432 !!!
      break;
    case Volume2:
      output *= 2;                   // (3) Add multiplier  | MAX = 12288 !!!
      break;
    case Volume1:
      output *= 1;                   // (3) Add multiplier  | MAX = 6144  !!!
      break;
  }
  output = output>>4;                // (4) Bring back down | MAX = 3840
  DAC_Out(output);                   // (5) Send to DAC
  
#if DEBUG
  PB2 ^= 0x04;
  if((0>output)||(output>4095)){
    while(1){}
  }
#endif //DEBUG
}

// ***************** Timer0A_Init ****************
// Activate TIMER0 interrupts to play note
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq), 32 bits
// Outputs: none
void static Timer0A_Init(uint32_t period){long sr;
  sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
  TIMER0_CTL_R = 0x00000000;    // 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER0_TAILR_R = period-1;    // 4) reload value
  TIMER0_TAPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<19;           // 9) enable IRQ 19 in NVIC
  TIMER0_CTL_R = 0x00000001;    // 10) enable TIMER0A
  EndCritical(sr);
}

void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;                                                    // (0) acknowledge timer0A timeout
  part0_stepsRemaining--;                                                               // (1) Decrement duration count
  if(part0_stepsRemaining){                                                             // (2) Check if duration complete
    do{
      part0_index = ( part0_index + Incrementer) & 0xFF;                                // (3) Rolling index 0 - 255
    } while(!((part0[part0_index].cyclesPerStep)&&(part0[part0_index].numberOfSteps))); // (4) Skip any zero values, they are invalid
    TIMER0_TAILR_R = part0[part0_index].cyclesPerStep - 1;                              // (5) Set new note
    part0_stepsRemaining = part0[part0_index].numberOfSteps;                            // (6) Set new duration
    part0_StepsPerEnvelope = part0_stepsRemaining / 20;                                 // (7) Calculate steps per envelope index using (6)
    part0_EnvelopeIndex = 0; part0_EnvelopeStepCount = 0;                               // (8) Start envelope at beginning
  }
  part0_OutValue = FindInstrumentArray(part0_instrument)[part0_instrumentIndex];        // (9) Get instrument value for DAC
  part0_instrumentIndex++;                                                              // (10) Increment instrument index... 
  if(part0_instrumentIndex>=part0_instrumentResolution){                                //      Instrument index DOES NOT rollover because of randomness of data
    part0_instrumentIndex = 0;
  }
  if(part0_EnvelopeStepCount >= part0_StepsPerEnvelope){                                // (11) Check for completed envelope index
    part0_EnvelopeStepCount = 0;
    part0_EnvelopeIndex++;
  }
#if ENVELOPE_ON
  part0_EnvelopeStepCount++;          
  switch(part0[part0_index].Envelope){                                                  // (12) Find correct envelope
    case FullEnvelope:
      part0_OutValue = (part0_OutValue*FullEnvelopeArray[part0_EnvelopeIndex])>>7;
      break;
    case FrontEnvelope:
      part0_OutValue = (part0_OutValue*FrontEnvelopeArray[part0_EnvelopeIndex])>>7;
      break;
    case BackEnvelope:
      part0_OutValue = (part0_OutValue*BackEnvelopeArray[part0_EnvelopeIndex])>>7;
      break;
    case NoEnvelope:
      break;
  }
#endif //ENVELOPE_ON
  if(Parts_Ready){                                                                      // (13) Only trigger PendSV if all other timers have triggered
    NVIC_INT_CTRL_R = 0x10000000;                                                       // (14) Trigger PendSV to output to DAC
  }
}

void static Timer1A_Init(uint32_t period){long sr;
  sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1A
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER1_TAILR_R = period-1;    // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00008000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 37, interrupt number 21
  NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC
  TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A  
  EndCritical(sr);
}

void Timer1A_Handler(void){
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;                                                    // (0) acknowledge timer1A timeout
  part1_stepsRemaining--;                                                               // (1) Decrement duration count
  if(part1_stepsRemaining){                                                             // (2) Check if duration complete
    do{
      part1_index = (part1_index + Incrementer) & 0xFF;                                 // (3) Rolling index 0 - 255
    } while(!((part1[part1_index].cyclesPerStep)&&(part1[part1_index].numberOfSteps))); // (4) Skip any zero values, they are invalid
    TIMER1_TAILR_R = part1[part1_index].cyclesPerStep - 1;                              // (5) Set new note
    part1_stepsRemaining = part1[part1_index].numberOfSteps;                            // (6) Set new duration
    part1_StepsPerEnvelope = part1_stepsRemaining / 20;                                 // (7) Calculate steps per envelope index using (6)
    part1_EnvelopeIndex = 0; part1_EnvelopeStepCount = 0;                               // (8) Start envelope at beginning
  }
  part1_OutValue = FindInstrumentArray(part1_instrument)[part1_instrumentIndex];        // (9) Get instrument value for DAC
  part1_instrumentIndex++;                                                              // (10) Increment instrument index... 
  if(part1_instrumentIndex>=part1_instrumentResolution){                                //      Instrument index DOES NOT rollover because of randomness of data
    part1_instrumentIndex = 0;
  }
  if(part1_EnvelopeStepCount >= part1_StepsPerEnvelope){                                // (11) Check for completed envelope index
    part1_EnvelopeStepCount = 0;
    part1_EnvelopeIndex++;
  }
#if ENVELOPE_ON
  part1_EnvelopeStepCount++;
  switch(part1[part1_index].Envelope){                                                  // (12) Find correct envelope
    case FullEnvelope:
      part1_OutValue = (part1_OutValue*FullEnvelopeArray[part1_EnvelopeIndex])>>7;
      break;
    case FrontEnvelope:
      part1_OutValue = (part1_OutValue*FrontEnvelopeArray[part1_EnvelopeIndex])>>7;
      break;
    case BackEnvelope:
      part1_OutValue = (part1_OutValue*BackEnvelopeArray[part1_EnvelopeIndex])>>7;
      break;
    case NoEnvelope:
      break;
  }
#endif //ENVELOPE_ON
  if(Parts_Ready){                                                                      // (13) Only trigger PendSV if all other timers have triggered
    NVIC_INT_CTRL_R = 0x10000000;                                                       // (14) Trigger PendSV to output to DAC
  }
}

void static Timer2A_Init(uint32_t period){long sr;
  sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate TIMER2
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
  EndCritical(sr);
}

void Timer2A_Handler(void){
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;                                                    // (0) Acknowledge timer2A timeout
  if(!Parts_Ready){                                                                     // (0.5) Signal to other timers that all timers have run 
    Parts_Ready++;
  }
  part2_stepsRemaining--;                                                               // (1) Decrement duration count
  if(part2_stepsRemaining){                                                             // (2) Check if duration complete
    do{
      part2_index = (part2_index + Incrementer) & 0xFF;                                 // (3) Rolling index 0 - 255
    } while(!((part2[part2_index].cyclesPerStep)&&(part2[part2_index].numberOfSteps))); // (4) Skip any zero values, they are invalid
    TIMER2_TAILR_R = part2[part2_index].cyclesPerStep - 1;                              // (5) Set new note
    part2_stepsRemaining = part2[part2_index].numberOfSteps;                            // (6) Set new duration
    part2_StepsPerEnvelope = part2_stepsRemaining / 20;                                 // (7) Calculate steps per envelope index using (6)
    part2_EnvelopeIndex = 0; part2_EnvelopeStepCount = 0;                               // (8) Start envelope at beginning
  }
  part2_OutValue = FindInstrumentArray(part2_instrument)[part2_instrumentIndex];        // (9) Get instrument value for DAC
  part2_instrumentIndex++;                                                              // (10) Increment instrument index... 
  if(part2_instrumentIndex>=part2_instrumentResolution){                                //      Instrument index DOES NOT rollover because of randomness of data
    part2_instrumentIndex = 0;
  }
  if(part2_EnvelopeStepCount >= part2_StepsPerEnvelope){                                // (11) Check for completed envelope index
    part2_EnvelopeStepCount = 0;
    part2_EnvelopeIndex++;
  }
#if ENVELOPE_ON
  part2_EnvelopeStepCount++;
  switch(part2[part2_index].Envelope){                                                  // (12) Find correct envelope
    case FullEnvelope:
      part2_OutValue = (part2_OutValue*FullEnvelopeArray[part2_EnvelopeIndex])>>7;
      break;
    case FrontEnvelope:
      part2_OutValue = (part2_OutValue*FrontEnvelopeArray[part2_EnvelopeIndex])>>7;
      break;
    case BackEnvelope:
      part2_OutValue = (part2_OutValue*BackEnvelopeArray[part2_EnvelopeIndex])>>7;
      break;
    case NoEnvelope:
      break;
  }
#endif //ENVELOPE_ON
  if(Parts_Ready){                                                                      // (13) Only trigger PendSV if all other timers have triggered
    NVIC_INT_CTRL_R = 0x10000000;                                                       // (14) Trigger PendSV to output to DAC
  }
}
