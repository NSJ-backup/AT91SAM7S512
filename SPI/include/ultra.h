#ifndef	__ULTRA_H__
#define	__ULTRA_H__

#define	UTRI_PIN	(1<<4)	// trigger ��(���)
#define	ECHO_PIN	(1<<3)	// echo ��(�Է�)

#define	INIT_ON		1
#define	INIT_OFF	0


void Ultra_Init(void);
unsigned int Ultra_Run(void);


#endif	/* __ULTRA_H__ */

