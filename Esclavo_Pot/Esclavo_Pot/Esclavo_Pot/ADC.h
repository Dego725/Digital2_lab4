/*
 * ADC.h
 *
 * Created: 8/05/2022 18:50:39
 *  Author: carlo
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

void ADC_init(void);
uint16_t  ADC_read(uint8_t canal);


#endif /* ADC_H_ */