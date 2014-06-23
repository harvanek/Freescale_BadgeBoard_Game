/*!
 * \file    mpr084.c
 * \brief   Simple MPR084 driver using I2C
 * \version $Revision: 1.3 $
 * \author  Anthony Huereca
 * updated on 14-Sep-2009 by Milan Brejl
 */

#include "mpr084.h"

/*********************************** Variables ***********************************/
byte SlaveID;
byte MasterTransmission;

volatile byte keyPressed;
volatile byte keyNum;
volatile byte keyNumLog[LOG_LENGTH];


/*******************************************************************/
/*!
 * MPR084 Initialization
 */
void MPR084Init(void) 
{   
  int i;

  /* Delay Loop to ensure MPR084 is powered up */
  for(i=0;i<10000;i++) {
    asm("nop");
  }

  /* Turn on I2C Clock */
  SCGC1_IIC1=1;
  
  /* Configure I2C */ 
  I2CInit();

  /* Turn on ATTN Pin */
  PTCDD_PTCDD6=1; 
  ATTN=1;

  /* Ensure MPR084 is awake */
  TOGGLE_ATTN
 
   /* Delay Loop to ensure MPR084 is powered up */
  for(i=0;i<10000;i++) {
    asm("nop");
  }
 
  /* Reset Touch Sensor */
  MPR084WriteRegister(CONFIGURATION_REGISTER,0x04);
  for(i=0;i<10000;i++) {
    asm("nop");
  }   
  
  TOGGLE_ATTN
   
  for(i=0;i<10000;i++) {
    asm("nop");
  }

  /* Initialize key logger */
  for(i=0;i< LOG_LENGTH;i++) 
  {
    keyNumLog[i]=0;
  }
  
  /* Put in Stop 1 Mode for initial Config */
  MPR084WriteRegister(CONFIGURATION_REGISTER,0x14);

  /* Set maximum number of concurrent touches allowed to 1 */
  MPR084WriteRegister(MAX_TOUCH,0x01);  
  
  /* Set Sensitivity Threshold to 0x3F */
  MPR084WriteRegister(SENSITIVITY_REGISTER1,0x3F);  
  /* Set Sensitivity Threshold to 0x3F */
  MPR084WriteRegister(SENSITIVITY_REGISTER2,0x3F);  
  /* Set Sensitivity Threshold to 0x3F */
  MPR084WriteRegister(SENSITIVITY_REGISTER3,0x3F);  
  /* Set Sensitivity Threshold to 0x3F */
  MPR084WriteRegister(SENSITIVITY_REGISTER4,0x3F);  
  /* Set Sensitivity Threshold to 0x3F */
  MPR084WriteRegister(SENSITIVITY_REGISTER5,0x3F);  
  /* Set Sensitivity Threshold to 0x3F */
  MPR084WriteRegister(SENSITIVITY_REGISTER6,0x3F);  
  /* Set Sensitivity Threshold to 0x3F */
  MPR084WriteRegister(SENSITIVITY_REGISTER7,0x3F);  
  /* Set Sensitivity Threshold to 0x3F */
  MPR084WriteRegister(SENSITIVITY_REGISTER8,0x3F);  
  
  /* Turn off Sounder */
  MPR084WriteRegister(SOUNDER_CONFIGURATION_REGISTER,0x00);
  
  /* Sound On, Auto Calibration On, Touch and Release Buffer On, Sensor On */
  MPR084WriteRegister(TOUCH_CONFIGURATION_REGISTER, 0x1D); 

  /* Set 5ms Master Clock */
  MPR084WriteRegister(MASTER_TICK_COUNTER_REGISTER,0x00);
      
  /* TASP Multiplier to 2 */
  MPR084WriteRegister(TOUCH_ACQUISITION_SAMPLE_PERIOD_REGISTER,0x01);
  
  /* No Delay for Touches */  
  MPR084WriteRegister(LOW_POWER_CONFIGURATION_REGISTER, 0x00);     

  /* Turn on IRQ and put in Run 2 Mode */  
  MPR084WriteRegister(CONFIGURATION_REGISTER,0x13);  

  /* Turn off I2C Clock */
  SCGC1_IIC1=0;
  
  /* Turn on IRQ */
  IRQSC=0x52;   
}


/*******************************************************************/
/*!
 * I2C Initialization
 * Set Baud Rate and turn on I2C
 */
