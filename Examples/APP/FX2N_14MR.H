#ifndef __FX2N_14MR_H
#define __FX2N_14MR_H


//******************* ‰≥ˆ∫Í
#define	DROUT(n)	LK3U_40MR_Pout##n

#define	DROUT_ALL_OFF	{						\
											DROUT(1)=0;\
											DROUT(2)=0;\
											DROUT(3)=0;\
											DROUT(4)=0;\
											DROUT(5)=0;\
											DROUT(6)=0;\
											DROUT(7)=0;\
											DROUT(8)=0;\
											DROUT(9)=0;\
											DROUT(10)=0;\
											DROUT(11)=0;\
											DROUT(12)=0;\
											DROUT(13)=0;\
											DROUT(14)=0;\
											DROUT(15)=0;\
											DROUT(16)=0;\
											}
											
#define	DROUT_ALL_ON	{						\
											DROUT(1)=1;\
											DROUT(2)=1;\
											DROUT(3)=1;\
											DROUT(4)=1;\
											DROUT(5)=1;\
											DROUT(6)=1;\
											DROUT(7)=1;\
											DROUT(8)=1;\
											DROUT(9)=1;\
											DROUT(10)=1;\
											DROUT(11)=1;\
											DROUT(12)=1;\
											DROUT(13)=1;\
											DROUT(14)=1;\
											DROUT(15)=1;\
											DROUT(16)=1;\
											}

#define	LK3U_40MR_Pout1	PB6
#define	LK3U_40MR_Pout2	PA3
#define	LK3U_40MR_Pout3	PB1
#define	LK3U_40MR_Pout4	PE13
#define	LK3U_40MR_Pout5	PC8
#define	LK3U_40MR_Pout6	PA2
#define	LK3U_40MR_Pout7	PA6
#define	LK3U_40MR_Pout8	PA7

#define	LK3U_40MR_Pout9	PE10
#define	LK3U_40MR_Pout10	PE8
#define	LK3U_40MR_Pout11	PE7
#define	LK3U_40MR_Pout12	PB0
#define	LK3U_40MR_Pout13	PC5
#define	LK3U_40MR_Pout14	PC4
#define	LK3U_40MR_Pout15	PE14
#define	LK3U_40MR_Pout16	PE15

//******************* ‰»Î∫Í
#define	SIN(n)	LK3U_40MR_Pin##n	

#define	SIN_ALL_ON	{						\
											SIN(1)=0;\
											SIN(2)=0;\
											SIN(3)=0;\
											SIN(4)=0;\
											SIN(5)=0;\
											SIN(6)=0;\
											SIN(7)=0;\
											SIN(8)=0;\
											SIN(9)=0;\
											SIN(10)=0;\
											SIN(11)=0;\
											SIN(12)=0;\
											SIN(13)=0;\
											SIN(14)=0;\
											SIN(15)=0;\
											SIN(16)=0;\
											SIN(17)=0;\
											SIN(18)=0;\
											SIN(19)=0;\
											SIN(20)=0;\
											SIN(21)=0;\
											SIN(22)=0;\
											SIN(23)=0;\
											SIN(24)=0;\
											SIN(25)=0;\
											}
											
#define	SIN_ALL_OFF	{						\
											SIN(1)=1;\
											SIN(2)=1;\
											SIN(3)=1;\
											SIN(4)=1;\
											SIN(5)=1;\
											SIN(6)=1;\
											SIN(7)=1;\
											SIN(8)=1;\
											SIN(9)=1;\
											SIN(10)=1;\
											SIN(11)=1;\
											SIN(12)=1;\
											SIN(13)=1;\
											SIN(14)=1;\
											SIN(15)=1;\
											SIN(16)=1;\
											SIN(17)=1;\
											SIN(18)=1;\
											SIN(19)=1;\
											SIN(20)=1;\
											SIN(21)=1;\
											SIN(22)=1;\
											SIN(23)=1;\
											SIN(24)=1;\
											SIN(25)=1;\
											}											
											
#define	LK3U_40MR_Pin1	PC13
#define	LK3U_40MR_Pin2	PA0
#define	LK3U_40MR_Pin3	PA1
#define	LK3U_40MR_Pin4	PE2
#define	LK3U_40MR_Pin5	PC6
#define	LK3U_40MR_Pin6	PC7
#define	LK3U_40MR_Pin7	PE5
#define	LK3U_40MR_Pin8	PE1
#define	LK3U_40MR_Pin9	PE0
#define	LK3U_40MR_Pin10	PE9
#define	LK3U_40MR_Pin11	PE11
#define	LK3U_40MR_Pin12	PB9
#define	LK3U_40MR_Pin13	PB4
#define	LK3U_40MR_Pin14	PB5
#define	LK3U_40MR_Pin15	PB3
#define	LK3U_40MR_Pin16	PD7
#define	LK3U_40MR_Pin17	PD6
#define	LK3U_40MR_Pin18	PD5
#define	LK3U_40MR_Pin19	PD4
#define	LK3U_40MR_Pin20	PD3
#define	LK3U_40MR_Pin21	PA12
#define	LK3U_40MR_Pin22	PA11
#define	LK3U_40MR_Pin23	PA8
#define	LK3U_40MR_Pin24	PC9
#define	LK3U_40MR_Pin25	PD15

#define	LK3U_40MR_LED485	PB13
#define	LK3U_40MR_LED232	PB12
#define	LK3U_40MR_LEDERR	PB15
#define	LK3U_40MR_LEDRUN	PB14

void FX2N_14MR_Configuration(void);
void FX2N_14MR_Server(void);
void FX2N_14MR_GPIOSET(void);
void FX2N_14MR_LEDDSP(unsigned char Dsp_num);
unsigned char FX2N_14MR_INPUT(void);		//∂¡π‚ÒÓ ‰≥ˆ–≈∫≈



#endif

