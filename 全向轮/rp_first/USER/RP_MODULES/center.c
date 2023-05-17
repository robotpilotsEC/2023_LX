



/*






Ŀǰûʲô˼·���¶��Ǵ��


ң�������ƣ�
sw1
1����
3��̨ģʽ
2����ģʽ

sw2
ң����ģʽ��
1����
3ֹͣ
2����

����ģʽ��
1��
3��
2����һ��

���ֿ���
ң����ģʽ��
���� Ħ����
ƫ�� ���ո�
�м� ����ر�
���� �Ӿ�����

����ģʽ��
���� ������λ
ƫ�� ������λ
ƫ�� ������λ
���� ������λ


*/







#include "center.h"

#include "DEVICES.h"
#include "RP_CONFIG.h"

#include "chassis.h"
#include "gimbal.h"
#include "rifle.h"

#include "string.h"


void Center_ModifiyState(Center_State *self,Center_State state);
void Center_ModifiyCtrlMode(center *self,Center_CtrlMode state);
void Center_ModifiyMoveMode(center *self,Center_MoveMode state);
void Center_ModifiyRifleMode(center *self,Center_RifleMode state);

void Center_Switch(center *self);
void Center_Ctrl(center *self);


void KeyMouse_Switch(void);

center Center = {

	
	.info.SysInit = RP_NO,
	
	.info.RifleLock = RP_NO,
	
	.info.FrictionSwitch = RP_NO,
	.info.MagazineSwitch = RP_NO,
	
	.info.CtrlMode = CTRL_NULL,
	.info.MoveMode = MOVE_NULL,	
	.info.RifleMode = RIFLE_NULL,	
	.info.VisionMode = VISION_NULL,
	
	.modifyState = Center_ModifiyState,

  .modifyCtrlMode  = Center_ModifiyCtrlMode,
	.modifyMoveMode  = Center_ModifiyMoveMode,
  .modifyRifleMode = Center_ModifiyRifleMode,

	.Switch = Center_Switch,
	.Ctrl   = Center_Ctrl,
};

void Center_Switch(center *self)
{
	if(rc.info.state == RC_OFFLINE){
		
		self->info.RifleLock = RP_NO;
		
		self->info.FrictionSwitch = RP_NO;
		self->info.MagazineSwitch = RP_NO;
		
		self->modifyCtrlMode(self,CTRL_NULL);
		self->modifyMoveMode(self,MOVE_NULL);
		self->modifyRifleMode(self,RIFLE_NULL);
		
		gun.ModifyLock(&gun,RIFLE_LOCK);
		head.ModifyLock(&head,GIMB_LOCK);
		omni.ModifyLock(&omni,CHAS_LOCK);

	}
	else{
		
		/* ����ģʽ */
		switch(SW1){
		
			case SW_UP:
				self->modifyCtrlMode(self,CTRL_KM);
				break;
			
			default:
				self->modifyCtrlMode(self,CTRL_RC);
				break;		
		}
		
		
		/* ң��ģʽ */
		if(self->info.CtrlMode == CTRL_RC){
			
			/* �ƶ�ģʽ */
			switch(SW1){
			
				case SW_MID:
					self->modifyMoveMode(self,MOVE_MASTER);
					break;
				
				case SW_DOWN:
					self->modifyMoveMode(self,MOVE_FOLLOW);
					break;	
			}
			
			/* ��̨ģʽ */
			if(self->info.MoveMode == MOVE_MASTER){
				
				
			}
			/* ����ģʽ */
			else if(self->info.MoveMode == MOVE_FOLLOW){
			
				
			}
			/* �ƶ�ģʽend */
			
							
			/* ����ģʽ */			
			switch(SW2){
			
				case SW_MID:
					self->info.RifleLock = RP_OK;
					self->modifyRifleMode(self,RIFLE_STOP);
					break;
				
				case SW_UP:
					
					self->info.RifleLock = RP_OK;
					self->modifyRifleMode(self,RIFLE_STAY);
					break;		
				
				case SW_DOWN:
					self->info.RifleLock = RP_OK;
					self->modifyRifleMode(self,RIFLE_SET);
					break;				
			}
			
			if(self->info.RifleLock == RP_NO){
			
				self->modifyRifleMode(self,RIFLE_STOP);
			
			}
			/* ����ģʽend */
			
			/* ���ֿ��� */
			if(rc.data.tw_step[0]){
			
				self->info.FrictionSwitch = RP_OK;
			}
			else{
			
				self->info.FrictionSwitch = RP_NO;
			}
			
			if(rc.data.tw_step[1]){
			
				self->info.MagazineSwitch = RP_NO;
			}
			else{
			
				self->info.MagazineSwitch = RP_OK;
			}
			
			if(rc.data.tw_step[3]){
			
				self->info.FrictionSwitch = RP_OK;
			}
			else{
			
				self->info.FrictionSwitch = RP_NO;
			}
			
			if(rc.data.tw_step[2]){
			
				self->info.MagazineSwitch = RP_NO;
			}
			else{
			
				self->info.MagazineSwitch = RP_OK;
			}
			
			/* ���ֿ���end */		
		}
		/* ң��ģʽend */
		
		
		/* ����ģʽ */
		else if(self->info.CtrlMode == CTRL_KM){
		
			self->info.RifleLock = RP_OK;

			KeyMouse_Switch();
		
		}
		/* ����ģʽend */
	}
	
	/* ����ģʽend */
}



