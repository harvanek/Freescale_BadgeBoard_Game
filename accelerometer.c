/*!
 * \file    accelerometer.c
 * \brief   Simple Accelerometr driver
 * \version $Revision: 1.3 $
 * \author  Anthony Huereca
 * reworked on 16-Sep-2009 by Milan Brejl 
 */

#include <mcf51jm128.h> /* include peripheral declarations */
#include "jmbadgeboard.h"  /* include pin assignment */
#include "accelerometer.h"

/*******************************************************************/
/*!
 * Get accelerometer out of sleep mode, turn on ADC clock,
 * and configure ADC
 */
void AccelOn(byte sensitivity)
{
  int i;

  switch(sensitivity) 
  {
  case ACCEL_RANGE_1_5G:
    MMA7260_GSEL1 = 0;  
    MMA7260_GSEL2 = 0; 
    break;
  case ACCEL_RANGE_2G:
    MMA7260_GSEL1 = 0;  
    MMA7260_GSEL2 = 1; 
    break;
  case ACCEL_RANGE_4G:
    MMA7260_GSEL1 = 1;  
    MMA7260_GSEL2 = 0; 
    break;
  case ACCEL_RANGE_6G:
    MMA7260_GSEL1 = 1;  
    MMA7260_GSEL2 = 1; 
    break;
  default:
    MMA7260_GSEL1 = 0;  
    MMA7260_GSEL2 = 0; 
    break;                  
  }
  
  /* Wake up accelerometer */
  MMA7260_SLEEP = 1;
  
  /* Turn on ADC Clock */
  SCGC1_ADC=1;
   
  /* Configure ADC since reset when turned clock off */  
  ADCCFG = 0x44; /* Bus clock as clock source, 12-bit conversion and divisor=4 */
  
  /* Delay to allow time for accel chip to wake up */
  for(i=0;i<3000;i++) 
  { 
    asm("nop");
  }  
}


/*******************************************************************/
/*!
 * Put accelerometer into sleep mode and turn off ADC clock
 */
void AccelOff(void)
{
  MMA7260_GSEL1 = 0;  
  MMA7260_GSEL2 = 0; 

  /* Put accelerometer into sleep mode */
  MMA7260_SLEEP=0; 
  
  /* Turn off ADC Clock */ 
  SCGC1_ADC=0;
}


/*******************************************************************/
/*!
 * Read ADC Values sampled from the Accelerometer
 *  - channel can be one of: ACCELL_CHAN_X, ACCELL_CHAN_Y, ACCELL_CHAN_Z
 *  - returns a 12-bit ADC Value
 */
word AccelRead(byte channel)
{
  /* Select channel and start conversion */
  ADCSC1 = channel;
  /* Waits until ADC conversion is completed */
  while (!(ADCSC1_COCO)) {}
  /* Return ADC Result */
  return ADCR;
}
