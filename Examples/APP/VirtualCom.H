#ifndef __VirtualCom_H
#define __VirtualCom_H


// Connected LED (GREEN)	//ÓÒLED
#define LED_CONNECTED_PORT     	GPIOB 
#define LED_CONNECTED_PIN    		GPIO_Pin_0
#define LED_CONNECTED_RCC    		RCC_APB2Periph_GPIOB

// Connected LED (GREEN)	//×óLED		
#define LED_RUNNING_PORT     	GPIOB 
#define LED_RUNNING_PIN    		GPIO_Pin_11
#define LED_RUNNING_RCC    		RCC_APB2Periph_GPIOB

void VirtualCom_Configuration(void);
void VirtualCom_Server(void);
void VirtualCom_LedConfiguration(void);
void VirtualCom_LedRun(void);

	
#endif



