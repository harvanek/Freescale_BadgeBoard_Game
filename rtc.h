/*!
 * \file    rtc.h
 * \brief   Defines and macros for RTC
 * \version Revision: 0.2
 * \author  Milan Brejl
 */

#ifndef _RTC_H_
#define _RTC_H_


/************************** API Function Prototypes ***************************/          
void RTCStart(byte interruptPeriodInMiliSec);
void RTCStop(void);
void RTCClearInterrupt(void);


/*******************************************************************/

#endif /* _RTC_H_ */