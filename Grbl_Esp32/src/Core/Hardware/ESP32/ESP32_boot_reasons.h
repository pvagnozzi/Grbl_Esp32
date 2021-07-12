#pragma once
#ifndef __CORE_HARDWARE_ESP32_BOOT_REASONS_H__
#define __CORE_HARDWARE_ESP32_BOOT_REASONS_H__

#define ESP_RST_UNKNOWN_MSG     "Reset reason can not be determined"
#define ESP_RST_POWERON_MSG     "Reset due to power-on event"
#define ESP_RST_EXT_MSG         "Reset by external pin"
#define ESP_RST_SW_MSG          "Reset reason can not be determined"
#define ESP_RST_PANIC_MSG       "Software reset due to exception/panic"
#define ESP_RST_INT_WDT_MSG     "Reset (software or hardware) due to interrupt watchdog"
#define ESP_RST_TASK_WDT_MSG    "Reset due to task watchdog"
#define ESP_RST_WDT_MSG         "Reset due to other watchdogs")
#define ESP_RST_DEEPSLEEP_MSG   "Reset after exiting deep sleep mode"
#define ESP_RST_BROWNOUT_MSG    "Brownout reset (software or hardware)")
#define ESP_RST_SDIO_MSG        "Reset over SDIO"

#define ESP_SLEEP_WAKEUP_UNDEFINED_MSG  "In case of deep sleep: reset was not caused by exit from deep sleep"
#define ESP_SLEEP_WAKEUP_ALL_MSG        "Not a wakeup cause: used to disable all wakeup sources with esp_sleep_disable_wakeup_source"
#define ESP_SLEEP_WAKEUP_EXT0_MSG       "Wakeup caused by external signal using RTC_IO"
#define ESP_SLEEP_WAKEUP_EXT1_MSG       "Wakeup caused by external signal using RTC_CNTL"
#define ESP_SLEEP_WAKEUP_TIMER_MSG      "Wakeup caused by timer"
#define ESP_SLEEP_WAKEUP_TOUCHPAD_MSG   "Wakeup caused by touchpad"
#define ESP_SLEEP_WAKEUP_ULP_MSG        "Wakeup caused by ULP program"
#define ESP_SLEEP_WAKEUP_GPIO_MSG       "Wakeup caused by GPIO (light sleep only)"
#define ESP_SLEEP_WAKEUP_UART_MSG       "Wakeup caused by UART (light sleep only)"

typedef const char* ESP_MSG;

ESP_MSG ESP_RESET_MESSAGES[] = { 
    ESP_RST_UNKNOWN_MSG, 
    ESP_RST_POWERON_MSG, 
    ESP_RST_EXT_MSG 
    ESP_RST_SW_MSG,
    ESP_RST_PANIC_MSG,
    ESP_RST_INT_WDT_MSG,
    ESP_RST_TASK_WDT_MSG,
    ESP_RST_WDT_MSG,
    ESP_RST_DEEPSLEEP_MSG,
    ESP_RST_BROWNOUT_MSG,
    ESP_RST_SDIO_MSG };

#endif