#pragma once
// clang-format off

/*
    polyshaper.h
    https://github.com/FYSETC/FYSETC-E4

    2021-02-06 Piergiorgio Vagnozzi

    This is a machine definition file to use the Polyshaper board controller
    This is a 2 motor controller. This is setup for XY.
    The controller has outputs for a Hotbed.

    Grbl_ESP32 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Grbl is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Grbl_ESP32.  If not, see <http://www.gnu.org/licenses/>.
*/

#define MACHINE_NAME                            "polyshaper"
#define CUSTOM_CODE_FILENAME                    "Custom/polyshaper.cpp"
#define POLYSHAPER_INFO                         "[PolyShaper Azul][P_C_2-125-65-10-1.0-2 202020202 1.0]"

#define N_AXIS                                  2

#define TRINAMIC_UART_RUN_MODE                  TrinamicUartMode::StealthChop
#define TRINAMIC_UART_HOMING_MODE               TrinamicUartMode::StallGuard

/* GRBL Settings */
#define USE_MACHINE_INIT
//#define USE_MACHINE_TRINAMIC_INIT
//#define USE_M30
//#define USE_CUSTOM_HOMING

/***********************/
/* Polyshaper Board R4 */
/***********************/

/* TMC_2209 */
#define X_TRINAMIC_DRIVER                       2209
#define X_STEP_PIN                              GPIO_NUM_16
#define X_DIRECTION_PIN                         GPIO_NUM_5
#define X_RSENSE                                0.1
#define X_DRIVER_ADDRESS                        0
#define DEFAULT_X_MICROSTEPS                    8
#define DEFAULT_X_CURRENT                       0.025
#define DEFAULT_X_HOLD_CURRENT                  0.025

#define Y_TRINAMIC_DRIVER                       2209
#define Y_STEP_PIN                              GPIO_NUM_15
#define Y_DIRECTION_PIN                         GPIO_NUM_13
#define Y_RSENSE                                0.1
#define Y_DRIVER_ADDRESS                        1
#define DEFAULT_Y_MICROSTEPS                    8
#define DEFAULT_Y_CURRENT                       0.025
#define DEFAULT_Y_HOLD_CURRENT                  0.025

#define SPINDLE_TYPE                            SpindleType::NONE

#define X_LIMIT_PIN                             GPIO_NUM_19
#define Y_LIMIT_PIN                             GPIO_NUM_17

#define TMC2209_STDBY_PIN                       GPIO_NUM_14
#define TMC2209_SERIAL1_PIN                     GPIO_NUM_18
#define TMC2209_EN_N_PIN                        GPIO_NUM_2

#define LED_PIN                                 GPIO_NUM_27
#define SWITCH_N_PIN                            GPIO_NUM_0

#define POWER_INTERRUPT_PIN                     GPIO_NUM_4

#define TMC_UART                                UART_NUM_1
#define TMC_UART_RX                             TMC2209_SERIAL1_PIN
#define TMC_UART_TX                             TMC2209_SERIAL1_PIN

#define LED_BLINK_DELAY_FAST                    250
#define LED_BLINK_DELAY_NORMAL                  500

/* HotWire */
#define HOTWIRE_PWM_PIN				            GPIO_NUM_12
#define HOTWIRE_CURRENT_ADC_CHANNEL             ADC1_GPIO35_CHANNEL
#define HOTWIRE_VOLTAGE_ADC_CHANNEL             ADC1_GPIO34_CHANNEL

#ifndef __POLYSHAPER_H__
#define __POLYSHAPER_H__


#endif
