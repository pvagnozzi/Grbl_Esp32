/*
    HotWireSpindle.cpp

    HotWire spindle

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
#include "HotWireSpindle.h"
#include "soc/ledc_struct.h"

namespace Spindles {

    HotWire::HotWire() :
        m_output(),
        m_current(SPINDLE_CURRENT_ADC_CHANNEL),
        m_voltage(SPINDLE_VOLTAGE_ADC_CHANNEL),
        m_supply_voltage(0),
	    m_error_count(0),
	    m_integral(0),
	    m_prev_error(0),
        m_wire_good(true) {
    }

    HotWire::~HotWire() {        
    }
    
    void HotWire::init() {
        
        m_output.init();

        m_current.initialize();
        m_voltage.initialize();

        uint32_t adc_reading = 0;
	    for (uint16_t i = HOTWIRE_VOLTAGE_OVERSAMPLE_N; i; --i) {
		    adc_reading += m_voltage.value();
	    }
	    m_supply_voltage = adc_reading * HOTWIRE_ADC_MV_TO_V;

        config_message();
    }

    void HotWire::stop() {
        m_output.stop();
    }

    void HotWire::set_state(SpindleState state, uint32_t rpm) {
        m_output.set_state(state, rpm);
    }
    
    SpindleState HotWire::get_state() {
        return m_output.get_state();
    }

    uint32_t HotWire::set_rpm(uint32_t rpm) {
        return m_output.set_rpm(rpm);
    }

    void HotWire::config_message() {
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "HotWire spindle: supply voltage %4.0f mV", m_supply_voltage);
    }

    bool HotWire::check() {	        
	    double currentValue = current();	    
        grbl_msg_sendf(CLIENT_SERIAL, MsgLevel::Debug, "Hotwire current value: %d", currentValue);

        if (!is_active()) {
            return false;
        }

		double err = m_rpm - currentValue;
        double absErr = abs(err);

		if((absErr >= abs(m_prev_error)) && (abs(err) > HOTWIRE_ERROR_THRESHOLD) ) {
            grbl_msg_sendf(
                CLIENT_SERIAL, 
                MsgLevel::Debug, 
                "Hotwire error: %d; previous: %d; tries: %d", 
                err, 
                m_prev_error, 
                m_prev_error);
			m_error_count ++;
            m_prev_error = err;
			//grbl_sendf(CLIENT_SERIAL, "[DBG: error_count: %2d]\r\n", error_count);
			if(m_error_count > HOTWIRE_ERROR_MAX_COUNT) {
				m_error_count = 0;				
				m_wire_good = false;
                wire_broken();
                return false;
			}
		}
		else {
            m_error_count = 0;
        }
        	
        grbl_msg_sendf(
            CLIENT_SERIAL, 
            MsgLevel::Debug, 
            "Hotwire current value: %d mV; requested: %d mV (%d mV)", 
            currentValue, m_rpm, err);

        double sum = err * HOTWIRE_K_PROPORTIONAL + m_integral;
        double out = constrain(sum, 0, SPINDLE_PWM_RANGE);
        if(!((err * m_integral > 0) && (out != sum))) {
            m_integral += HOTWIRE_K_INTEGRAL_MS * err;
        }

        uint16_t pwm_val = out;
        grbl_msg_sendf(CLIENT_SERIAL, MsgLevel::Debug, "Hotwire spindle value changed: %d", pwm_val);
        m_output.set_rpm(pwm_val);      
        return true;  
	}	

    void HotWire::wire_broken() {
        grbl_msg_sendf(CLIENT_SERIAL, MsgLevel::Error, "Hotwire broken");        
    }

    void HotWire::wire_off() {
        m_output.set_state(SpindleState::Disable, SPINDLE_PWM_OFF_VALUE);
    }

    void HotWire::wire_on() {
        m_output.set_state(SpindleState::Cw, SPINDLE_PWM_MIN_VALUE);
    }
}
