#include <AT91SAM7S256.h>
#include "ultra.h"

static volatile unsigned int uiCm;
static volatile unsigned int uiState = INIT_OFF;

static void Timer_ISR(void);

static void UTimer_Init(void)
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
	AT91C_AIC_SVR[AT91C_ID_TC0] = (unsigned int)Timer_ISR;	// ���ͷ�Ʈ �Լ��� �ּ�(���Ͷ� �θ���)
	AT91C_AIC_SMR[AT91C_ID_TC0] = AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL|AT91C_AIC_PRIOR_LOWEST;
	*AT91C_AIC_ICCR = 1<<AT91C_ID_TC0;		// ���ͷ�Ʈ�� ȣ��� ������ ���� (�������ڸ��� ���ͷ�Ʈ�� �ɸ��� �ʵ���)

// TC_RC �� �� Ÿ�̸� ���ͷ�Ʈ Ȱ��ȭ(TC_IER, AT91C_TC_CPCS)
	*AT91C_TC0_IER = AT91C_TC_CPCS;
	*AT91C_TC0_RC = 22;				// 1cm �����ϸ� ���ͷ�Ʈ�� �߻�(58us ����)

// 2. �� : Ÿ�̸� ���ͷ�Ʈ ��Ȱ��ȭ ---------------

// Ÿ�̸� ī���� 0 ���ͷ�Ʈ Ȱ��ȭ(AIC_IECR, AT91C_ID_TC0)
	*AT91C_AIC_IECR =1<< AT91C_ID_TC0;	

// 3. �� : Ÿ�̸� ī���� 0 ���ͷ�Ʈ ��Ȱ��ȭ -------

// Ÿ�̸� Ŭ�� Ȱ��ȭ(TC_CCR, AT91C_TC_CLKEN)
	// *AT91C_TC0_CCR = AT91C_TC_CLKEN;

// 1. �� : Ÿ�̸� Ŭ�� ��Ȱ��ȭ ------------------

// Ÿ�̸� ����(TC_CCR, AT91C_TC_SWTRG)
	// *AT91C_TC0_CCR = AT91C_TC_SWTRG;

}

static void Timer_ISR(void)	// �Ҹ��� 1cm �����ϸ� �߻��ϴ� ���ͷ�Ʈ
{
// ���ͷ�Ʈ ���� ���� �ʱ�ȭ(TC_SR �б�)
	*AT91C_TC0_SR;	// �������� �б⸸ �Ѵ�

	++uiCm;		// ��ü���� �Ÿ�����
}


static void UTrigger_Init(void)		// ������ ���� trigger �ʱ�ȭ
{ 
	*AT91C_PMC_PCER 	= 1<<AT91C_ID_PIOA;	// PIOA�� Ŭ�� ���� on
	*AT91C_PIOA_PER 		= UTRI_PIN;			// ������ trigger �� Ȱ��ȭ
	*AT91C_PIOA_PPUDR 	= UTRI_PIN;			// PIOA pull-up off(����ġ�� ����ϴµ� �ַ� ���)
	*AT91C_PIOA_OER 		= UTRI_PIN;			// ��� enable
	*AT91C_PIOA_CODR 	= UTRI_PIN;			// PIOA�� clear
	
	return;
}

static void Echo_Init(void)		// ������ ���� echo �ʱ�ȭ
{ 
	*AT91C_PMC_PCER 	= 1<<AT91C_ID_PIOA;	// PIOA�� Ŭ�� ���� on
	*AT91C_PIOA_PER 		= ECHO_PIN;			// ������ echo �� Ȱ��ȭ
	*AT91C_PIOA_PPUDR 	= ECHO_PIN;			// PIOA pull-up off(����ġ�� ����ϴµ� �ַ� ���)
	
	return;
}

static void UTrigger(void)			// ������ ���� trigger ��ȣ ���
{
	volatile unsigned int uiCnt;
	
	// trigger pulse
	*AT91C_PIOA_CODR 	= UTRI_PIN;			// PIOA�� clear
	for(uiCnt = 0; uiCnt<1000; uiCnt++);	// delay
	*AT91C_PIOA_SODR 	= UTRI_PIN;			// PIOA�� set
	for(uiCnt = 0; uiCnt<1000; uiCnt++);	// delay
	*AT91C_PIOA_CODR 	= UTRI_PIN;			// PIOA�� clear
	
	return;
}

unsigned int Ultra_Run(void)
{
	if( INIT_OFF == uiState)
	{
		return 0;
	}

	uiCm = 0;						// �Ÿ� �ʱ�ȭ
	*AT91C_TC0_CCR = AT91C_TC_CLKEN;	// timer/counter clock enable
	
	UTrigger();		// trigger ��ȣ �߻�
	
	// echo ��ȣ : polling��� ����
	while( 0 == (ECHO_PIN&(*AT91C_PIOA_PDSR)) );	// pin �Է��� low�� ���¿��� ���
	*AT91C_TC0_CCR = AT91C_TC_SWTRG;				// timer/counter ����

	while( 0 != (ECHO_PIN&(*AT91C_PIOA_PDSR)) );	// pin �Է��� high�� ���¿��� ���
	*AT91C_TC0_CCR = AT91C_TC_CLKDIS;				// timer/counter clock disable

	
	return uiCm;
}

void Ultra_Init(void)
{
	UTimer_Init();			// �Ÿ� ������ ���� Ÿ�̸� �ʱ�ȭ
	UTrigger_Init();		// ������ ���� trigger ��ȣ�� �ֱ����� �� �ʱ�ȭ
	Echo_Init();			// echo ��ȣ�� �ޱ����� �� �ʱ�ȭ
	
	uiState = INIT_ON;

	return;
}

