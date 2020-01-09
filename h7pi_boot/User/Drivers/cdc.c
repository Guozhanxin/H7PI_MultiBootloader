#include "cdc.h"
#include <stdarg.h>
 
cdc_t cdc;

uint8_t usb_cmds[][CDC_CMD_LENGTH]={
  "u2u",
	"dfu",
	"reboot"
};
/***************************************************************************
�������buffer

****************************************************************************/
void cdc_clear_buffer(void)
{
	memset(cdc.rxBuffer,0,sizeof(cdc.rxBuffer));
	cdc.rxEnd = 0;
	cdc.rxLength = 0;
}
/***************************************************************************
cdc����
���������static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)��Ĭ�ϵ���

****************************************************************************/
void cdc_receive(uint8_t* Buf, uint16_t Len)
{
	memcpy(cdc.rxBuffer,Buf,Len);
	cdc.rxLength = Len;
	cdc.rxEnd = 1;
}
/***************************************************************************
cdc����
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len) ��usbd_cdc_if.c����
****************************************************************************/
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
void cdc_send(uint8_t* Buf, uint16_t Len)
{
	CDC_Transmit_FS(Buf,Len);
}

/***************************************************************************
ϵͳ��־��ӡ
****************************************************************************/
void cdc_printf(const char *format, ...)
{
    va_list args;
    uint32_t length;
	  uint8_t buffer[CDC_BUFFER_SIZE];
 
    va_start(args, format);
    length = vsnprintf((char *)buffer, CDC_BUFFER_SIZE, (char *)format, args);
    va_end(args);
    CDC_Transmit_FS(buffer, length);
}

/***************************************************************************
cmd line ָ�����
****************************************************************************/
__weak void cdc_cli_task(void)
{	
	int i = 0;
	int len = 0;
	char* cmd=NULL;
	
	if(cdc.rxLength > 0)
	{
		for(i = 0; i < (sizeof(usb_cmds)/CDC_CMD_LENGTH); i++)
		{
			cmd = strstr((const char*)cdc.rxBuffer,(const char*)usb_cmds[i]);
			if(cmd != NULL)
			{
				//
				//ִ��ָ�����
				//
				//dfuָ��
				if(strstr((const char*)usb_cmds[i],"dfu")!=NULL)
				{
					
				}
				//
				//����
				else if(strstr((const char*)usb_cmds[i],"reboot")!=NULL)
				{
					NVIC_SystemReset();
				}
				//
				//����bc20
				else if(strstr((const char*)usb_cmds[i],"bc20")!=NULL)
				{
					if(strstr((const char*)cdc.rxBuffer,"ip")!=NULL)
					{
						
					}
					else if(strstr((const char*)cdc.rxBuffer,"port")!=NULL)
					{
						
					}
				}
				cdc_clear_buffer();
			}
		}
		
	}
}





