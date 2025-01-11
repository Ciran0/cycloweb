//
//	MBsysTran - Release 8.1
//
//	Copyright 
//	Universite catholique de Louvain (UCLouvain) 
//	Mechatronic, Electrical Energy, and Dynamic systems (MEED Division) 
//	2, Place du Levant
//	1348 Louvain-la-Neuve 
//	Belgium 
//
//	http://www.robotran.be 
//
//	==> Generation Date: Fri Nov  3 09:45:19 2023
//
//	==> Project name: DutchBike
//
//	==> Number of joints: 32
//
//	==> Function: F6 - Sensors Kinematics
//
//	==> Git hash: 8f46effdc05c898000a15b4c4dfc8f70efce4fc0
//

#include <math.h> 

#include "mbs_data.h"
#include "mbs_sensor.h"

void mbs_sensor(MbsSensor *sens,
MbsData *s, int isens)
{
#include "mbs_sensor_DutchBike.h"

double *q, *qd, *qdd;
double **dpt;

q = s->q;
qd = s->qd;
qdd = s->qdd;

dpt = s->dpt;
 
// Trigonometric functions

S4 = sin(q[4]);
C4 = cos(q[4]);
S5 = sin(q[5]);
C5 = cos(q[5]);
S6 = sin(q[6]);
C6 = cos(q[6]);
S10 = sin(q[10]);
C10 = cos(q[10]);
S11 = sin(q[11]);
C11 = cos(q[11]);
S12 = sin(q[12]);
C12 = cos(q[12]);
S13 = sin(q[13]);
C13 = cos(q[13]);
 
// Augmented Joint Position Vectors

 
// Sensor Kinematics


switch(isens)
{
case 1:

ROcp1_45 = -S4*C5;
ROcp1_55 = C4*C5;
ROcp1_75 = S4*S5;
ROcp1_85 = -C4*S5;
ROcp1_16 = -ROcp1_75*S6+C4*C6;
ROcp1_26 = -ROcp1_85*S6+S4*C6;
ROcp1_36 = -C5*S6;
ROcp1_76 = ROcp1_75*C6+C4*S6;
ROcp1_86 = ROcp1_85*C6+S4*S6;
ROcp1_96 = C5*C6;
OMcp1_15 = qd[5]*C4;
OMcp1_25 = qd[5]*S4;
OPcp1_15 = qdd[5]*C4-qd[4]*qd[5]*S4;
OPcp1_25 = qdd[5]*S4+qd[4]*qd[5]*C4;
OMcp1_16 = OMcp1_15+ROcp1_45*qd[6];
OMcp1_26 = OMcp1_25+ROcp1_55*qd[6];
OMcp1_36 = qd[4]+qd[6]*S5;
OPcp1_16 = OPcp1_15+ROcp1_45*qdd[6]+qd[6]*(OMcp1_25*S5-ROcp1_55*qd[4]);
OPcp1_26 = OPcp1_25+ROcp1_55*qdd[6]+qd[6]*(-OMcp1_15*S5+ROcp1_45*qd[4]);
OPcp1_36 = qdd[4]+qdd[6]*S5+qd[6]*(OMcp1_15*ROcp1_55-OMcp1_25*ROcp1_45);
RLcp1_17 = ROcp1_16*dpt[1][4]+ROcp1_76*dpt[3][4];
RLcp1_27 = ROcp1_26*dpt[1][4]+ROcp1_86*dpt[3][4];
RLcp1_37 = ROcp1_36*dpt[1][4]+ROcp1_96*dpt[3][4];
POcp1_17 = RLcp1_17+q[1];
POcp1_27 = RLcp1_27+q[2];
POcp1_37 = RLcp1_37+q[3];
ORcp1_17 = OMcp1_26*RLcp1_37-OMcp1_36*RLcp1_27;
ORcp1_27 = -OMcp1_16*RLcp1_37+OMcp1_36*RLcp1_17;
ORcp1_37 = OMcp1_16*RLcp1_27-OMcp1_26*RLcp1_17;
VIcp1_17 = ORcp1_17+qd[1];
VIcp1_27 = ORcp1_27+qd[2];
VIcp1_37 = ORcp1_37+qd[3];
ACcp1_17 = qdd[1]+OMcp1_26*ORcp1_37-OMcp1_36*ORcp1_27+OPcp1_26*RLcp1_37-OPcp1_36*RLcp1_27;
ACcp1_27 = qdd[2]-OMcp1_16*ORcp1_37+OMcp1_36*ORcp1_17-OPcp1_16*RLcp1_37+OPcp1_36*RLcp1_17;
ACcp1_37 = qdd[3]+OMcp1_16*ORcp1_27-OMcp1_26*ORcp1_17+OPcp1_16*RLcp1_27-OPcp1_26*RLcp1_17;
sens->P[1] = POcp1_17;
sens->P[2] = POcp1_27;
sens->P[3] = POcp1_37;
sens->R[1][1] = ROcp1_16;
sens->R[1][2] = ROcp1_26;
sens->R[1][3] = ROcp1_36;
sens->R[2][1] = ROcp1_45;
sens->R[2][2] = ROcp1_55;
sens->R[2][3] = S5;
sens->R[3][1] = ROcp1_76;
sens->R[3][2] = ROcp1_86;
sens->R[3][3] = ROcp1_96;
sens->V[1] = VIcp1_17;
sens->V[2] = VIcp1_27;
sens->V[3] = VIcp1_37;
sens->OM[1] = OMcp1_16;
sens->OM[2] = OMcp1_26;
sens->OM[3] = OMcp1_36;
sens->A[1] = ACcp1_17;
sens->A[2] = ACcp1_27;
sens->A[3] = ACcp1_37;
sens->OMP[1] = OPcp1_16;
sens->OMP[2] = OPcp1_26;
sens->OMP[3] = OPcp1_36;

break;

case 2:

ROcp2_45 = -S4*C5;
ROcp2_55 = C4*C5;
ROcp2_75 = S4*S5;
ROcp2_85 = -C4*S5;
ROcp2_16 = -ROcp2_75*S6+C4*C6;
ROcp2_26 = -ROcp2_85*S6+S4*C6;
ROcp2_36 = -C5*S6;
ROcp2_76 = ROcp2_75*C6+C4*S6;
ROcp2_86 = ROcp2_85*C6+S4*S6;
ROcp2_96 = C5*C6;
ROcp2_110 = ROcp2_16*C10-ROcp2_76*S10;
ROcp2_210 = ROcp2_26*C10-ROcp2_86*S10;
ROcp2_310 = ROcp2_36*C10-ROcp2_96*S10;
ROcp2_710 = ROcp2_16*S10+ROcp2_76*C10;
ROcp2_810 = ROcp2_26*S10+ROcp2_86*C10;
ROcp2_910 = ROcp2_36*S10+ROcp2_96*C10;
ROcp2_111 = ROcp2_110*C11+ROcp2_45*S11;
ROcp2_211 = ROcp2_210*C11+ROcp2_55*S11;
ROcp2_311 = ROcp2_310*C11+S11*S5;
ROcp2_411 = -ROcp2_110*S11+ROcp2_45*C11;
ROcp2_511 = -ROcp2_210*S11+ROcp2_55*C11;
ROcp2_611 = -ROcp2_310*S11+C11*S5;
ROcp2_112 = ROcp2_111*C12-ROcp2_710*S12;
ROcp2_212 = ROcp2_211*C12-ROcp2_810*S12;
ROcp2_312 = ROcp2_311*C12-ROcp2_910*S12;
ROcp2_712 = ROcp2_111*S12+ROcp2_710*C12;
ROcp2_812 = ROcp2_211*S12+ROcp2_810*C12;
ROcp2_912 = ROcp2_311*S12+ROcp2_910*C12;
OMcp2_15 = qd[5]*C4;
OMcp2_25 = qd[5]*S4;
OPcp2_15 = qdd[5]*C4-qd[4]*qd[5]*S4;
OPcp2_25 = qdd[5]*S4+qd[4]*qd[5]*C4;
OMcp2_16 = OMcp2_15+ROcp2_45*qd[6];
OMcp2_26 = OMcp2_25+ROcp2_55*qd[6];
OMcp2_36 = qd[4]+qd[6]*S5;
OPcp2_16 = OPcp2_15+ROcp2_45*qdd[6]+qd[6]*(OMcp2_25*S5-ROcp2_55*qd[4]);
OPcp2_26 = OPcp2_25+ROcp2_55*qdd[6]+qd[6]*(-OMcp2_15*S5+ROcp2_45*qd[4]);
OPcp2_36 = qdd[4]+qdd[6]*S5+qd[6]*(OMcp2_15*ROcp2_55-OMcp2_25*ROcp2_45);
RLcp2_17 = ROcp2_16*dpt[1][1]+ROcp2_76*dpt[3][1];
RLcp2_27 = ROcp2_26*dpt[1][1]+ROcp2_86*dpt[3][1];
RLcp2_37 = ROcp2_36*dpt[1][1]+ROcp2_96*dpt[3][1];
POcp2_17 = RLcp2_17+q[1];
POcp2_27 = RLcp2_27+q[2];
POcp2_37 = RLcp2_37+q[3];
OMcp2_17 = OMcp2_16+ROcp2_45*qd[10];
OMcp2_27 = OMcp2_26+ROcp2_55*qd[10];
OMcp2_37 = OMcp2_36+qd[10]*S5;
ORcp2_17 = OMcp2_26*RLcp2_37-OMcp2_36*RLcp2_27;
ORcp2_27 = -OMcp2_16*RLcp2_37+OMcp2_36*RLcp2_17;
ORcp2_37 = OMcp2_16*RLcp2_27-OMcp2_26*RLcp2_17;
VIcp2_17 = ORcp2_17+qd[1];
VIcp2_27 = ORcp2_27+qd[2];
VIcp2_37 = ORcp2_37+qd[3];
OPcp2_17 = OPcp2_16+ROcp2_45*qdd[10]+qd[10]*(OMcp2_26*S5-OMcp2_36*ROcp2_55);
OPcp2_27 = OPcp2_26+ROcp2_55*qdd[10]+qd[10]*(-OMcp2_16*S5+OMcp2_36*ROcp2_45);
OPcp2_37 = OPcp2_36+qdd[10]*S5+qd[10]*(OMcp2_16*ROcp2_55-OMcp2_26*ROcp2_45);
ACcp2_17 = qdd[1]+OMcp2_26*ORcp2_37-OMcp2_36*ORcp2_27+OPcp2_26*RLcp2_37-OPcp2_36*RLcp2_27;
ACcp2_27 = qdd[2]-OMcp2_16*ORcp2_37+OMcp2_36*ORcp2_17-OPcp2_16*RLcp2_37+OPcp2_36*RLcp2_17;
ACcp2_37 = qdd[3]+OMcp2_16*ORcp2_27-OMcp2_26*ORcp2_17+OPcp2_16*RLcp2_27-OPcp2_26*RLcp2_17;
OMcp2_18 = OMcp2_17+ROcp2_710*qd[11];
OMcp2_28 = OMcp2_27+ROcp2_810*qd[11];
OMcp2_38 = OMcp2_37+ROcp2_910*qd[11];
OPcp2_18 = OPcp2_17+ROcp2_710*qdd[11]+qd[11]*(OMcp2_27*ROcp2_910-OMcp2_37*ROcp2_810);
OPcp2_28 = OPcp2_27+ROcp2_810*qdd[11]+qd[11]*(-OMcp2_17*ROcp2_910+OMcp2_37*ROcp2_710);
OPcp2_38 = OPcp2_37+ROcp2_910*qdd[11]+qd[11]*(OMcp2_17*ROcp2_810-OMcp2_27*ROcp2_710);
RLcp2_19 = ROcp2_111*dpt[1][5]+ROcp2_710*dpt[3][5];
RLcp2_29 = ROcp2_211*dpt[1][5]+ROcp2_810*dpt[3][5];
RLcp2_39 = ROcp2_311*dpt[1][5]+ROcp2_910*dpt[3][5];
POcp2_19 = POcp2_17+RLcp2_19;
POcp2_29 = POcp2_27+RLcp2_29;
POcp2_39 = POcp2_37+RLcp2_39;
OMcp2_19 = OMcp2_18+ROcp2_411*qd[12];
OMcp2_29 = OMcp2_28+ROcp2_511*qd[12];
OMcp2_39 = OMcp2_38+ROcp2_611*qd[12];
ORcp2_19 = OMcp2_28*RLcp2_39-OMcp2_38*RLcp2_29;
ORcp2_29 = -OMcp2_18*RLcp2_39+OMcp2_38*RLcp2_19;
ORcp2_39 = OMcp2_18*RLcp2_29-OMcp2_28*RLcp2_19;
VIcp2_19 = ORcp2_19+VIcp2_17;
VIcp2_29 = ORcp2_29+VIcp2_27;
VIcp2_39 = ORcp2_39+VIcp2_37;
OPcp2_19 = OPcp2_18+ROcp2_411*qdd[12]+qd[12]*(OMcp2_28*ROcp2_611-OMcp2_38*ROcp2_511);
OPcp2_29 = OPcp2_28+ROcp2_511*qdd[12]+qd[12]*(-OMcp2_18*ROcp2_611+OMcp2_38*ROcp2_411);
OPcp2_39 = OPcp2_38+ROcp2_611*qdd[12]+qd[12]*(OMcp2_18*ROcp2_511-OMcp2_28*ROcp2_411);
ACcp2_19 = ACcp2_17+OMcp2_28*ORcp2_39-OMcp2_38*ORcp2_29+OPcp2_28*RLcp2_39-OPcp2_38*RLcp2_29;
ACcp2_29 = ACcp2_27-OMcp2_18*ORcp2_39+OMcp2_38*ORcp2_19-OPcp2_18*RLcp2_39+OPcp2_38*RLcp2_19;
ACcp2_39 = ACcp2_37+OMcp2_18*ORcp2_29-OMcp2_28*ORcp2_19+OPcp2_18*RLcp2_29-OPcp2_28*RLcp2_19;
sens->P[1] = POcp2_19;
sens->P[2] = POcp2_29;
sens->P[3] = POcp2_39;
sens->R[1][1] = ROcp2_112;
sens->R[1][2] = ROcp2_212;
sens->R[1][3] = ROcp2_312;
sens->R[2][1] = ROcp2_411;
sens->R[2][2] = ROcp2_511;
sens->R[2][3] = ROcp2_611;
sens->R[3][1] = ROcp2_712;
sens->R[3][2] = ROcp2_812;
sens->R[3][3] = ROcp2_912;
sens->V[1] = VIcp2_19;
sens->V[2] = VIcp2_29;
sens->V[3] = VIcp2_39;
sens->OM[1] = OMcp2_19;
sens->OM[2] = OMcp2_29;
sens->OM[3] = OMcp2_39;
sens->A[1] = ACcp2_19;
sens->A[2] = ACcp2_29;
sens->A[3] = ACcp2_39;
sens->OMP[1] = OPcp2_19;
sens->OMP[2] = OPcp2_29;
sens->OMP[3] = OPcp2_39;

break;

case 3:

ROcp3_45 = -S4*C5;
ROcp3_55 = C4*C5;
ROcp3_75 = S4*S5;
ROcp3_85 = -C4*S5;
ROcp3_16 = -ROcp3_75*S6+C4*C6;
ROcp3_26 = -ROcp3_85*S6+S4*C6;
ROcp3_36 = -C5*S6;
ROcp3_76 = ROcp3_75*C6+C4*S6;
ROcp3_86 = ROcp3_85*C6+S4*S6;
ROcp3_96 = C5*C6;
ROcp3_113 = ROcp3_16*C13-ROcp3_76*S13;
ROcp3_213 = ROcp3_26*C13-ROcp3_86*S13;
ROcp3_313 = ROcp3_36*C13-ROcp3_96*S13;
ROcp3_713 = ROcp3_16*S13+ROcp3_76*C13;
ROcp3_813 = ROcp3_26*S13+ROcp3_86*C13;
ROcp3_913 = ROcp3_36*S13+ROcp3_96*C13;
OMcp3_15 = qd[5]*C4;
OMcp3_25 = qd[5]*S4;
OPcp3_15 = qdd[5]*C4-qd[4]*qd[5]*S4;
OPcp3_25 = qdd[5]*S4+qd[4]*qd[5]*C4;
OMcp3_16 = OMcp3_15+ROcp3_45*qd[6];
OMcp3_26 = OMcp3_25+ROcp3_55*qd[6];
OMcp3_36 = qd[4]+qd[6]*S5;
OPcp3_16 = OPcp3_15+ROcp3_45*qdd[6]+qd[6]*(OMcp3_25*S5-ROcp3_55*qd[4]);
OPcp3_26 = OPcp3_25+ROcp3_55*qdd[6]+qd[6]*(-OMcp3_15*S5+ROcp3_45*qd[4]);
OPcp3_36 = qdd[4]+qdd[6]*S5+qd[6]*(OMcp3_15*ROcp3_55-OMcp3_25*ROcp3_45);
RLcp3_17 = ROcp3_16*dpt[1][2]+ROcp3_76*dpt[3][2];
RLcp3_27 = ROcp3_26*dpt[1][2]+ROcp3_86*dpt[3][2];
RLcp3_37 = ROcp3_36*dpt[1][2]+ROcp3_96*dpt[3][2];
POcp3_17 = RLcp3_17+q[1];
POcp3_27 = RLcp3_27+q[2];
POcp3_37 = RLcp3_37+q[3];
OMcp3_17 = OMcp3_16+ROcp3_45*qd[13];
OMcp3_27 = OMcp3_26+ROcp3_55*qd[13];
OMcp3_37 = OMcp3_36+qd[13]*S5;
ORcp3_17 = OMcp3_26*RLcp3_37-OMcp3_36*RLcp3_27;
ORcp3_27 = -OMcp3_16*RLcp3_37+OMcp3_36*RLcp3_17;
ORcp3_37 = OMcp3_16*RLcp3_27-OMcp3_26*RLcp3_17;
VIcp3_17 = ORcp3_17+qd[1];
VIcp3_27 = ORcp3_27+qd[2];
VIcp3_37 = ORcp3_37+qd[3];
OPcp3_17 = OPcp3_16+ROcp3_45*qdd[13]+qd[13]*(OMcp3_26*S5-OMcp3_36*ROcp3_55);
OPcp3_27 = OPcp3_26+ROcp3_55*qdd[13]+qd[13]*(-OMcp3_16*S5+OMcp3_36*ROcp3_45);
OPcp3_37 = OPcp3_36+qdd[13]*S5+qd[13]*(OMcp3_16*ROcp3_55-OMcp3_26*ROcp3_45);
ACcp3_17 = qdd[1]+OMcp3_26*ORcp3_37-OMcp3_36*ORcp3_27+OPcp3_26*RLcp3_37-OPcp3_36*RLcp3_27;
ACcp3_27 = qdd[2]-OMcp3_16*ORcp3_37+OMcp3_36*ORcp3_17-OPcp3_16*RLcp3_37+OPcp3_36*RLcp3_17;
ACcp3_37 = qdd[3]+OMcp3_16*ORcp3_27-OMcp3_26*ORcp3_17+OPcp3_16*RLcp3_27-OPcp3_26*RLcp3_17;
sens->P[1] = POcp3_17;
sens->P[2] = POcp3_27;
sens->P[3] = POcp3_37;
sens->R[1][1] = ROcp3_113;
sens->R[1][2] = ROcp3_213;
sens->R[1][3] = ROcp3_313;
sens->R[2][1] = ROcp3_45;
sens->R[2][2] = ROcp3_55;
sens->R[2][3] = S5;
sens->R[3][1] = ROcp3_713;
sens->R[3][2] = ROcp3_813;
sens->R[3][3] = ROcp3_913;
sens->V[1] = VIcp3_17;
sens->V[2] = VIcp3_27;
sens->V[3] = VIcp3_37;
sens->OM[1] = OMcp3_17;
sens->OM[2] = OMcp3_27;
sens->OM[3] = OMcp3_37;
sens->A[1] = ACcp3_17;
sens->A[2] = ACcp3_27;
sens->A[3] = ACcp3_37;
sens->OMP[1] = OPcp3_17;
sens->OMP[2] = OPcp3_27;
sens->OMP[3] = OPcp3_37;

break;

default:

break;

}


// Number of continuation lines = 0

}
