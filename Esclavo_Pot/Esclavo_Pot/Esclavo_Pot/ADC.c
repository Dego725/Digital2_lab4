#include "ADC.h"
void ADC_init(void)
{
	ADMUX |= (1<<REFS0);                         // Seleccionar AVCC como referencia
	ADMUX &= ~(1<<ADLAR);                        // Ajustar el resultado a la DERECHA para 10 bits
	ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2); // Divisor = 128 para frecuencia ADC adecuada
	ADCSRA |= (1<<ADEN);                         // Encender el ADC
}

uint16_t ADC_read(uint8_t canal)                 // Retorna valor uint16_t para 10 bits
{
	canal &= 0b00000111;                         // Limitar a canales 0-7
	ADMUX = (ADMUX & 0xF0) | canal;              // Limpiar los bits MUX y establecer canal
	ADCSRA |= (1<<ADSC);                         // Iniciar la conversión
	while(ADCSRA & (1<<ADSC));                   // Esperar a que termine la conversión
	
	return ADC;                                  // Devolver el valor completo de 10 bits
}