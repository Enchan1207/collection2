//
//
//
#include <avr/io.h>
#include <util/delay.h>

#include "collection2/buffer.hpp"

int main() {
    DDRB = 0xFF;
    PORTB = 0xFF;

    while (true) {
        PORTB ^= 0x55;
        _delay_ms(1000);
    }
}
