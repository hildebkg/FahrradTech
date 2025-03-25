#include <stdio.h>
#include "bme280.h"
#include "i2c_bus.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*** 
 * This file is a quick-and-simple demonstration of using the 
 * espressif/bme280 component to read barometric pressure, temperature, 
 * and humidity and calculate altitude with a BME280 sensor and an 
 * ESP32. This initializes the I2C bus (espressif/i2c_bus) and the 
 * sensor object. My particular sensor board is configured to run with 
 * the default address of 0x76. Measurements for pressure, temperature 
 * and humidity are requested every 5 seconds until operation is 
 * interrupted. Altitude is also calculated every 5 seconds (it is not 
 * measured but rather calculated from a pressure measurement).
 * NOTE: I made several changes to the original component source code. 
 * Some were for debugging, but others were functional. There were bugs 
 * in the way pressure and altitude were calculated which have now been 
 * fixed. These fixes are present in the managed_components folder of 
 * this project.
*/

#define I2C_MASTER_PORT   I2C_NUM_0  // I2C port number
#define I2C_MASTER_SCL_IO 33         // GPIO pin for SCL
#define I2C_MASTER_SDA_IO 32         // GPIO pin for SDA
#define I2C_FREQ_HZ       100000     // I2C clock speed (Hz)
#define SEA_LEVEL_PRESS   1013.25f   // standard sea level pressure (hPa)

static const char *TAG = "BME280_DEMO";

void app_main(void)
{
    // measurements
    float temperature = 0;
    float pressure = 0;
    float humidity = 0; 
    float altitude = 0;

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

    // instantiate BME280 sensor
    bme280_handle_t bme280 = bme280_create(i2c_bus, BME280_I2C_ADDRESS_DEFAULT);
    if (bme280 == NULL) 
    {
        ESP_LOGE(TAG, "Failed to create BME280 sensor! Terminating.");
        i2c_bus_delete(&i2c_bus);
        return;
    }
    else 
    {
        ESP_LOGI(TAG, "BME280 sensor created successfully!");
    }

    // initialize BME280 sensor
    if (bme280_default_init(bme280) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to initialize BME280 sensor! Terminating.");
        bme280_delete(&bme280);
        i2c_bus_delete(&i2c_bus);
        return;
    }
    else
    {
        ESP_LOGI(TAG, "BME280 sensor initialized successfully!");
    }

    /*** NOTE:  adding the delay here got rid of the issue where the first 
     * set of measurements taken below would always fail
    */
    vTaskDelay(500 / portTICK_PERIOD_MS);

    // infinite loop to read sensor data
    ESP_LOGI(TAG, "Starting infinite measurement loop...");    
    while (1) 
    {
        // read temperature
        if (bme280_read_temperature(bme280, &temperature) == ESP_OK) 
        {
            ESP_LOGI(TAG, "Temperature: %.2f °C", temperature);
        } 
        else 
        {
            ESP_LOGE(TAG, "Failed to read temperature!");
        }

        // read pressure
        if (bme280_read_pressure(bme280, &pressure) == ESP_OK) 
        {
            ESP_LOGI(TAG, "Pressure: %.2f hPa", pressure);
        } 
        else 
        {
            ESP_LOGE(TAG, "Failed to read pressure!");
        }

        // read humidity
        if (bme280_read_humidity(bme280, &humidity) == ESP_OK) 
        {
            ESP_LOGI(TAG, "Humidity: %.2f %%", humidity);
        } 
        else 
        {
            ESP_LOGE(TAG, "Failed to read humidity!");
        }

        // calculate altitude
        if (bme280_read_altitude(bme280, SEA_LEVEL_PRESS, &altitude) == ESP_OK) 
        {
            ESP_LOGI(TAG, "Altitude: %.2f meters", altitude);
        } 
        else 
        {
            ESP_LOGE(TAG, "Failed to calculate altitude!");
        }

        ESP_LOGI(TAG, "-----------------------------");
        
        // wait 5 seconds before taking the next measurement
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }

    // cleanup! this part is never reached due to the infinite loop.
    //bme280_delete(&bme280);
    //i2c_bus_delete(&i2c_bus);
}
