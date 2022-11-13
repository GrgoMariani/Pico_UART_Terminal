#include "terminal.h"

#include "mainLoop.h"
#include "interface.h"
#include "gameshared.h"
#include "gpu.h"

#include <stdio.h>
#include <string.h>

uint8_t termarray[ROWS][COLUMNS];

uint16_t current_x;
uint16_t current_y;




#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"


/// \tag::uart_advanced[]

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

static int chars_rxed = 0;




void add_row()
{
    current_x = 0;
    current_y++;
    if (current_y >= ROWS)
    {
        for (uint16_t i=0; i<ROWS-1; i++)
        memcpy(&termarray[i][0], &termarray[i+1][0], COLUMNS*sizeof(uint8_t));
        current_y--;
    }
    memset(&termarray[ROWS-1][0], 0, COLUMNS);
}

void term_add(uint8_t c)
{
    if (c == '\n' || c == '\r')
    {
        add_row();
        return;
    }
    termarray[current_y][current_x] = c;
    current_x++;
    if (current_x >= COLUMNS)
    {
        add_row();
        current_x = 0;
    }
}



// RX interrupt handler
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        term_add(ch);
    }
}

void Term_Init()
{
    current_x = 0;
    current_y = 0;
    for (uint16_t i=0; i<ROWS; i++)
        for (uint16_t j=0; j<COLUMNS; j++)
            termarray[i][j] = 0;
    
    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, 2400);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Actually, we want a different speed
    // The call will return the actual baud rate selected, which will be as close as
    // possible to that requested
    int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);
    char first_message[26] = "---SCREEN INITIALIZED---\n";
    for (int i=0 ; i<25; i++)
        term_add(first_message[i]);
}

void Term_Update()
{

}


void Term_Draw()
{
    GPU_ClearFramebuffers();
    for (uint16_t i=0; i<ROWS; i++)
        for (uint16_t j=0; j<COLUMNS; j++)
            GPU_DrawLetter(C_WHITE, &fontDescription[0], j*F_W, i*F_H, termarray[i][j]);
}

const Callbacks Term_Callbacks = { &Term_Init, &Term_Update, &Term_Draw, NULL };