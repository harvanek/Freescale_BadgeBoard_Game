/*!
 * \file    MPR084.h
 * \brief   Defines and macros for MPR084 Chip and I2C
 * \version $Revision: 1.3 $
 * \author  Anthony Huereca
 * updated on 14-Sep-2009 by Milan Brejl
 */

#ifndef _MPR084_H_
#define _MPR084_H_

#include <MCF51JM128.h>


/*************************** Configuration Macros *****************************/
#define MPR084_I2C_ADDRESS                         0x5D
#define LOG_LENGTH                                 8


/************************** API Function Prototypes ***************************/          
void MPR084Init(void);
void MPR084InterruptHandler(void);
byte LastKeyNumPressed(void);
byte KeyPressed(void);
byte SlideLeftUp(void);
byte SlideLeftDown(void);
byte SlideRightUp(void);
byte SlideRightDown(void);
byte CheckCode(long code);
void MPR084SetLowPower(void);
void MPR084SetNormalPower(void);


/************************* Local Function Prototypes **************************/          
void I2CInit(void);
void I2CStartTransmission(byte SlaveID, byte Mode);
void MPR084WriteRegister(byte u8RegisterAddress, byte u8Data);
byte u8MPR084ReadRegister(byte u8RegisterAddress);
void Pause(void);


/********************************** Macros ************************************/
/* MPR084 registers */
#define FIFO_REGISTER                              0x00
#define FAULT_REGISTER                             0x01
#define TOUCH_STATUS_REGISTER                     0x02
#define TOUCH_CONFIGURATION_REGISTER              0x03
#define SENSITIVITY_REGISTER1                      0x04
#define SENSITIVITY_REGISTER2                      0x05
#define SENSITIVITY_REGISTER3                      0x06
#define SENSITIVITY_REGISTER4                      0x07
#define SENSITIVITY_REGISTER5                      0x08
#define SENSITIVITY_REGISTER6                      0x09
#define SENSITIVITY_REGISTER7                      0x0A
#define SENSITIVITY_REGISTER8                      0x0B
#define ELECTRODE_CHANNEL                          0x0C
#define MAX_TOUCH                                  0x0D
#define MASTER_TICK_COUNTER_REGISTER               0x0E
#define TOUCH_ACQUISITION_SAMPLE_PERIOD_REGISTER   0x0F
#define SOUNDER_CONFIGURATION_REGISTER             0x10
#define LOW_POWER_CONFIGURATION_REGISTER           0x11
#define STUCK_KEY_TIMEOUT_REGISTER                 0x12
#define CONFIGURATION_REGISTER                     0x13
#define SENSOR_INFORMATION_REGISTER                0x14

/* ATTN */
#define ATTN PTCD_PTCD6 /* ATTN Pin */
#define TOGGLE_ATTN ATTN=1;Pause();ATTN=0;Pause();ATTN=1; /* ATTN pin toggle */

/* I2C Macros */
#define I2CDisableAck()        IIC1C1_TXAK = 1  
#define I2CRepeatedStart()     IIC1C1     |= 0x04;
#define I2CStart()             IIC1C1     |= 0x10;\
                               IIC1C1_MST  = 1
#define I2CStop()              IIC1C1_MST  = 0;\
                               IIC1C1_TX   = 0
#define I2CEnterRxMode()       IIC1C1_TX   = 0;\
                               IIC1C1_TXAK = 0 
#define I2CWait()              while(!IIC1S_IICIF) {} \
                                 IIC1S_IICIF = 1;  
#define I2CWriteByte(data)     IIC1D = data

#define MWSR                   0x00  /* Master write  */
#define MRSW                   0x01  /* Master read */


/*******************************************************************/

#endif /* _MPR084_H_ */