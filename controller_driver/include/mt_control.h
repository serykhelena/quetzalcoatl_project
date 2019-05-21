#ifndef INCLUDE_MT_CONTROL
#define INCLUDE_MT_CONTROL

#include <common.h>



/* Debug function */
void resetLEDs ( void );

/*
 * @brief     perephery initialization
 *            driver control unit initialization
 * @note      In function sets up (x,y) coordinates of "gear" positions
 */
void mtControlInit ( void );

/*
 * @brief    move carriage to neutral position
 */
int8_t shiftMTToNeutral ( uint16_t speed );
/*
 * @brief    move carriage to position corresponding to selected gear
 */
int8_t shiftMTToNextGear (int8_t gear_num, uint16_t speed);

#if 0
void shiftToFirstGear ( void );
void shiftToSecondGear ( void );
void shiftToThirdGear ( void );
void shiftToForthGear ( void );
void shiftToFifthGear ( void );
void shiftToReverseGear ( void );
#endif


void verticalCaclibration( bool dir, uint16_t speed, uint16_t step );
void gorisontalCaclibration( bool dir, uint16_t speed, uint16_t step );
void setTrackedMode ( uint16_t vertical_speed, uint16_t gorisontal_speed );
void setTrackedModePositionVerticalMotor ( int32_t pos );
void setTrackedModePositionGorisontalMotor ( int32_t pos );
void verticalMotorStop ( void );
void gorisontalMotorStop ( void );
void setVerticalMotorMaxPosition ( int32_t max_pos );
void setGorisontalMotorMaxPosition ( int32_t max_pos );

int32_t getVerticalPosition ( void );
int32_t getGorisontalPosition ( void );
int32_t getVerticalMaxPosition ( void );
int32_t getGorisontalMaxPosition ( void );
int32_t getVerticalTargetPosition ( void );
int32_t getGorisontalTargetPosition ( void );
bool getGorisontalState ( void );
bool getVerticalState ( void );
uint8_t getGorisontalMode ( void );
uint8_t getVerticalMode ( void );
uint16_t getGorisontalSpeed ( void );
uint16_t getVerticalSpeed ( void );



#endif /* INCLUDE_MT_CONTROL */
