#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "I2C.h"

#define SLAVE_ADDRESS 0x20  // Direcci�n I2C

volatile uint8_t counter = 0;
uint8_t buffer = 0;

void setup_ports() {
	// Configurar PB4-PB1 como salidas para LEDs
	DDRB |= (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1);
	
	// Configurar PD2 y PD3 como entradas con pull-up (botones)
	DDRD &= ~((1 << PD2) | (1 << PD3));
	PORTD |= (1 << PD2) | (1 << PD3);
	I2C_Slave_Init(SLAVE_ADDRESS);
	sei();
}

ISR(TWI_vect) {
	uint8_t estado = TWSR & 0xF8;  // M�scara correcta para estados I2C
	
	switch(estado) {
		// Escritura (maestro -> esclavo)
		case 0x60: // SLA+W recibido, ACK enviado
		case 0x70: // GENERAL CALL recibido, ACK enviado
		TWCR |= (1 << TWINT) | (1 << TWEA); // ACK y limpia bandera
		break;
		
		case 0x80: // Dato recibido, ACK enviado
		case 0x90: // Dato recibido despu�s de GENERAL CALL
		//i2c_reg_addr = TWDR; // Guarda la direcci�n del registro solicitado
		TWCR |= (1 << TWINT) | (1 << TWEA); // ACK y limpia bandera
		break;
		
		// Lectura (esclavo -> maestro)
		case 0xA8: // SLA+R recibido, ACK enviado
		TWDR = counter; // Carga dato a enviar
		TWCR |= (1 << TWINT) | (1 << TWEA); // Env�a dato
		break;
		
		case 0xB8: // Dato transmitido, ACK recibido
		//i2c_reg_addr++; // Incrementa registro autom�tico (opcional)
		TWDR = counter; // Carga siguiente dato
		TWCR |= (1 << TWINT) | (1 << TWEA); // Env�a dato
		break;
		
		case 0xC0: // Dato transmitido, NACK recibido (fin de transmisi�n)
		TWCR |= (1 << TWINT); // Solo limpia bandera
		break;
		
		// Manejo de errores
		case 0x00: // Bus error
		default:
		TWCR |= (1 << TWINT) | (1 << TWSTO); // Libera el bus
		break;
	}
}

void check_buttons() {
	static uint8_t last_state = 0xFF;
	uint8_t current_state = PIND & ((1 << PD2) | (1 << PD3)); // Estados de PD2 y PD3

	if (current_state != last_state) {  // Si hubo cambio
		_delay_ms(20);  // Anti-rebote (20ms)

		// L�gica de botones:
		if (!(PIND & (1 << PD2))) {  // Bot�n PD2 presionado (LOW)
			counter = (counter < 15) ? counter + 1 : 15;  // Incrementa (m�x. 15)
		}
		if (!(PIND & (1 << PD3))) {  // Bot�n PD3 presionado (LOW)
			counter = (counter > 0) ? counter - 1 : 0;    // Decrementa (m�n. 0)
		}

		// Actualiza LEDs (PB4-PB1):
		PORTB = (PORTB & 0xE0) | ((counter & 0x0F) << 1);
		last_state = current_state;  // Guarda el nuevo estado
	}
}

int main() {
	setup_ports();
	
	while(1) {
		check_buttons();
		_delay_ms(10);
	}
}