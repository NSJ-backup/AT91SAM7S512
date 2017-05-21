#include <AT91SAM7S256.h>
#include "aic.h"

void Aic_Init(void)
{
	Lcd_Init();

	*AT91C_PMC_PCER = 1<<AT91C_ID_PIOA;		// PIOA ��ġ Ȱ��ȭ
	*AT91C_PIOA_PER = 1<<INT_PIN;			// �� Enable
	*AT91C_PIOA_OER = 1<<INT_PIN;			// Output Enable
	*AT91C_PIOA_IDR = 1<<INT_PIN;			// ���ͷ�Ʈ �� Disable
	*AT91C_PIOA_IFER = 1<<INT_PIN;			// �Է����� Enable

	*AT91C_AIC_IDCR = 1<<AT91C_ID_PIOA;		// ���ͷ�Ʈ �� command  disable
	*AT91C_AIC_ICCR = 1<<AT91C_ID_PIOA;		// command register clear

	AT91C_AIC_SVR[AT91C_ID_PIOA] = (unsigned int)Handler;	// �ڵ鷯 ���
	AT91C_AIC_SMR[AT91C_ID_PIOA] = 
		AT91C_AIC_SRCTYPE_POSITIVE_EDGE|AT91C_AIC_PRIOR_LOWEST;	// ���۸�� �� �켱���� ����

	*AT91C_PIOA_IER = 1<<INT_PIN;			// PIOA �� 15�� �� ���ͷ�Ʈ Ȱ��ȭ
	*AT91C_AIC_IECR = 1<<AT91C_ID_PIOA;		// PIOA ���ͷ�Ʈ Ȱ��ȭ

	return;
}

void Handler(void)
{
	volatile unsigned int uiIsr;
	volatile unsigned int uiCnt;

	uiIsr = *AT91C_PIOA_ISR;

	if( (uiIsr&(1<<INT_PIN)) != 0 )
	{
		Lcd_Data('A');
	}
	
	for(uiCnt=0; 1000000>uiCnt; ++uiCnt);	// ä�͸� ����

	uiIsr = *AT91C_PIOA_ISR;	// ISR �������� Ŭ����
	*AT91C_AIC_EOICR = 0;	// End Code

	return;
}

