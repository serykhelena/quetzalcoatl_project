#include <ch.h>
#include <hal.h>
#include <communication.h>

/* For module test.
Obtaining speed and angle values and sending them is processed.
*/

void funct_on_stop(void)
{
    palToggleLine(LINE_LED1);
    comm_dbgprintf_warning("Warning! I'm stop \n ");
}

void funct_on_start(void)
{
    palToggleLine(LINE_LED2);
    comm_dbgprintf_error("Oops... Error I'm start\n");
}

void fucnt_on_set(comm_speed_t speed, comm_steer_t street)
{
    palToggleLine(LINE_LED3);
    comm_dbgprintf_info("I'm get value speed dbg %d, value angle dbg %d\n", speed, street);
}

void funct_on_interrupt_timer(void)
{
    palToggleLine(LINE_LED1);
    palToggleLine(LINE_LED2);
    palToggleLine(LINE_LED3);
    
    comm_dbgprintf_error("Timer overflow interrupt \n");
}

void testCommunication(void)
{
    communicationEventFun_t structForFunc = getDefaultCfg();
 
    structForFunc.on_set    = fucnt_on_set; //
    structForFunc.on_start  = funct_on_start;
    structForFunc.on_stop   = funct_on_stop;
    structForFunc.on_interrupt_timer = funct_on_interrupt_timer;
    
    uint32_t time_for_vt_MS = 2000;

    comm_init(&structForFunc, time_for_vt_MS, true);
    comm_start_protocol();

    while ( true )
    {   palToggleLine(LINE_LED3);
        chThdSleepMilliseconds(1000);
    }
}
