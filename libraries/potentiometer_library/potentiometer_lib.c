#include <avr/io.h>

void initADC()
{
  ADMUX |= (1 << REFS0);                                // Set reference voltage to 5V
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescaler to 128 for sample rate
  ADCSRA |= (1 << ADEN);                                // Enable ADC
}

uint16_t readPotentiometer()
{
  ADCSRA |= (1 << ADSC); // Start analog to digital conversion
  while (ADCSRA & (1 << ADSC))
    ;                   // Wait for conversion to complete
  uint16_t value = ADC; // Read the result
  return value;
}
