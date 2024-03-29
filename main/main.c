#include <stdio.h>

#include "app_config.h"
#include "esp_attr.h"
#include "esp_sleep.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "prod_app.h"

void app_main( void )
{
  ProdApp_Start();
}
