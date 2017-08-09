#ifndef __PD014V14_H
#define __PD014V14_H


//发药电磁铁控制
#define	ct_pmos1	PA7
#define	ct_pmos2	PC5
#define	ct_pmos3	PB1
#define	ct_pmos4	PB11
#define	ct_pmos5	PB13
#define	ct_pmos6	PB15
#define	ct_pmos7	PC7
#define	ct_pmos8	PC9

//发药传感器供电开关
#define	P_Sens	PA5

//发药传感器
#define	Sens_In1	PA6in
#define	Sens_In2	PC4in
#define	Sens_In3	PB0in
#define	Sens_In4	PB10in
#define	Sens_In5	PB12in
#define	Sens_In6	PB14in
#define	Sens_In7	PC6in
#define	Sens_In8	PC8in


void PD014V14_Configuration(void);
void PD014V14_Server(void);


#endif

