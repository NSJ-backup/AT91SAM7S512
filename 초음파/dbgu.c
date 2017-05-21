#include <AT91SAM7S256.h>
#include "project.h"



void Dbgu_Str(char * cpStr)	// ���ڿ� ����
{
	while( 0 != *cpStr )
	{
		Dbgu_Tx(*cpStr);
		++cpStr;
	}
}

void Dbgu_Menu(void)
{
	Dbgu_Str("1. Lcd Print\n\r");
	Dbgu_Str("M. Menu Print\n\r");
}

void Dbgu_Rx_ISR(void)
{
	switch(  (*AT91C_DBGU_RHR)&(0x000000FF)  )	// ���Ⱚ ����
	{
		case 'M'	:
		case 'm'	: 	Dbgu_Menu();	
					break;
					
		case '1' 	:	Lcd_Inst(LCD_INST_CLR);
					Lcd_Print("Hello ARM");
					break;
					
		default 	: 	Dbgu_Str("Wrong Command\n\r");
					Dbgu_Menu();
					break;
	}
}

void Dbgu_Init_Polling(void)
{
	*AT91C_PIOA_PDR		= AT91C_PA9_DRXD|AT91C_PA10_DTXD;	// PIO ����� off
	*AT91C_PIOA_ASR 	= AT91C_PA9_DRXD|AT91C_PA10_DTXD;	// PA9, 10�� Peripheral A Ȱ��ȭ
	*AT91C_DBGU_CR 		= AT91C_US_RSTRX|AT91C_US_RSTTX;	// RX&TX�� reset & disable
	*AT91C_DBGU_MR 		= AT91C_US_CHMODE_NORMAL|AT91C_US_PAR_EVEN;	// DBGU ��� ����
	*AT91C_DBGU_BRGR 	= DBGU_CD;		// BaudRate ����

	*AT91C_DBGU_CR 		= AT91C_US_RXEN|AT91C_US_TXEN;		// ��/���� Ȱ��ȭ

	return;
}

void Dbgu_Init_ISR(void)
{
	*AT91C_PIOA_PDR	= AT91C_PA9_DRXD|AT91C_PA10_DTXD;		// PIO ����� off
	*AT91C_PIOA_ASR 	= AT91C_PA9_DRXD|AT91C_PA10_DTXD;	// PA9, 10�� Peripheral A Ȱ��ȭ
	*AT91C_DBGU_CR 	= AT91C_US_RSTRX|AT91C_US_RSTTX;		// RX&TX�� reset & disable
	// DBGU ��� ����
	*AT91C_DBGU_MR 	= AT91C_US_CHMODE_NORMAL|AT91C_US_PAR_EVEN;	// Normal Mode, Even parity
	*AT91C_DBGU_BRGR 	= DBGU_CD;		// BaudRate ���� : 115200


	*AT91C_AIC_IDCR 	= 1<<AT91C_ID_SYS;	// System Peripheral interrupt disable

	AT91C_AIC_SVR[AT91C_ID_SYS] = (unsigned int)Dbgu_Rx_ISR;	// �ڵ鷯 ���
	AT91C_AIC_SMR[AT91C_ID_SYS] = AT91C_AIC_SRCTYPE_POSITIVE_EDGE|AT91C_AIC_PRIOR_LOWEST;	// ���۸�� �� �켱���� ����
	*AT91C_AIC_ICCR	= 1<<AT91C_ID_SYS;	// command register clear(������ ���� ���ͷ�Ʈ ����)

	*AT91C_DBGU_IER	= AT91C_US_RXRDY;	// DBGU ���ͷ�Ʈ Ȱ��ȭ

	*AT91C_AIC_IECR	= 1<<AT91C_ID_SYS;	// System Peripheral interrupt enable

	*AT91C_DBGU_CR	= AT91C_US_RXEN|AT91C_US_TXEN;	// ��/���� Ȱ��ȭ

	return;
}

void Dbgu_Init(void)
{
	Dbgu_Init_ISR();
	//Dbgu_Init_Polling();

	return;
}

unsigned char Dbgu_Rx(void)		// polling ���
{
	while( 0 == (*AT91C_DBGU_CSR&AT91C_US_RXRDY) );

	return *AT91C_DBGU_RHR;
}

void Dbgu_Tx(unsigned char cData)	// polling ���
{
	while( 0 == (*AT91C_DBGU_CSR&AT91C_US_TXRDY) );

	*AT91C_DBGU_THR = cData;

	return;
}
