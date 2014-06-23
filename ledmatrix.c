/*!
 * \file    ledmatrix.c
 * \brief   Driver code for LED Matrix
 * \version $Revision: 1.3 $
 * \author  Anthony Huereca and Michael Norman
 *          Based off code from Defcon 15 Badge
 *          http://www.grandideastudio.com/portfolio/defcon-15-badge
 * reworked on 16-Sep-2009 by Milan Brejl 
 */

#include "ledmatrix.h"

/*******************************************************************/
/* ASCII Font Table for 5x5 Character Set */
const unsigned char FontTable [][5] = 
{ 
  { 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 }, // Space
  { 0b00000, 0b00000, 0b10111, 0b00000, 0b00000 }, // !
  { 0b00100, 0b00011, 0b00100, 0b00011, 0b00000 }, // "
  { 0b01010, 0b11111, 0b01010, 0b11111, 0b01010 }, // #
  { 0b10010, 0b10101, 0b11111, 0b10101, 0b01001 }, // $
  { 0b10011, 0b01011, 0b00100, 0b11010, 0b11001 }, // %
  { 0b01010, 0b10101, 0b10111, 0b01000, 0b11000 }, // &
  { 0b00000, 0b00100, 0b00011, 0b00000, 0b00000 }, // '
  { 0b00000, 0b01110, 0b10001, 0b00000, 0b00000 }, // (
  { 0b00000, 0b10001, 0b01110, 0b00000, 0b00000 }, // )
  { 0b01010, 0b00100, 0b11111, 0b00100, 0b01010 }, // *
  { 0b00000, 0b00100, 0b01110, 0b00100, 0b00000 }, // +
  { 0b00000, 0b10000, 0b01000, 0b00000, 0b00000 }, // ,
  { 0b00000, 0b00100, 0b00100, 0b00100, 0b00000 }, // -
  { 0b00000, 0b00000, 0b10000, 0b00000, 0b00000 }, // .
  { 0b10000, 0b01000, 0b00100, 0b00010, 0b00001 }, // /
  { 0b01110, 0b11001, 0b10101, 0b01110, 0b00000 }, // 0
  { 0b00000, 0b10010, 0b11111, 0b10000, 0b00000 }, // 1
  { 0b11001, 0b10101, 0b10101, 0b10010, 0b00000 }, // 2
  { 0b10001, 0b10101, 0b10101, 0b01010, 0b00000 }, // 3
  { 0b01100, 0b01010, 0b11111, 0b01000, 0b00000 }, // 4
  { 0b10011, 0b10101, 0b10101, 0b01001, 0b00000 }, // 5
  { 0b01110, 0b10101, 0b10101, 0b01000, 0b00000 }, // 6
  { 0b00001, 0b11001, 0b00101, 0b00011, 0b00000 }, // 7
  { 0b01010, 0b10101, 0b10101, 0b01010, 0b00000 }, // 8
  { 0b00010, 0b10101, 0b10101, 0b11110, 0b00000 }, // 9
  { 0b00000, 0b00000, 0b01010, 0b00000, 0b00000 }, // :
  { 0b00000, 0b10000, 0b01010, 0b00000, 0b00000 }, // ;
  { 0b00000, 0b00100, 0b01010, 0b10001, 0b00000 }, // <
  { 0b00000, 0b01010, 0b01010, 0b01010, 0b00000 }, // =
  { 0b00000, 0b10001, 0b01010, 0b00100, 0b00000 }, // >
  { 0b00010, 0b00001, 0b10101, 0b00101, 0b00010 }, // ?
  { 0b01110, 0b10001, 0b10101, 0b10101, 0b10110 }, // @
  { 0b11110, 0b00101, 0b00101, 0b11110, 0b00000 }, // A
  { 0b11111, 0b10101, 0b10101, 0b01010, 0b00000 }, // B
  { 0b01110, 0b10001, 0b10001, 0b10001, 0b00000 }, // C
  { 0b11111, 0b10001, 0b10001, 0b01110, 0b00000 }, // D
  { 0b11111, 0b10101, 0b10101, 0b10001, 0b00000 }, // E
  { 0b11111, 0b00101, 0b00101, 0b00001, 0b00000 }, // F
  { 0b01110, 0b10001, 0b10101, 0b11100, 0b00000 }, // G
  { 0b11111, 0b00100, 0b00100, 0b11111, 0b00000 }, // H
  { 0b00000, 0b10001, 0b11111, 0b10001, 0b00000 }, // I
  { 0b01000, 0b10001, 0b01111, 0b00001, 0b00000 }, // J
  { 0b11111, 0b00100, 0b01010, 0b10001, 0b00000 }, // K
  { 0b11111, 0b10000, 0b10000, 0b10000, 0b00000 }, // L
  { 0b11111, 0b00010, 0b00100, 0b00010, 0b11111 }, // M
  { 0b11111, 0b00010, 0b00100, 0b11111, 0b00000 }, // N
  { 0b01110, 0b10001, 0b10001, 0b01110, 0b00000 }, // O
  { 0b11111, 0b00101, 0b00101, 0b00010, 0b00000 }, // P
  { 0b01110, 0b10001, 0b01001, 0b10110, 0b00000 }, // Q
  { 0b11111, 0b00101, 0b01101, 0b10010, 0b00000 }, // R
  { 0b10010, 0b10101, 0b10101, 0b01001, 0b00000 }, // S
  { 0b00001, 0b00001, 0b11111, 0b00001, 0b00001 }, // T
  { 0b01111, 0b10000, 0b10000, 0b01111, 0b00000 }, // U
  { 0b00011, 0b01100, 0b10000, 0b01100, 0b00011 }, // V
  { 0b11111, 0b01000, 0b00100, 0b01000, 0b11111 }, // W
  { 0b10001, 0b01010, 0b00100, 0b01010, 0b10001 }, // X
  { 0b00001, 0b00010, 0b11100, 0b00010, 0b00001 }, // Y
  { 0b10001, 0b11001, 0b10101, 0b10011, 0b10001 }, // Z
  { 0b00000, 0b11111, 0b10001, 0b10001, 0b00000 }, // [
  { 0b00001, 0b00010, 0b00100, 0b01000, 0b10000 }, // (
  { 0b00000, 0b10001, 0b10001, 0b11111, 0b00000 }, // ]
  { 0b00000, 0b00010, 0b00001, 0b00010, 0b00000 }, // ^
  { 0b10000, 0b10000, 0b10000, 0b10000, 0b10000 }, // _
  { 0b00000, 0b00011, 0b00100, 0b00000, 0b00000 }, // '
  { 0b01000, 0b10101, 0b10101, 0b10101, 0b11110 }, // a
  { 0b11111, 0b10100, 0b10100, 0b10100, 0b01000 }, // b
  { 0b01100, 0b10010, 0b10010, 0b10010, 0b00000 }, // c
  { 0b01000, 0b10100, 0b10100, 0b10100, 0b11111 }, // d
  { 0b01110, 0b10101, 0b10101, 0b10101, 0b10110 }, // e
  { 0b00100, 0b11110, 0b00101, 0b00001, 0b00010 }, // f
  { 0b10010, 0b10101, 0b10101, 0b10101, 0b01110 }, // g
  { 0b11111, 0b00100, 0b00100, 0b11000, 0b00000 }, // h
  { 0b00000, 0b00000, 0b11101, 0b00000, 0b00000 }, // i
  { 0b00000, 0b10000, 0b01101, 0b00000, 0b00000 }, // j
  { 0b11111, 0b00100, 0b01010, 0b10001, 0b00000 }, // k
  { 0b00000, 0b10001, 0b11111, 0b10000, 0b00000 }, // l
  { 0b11110, 0b00001, 0b00110, 0b00001, 0b11110 }, // m
  { 0b11111, 0b00001, 0b00001, 0b00001, 0b11110 }, // n
  { 0b01110, 0b10001, 0b10001, 0b01110, 0b00000 }, // o
  { 0b11111, 0b00101, 0b00101, 0b00010, 0b00000 }, // p
  { 0b00010, 0b00101, 0b00101, 0b11111, 0b10000 }, // q
  { 0b11111, 0b00010, 0b00001, 0b00001, 0b00010 }, // r
  { 0b10010, 0b10101, 0b10101, 0b01001, 0b00000 }, // s
  { 0b00010, 0b01111, 0b10010, 0b10010, 0b00000 }, // t
  { 0b01111, 0b10000, 0b10000, 0b01111, 0b00000 }, // u
  { 0b00011, 0b01100, 0b10000, 0b01100, 0b00011 }, // v
  { 0b01111, 0b10000, 0b01100, 0b10000, 0b01111 }, // w
  { 0b10001, 0b01010, 0b00100, 0b01010, 0b10001 }, // x
  { 0b10011, 0b10100, 0b10100, 0b01111, 0b00000 }, // y
  { 0b10010, 0b11010, 0b10110, 0b10010, 0b00000 }, // z
  { 0b00110, 0b01111, 0b11110, 0b01111, 0b00110 }, // {
  { 0b10010, 0b01000, 0b00111, 0b01000, 0b10010 }, // |
  { 0b00100, 0b01110, 0b10101, 0b00100, 0b00100 }, // }
  { 0b11111, 0b11111, 0b11111, 0b11111, 0b11111 }, // 
};

