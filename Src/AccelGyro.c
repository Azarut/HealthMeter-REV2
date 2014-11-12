#include "AccelGyro.h"
#include "Sendings.h"
uint16_t ACCEL_cnt = 0;
uint16_t GYRO_cnt = 0;
void ReadAccel(void)
{ 
	static uint16_t i = 0; 
  Accel_Buf[i] = ACCEL_cnt;
	i++;
	ACCEL_cnt++;
	if(i >= 900)
	{
		Accel_to_Send = 900;
		i = 0;
	}
	if(Accel_to_Send != 900) Accel_to_Send = i;
}

void ReadGyro(void)
{ 
	static uint16_t i = 0;
	Gyro_Buf[i] = GYRO_cnt;
	GYRO_cnt++;
	i++;
	if(i >= 900)
	{
		Gyro_to_Send = 900;
		i = 0;
	}
	if(Gyro_to_Send != 900) Gyro_to_Send = i;
}

void SetAccel(uint8_t G)
{

}

void SetGyro(uint16_t V)
{

}
