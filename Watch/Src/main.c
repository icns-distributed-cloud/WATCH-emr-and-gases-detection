/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "i2s.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ST7789.h"
#include "Font.h"
#include "stdbool.h"
#include <stdio.h>
#define Size_1 10000-1
#define Size 10000-1//sampling size.
//volatile uint16_t adcval1;
//volatile uint16_t adcval2;
//volatile global variable


// This file is a part of stm-cartcontroller
// BSD 3-Clause License 
// 
// Copyright (c) 2019, Intelligent-distributed Cloud and Security Laboratory (ICNS Lab.)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// title           : main.c
// description     : C, stm-safety watch
// author          : Bon Kwan Gu, Sang Woo Yoon
// date            : 20190905
// version         : 0.1
// TrueSTUDIO	   : 9.3.0
// notes           : This stm-safety watch is an implementation of a safe watch for
//					 detecting electromagnetic wave in the C Programming Language.
// ==============================================================================

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
_Bool emfMode = false;
_Bool wcMode = false;
_Bool cMode = false;
_Bool rst=false;
int cnt=0;
RTC_HandleTypeDef hrtc;
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
//RTC structure.
void magneticMode();
//emf detector function.
void watchControl();
void Counting();
void DisplayWC(int a,int b,int c,int d,int e,int f);
void setTime(int a,int b,int c,int d,int e,int f);
void TimeDisplay(int y,int xs1,int xs2,int xm1,int xm2,int xh1,int xh2,int xc1,int xc2,uint32_t **font);
//watch control function.
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_I2S3_Init();
  MX_SPI1_Init();
  MX_USB_HOST_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM7_Init();
  MX_RTC_Init();
  MX_USART2_UART_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  ST7789_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  ST7789_Clear(0,240,0,240);
  while (1)
  {
   /*  if(count==true){
       	cnt++;
       	HAL_Delay(150);
       	count = false;
     }*/
	 //switch counting code.
     if(emfMode==true){
    	 ST7789_Clear(0,240,0,240);
         magneticMode();
         ST7789_Clear(0,240,0,240);
     }
     if(wcMode==true){
         ST7789_Clear(0,240,0,240);

    	 Counting();
    	 watchControl();
         ST7789_Clear(0,240,0,240);

     }
     //function select.

     while((emfMode==false)&&(wcMode==false)){
       TimeDisplay(30,158,182,94,118,30,54,78,142,FONT_1);

	   ST7789_drawInt(30,70,FONT_2,20,0xFFFF);
	   ST7789_drawInt(62,70,FONT_2,sDate.Year,0xFFFF);
	   ST7789_drawString(94,70,FONT_2,".",0xFFFF);

	   if(sDate.Month >9){
		   ST7789_drawInt(102,110,FONT_2,sDate.Month,0xFFFF);
	   }
	   else{
		   ST7789_drawInt(102,70,FONT_2,0,0xFFFF);
		   ST7789_drawInt(118,70,FONT_2,sDate.Month,0xFFFF);
	   }
	   ST7789_drawString(134,70,FONT_2,".",0xFFFF);

	   if(sDate.Date / 10 > 1){
		   ST7789_drawInt(142,70,FONT_2,sDate.Date,0xFFFF);
	   }
	   else{
		   ST7789_drawInt(142,70,FONT_2,0,0xFFFF);
		   ST7789_drawInt(158,70,FONT_2,sDate.Date,0xFFFF);
	   }
	   ST7789_drawString(174,70,FONT_2,".",0xFFFF);

	   HAL_Delay(20);
	   HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_12);
	   HAL_Delay(20);
	   HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_13);
	   HAL_Delay(20);
	   HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_14);
	   HAL_Delay(20);
	   HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_15);
	   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
	   //buzzer off.
	   HAL_Delay(20);
     }
    /* USER CODE END WHILE */
    MX_USB_HOST_Process();

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S|RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* EXTI15_10_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  /* EXTI9_5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  /* EXTI0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

/* USER CODE BEGIN 4 */
void Counting(){
	if(wcMode==true){
		cnt++;
	  	HAL_Delay(400);
	  	wcMode = false;
	}
}
//counting function.
void DisplayWC(int f,int e,int d,int c,int b,int a){

	if(cnt==6){
		ST7789_drawInt(130,24,FONT_1,f,0x0000);
	}
	if(cnt==5){
		ST7789_drawInt(105,24,FONT_1,e,0x0000);
	}
	if(cnt==4){
		ST7789_drawInt(80,24,FONT_1,d,0x0000);
	}
	if(cnt==3){
		ST7789_drawInt(55,24,FONT_1,c,0x0000);
	}
	if(cnt==2){
		ST7789_drawInt(30,24,FONT_1,b,0x0000);
	}
	if(cnt==1){
		ST7789_drawInt(5,24,FONT_1,a,0x0000);
	}

	ST7789_drawInt(130,24,FONT_1,f,0xFFFF);
	ST7789_drawInt(105,24,FONT_1,e,0xFFFF);
	ST7789_drawInt(80,24,FONT_1,d,0xFFFF);
	ST7789_drawInt(55,24,FONT_1,c,0xFFFF);
	ST7789_drawInt(30,24,FONT_1,b,0xFFFF);
	ST7789_drawInt(5,24,FONT_1,a,0xFFFF);

}
//Display watch function.
void setTime(int a,int b,int c,int d,int e,int f){
	sTime.Seconds=b*10+a;
	sTime.Minutes=d*10+c;
	sTime.Hours=f*10+e;
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	wcMode=false;
	emfMode=false;
	cnt=0;
	rst=false;
}
//timeseting function.
void TimeDisplay(int y,int xs1,int xs2,int xm1,int xm2,int xh1,int xh2,int xc1,int xc2,uint32_t **font){

     HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
     HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
     //time,date function.
   /*  if(*c != sTime.Hours){
    	 if(*c / 10 < 1){
    	     ST7789_drawInt(12,12,FONT_1,0,0x0);
    	     ST7789_drawInt(36,12,FONT_1,*c,0x0);
    	 }
    	 else{
    		 ST7789_drawInt(12,12,FONT_1,*c,0x0);
    	 }
     }*/
     if(sTime.Hours / 10 < 1){
    	 ST7789_drawInt(xh1,y,font,0,0xFFFF);
    	 ST7789_drawInt(xh2,y,font,sTime.Hours,0xFFFF);
     }
     else{
         ST7789_drawInt(xh1,y,font,sTime.Hours,0xFFFF);
     }
     /*if(*b != sTime.Minutes){
    	 if(*b / 10 < 1){
    		 ST7789_drawInt(76,12,FONT_1,0,0x0);
    		 ST7789_drawInt(100,12,FONT_1,*b,0x0);
         }
         else{
             ST7789_drawInt(76,12,FONT_1,*b,0x0);
         }
     }
*/
     if(sTime.Minutes / 10 < 1){
         ST7789_drawInt(xm1,y,font,0,0xFFFF);
         ST7789_drawInt(xm2,y,font,sTime.Minutes,0xFFFF);
     }
     else{
         ST7789_drawInt(xm1,y,font,sTime.Minutes,0xFFFF);
     }
   /*  if(*a != sTime.Seconds){
    	 if(*a / 10 < 1){
    		 ST7789_drawInt(140,12,FONT_1,0,0x0);
    	     ST7789_drawInt(164,12,FONT_1,*a,0x0);
    	 }
    	 else{
    	     ST7789_drawInt(140,12,FONT_1,*a,0x0);
    	 }
     }
*/
     if(sTime.Seconds / 10 < 1){
    	 ST7789_drawInt(xs1,y,font,0,0xFFFF);
    	 ST7789_drawInt(xs2,y,font,sTime.Seconds,0xFFFF);
     }
     else{
         ST7789_drawInt(xs1,y,font,sTime.Seconds,0xFFFF);
     }

     ST7789_drawChar(xc1,y,font,':',0xFFFF);
     ST7789_drawString(xc2,y,font,":",0xFFFF);
   /*     // 날짜 요일. 요일은 CubeMX 설정 int>글자 변환만 있어서 숫자를 글자로 변환할 필요있음
     OLED_drawInt(12,24,FONT_8,20);
     OLED_drawInt(24,24,FONT_8,sDate.Year);
     OLED_drawString(35,24,FONT_8,".");
     if(sDate.Month >9){
        OLED_drawInt(39,24,FONT_8,sDate.Month);
     }
     else{
        OLED_drawInt(39,24,FONT_8,0);
        OLED_drawInt(45,24,FONT_8,sDate.Month);
     }
     OLED_drawString(51,24,FONT_8,".");
     if(sDate.Date / 10 > 1){
        OLED_drawInt(56,24,FONT_8,sDate.Date);
     }
     else{
        OLED_drawInt(56,24,FONT_8,0);
        OLED_drawInt(62,24,FONT_8,sDate.Month);
     }
     OLED_drawString(68,24,FONT_8,".");*/

}
//time function
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
   if(GPIO_Pin==GPIO_PIN_11){
       emfMode = true;
   }
   else if(GPIO_Pin==GPIO_PIN_9){
       wcMode = true;
   }
   else if(GPIO_Pin==GPIO_PIN_0){
	   rst = true;
   }

}
//Interrupt function.
void watchControl(){
   int S_1=0;
   int S_2=0;
   int M_1=0;
   int M_2=0;
   int H_1=0;
   int H_2=0;
   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);

   while(cnt>0){
	   while(cnt==1){
		  if(emfMode==true){
			 H_2++;
			 HAL_Delay(200);
			 emfMode=false;
		  }
		  if(H_2==3){
			 H_2=0;
		  }
		  Counting();
		  DisplayWC(S_1,S_2,M_1,M_2,H_1,H_2);

		  if(rst==true){

			 setTime(S_1,S_2,M_1,M_2,H_1,H_2);
		  }
	   }

	   while(cnt==2){
		  if(emfMode==true){
			 H_1++;
			 HAL_Delay(200);
			 emfMode=false;
		  }
		  if(H_2==2){
			 if(H_1==4){
				H_1 = 0;
			 }
		  }
		  else{
			 if(H_1==10){
				H_1 = 0;
			 }
		  }

		  Counting();
		  DisplayWC(S_1,S_2,M_1,M_2,H_1,H_2);
		  if(rst==true){
			  setTime(S_1,S_2,M_1,M_2,H_1,H_2);
		  }
	   }

	   while(cnt==3){
		 if(emfMode==true){
			M_2++;
			HAL_Delay(200);
			emfMode=false;
		 }
		 if(M_2==6){
			M_2=0;
		 }
		Counting();
		DisplayWC(S_1,S_2,M_1,M_2,H_1,H_2);
		if(rst==true){
			setTime(S_1,S_2,M_1,M_2,H_1,H_2);
		}
	  }

   	   while(cnt==4){
         if(emfMode==true){
            M_1++;
            HAL_Delay(200);
            emfMode=false;
         }
         if(M_1==10){
            M_1=0;
         }
         Counting();
         DisplayWC(S_1,S_2,M_1,M_2,H_1,H_2);
         if(rst==true){
           	setTime(S_1,S_2,M_1,M_2,H_1,H_2);
         }
      }

   	   while(cnt==5){
         if(emfMode==true){
            S_2++;
            HAL_Delay(200);
            emfMode=false;
         }
         if(S_2==6){
            S_2=0;
         }
         Counting();
         DisplayWC(S_1,S_2,M_1,M_2,H_1,H_2);
         if(rst==true){
       	   setTime(S_1,S_2,M_1,M_2,H_1,H_2);
         }
       }

   	   while(cnt==6){
         if(emfMode==true){
            S_1++;
            HAL_Delay(200);
            emfMode=false;
         }
         if(S_1==10){
            S_1=0;
         }
         Counting();
         DisplayWC(S_1,S_2,M_1,M_2,H_1,H_2);

         if(rst==true){
       	   setTime(S_1,S_2,M_1,M_2,H_1,H_2);
         }
       }
   	   while(cnt>6){
   		   cnt=1;
   	   }
   }
}
//Watch_control function.
void magneticMode(){

	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);

   volatile uint16_t adcval1;
   volatile uint16_t adcval2;
   HAL_ADC_Start_DMA(&hadc1,&adcval1,1);
   HAL_ADC_Start_DMA(&hadc2,&adcval2,1);//ADC-DMA function.
   HAL_Delay(1000);
   int ARR_5055_REF[Size_1];
   int ARR_5056_REF[Size_1];
   int ARR_5055_MEA[Size];
   int ARR_5056_MEA[Size];//ADC sampling array variable.
   int Sum_5055_REF=0;
   int Sum_5056_REF=0;
   int Sum_5055_MEA=0;
   int Sum_5056_MEA=0;//Sampling sum variable.
   int Aver_5055_REF=0;
   int Aver_5056_REF=0;
   int Aver_5055_MEA=0;
   int Aver_5056_MEA=0;//Sampling average variable.
   int Def_5055=0;
   int Def_5056=0;//Difference variable.

   for(int i=0;i<Size_1;i++){
      ARR_5055_REF[i]=adcval1;
      ARR_5056_REF[i]=adcval2;
      Sum_5055_REF=ARR_5055_REF[i]+Sum_5055_REF;
      Sum_5056_REF=ARR_5056_REF[i]+Sum_5056_REF;
   }
   Aver_5055_REF=Sum_5055_REF/Size_1;
   Aver_5056_REF=Sum_5056_REF/Size_1;

   while(emfMode){
	   Sum_5055_MEA=0;
	   Sum_5056_MEA=0;
	   for(int i=0;i<Size;i++){
		  ARR_5055_MEA[i]=adcval1;
		  ARR_5056_MEA[i]=adcval2;
		  Sum_5055_MEA=ARR_5055_MEA[i]+Sum_5055_MEA;
		  Sum_5056_MEA=ARR_5056_MEA[i]+Sum_5056_MEA;
	   }
	   Aver_5055_MEA=Sum_5055_MEA/Size;
	   Aver_5056_MEA=Sum_5056_MEA/Size;
	   //array average
	   Def_5055 = (Aver_5055_REF - Aver_5055_MEA);
	   Def_5056 = (Aver_5056_REF - Aver_5056_MEA);
	   //difference equation
	   ST7789_drawInt(12,188,FONT_2,Def_5055,0xffff);
	   ST7789_drawInt(12,212,FONT_2,Def_5056,0xffff);
	   ST7789_drawInt(82,188,FONT_2,Aver_5055_REF,0xffff);
	   ST7789_drawInt(82,212,FONT_2,Aver_5056_REF,0xffff);
	   ST7789_drawInt(152,188,FONT_2,Aver_5055_MEA,0xffff);
	   ST7789_drawInt(152,212,FONT_2,Aver_5056_MEA,0xffff);
	   TimeDisplay(3,226,232,208,214,190,196,202,220,FONT_8);
	   //emf LCD code
	   ST7789_drawInt(12,188,FONT_2,Def_5055,0x0);
	   ST7789_drawInt(12,212,FONT_2,Def_5056,0x0);
	   //display
	   if(abs(Def_5055)>7  ||abs(Def_5056)>14){
		  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
	   }
	   else{
		  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
	   }//buzzer code
	   if(rst==true){

			 HAL_ADC_Stop_DMA(&hadc1);
			 HAL_ADC_Stop_DMA(&hadc2);
			 rst=false;
			 emfMode=false;
			 wcMode=false;
	   }//out code
	}
}
//EMF measure function.
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
