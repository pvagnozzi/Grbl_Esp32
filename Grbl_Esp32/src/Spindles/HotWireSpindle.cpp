/*
	HotWireSpindle.cpp

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

#include "HotWireSpindle.h"
#include "soc/ledc_struct.h"

#define PIN_NAME(x) (pinName(x).c_str())

namespace Spindles {
    

	uint32_t		                HotWire::m_rpm              = 0;
    bool			                HotWire::m_task_running     = false;
    bool							HotWire::m_wire_ok          = true;		
		
	uint32_t						HotWire::m_supply_voltage   = 0;

	esp_adc_cal_characteristics_t 	HotWire::m_current_chars    = esp_adc_cal_characteristics_t();
	esp_adc_cal_characteristics_t	HotWire::m_voltage_chars    = esp_adc_cal_characteristics_t();

	adc_atten_t						HotWire::m_current_atten    = ADC_ATTEN_MAX; 
	adc_atten_t						HotWire::m_voltage_atten    = ADC_ATTEN_MAX;		
		
	adc_unit_t						HotWire::m_current_unit     = ADC_UNIT_1;
	adc_unit_t 						HotWire::m_voltage_unit     = ADC_UNIT_1;

    QueueHandle_t 					HotWire::m_queue            = NULL;
    TaskHandle_t  					HotWire::m_task             = NULL;
		
	gpio_num_t						HotWire::m_output           = HOTWIRE_PWM_PIN;		
	adc1_channel_t					HotWire::m_current          = HOTWIRE_FEEDBACK_CURRENT_CHANNEL;
	adc1_channel_t					HotWire::m_voltage          = HOTWIRE_FEEDBACK_VOLTAGE_CHANNEL;		
	uint32_t						HotWire::m_current_output   = 0;


    HotWire::HotWire() {

    }

    void HotWire::init() {
        init_feedback();
        set_output(HOTWIRE_PWM_OFF_VALUE);


    }

    void HotWire::config_message() {

        grbl_msg_sendf(CLIENT_SERIAL,
                       MsgLevel::Info,
                       "VFD RS485  Tx:%s Rx:%s RTS:%s"
        );    
    }
        
    void HotWire::set_state(SpindleState state, uint32_t rpm) {

    }
        
    SpindleState HotWire::get_state() {
        return SpindleState::Disable;
    }
        
    uint32_t HotWire::set_rpm(uint32_t rpm) {
        return rpm;
    }
        
    void HotWire::stop() {
        
    }

    void HotWire::wire_task(void* pvParameters) {

    }

    void HotWire::set_output(uint32_t duty) {
        if (m_output == UNDEFINED_PIN) {
            return;
        }

        if (duty == m_current_output) {
            return;
        }

        m_current_output = duty;

        LEDC.channel_group[0].channel[0].duty.duty        = duty << 4;
        bool on                                           = !!duty;
        LEDC.channel_group[0].channel[0].conf0.sig_out_en = on;
        LEDC.channel_group[0].channel[0].conf1.duty_start = on;
        LEDC.channel_group[0].channel[0].conf0.clk_en     = on;
    }

    bool HotWire::init_feedback() {
        return init_adc() 
            && init_channel(HOTWIRE_FEEDBACK_CURRENT_CHANNEL, m_current_atten, &m_current_chars)
            && init_channel(HOTWIRE_FEEDBACK_VOLTAGE_CHANNEL, m_voltage_atten, &m_voltage_chars);
    }

    bool HotWire::init_adc() {
        if (adc1_config_width(HOTWIRE_FEEDBACK_RES)) {
            grbl_msg_sendf(
                CLIENT_SERIAL,
                MsgLevel::Error,
                "ADC Resolution configuration failed: %d",                
                HOTWIRE_FEEDBACK_RES
            );  
            return false;
        }

        return true;
    }
	
    bool HotWire::init_channel(adc1_channel_t channel, adc_atten_t atten, esp_adc_cal_characteristics_t* chars) {
        if (adc1_config_channel_atten(channel, atten) == ESP_ERR_INVALID_ARG) {
            grbl_msg_sendf(
                CLIENT_SERIAL,
                MsgLevel::Error,
                "Sensor attenuation configuration failed Channel: %s Attenuation: %d",
                PIN_NAME(channel),
                atten
            );    

            return false;
        }
	    
	    if (esp_adc_cal_characterize(
            HOTWIRE_FEEDBACK_UNIT, 
            atten, 
            HOTWIRE_FEEDBACK_RES, 
            HOTWIRE_FEEDBACK_VREF, 
            chars) == ESP_ADC_CAL_VAL_DEFAULT_VREF) {
            grbl_msg_sendf(
                CLIENT_SERIAL,
                MsgLevel::Error,
                "Sensor calibration failed Channel: %s Attenuation: %d Resolution: %d VRef: %dmv",
                PIN_NAME(channel),
                atten,
                HOTWIRE_FEEDBACK_RES,
                HOTWIRE_FEEDBACK_VREF
            );    
            return false;
        }

        grbl_msg_sendf(
            CLIENT_SERIAL,
            MsgLevel::Info,
            "Sensor calibrated failed Channel: %s Attenuation: %d Resolution: %d VRef: %dmv",
            PIN_NAME(channel),
            atten,
            HOTWIRE_FEEDBACK_RES,
            HOTWIRE_FEEDBACK_VREF
        ); 

        return true;
    }

	uint32_t HotWire::read_channel(adc1_channel_t channel, esp_adc_cal_characteristics_t* chars) {
        return esp_adc_cal_raw_to_voltage(adc1_get_raw(channel), chars);
    }
	
    uint32_t HotWire::read_current() {
        return read_channel(HOTWIRE_FEEDBACK_CURRENT_CHANNEL, &m_current_chars);
    }

	uint32_t HotWire::read_voltage() {
        return read_channel(HOTWIRE_FEEDBACK_VOLTAGE_CHANNEL, &m_voltage_chars);
    }
}