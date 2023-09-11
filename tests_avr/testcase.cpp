//
//
//
#include "testcase.hpp"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stddef.h>
#include <stdlib.h>

#include <uart/uart.hpp>

using namespace uart;
using namespace collection2tests;

int main() {
    sei();
    Serial.begin(115200);

    bool allPassed = true;
    for (size_t i = 0; i < testCount; i++) {
        Serial.print_P(PSTR("Testcase #"));
        Serial.println(i);
        bool result = tests[i]();
        if (!result) {
            Serial.println_P(PSTR("  FAIL"));
            allPassed = false;
        } else {
            Serial.println_P(PSTR("  PASS"));
        }
    }

    if (allPassed) {
        Serial.println_P(PSTR("All testcases passed."));
    } else {
        Serial.println_P(PSTR("Some testcases failed."));
    }

    abort();
}

bool collection2tests::expect(const char* const expressionStr, bool expression) {
    if (!expression) {
        Serial.print_P(PSTR("Assertion failed. expression: "));
        Serial.println(expressionStr);
    }
    return expression;
}
