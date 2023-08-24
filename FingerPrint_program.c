/******************************************************************************/
/*****************************************************************************/
/****************    Author    : Mahmoud Radwan       ***********************/
/****************    Layer     : HAL                  **********************/
/****************    SWC       : FINGER PRINT         *********************/
/****************    version   : 1.0                  ********************/
/************************************************************************/
/***********************************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "MEM_MAP.h"
#include "UART_interface.h"
#include "FingerPrint_config.h"
#include "FingerPrint_private.h"
#include "FingerPrint_interface.h"
//#include "LCD_interface.h"


volatile u8 Frame_Received[20];
volatile static u8 index= 0;

static void FUNC_RX(void)
{
	UART_ReceiveNoBlock((u8 *)&(Frame_Received[index]));
	index++;
}

void FingerPS_Init(void)
{
	UART_RX_SetCallBack(FUNC_RX);
	UART_RX_InterruptEnable();
}


Error_ConfirmCode_t FingerPS_AuraLedControl(AuraLightControl_t l_code, u8 l_speed, AuraColorControl_t l_color, u8 l_count)
{
	Error_ConfirmCode_t ret= FAILED_TO_OPERATE_COMM_PORT;

	u8 i;
	u8 Frame_TX[16]= {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x07, 0x35, l_code, l_speed, l_color, l_count, 0x00, 0x00};
	u16 sum= 0;

	sum= FingerPS_CalculateCheckSum(Frame_TX, 16);
	Frame_TX[14]= (u8)(sum >> 8);
	Frame_TX[15]= (u8)(sum & 0x00FF);

	/* Send Command Frame using UART Sync */
	for(i= 0; i<16; i++)
	{
		UART_Send(Frame_TX[i]);
	}

	/* Wait till Receive ACK Frame (Using RX Interrupt) */
	while(index < 12);
	ret= Frame_Received[9];
	index= 0;

	return ret;
}


Error_ConfirmCode_t FingerPS_HandShake(void)
{
	Error_ConfirmCode_t ret= FAILED_TO_OPERATE_COMM_PORT;
	u8 i;
	u8 Frame_TX[12]= {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x40, 0x00, 0x44};

	/* Send Command Frame using UART Sync */
	for(i= 0; i<12; i++)
	{
		UART_Send(Frame_TX[i]);
	}

	/* Wait till Receive ACK Frame (Using RX Interrupt) */
	while(index < 12);
	ret= Frame_Received[9];
	index= 0;

	return ret;
}


Error_ConfirmCode_t FingerPS_GenImage(void)
{
	Error_ConfirmCode_t ret= FAILED_TO_OPERATE_COMM_PORT;
	u8 i;
	u8 Frame_TX[12]= {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x01, 0x00, 0x05};

	for(i= 0; i<12; i++)
	{
		UART_Send(Frame_TX[i]);
	}

	while(index < 12);
	ret= Frame_Received[9];
	index= 0;
	return ret;
}


Error_ConfirmCode_t FingerPS_ConvertImageToCharFile(Template_Buffer_t l_bufferID)
{
	Error_ConfirmCode_t ret= FAILED_TO_OPERATE_COMM_PORT;
	u8 i;
	u8 Frame_TX[13]= {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x04, 0x02, l_bufferID, 0x00, 0x00};
	u16 sum= 0;

	sum= FingerPS_CalculateCheckSum(Frame_TX, 13);
	Frame_TX[11]= (u8)(sum >> 8);
	Frame_TX[12]= (u8)(sum & 0x00FF);

	for(i= 0; i<13; i++)
	{
		UART_Send(Frame_TX[i]);
	}

	while(index < 12);
	ret= Frame_Received[9];
	index= 0;
	return ret;
}


Error_ConfirmCode_t FingerPS_GenTemplate(void)
{
	Error_ConfirmCode_t ret= FAILED_TO_OPERATE_COMM_PORT;
	u8 i;
	u8 Frame_TX[12]= {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x05, 0x00, 0x09};

	for(i= 0; i<12; i++)
	{
		UART_Send(Frame_TX[i]);
	}

	while(index < 12);
	ret= Frame_Received[9];
	index= 0;
	return ret;
}


Error_ConfirmCode_t FingerPS_StrTemplate(Template_Buffer_t l_bufferID, u8 l_pageID)
{
	Error_ConfirmCode_t ret= FAILED_TO_OPERATE_COMM_PORT;
	u8 i;
	u8 Frame_TX[15]= {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x06, 0x06, l_bufferID, 0x00, l_pageID, 0x00, 0x00};
	u16 sum= 0;

	sum= FingerPS_CalculateCheckSum(Frame_TX, 15);
	Frame_TX[13]= (u8)(sum >> 8);
	Frame_TX[14]= (u8)(sum & 0x00FF);

	for(i= 0; i<15; i++)
	{
		UART_Send(Frame_TX[i]);
	}

	while(index < 12);
	ret= Frame_Received[9];
	index= 0;

	return ret;
}


Error_ConfirmCode_t FingerPS_SearchFinger(Template_Buffer_t l_bufferID, u8 l_startPage, u8 l_pageNum)
{
	Error_ConfirmCode_t ret= FAILED_TO_OPERATE_COMM_PORT;
	u8 i;
	u8 Frame_TX[17]= {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x08, 0x04, l_bufferID, 0x00, l_startPage, 0x00, l_pageNum, 0x00, 0x00};
	u16 sum= 0;

	sum= FingerPS_CalculateCheckSum(Frame_TX, 17);
	Frame_TX[15]= (u8)(sum >> 8);
	Frame_TX[16]= (u8)(sum & 0x00FF);

	for(i= 0; i<17; i++)
	{
		UART_Send(Frame_TX[i]);
	}

	while(index < 16);
	ret= Frame_Received[9];
	index= 0;
	// need return pageID, matchScore
	return ret;
}


Error_ConfirmCode_t FingerPS_DeleteFinger(u8 l_startPage, u8 l_numOfTemp)
{
	Error_ConfirmCode_t ret= FAILED_TO_OPERATE_COMM_PORT;
	u8 i;
	u8 Frame_TX[16]= {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x07, 0x0C, 0x00, l_startPage, 0x00, l_numOfTemp, 0x00, 0x00};
	u16 sum= 0;

	sum= FingerPS_CalculateCheckSum(Frame_TX, 16);
	Frame_TX[14]= (u8)(sum >> 8);
	Frame_TX[15]= (u8)(sum & 0x00FF);

	for(i= 0; i<12; i++)
	{
		UART_Send(Frame_TX[i]);
	}

	while(index < 12);
	ret= Frame_Received[9];
	index= 0;

	return ret;
}


Error_ConfirmCode_t FingerPS_EmptyLibrary(void)
{
	Error_ConfirmCode_t ret= FAILED_TO_OPERATE_COMM_PORT;
	u8 i;
	u8 Frame_TX[12]= {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x0D, 0x00, 0x11};

	for(i= 0; i<12; i++)
	{
		UART_Send(Frame_TX[i]);
	}

	while(index < 12);
	ret= Frame_Received[9];
	index= 0;
	return ret;
}


static u16 FingerPS_CalculateCheckSum(u8 *pArr, u8 size)
{
	u16 sum= 0;

	if(pArr == NULL)
	{
		/* return error */
	}
	else
	{
		for(int i= 6; i< (size-2); i++)
		{
			sum+= pArr[i];
		}
	}
	return sum;
}
