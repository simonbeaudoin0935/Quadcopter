#ifndef UART6_H
#define UART6_H

#define UART6_RX_BUFF_SIZE 100
#define UART6_TX_BUFF_SIZE 100

typedef union{
	float floating;
	char  bytes[sizeof(float)];
}floatUnion_t;

typedef union{
	int integer;
	char bytes[sizeof(int)];
}intUnion_t;



void UART6_init(unsigned int p_baud_rate);
char UART6_read();
int  UART6_available();
void UART6_write(char p_data);
void UART6_print(char* p_string);

void UART6_writeFloatUnion(float p_float);
void UART6_writeIntegerUnion(int p_integer);


#endif //UART2_H
