/*!
 * \file    ledmatrix.h
 * \brief   Defines and macros for ledmatrix.c
 * \version $Revision: 1.3 $
 * \author  Anthony Huereca and Michael Norman
 * updated on 16-Sep-2009 by Milan Brejl
 */

#ifndef _LEDMATRIX_H_
#define _LEDMATRIX_H_

#include <MCF51JM128.h>

/*************************** Configuration Macros *****************************/
/*! Number of columns in the LED matrix */
#define COLUMNS 16
#define ROWS     5

/*! Maximum number of characters in the message string */
#define MAX_CHAR_NUM    (100 + 1)

/*! Size of the message buffer to contain the max message string */
#define MAX_MSG_LENGTH  (MAX_CHAR_NUM * (5 + 1)) + (2 * COLUMNS)

/* Scrolling speed */
#define SCROLL_MAX       3    /* Maximum scroll speed (min delay) */
#define SCROLL_MIN      10    /* Minimum scroll speed (max delay) */
#define SCROLL_DEFAULT   5

/************************** API Function Prototypes ***************************/          
void DisplayRows(byte rows);
void DisplayColumns(word columns);
void DisplayPoint(byte row, byte col);
void DisplayText(char *text);
void DisplayScrollingText(char *text);
void DisplayGraphics(word *graphics);
void DisplayOff(void);


#endif /* _LEDMATRIX_H_ */