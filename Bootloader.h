#include <mcf51jm128.h>

#ifdef  _MCF51JM128_H
    #define MIN_RAM1_ADDRESS        0x800000
    #define MAX_RAM1_ADDRESS        0x803FFF
    #define MIN_FLASH1_ADDRESS      0x000000
    #define MAX_FLASH1_ADDRESS      0x01FFFF
    #define FLASH_PROTECTED_ADDRESS 0x0037FF
    #define USER_ENTRY_ADDRESS      0x0039C0
    #define REDIRECT_VECTORS        0x003800
       
    #define FLASH_PAGE_SIZE     1024

    void _Entry(void) ;
#endif

// Bootloader Status
#define BootloaderReady       0
#define BootloaderS19Error    1
#define BootloaderFlashError  2
#define BootloaderSuccess     3
#define BootloaderStarted     4

  #define TRUE  1
    /*!< Definitioni for TRUE. */
  #define FALSE 0
    /*!< Definition for FALSE. */

#ifndef USB_HANDLE_H
#define USB_HANDLE_H

// USB command
#define mGET_STATUS           0
#define mCLR_FEATURE          1
#define mSET_FEATURE          3
#define mSET_ADDR             5
#define mGET_DESC             6
#define mSET_DESC             7
#define mGET_CONFIG           8
#define mSET_CFG              9
#define mGET_INTF             10
#define mSET_INTF             11
#define mSYNC_FRAME           12
#define	mGET_MAXLUN	          0xFE		// Mass Storage command


// USB ICP Vendor Commands
#define mPROGRAM			0x81
#define mSECTOR_ERASE	0x82
#define mVERIFY       0x87
#define mGET_RESULT		0x8F

// Descriptor Types
#define mDESC_DEV     0x01
#define mDESC_CFG     0x02
#define mDESC_STR     0x03

// Endpoint0 Token define
#define mEP0_OUT      0x00
#define mEP0_IN       0x08
#define mEP1_IN       0x18
#define mEP2_OUT      0x20
#define mSETUP_TOKEN  0b00001101


#define mMCU      0x00
#define mUDATA0   0x88
#define mUDATA1   0xC8

#define mEP0_BUFF_SIZE  64
#define cEP1_BUFF_SIZE  64
#define cEP2_BUFF_SIZE  64

#define gFlashSuccess  0x01
#define gFlashError    0xFF


//#define GUI_BOOTLOADER  1
#define MSD_BOOTLOADER  2


// USB state machine
enum {
  cDETACH=0,
  cATTACH,
  cPOWER,
  cDEFAULT,
  cADR_PENDING,
  cADDRESS,
  cCONFIGURE,
  cSUSPEND
};



// Control transfer state machine
enum
{
//  cUSBSetup=0,                      
//  cUSBWait,  
  cCBW,                 
  cCSW,
  cEP1Tx,
  cEP2Rx,
  mICP_WAIT,     
  mICP_TX,       
  mICP_RX       
};



typedef union _tBDT_STAT
{
    byte _byte;
    struct{
        byte  :2;                    // bit 0-1 reserved
        byte BSTALL:1;              // BDT stall'
        byte DTS:1;                 //Data Toggle Synch Enable
        byte NINC:1;                //Address Increment Disable
        byte KEEP:1;                //BD Keep Enable
        byte DATA:1;                //Data Toggle Synch Value
        byte UOWN:1;                //USB Ownership   
    }CtlBit;
    
  
    struct{
        byte    :2;
        byte PID:4;                 //Packet Identifier
        byte    :2;
        
    }RecPid;
} tBDT_STAT;                            //Buffer Descriptor Status Register

typedef struct _tBDT_ADDR
{
  byte  addr0:8;
  byte  addr1:8;
  byte  addr2:8;
  byte  addr3:8;
}tBDT_ADDR;


typedef struct _tICP_BDT
{
  tBDT_STAT BDT_Stat;
  byte      Rev;
  byte      Cnt;
  byte      Rev2;
  byte      Addr0;
  byte      Addr1;
  byte      Addr2;
  byte      Addr3;
}tICP_BDT;


typedef struct _tUSB_Setup {
       byte bmRequestType;
       byte bRequest;
       byte wValue_l;
       byte wValue_h;
       byte wIndex_l;
       byte wIndex_h;
       byte wLength_l;
       byte wLength_h;
}tUSB_Setup;

typedef struct _tICPSTR
{
  byte Byte;
  struct {
    byte AddrPend     :1;                                      
    byte none         :1;                                       
    byte Prog         :1;                                       
    byte Verify       :1;                                      
    byte none2        :1;                                       
    byte none3        :1;
    byte none4        :1;
    byte none5        :1;                                      
  } Bits;
}tICPSTR;

#endif USB_HANDLE_H


