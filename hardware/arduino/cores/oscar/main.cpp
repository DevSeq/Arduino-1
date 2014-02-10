#include <Arduino.h>
#include <config.h>
#include <wdt_drv.h>
#include <power_drv.h>

//Main entry point for OSCAR.
//Starts the scheduler, and then runs threads afterwards
//Check out device_template_task for the Arduino-side threads
int main(void)
{
  wdtdrv_disable();
  Clear_prescaler();
  scheduler();
  return 0;
}

//Set up a lower-level initialization function
//Increment the CPU clock before RAM initialization
#ifdef __GNUC__
char __low_level_init(void) __atribute__ ((section(".init3"), naked));
#endif

#ifdef __cplusplus
extern "C" {
#endif
char __low_level_init()
{
  Clear_prescaler();
  return 1;
}
#ifdef __cplusplus
}
#endif
