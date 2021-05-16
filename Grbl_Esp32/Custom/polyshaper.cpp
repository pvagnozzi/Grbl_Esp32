/*
	polyshaper.cpp (polyshaper)
	Part of Grbl_ESP32

	copyright (c) 2020 - Bart Dring. This file was intended for use on the ESP32
                2021 - Piergiorgio Vagnozzi

	CPU. Do not use this with Grbl for atMega328P

	Grbl_ESP32 is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Grbl_ESP32 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Grbl.  If not, see <http://www.gnu.org/licenses/>.

	=======================================================================

*/

#include <TMCStepper.h>
#include "../src/Machines/polyshaper.h"
#include "../src/Report.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <driver/adc.h>
#include <esp_adc_cal.h>

#include "Arduino.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

/***************/
/* Custom Code */
/***************/

/* TMC2209 */
typedef struct {
    char axis_name = '0';
    Stream * serial_port = &Serial1;
    uint8_t address = 0;
    float r_sense = 0.1;
    uint16_t rms_current = 1000;
    uint16_t microsteps = 8;
    uint8_t toff = 5;
    bool spreadcycle = false;
    bool pwm_autoscale = true;
    uint8_t blank_time = 24;
    uint32_t tcoolthrs = 0xFFFFF;
    uint8_t semin = 5;
    uint8_t semax = 2;
    uint8_t sedn = 1;
    uint8_t sgthrs = 10;
} tmc2209_conf_t;

bool tmc2209_enabled;
const uint8_t tmc2209_num = 2;
tmc2209_conf_t tmc2209_conf[tmc2209_num];
TMC2209Stepper * tmc2209[tmc2209_num];

void TMC2209_Init()
{
  // reset
  pinMode(TMC2209_STDBY_PIN, OUTPUT);
  digitalWrite(TMC2209_STDBY_PIN, HIGH);
  delay(1);
  digitalWrite(TMC2209_STDBY_PIN, LOW);

  Serial1.begin(115200, SERIAL_8N1, TMC2209_SERIAL1_PIN, TMC2209_SERIAL1_PIN);
  pinMode(TMC2209_SERIAL1_PIN, OUTPUT_OPEN_DRAIN);

  tmc2209_conf[0].axis_name = 'X';
  tmc2209_conf[0].address = 0;
  tmc2209_conf[0].sgthrs = 25;
  tmc2209_conf[0].rms_current = 200;

  tmc2209_conf[1].axis_name = 'Y';
  tmc2209_conf[1].address = 1;
  tmc2209_conf[1].sgthrs = 25;
  tmc2209_conf[1].rms_current = 800;

  for (uint8_t i = 0; i < tmc2209_num; ++i) {
    tmc2209[i] = new TMC2209Stepper(tmc2209_conf[i].serial_port, tmc2209_conf[i].r_sense, tmc2209_conf[i].address);
    tmc2209[i]->begin();
    char msg[] = "? driver not connected";
    msg[0] = tmc2209_conf[i].axis_name;
    if (tmc2209[i]->test_connection()) grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, msg);
    tmc2209[i]->toff(5);
    tmc2209[i]->rms_current(tmc2209_conf[i].rms_current);
    tmc2209[i]->microsteps(tmc2209_conf[i].microsteps);
    tmc2209[i]->en_spreadCycle(tmc2209_conf[i].spreadcycle);
    tmc2209[i]->pwm_autoscale(tmc2209_conf[i].pwm_autoscale);
    tmc2209[i]->blank_time(tmc2209_conf[i].blank_time);
    //tmc2209[i]->sfilt(true);
    tmc2209[i]->TCOOLTHRS(tmc2209_conf[i].tcoolthrs);
    //tmc2209[i]->THIGH(0);
    tmc2209[i]->semin(tmc2209_conf[i].semin);
    tmc2209[i]->semax(tmc2209_conf[i].semax);
    tmc2209[i]->sedn(tmc2209_conf[i].sedn);
    tmc2209[i]->SGTHRS(tmc2209_conf[i].sgthrs);
  }

  //TMC2209_enable(false);
}

void TMC2209_enable(bool en) {
  for (uint8_t i = 0; i < tmc2209_num; ++i) {
    if(en) tmc2209[i]->toff(tmc2209_conf[i].toff);
    else tmc2209[i]->toff(0);
  }
  tmc2209_enabled = en;
}

bool TMC2209_enabled() {
  return tmc2209_enabled;
}

void led_on() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
}

void led_off() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  
}

void led_blink(bool fast) {
  auto tm = fast ? LED_BLINK_DELAY_FAST : LED_BLINK_DELAY_NORMAL;
  led_off();
  led_on();
  delay(tm);
  led_off();
  delay(tm);
}

void led_flash(int count) {
  for(;count;count--) {
    led_blink(false);
  }
}

void boot_reason_led()
{
    esp_reset_reason_t reset_reason = esp_reset_reason();
    led_flash(reset_reason);

    delay(2000);

    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    led_flash(wakeup_reason);
}

