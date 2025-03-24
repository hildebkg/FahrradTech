/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include "bmp280.h"
#include "esp_system.h"
#include "esp_log.h"

#define I2C_MASTER_SCL_IO 26      /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 25      /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0  /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000 /*!< I2C master clock frequency */

static const char *TAG = "bmp280 test";
i2c_bus_handle_t i2c_bus = NULL;
static bmp280_handle_t bmp280 = NULL;

/**
 * @brief i2c master initialization
 */
static void i2c_bus_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = IIC_SDA_NUM,
        .scl_io_num = IIC_SCL_NUM,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {0},
        .clk_flags = 0,
    };
    conf.master.clk_speed = 400000,
    i2c_bus = i2c_bus_create(IIC_MASTER_NUM, &conf);
}

/**
 * @brief i2c master initialization
 */
static void i2c_sensor_bmp280_init(void)
{
    esp_err_t ret;
    bmp280 = bmp280_create(i2c_bus, BMP280_I2C_ADDRESS);
    ESP_ERROR_CHECK(bmp280_default_init(bmp280));
}

void app_main(void)
{
    static struct tm time_user;
    i2c_bus_init();
    i2c_sensor_bmp280_init();
    while (1)
    {
        float pressure = 0.0f;
        if (ESP_OK == bmp280_read_pressure(bmp280, &pressure))
        {
            ESP_LOGI(TAG, "pressure:%f ", pressure);
        }
        float temperature = 0.0f;
        if (ESP_OK == bmp280_read_temperature(bmp280, &temperature))
        {
            ESP_LOGI(TAG, "temperature:%f ", temperature);
        }
        vTaskDelay(1000);
    }
}