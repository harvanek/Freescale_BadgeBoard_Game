/*!
 * \file    rtc.h
 * \brief   Defines and macros for RTC
 * \version Revision: 0.2
 * \author  Milan Brejl
 */

#include <MCF51JM128.h>
#include "rtc.h"

/*******************************************************************/
/*!
 * Initialize and Start RTC running a 1 to 255 mSec interrupt
 */
void RTCStart(byte interruptPeriodInMiliSec)
{   
  /* Turn on RTC module */
  SCGC2_RTC = 1; 
  
  /* Set RTC modulo */
  RTCMOD = interruptPeriodInMiliSec;
  
  /* Select 1kHz lop-power oscilator as RTC clock source */
  RTCSC_RTCLKS = 0;
  
  /* Enable RTC interrupts */
  RTCSC_RTIE = 1;
  
  /* Select prescaler 1 and start RTC */
  RTCSC_RTCPS = 8;
}

/*******************************************************************/
/*!
 * Stop RTC module
 */
void RTCStop(void)
{
  /* Stop RTC */
  RTCSC_RTCPS = 0;

  /* Turn off RTC module */
  SCGC2_RTC = 0; 
}

/*******************************************************************/
/*!
 * Clear RTC interrupt flag
 */
void RTCClearInterrupt(void)
{
  RTCSC_RTIF = 1;
}