/*******************************************************************/


/*******************************************************************/
/*!
 * Display Rows
 */
void DisplayRows(byte rows)
{
  PTAD = rows;
  PTED = 0x00;
  PTDD = 0x00;
}

/*******************************************************************/
/*!
 * Display Columns
 */
void DisplayColumns(word columns)
{
  PTED = ~(byte)(columns);
  PTDD = ~(byte)(columns >> 8);
  PTAD = 0xFF;
}


/*******************************************************************/
/*!
 * Display a single point
 */
void DisplayPoint(byte row, byte col)
{
  PTAD = 0x00;
  PTDD = 0xFF;
  PTED = 0xFF;

  if(col < 8)
  {
    PTED = ~(1 << col);
  }
  else if(col < COLUMNS)
  {
    PTDD = ~(1 << (col - 8));
  }

  if(row <  ROWS)
  {
    PTAD = 1 << row;
  }
}


/*******************************************************************/
/*!
 * Display Text
 */
void DisplayText(char *text)
{
  static char *lastText;
  static byte fb[MAX_MSG_LENGTH];
  static int messageLength, firstCol, lastCol;
  int c, i, j;
   
  /* If new text */
  if(text != lastText)
  {
    lastText = text;
    
    /* Set up bit array of the text */
    i = 0;
    /* put characters in */
    while(*text)
    {
      /* convert non-printable character to a space */
      if ((*text < 0x20) || (*text > 0x7E))
      {
        *text = 32;
      }
      /* draw character to the bit array */
      for (j = 0; j < 5; j++)
      {
        fb[i++] = FontTable[*text - 32][j];
      }
      /* make a space narrow */
      if (*text == 32)
      {
        i--; 
      }
      text++;
    }
    messageLength = i;

    /* center or left align text */
    if(messageLength < COLUMNS)
    {
      firstCol = (COLUMNS - messageLength)/2;
      lastCol = firstCol + messageLength;
    }
    else
    {
      firstCol = 0;
      lastCol = COLUMNS;
    }
  }

  /* Display Message */
  /* Send out Characters */
  i = 0;
  for (c = firstCol; c < lastCol; c++, i++) 
  {
    if (c < 8) 
    {
      PTAD = 0x00;
      PTED = ~(1 << c);
      PTDD = 0xFF;
      PTAD = fb[i];
    }
    else
    {
      PTAD = 0x00;
      PTDD = ~(1 << (c - 8));
      PTED = 0xFF;
      PTAD = fb[i];
    }
    /* keep it lighting for a while */
    for(j=0;j<100;j++)
      asm("nop");
  }
  /* turn all rows off */
  PTAD = 0x00;
}


