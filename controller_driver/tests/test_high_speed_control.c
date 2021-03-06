#include <tests.h>
#include <chprintf.h>

#include <high_speed_control.h>
#include <feedback.h>
#include <pedals.h>

static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0, .cr2 = 0, .cr3 = 0
};


void TestHSC1 ( void )
{
    sdStart( &SD3, &sdcfg );
    palSetPadMode( GPIOD, 8, PAL_MODE_ALTERNATE(7) );   // TX
    palSetPadMode ( GPIOD, 9, PAL_MODE_ALTERNATE(7) );   // RX

    palSetPadMode( GPIOB, 7, PAL_MODE_OUTPUT_PUSHPULL );    //Led
    palSetPadMode( GPIOB, 0, PAL_MODE_OUTPUT_PUSHPULL );    //Led
    palSetPadMode( GPIOB, 14, PAL_MODE_OUTPUT_PUSHPULL );   //Led


    hscInit();
    feedbackInit();
    pedalsInit();



    uint8_t sd_buff[10];


    while(1) {
        palToggleLine(LINE_LED1);

        sdReadTimeout( &SD3, sd_buff, 9, TIME_IMMEDIATE );


        if(sd_buff[0]=='y') hscStart();
        if(sd_buff[0]=='h') hscStop();
        if(sd_buff[0]=='u') pedalsClutchRelease(5000);
        if(sd_buff[0]=='k') pedalsClutchPress(10000);


       // if(sd_buff[4]=='f') hscTestSetCurrentSpeed (  atoi(sd_buff) );

//gazelGetSpeed()
//pedalsBrakeGetPosition()

        chprintf( (BaseSequentialStream *)&SD3, "speed %.02f \t brake %d \t \r\n", gazelGetSpeed(), pedalsBrakeGetPosition()  );


        for (int i = 0; i < 9; i++)
        {
          sd_buff[i]='?';
        }

        chThdSleepMilliseconds( 500 );
    }
}
