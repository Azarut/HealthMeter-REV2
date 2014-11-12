#include <cmsis_os.h> 
#include "Sendings.h"
#include "init.h"
#include "main.h"

uint8_t OEM_Settings[11] = {'S', 16, 0x07, 0xD0, 0x03, 0xE8, 0x03, 0xE8, 10, 0x55, 0x55};
uint8_t OEM_Error[2] = {'E', 0};
uint8_t OEM_Config[2] = {'C', 0x01};
uint8_t Send_Buf[904] = {0};
uint8_t Accel_Buf[900] = {0};
uint8_t Gyro_Buf[900] = {0};
uint8_t SOS_Buf[9] = {0};
uint16_t CRC_value = 0;
uint16_t Accel_to_Send = 50;
uint16_t Gyro_to_Send = 15;



/*    АТ-комманды SIM800       */
uint8_t ECHO[5] = "ATE0\r"; 
uint8_t SET_SMS_OP_1[10] = "AT+CMGF=1\r"; 
uint8_t SET_SMS_OP_2[15] = "AT+CSCS= \"GSM\"\r"; 
uint8_t READ_SMS[10] = "AT+CMGR=1\r"; 
uint8_t DEL_SMS[19] = "AT+CMGDA=\"DEL ALL\"\r";
uint8_t APN[19] = "AT+CSTT=\"internet\"\r"; 
uint8_t CHK_CONNECT[9] =  "AT+CIICR\r";
uint8_t CHK_IP[9] = "AT+CIFSR\r";
uint8_t SRV_CONNECT[42] = "AT+CIPSTART=\"TCP\",\"82.204.241.138\",\"8081\"\r";
uint8_t SEND_DATA[11] = "AT+CIPSEND\r";
uint8_t CHK_GPRS[10] = "AT+CGATT?\r";
uint8_t GPRS_CONNECT[11] = "AT+CGATT=1\r";
uint8_t END_LINE[1] = {0x1A};
/*******************************/

void Init_SIM800(void)
{
					
			osDelay(7000);
		  send_str(ECHO, 5);
		  osDelay(1000);
		  send_str(SET_SMS_OP_1, 10);
		  osDelay(1000);
		  send_str(SET_SMS_OP_2, 15);;
		  osDelay(1000);
		  send_str(DEL_SMS, 19);
		  osDelay(1000);
		  send_str(CHK_GPRS, 10);
		  osDelay(1000);
	    send_str(GPRS_CONNECT, 11);
		  osDelay(1000);
      send_str(APN, 19);
		  osDelay(1000);
	    send_str(CHK_CONNECT, 9);
		  osDelay(1000);
	    send_str(CHK_IP, 9);
		  osDelay(1000);
	    send_str(SRV_CONNECT, 42);
		  osDelay(3000); 
}

void send_str(uint8_t string[], uint8_t lenghth) 
{
	  DMA1_Channel2->CMAR = (uint32_t)string; /* (3) */
		DMA1_Channel2->CCR &=~ DMA_CCR_EN;
    DMA1_Channel2->CNDTR = lenghth;/* Data size */
    DMA1_Channel2->CCR |= DMA_CCR_EN;
	  DMA1_Channel3->CCR &=~ DMA_CCR_EN;
    DMA1_Channel3->CNDTR = 32;/* Data size */
    DMA1_Channel3->CCR |= DMA_CCR_EN;
}

void Send_HRM_Settings(void) 
{
		CRC_value = crc16(OEM_Settings, 9);
	  OEM_Settings[9] = (uint8_t)(CRC_value >> 0x08);
	  OEM_Settings[10] = (uint8_t)CRC_value;
		DMA1_Channel4->CMAR = (uint32_t)OEM_Settings; /* (3) */
		DMA1_Channel4->CCR &=~ DMA_CCR_EN;
    DMA1_Channel4->CNDTR = 11;/* Data size */
    DMA1_Channel4->CCR |= DMA_CCR_EN;
	  DMA1_Channel5->CCR &=~ DMA_CCR_EN;
    DMA1_Channel5->CNDTR = 16;/* Data size */
    DMA1_Channel5->CCR |= DMA_CCR_EN;
}

void Send_HRM_Error(uint8_t error_code) 
{
		OEM_Error[1] = error_code;
	  DMA1_Channel4->CMAR = (uint32_t)OEM_Error; /* (3) */
		DMA1_Channel4->CCR &=~ DMA_CCR_EN;
    DMA1_Channel4->CNDTR = 2;/* Data size */
    DMA1_Channel4->CCR |= DMA_CCR_EN;
	  DMA1_Channel5->CCR &=~ DMA_CCR_EN;
    DMA1_Channel5->CNDTR = 16;/* Data size */
    DMA1_Channel5->CCR |= DMA_CCR_EN;
}

void Send_HRM_Config(void) 
{
	  DMA1_Channel4->CMAR = (uint32_t)OEM_Config; /* (3) */
		DMA1_Channel4->CCR &=~ DMA_CCR_EN;
    DMA1_Channel4->CNDTR = 2;/* Data size */
    DMA1_Channel4->CCR |= DMA_CCR_EN;
	  DMA1_Channel5->CCR &=~ DMA_CCR_EN;
    DMA1_Channel5->CNDTR = 16;/* Data size */
    DMA1_Channel5->CCR |= DMA_CCR_EN;
}

void Send_HRM_Accel(void) 
{uint16_t i = 0;
	
	  Send_Buf[0] = 'A';
	  Send_Buf[1] = Accel_to_Send;
	  for(i=0; i<Accel_to_Send*6; i++)
		{
			Send_Buf[i+2] = Accel_Buf[i];
		}
		CRC_value = crc16(Send_Buf, i+2);
	  Send_Buf[i+2] = (uint8_t)(CRC_value >> 0x08);
	  Send_Buf[i+3] = (uint8_t)CRC_value;
	  DMA1_Channel4->CMAR = (uint32_t)Send_Buf; /* (3) */
		DMA1_Channel4->CCR &=~ DMA_CCR_EN;
    DMA1_Channel4->CNDTR = Send_Buf[1]*6 + 4;/* Data size */
    DMA1_Channel4->CCR |= DMA_CCR_EN;
	  DMA1_Channel5->CCR &=~ DMA_CCR_EN;
    DMA1_Channel5->CNDTR = 16;/* Data size */
    DMA1_Channel5->CCR |= DMA_CCR_EN;
}

void Send_HRM_Gyro(void) 
{uint16_t i = 0;
	
	  Send_Buf[0] = 'G';
	  Send_Buf[1] = Gyro_to_Send;
	  for(i=0; i<Send_Buf[1]*6; i++)
		{
			Send_Buf[i+2] = Gyro_Buf[i];
		}
		CRC_value = crc16(Send_Buf, i+2);
	  Send_Buf[i+2] = (uint8_t)(CRC_value >> 0x08);
	  Send_Buf[i+3] = (uint8_t)CRC_value;
	  DMA1_Channel4->CMAR = (uint32_t)Send_Buf; /* (3) */
		DMA1_Channel4->CCR &=~ DMA_CCR_EN;
    DMA1_Channel4->CNDTR = Send_Buf[1]*6 + 4;/* Data size */
    DMA1_Channel4->CCR |= DMA_CCR_EN;
	  DMA1_Channel5->CCR &=~ DMA_CCR_EN;
    DMA1_Channel5->CNDTR = 16;/* Data size */
    DMA1_Channel5->CCR |= DMA_CCR_EN;
}

void Send_Server_SOS(void) 
{	
	  Init_SIM800();
		send_str(SOS_Buf, 9);
	  send_str(END_LINE, 1);
		
}

