/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/gpio_types.h"
#include "hal/ledc_types.h"
#include "soc/clk_tree_defs.h"
#include "driver/ledc.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define SOUND_HZ 1000
#define GPIO_SOUND_PIN 16
#define GPIO_LED_PIN 15
#define SENSOR_PIN 23

#define STEP 10

void do_pwm_stuff() {
  // gpio_config_t gpio = {
  //   .intr_type = GPIO_INTR_DISABLE,
  //   .mode = G,
  //   .pull_up_en = GPIO_PULLUP_DISABLE,
  //   .pull_down_en = GPIO_PULLDOWN_DISABLE,
  //   .pin_bit_mask = 1ULL << GPIO_LED_PIN
  // };
  // ESP_ERROR_CHECK(gpio_config(&gpio));

  ledc_timer_config_t timer_cfg = {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .timer_num = LEDC_TIMER_0,
    .freq_hz = SOUND_HZ,
    .clk_cfg = LEDC_AUTO_CLK,
    .duty_resolution = 10,
  };
  ESP_ERROR_CHECK(ledc_timer_config(&timer_cfg));

  ledc_channel_config_t chanl_cfg = {
    .gpio_num = GPIO_SOUND_PIN,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .intr_type = LEDC_INTR_DISABLE,
    .timer_sel = LEDC_TIMER_0,
    .duty = 512,
    .sleep_mode = LEDC_SLEEP_MODE_KEEP_ALIVE,
  };
  ESP_ERROR_CHECK(ledc_channel_config(&chanl_cfg));
  
  ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 512));
  ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
}

void app_main(void) {
  printf("Hello Raj! pwm2!\n");
  do_pwm_stuff();

  for (int j = 0; j < 5; j++) {
    for (int i = STEP; i >= 0; i--) {
      ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, (1024 / STEP) * i));
      ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
      printf("Setting duty to %d %lx \n", i, ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    printf("Restarting in %d seconds...\n", j);
  }
  printf("Restarting now.\n");
  fflush(stdout);
  esp_restart();
}
