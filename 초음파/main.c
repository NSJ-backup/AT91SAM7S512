#include <project.h>

int main(void)
{
	unsigned char ucaDist[] = "000cm";
	unsigned int uiCm;
	volatile unsigned int uiCnt = 0;
	
	Lcd_Init();
	Ultra_Init();
	
	while (1)
	{
		uiCm = Ultra_Run();

		for(uiCnt = 0; uiCnt<0x5FFFF; uiCnt++);		// delay
		Lcd_Inst(LCD_INST_HOME);				// LCD Ŀ���� �ű��
		
		if( 0 == uiCm )				// ��� ���� 0�̸� ����
		{
			Lcd_Print("Error");		// ������ ����ϰ� ������
			continue;
		}

		ucaDist[0] = '0' + (uiCm/100)%10;	// �Ÿ����� �ƽ�Ű ���ڿ��� ��ȯ
		ucaDist[1]	 = '0' + (uiCm%100)/10;
		ucaDist[2]	 = '0' + uiCm%10;
		
		Lcd_Print(ucaDist);				// �Ÿ��� ���
	}
	
	return 0;
}

