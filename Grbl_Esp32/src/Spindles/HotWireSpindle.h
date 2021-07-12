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
#include "PWMSpindle.h"
#include "../Core/Core.h"

#ifndef SPINDLE_OUTPUT_PIN
#   define SPINDLE_OUTPUT_PIN               GPIO_NUM_12
#endif

#ifndef HOTWIRE_CURRENT_SHUNT_VAL
#   define HOTWIRE_CURRENT_SHUNT_VAL        0.1
#endif

#ifndef HOTWIRE_CURRENT_AMP_GAIN
#   define HOTWIRE_CURRENT_AMP_GAIN	        50.0
#endif

#ifndef HOTWIRE_ADC_VREF_MILLIVOLT
#   define HOTWIRE_ADC_VREF_MILLIVOLT	    1114
#endif

#ifndef HOTWIRE_VOLTAGE_OVERSAMPLE_N
#   define HOTWIRE_VOLTAGE_OVERSAMPLE_N	    256
#endif

#ifndef HOTWIRE_VOLTAGE_DIV_R_HI
#   define HOTWIRE_VOLTAGE_DIV_R_HI		    10.0
#endif

#ifndef HOTWIRE_VOLTAGE_DIV_R_LO
#   define HOTWIRE_VOLTAGE_DIV_R_LO         3.3
#endif

#ifndef HOTWIRE_ERROR_THRESHOLD
#   define HOTWIRE_ERROR_THRESHOLD          40
#endif

#ifndef HOTWIRE_ERROR_MAX_COUNT
#   define HOTWIRE_ERROR_MAX_COUNT          5
#endif

#ifndef HOTWIRE_LOOP_PERIOD_MS
#   define HOTWIRE_LOOP_PERIOD_MS           5
#endif

#ifndef HOTWIRE_K_INTEGRAL
#   define HOTWIRE_K_INTEGRAL               25.0
#endif

#ifndef HOTWIRE_K_PROPORTIONAL
#   define HOTWIRE_K_PROPORTIONAL           2.5
#endif

#ifndef SPINDLE_PWM_RANGE
#   define SPINDLE_PWM_RANGE                (SPINDLE_PWM_MAX_VALUE - SPINDLE_PWM_MIN_VALUE)
#endif

#define HOTWIRE_K_INTEGRAL_MS		        (HOTWIRE_K_INTEGRAL * HOTWIRE_LOOP_PERIOD_MS /1000.0)
#define HOTWIRE_ADC_MV_TO_MA		        (1.0 / (HOTWIRE_CURRENT_SHUNT_VAL * HOTWIRE_CURRENT_AMP_GAIN))
#define HOTWIRE_ADC_MV_TO_V                 ((0.001 * (HOTWIRE_VOLTAGE_DIV_R_HI + HOTWIRE_VOLTAGE_DIV_R_LO) / HOTWIRE_VOLTAGE_DIV_R_LO) / HOTWIRE_VOLTAGE_OVERSAMPLE_N)

namespace Spindles {

    class HotWire : public Spindle {
    private:
        PWM                 m_output;
        Core::AnalogChannel m_current;  
        Core::AnalogChannel m_voltage;  
        double              m_supply_voltage;
        uint32_t            m_rpm;      

	    uint8_t             m_error_count;
	    double              m_integral;
	    double              m_prev_error;
        bool                m_wire_good;
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

        virtual ~HotWire();

        inline double voltage() const noexcept {
            return m_voltage.value();
        }

        inline double current() const noexcept {
            return voltage() * HOTWIRE_ADC_MV_TO_MA;
        }

        inline bool is_active() noexcept {
            return 
                (get_state() != SpindleState::Disable) && 
                (m_rpm > SPINDLE_PWM_MIN_VALUE);
        }

        inline bool wire_good() const noexcept {
            return m_wire_good;
        }

        bool        check();
        void        wire_off();
        void        wire_on();

    protected:
        virtual void wire_broken();
    };
}
