/*!
 * \file    jmbadgeboard.c
 * \brief   Deafult low-power initialization of the JM Badge Board
 * \version $Revision: 0.1 $
 */

#include <mcf51jm128.h> /* include peripheral declarations */
#include "jmbadgeboard.h"

/*******************************************************************/
/*!
 * Initialize system, GPIO pins, clock, turn off all peripherals
 * and put accelerometer and touch sensors in low power modes
 */
void JMBadgeBoardInit(void) 
{
  /* Disables COP; Enable STOP instruction */
  SOPT1 = 0x20;
  
  /* Disable low-voltage detects */
  SPMSC1 = 0x00;
  
  /* Use Stop3 Mode */
  SPMSC2 = 0x00;   
  
  /* Enable the interupt wakeup enable */
  INTC_WCR =  0x80;     
 
  /* Enable pullups */
  PTAPE = 0xFF; 
  PTBPE = 0xFF; 
  PTCPE = 0x00;  //Don't pull up PortC
  PTDPE = 0xFF; 
  PTEPE = 0xFF; 
  PTFPE = 0xFB;  //Don't pull up PTF2
  PTGPE = 0xFF; 

  /* Initialize port data so LED Matrix is OFF by default */
  PTED  = 0xFF; /* set PORTE output data to all 1's */
  PTDD  = 0xFF; /* set PORTD output data to all 1's */
  PTAD  = 0x00; /* set PORTA output data to all 0's */
  PTEDD = 0xFF; /* set PortE to output */
  PTDDD = 0xFF; /* set PortD to output */ 
  PTADD = 0xFF; /* set PortA to output */

  /* PORTB */
  PTBD_PTBD0 = 0;  /* No Connect */
  PTBD_PTBD1 = 0;  /* No Connect */
  PTBD_PTBD2 = 0;  /* No Connect */
  PTBD_PTBD3 = 0;  /* No Connect */ 
  PTBDD=0x0F;    
  
  /* PORTC */
  PTCD_PTCD0 = 0;     /* I2C pin */
  PTCD_PTCD1 = 0;     /* I2C pin */
  IR = 1;             /* IR off */
  MMA7260_SLEEP = 0;  /* Put the accelerometer to sleep for now */
  MMA7260_GSEL1 = 0;  /* Set for 1.5G */
  MMA7260_GSEL2 = 0;  /* 00 = 1.5G, 01 = 2.0G, 10 = 4.0G, 11 = 6.0G */
  MPR083_ATTN = 1;    /* Don't request attention from MPR084 */
  PTCDD=0xFF;
  
  /* PORTF */
  LED_RED = OFF; /* Turn off Red LED */
  LED_GRN = OFF; /* Turn off Green LED */
  SOUNDER = 0;   /* Turn off Sounder */
  PTFD_PTFD3=0;  /* No Connect */ 
  PTFD_PTFD4=0;  /* No Connect */
  PTFD_PTFD5=0;  /* No Connect */
  PTFD_PTFD6=0;  /* No Connect */
  PTFD_PTFD7=0;  /* No Connect */ 
  PTFDD=0xFF;    

  /* PORTG */
  MC3467X_EN = 1; /* Disable battery charging */
  PTGD_PTGD3 = 0; /* No Connect */
  
  PTGDD_PTGDD0=0; /* CHG Signal on Charger*/
  PTGDD_PTGDD1=0; /* PPR Signal on Charger*/
  PTGDD_PTGDD2=1; /* EN Signal on Charger*/
  PTGDD_PTGDD3=1; /* No Connect */
  PTGDD_PTGDD4=0; /* Xtal */
  PTGDD_PTGDD5=0; /* extal */

 
  /* Initialize clock when running of battery power for low-power.
     Use external Crystal at 12MHz (BLPE mode) */   
  MCGC2 = 0x66;   /* BUS div by 2, Range is high, oscillator */      
  while (MCGSC_OSCINIT == 0)
    ;
  MCGC1 = 0x98;   /* CLKS = 10 -> external reference clock. */    /* RDIV = 3 -> 12MHz/8=1.5 MHz */
  MCGC2_ERCLKEN=0;
  /* wait for mode change to be done */
  while (MCGSC_IREFST != 0)
    ;
  while (MCGSC_CLKST != 2)
    ;
  MCGC2_HGO=0; 
  MCGC3_PLLS=1;
  MCGC2_LP=1;  /* Turn off FLL and PLL */
  MCGC2_BDIV=3; /* Divide clock by 8: 12Mhz/8=1.5MHz */
   
  /* If running of battery, turn off all clocks except IRQ and KBI */ 
  if (MC3467X_PPR)   
  {
    SCGC1=0x00;       
    SCGC2=0x30; 
    SCGC3=0x00;     
  } 
}
