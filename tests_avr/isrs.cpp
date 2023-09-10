//
// ISR定義
//
#include <avr/interrupt.h>

#include <uart/uart.hpp>

using namespace uart;

/**
 * 送信バッファ空き
 */
ISR(USART_UDRE_vect) {
    Serial0.onSendBufferEmpty();
}

/**
 * 受信完了
 */
ISR(USART_RX_vect) {
    Serial0.onReceive();
}
