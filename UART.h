
#ifndef UART_H_
#define UART_H_
#include <stdint.h>

void initUART(void);
void initUARTWriteOnly(void);
void write_UART(char *string);
void writeDigit(uint8_t myNum);
void newLine(void);
#define RETURN 0x0D
#define TO_CHAR 0x30
#define ASCIITOCHAR '0'
#define PLACEHOLDER 'a'
#define CLEAR "aaaa"
#define UART_ENTER_NAME "Type in name and press enter: "
#define UART_START_ENTRY "Press 1 to add new voice entry: "
#define IDENTIFY "Press 2 to begin voice recognition: "
#define STANDBY 1
#define GO 0
#define FIRST 1
#define SECOND 2
#define THIRD 3
#define FOURTH 4
#define FIFTH 5
#define SIXTH 6

#endif /* UART_H_ */
