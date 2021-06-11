/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                        Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : ShimGeonwoo
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <app_cfg.h>
#include <bsp.h>
#include <os.h>
#include <stm32f10x_usart.h>
#include <lcd.h>
#include <touch.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define APP_TASK_STK_SIZE 128
#define APP_TASK_PRIO 3
#define TASK1_CLOCK_TICK 1
#define TASK2_CLOCK_TICK 2
#define TASK3_CLOCK_TICK 4

GPIO_InitTypeDef  GPIO_InitStruct;

/*
*********************************************************************************************************
*                                           LOCAL VARIABLES
*********************************************************************************************************
*/

static  OS_TCB          AppTaskStartTCB; 

static  OS_TCB          AppTask1_TCB;
static  OS_TCB          AppTask2_TCB;
static  OS_TCB          AppTask3_TCB;

static const int THRESHOLD = 300000;
 
static int Task1_Count = 0;
static int Task2_Count = 0;
static int Task3_Count = 0;

/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/

static  CPU_STK         AppTaskStartStk[APP_TASK_START_STK_SIZE];

static  CPU_STK         AppTask1_Stk[APP_TASK_STK_SIZE];
static  CPU_STK         AppTask2_Stk[APP_TASK_STK_SIZE];
static  CPU_STK         AppTask3_Stk[APP_TASK_STK_SIZE];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskCreate     (void);
static  void  AppTaskStart      (void *p_arg);

static  void  USART_CNF         ();

static  void  AppTask1          (void *p_arg);
static  void  AppTask2          (void *p_arg);
static  void  AppTask3          (void *p_arg);
/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/
void RCC_Configure(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//TIM2 Clock enable
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
   /* Alternate Function IO clock enable */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
   
}


void GPIO_Configure(void)
{

   // TODO: Initialize the GPIO pins using the structure 'GPIO_InitTypeDef' and the function 'GPIO_Init'
   GPIO_InitTypeDef GPIO_InitStructure;
      
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Init(GPIOD, &GPIO_InitStructure);
    
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Init(GPIOD, &GPIO_InitStructure); 
}

int  main (void)
{   
    OS_ERR  err;
    BSP_IntDisAll();                                            /* Disable all interrupts.                              */
    OSInit(&err);                                               /* Init uC/OS-III.                                      */
    RCC_Configure();
    GPIO_Configure();

    LCD_Init();
    Touch_Configuration();
    Touch_Adjust();
    LCD_Clear(BLUE);
    
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* Create the start task                                */
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR )AppTaskStart, 
                 (void       *)0,
                 (OS_PRIO     )APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE)APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE)APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */
}



/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  USART_CNF() {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;
  
   (void)p_arg;
 
    BSP_Init();                                                 
    USART_CNF();
    CPU_Init();
    
    // Kernel Tick Initialization 부분
    cpu_clk_freq = BSP_CPU_ClkFreq();
    cnts         = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;
    OS_CPU_SysTickInit(cnts);
    
    #if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               
    #endif
    
    CPU_IntDisMeasMaxCurReset();
    
    AppTaskCreate();                                            
 
    BSP_LED_Off(0);
    while (DEF_TRUE) {                                          
        OSTimeDlyHMSM(0, 0, 0, 500, 
                      OS_OPT_TIME_HMSM_STRICT, 
                      &err);
    }
}


/*
*********************************************************************************************************
*                                      CREATE APPLICATION TASKS
*
* Description:  This function creates the application tasks.
*
* Arguments  :  none
*
* Returns    :  none
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
    OS_ERR  err;
    
    OSTaskCreate((OS_TCB     *)&AppTask1_TCB, 
                 (CPU_CHAR   *)"App Task1 Start",
                 (OS_TASK_PTR )AppTask1, 
                 (void       *)0,
                 (OS_PRIO     )APP_TASK_PRIO,
                 (CPU_STK    *)&AppTask1_Stk[0],
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )TASK1_CLOCK_TICK, // 보고서에서 설명하는 Task의 Tick 배정하는 부분
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
    
    // create AppTask3
    OSTaskCreate((OS_TCB     *)&AppTask3_TCB, 
                 (CPU_CHAR   *)"App Task3 Start",
                 (OS_TASK_PTR )AppTask3, 
                 (void       *)0,
                 (OS_PRIO     )APP_TASK_PRIO,
                 (CPU_STK    *)&AppTask3_Stk[0],
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )TASK3_CLOCK_TICK, // Time Quanta
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
    
    // create AppTask2
    OSTaskCreate((OS_TCB     *)&AppTask2_TCB, 
                 (CPU_CHAR   *)"App Task2 Start",
                 (OS_TASK_PTR )AppTask2, 
                 (void       *)0,
                 (OS_PRIO     )APP_TASK_PRIO,
                 (CPU_STK    *)&AppTask2_Stk[0],
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE)APP_TASK_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )TASK2_CLOCK_TICK, // Time Quanta
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
}

static void AppTask1(void *p_arg) {
    while (DEF_TRUE) {
        ++Task1_Count;
        if(Task1_Count == THRESHOLD) {
            Task1_Count = 0;
            BSP_LED_Toggle(1);
        }
        USART_SendData(USART1, (u16)'1');
    }
}
 
static void AppTask2(void *p_arg) {
    while (DEF_TRUE) {
        ++Task2_Count;
        if(Task2_Count == THRESHOLD) {
            Task2_Count = 0;
            BSP_LED_Toggle(2);
        }
        USART_SendData(USART1, (u16)'2');
    }
}
 
static void AppTask3(void *p_arg) {
    while (DEF_TRUE) {
        ++Task3_Count;
        if(Task3_Count == THRESHOLD) {
            Task3_Count = 0;
            BSP_LED_Toggle(3);
        }
        USART_SendData(USART1, (u16)'3');
    }
}