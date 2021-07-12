#include "AnalogChannel.h"

namespace Core {

    adc_bits_width_t AnalogChannel::m_width = ADC_WIDTH_BIT_12;    
    bool AnalogChannel::m_global_init = false;

    AnalogChannel::AnalogChannel(
        adc1_channel_t channel, 
        adc_atten_t atten, 
        adc_unit_t unit,
        uint32_t vref) :
        m_channel(channel),
        m_atten(atten),
        m_unit(unit),
        m_vref(vref),
        m_chars() {           

        initialize(); 
        if (m_global_init) {
            return;
        }

        m_global_init = true;
        set_width(m_width);
    }

    void AnalogChannel::set_width(adc_bits_width_t width) {
        m_width = width;
        global_init();
    }

    void AnalogChannel::global_init() {
        adc1_config_width(m_width);
    }

    void AnalogChannel::initialize() {
        adc1_config_channel_atten(m_channel, m_atten);
        esp_adc_cal_value_t val_type = esp_adc_cal_characterize(
            m_unit, 
            m_atten, m_width, 
            m_vref, 
            &m_chars);
        
        /*
        if (val_type == ESP_ADC_CAL_VAL_DEFAULT_VREF) {
            LOG_ERROR("Current ADC not configured: %s", pinName(m_channel));
        }
        else {
            LOG_INFO("Current ADC not configured: %s", pinName(m_channel));
        }
        */
    }
}