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
//	==> Function: F19 - External Forces
//
//	==> Git hash: 8f46effdc05c898000a15b4c4dfc8f70efce4fc0
//

#include <math.h> 

#include "mbs_data.h"

#include "mbs_project_interface.h"

// #include "mbs_extforces_xml_DutchBike.h"  // future development
// #include "mbs_extforces_hard_DutchBike.h" // future development
 
void mbs_extforces(double **frc, double **trq,
MbsData *s, double tsim)
{
#include "mbs_extforces_DutchBike.h"

double PxF1[4]; 
double RxF1[4][4]; 
double VxF1[4]; 
double OMxF1[4]; 
double AxF1[4]; 
double OMPxF1[4]; 
double *SWr1; 

double PxF2[4]; 
double RxF2[4][4]; 
double VxF2[4]; 
double OMxF2[4]; 
double AxF2[4]; 
double OMPxF2[4]; 
double *SWr2; 

double *q, *qd, *qdd;
double **dpt, **l;

frc = s->frc;
trq = s->trq;

q = s->q;
qd = s->qd;
qdd = s->qdd;

dpt = s->dpt;
l = s->l;
 
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
OPcp2_15 = -qd[4]*qd[5]*S4+qdd[5]*C4;
OPcp2_25 = qd[4]*qd[5]*C4+qdd[5]*S4;
OMcp2_16 = OMcp2_15+qd[6]*ROcp2_45;
OMcp2_26 = OMcp2_25+qd[6]*ROcp2_55;
OMcp2_36 = qd[4]+qd[6]*S5;
OPcp2_16 = OPcp2_15+qd[6]*(-qd[4]*ROcp2_55+OMcp2_25*S5)+qdd[6]*ROcp2_45;
OPcp2_26 = OPcp2_25+qd[6]*(qd[4]*ROcp2_45-OMcp2_15*S5)+qdd[6]*ROcp2_55;
OPcp2_36 = qdd[4]+qd[6]*(OMcp2_15*ROcp2_55-OMcp2_25*ROcp2_45)+qdd[6]*S5;
RLcp2_17 = ROcp2_16*dpt[1][1]+ROcp2_76*dpt[3][1];
RLcp2_27 = ROcp2_26*dpt[1][1]+ROcp2_86*dpt[3][1];
RLcp2_37 = ROcp2_36*dpt[1][1]+ROcp2_96*dpt[3][1];
POcp2_17 = q[1]+RLcp2_17;
POcp2_27 = q[2]+RLcp2_27;
POcp2_37 = q[3]+RLcp2_37;
OMcp2_17 = OMcp2_16+qd[10]*ROcp2_45;
OMcp2_27 = OMcp2_26+qd[10]*ROcp2_55;
OMcp2_37 = OMcp2_36+qd[10]*S5;
ORcp2_17 = OMcp2_26*RLcp2_37-OMcp2_36*RLcp2_27;
ORcp2_27 = -OMcp2_16*RLcp2_37+OMcp2_36*RLcp2_17;
ORcp2_37 = OMcp2_16*RLcp2_27-OMcp2_26*RLcp2_17;
VIcp2_17 = qd[1]+ORcp2_17;
VIcp2_27 = qd[2]+ORcp2_27;
VIcp2_37 = qd[3]+ORcp2_37;
OPcp2_17 = OPcp2_16+qd[10]*(OMcp2_26*S5-OMcp2_36*ROcp2_55)+qdd[10]*ROcp2_45;
OPcp2_27 = OPcp2_26+qd[10]*(-OMcp2_16*S5+OMcp2_36*ROcp2_45)+qdd[10]*ROcp2_55;
OPcp2_37 = OPcp2_36+qd[10]*(OMcp2_16*ROcp2_55-OMcp2_26*ROcp2_45)+qdd[10]*S5;
ACcp2_17 = qdd[1]+OMcp2_26*ORcp2_37-OMcp2_36*ORcp2_27+OPcp2_26*RLcp2_37-OPcp2_36*RLcp2_27;
ACcp2_27 = qdd[2]-OMcp2_16*ORcp2_37+OMcp2_36*ORcp2_17-OPcp2_16*RLcp2_37+OPcp2_36*RLcp2_17;
ACcp2_37 = qdd[3]+OMcp2_16*ORcp2_27-OMcp2_26*ORcp2_17+OPcp2_16*RLcp2_27-OPcp2_26*RLcp2_17;
OMcp2_18 = OMcp2_17+qd[11]*ROcp2_710;
OMcp2_28 = OMcp2_27+qd[11]*ROcp2_810;
OMcp2_38 = OMcp2_37+qd[11]*ROcp2_910;
OPcp2_18 = OPcp2_17+qd[11]*(OMcp2_27*ROcp2_910-OMcp2_37*ROcp2_810)+qdd[11]*ROcp2_710;
OPcp2_28 = OPcp2_27+qd[11]*(-OMcp2_17*ROcp2_910+OMcp2_37*ROcp2_710)+qdd[11]*ROcp2_810;
OPcp2_38 = OPcp2_37+qd[11]*(OMcp2_17*ROcp2_810-OMcp2_27*ROcp2_710)+qdd[11]*ROcp2_910;
RLcp2_19 = ROcp2_111*dpt[1][5]+ROcp2_710*dpt[3][5];
RLcp2_29 = ROcp2_211*dpt[1][5]+ROcp2_810*dpt[3][5];
RLcp2_39 = ROcp2_311*dpt[1][5]+ROcp2_910*dpt[3][5];
POcp2_19 = POcp2_17+RLcp2_19;
POcp2_29 = POcp2_27+RLcp2_29;
POcp2_39 = POcp2_37+RLcp2_39;
OMcp2_19 = OMcp2_18+qd[12]*ROcp2_411;
OMcp2_29 = OMcp2_28+qd[12]*ROcp2_511;
OMcp2_39 = OMcp2_38+qd[12]*ROcp2_611;
ORcp2_19 = OMcp2_28*RLcp2_39-OMcp2_38*RLcp2_29;
ORcp2_29 = -OMcp2_18*RLcp2_39+OMcp2_38*RLcp2_19;
ORcp2_39 = OMcp2_18*RLcp2_29-OMcp2_28*RLcp2_19;
VIcp2_19 = ORcp2_19+VIcp2_17;
VIcp2_29 = ORcp2_29+VIcp2_27;
VIcp2_39 = ORcp2_39+VIcp2_37;
OPcp2_19 = OPcp2_18+qd[12]*(OMcp2_28*ROcp2_611-OMcp2_38*ROcp2_511)+qdd[12]*ROcp2_411;
OPcp2_29 = OPcp2_28+qd[12]*(-OMcp2_18*ROcp2_611+OMcp2_38*ROcp2_411)+qdd[12]*ROcp2_511;
OPcp2_39 = OPcp2_38+qd[12]*(OMcp2_18*ROcp2_511-OMcp2_28*ROcp2_411)+qdd[12]*ROcp2_611;
ACcp2_19 = ACcp2_17+OMcp2_28*ORcp2_39-OMcp2_38*ORcp2_29+OPcp2_28*RLcp2_39-OPcp2_38*RLcp2_29;
ACcp2_29 = ACcp2_27-OMcp2_18*ORcp2_39+OMcp2_38*ORcp2_19-OPcp2_18*RLcp2_39+OPcp2_38*RLcp2_19;
ACcp2_39 = ACcp2_37+OMcp2_18*ORcp2_29-OMcp2_28*ORcp2_19+OPcp2_18*RLcp2_29-OPcp2_28*RLcp2_19;
PxF1[1] = POcp2_19;
PxF1[2] = POcp2_29;
PxF1[3] = POcp2_39;
RxF1[1][1] = ROcp2_112;
RxF1[1][2] = ROcp2_212;
RxF1[1][3] = ROcp2_312;
RxF1[2][1] = ROcp2_411;
RxF1[2][2] = ROcp2_511;
RxF1[2][3] = ROcp2_611;
RxF1[3][1] = ROcp2_712;
RxF1[3][2] = ROcp2_812;
RxF1[3][3] = ROcp2_912;
VxF1[1] = VIcp2_19;
VxF1[2] = VIcp2_29;
VxF1[3] = VIcp2_39;
OMxF1[1] = OMcp2_19;
OMxF1[2] = OMcp2_29;
OMxF1[3] = OMcp2_39;
AxF1[1] = ACcp2_19;
AxF1[2] = ACcp2_29;
AxF1[3] = ACcp2_39;
OMPxF1[1] = OPcp2_19;
OMPxF1[2] = OPcp2_29;
OMPxF1[3] = OPcp2_39;
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
OPcp3_15 = -qd[4]*qd[5]*S4+qdd[5]*C4;
OPcp3_25 = qd[4]*qd[5]*C4+qdd[5]*S4;
OMcp3_16 = OMcp3_15+qd[6]*ROcp3_45;
OMcp3_26 = OMcp3_25+qd[6]*ROcp3_55;
OMcp3_36 = qd[4]+qd[6]*S5;
OPcp3_16 = OPcp3_15+qd[6]*(-qd[4]*ROcp3_55+OMcp3_25*S5)+qdd[6]*ROcp3_45;
OPcp3_26 = OPcp3_25+qd[6]*(qd[4]*ROcp3_45-OMcp3_15*S5)+qdd[6]*ROcp3_55;
OPcp3_36 = qdd[4]+qd[6]*(OMcp3_15*ROcp3_55-OMcp3_25*ROcp3_45)+qdd[6]*S5;
RLcp3_17 = ROcp3_16*dpt[1][2]+ROcp3_76*dpt[3][2];
RLcp3_27 = ROcp3_26*dpt[1][2]+ROcp3_86*dpt[3][2];
RLcp3_37 = ROcp3_36*dpt[1][2]+ROcp3_96*dpt[3][2];
POcp3_17 = q[1]+RLcp3_17;
POcp3_27 = q[2]+RLcp3_27;
POcp3_37 = q[3]+RLcp3_37;
OMcp3_17 = OMcp3_16+qd[13]*ROcp3_45;
OMcp3_27 = OMcp3_26+qd[13]*ROcp3_55;
OMcp3_37 = OMcp3_36+qd[13]*S5;
ORcp3_17 = OMcp3_26*RLcp3_37-OMcp3_36*RLcp3_27;
ORcp3_27 = -OMcp3_16*RLcp3_37+OMcp3_36*RLcp3_17;
ORcp3_37 = OMcp3_16*RLcp3_27-OMcp3_26*RLcp3_17;
VIcp3_17 = qd[1]+ORcp3_17;
VIcp3_27 = qd[2]+ORcp3_27;
VIcp3_37 = qd[3]+ORcp3_37;
OPcp3_17 = OPcp3_16+qd[13]*(OMcp3_26*S5-OMcp3_36*ROcp3_55)+qdd[13]*ROcp3_45;
OPcp3_27 = OPcp3_26+qd[13]*(-OMcp3_16*S5+OMcp3_36*ROcp3_45)+qdd[13]*ROcp3_55;
OPcp3_37 = OPcp3_36+qd[13]*(OMcp3_16*ROcp3_55-OMcp3_26*ROcp3_45)+qdd[13]*S5;
ACcp3_17 = qdd[1]+OMcp3_26*ORcp3_37-OMcp3_36*ORcp3_27+OPcp3_26*RLcp3_37-OPcp3_36*RLcp3_27;
ACcp3_27 = qdd[2]-OMcp3_16*ORcp3_37+OMcp3_36*ORcp3_17-OPcp3_16*RLcp3_37+OPcp3_36*RLcp3_17;
ACcp3_37 = qdd[3]+OMcp3_16*ORcp3_27-OMcp3_26*ORcp3_17+OPcp3_16*RLcp3_27-OPcp3_26*RLcp3_17;
PxF2[1] = POcp3_17;
PxF2[2] = POcp3_27;
PxF2[3] = POcp3_37;
RxF2[1][1] = ROcp3_113;
RxF2[1][2] = ROcp3_213;
RxF2[1][3] = ROcp3_313;
RxF2[2][1] = ROcp3_45;
RxF2[2][2] = ROcp3_55;
RxF2[2][3] = S5;
RxF2[3][1] = ROcp3_713;
RxF2[3][2] = ROcp3_813;
RxF2[3][3] = ROcp3_913;
VxF2[1] = VIcp3_17;
VxF2[2] = VIcp3_27;
VxF2[3] = VIcp3_37;
OMxF2[1] = OMcp3_17;
OMxF2[2] = OMcp3_27;
OMxF2[3] = OMcp3_37;
AxF2[1] = ACcp3_17;
AxF2[2] = ACcp3_27;
AxF2[3] = ACcp3_37;
OMPxF2[1] = OPcp3_17;
OMPxF2[2] = OPcp3_27;
OMPxF2[3] = OPcp3_37;
 
// Sensor Forces 

SWr1 = user_call_ExtForces(PxF1,RxF1,VxF1,OMxF1,AxF1,OMPxF1,s,tsim,1);
SWr2 = user_call_ExtForces(PxF2,RxF2,VxF2,OMxF2,AxF2,OMPxF2,s,tsim,2);
xfrc12 = RxF1[1][1]*SWr1[1]+RxF1[1][2]*SWr1[2]+RxF1[1][3]*SWr1[3];
xfrc22 = RxF1[2][1]*SWr1[1]+RxF1[2][2]*SWr1[2]+RxF1[2][3]*SWr1[3];
xfrc32 = RxF1[3][1]*SWr1[1]+RxF1[3][2]*SWr1[2]+RxF1[3][3]*SWr1[3];
xtrq12 = RxF1[1][1]*SWr1[4]+RxF1[1][2]*SWr1[5]+RxF1[1][3]*SWr1[6];
xtrq22 = RxF1[2][1]*SWr1[4]+RxF1[2][2]*SWr1[5]+RxF1[2][3]*SWr1[6];
xtrq32 = RxF1[3][1]*SWr1[4]+RxF1[3][2]*SWr1[5]+RxF1[3][3]*SWr1[6];
trqext_1_12_1 = xtrq12-xfrc22*SWr1[9]+xfrc32*SWr1[8];
trqext_2_12_1 = xtrq22+xfrc12*SWr1[9]-xfrc32*SWr1[7];
trqext_3_12_1 = xtrq32-xfrc12*SWr1[8]+xfrc22*SWr1[7];
xfrc13 = RxF2[1][1]*SWr2[1]+RxF2[1][2]*SWr2[2]+RxF2[1][3]*SWr2[3];
xfrc23 = RxF2[2][1]*SWr2[1]+RxF2[2][2]*SWr2[2]+RxF2[2][3]*SWr2[3];
xfrc33 = RxF2[3][1]*SWr2[1]+RxF2[3][2]*SWr2[2]+RxF2[3][3]*SWr2[3];
xtrq13 = RxF2[1][1]*SWr2[4]+RxF2[1][2]*SWr2[5]+RxF2[1][3]*SWr2[6];
xtrq23 = RxF2[2][1]*SWr2[4]+RxF2[2][2]*SWr2[5]+RxF2[2][3]*SWr2[6];
xtrq33 = RxF2[3][1]*SWr2[4]+RxF2[3][2]*SWr2[5]+RxF2[3][3]*SWr2[6];
trqext_1_13_2 = xtrq13-xfrc23*SWr2[9]+xfrc33*SWr2[8];
trqext_2_13_2 = xtrq23+xfrc13*SWr2[9]-xfrc33*SWr2[7];
trqext_3_13_2 = xtrq33-xfrc13*SWr2[8]+xfrc23*SWr2[7];
 
// Symbolic model output

frc[1][12] = frc[1][12]+xfrc12;
frc[2][12] = frc[2][12]+xfrc22;
frc[3][12] = frc[3][12]+xfrc32;
trq[1][12] = trq[1][12]+trqext_1_12_1;
trq[2][12] = trq[2][12]+trqext_2_12_1;
trq[3][12] = trq[3][12]+trqext_3_12_1;
frc[1][13] = frc[1][13]+xfrc13;
frc[2][13] = frc[2][13]+xfrc23;
frc[3][13] = frc[3][13]+xfrc33;
trq[1][13] = trq[1][13]+trqext_1_13_2;
trq[2][13] = trq[2][13]+trqext_2_13_2;
trq[3][13] = trq[3][13]+trqext_3_13_2;

// Number of continuation lines = 0

}
