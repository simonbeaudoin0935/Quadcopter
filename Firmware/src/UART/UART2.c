#include "UART/UART2.h"
#include "stm32f4xx.h"


static volatile char UART2_rx_buf[UART2_RX_BUFF_SIZE];
static volatile char UART2_tx_buf[UART2_TX_BUFF_SIZE];

static volatile uint32_t UART2_rx_head;
static volatile uint32_t UART2_rx_tail;
static volatile uint32_t UART2_tx_head;
static volatile uint32_t UART2_tx_tail;




void UART2_init(unsigned int p_baud_rate){
  
  UART2_rx_head = 0;
  UART2_rx_tail = 0;
  UART2_tx_head = 0;
  UART2_tx_tail = 0;

  
//activer la clock du port A
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
//Configurer les pin 2 et 3 comme fonction alternative USART2
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
  
 //Initialiser les pins avec fonction alternative
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  

//Activer la clock pour le USART2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  
//Initialiser les paramètres du USART
  USART_InitTypeDef USART_InitStruct;
  USART_InitStruct.USART_BaudRate = p_baud_rate;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_Init(USART2, &USART_InitStruct);
  USART_Cmd(USART2, ENABLE);
  

//Activer interruption RX Not Empty
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  
//Activer interruption TX Empty
  USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
  

//Initialiser le NVIC pour activer l'interruption du USART2
  NVIC_InitTypeDef NVIC_InitStruct;
  NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;

  NVIC_Init(&NVIC_InitStruct);
  

}

char UART2_read(void){
  
  //Si aucune données dans le rx buf, normalement ne devrait pas arriver car on appelle available()
  //pour connaitre le nombre de byte disponibles
  if(UART2_rx_head == UART2_rx_tail){
    return 0;
  }
  
  else{
    char c = UART2_rx_buf[UART2_rx_tail];                   //le prochain byte dans le buffer
    UART2_rx_tail = (UART2_rx_tail + 1) % UART2_RX_BUFF_SIZE;     //on incrémente la queue du buffer et on fait un wrap around
    return c;
  }
}

int UART2_available(){
  return (int) ((UART2_RX_BUFF_SIZE + UART2_rx_head - UART2_rx_tail) % UART2_RX_BUFF_SIZE); //retourne le nombre de byte dans le buffer
}

void UART2_write(char p_data){

  //Si le buffer est vide et que le TX_data_register aussi, on skip
  //le processus et on envoit directement le byte.
  if((UART2_tx_tail == UART2_tx_head) && (USART2->SR & USART_FLAG_TXE))
  {
    USART2->DR = p_data;
    return;
  }
  
  //sinon il y a des donnes dans le buffer ou le UART est en train de transferer le byte
  //mit dans le registre de transfert  mit lors de l'appel précédend en #1
  uint32_t v_index = (UART2_tx_head + 1) % UART2_TX_BUFF_SIZE;
  
  //on boucle tant que le buffer est plein.
  while(v_index == UART2_tx_tail)
  {
    //si on est en train de boucler ici, l'interruption de transfert finira pas arriver
    //et liberera un byte dans le buffer
  }
  
  UART2_tx_buf[UART2_tx_head] = p_data;                   //On place le byte dans le buffer
  
  UART2_tx_head = v_index;                                //On incrémente la tail
    
  USART2->CR1 |= USART_FLAG_TXE;                          //Active Transmit Empty interrupt, s'il ne l'est pas déjà
}


void UART2_print(char* p_string)
{  
  while(*p_string != 0)                                   //Tant qu'on a pas atteind la fin de la string
  {
    UART2_write(*p_string);                               //On écrit le char dans le buffer.
    
    p_string++;                                           //On incrémente
  }
  
}


void UART2_writeFloatUnion(float p_float)
{
	floatUnion_t v_floatUnion;

	v_floatUnion.floating = p_float;

	UART2_write(v_floatUnion.bytes[0]);
	UART2_write(v_floatUnion.bytes[1]);
	UART2_write(v_floatUnion.bytes[2]);
	UART2_write(v_floatUnion.bytes[3]);
}
void UART2_writeIntegerUnion(int p_integer)
{
	intUnion_t v_intUnion;

	v_intUnion.integer = p_integer;

	UART2_write(v_intUnion.bytes[0]);
	UART2_write(v_intUnion.bytes[1]);
	UART2_write(v_intUnion.bytes[2]);
	UART2_write(v_intUnion.bytes[3]);
}

void USART2_IRQHandler(void)
{ 
  if(USART2->SR & USART_FLAG_RXNE)                        //Si on a recu un byte
  {
    USART2->SR &= ~USART_FLAG_RXNE;                       //clear la source de linterrupt
    
    UART2_rx_buf[UART2_rx_head] = USART2->DR;             //On place le byte recu dans le buffer
    
    UART2_rx_head = (UART2_rx_head + 1) % UART2_RX_BUFF_SIZE;   //On incrémente l'indice de tête, et fait un wrap around;
  }
  
  if(USART2->SR & USART_FLAG_TXE)                         //if tx empty
  {            
    USART2->SR &= ~USART_FLAG_TXE;                        //On clear le flag
    
    if(UART2_tx_head != UART2_tx_tail)                    //Si il y a des bytes à envoyer dans le tx_buf
    { 
      USART2->DR = UART2_tx_buf[UART2_tx_tail];           //On envoit le prochain byte
      
      UART2_tx_tail = (UART2_tx_tail + 1) % UART2_TX_BUFF_SIZE; //On incrémente l'indice de queue, et fait un wrap around;
    }
    
    if(UART2_tx_head == UART2_tx_tail)                    //Si il n'y a plus de données dans le tx_buf
    {                   
      USART2->CR1 &= ~USART_FLAG_TXE;                     //on désactive les interruption du tx empty.
    }
  }
}
