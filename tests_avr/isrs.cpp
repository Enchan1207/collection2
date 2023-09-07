//
// ISR定義
//
#include <avr/interrupt.h>

#include <usart/impl.hpp>

/**
 * 送信バッファ空き
 */
ISR(USART_UDRE_vect) {
    usart::USART0::shared().onSendBufferEmpty();
}

/**
 * 受信完了
 */
ISR(USART_RX_vect) {
    usart::USART0::shared().onReceive();
}
