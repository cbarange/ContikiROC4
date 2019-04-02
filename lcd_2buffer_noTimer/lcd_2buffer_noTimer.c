#include "contiki.h"

#include <stdio.h> 

#include "sys/etimer.h"
#include "als-sensor.h"
#include "cpu.h"
#include "sys/rtimer.h"
#include "dev/uart.h"
#include "dev/cc2538-sensors.h"
#include "dev/serial-line.h"
#include "dev/sys-ctrl.h"
#include "dev/leds.h"

#include <stdlib.h>
#include "lcd_dogm128_6_lcd.h"
#include "bsp.h"
#include "bsp_key.h"
#include "sys_ctrl.h"           // Access to driverlib SysCtrl fns
#include "interrupt.h"          // Access to driverlib interrupt fns


static char secondLcdBuf[LCD_BYTES];
static char temperature[100];
static char luminosite[100];
static void lcd_start(void);
static void look_event(void);
static struct etimer et;
static uint8_t ui8BtnPressed = 0;
static bool bShowingDefault = true;

PROCESS(lcd_process, "LCD process");
AUTOSTART_PROCESSES(&lcd_process);

PROCESS_THREAD(lcd_process, ev, data)
{
	PROCESS_BEGIN();
	etimer_set(&et,CLOCK_SECOND*2);
	lcd_start();
	while(1){
		look_event();
	}
	PROCESS_END();
}
static void look_event(void){
	ui8BtnPressed = bspKeyGetDir();
        if(bShowingDefault){
			sprintf((char*)temperature,"%d",cc2538_temp_sensor.value(CC2538_SENSORS_VALUE_TYPE_RAW));
			lcdBufferClearPage(0,eLcdPage4);
			lcdBufferPrintStringAligned(0, temperature, eLcdAlignCenter,eLcdPage4);
			lcdBufferSetHLine(0, 15, 112, 8*eLcdPage4-2);
			lcdBufferSetHLine(0, 15, 112, 8*eLcdPage5);
			lcdSendBuffer(0);
		}else{
			sprintf((char*)luminosite,"%d",als_sensor.value(0));
			lcdBufferClearPage(secondLcdBuf,eLcdPage4);
			lcdBufferPrintStringAligned(secondLcdBuf, luminosite, eLcdAlignCenter,eLcdPage4);
			lcdBufferSetHLine(secondLcdBuf, 15, 112, 8*eLcdPage4-2);
			lcdBufferSetHLine(secondLcdBuf, 15, 112, 8*eLcdPage5);
			lcdSendBuffer(secondLcdBuf);
		}
		if(ui8BtnPressed == BSP_KEY_EVT_LEFT && bShowingDefault){
            // Smooth leftward transition
            lcdSendBufferAnimated(secondLcdBuf, 0, eLcdSlideLeft);
            bShowingDefault = false;
        }else if (ui8BtnPressed == BSP_KEY_EVT_RIGHT && !bShowingDefault){
			// Smooth rightward transition
            lcdSendBufferAnimated(0, secondLcdBuf, eLcdSlideRight);
            bShowingDefault = true;
        }
}


static void lcd_start(void){
    
    // Initialize board
    bspInit(BSP_SYS_CLK_SPD);
    // Initialize SPI interface
    bspSpiInit(BSP_SPI_CLK_SPD);
    // Initialize keys driver
	bspKeyInit(BSP_KEY_MODE_ISR);
    // Initialize LCD
    lcdInit();
    // Clear local LCD buffers
    lcdBufferClear(0);
    lcdBufferClear(secondLcdBuf);
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
	
	lcdBufferPrintStringAligned(0, "Press LEFT", eLcdAlignCenter,eLcdPage7);
    
    
    // Write second buffer
    lcdBufferPrintString(secondLcdBuf, "Luminosite", 1, eLcdPage0);
    lcdBufferInvertPage(secondLcdBuf, 0, 127, eLcdPage0);
	
	// Set and Write luminosite
    sprintf((char*)luminosite,"%d",als_sensor.value(0));
    lcdBufferPrintStringAligned(secondLcdBuf, luminosite, eLcdAlignCenter,eLcdPage4);
	//================
    	
    // Display Rectangle
	lcdBufferSetHLine(secondLcdBuf, 15, 112, 8*eLcdPage4-2);
    lcdBufferSetHLine(secondLcdBuf, 15, 112, 8*eLcdPage5);
    lcdBufferSetVLine(secondLcdBuf, 15, 8*eLcdPage4-2, 8*eLcdPage5);
    lcdBufferSetVLine(secondLcdBuf, 112, 8*eLcdPage4-2, 8*eLcdPage5);
	//================
	
    // Display buttom
	lcdBufferPrintStringAligned(secondLcdBuf, "Press RIGHT", eLcdAlignCenter,eLcdPage7);
    //================
	
	// Send the default buffer to LCD
    lcdSendBuffer(0);
    // Enable interrupts on needed keys
    bspKeyIntEnable(BSP_KEY_LEFT|BSP_KEY_RIGHT|BSP_KEY_UP);
    // Enable global interrupts
    IntMasterEnable();
}
