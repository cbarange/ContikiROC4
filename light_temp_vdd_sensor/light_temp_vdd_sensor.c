/* === Light sensor === */

#include "contiki.h"
#include "sys/etimer.h"
#include <stdio.h>
#include "als-sensor.h"


#include "dev/sht11/sht11.h"
#include "dev/sht11/sht11-sensor.h"


#include "cpu.h"
#include "dev/vdd3-sensor.h"
#include "sys/rtimer.h"
#include "dev/leds.h"
#include "dev/uart.h"
#include "dev/cc2538-sensors.h"
#include "dev/button-sensor.h"
#include "dev/als-sensor.h"
#include "dev/serial-line.h"
#include "dev/sys-ctrl.h"


PROCESS(sensor, "Sensor process");
AUTOSTART_PROCESSES(&sensor);

PROCESS_THREAD(sensor, ev, data){
	static struct etimer et;
	
	PROCESS_BEGIN();
	etimer_set(&et,CLOCK_SECOND);
	while(1){
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		etimer_reset(&et);
		printf("Luminosite = %d \n",als_sensor.value(0));
		printf("Temperature = %d mC\n",cc2538_temp_sensor.value(CC2538_SENSORS_VALUE_TYPE_CONVERTED));
		printf("vdd3 %d \n\n",vdd3_sensor.value(CC2538_SENSORS_VALUE_TYPE_CONVERTED));
	}
	PROCESS_END();
  
}