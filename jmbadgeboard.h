/*!
 * \file    jmbadgeboard.h
 * \brief   JM Badge Board pin assignment and function prototypes
 *          for jmbadgeboard.c
 * \version $Revision: 0.1 $
 */

#ifndef _JMBADGEBOARD_H_
#define _JMBADGEBOARD_H_

/**************************** Function Prototypes *****************************/          
void JMBadgeBoardInit(void);


/************************** JM Badge Board Macros *****************************/
/* MC3467x Interface Pins */
#define MC3467X_CHG     PTGD_PTGD0  /*!< MC3467x Charge indicator */
#define MC3467X_PPR     PTGD_PTGD1  /*!< MC3467x Power Present indicator */
#define MC3467X_EN      PTGD_PTGD2  /*!< MC3467x Enable */

/* MMA7260 Interface Pins */
#define MMA7260_SLEEP   PTCD_PTCD3  /*!< MMA7260 Sleep Mode control */
#define MMA7260_GSEL1   PTCD_PTCD4  /*!< MMA7260 G Select 1 */
#define MMA7260_GSEL2   PTCD_PTCD5  /*!< MMA7260 G Select 2 */

/* MPR083 Interface Pins */
#define MPR083_ATTN     PTCD_PTCD6  /*!< MPR083 Attention control */

/* Red/Green LED Pins */
#define LED_RED         PTFD_PTFD0  /*!< Red portion of R/G LED */
#define LED_GRN         PTFD_PTFD1  /*!< Green portion of R/G LED */

/* Misc Pins */
#define SOUNDER         PTFD_PTFD2  /*!< Sounder control pin */
#define IR              PTCD_PTCD2  /*!< IR control pin */

/* ON/OFF State Indicators */
#define ON      0   /*! Active-low ON indicator */
#define OFF     1   /*! Active-low OFF indicator */


#endif /* _JMBADGEBOARD_H_ */