#include <lld_sensor_module.h>


/************************/
/*** Common ADC1 unit ***/
/************************/

#define COMMON_ADC_NUM_CHANNELS   2
#define COMMON_ADC_BUF_DEPTH      1
static adcsample_t adc_buffer[COMMON_ADC_NUM_CHANNELS * COMMON_ADC_BUF_DEPTH];

static ADCDriver                *commonADCDriver    = &ADCD1;
static GPTDriver                *adcTriggerDriver   = &GPTD4;

/*
 * GPT4 configuration. This timer is used as trigger for the ADC.
 */
static const GPTConfig gpt4cfg1 = {
  .frequency =  100000U,
  .callback  =  NULL,
  .cr2       =  TIM_CR2_MMS_1,  /* MMS = 010 = TRGO on Update Event.        */
  .dier      =  0U
};

static uint16_t analog_in_ch1 = 0, analog_in_ch2 = 0 ;

/*
 * ADC streaming callback.
 */
static void adccallback(ADCDriver *adcp, adcsample_t *buffer, size_t n)
{
    (void)adcp;
    (void)buffer;
    (void)n;

    analog_in_ch1 = adc_buffer[0]; // channel 1
    analog_in_ch2 = adc_buffer[1]; // channel 1

}

/*
 * ADC errors callback, should never happen.
 */
static void adcerrorcallback(ADCDriver *adcp, adcerror_t err)
{
    (void)adcp;
    (void)err;
}

/*
 * ADC conversion group
 */
static const ADCConversionGroup adcgrpcfg1 =
{
    /* Enable circular buffer mode for group (it may be circular or linear) */
    .circular = true,

    /* Number of the analog channels belonging to the conversion group*/
    .num_channels = COMMON_ADC_NUM_CHANNELS,

    /*Callback function associated to the group or @p NULL
     *This callback is called when the conversation ends */
    .end_cb = adccallback,

    /*Error callback or @p NULL*/
    .error_cb = adcerrorcallback,

    /*End of the mandatory fields*/
    /****************************************************************/

    /*ADC control register 1. Choose resolution of the conversion.
     * 0                             --->>>       12-bit resolution
     * ADC_CR1_RES_0                  --->>>      10-bit resolution
     * ADC_CR1_RES_1                  --->>>      8 -bit resolution
     * ADC_CR1_RES_0 | ADC_CR1_RES_1  --->>>      6-bit  resolution
     */
    .cr1 = 0,

    /* ADC control register 2.
     * EXTEN  -  Trigger detection on the rising edge
     * EXTSEL -  Trigger source select (1100: Timer 4 TRGO)
     */
    .cr2 = ADC_CR2_EXTEN_RISING | ADC_CR2_EXTSEL_SRC(0b1100),

    /*Sample time (for each channel[10-15])
     * Maximum ADC frequency = 108 MHz
     * Prescaler = 4 ->> fadc = 108 / 4 = 27 MHz  ->> Tadc = 1/fadc
     * Sample time = 480 * Tadc
     */
    .smpr1 = ADC_SMPR1_SMP_AN10(ADC_SAMPLE_144),

    /*Sample time (for each channel[0-9])*/
    .smpr2 = ADC_SMPR2_SMP_AN3(ADC_SAMPLE_144),

    /*Channel sequence [13-16] or number of channels */
    .sqr1 = ADC_SQR1_NUM_CH(COMMON_ADC_NUM_CHANNELS),

    /*Channel sequence [7-12]*/
    .sqr2 = 0,

    /*Channel sequence [1-6]*/
    .sqr3 = ADC_SQR3_SQ1_N(ADC_CHANNEL_IN3) |
            ADC_SQR3_SQ2_N(ADC_CHANNEL_IN10)

};




static bool     adcInitialized      = false;

void commonADC1UnitInit ( void )
{
  if ( adcInitialized )
      return;

  /* Starting GPT4 driver, it is used for triggering the ADC*/
  gptStart(&GPTD4, &gpt4cfg1);

  /* Activates the ADC1 driver */
  adcStart(&ADCD1, NULL);

  /*analog input on PC0 pin */
  palSetLineMode( LINE_ADC123_IN13, PAL_MODE_INPUT_ANALOG );  // PC0
  palSetLineMode( LINE_ADC123_IN3, PAL_MODE_INPUT_ANALOG );   // PA3

  /*
   * Starts an ADC continuous conversion triggered with a period of
   * 10 millisecond.
   */
   adcStartConversion(&ADCD1, &adcgrpcfg1, adc_buffer, COMMON_ADC_BUF_DEPTH);
   gptStartContinuous(&GPTD4, gpt4cfg1.frequency / 1000);

   adcInitialized = true;

}

adcsample_t commonADC1UnitGetValue ( uint8_t ch )
{
#if 0
    if ( ch > COMMON_ADC_NUM_CHANNELS )
    {
        return 0;
    }
#endif

   // return adc_buffer[0];

    if ( ch == 1 )
    {
        return analog_in_ch1;
    }
    else if ( ch == 2 )
    {
        return analog_in_ch2;
    }
    else
    {
        return 0;
    }

}








