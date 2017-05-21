#include <AT91SAM7S256.h>
#include "pwm.h"

void Pwm2_Init(void)
{
	*AT91C_PMC_PCER = 1<<AT91C_ID_PWMC;		// PWM ��ġ�� Ŭ�� ����
	*AT91C_PIOA_PPUDR = AT91C_PA25_PWM2;		// 25�� ���� pull-up off
	
	*AT91C_PIOA_PDR 	= AT91C_PA25_PWM2;		// 25�� ���� PIO��� off
	*AT91C_PIOA_BSR 	= AT91C_PA25_PWM2;		// 25�� ���� PWM2 ��� on
	// CLKA, CLKB �� 200kHz�� ����
	*AT91C_PWMC_MR = (30<<DIVA)|(30<<DIVB)|(3<<PREA)|(3<<PREB);
	// CLKA Ŭ���� ���, left aligned, low level start, CUPD modify period cycle
	*AT91C_PWMC_CH2_CMR = (5)&(~AT91C_PWMC_CALG)&(~AT91C_PWMC_CPOL)|(AT91C_PWMC_CPD);

	*AT91C_PWMC_CH2_CPRDR = OCT5_C;		// PWM2 �ֱ⸦ ����
	*AT91C_PWMC_CH2_CDTYR = (unsigned int)(OCT5_C/2);		// PWM2 ��Ƽ�� ����
	//*AT91C_PWMC_ENA = AT91C_PWMC_CHID2;	// PWM2 ����
	
	return;
}

void Pwm2_Run(int iPeriod)
{
	*AT91C_PWMC_DIS = AT91C_PWMC_CHID2;	// PWM2 ����

	*AT91C_PWMC_CH2_CPRDR = iPeriod;				// PWM2 �ֱ⸦ ����
	*AT91C_PWMC_CH2_CDTYR = (unsigned int)(iPeriod/2);	// PWM2 ��Ƽ�� ����

	*AT91C_PWMC_ENA = AT91C_PWMC_CHID2;	// PWM2 ����
	
	return;
}

void Pwm2_Stop(void)
{
	*AT91C_PWMC_DIS = AT91C_PWMC_CHID2;	// PWM2 ����

	return;
}



