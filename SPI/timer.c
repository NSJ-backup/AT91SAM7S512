#include <AT91SAM7S256.h>
#include "timer.h"

volatile static unsigned int uiMSec;	// �ٸ� �ҽ�(.c �����)�� ��ġ�ų� �������� ���ϵ��� �Ѵ�

void Timer_ISR(void);

// Timer.c

// 0�� Ÿ�̸� ���ξ�: TC0 // 1�� Ÿ�̸� ���ξ�: TC1 // 2�� Ÿ�̸� ���ξ�: TC2
// ��: #define TC0_CCR    ((volatile unsigned int *)0xFFFA0000)
// ���ͷ��� ���� : AT91C_BASE_AIC
// 0�� Ÿ�̸� ���� : AT91C_BASE_TC0
// PMC ����   : AT91C_BASE_PMC

void Timer_Init(void)
{
// Ÿ�̸� ī���� 0 ����� ���� PMC Ȱ��ȭ(AT91C_ID_TC0)
	*AT91C_PMC_PCER = 1<<AT91C_ID_TC0;

// 1.���� : Ÿ�̸� Ŭ�� ��Ȱ��ȭ ------------------
// Ÿ�̸� Ŭ�� ��Ȱ��ȭ(TC_CCR ����)
	*AT91C_TC0_CCR = AT91C_TC_CLKDIS;

// 2. ���� : Ÿ�̸� ���ͷ�Ʈ ��Ȱ��ȭ -------------
// Ÿ�̸� ���ͷ�Ʈ ��Ȱ��ȭ(TC_IDR ����)
// ���ͷ�Ʈ ���� ���� �ʱ�ȭ(TC_SR �б�)
	*AT91C_TC0_IDR = 0xFFFFFFFF;
	*AT91C_TC0_SR;	// �������� �б⸸ �Ѵ�

// ���ֺ� 128, �� ��� �������� ����(TC_CMR, DIV4_CLOCK, AT91C_TC_CPCTRG)
// MCKR divided by 128 => 12, 0.25us(TC_RC ����)
	*AT91C_TC0_CMR = AT91C_TC_CLKS_TIMER_DIV4_CLOCK|AT91C_TC_CPCTRG;
	
// Ÿ�̸� ī���� 0 ���ͷ�Ʈ ��Ȱ��ȭ(AIC_IDCR, AT91C_ID_TC0)
	*AT91C_AIC_IDCR = 1<<AT91C_ID_TC0;	

// 3. ���� : Ÿ�̸� ī���� 0 ���ͷ�Ʈ ��Ȱ��ȭ ------
// Ÿ�̸� ī���� 0 ���ͷ�Ʈ �ڵ鷯 ���(AIC_SVR[AT91C_ID_TC0], timer_handler)
// Ÿ�̸� ī���� 0 ���ͷ�Ʈ ��� ����(AIC_SMR[AT91C_ID_TC0], AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, AT91C_AIC_PRIOR_LOWEST)
// Ÿ�̸� ī���� 0 ���ͷ�Ʈ Ŭ����(AIC_ICCR, AT91C_ID_TC0)
	AT91C_AIC_SVR[AT91C_ID_TC0] = (unsigned int)Timer_ISR;
	AT91C_AIC_SMR[AT91C_ID_TC0] = AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL|AT91C_AIC_PRIOR_LOWEST;
	*AT91C_AIC_ICCR = 1<<AT91C_ID_TC0;		// ���ͷ�Ʈ�� ȣ��� ������ ���� (�������ڸ��� ���ͷ�Ʈ�� �ɸ��� �ʵ���)

// TC_RC �� �� Ÿ�̸� ���ͷ�Ʈ Ȱ��ȭ(TC_IER, AT91C_TC_CPCS)
	*AT91C_TC0_IER = AT91C_TC_CPCS;
	*AT91C_TC0_RC = 375;

// 2. �� : Ÿ�̸� ���ͷ�Ʈ ��Ȱ��ȭ ---------------

// Ÿ�̸� ī���� 0 ���ͷ�Ʈ Ȱ��ȭ(AIC_IECR, AT91C_ID_TC0)
	*AT91C_AIC_IECR =1<< AT91C_ID_TC0;	

// 3. �� : Ÿ�̸� ī���� 0 ���ͷ�Ʈ ��Ȱ��ȭ -------

// Ÿ�̸� Ŭ�� Ȱ��ȭ(TC_CCR, AT91C_TC_CLKEN)
	*AT91C_TC0_CCR = AT91C_TC_CLKEN;

// 1. �� : Ÿ�̸� Ŭ�� ��Ȱ��ȭ ------------------

// Ÿ�̸� ����(TC_CCR, AT91C_TC_SWTRG)
	*AT91C_TC0_CCR = AT91C_TC_SWTRG;

}

void Timer_ISR(void)
{
// ���ͷ�Ʈ ���� ���� �ʱ�ȭ(TC_SR �б�)
	*AT91C_TC0_SR;	// �������� �б⸸ �Ѵ�

	++uiMSec;
}

void ms_delay(unsigned int uiMS)	// Ÿ�̸� ī���� interrupt�� �̿��ؼ� uiMS��ŭ ������Ű�� �Լ�
{
	uiMSec = 0;

	while(uiMSec < uiMS);	// interrupt�� uiMS �� �߻��ҵ��� �����ȴ�

	return;
}