void I2CInit(void) 
{
  if(!PTGD_PTGD1) /* If connected to USB and thus 12MHz bus */ 
  {
    IIC1F  = 0x14;       /* set MULT and ICR */     
  } 
  else           /* 750kHz bus */
  {
    IIC1F  = 0x00;       /* set MULT and ICR */   
  }
  IIC1C1 = 0x80;       /* enable IIC */ 
}


/*******************************************************************/
/*!
 * ProximitySensorInterruptHnadler
 * Toggles the Attention pin to wake up MPR084 for I2C communications
 * Then reads Status register, clears FIFO buffer, and put back in low power mode
 * Only executes when button is either pushed or released
 */
void MPR084InterruptHandler()
{
  byte fault; 
  byte result;
  byte i;
  
  
  /* Turn on I2C Clock and Re-init I2C */
  SCGC1_IIC1=1;
  I2CInit();  
  
  /* Put MPR084 into Run1 Mode for I2C Communication */
  TOGGLE_ATTN 

  /* Turn off interrupts */
  MPR084WriteRegister(CONFIGURATION_REGISTER,0x15);

  /* Check for fault */
  fault = u8MPR084ReadRegister(FAULT_REGISTER);
  if(fault & 0x07) 
  {   
    keyPressed=0;
  } 
  else
  {
    /* Read Sensor */
    /* Get current status of touch pad */
    result = u8MPR084ReadRegister(TOUCH_STATUS_REGISTER);
     
    /* Determine if a key is currently being touched */  
    if(result & 0xFF) 
    { 
      keyPressed = 1;
  
      /* If key is touched, figure out which one it is... */
      switch(result&0xFF) 
      {
      case 0x01:
        keyNum = 1;
        break;
      case 0x02:
        keyNum = 2;
        break; 
      case 0x04:
        keyNum = 3;
        break; 
      case 0x08:
        keyNum = 4;
        break;  
      case 0x10:
        keyNum = 5;
        break; 
      case 0x20:
        keyNum = 6;
        break;
      case 0x40:
        keyNum = 7;
        break; 
      case 0x80:
        keyNum = 8;
        break;  
      default:
        keyNum = 0;
        break;           
      }

      /* Add key to history */
      i = LOG_LENGTH-1;
      while(i>0) 
      {
        keyNumLog[i] = keyNumLog[i-1];
        i--;
      }  
      keyNumLog[0] = keyNum;
    } 
    else 
    {
      /* No key touched */
      keyPressed = 0;
    }  
  }

  /* Clear interrupt signal */   
  while(IRQSC_IRQF) 
  {
    MPR084WriteRegister(FAULT_REGISTER,0x00);
    u8MPR084ReadRegister(FIFO_REGISTER);
        
    IRQSC_IRQACK=1; 
  } 

  /* Put back in Low Power Mode */
  MPR084WriteRegister(CONFIGURATION_REGISTER,0x13);

  /* Turn off I2C Clock */ 
  SCGC1_IIC1=0;
}
   

/*******************************************************************/
/*!
 * Put the touch sensor into a low power mode. Drawback is that touches
 * take longer to register
 */
void MPR084SetLowPower() 
{
  /* Turn on I2C Clock and Re-init I2C */
  SCGC1_IIC1=1;
  I2CInit(); 

  /* Ensure MPR084 is awake */
  TOGGLE_ATTN

  /* Put in Stop 1 Mode for initial Config */
  MPR084WriteRegister(CONFIGURATION_REGISTER,0x14);

  /* Sleep longer so require a long touch to wake up */  
  MPR084WriteRegister(LOW_POWER_CONFIGURATION_REGISTER, 0x2F);     

  /* Turn on IRQ and put in Run 2 Mode */  
  MPR084WriteRegister(CONFIGURATION_REGISTER,0x13);  
  
  SCGC1_IIC1=0;
}

/*******************************************************************/
/*!
 * Put the touch sensor into normal mode. Touches respond much faster, 
 *  but draws about 400uA more current.
 */
void MPR084SetNormalPower() 
{
  /* Turn on I2C Clock and Re-init I2C */
  SCGC1_IIC1=1;
  I2CInit(); 

  /* Ensure MPR084 is awake */
  TOGGLE_ATTN

  /* Put in Stop 1 Mode for initial Config */
  MPR084WriteRegister(CONFIGURATION_REGISTER,0x14);

  /* No Delay for Touches */  
  MPR084WriteRegister(LOW_POWER_CONFIGURATION_REGISTER, 0x00);     

  /* Turn on IRQ and put in Run 2 Mode */  
  MPR084WriteRegister(CONFIGURATION_REGISTER,0x13);  

  SCGC1_IIC1=0;
}


