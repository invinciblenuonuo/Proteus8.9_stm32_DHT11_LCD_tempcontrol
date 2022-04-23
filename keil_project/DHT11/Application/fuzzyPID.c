#include "fuzzyPID.h"



float DeFuzzy(int eLevel,int ecLevel,uint_8 ID_item)
{
 
		switch(ID_item)
		{
		 
				case ID_dKp:
				return fuzzyRuleKp[ecLevel+3][eLevel+3] / Gkp;
				 
				case ID_dKi:
				return fuzzyRuleKi[ecLevel+3][eLevel+3] / Gki;
				 
				case ID_dKd:
				return fuzzyRuleKd[ecLevel+3][eLevel+3] / Gkd;
				 
				default:
				return 0;
		 
		}
 
}
 
sPID Fuzzifier(float e, float ec)  //e是目标值和反馈值的误差,ec是误差变化率(误差e的微分) 
{
 
			int eLeftIndex,eRightIndex,ecLeftIndex,ecRightIndex;
			float eLeftMs,eRightMs,ecLeftMs,ecRightMs;//隶属度
			sPID fuzzyDetPID;
			 
			e /= Ge;
			ec /= Gec;
			 
			eLeftIndex = (e/levelInterval)>3.0?3:(e/levelInterval)<-3.0?-4:(e/levelInterval)>0?(int)(e/levelInterval):(int)(e/levelInterval)-1;
			eRightIndex = eLeftIndex + 1;
			 
			eLeftMs = eLeftIndex<-3?0:eLeftIndex==3?1.0:eRightIndex-e/levelInterval;
			eRightMs = eRightIndex>3?0:eRightIndex==-3?1.0:e/levelInterval-eLeftIndex;
			 
			ecLeftIndex = (ec/levelInterval)>3.0?3:(ec/levelInterval)<-3.0?-4:(ec/levelInterval)>0?(int)(ec/levelInterval):(int)(ec/levelInterval)-1;
			ecRightIndex = ecLeftIndex + 1;
			 
			ecLeftMs = ecLeftIndex<-3?0:ecLeftIndex==3?1.0:ecRightIndex-ec/levelInterval;
			ecRightMs = ecRightIndex>3?0:ecRightIndex==-3?1.0:ec/levelInterval-ecLeftIndex;
			 
			 /*************************************反模糊*************************************/
			fuzzyDetPID.Kp = (eLeftMs * ecLeftMs * DeFuzzy(eLeftIndex, ecLeftIndex, ID_dKp)
			+ eLeftMs * ecRightMs * DeFuzzy(eLeftIndex, ecRightIndex, ID_dKp)
			+ eRightMs * ecLeftMs * DeFuzzy(eRightIndex, ecLeftIndex, ID_dKp)
			+ eRightMs * ecRightMs * DeFuzzy(eRightIndex, ecRightIndex, ID_dKp));
			fuzzyDetPID.Ki = (eLeftMs * ecLeftMs * DeFuzzy(eLeftIndex, ecLeftIndex, ID_dKi)
			+ eLeftMs * ecRightMs * DeFuzzy(eLeftIndex, ecRightIndex, ID_dKi)
			+ eRightMs * ecLeftMs * DeFuzzy(eRightIndex, ecLeftIndex, ID_dKi)
			+ eRightMs * ecRightMs * DeFuzzy(eRightIndex, ecRightIndex, ID_dKi));
			fuzzyDetPID.Kd = (eLeftMs * ecLeftMs * DeFuzzy(eLeftIndex, ecLeftIndex, ID_dKd)
			+ eLeftMs * ecRightMs * DeFuzzy(eLeftIndex, ecRightIndex, ID_dKd)
			+ eRightMs * ecLeftMs * DeFuzzy(eRightIndex, ecLeftIndex, ID_dKd)
			+ eRightMs * ecRightMs * DeFuzzy(eRightIndex, ecRightIndex, ID_dKd));
			 
			return fuzzyDetPID;
 
}
 
int tarSpeed=0;
int curSpeed=0;

int PidControler()
{
		static sPID motor= {1.5, 0.5, 0};
		static int sumE = 0;
		static int lastE = 0;
 
		sPID dPID = {0, 0, 0};
		int e,ec;
 
		tarSpeed = tarSpeed>MaxMotorSpeed?MaxMotorSpeed:tarSpeed<-MaxMotorSpeed?-MaxMotorSpeed:tarSpeed;
		 
		e = tarSpeed - curSpeed;
		ec = e - lastE;
		sumE += e;
		lastE = e;
		 
		if(fabs(e/tarSpeed)<0.05)//误差小于5%
		dPID = Fuzzifier(e, ec);
		 
		return (motor.Kp+dPID.Kp)*e + (motor.Kd+dPID.Kd)*ec + (motor.Ki+dPID.Ki)*sumE;//直接返回U即为位置式
}

