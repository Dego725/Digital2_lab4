#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "I2C.h"
#include <stdlib.h>

#define SLAVE_ADDRESS1 0x20  // Dirección I2C del esclavo
#define SLAVE_ADDRESS2 0x30

uint8_t datoRecibido = 0;
uint8_t contador = 0;  // Usamos uint8_t para coincidir con el esclavo
uint8_t adc = 1;

void I2C_Reset_Bus() {
	TWCR = 0; // Deshabilita el módulo I2C
	_delay_us(10);
	TWCR = (1 << TWEN); // Reinicia el módulo
}

uint8_t I2C_Read_Slave(uint8_t slave_address, uint8_t *data) {
	uint8_t status = 0;
	uint8_t address_byte = (slave_address << 1) | 0x01;  // Modo lectura

	I2C_Reset_Bus();  // Limpia el bus antes de iniciar
	
	I2C_Master_Start();
	
	// Envía dirección + modo lectura
	status = I2C_Master_Write(address_byte);
	if(status != 1) {
		I2C_Master_Stop();
		return 0;  // Error al enviar dirección
	}
	
	// Lee el dato
	status = I2C_Master_Read(data, 1);  // Lee 1 byte con ACK
	if(status != 1) {
		I2C_Master_Stop();
		return 0;  // Error en lectura
	}
	
	I2C_Master_Stop();
	return 1;  // Éxito
}

void uint8_to_string(uint8_t num, char* str) {
	// Convertimos el número a string directamente
	itoa(num, str, 10);
	
	// Aseguramos que ocupe 3 caracteres (espacios para números <100)
	uint8_t len = 0;
	while(str[len] != '\0') len++;
	
	while(len < 3) {
		str[len++] = ' ';
	}
	str[len] = '\0';
}

void lcd_display_refresh(void) {
	char buffer1[5] = {0};
	char buffer2[5] = {0};	
	uint8_to_string(contador, buffer1);
	uint8_to_string(adc, buffer2);
	
	lcd_set_cursor(1, 1);
	lcd_string(buffer1);
	lcd_set_cursor(1, 8);
	lcd_string(buffer2);
}

int main(void) {
	DDRB |= (1<<DDB5);
	I2C_Master_Init(100000, 1);
	lcd_init();
	lcd_clear();
	lcd_set_cursor(0, 1);
	lcd_string("S1:");
	lcd_set_cursor(0, 8);
	lcd_string("S2:");
	lcd_display_refresh();
	
	while(1) {
		if(I2C_Read_Slave(SLAVE_ADDRESS1, &contador)) {
			// contador ahora tiene el valor leído
		}
		_delay_ms(20);
		// Lectura del segundo esclavo
		if(I2C_Read_Slave(SLAVE_ADDRESS2, &adc)) {
			// potenciometro ahora tiene el valor leído
		}
		_delay_ms(20);
		lcd_display_refresh();
		_delay_ms(20);
	}
}
