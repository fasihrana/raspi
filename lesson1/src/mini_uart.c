#include "utils.h"
#include "peripherals/mini_uart.h"
#include "peripherals/gpio.h"

void uart_send(char c)
{
	while(1)
	{
		if (get32(AUX_MU_LSR_REG)&0x20)
			break;
	}
	put32(AUX_MU_IO_REG, c);
}

char uart_recv(void)
{
	while(1)
	{
		if (get32(AUX_MU_LSR_REG)&0x20)
			break;
	}
	return (get32(AUX_MU_IO_REG)&0xFF);
}

void uart_send_string(char* str)
{
	for (int i=0; str[i] != '\0'; i++)
	{
		uart_send((char)str[i]);
	}
}

void uart_init()
{
	unsigned int selector;

	selector = get32(GPFSEL1);
	selector &= ~(7<<12);		// clean gpio4
	selector |= 2<<12;		// set alt5 for gpio4
	selector &= ~(7<<15);		// clean gpio5
	selector |= 2<<15;		// set alt5 for gpio5
	put32(GPFSEL1,selector);

	selector = get32(GP_PUP_PDN_CNTRL_REG0);
	selector &= 0x0FFFFFFF;
	put32(GP_PUP_PDN_CNTRL_REG0, selector);

	put32(AUX_ENABLES, 1);		// enable mini_uart
	put32(AUX_MU_CNTL_REG, 0);	// disable auto flow control, receiver and transmitter (for now)
	put32(AUX_MU_IER_REG, 0);	// disable receive and transmit interrupts
	put32(AUX_MU_LCR_REG, 3);	// enable 8 bit mode
	put32(AUX_MU_MCR_REG, 0);	// set RTS line to be always high
	put32(AUX_MU_BAUD_REG, 541);	// set baud rate to 115200

	put32(AUX_MU_CNTL_REG, 3);	// finally enable transmitter and receiver
}
