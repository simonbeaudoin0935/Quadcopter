#include "COM/UART/UART1.h"
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"



static volatile char UART1_rx_buf[UART1_RX_BUFF_SIZE];
static volatile char UART1_tx_buf[UART1_TX_BUFF_SIZE];

static volatile uint32_t UART1_rx_head;
static volatile uint32_t UART1_rx_tail;
static volatile uint32_t UART1_tx_head;
static volatile uint32_t UART1_tx_tail;




void UART1_init(unsigned int p_baud_rate){
  
  UART1_rx_head = 0;
  UART1_rx_tail = 0;
  UART1_tx_head = 0;
  UART1_tx_tail = 0;

  
//activer la clock du port A
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
//Configurer les pin 2 et 3 comme fonction alternative USART2
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
  
 //Initialiser les pins avec fonction alternative
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  

//Activer la clock pour le USART2
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
//Initialiser les paramètres du USART
  USART_InitTypeDef USART_InitStruct;
  USART_InitStruct.USART_BaudRate = p_baud_rate;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_Init(USART1, &USART_InitStruct);
  USART_Cmd(USART1, ENABLE);
  

//Activer interruption RX Not Empty
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  
//Activer interruption TX Empty
 // USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  

//Initialiser le NVIC pour activer l'interruption du USART1
  NVIC_SetPriority(USART1_IRQn, 6);
  NVIC_EnableIRQ(USART1_IRQn);
}

char UART1_read(void){
  
  //Si aucune données dans le rx buf, normalement ne devrait pas arriver car on appelle available()
  //pour connaitre le nombre de byte disponibles
  if(UART1_rx_head == UART1_rx_tail){
    return 0;
  }
  
  else{
    char c = UART1_rx_buf[UART1_rx_tail];                   //le prochain byte dans le buffer
    UART1_rx_tail = (UART1_rx_tail + 1) % UART1_RX_BUFF_SIZE;     //on incrémente la queue du buffer et on fait un wrap around
    return c;
  }
}

int UART1_available(){
  return (int) ((UART1_RX_BUFF_SIZE + UART1_rx_head - UART1_rx_tail) % UART1_RX_BUFF_SIZE); //retourne le nombre de byte dans le buffer
}

void UART1_write(char p_data){

  //Si le buffer est vide et que le TX_data_register aussi, on skip
  //le processus et on envoit directement le byte.
  if((UART1_tx_tail == UART1_tx_head) && (USART1->SR & USART_FLAG_TXE))
  {
    USART1->DR = p_data;
    return;
  }
  
  //sinon il y a des donnes dans le buffer ou le UART est en train de transferer le byte
  //mit dans le registre de transfert  mit lors de l'appel précédend en #1
  uint32_t v_index = (UART1_tx_head + 1) % UART1_TX_BUFF_SIZE;
  
  //on boucle tant que le buffer est plein.
  while(v_index == UART1_tx_tail)
  {
    //si on est en train de boucler ici, l'interruption de transfert finira par arriver
    //et liberera un byte dans le buffer
  }
  
  UART1_tx_buf[UART1_tx_head] = p_data;                   //On place le byte dans le buffer
  
  UART1_tx_head = v_index;                                //On incrémente la tail
    
  USART1->CR1 |= USART_FLAG_TXE;                          //Active Transmit Empty interrupt, s'il ne l'est pas déjà
}

void UART1_print(char* p_string)
{  
  while(*p_string != 0)                                   //Tant qu'on a pas atteind la fin de la string
  {
    UART1_write(*p_string);                               //On écrit le char dans le buffer.
    
    p_string++;                                           //On incrémente
  }
  
}


void USART1_IRQHandler(void)
{ 
  if(USART1->SR & USART_FLAG_RXNE)                        //Si on a recu un byte
  {
    USART1->SR &= ~USART_FLAG_RXNE;                       //clear la source de linterrupt
    
    UART1_rx_buf[UART1_rx_head] = USART1->DR;             //On place le byte recu dans le buffer
    
    UART1_rx_head = (UART1_rx_head + 1) % UART1_RX_BUFF_SIZE;   //On incrémente l'indice de tête, et fait un wrap around;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;


    extern struct{
    	TaskHandle_t TaskHandle_FlashHeartbeatLED;
    	TaskHandle_t TaskHandle_RPIReception;
    	TaskHandle_t TaskHandle_PIDLoop;
    	TaskHandle_t TaskHandle_IMURead;
    	TaskHandle_t TaskHandle_SendAttitudeMessage;
    	TaskHandle_t TaskHandle_PMUReader;
    	TaskHandle_t TaskHandle_GPSParser;
    }TaskHandles;

    vTaskNotifyGiveFromISR(TaskHandles.TaskHandle_RPIReception, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
  }
  
  if(USART1->SR & USART_FLAG_TXE)                         //if tx empty
  {            
    USART1->SR &= ~USART_FLAG_TXE;                        //On clear le flag
    
    if(UART1_tx_head != UART1_tx_tail)                    //Si il y a des bytes à envoyer dans le tx_buf
    { 
      USART1->DR = UART1_tx_buf[UART1_tx_tail];           //On envoit le prochain byte
      
      UART1_tx_tail = (UART1_tx_tail + 1) % UART1_TX_BUFF_SIZE; //On incrémente l'indice de queue, et fait un wrap around;
    }
    
    if(UART1_tx_head == UART1_tx_tail)                    //Si il n'y a plus de données dans le tx_buf
    {                   
      USART1->CR1 &= ~USART_FLAG_TXE;                     //on désactive les interruption du tx empty.
    }
  }
}
