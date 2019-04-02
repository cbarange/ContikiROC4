
/*---------------------------------------------------------------------------*/
/* === Clignotement led === */

#include "leds.h"
#include "contiki.h"
#include "sys/etimer.h"
#include <stdio.h>

PROCESS(active_leds_process, "Leds process");
AUTOSTART_PROCESSES(&active_leds_process);

PROCESS_THREAD(active_leds_process, ev, data)
{
  static struct etimer et;
  static int cpt=0;
  PROCESS_BEGIN();
  printf("Activation des leds en cours\n");
  etimer_set(&et,CLOCK_SECOND);
  
  while(1){
	  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
     
     etimer_reset(&et);
	 switch (cpt){
		 case 1:leds_off(LEDS_BLUE);
		 leds_on(LEDS_RED);
		 break;
		 case 2:leds_off(LEDS_RED);
		 leds_on(LEDS_GREEN);
		 break;
		 case 3:leds_off(LEDS_GREEN);
		 leds_on(LEDS_BLUE);
			cpt=0;
	 }
	 cpt++;
	 
  }
  
  PROCESS_END();
  
}

