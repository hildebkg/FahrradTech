#include <stdio.h>
#include "bmp280.h"
#include "i2c_bus.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*** 
 * This file is a quick-and-simple demonstration of using the 
 * maucke/bmp280 component to read barometric pressure and 
 * calculate altitude with a BMP280 sensor and an ESP32. This 
 * initializes the I2C bus (espressif/i2c_bus) and the sensor object. 
 * The sensor is configured to run with its default address of 
 * 0x76. Measurements are requested every 5 seconds until operation 
 * is interrupted.
 * See "TODO:"s for more things to be added.
 */

#define I2C_MASTER_PORT   I2C_NUM_0  // I2C port number
#define I2C_MASTER_SCL_IO 33         // GPIO pin for SCL
#define I2C_MASTER_SDA_IO 32         // GPIO pin for SDA
#define I2C_FREQ_HZ       100000     // I2C clock speed (Hz)

static const char *TAG = "BMP280_TEST";

void app_main(void)
{
    // configure settings for I2C bus
    i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQ_HZ,
    };

    // create I2C bus instance
    i2c_bus_handle_t i2c_bus = i2c_bus_create(I2C_MASTER_PORT, &i2c_conf);
    if (i2c_bus == NULL) 
    {
        ESP_LOGE(TAG, "Failed to initialize I2C bus! Terminating.");
        return;
    }
    else 
    {
        ESP_LOGI(TAG, "I2C bus initialized successfully!");
    }

    // instantiate BMP280 sensor
    bmp280_handle_t bmp280 = bmp280_create(i2c_bus, BMP280_I2C_ADDRESS_DEFAULT);
    if (bmp280 == NULL) 
    {
        ESP_LOGE(TAG, "Failed to create BMP280 sensor! Terminating.");
        i2c_bus_delete(&i2c_bus);
        return;
    }

    // initialize BMP280 sensor
    if (bmp280_default_init(bmp280) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to initialize BMP280 sensor! Terminating.");
        bmp280_delete(&bmp280);
        i2c_bus_delete(&i2c_bus);
        return;
    }

    ESP_LOGI(TAG, "BMP280 sensor initialized successfully");

    // infinite loop to read sensor data
    while (1) 
    {
        float temperature, pressure, altitude;

        // read temperature
        if (bmp280_read_temperature(bmp280, &temperature) == ESP_OK) 
        {
            ESP_LOGI(TAG, "Temperature: %.2f °C", temperature);
        } 
        else 
        {
            ESP_LOGE(TAG, "Failed to read temperature!");
        }

        // read pressure
        if (bmp280_read_pressure(bmp280, &pressure) == ESP_OK) 
        {
            // convert Pa to hPa before outputting
            ESP_LOGI(TAG, "Pressure: %.2f hPa", pressure / 100.0f);
        } 
        else 
        {
            ESP_LOGE(TAG, "Failed to read pressure!");
        }

        // read altitude (assume sea level pressure = 1013.25 hPa)
        if (bmp280_read_altitude(bmp280, 1013.25f, &altitude) == ESP_OK) 
        {
            ESP_LOGI(TAG, "Altitude: %.2f meters", altitude);
        } 
        else 
        {
            ESP_LOGE(TAG, "Failed to read altitude!");
        }

        // wait 5 seconds before taking the next measurement
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }

    /*** cleanup! this part is never reached due to the infinite loop.
    * TODO: add example that uses a finite loop (which would use the 
    * following two lines)
    */
    //bmp280_delete(&bmp280);
    //i2c_bus_delete(&i2c_bus);
}
