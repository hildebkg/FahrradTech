#include <stdio.h>
#include "sht3x.h"
#include "i2c_bus.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*** 
 * This file is a quick-and-simple demonstration of using the 
 * espressif/sht3x component to read temperature and humidity 
 * data from a SHT3X sensor with an ESP32. This initializes 
 * the I2C bus (espressif/i2c_bus) and the sensor object. The 
 * sensor is configured to run with its default address of 
 * 0x44 (address pin tied to VSS). Some breakout boards for this 
 * part may tie the address pin to VDD. These have address 0x45.
 * This configures the sensor to run in single shot mode with 
 * high repeatability. Temperature and sensor measurements are 
 * requested every 5 seconds until operation is interrupted.
 * See "TODO:"s for more things to be added.
 */

#define I2C_MASTER_PORT   I2C_NUM_0  // I2C port number
#define I2C_MASTER_SCL_IO 33         // GPIO pin for SCL
#define I2C_MASTER_SDA_IO 32         // GPIO pin for SDA
#define I2C_FREQ_HZ       100000     // I2C clock speed (Hz)

static const char *TAG = "SHT3X_TEST";

void app_main(void)
{
    // measurements
    float temperature = 0;
    float humidity = 0;

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

    // instantiate SHT3x sensor
    sht3x_handle_t sht3x_dev = sht3x_create(i2c_bus, SHT3x_ADDR_PIN_SELECT_VSS);
    if (sht3x_dev == NULL) 
    {
        ESP_LOGE(TAG, "Failed to initialize SHT3x sensor! Terminating.");
        return;
    }
    else 
    {
        ESP_LOGI(TAG, "SHT3x sensor initialized successfully!");
    }

    // set the measurement mode before taking any readings
    sht3x_set_measure_mode(sht3x_dev, SHT3x_SINGLE_HIGH_DISABLED);

    ESP_LOGI(TAG, "Starting infinite measurement loop...");
    vTaskDelay(pdMS_TO_TICKS(1000));

    // read sensor data every 5 seconds in an infinite loop
    while (1) 
    {
        // get single temperature reading and single humidity reading
        if (sht3x_get_single_shot(sht3x_dev, &temperature, &humidity) == ESP_OK) 
        {
            ESP_LOGI(TAG, "Temperature: %.2f °C, Humidity: %.2f %%", temperature, humidity);
        } 
        else 
        {
            ESP_LOGE(TAG, "Failed to read from SHT3x sensor!");
        }

        /*** TODO: 
         * Add writeup here about why resetting the measurement mode here is 
         * necessary. Check if there is a better way to do this. 
         */
        // set the measurement mode to "single" before taking a reading
        sht3x_set_measure_mode(sht3x_dev, SHT3x_SINGLE_HIGH_DISABLED);

        // wait 5 seconds before taking the next measurement
        vTaskDelay(pdMS_TO_TICKS(5000));
    }

    /*** cleanup! this part is never reached due to the infinite loop.
    * TODO: add example that uses periodic acquision mode in a finite 
    * loop (which would use the following two lines)
    */
    // sht3x_delete(&sht3x_dev);
    // i2c_bus_delete(i2c_bus);
}
