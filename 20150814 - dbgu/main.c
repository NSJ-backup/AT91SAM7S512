#include <project.h>

int main()
{
	Lcd_Init();
	Dbgu_Init();
	
	while (1)
	{
		// DBGU���� ���ŵ� ���ڸ� LCD�� ���
		//Lcd_Data(Dbgu_Rx());
	}
	
	return 0;
}


