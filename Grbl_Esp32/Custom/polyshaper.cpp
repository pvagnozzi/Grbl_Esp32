
#include "../src/Machines/polyshaper.h"
#include "../src/Report.h"
#include "../src/Motors/Motors.h"

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <driver/adc.h>
#include <esp_adc_cal.h>

#include "Arduino.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

/********************/
/* Custom Functions */
/********************/

void led(bool on) {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, on ? HIGH : LOW);
}

void led_blink(bool fast, int count) {
  auto tm = fast ? LED_BLINK_DELAY_FAST : LED_BLINK_DELAY_NORMAL;
  led(false);
  for(;count; count--) {
    led(true);
    delay(tm);
    led(false);
    delay(tm);
  }
}


void boot_reason_led()
{
    esp_reset_reason_t reset_reason = esp_reset_reason();
    led_blink(false, reset_reason);

    delay(2000);

    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    led_blink(false, wakeup_reason);
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
    boot_reason();

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
    led(false);

    gpio_config_t io_conf;
    io_conf.pin_bit_mask = (1ULL<<4);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.intr_type = GPIO_INTR_POSEDGE;

    gpio_config(&io_conf);
    gpio_isr_register(power_isr_handler, NULL, (ESP_INTR_FLAG_IRAM | ESP_INTR_FLAG_EDGE | ESP_INTR_FLAG_LEVEL3), NULL);
    grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Power off handler configured");

    Serial1.begin(115200, SERIAL_8N1, TMC2209_SERIAL1_PIN, TMC2209_SERIAL1_PIN);
    pinMode(TMC2209_SERIAL1_PIN, OUTPUT_OPEN_DRAIN);
    grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Stepper UART configured");

    pinMode(TMC2209_STDBY_PIN, OUTPUT);
    digitalWrite(TMC2209_STDBY_PIN, HIGH);
    delay(1);
    digitalWrite(TMC2209_STDBY_PIN, LOW);
    grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Stepper reset");

    init_motors();
    grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Stepper configured");
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