/*******************************************************************/
/*!
 * Display Scrolling Text
 */
void DisplayScrollingText(char *text)
{
  static char *lastText;
  static byte fb[MAX_MSG_LENGTH];
  static int i, scrollDelay, messageLength;
  int c, j;
   
  /* If new text */
  if(text != lastText)
  {
    lastText = text;
    
    /* Set up bit array of the text */
    /* put a space to the beginning */
    for (i = 0; i < COLUMNS; i++)
      fb[i] = 0x00; 
    /* add characters */
    while(*text)
    {
      /* convert non-printable character to a space */
      if ((*text < 0x20) || (*text > 0x7E))
      {
        *text = 32;
      }
      /* draw character to the bit array */
      for (j = 0; j < 5; j++)
      {
        fb[i++] = FontTable[*text - 32][j];
      }
      /* add a blank line between characters */
      if (*text != 32)
      {
        fb[i++] = 0x00;
      }
      else
      {
        i--; /* make a space narrow */
      }
      text++;
    }
    /* add a space to the end */
    for (j = 0; j < COLUMNS; j++, i++)
    {
      fb[i] = 0x00; 
    }

    messageLength = i;
    i = 0;
    scrollDelay = SCROLL_DEFAULT;
  }
  
  /* Display Message */
  /* Send out Characters */
  for (c = 0; c < COLUMNS; c++) 
  {
    if (c < 8) 
    {
      PTAD = 0x00;
      PTED = ~(1 << c);
      PTDD = 0xFF;
      PTAD = fb[i+c];
    }
    else
    {
      PTAD = 0x00;
      PTDD = ~(1 << (c - 8));
      PTED = 0xFF;
      PTAD = fb[i+c];
    }
    /* keep it lighting for a while */
    for(j=0;j<100;j++)
      asm("nop");
  }
  /* update counters */
  if(--scrollDelay == 0)
  {
    scrollDelay = SCROLL_DEFAULT;
    i++;
    if((i + COLUMNS) == messageLength)
    {
      i = 0;  /* restart scrolling */
    }
  }
  /* turn all rows off */
  PTAD = 0x00;
}


/*******************************************************************/
/*!
 * Display graphics
 *  - displays a matrix of 5x16 bits
 *  - the matrix should be declared as "word graphics[ROWS];"
 *  - to be used in a loop
 */
void DisplayGraphics(word *graphics)
{
  int r,j;
  word row, rev;
  
  for (r = 0; r < ROWS; r++) 
  {
    /* reverse graphics row */
    row = graphics[r];
    rev = 0;
    for(j = 0; j < 16; j++)
    {
      rev <<= 1;
      rev  |= row & 1;
      row >>= 1;
    }
    /* turn off previous row */
    PTDD = 0xFF;
    PTED = 0xFF;
    /* select row */
    PTAD = (1 << r);
    /* turn on row graphics */
    PTDD = ~(byte)(rev>>8);
    PTED = ~(byte)(rev);
    /* keep it lighting for a while */
    for(j=0;j<100;j++)
      asm("nop");
  }
  /* turn all rows off */
  PTAD = 0x00;
}


/*******************************************************************/
/*!
 * Display Off
 */
void DisplayOff(void)
{
  PTAD = 0x00;
  PTDD = 0xFF;
  PTED = 0xFF;
}