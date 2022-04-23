#ifndef _FUZZY_PID_H
#define _FUZZY_PID_H

#include "math.h"


#define ID_dKp 1
#define ID_dKi 2
#define ID_dKd 3

#define uint_8	unsigned char
//3  0.2   0.5
static const float MaxMotorSpeed=500.0;
static const float Ge = 1.0;
static const float Gec = 1.0;
static const float Gkp = 50.0;//3->15/50=0.3
static const float Gki = 1000.0;//0.2->15/1000=0.03
static const float Gkd = 50.0;//0.0->15/50=0.3比例因子
 
static const float levelInterval = 5;//论域的分级间隔必须相同，此处设置为5
static const float NL = -3 * levelInterval;
static const float NM= -2 * levelInterval;
static const float NS= -1 * levelInterval;

static const float ZE= 0;

static const float PS= 1 * levelInterval;
static const float PM= 2 * levelInterval;
static const float PL= 3 * levelInterval;
 
/************************************
dKp模糊控制规则表
dKpeNLNMNSZEPSPMPL
ec
NLPLPLPMPMPSPSZE
NMPLPLPMPMPSZEZE
NSPMPMPMPSZENSNM
ZEPMPSPSZENSNMNM
PSPSPSZENSNSNMNM
PMZEZENSNMNMNMNL
PLZENSNSNMNMNLNL
************************************
dKi模糊控制规则表
dKieNLNMNSZEPSPMPL
ec
NLNLNLNLNMNMZEZE
NMNLNLNMNMNSZEZE
NSNMNMNSNSZEPSPS
ZENMNSNSZEPSPSPM
PSNSNSZEPSPSPMPM
PMZEZEPSPMPMPLPL
PLZEZEPSPMPLPLPL
************************************
dKd模糊控制规则表
dKdeNLNMNSZEPSPMPL
ec
NLPSPSZEZEZEPLPL
NMNSNSNSNSZENSPM
NSNLNLNMNSZEPSPM
ZENLNMNMNSZEPSPM
PSNLNMNSNSZEPSPS
PMNMNSNSNSZEPSPS
PLPSZEZEZEZEPLPL
************************************/
 
static const float fuzzyRuleKp[7][7]={
 
PL,PL,PM,PM,PS,PS,ZE,
PL,PL,PM,PM,PS,ZE,ZE,
PM,PM,PM,PS,ZE,NS,NM,
PM,PS,PS,ZE,NS,NM,NM,
PS,PS,ZE,NS,NS,NM,NM,
ZE,ZE,NS,NM,NM,NM,NL,
ZE,NS,NS,NM,NM,NL,NL
 
};//dKp模糊规则表
 
static const float fuzzyRuleKi[7][7]={
 
NL,NL,NL,NM,NM,ZE,ZE,
NL,NL,NM,NM,NS,ZE,ZE,
NM,NM,NS,NS,ZE,PS,PS,
NM,NS,NS,ZE,PS,PS,PM,
NS,NS,ZE,PS,PS,PM,PM,
ZE,ZE,PS,PM,PM,PL,PL,
ZE,ZE,PS,PM,PL,PL,PL
 
};//dKi模糊规则表
 
static const float fuzzyRuleKd[7][7]={
 
PS,PS,ZE,ZE,ZE,PL,PL,
NS,NS,NS,NS,ZE,NS,PM,
NL,NL,NM,NS,ZE,PS,PM,
NL,NM,NM,NS,ZE,PS,PM,
NL,NM,NS,NS,ZE,PS,PS,
NM,NS,NS,NS,ZE,PS,PS,
PS,ZE,ZE,ZE,ZE,PL,PL
 
};//dKd模糊规则表
 
typedef struct {
 
float Kp;
float Ki;
float Kd;
 
}sPID;
 
int PidControler(void );
 
#endif













