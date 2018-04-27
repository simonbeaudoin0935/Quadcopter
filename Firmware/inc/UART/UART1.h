#ifndef UART1_H
#define UART1_H

#define UART1_RX_BUFF_SIZE 100
#define UART1_TX_BUFF_SIZE 100

typedef union{
	float floating;
	char  bytes[sizeof(float)];
}floatUnion_t;

typedef union{
	int integer;
	char bytes[sizeof(int)];
}intUnion_t;



void UART1_init(unsigned int p_baud_rate);
char UART1_read();
int  UART1_available();
void UART1_write(char p_data);
void UART1_print(char* p_string);

void UART1_writeFloatUnion(float p_float);
void UART1_writeIntegerUnion(int p_integer);


#endif //UART2_H
