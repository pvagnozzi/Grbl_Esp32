#pragma once

/*
	HotWireSpindle.h

	Hotwire Spindle

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
#include <driver/adc.h>
#include <esp_adc_cal.h>

#ifndef HOTWIRE_CURRENT_SHUNT_VAL
#define HOTWIRE_CURRENT_SHUNT_VAL           0.1
#endif

#define HOTWIRE_CURRENT_AMP_GAIN	        50.0
#define HOTWIRE_ADC_VREF_MILLIVOLT	        1114


#define HOTWIRE_VOLTAGE_OVERSAMPLE_N	    256
#define HOTWIRE_VOLTAGE_DIV_R_HI		    10.0
#define HOTWIRE_VOLTAGE_DIV_R_LO            3.3


#define HOTWIRE_ERROR_THRESHOLD		        40
#define HOTWIRE_ERROR_MAX_COUNT		        5
#define HOTWIRE_LOOP_PERIOD_MS		        5
#define HOTWIRE_K_INTEGRAL			        25.0
#define HOTWIRE_K_PROPORTIONAL		        2.5


#define HOTWIRE_K_INTEGRAL_MS		        (HOTWIRE_K_INTEGRAL * HOTWIRE_LOOP_PERIOD_MS /1000.0)
#define HOTWIRE_ADC_MV_TO_MA		        (1.0 / (HOTWIRE_CURRENT_SHUNT_VAL * HOTWIRE_CURRENT_AMP_GAIN))
#define HOTWIRE_ADC_MV_TO_V                 ((0.001 * (HOTWIRE_VOLTAGE_DIV_R_HI + HOTWIRE_VOLTAGE_DIV_R_LO) / HOTWIRE_VOLTAGE_DIV_R_LO) / HOTWIRE_VOLTAGE_OVERSAMPLE_N)


namespace Spindles {
    class HotWire : public Spindle {
    private:
        esp_adc_cal_characteristics_t   m_current_adc_chars;
        esp_adc_cal_characteristics_t   m_voltage_adc_chars;

        adc1_channel_t                  m_current_adc_channel;
        adc1_channel_t                  m_voltage_adc_channel;

        adc_atten_t                     m_current_adc_atten;
        adc_atten_t                     m_voltage_adc_atten;

        adc_unit_t                      m_adc_unit;
        adc_bits_width_t                m_adc_bits;
        double                          m_supply_voltage;

        uint32_t                        m_current_pwm_duty;
        uint8_t                         m_output_pin;
        uint32_t                        m_min_rpm;
        uint32_t                        m_max_rpm;
        uint32_t                        m_pwm_off_value;
        uint32_t                        m_pwm_min_value;
        uint32_t                        m_pwm_max_value;

    private:
        void set_output(uint32_t duty);

    public:
        HotWire();

        HotWire(const HotWire&) = delete;
        HotWire(HotWire&&)      = delete;
        HotWire& operator=(const HotWire&) = delete;
        HotWire& operator=(HotWire&&) = delete;

        void         init() override;
        uint32_t     set_rpm(uint32_t rpm) override;
        void         set_state(SpindleState state, uint32_t rpm) override;
        SpindleState get_state() override;
        void         stop() override;
        void         config_message() override;

        virtual ~HotWire() {}
    };
}
