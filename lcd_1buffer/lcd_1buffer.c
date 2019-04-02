#include "contiki.h"

#include <stdio.h> 

#include "sys/etimer.h"
#include "dev/cc2538-sensors.h"
#include "als-sensor.h"
#include "sys/rtimer.h"
//#include "cpu.h"
//#include "dev/uart.h"
//#include "dev/serial-line.h"
//#include "dev/sys-ctrl.h"

#include "lcd_dogm128_6_lcd.h"
#include "bsp.h"
//#include <stdlib.h>
//#include "bsp_key.h"
//#include "sys_ctrl.h"           // Access to driverlib SysCtrl fns
//#include "interrupt.h"          // Access to driverlib interrupt fns




static char temperature[100];
static void lcd_start(void);


PROCESS(lcd_process, "LCD process");
AUTOSTART_PROCESSES(&lcd_process);

PROCESS_THREAD(lcd_process, ev, data)
{
	static struct etimer et;
	PROCESS_BEGIN();
	lcd_start();
	etimer_set(&et,CLOCK_SECOND*2);
	while(1){
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		etimer_reset(&et);
		lcdBufferClearPage(0,eLcdPage4);
		sprintf((char*)temperature,"%d mC",cc2538_temp_sensor.value(CC2538_SENSORS_VALUE_TYPE_RAW)-cc2538_temp_sensor.value(CC2538_SENSORS_VALUE_TYPE_RAW));
		lcdBufferPrintStringAligned(0, temperature, eLcdAlignCenter,eLcdPage4);
		lcdBufferSetHLine(0, 15, 112, 8*eLcdPage4-2);
		lcdBufferSetHLine(0, 15, 112, 8*eLcdPage5);
		lcdSendBuffer(0);
		
	}
	
	PROCESS_END();
}

static void lcd_start(void){
    // Initialize board
    bspInit(BSP_SYS_CLK_SPD);
    // Initialize SPI interface
    bspSpiInit(BSP_SPI_CLK_SPD);
    // Initialize LCD
    lcdInit();
    // Clear local LCD buffers
    lcdBufferClear(0);
    // Write default buffer
    lcdBufferPrintString(0, "Temperature", 1, eLcdPage0);
    lcdBufferInvertPage(0, 0, 127, eLcdPage0);
	
	// Set and Write temperature
	sprintf((char*)temperature,"%d",cc2538_temp_sensor.value(CC2538_SENSORS_VALUE_TYPE_RAW));
    lcdBufferPrintStringAligned(0, temperature, eLcdAlignCenter,eLcdPage4);
	//================
	
	// Display Rectangle
	lcdBufferSetHLine(0, 15, 112, 8*eLcdPage4-2);
    lcdBufferSetHLine(0, 15, 112, 8*eLcdPage5);
    lcdBufferSetVLine(0, 15, 8*eLcdPage4-2, 8*eLcdPage5);
    lcdBufferSetVLine(0, 112, 8*eLcdPage4-2, 8*eLcdPage5);
	//================
	
	// Send the default buffer to LCD
    lcdSendBuffer(0);
}
