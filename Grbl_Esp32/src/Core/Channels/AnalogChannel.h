#pragma once
#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <driver/adc.h>
#include <esp_adc_cal.h>

#ifndef SENSOR_ADC_VREF_MILLIVOLT
#define SENSOR_ADC_VREF_MILLIVOLT       1114
#endif

#ifndef SENSOR_ADC_WIDTH_DEFAULT
#define SENSOR_ADC_WIDTH_DEFAULT        ADC_WIDTH_BIT_12
#endif

#include "../Object.h"

namespace Core {

    class AnalogChannel : public Object {
    private:
        static adc_bits_width_t         m_width;
        static bool                     m_global_init;
        
        adc1_channel_t                  m_channel;
        adc_atten_t                     m_atten;
        adc_unit_t                      m_unit;
        uint32_t                        m_vref;
        esp_adc_cal_characteristics_t   m_chars;   

    private:
        static void global_init();

    public:
        AnalogChannel(
            adc1_channel_t channel, 
            adc_atten_t atten = ADC_ATTEN_DB_11, 
            adc_unit_t unit = ADC_UNIT_1,
            uint32_t vref = SENSOR_ADC_VREF_MILLIVOLT);
        AnalogChannel(const AnalogChannel&) = delete;
        AnalogChannel(AnalogChannel&&) = delete;
        ~AnalogChannel() = default;

        inline static adc_bits_width_t width() {
            return m_width;
        }
        
        static void set_width(adc_bits_width_t width);
        void initialize();

        inline adc1_channel_t channel() const noexcept {
            return m_channel;
        }

        inline adc_atten_t atten() const noexcept {
            return m_atten;
        }

        inline adc_unit_t unit() const noexcept {
            return m_unit;
        }

        inline int rawValue() const noexcept {
            return adc1_get_raw(m_channel);
        }

        inline uint32_t value() const noexcept {
            return esp_adc_cal_raw_to_voltage(rawValue(), &m_chars);
        }
    };
}

#endif