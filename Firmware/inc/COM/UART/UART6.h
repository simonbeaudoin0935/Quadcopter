#ifndef UART6_H
#define UART6_H

#define UART6_RX_BUFF_SIZE 100
#define UART6_TX_BUFF_SIZE 100

void UART6_init(unsigned int p_baud_rate);
char UART6_read();
int  UART6_available();
void UART6_write(char p_data);
void UART6_print(char* p_string);

void UART6_writeFloatUnion(float p_float);
void UART6_writeIntegerUnion(int p_integer);


#endif //UART2_H
