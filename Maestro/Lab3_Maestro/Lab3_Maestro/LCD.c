#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"

// Pulso de habilitaci�n
void lcd_pulse_enable(void) {
	LCD_CTRL_PORT |= (1 << LCD_E);   // E = 1
	_delay_us(1);                    // Pulso m�nimo
	LCD_CTRL_PORT &= ~(1 << LCD_E);  // E = 0
	_delay_us(50);                   // Tiempo de setup
}

// Enviar comando a la LCD
void lcd_command(uint8_t cmd) {
	LCD_CTRL_PORT &= ~(1 << LCD_RS); // RS = 0 para comando
	lcd_send_data(cmd);             // Colocar comando en puerto de datos
	lcd_pulse_enable();              // Generar pulso Enable
	_delay_ms(2);                    // Tiempo de ejecuci�n del comando
}

// Enviar dato a la LCD
void lcd_data(uint8_t data) {
	LCD_CTRL_PORT |= (1 << LCD_RS);  // RS = 1 para datos
	lcd_send_data(data);            // Colocar dato en puerto de datos
	lcd_pulse_enable();              // Generar pulso Enable
	_delay_us(50);                   // Tiempo de ejecuci�n del dato
}

// Inicializar LCD
void lcd_init(void) {
	// Configurar pines como salida
	DDRD |= 0xFF;  // Bits 7,6,5,4 del PORTD como salida (parte alta)
	//DDRB |= 0x0F;  // Bits 3,2,1,0 del PORTB como salida (parte baja)
	LCD_CTRL_DDR |= (1 << LCD_RS) | (1 << LCD_E); // RS y E como salida
	
	// Esperar estabilizaci�n de la alimentaci�n
	_delay_ms(20);
	
	// Secuencia de inicializaci�n por software (seg�n datasheet HD44780)
	lcd_command(0x30);  // Function set: 8-bit
	_delay_ms(5);
	lcd_command(0x30);  // Function set: 8-bit
	_delay_us(100);
	lcd_command(0x30);  // Function set: 8-bit
	
	// Configuraci�n final de la LCD
	lcd_command(LCD_FUNCTION_SET);   // 8-bit, 2 l�neas, 5x7 dots
	lcd_command(LCD_DISPLAY_OFF);    // Display off
	lcd_command(LCD_CLEAR);          // Clear display
	lcd_command(LCD_ENTRY_MODE);     // Entry mode: incrementar cursor
	lcd_command(LCD_DISPLAY_ON);     // Display on, cursor off
}

// Mostrar cadena de caracteres
void lcd_string(char* str) {
	while(*str) {
		lcd_data(*str++);
	}
}

// Posicionar cursor
void lcd_set_cursor(uint8_t row, uint8_t col) {
	uint8_t address;
	if(row == 0) {
		address = 0x00 + col;  // Primera l�nea
		} else {
		address = 0x40 + col;  // Segunda l�nea
	}
	lcd_command(LCD_SET_CURSOR | address);
}

// Limpiar pantalla
void lcd_clear(void) {
	lcd_command(LCD_CLEAR);
	_delay_ms(2);
}

void lcd_send_data(uint8_t data) {
	// Limpiar solo los bits que vamos a usar
/*	PORTD &= 0x0F;  // Mantener bits 3,2,1,0 del PORTD, limpiar 7,6,5,4
	PORTB &= 0xF0;  // Mantener bits 7,6,5,4 del PORTB, limpiar 3,2,1,0
	
	// Distribuir los datos
	PORTD |= (data & 0xF0);        // Bits 7,6,5,4 van a PORTD
	PORTB |= (data & 0x0F);        // Bits 3,2,1,0 van a PORTB
	*/
	PORTD = 0x00;
	PORTD = data;
}