/*******************************************************************/
/*!
 * Return value of last key pressed
 * @return Number of the last button pressed
 */
byte LastKeyNumPressed() 
{
  return keyNum;
}

/*******************************************************************/
/*!
 * Return 1 if key is currently being pressed. 0 if no key is being pressed
 */
byte KeyPressed() 
{
  return keyPressed;
}


/*******************************************************************/
/*!
 * Return True if Slide Up on Left Side occured
 */ 
byte SlideLeftUp() 
{
  return (keyNumLog[0]==1) && (keyNumLog[1]==2) && (keyNumLog[2]==3) && (keyNumLog[3]==4);
}

/*******************************************************************/
/*!
 * Return True if Slide Down on Left Side occured
 */ 
byte SlideLeftDown() 
{
  return (keyNumLog[0]==4) && (keyNumLog[1]==3) && (keyNumLog[2]==2) && (keyNumLog[3]==1);
}

/*******************************************************************/
/*!
 * Return True if Slide Up on Right Side occured
 */ 
byte SlideRightUp() 
{
  return (keyNumLog[0]==5) && (keyNumLog[1]==6) && (keyNumLog[2]==7) && (keyNumLog[3]==8);
}

/*******************************************************************/
/*!
 * Return True if Slide Down on Right Side occured
 */ 
byte SlideRightDown() 
{
  return (keyNumLog[0]==8) && (keyNumLog[1]==7) && (keyNumLog[2]==6) && (keyNumLog[3]==5);
}

/*******************************************************************/
/*!
 * Return True if key history matches the code
 * example: if(SecretCode(0x00842617)) {...} 
 */ 
byte CheckCode(long code) 
{
  byte result = 1;
  byte i = 0;
  
  while(i<8)
  {
    result &= (((code & 0x0F) == 0) || ((code & 0x0F) == (keyNumLog[i])));
    code = code>>4;
    i++;
  }
  return result;
}


/*******************************************************************/
/*!
 * Pause Routine
 */ 
void Pause(void)
{
    int n;
    for(n=1;n<50;n++) {
      asm("nop");
    }
}


/*******************************************************************/
/*!
 * Start I2C Transmision
 * @param SlaveID is the 7 bit Slave Address
 * @param Mode sets Read or Write Mode
 */
void I2CStartTransmission(byte SlaveID, byte Mode)
{
  if(Mode == MWSR)
  {      
    /* set transmission mode */
    MasterTransmission = MWSR;
  }
  else
  {
    /* set transmission mode */
    MasterTransmission = MRSW;
  }
    
  /* shift ID in right possition */
  SlaveID = (byte) MPR084_I2C_ADDRESS << 1; 
  
  /* Set R/W bit at end of Slave Address */
  SlaveID |= (byte)MasterTransmission;

  /* send start signal */
  I2CStart();

  /* send ID with W/R bit */
  I2CWriteByte(SlaveID);
}


/*******************************************************************/
/*!
 * Read a register from the MPR084
 * @param u8RegisterAddress is Register Address
 * @return Data stored in Register
 */
byte u8MPR084ReadRegister(byte u8RegisterAddress)
{
  byte result, result2;
  
  /* Set Register Pointer on MPR084 */
  I2CStartTransmission(SlaveID,MWSR);  
  I2CWait();
  
  IIC1D = u8RegisterAddress;
  I2CWait();    
  
  I2CStop();     
  
  Pause();
          
  /* Request data from Register */         
  I2CStartTransmission(SlaveID,MRSW);
  I2CWait();
   
  I2CEnterRxMode(); 
  result2 = IIC1D;
  I2CWait();   
  
  result = IIC1D;
  I2CDisableAck();
  I2CWait(); 
   
  I2CStop(); 
  result2 = IIC1D;
 
  Pause();

  return result;
}

/*******************************************************************/
/*!
 * Write a byte of Data to specified register on MPR084
 * @param u8RegisterAddress is Register Address
 * @param u8Data is Data to write
 */
void MPR084WriteRegister(byte u8RegisterAddress, byte u8Data)
{
  /* send data to slave */
  I2CStartTransmission(SlaveID,MWSR);  
  I2CWait();
  
  IIC1D = u8RegisterAddress;
  I2CWait();
  
  IIC1D = u8Data;
  I2CWait();    
  
  I2CStop();        

  Pause();
}  


