//
//
//
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include <uart/uart.hpp>

#include "collection2/buffer.hpp"

using namespace uart;

int main() {
    sei();
    Serial.begin(115200);

    while (true) {
        Serial.println("Hello, World!!");
        _delay_ms(10000);
    }
}
