//
// ISR定義
//
#include <avr/interrupt.h>

#include "uart/uart.hpp"

using namespace uart;

#ifdef HAS_UART_0

#ifdef USART0_UDRE_vect
ISR(USART0_UDRE_vect) {
#else
ISR(USART_UDRE_vect) {
#endif
    Serial0.onSendBufferEmpty();
}

#ifdef USART0_RX_vect
ISR(USART0_RX_vect) {
#else
ISR(USART_RX_vect) {
#endif
    Serial0.onReceive();
}

#endif

#ifdef HAS_UART_1
ISR(USART1_UDRE_vect) {
    Serial1.onSendBufferEmpty();
}

ISR(USART1_RX_vect) {
    Serial1.onReceive();
}
#endif

#ifdef HAS_UART_2
ISR(USART2_UDRE_vect) {
    Serial2.onSendBufferEmpty();
}

ISR(USART2_RX_vect) {
    Serial2.onReceive();
}
#endif

#ifdef HAS_UART_3
ISR(USART3_UDRE_vect) {
    Serial3.onSendBufferEmpty();
}

ISR(USART3_RX_vect) {
    Serial3.onReceive();
}
#endif
