#include "app_bat.h"

static  volatile float vbat = 0.0f;//作用范围在文件内部，可能被外部改变

//
//@简介：对电池电压检测模块进行初始化
//
void App_Bat_Init(void){
	
	//#1.初始化TIM2_TRGO每10ms产生一个脉冲
	
	//初始化时基单元
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//开启TIM2时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
	
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 9999;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	//配置从模式控制器
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);//选择主机为Update模式
	
	//闭合时基单元开关
	TIM_Cmd(TIM2, ENABLE);
	
	//#2.对分频系数进行初始化
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//六分频
	
	//将PB0初始化为模拟模式
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//开启GPIOB时钟
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//初始化ADC基本参数
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//开启ADC1时钟
	
	ADC_InitTypeDef ADC_InitStruct = {0};
	
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//常规序列软件输入
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;//不使用双ADC模式
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//扫描模式针对多输入同时进行
	
	ADC_Init(ADC1, &ADC_InitStruct);
	
	//设置注入序列参数
	ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T2_TRGO);//把TIM2作为注入序列的外部触发信号
	
	ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);//闭合注入序列外部触发开关
	
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_8, 1,ADC_SampleTime_1Cycles5);
	
	//配置ADC的JEOC中断
	ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);//闭合JEOC开关
	
	//NVIC 中断优先级分组
	NVIC_InitTypeDef NVIC_InitStruct = {0};
	
	NVIC_InitStruct.NVIC_IRQChannel =ADC1_2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;	//子优先级（对实时性要求不高）
	
	NVIC_Init(&NVIC_InitStruct);
	
	ADC_Cmd(ADC1, ENABLE);//闭合ADC1总开关
}


// 
//@简介：获取当前电池电压，单位是V
//
float App_Bat_Get(void){
	return vbat;
}

//
//@ADC1和ADC2的中断响应函数
//
void ADC1_2_IRQHandler(void){
	if(ADC_GetFlagStatus(ADC1, ADC_FLAG_JEOC) == SET){//确保JEOC标志位触发
		
		ADC_ClearFlag(ADC1, ADC_FLAG_JEOC);//清除标志位
		
		uint16_t jdr1 = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1);//读取jdr1的值
		
		vbat = (jdr1 / 4095.0f * 3.3) * (8.4f / 3.3);//数字信号转换成电压
	}
}
