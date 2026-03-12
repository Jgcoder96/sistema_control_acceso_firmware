#include "driver/i2c.h"

#include "rtc_config.h"

// static const char *TAG = "RTC_I2C_CONFIG";

esp_err_t rtc_i2c_config() {
  i2c_config_t i2c_config = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = I2C_MASTER_SDA_IO,
    .scl_io_num = I2C_MASTER_SCL_IO,
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = I2C_MASTER_FREQ_HZ,
  };

  i2c_param_config(I2C_MASTER_NUM, &i2c_config);

  return i2c_driver_install(I2C_MASTER_NUM, i2c_config.mode, 0, 0, 0);
}