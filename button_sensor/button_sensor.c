
/* === Buttun test === */

#include "dev/button-sensor.h"
#include "leds.h"
#include "contiki.h"
#include "sys/etimer.h"
#include <stdio.h>

PROCESS(active_buttun_process, "buttun process");
PROCESS(active_timer_process, "timmer process");
AUTOSTART_PROCESSES(&active_buttun_process, &active_timer_process);
int pause=0;
int cpt=0;
PROCESS_THREAD(active_timer_process, ev, data){
	static struct etimer et;
	PROCESS_BEGIN();	
	etimer_set(&et,CLOCK_SECOND);	
	while(1){
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		etimer_reset(&et);
		if(!pause){
			cpt++;
			switch(cpt){
				case 1:
				leds_on(1);
				leds_off(8);
				break;
				case 2:
				leds_on(2);
				leds_off(1);
				break;
				case 3:
				leds_on(4);
				leds_off(2);
				break;
				case 4:
				leds_on(8);
				leds_off(4);
				cpt=0;
			}
			printf("%d %d /",pause,cpt);
		}			
	}
	PROCESS_END();
}
PROCESS_THREAD(active_buttun_process, ev, data){
	PROCESS_BEGIN();
	SENSORS_ACTIVATE(button_sensor);
	while(1){
		PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);	
		pause=(pause-1)*-1;		
	}
	PROCESS_END();  
}
