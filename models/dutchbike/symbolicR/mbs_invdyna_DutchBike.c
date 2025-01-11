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
//	==> Function: F2 - Recursive Inverse Dynamics of tree-like MBS
//
//	==> Git hash: 8f46effdc05c898000a15b4c4dfc8f70efce4fc0
//

#include <math.h> 

#include "mbs_data.h"

 
void mbs_invdyna(double *Qq,
MbsData *s, double tsim)
{
#include "mbs_invdyna_DutchBike.h"

double *q, *qd, *qdd;
double *g, *m;
double **l, **In, **dpt, **frc, **trq;

q = s->q;
qd = s->qd;
qdd = s->qdd;

dpt = s->dpt;
l   = s->l;

m = s->m;
In  = s->In;

frc = s->frc;
trq = s->trq;
g = s->g;
 
// Trigonometric functions

S4 = sin(q[4]);
C4 = cos(q[4]);
S5 = sin(q[5]);
C5 = cos(q[5]);
S6 = sin(q[6]);
C6 = cos(q[6]);
S9 = sin(q[9]);
C9 = cos(q[9]);
S10 = sin(q[10]);
C10 = cos(q[10]);
S11 = sin(q[11]);
C11 = cos(q[11]);
S12 = sin(q[12]);
C12 = cos(q[12]);
S13 = sin(q[13]);
C13 = cos(q[13]);
S14 = sin(q[14]);
C14 = cos(q[14]);
S15 = sin(q[15]);
C15 = cos(q[15]);
S16 = sin(q[16]);
C16 = cos(q[16]);
S17 = sin(q[17]);
C17 = cos(q[17]);
S18 = sin(q[18]);
C18 = cos(q[18]);
S19 = sin(q[19]);
C19 = cos(q[19]);
S20 = sin(q[20]);
C20 = cos(q[20]);
S21 = sin(q[21]);
C21 = cos(q[21]);
S22 = sin(q[22]);
C22 = cos(q[22]);
S23 = sin(q[23]);
C23 = cos(q[23]);
S24 = sin(q[24]);
C24 = cos(q[24]);
S25 = sin(q[25]);
C25 = cos(q[25]);
S26 = sin(q[26]);
C26 = cos(q[26]);
S27 = sin(q[27]);
C27 = cos(q[27]);
S28 = sin(q[28]);
C28 = cos(q[28]);
S29 = sin(q[29]);
C29 = cos(q[29]);
S30 = sin(q[30]);
C30 = cos(q[30]);
S31 = sin(q[31]);
C31 = cos(q[31]);
S32 = sin(q[32]);
C32 = cos(q[32]);
 
// Augmented Joint Position Vectors

 
// Forward Kinematics

ALPHA33 = qdd[3]-g[3];
ALPHA14 = qdd[1]*C4+qdd[2]*S4;
ALPHA24 = -qdd[1]*S4+qdd[2]*C4;
OM25 = qd[4]*S5;
OM35 = qd[4]*C5;
OMp25 = qd[4]*qd[5]*C5+qdd[4]*S5;
OMp35 = -qd[4]*qd[5]*S5+qdd[4]*C5;
ALPHA25 = ALPHA24*C5+ALPHA33*S5;
ALPHA35 = -ALPHA24*S5+ALPHA33*C5;
OM16 = qd[5]*C6-OM35*S6;
OM26 = qd[6]+OM25;
OM36 = qd[5]*S6+OM35*C6;
OMp16 = C6*(qdd[5]-qd[6]*OM35)-S6*(OMp35+qd[5]*qd[6]);
OMp26 = qdd[6]+OMp25;
OMp36 = C6*(OMp35+qd[5]*qd[6])+S6*(qdd[5]-qd[6]*OM35);
BS16 = -OM26*OM26-OM36*OM36;
BS26 = OM16*OM26;
BS36 = OM16*OM36;
BS66 = OM26*OM36;
BS96 = -OM16*OM16-OM26*OM26;
BETA36 = BS36+OMp26;
BETA46 = BS26+OMp36;
BETA66 = BS66-OMp16;
BETA76 = BS36-OMp26;
ALPHA16 = ALPHA14*C6-ALPHA35*S6;
ALPHA36 = ALPHA14*S6+ALPHA35*C6;
BS37 = OM16*OM36;
BS67 = OM26*OM36;
BS97 = -OM16*OM16-OM26*OM26;
BETA37 = BS37+OMp26;
BETA67 = BS67-OMp16;
ALPHA17 = qdd[7]+ALPHA16+q[7]*BS16;
ALPHA27 = ALPHA25+q[7]*BETA46+(2.0)*qd[7]*OM36;
ALPHA37 = ALPHA36+q[7]*BETA76-(2.0)*qd[7]*OM26;
ALPHA18 = ALPHA17+q[8]*BETA37+(2.0)*qd[8]*OM26;
ALPHA28 = ALPHA27+q[8]*BETA67-(2.0)*qd[8]*OM16;
ALPHA38 = qdd[8]+ALPHA37+q[8]*BS97;
ALPHA19 = ALPHA18*C9-ALPHA38*S9;
ALPHA39 = ALPHA18*S9+ALPHA38*C9;
OM110 = OM16*C10-OM36*S10;
OM210 = qd[10]+OM26;
OM310 = OM16*S10+OM36*C10;
OMp110 = C10*(OMp16-qd[10]*OM36)-S10*(OMp36+qd[10]*OM16);
OMp210 = qdd[10]+OMp26;
OMp310 = C10*(OMp36+qd[10]*OM16)+S10*(OMp16-qd[10]*OM36);
ALPHA110 = C10*(ALPHA16+BETA36*dpt[3][1]+BS16*dpt[1][1])-S10*(ALPHA36+BETA76*dpt[1][1]+BS96*dpt[3][1]);
ALPHA210 = ALPHA25+BETA46*dpt[1][1]+BETA66*dpt[3][1];
ALPHA310 = C10*(ALPHA36+BETA76*dpt[1][1]+BS96*dpt[3][1])+S10*(ALPHA16+BETA36*dpt[3][1]+BS16*dpt[1][1]);
OM111 = OM110*C11+OM210*S11;
OM211 = -OM110*S11+OM210*C11;
OM311 = qd[11]+OM310;
OMp111 = C11*(OMp110+qd[11]*OM210)+S11*(OMp210-qd[11]*OM110);
OMp211 = C11*(OMp210-qd[11]*OM110)-S11*(OMp110+qd[11]*OM210);
OMp311 = qdd[11]+OMp310;
BS111 = -OM211*OM211-OM311*OM311;
BS211 = OM111*OM211;
BS311 = OM111*OM311;
BS611 = OM211*OM311;
BS911 = -OM111*OM111-OM211*OM211;
BETA311 = BS311+OMp211;
BETA411 = BS211+OMp311;
BETA611 = BS611-OMp111;
BETA711 = BS311-OMp211;
ALPHA111 = ALPHA110*C11+ALPHA210*S11;
ALPHA211 = -ALPHA110*S11+ALPHA210*C11;
OM112 = OM111*C12-OM311*S12;
OM212 = qd[12]+OM211;
OM312 = OM111*S12+OM311*C12;
OMp112 = C12*(OMp111-qd[12]*OM311)-S12*(OMp311+qd[12]*OM111);
OMp212 = qdd[12]+OMp211;
OMp312 = C12*(OMp311+qd[12]*OM111)+S12*(OMp111-qd[12]*OM311);
ALPHA112 = C12*(ALPHA111+BETA311*dpt[3][5]+BS111*dpt[1][5])-S12*(ALPHA310+BETA711*dpt[1][5]+BS911*dpt[3][5]);
ALPHA212 = ALPHA211+BETA411*dpt[1][5]+BETA611*dpt[3][5];
ALPHA312 = C12*(ALPHA310+BETA711*dpt[1][5]+BS911*dpt[3][5])+S12*(ALPHA111+BETA311*dpt[3][5]+BS111*dpt[1][5]);
OM113 = OM16*C13-OM36*S13;
OM213 = qd[13]+OM26;
OM313 = OM16*S13+OM36*C13;
OMp113 = C13*(OMp16-qd[13]*OM36)-S13*(OMp36+qd[13]*OM16);
OMp213 = qdd[13]+OMp26;
OMp313 = C13*(OMp36+qd[13]*OM16)+S13*(OMp16-qd[13]*OM36);
ALPHA113 = C13*(ALPHA16+BETA36*dpt[3][2]+BS16*dpt[1][2])-S13*(ALPHA36+BETA76*dpt[1][2]+BS96*dpt[3][2]);
ALPHA213 = ALPHA25+BETA46*dpt[1][2]+BETA66*dpt[3][2];
ALPHA313 = C13*(ALPHA36+BETA76*dpt[1][2]+BS96*dpt[3][2])+S13*(ALPHA16+BETA36*dpt[3][2]+BS16*dpt[1][2]);
 
// Backward Dynamics

Fq131 = -frc[1][31]-frc[1][32]*C32-frc[3][32]*S32;
Fq231 = -frc[2][31]-frc[2][32];
Fq331 = -frc[3][31]+frc[1][32]*S32-frc[3][32]*C32;
Cq131 = -trq[1][31]+frc[2][32]*dpt[3][21]-trq[1][32]*C32-trq[3][32]*S32;
Cq231 = -trq[2][31]-trq[2][32]+dpt[3][21]*(-frc[1][32]*C32-frc[3][32]*S32);
Cq331 = -trq[3][31]+trq[1][32]*S32-trq[3][32]*C32;
Fq130 = -frc[1][30]+Fq131*C31+Fq331*S31;
Fq230 = -frc[2][30]+Fq231;
Fq330 = -frc[3][30]-Fq131*S31+Fq331*C31;
Cq130 = -trq[1][30]+Cq131*C31+Cq331*S31-Fq231*dpt[3][20]+dpt[2][20]*(-Fq131*S31+Fq331*C31);
Cq230 = -trq[2][30]+Cq231+dpt[3][20]*(Fq131*C31+Fq331*S31);
Cq330 = -trq[3][30]-Cq131*S31+Cq331*C31-dpt[2][20]*(Fq131*C31+Fq331*S31);
Fq128 = -frc[1][28]-frc[1][29]*C29-frc[3][29]*S29;
Fq228 = -frc[2][28]-frc[2][29];
Fq328 = -frc[3][28]+frc[1][29]*S29-frc[3][29]*C29;
Cq128 = -trq[1][28]+frc[2][29]*dpt[3][19]-trq[1][29]*C29-trq[3][29]*S29;
Cq228 = -trq[2][28]-trq[2][29]+dpt[3][19]*(-frc[1][29]*C29-frc[3][29]*S29);
Cq328 = -trq[3][28]+trq[1][29]*S29-trq[3][29]*C29;
Fq127 = -frc[1][27]+Fq128*C28+Fq328*S28;
Fq227 = -frc[2][27]+Fq228;
Fq327 = -frc[3][27]-Fq128*S28+Fq328*C28;
Cq127 = -trq[1][27]+Cq128*C28+Cq328*S28-Fq228*dpt[3][18]+dpt[2][18]*(-Fq128*S28+Fq328*C28);
Cq227 = -trq[2][27]+Cq228+dpt[3][18]*(Fq128*C28+Fq328*S28);
Cq327 = -trq[3][27]-Cq128*S28+Cq328*C28-dpt[2][18]*(Fq128*C28+Fq328*S28);
Fq124 = -frc[1][24]-frc[1][25];
Fq224 = -frc[2][24]-frc[2][25]*C25+frc[3][25]*S25;
Fq324 = -frc[3][24]-frc[2][25]*S25-frc[3][25]*C25;
Cq124 = -trq[1][24]-trq[1][25]-dpt[3][17]*(-frc[2][25]*C25+frc[3][25]*S25);
Cq224 = -trq[2][24]-frc[1][25]*dpt[3][17]-trq[2][25]*C25+trq[3][25]*S25;
Cq324 = -trq[3][24]-trq[2][25]*S25-trq[3][25]*C25;
Fq123 = -frc[1][23]+Fq124*C24+Fq324*S24;
Fq223 = -frc[2][23]+Fq224;
Fq323 = -frc[3][23]-Fq124*S24+Fq324*C24;
Cq123 = -trq[1][23]+Cq124*C24+Cq324*S24-Fq224*dpt[3][16];
Cq223 = -trq[2][23]+Cq224+dpt[3][16]*(Fq124*C24+Fq324*S24);
Cq323 = -trq[3][23]-Cq124*S24+Cq324*C24;
Fq122 = Fq123*C23-Fq223*S23;
Fq222 = Fq123*S23+Fq223*C23;
Cq122 = Cq123*C23-Cq223*S23;
Cq222 = Cq123*S23+Cq223*C23;
Fq121 = Fq122*C22+Fq323*S22;
Fq321 = -Fq122*S22+Fq323*C22;
Cq121 = Cq122*C22+Cq323*S22;
Cq321 = -Cq122*S22+Cq323*C22;
Fq119 = -frc[1][19]-frc[1][20];
Fq219 = -frc[2][19]-frc[2][20]*C20+frc[3][20]*S20;
Fq319 = -frc[3][19]-frc[2][20]*S20-frc[3][20]*C20;
Cq119 = -trq[1][19]-trq[1][20]-dpt[3][15]*(-frc[2][20]*C20+frc[3][20]*S20);
Cq219 = -trq[2][19]-frc[1][20]*dpt[3][15]-trq[2][20]*C20+trq[3][20]*S20;
Cq319 = -trq[3][19]-trq[2][20]*S20-trq[3][20]*C20;
Fq118 = -frc[1][18]+Fq119*C19+Fq319*S19;
Fq218 = -frc[2][18]+Fq219;
Fq318 = -frc[3][18]-Fq119*S19+Fq319*C19;
Cq118 = -trq[1][18]+Cq119*C19+Cq319*S19-Fq219*dpt[3][14];
Cq218 = -trq[2][18]+Cq219+dpt[3][14]*(Fq119*C19+Fq319*S19);
Cq318 = -trq[3][18]-Cq119*S19+Cq319*C19;
Fq117 = Fq118*C18-Fq218*S18;
Fq217 = Fq118*S18+Fq218*C18;
Cq117 = Cq118*C18-Cq218*S18;
Cq217 = Cq118*S18+Cq218*C18;
Fq116 = Fq117*C17+Fq318*S17;
Fq316 = -Fq117*S17+Fq318*C17;
Cq116 = Cq117*C17+Cq318*S17;
Cq316 = -Cq117*S17+Cq318*C17;
Fq115 = -frc[1][15]+Fq116+Fq121-frc[1][26]*C26-frc[3][26]*S26;
Fq215 = -frc[2][15]-frc[2][26]+Fq217*C16+Fq222*C21-Fq316*S16-Fq321*S21;
Fq315 = -frc[3][15]+frc[1][26]*S26-frc[3][26]*C26+Fq217*S16+Fq222*S21+Fq316*C16+Fq321*C21;
Cq115 = -trq[1][15]+Cq116+Cq121+frc[2][26]*dpt[3][13]-trq[1][26]*C26-trq[3][26]*S26+dpt[2][11]*(Fq217*S16+Fq316*C16)+
 dpt[2][12]*(Fq222*S21+Fq321*C21)-dpt[3][11]*(Fq217*C16-Fq316*S16)-dpt[3][12]*(Fq222*C21-Fq321*S21);
Cq215 = -trq[2][15]-trq[2][26]+Cq217*C16+Cq222*C21-Cq316*S16-Cq321*S21+Fq116*dpt[3][11]+Fq121*dpt[3][12]+dpt[3][13]*(-
 frc[1][26]*C26-frc[3][26]*S26);
Cq315 = -trq[3][15]+trq[1][26]*S26-trq[3][26]*C26+Cq217*S16+Cq222*S21+Cq316*C16+Cq321*C21-Fq116*dpt[2][11]-Fq121*
 dpt[2][12];
Fq114 = -frc[1][14]+Fq115*C15+Fq127*C27+Fq130*C30+Fq315*S15+Fq327*S27+Fq330*S30;
Fq214 = -frc[2][14]+Fq215+Fq227+Fq230;
Fq314 = -frc[3][14]-Fq115*S15-Fq127*S27-Fq130*S30+Fq315*C15+Fq327*C27+Fq330*C30;
Cq114 = -trq[1][14]+Cq115*C15+Cq127*C27+Cq130*C30+Cq315*S15+Cq327*S27+Cq330*S30-Fq215*dpt[3][8]-Fq227*dpt[3][9]-Fq230*
 dpt[3][10]+dpt[2][10]*(-Fq130*S30+Fq330*C30)+dpt[2][9]*(-Fq127*S27+Fq327*C27);
Cq214 = -trq[2][14]+Cq215+Cq227+Cq230+dpt[3][10]*(Fq130*C30+Fq330*S30)+dpt[3][8]*(Fq115*C15+Fq315*S15)+dpt[3][9]*(
 Fq127*C27+Fq327*S27);
Cq314 = -trq[3][14]-Cq115*S15-Cq127*S27-Cq130*S30+Cq315*C15+Cq327*C27+Cq330*C30-dpt[2][10]*(Fq130*C30+Fq330*S30)-
 dpt[2][9]*(Fq127*C27+Fq327*S27);
Fs113 = -frc[1][13]+m[13]*ALPHA113;
Fs213 = -frc[2][13]+m[13]*ALPHA213;
Fs313 = -frc[3][13]+m[13]*ALPHA313;
Cq113 = -trq[1][13]+In[1][13]*OMp113-In[5][13]*OM213*OM313+In[9][13]*OM213*OM313;
Cq213 = -trq[2][13]+In[1][13]*OM113*OM313+In[5][13]*OMp213-In[9][13]*OM113*OM313;
Cq313 = -trq[3][13]-In[1][13]*OM113*OM213+In[5][13]*OM113*OM213+In[9][13]*OMp313;
Fs112 = -frc[1][12]+m[12]*ALPHA112;
Fs212 = -frc[2][12]+m[12]*ALPHA212;
Fs312 = -frc[3][12]+m[12]*ALPHA312;
Cq112 = -trq[1][12]+In[1][12]*OMp112-In[5][12]*OM212*OM312+In[9][12]*OM212*OM312;
Cq212 = -trq[2][12]+In[1][12]*OM112*OM312+In[5][12]*OMp212-In[9][12]*OM112*OM312;
Cq312 = -trq[3][12]-In[1][12]*OM112*OM212+In[5][12]*OM112*OM212+In[9][12]*OMp312;
Fs111 = -frc[1][11]+m[11]*(ALPHA111+BETA311*l[3][11]+BS111*l[1][11]);
Fs211 = -frc[2][11]+m[11]*(ALPHA211+BETA411*l[1][11]+BETA611*l[3][11]);
Fs311 = -frc[3][11]+m[11]*(ALPHA310+BETA711*l[1][11]+BS911*l[3][11]);
Fq111 = Fs111+Fs112*C12+Fs312*S12;
Fq211 = Fs211+Fs212;
Fq311 = Fs311-Fs112*S12+Fs312*C12;
Cq111 = -trq[1][11]+In[1][11]*OMp111+In[3][11]*OMp311-In[5][11]*OM211*OM311+Cq112*C12+Cq312*S12-Fs211*l[3][11]-Fs212*
 dpt[3][5]+OM211*(In[3][11]*OM111+In[9][11]*OM311);
Cq211 = -trq[2][11]+Cq212+In[5][11]*OMp211+Fs111*l[3][11]-Fs311*l[1][11]-OM111*(In[3][11]*OM111+In[9][11]*OM311)+OM311
 *(In[1][11]*OM111+In[3][11]*OM311)-dpt[1][5]*(-Fs112*S12+Fs312*C12)+dpt[3][5]*(Fs112*C12+Fs312*S12);
Cq311 = -trq[3][11]+In[3][11]*OMp111+In[5][11]*OM111*OM211+In[9][11]*OMp311-Cq112*S12+Cq312*C12+Fs211*l[1][11]+Fs212*
 dpt[1][5]-OM211*(In[1][11]*OM111+In[3][11]*OM311);
Fq110 = Fq111*C11-Fq211*S11;
Fq210 = Fq111*S11+Fq211*C11;
Cq110 = Cq111*C11-Cq211*S11;
Cq210 = Cq111*S11+Cq211*C11;
Fs19 = -frc[1][9]+m[9]*ALPHA19;
Fs29 = -frc[2][9]+m[9]*ALPHA28;
Fs39 = -frc[3][9]+m[9]*ALPHA39;
Fq18 = Fs19*C9+Fs39*S9;
Fq38 = -Fs19*S9+Fs39*C9;
Cq18 = -trq[1][9]*C9-trq[3][9]*S9;
Cq38 = trq[1][9]*S9-trq[3][9]*C9;
Cq17 = Cq18-q[8]*Fs29;
Cq27 = -trq[2][9]+q[8]*Fq18;
Fs16 = -frc[1][6]+m[6]*(ALPHA16+BETA36*l[3][6]+BS16*l[1][6]);
Fs26 = -frc[2][6]+m[6]*(ALPHA25+BETA46*l[1][6]+BETA66*l[3][6]);
Fs36 = -frc[3][6]+m[6]*(ALPHA36+BETA76*l[1][6]+BS96*l[3][6]);
Fq16 = Fq18+Fs16+Fq110*C10+Fq114*C14+Fq311*S10+Fq314*S14+Fs113*C13+Fs313*S13;
Fq26 = Fq210+Fq214+Fs213+Fs26+Fs29;
Fq36 = Fq38+Fs36-Fq110*S10-Fq114*S14+Fq311*C10+Fq314*C14-Fs113*S13+Fs313*C13;
Cq16 = -trq[1][6]+Cq17+In[1][6]*OMp16+In[3][6]*OMp36-In[5][6]*OM26*OM36+Cq110*C10+Cq113*C13+Cq114*C14+Cq311*S10+Cq313*
 S13+Cq314*S14-Fq210*dpt[3][1]-Fq214*dpt[3][3]-Fs213*dpt[3][2]-Fs26*l[3][6]+OM26*(In[3][6]*OM16+In[9][6]*OM36);
Cq26 = -trq[2][6]+Cq210+Cq213+Cq214+Cq27-q[7]*Fq38+In[5][6]*OMp26+Fs16*l[3][6]-Fs36*l[1][6]-OM16*(In[3][6]*OM16+
 In[9][6]*OM36)+OM36*(In[1][6]*OM16+In[3][6]*OM36)-dpt[1][1]*(-Fq110*S10+Fq311*C10)-dpt[1][2]*(-Fs113*S13+Fs313*C13)-
 dpt[1][3]*(-Fq114*S14+Fq314*C14)+dpt[3][1]*(Fq110*C10+Fq311*S10)+dpt[3][2]*(Fs113*C13+Fs313*S13)+dpt[3][3]*(Fq114*C14+Fq314*
 S14);
Cq36 = -trq[3][6]+Cq38+q[7]*Fs29+In[3][6]*OMp16+In[5][6]*OM16*OM26+In[9][6]*OMp36-Cq110*S10-Cq113*S13-Cq114*S14+Cq311*
 C10+Cq313*C13+Cq314*C14+Fq210*dpt[1][1]+Fq214*dpt[1][3]+Fs213*dpt[1][2]+Fs26*l[1][6]-OM26*(In[1][6]*OM16+In[3][6]*OM36);
Fq15 = Fq16*C6+Fq36*S6;
Fq35 = -Fq16*S6+Fq36*C6;
Cq15 = Cq16*C6+Cq36*S6;
Cq35 = -Cq16*S6+Cq36*C6;
Fq24 = Fq26*C5-Fq35*S5;
Fq34 = Fq26*S5+Fq35*C5;
Cq34 = Cq26*S5+Cq35*C5;
Fq13 = Fq15*C4-Fq24*S4;
Fq23 = Fq15*S4+Fq24*C4;
 
// Symbolic model output

Qq[1] = Fq13;
Qq[2] = Fq23;
Qq[3] = Fq34;
Qq[4] = Cq34;
Qq[5] = Cq15;
Qq[6] = Cq26;
Qq[7] = Fq18;
Qq[8] = Fq38;
Qq[9] = -trq[2][9];
Qq[10] = Cq210;
Qq[11] = Cq311;
Qq[12] = Cq212;
Qq[13] = Cq213;
Qq[14] = Cq214;
Qq[15] = Cq215;
Qq[16] = Cq116;
Qq[17] = Cq217;
Qq[18] = Cq318;
Qq[19] = Cq219;
Qq[20] = -trq[1][20];
Qq[21] = Cq121;
Qq[22] = Cq222;
Qq[23] = Cq323;
Qq[24] = Cq224;
Qq[25] = -trq[1][25];
Qq[26] = -trq[2][26];
Qq[27] = Cq227;
Qq[28] = Cq228;
Qq[29] = -trq[2][29];
Qq[30] = Cq230;
Qq[31] = Cq231;
Qq[32] = -trq[2][32];

// Number of continuation lines = 3

}