void Center_CtrlModeInit(center *self)
{
	rc.data.tw_step[0] = 0;
  rc.data.tw_step[0] = 0;
  rc.data.tw_step[0] = 0;
  rc.data.tw_step[0] = 0;
	
	
	self->info.CtrlInit = RP_OK;
}


void Center_MoveModeInit(center *self)
{
	



}

void Center_RifleModeInit(center *self)
{




}

void Center_VisionModeInit(center *self)
{




}

void Center_Ctrl(center *self)
{	
	if(self->info.SysInit == RP_NO){
	
		/*�ȴ�imu���������л����͵�kp���ڿ���ʹ��*/
		if(HAL_GetTick() > 1000){

			imu.algo.KP = IMU_PID_KP_CONTROL;
			
			self->info.SysInit = RP_OK;
		}
	}
	
/*------------------------------------------------------------------*/	
	
	
	if(self->info.MoveMode == MOVE_MASTER){
	
		head.Updata(&head,
		            imu.data.rpy.roll,
		            imu.data.rpy.pitch,
		            imu.data.rpy.yaw,
		            imu.data.worldGyr.x,
		            imu.data.worldGyr.y,
		            imu.data.worldGyr.z);	
	}
	else if(self->info.MoveMode == MOVE_FOLLOW){
		
		head.Updata(&head,
		            0,
		            motor[GIMB_P].rx_info.angle,
		            motor[GIMB_Y].rx_info.angle,
		            imu.data.acc_gyr.gyr_x,
		            imu.data.acc_gyr.gyr_y,
		            imu.data.acc_gyr.gyr_z);	
	}
	
	omni.Updata(&omni);
	gun.Updata(&gun);
	
/*---------------------------------------------------------------*/	
	
	head.Resolving(&head);
	omni.Resolving(&omni);
	
	if(self->info.MoveMode == MOVE_MASTER){
		
		head.Translation(&head,
										 master[M1].data.chasRPY.x,
										 master[M1].data.chasRPY.y,
										 master[M1].data.chasRPY.z);
	}	

/*---------------------------------------------------------------*/	
	
	head.Ctrl(&head);
	omni.Ctrl(&omni);
	
	gun.FrictionCtrl(&gun);
	gun.MagazineCtrl(&gun);
	gun.BoxCtrl(&gun);
	
	
}


void KeyMouse_Switch(void)
{







}







/*------------------------------------------------------------------*/	
/*------------------------------------------------------------------*/	


/*
ģʽ�л�

�Ƚϴ�ʱ��ģʽ������
�����ͬ�򷵻�
�����ͬ����δ���г�ʼ���������ʼ����״̬

�����ʱ���ڳ�ʼ��״̬������ʼ������
��ʼ����ɺ�״̬��Ϊ����ɳ�ʼ��
��ʼ����ɱ�־�������޸Ĵ�ʱ��ģʽ

*/
void Center_ModifiyState(Center_State *self,Center_State state)
{
	if(*self != state){
	
		*self = state;
	}
}



void Center_ModifiyCtrlMode(center *self,Center_CtrlMode state)
{
	if(self->info.CtrlMode == state)return;

	if(self->info.CtrlInit == RP_ING){
	
		Center_CtrlModeInit(self);
		
		if(self->info.CtrlInit == RP_OK){
		
			self->info.CtrlMode = state;
		}
	}	
	
	if(self->info.CtrlMode != state){
	
		self->info.CtrlMode = CTRL_NULL;
		self->modifyState(&Center.info.CtrlInit, RP_ING);
	}
}

void Center_ModifiyMoveMode(center *self,Center_MoveMode state)
{
	if(self->info.MoveMode == state)return;

	if(self->info.MoveInit == RP_ING){
	
		Center_MoveModeInit(self);

		if(self->info.MoveInit == RP_OK){
		
			self->info.MoveMode = state;
		}		
	}
	
	if(self->info.MoveMode != state){
	
		self->info.MoveMode = MOVE_NULL;
		self->modifyState(&Center.info.MoveInit, RP_ING);		
	}
}

void Center_ModifiyRifleMode(center *self,Center_RifleMode state)
{
	if(self->info.RifleMode == state)return;

	if(self->info.RifleInit == RP_ING){
	
		Center_RifleModeInit(self);

		if(self->info.RifleInit == RP_OK){
		
			self->info.RifleMode = state;
		}				
	}		
	
	if(self->info.RifleMode != state){
	
		self->info.RifleMode = RIFLE_NULL;
		self->modifyState(&Center.info.RifleInit, RP_ING);		
	}
}

void Center_ModifiyVisionMode(center *self,Center_VisionMode state)
{
	if(self->info.VisionMode == state)return;

	if(self->info.VisionInit == RP_ING){
	
		Center_VisionModeInit(self);

		if(self->info.VisionInit == RP_OK){
		
			self->info.VisionMode = state;
		}				
	}		
	
	if(self->info.VisionMode != state){
	
		self->info.VisionMode = VISION_NULL;
		self->modifyState(&Center.info.VisionInit, RP_ING);		
	}
}
