void boot_reason_print() {
    
    esp_reset_reason_t reset_reason = esp_reset_reason();

    switch (reset_reason)  {
      case ESP_RST_UNKNOWN:    
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Reset reason can not be determined"); 
        break;
      
      case ESP_RST_POWERON:    
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Reset due to power-on event"); 
        break;
      
      case ESP_RST_EXT:        
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Reset by external pin"); 
        break;
      
      case ESP_RST_SW:         
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Software reset via esp_restart"); 
        break;
      
      case ESP_RST_PANIC:      
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Software reset due to exception/panic"); 
        break;
      
      case ESP_RST_INT_WDT:    
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Reset (software or hardware) due to interrupt watchdog"); 
        break;
      
      case ESP_RST_TASK_WDT:   
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Reset due to task watchdog"); 
        break;
      
      case ESP_RST_WDT:        
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Reset due to other watchdogs"); 
        break;
      
      case ESP_RST_DEEPSLEEP:  
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Reset after exiting deep sleep mode"); 
        break;
      
      case ESP_RST_BROWNOUT:   
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Brownout reset (software or hardware)"); 
        break;
      
      case ESP_RST_SDIO:       
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Reset over SDIO"); 
        break;
    }

    if (reset_reason != ESP_RST_DEEPSLEEP)
    {
      return;
    }

    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

    switch(wakeup_reason) { 
      case ESP_SLEEP_WAKEUP_UNDEFINED:    
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "In case of deep sleep: reset was not caused by exit from deep sleep"); 
        break;
            
      case ESP_SLEEP_WAKEUP_ALL:          
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Not a wakeup cause: used to disable all wakeup sources with esp_sleep_disable_wakeup_source"); 
        break;
            
      case ESP_SLEEP_WAKEUP_EXT0:         
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Wakeup caused by external signal using RTC_IO"); 
        break;
            
      case ESP_SLEEP_WAKEUP_EXT1:         
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Wakeup caused by external signal using RTC_CNTL"); 
        break;
            
      case ESP_SLEEP_WAKEUP_TIMER:        
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Wakeup caused by timer"); 
        break;
            
      case ESP_SLEEP_WAKEUP_TOUCHPAD:     
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Wakeup caused by touchpad"); 
        break;

      case ESP_SLEEP_WAKEUP_ULP:          
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Wakeup caused by ULP program"); 
        break;
            
      case ESP_SLEEP_WAKEUP_GPIO:         
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Wakeup caused by GPIO (light sleep only)"); 
        break;
            
      case ESP_SLEEP_WAKEUP_UART: 
        grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Wakeup caused by UART (light sleep only)"); 
        break;
    }
}

void boot_reason() {
  boot_reason_print();
  boot_reason_led();
}

static void IRAM_ATTR power_isr_handler(void *) {
  grbl_msg_sendf(CLIENT_ALL, MsgLevel::Error, "Power panic triggered");
}

#ifdef USE_MACHINE_INIT
void machine_init() {
    
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Brownout reset disabled");

    pinMode(X_STEP_PIN, OUTPUT);
    pinMode(X_DIRECTION_PIN, OUTPUT);
    pinMode(X_LIMIT_PIN, INPUT);

    pinMode(Y_STEP_PIN, OUTPUT);
    pinMode(Y_DIRECTION_PIN, OUTPUT);    
    pinMode(Y_LIMIT_PIN, INPUT);

    pinMode(TMC2209_STDBY_PIN, OUTPUT);
    pinMode(TMC2209_EN_N_PIN, OUTPUT);
    digitalWrite(TMC2209_EN_N_PIN, LOW);
    pinMode(LED_PIN, OUTPUT);
    pinMode(SWITCH_N_PIN, INPUT_PULLUP);
    pinMode(POWER_INTERRUPT_PIN, INPUT);
    grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Pins configured");    
    led_off();

    gpio_config_t io_conf;
    io_conf.pin_bit_mask = (1ULL<<4);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.intr_type = GPIO_INTR_POSEDGE;
  
    gpio_config(&io_conf);
    gpio_isr_register(power_isr_handler, NULL, (ESP_INTR_FLAG_IRAM | ESP_INTR_FLAG_EDGE | ESP_INTR_FLAG_LEVEL3), NULL);
    grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Power off handler configured");    

    pinMode(HOTWIRE_PWM_PIN, OUTPUT);
    pinMode(HOTWIRE_CURRENT_ADC_CHANNEL, INPUT);
    pinMode(HOTWIRE_VOLTAGE_ADC_CHANNEL, INPUT);
    grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Hotwire configured");

    boot_reason();
}

#endif

#ifdef USE_CUSTOM_HOMING

bool user_defined_homing(uint8_t cycle_mask) {
    return true;
}
#endif

#ifdef USE_M30
/*
  user_m30() is called when an M30 gcode signals the end of a gcode file.
*/
void user_m30() {

}
#endif

#ifdef USE_MACHINE_TRINAMIC_INIT

void machine_trinamic_setup() {
    TMC2209_Init();
    TMC2209_enable(true);
    grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Motors configured");
}

#endif

