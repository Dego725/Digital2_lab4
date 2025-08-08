#ifndef INCFILE1_H_
#define INCFILE1_H_

#define I2C_MT_SLA_ACK  0x18  // SLA+W transmitido, ACK recibido
#define I2C_MT_DATA_ACK 0x28  // Datos transmitidos, ACK recibido
#define I2C_MR_SLA_ACK  0x40  // SLA+R transmitido, ACK recibido
#define I2C_MR_DATA_NACK 0x58  // Datos recibidos, NACK enviado

#include <avr/io.h>
#include <stdint.h>

// Funcion para inicializar I2C Maestro
void I2C_Master_Init(unsigned long SCL_Clock, uint8_t Prescaler);

// Funcion de inicio de la comunicacion I2C
void I2C_Master_Start(void);

// Funcion de parada de la comunicacion I2C
void I2C_Master_Stop(void);

// Funcion de transmision de datos del maestro al esclavo
// (Devolvera un 0 si el esclavo ha recibido el dato)
uint8_t I2C_Master_Write(uint8_t dato);

// Funcion de recepcion de datos enviados por el esclavo al maestro
// (Lee los datos que estan en el esclavo)
uint8_t I2C_Master_Read(uint8_t *buffer, uint8_t ack);

// Funcion para inicializar I2C Esclavo
void I2C_Slave_Init(uint8_t address);



#endif /* INCFILE1_H_ */