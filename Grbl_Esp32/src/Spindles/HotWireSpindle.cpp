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
        m_current_adc_channel(SPINDLE_CURRENT_ADC_CHANNEL),
        m_voltage_adc_channel(SPINDLE_VOLTAGE_ADC_CHANNEL),
        m_current_adc_atten(ADC_ATTEN_DB_11),
        m_voltage_adc_atten(ADC_ATTEN_DB_11),
        m_adc_unit(ADC_UNIT_1),
        m_adc_bits(ADC_WIDTH_BIT_12),
        m_pwm_off_value(SPINDLE_PWM_OFF_VALUE),
        m_pwm_min_value(SPINDLE_PWM_MIN_VALUE),
        m_pwm_max_value(SPINDLE_PWM_MAX_VALUE)  {

    }

    void HotWire::init() {
        is_reversable = false;
        use_delays    = false;

        adc1_config_width(m_adc_bits);
        adc1_config_channel_atten(m_current_adc_channel, m_current_adc_atten);
        esp_adc_cal_value_t val_type = esp_adc_cal_characterize(m_adc_unit, m_current_adc_atten, m_adc_bits, HOTWIRE_ADC_VREF_MILLIVOLT, &m_current_adc_chars);
        if (val_type == ESP_ADC_CAL_VAL_DEFAULT_VREF) {
            grbl_msg_sendf(CLIENT_ALL, MsgLevel::Error, "Current ADC not configured");
        }
        else {
            grbl_msg_sendf(CLIENT_ALL, MsgLevel::Debug, "Current ADC configured");
        }

        adc1_config_channel_atten(m_voltage_adc_channel, m_voltage_adc_atten);
        val_type = esp_adc_cal_characterize(m_adc_unit, m_voltage_adc_atten, m_adc_bits, HOTWIRE_ADC_VREF_MILLIVOLT, &m_voltage_adc_chars);
        if (val_type == ESP_ADC_CAL_VAL_DEFAULT_VREF) {
            grbl_msg_sendf(CLIENT_ALL, MsgLevel::Error, "Voltage ADC not configured");
        }
        else {
            grbl_msg_sendf(CLIENT_ALL, MsgLevel::Debug, "Voltage ADC configured");
        }

        uint32_t adc_reading = 0;
        for (int i = 0; i < HOTWIRE_VOLTAGE_OVERSAMPLE_N; ++i) {
            adc_reading += esp_adc_cal_raw_to_voltage(adc1_get_raw(m_voltage_adc_channel), &m_voltage_adc_chars);
        }
        m_supply_voltage = adc_reading * HOTWIRE_ADC_MV_TO_V;
        //grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Supply voltage: %4.0f mV", m_supply_voltage);

        ledcSetup(SPINDLE_PWM_CHANNEL, SPINDLE_PWM_BASE_FREQ, SPINDLE_PWM_BIT_PRECISION);
        ledcAttachPin(SPINDLE_PWM_PIN, SPINDLE_PWM_CHANNEL);
        ledcWrite(SPINDLE_PWM_CHANNEL, 0);
        //grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Spindle PWM configured");

        //xTaskCreate(wire_task, "wire_task", 2000, NULL, 1, NULL);

        config_message();
    }

    void HotWire::set_output(uint32_t duty) {
        if (m_output_pin == UNDEFINED_PIN) {
            return;
        }

        // to prevent excessive calls to ledcWrite, make sure duty hass changed //
        if (duty == m_current_pwm_duty) {
            return;
        }

        m_current_pwm_duty = duty;

        LEDC.channel_group[0].channel[0].duty.duty        = duty << 4;
        bool on                                           = !!duty;
        LEDC.channel_group[0].channel[0].conf0.sig_out_en = on;
        LEDC.channel_group[0].channel[0].conf1.duty_start = on;
        LEDC.channel_group[0].channel[0].conf0.clk_en     = on;
    }

    uint32_t HotWire::set_rpm(uint32_t rpm) {
        sys.spindle_speed = rpm;
        return rpm;
    }

    void HotWire::set_state(SpindleState state, uint32_t rpm) {
        _current_state    = state;
        sys.spindle_speed = rpm;
    }

    SpindleState HotWire::get_state() { return _current_state; }

    void HotWire::stop() {

    }

    void HotWire::config_message() {
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "HotWire spindle: supply voltage %4.0f mV", m_supply_voltage);
    }
}
