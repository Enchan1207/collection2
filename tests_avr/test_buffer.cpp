//
//
//
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include <usart/impl.hpp>

#include "collection2/buffer.hpp"

int main() {
    sei();
    Serial.begin(115200);

    while (true) {
        Serial.println("Hello, World!!");
        _delay_ms(10000);
    }
}
