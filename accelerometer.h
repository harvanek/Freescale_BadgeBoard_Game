/*!
 * \file    accelerometer.h
 * \brief   Defines and macros for accelerometer.c
 * \version $Revision: 1.2 $
 * \author  Anthony Huereca
 * reworked on 16-Sep-2009 by Milan Brejl 
 */

#ifndef _ACCELEROMETER_H_
#define _ACCELEROMETER_H_

/********************************* Macros *************************************/          
#define ACCEL_RANGE_1_5G  0
#define ACCEL_RANGE_2G    1
#define ACCEL_RANGE_4G    2
#define ACCEL_RANGE_6G    3

#define ACCEL_CHAN_X      0x04
#define ACCEL_CHAN_Y      0x05
#define ACCEL_CHAN_Z      0x06

/************************** API Function Prototypes ***************************/          
void AccelOn(byte sensitivity);
void AccelOff(void);
word AccelRead(byte channel);

#endif /* _ACCELEROMETER_H_ */