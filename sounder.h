/*!
 * \file    sounder.h
 * \brief   Defines and macros for sounder.h
 * \version Revision: 0.1
 * \author  Milan Brejl
 */

#ifndef _SOUNDER_H_
#define _SOUNDER_H_

/************************** API Function Prototypes ***************************/          
void SounderOn(word period);
void SounderPeriod(word period);
void SounderOff(void);

#endif /* _SOUNDER_H_ */