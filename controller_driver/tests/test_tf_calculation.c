#include <tests.h>
#include <tf_calculation.h>
#include <chprintf.h>

static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0, .cr2 = 0, .cr3 = 0
};

TFConf_t tfconf = {
                   .k             = 3.0,
                   .T             = 100.0,
                   .prev_output   = 0.0,
                   .output        = 0.0,
                   .input         = 100.0
};

TFConf_t* p_tfconf = &tfconf;

static void gpt3_callback (GPTDriver *gptp)
{
    (void)gptp;
    tfOutCalculation ( p_tfconf );
}

/*
 * GPT3 configuration. This timer is used as trigger for the transfer function
 * calculation.
 */
static const GPTConfig gpt3cfg1 = {
  .frequency =  100000U,
  .callback  =  gpt3_callback,
  .cr2       =  TIM_CR2_MMS_1,  /* MMS = 010 = TRGO on Update Event.        */
  .dier      =  0U
};

void testTFCalcRouting ( void )
{
    sdStart( &SD7, &sdcfg );
    palSetPadMode( GPIOE, 8, PAL_MODE_ALTERNATE(8) );   // TX
    palSetPadMode( GPIOE, 7, PAL_MODE_ALTERNATE(8) );   // RX

    /* Starting GPT3 driver */
    gptStart(&GPTD3, &gpt3cfg1);
    gptStartContinuous(&GPTD3, gpt3cfg1.frequency / 1000); //1ms


    while ( 1 )
    {
      chprintf( (BaseSequentialStream *)&SD7, "k= %d\t T = %d\t input = %d\t prev_out = %d\t y = %d\n\r",
                (uint8_t) (tfconf.k * 10),
                (uint8_t) (tfconf.T * 10),
                (uint8_t) (tfconf.input * 10),
                (uint8_t) (tfconf.prev_output * 10),
                (int32_t) (tfconf.output * 1000));

       // chprintf( (BaseSequentialStream *)&SD7, "y = :  %d\r\n", (int32_t) (tfconf.output * 10000) );
        chThdSleepMilliseconds( 100 );

    }
}