/*
 * gpio.h
 *
 *  Created on: Sep 7, 2015
 *      Author: ryochack
 *
 * @note
 * MIT License
 */

#ifndef GPIO_H_
#define GPIO_H_

#if defined (__USE_LPCOPEN)
#include "chip.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void gpio_init(void);
void gpio_set_mode(uint8_t pin, CHIP_PIN_MODE_T mode);
void gpio_set_dir(uint8_t pin, bool output);
bool gpio_get_dir(uint8_t pin);
void gpio_write(uint8_t pin, bool level);
bool gpio_read(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif /* SHELL_H_ */

