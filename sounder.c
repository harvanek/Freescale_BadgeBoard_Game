/*!
 * \file    sounder.c
 * \brief   Simple Sounder driver using TPM1 for PWM generation
 * \version Revision: 0.1
 * \author  Milan Brejl
 */

#include <mcf51jm128.h> /* include peripheral declarations */
#include "sounder.h"

/*******************************************************************/
/*!
 * Turn TPM1 clock on and start PWM generation
 */
void SounderOn(word period)
{
  /* Turn on TPM1 clock */
  SCGC1_TPM1=1;
  /* If turned to sleep, go to wait instead of stop 
     (TPM does not run in stop mode) */
  asm ( mov3q #4,d0; bset.b d0,SOPT1; );
  /* Reset TPM1 Counter */
  TPM1CNTH = 0;
  /* Setup TPM1: no IRQs, bus clock, prescaler=1 */
  TPM1SC=0x08;
  /* Setup Channel 4: no IRQs, high-true PWM */
  TPM1C4SC=0x28;
  /* Write period to TPM1 Modulo Register */
  TPM1MOD = period;
  /* Write Channel 4 Value Register for 25% duty-cycle */
  TPM1C4V = period>>2;
}


/*******************************************************************/
/*!
 * Adjust PWM period
 */
void SounderPeriod(word period)
{
  /* Write period to TPM1 Modulo Register */
  TPM1MOD = period;
  /* Write Channel 4 Value Register for 25% duty-cycle */
  TPM1C4V = period>>2;
}


/*******************************************************************/
/*!
 * Stop PWM generation and turn off TPM1 clock
 */
void SounderOff(void)
{
  /* Stop PWM generation */
  TPM1SC=0x00;
  /* Turn off TPM1 clock */
  SCGC1_TPM1=0;
  /* If turned to sleep, go to stop mode */
  asm ( mov3q #4,d0; bclr.b d0,SOPT1; );
}
