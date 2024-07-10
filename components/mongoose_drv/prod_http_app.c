
/**
 *******************************************************************************
 * @file    prod_http_app.c
 * @author  Dmytro Shevchenko
 * @brief   Production HTTP app
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "app_config.h"
#include "dev_config.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "mongoose.h"
#include "ota_drv.h"

/* Private macros ------------------------------------------------------------*/
#define HTTP_URL "http://0.0.0.0:8000"

/* Private functions declaration ---------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

static const char* ota_state_name[] =
  {
    [OTA_DRIVER_STATE_IDLE] = "idle",
    [OTA_DRIVER_STATE_DOWNLOAD] = "download",
    [OTA_DRIVER_STATE_ERROR] = "error",
    [OTA_DRIVER_STATE_DONWLOAD_FINISHED] = "finish",
};

static void _restart_timer_cb( TimerHandle_t xTimer )
{
  printf( "Restart callback\n\r" );
  esp_restart();
}

static void fn( struct mg_connection* c, int ev, void* ev_data )
{
  if ( ev == MG_EV_HTTP_MSG )
  {
    struct mg_http_message* hm = (struct mg_http_message*) ev_data;
    if ( mg_http_match_uri( hm, "/api/sn" ) )
    {
      if ( mg_vcasecmp( &hm->method, "PUT" ) == 0 || mg_vcasecmp( &hm->method, "POST" ) == 0 )
      {
        char sn[33] = { 0 };
        assert( hm->body.len <= sizeof( sn ) );
        memcpy( sn, hm->body.ptr, hm->body.len );
        assert( DevConfig_SetSerialNumber( sn ) );
        mg_http_reply( c, 200, "", "OK\n" );
      }
      else if ( mg_vcasecmp( &hm->method, "GET" ) == 0 )
      {
        const char* sn = DevConfig_GetSerialNumber();
        mg_http_reply( c, 200, "", "%s\n", sn );
      }
      else
      {
        mg_http_reply( c, 400, "", "FAIL\n" );
      }
    }
    else if ( mg_http_match_uri( hm, "/api/ota" ) )
    {
      if ( mg_vcasecmp( &hm->method, "PUT" ) == 0 || mg_vcasecmp( &hm->method, "POST" ) == 0 )
      {
        char url[256] = { 0 };
        assert( hm->body.len <= sizeof( url ) );
        memcpy( url, hm->body.ptr, hm->body.len );
        if ( OTA_Download( url ) )
        {
          mg_http_reply( c, 200, "", "OK\n" );
        }
        else
        {
          mg_http_reply( c, 500, "", "Internal Server Error\n" );
        }
      }
      else
      {
        mg_http_reply( c, 400, "", "FAIL\n" );
      }
    }
    else if ( mg_http_match_uri( hm, "/api/ota/state" ) )
    {
      if ( mg_vcasecmp( &hm->method, "GET" ) == 0 )
      {
        size_t percentage = OTA_GetDownloadPercentage();
        const char* state = ota_state_name[OTA_GetState()];
        mg_http_reply( c, 200, "", "{\"percentage\":%d, \"state\":\"%s\"}\n", percentage, state );
      }
      else
      {
        mg_http_reply( c, 400, "", "FAIL\n" );
      }
    }
    else if ( mg_http_match_uri( hm, "/api/restart" ) )
    {
      mg_http_reply( c, 200, "", "restart after 5 seconds" );
      TimerHandle_t timer = xTimerCreate( "Timer", MS2ST( 5000 ), pdTRUE, (void*) 0, _restart_timer_cb );
      xTimerStart( timer, 0 );
    }
    else
    {
      mg_http_reply( c, 405, "", "Method Not Allowed\n" );
    }
  }
}

static void _task( void* argv )
{
  struct mg_mgr mgr;
  mg_mgr_init( &mgr );    // Init manager
  mg_log_set( MG_LL_DEBUG );    // Set log level
  mg_http_listen( &mgr, HTTP_URL, fn, &mgr );    // Setup listener
  for ( ;; )
    mg_mgr_poll( &mgr, 1000 );    // Event loop
  mg_mgr_free( &mgr );    // Cleanup
}

/* Public functions ---------------------------------------------------------*/

void ProdHTTPApp_Init( void )
{
  xTaskCreate( _task, "mongoose", 8096, NULL, 13, NULL );
}
