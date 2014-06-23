/*!
 * \file    ramvector.h
 * \brief   Defines and prototypes for ramvector.c
 * \version $Revision: 0.1 $
 */

#ifndef _RAMVECTOR_H_
#define _RAMVECTOR_H_

/**************************** Function Prototypes *****************************/          
void RedirectInterruptVectorsToRAM(void);

/********************* Extern Interrupt Handler Prototypes ********************/          
extern interrupt VectorNumber_Vrtc void rtc_interrupt(void);
extern interrupt VectorNumber_Virq void irq_interrupt(void);

#endif /* _RAMVECTOR_H_ */