#include "parameters.h"

#include "nvs.h"
#include "nvs_flash.h"
#include "parse_cmd.h"

#define MODULE_NAME "[PARAM] "
#define DEBUG_LVL   PRINT_INFO

#if CONFIG_DEBUG_MENU_BACKEND
#define LOG( _lvl, ... ) \
  debug_printf( DEBUG_LVL, _lvl, MODULE_NAME __VA_ARGS__ )
#else
#define LOG( PRINT_INFO, ... )
#endif

void parameters_debugPrint( void )
{
}

void parameters_debugPrintValue( parameter_value_t val )
{
}

bool parameters_save( void )
{
  return false;
}

void parameters_setDefaultValues( void )
{
}

uint32_t parameters_getValue( parameter_value_t val )
{
  return 0;
}

uint32_t parameters_getMaxValue( parameter_value_t val )
{
  return 0;
}

uint32_t parameters_getMinValue( parameter_value_t val )
{
  return 0;
}

uint32_t parameters_getDefaultValue( parameter_value_t val )
{
  return 0;
}

bool parameters_setValue( parameter_value_t val, uint32_t value )
{
  return false;
}

bool parameters_setString( parameter_string_t val, const char* str )
{
  return false;
}

bool parameters_getString( parameter_string_t val, char* str, uint32_t str_len )
{
  return false;
}

void parameters_init( void )
{
}
