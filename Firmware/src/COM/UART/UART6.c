#include "COM/UART/UART6.h"
#include "stm32f4xx.h"


static volatile char UART6_rx_buf[UART6_RX_BUFF_SIZE];
static volatile char UART6_tx_buf[UART6_TX_BUFF_SIZE];

static volatile uint32_t UART6_rx_head;
static volatile uint32_t UART6_rx_tail;
static volatile uint32_t UART6_tx_head;
static volatile uint32_t UART6_tx_tail;




void UART6_init(unsigned int p_baud_rate){
  
  UART6_rx_head = 0;
  UART6_rx_tail = 0;
  UART6_tx_head = 0;
  UART6_tx_tail = 0;

  
//activer la clock du port A
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  
//Configurer les pin 2 et 3 comme fonction alternative USART2
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
  
 //Initialiser les pins avec fonction alternative
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOC, &GPIO_InitStruct);
  

//Activer la clock pour le USART2
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
  
//Initialiser les paramètres du USART
  USART_InitTypeDef USART_InitStruct;
  USART_InitStruct.USART_BaudRate = p_baud_rate;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_Init(USART6, &USART_InitStruct);
  USART_Cmd(USART6, ENABLE);
  

//Activer interruption RX Not Empty
  USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
  
//Activer interruption TX Empty
  USART_ITConfig(USART6, USART_IT_TXE, ENABLE);
  

  NVIC_SetPriority(USART6_IRQn, 6);
  NVIC_EnableIRQ(USART6_IRQn);
  

}

char UART6_read(void){
  
  //Si aucune données dans le rx buf, normalement ne devrait pas arriver car on appelle available()
  //pour connaitre le nombre de byte disponibles
  if(UART6_rx_head == UART6_rx_tail){
    return 0;
  }
  
  else{
    char c = UART6_rx_buf[UART6_rx_tail];                   //le prochain byte dans le buffer
    UART6_rx_tail = (UART6_rx_tail + 1) % UART6_RX_BUFF_SIZE;     //on incrémente la queue du buffer et on fait un wrap around
    return c;
  }
}

int UART6_available(){
  return (int) ((UART6_RX_BUFF_SIZE + UART6_rx_head - UART6_rx_tail) % UART6_RX_BUFF_SIZE); //retourne le nombre de byte dans le buffer
}

void UART6_write(char p_data){

  //Si le buffer est vide et que le TX_data_register aussi, on skip
  //le processus et on envoit directement le byte.
  if((UART6_tx_tail == UART6_tx_head) && (USART6->SR & USART_FLAG_TXE))
  {
    USART6->DR = p_data;
    return;
  }
  
  //sinon il y a des donnes dans le buffer ou le UART est en train de transferer le byte
  //mit dans le registre de transfert  mit lors de l'appel précédend en #1
  uint32_t v_index = (UART6_tx_head + 1) % UART6_TX_BUFF_SIZE;
  
  //on boucle tant que le buffer est plein.
  while(v_index == UART6_tx_tail)
  {
    //si on est en train de boucler ici, l'interruption de transfert finira pas arriver
    //et liberera un byte dans le buffer
  }
  
  UART6_tx_buf[UART6_tx_head] = p_data;                   //On place le byte dans le buffer
  
  UART6_tx_head = v_index;                                //On incrémente la tail
    
  USART6->CR1 |= USART_FLAG_TXE;                          //Active Transmit Empty interrupt, s'il ne l'est pas déjà
}


void UART6_print(char* p_string)
{  
  while(*p_string != 0)                                   //Tant qu'on a pas atteind la fin de la string
  {
    UART6_write(*p_string);                               //On écrit le char dans le buffer.
    
    p_string++;                                           //On incrémente
  }
  
}


void UART6_writeFloatUnion(float p_float)
{

	union{
		float floating;
		char  bytes[sizeof(float)];
	}v_floatUnion;


	v_floatUnion.floating = p_float;

	UART6_write(v_floatUnion.bytes[0]);
	UART6_write(v_floatUnion.bytes[1]);
	UART6_write(v_floatUnion.bytes[2]);
	UART6_write(v_floatUnion.bytes[3]);
}
void UART6_writeIntegerUnion(int p_integer)
{
    union{
		int integer;
		char bytes[sizeof(int)];
	}v_intUnion;

	v_intUnion.integer = p_integer;

	UART6_write(v_intUnion.bytes[0]);
	UART6_write(v_intUnion.bytes[1]);
	UART6_write(v_intUnion.bytes[2]);
	UART6_write(v_intUnion.bytes[3]);
}

void USART6_IRQHandler(void)
{ 
  if(USART6->SR & USART_FLAG_RXNE)                        //Si on a recu un byte
  {
    USART6->SR &= ~USART_FLAG_RXNE;                       //clear la source de linterrupt
    
    UART6_rx_buf[UART6_rx_head] = USART6->DR;             //On place le byte recu dans le buffer
    
    UART6_rx_head = (UART6_rx_head + 1) % UART6_RX_BUFF_SIZE;   //On incrémente l'indice de tête, et fait un wrap around;
  }
  
  if(USART6->SR & USART_FLAG_TXE)                         //if tx empty
  {            
    USART6->SR &= ~USART_FLAG_TXE;                        //On clear le flag
    
    if(UART6_tx_head != UART6_tx_tail)                    //Si il y a des bytes à envoyer dans le tx_buf
    { 
      USART6->DR = UART6_tx_buf[UART6_tx_tail];           //On envoit le prochain byte
      
      UART6_tx_tail = (UART6_tx_tail + 1) % UART6_TX_BUFF_SIZE; //On incrémente l'indice de queue, et fait un wrap around;
    }
    
    if(UART6_tx_head == UART6_tx_tail)                    //Si il n'y a plus de données dans le tx_buf
    {                   
      USART6->CR1 &= ~USART_FLAG_TXE;                     //on désactive les interruption du tx empty.
    }
  }
}
