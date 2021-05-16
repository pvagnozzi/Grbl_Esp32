#pragma once

/*
	HotWireSpindle.h

	This is a full featured TTL PWM spindle This does not include speed/power
	compensation. Use the Laser class for that.

	Part of Grbl_ESP32
	2021 -	Piergiorgio Vagnozzi

	Grbl is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	Grbl is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with Grbl.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Spindle.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#ifndef HOTWIRE_CURRENT_SHUNT_VAL
#define HOTWIRE_CURRENT_SHUNT_VAL				0.1
#endif 

#ifndef HOTWIRE_CURRENT_AMP_GAIN
#define HOTWIRE_CURRENT_AMP_GAIN				50.0
#endif

#ifndef HOTWIRE_VOLTAGE_OVERSAMPLE_N
#define HOTWIRE_VOLTAGE_OVERSAMPLE_N			256
#endif

#ifndef HOTWIRE_VOLTAGE_DIV_R_HI
#define HOTWIRE_VOLTAGE_DIV_R_HI				10.0
#endif 

#ifndef HOTWIRE_VOLTAGE_DIV_R_LO
#define HOTWIRE_VOLTAGE_DIV_R_LO				3.3
#endif

#ifndef HOTWIRE_ERROR_THRESHOLD
#define HOTWIRE_ERROR_THRESHOLD					40
#endif

#ifndef HOTWIRE_ERROR_MAX_COUNT
#define HOTWIRE_ERROR_MAX_COUNT					5
#endif

#ifndef HOTWIRE_LOOP_PERIOD_MS
#define HOTWIRE_LOOP_PERIOD_MS					5
#endif 

#ifndef HOTWIRE_K_INTEGRAL
#define HOTWIRE_K_INTEGRAL						25.0
#endif

#ifndef HOTWIRE_K_PROPORTIONAL
#define HOTWIRE_K_PROPORTIONAL					2.5
#endif

#ifndef HOTWIRE_FEEDBACK_RES
#define HOTWIRE_FEEDBACK_RES					ADC_WIDTH_BIT_12
#endif

#ifndef HOTWIRE_FEEDBACK_VREF
#define HOTWIRE_FEEDBACK_VREF					1110
#endif

#ifndef HOTWIRE_FEEDBACK_UNIT
#define HOTWIRE_FEEDBACK_UNIT					ADC_UNIT_1
#endif

#ifndef HOTWIRE_FEEDBACK_CURRENT_CHANNEL
#define HOTWIRE_FEEDBACK_CURRENT_CHANNEL		ADC1_GPIO35_CHANNEL
#endif

#ifndef HOTWIRE_FEEDBACK_VOLTAGE_CHANNEL
#define HOTWIRE_FEEDBACK_VOLTAGE_CHANNEL		ADC1_GPIO34_CHANNEL
#endif

#ifndef HOTWIRE_PWM_PIN
#define HOTWIRE_PWM_PIN							GPIO_NUM_12
#endif

#ifndef HOTWIRE_PWM_BIT_PRECISION
#define HOTWIRE_PWM_BIT_PRECISION				12
#endif

#ifndef HOTWIRE_PWM_OFF_VALUE
#define HOTWIRE_PWM_OFF_VALUE					10
#endif

#ifndef HOTWIRE_PWM_MAX_VALUE
#define HOTWIRE_PWM_MAX_VALUE					4096
#endif

#ifndef HOTWIRE_PWM_MIN_VALUE
#define HOTWIRE_PWM_MIN_VALUE					30
#endif

#define HOTWIRE_K_INTEGRAL_MS					(HOTWIRE_K_INTEGRAL * HOTWIRE_LOOP_PERIOD_MS / 1000.0)
#define HOTWIRE_ADC_MV_TO_MA					(1.0 / (HOTWIRE_CURRENT_SHUNT_VAL * HOTWIRE_CURRENT_AMP_GAIN))
#define HOTWIRE_ADC_MV_TO_V						((0.001 * (HOTWIRE_VOLTAGE_DIV_R_HI + HOTWIRE_VOLTAGE_DIV_R_LO) / HOTWIRE_VOLTAGE_DIV_R_LO) / HOTWIRE_VOLTAGE_OVERSAMPLE_N)

namespace Spindles {

    class HotWire : public Spindle {
    private:
		static uint32_t							m_rpm;
		static SpindleState						m_state;
        static bool								m_task_running;
        static bool								m_wire_ok;		
		
		static uint32_t							m_supply_voltage;

		static esp_adc_cal_characteristics_t 	m_current_chars;
		static esp_adc_cal_characteristics_t	m_voltage_chars;

		static adc_atten_t						m_current_atten;
		static adc_atten_t						m_voltage_atten;		
		
		static adc_unit_t						m_current_unit;
		static adc_unit_t 						m_voltage_unit;

        static QueueHandle_t 					m_queue;
        static TaskHandle_t  					m_task;
		
		static gpio_num_t						m_output;		
		static adc1_channel_t					m_current;
		static adc1_channel_t					m_voltage;		
		static uint32_t							m_current_output;		
		

	private:
		static bool init_adc();
		static bool init_channel(adc1_channel_t channel, adc_atten_t atten, esp_adc_cal_characteristics_t* chars);

		static uint32_t read_channel(adc1_channel_t channel, esp_adc_cal_characteristics_t* chars);
		static uint32_t read_current();
		static uint32_t read_voltage();
		
		static bool init_feedback();
		static void set_output(uint32_t duty);

		static void send_command();
		static void wire_task(void* pvParameters);		
	public:
 		HotWire();
        HotWire(const HotWire&) = delete;
        HotWire(HotWire&&)      = delete;
		virtual ~HotWire() {}

        HotWire& operator=(const HotWire&) = delete;
        HotWire& operator=(HotWire&&) = delete;

        void         init();
        void         config_message();
        void         set_state(SpindleState state, uint32_t rpm);
        SpindleState get_state();
        uint32_t     set_rpm(uint32_t rpm);
        void         stop();

    };
}