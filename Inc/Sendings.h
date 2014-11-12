#include "stm32l0xx.h"

extern void send_str(uint8_t string[], uint8_t lenghth);
extern void Send_HRM_Settings(void);
extern void Send_HRM_Error(uint8_t error_code);
extern void Send_HRM_Config(void);
extern void Send_HRM_Accel(void);
extern void Send_HRM_Gyro(void);
extern void Send_Server_SOS(void);

extern uint8_t Accel_Buf[900];
extern uint8_t Gyro_Buf[900];
extern uint16_t Accel_to_Send;
extern uint16_t Gyro_to_Send;
extern uint8_t SOS_Buf[9];
extern uint8_t OEM_Settings[11];
