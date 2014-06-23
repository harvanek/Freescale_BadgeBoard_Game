/*!
 * \file    main.c
 * \brief   JM Badge Board project template
 * \version Revision: 1.0
 * \date    27-Jan-2014
 * \author  Michal Harvanek
 */

/* mandatory for JM Badge Board */
#include <hidef.h> /* for EnableInterrupts macro */
#include <mcf51jm128.h> /* include peripheral declarations */
#include "ramvector.h" /* redirects interrupt vectors into RAM */
#include "jmbadgeboard.h" /* default JM Badge Board low-power initialization */
/* optional JM Badge Board drivers */
#include "rtc.h" /* Real Time Clock */
#include "ledmatrix.h" /* LED Matrix */
#include "sounder.h" /* Sounder */
#include "mpr084.h" /* Touch Pad driver */

/******************************************************************************
* Constants and macros
******************************************************************************/
#define TONE_C1     1433
/******************************************************************************
* Global variables
******************************************************************************/
byte col = 1;         //poc. stav micku
byte row = 2;         //poc. stav micku
signed char y = +1;   //musi byt signet char, jinak se pri odpalku spodni hranou, kdy se vyhonocuje jestli micek leti se shora nebo z dola (-y nebo +y) delaly chyby
signed char x = -1;   //pro jistotu taky char...
byte bat1 = 3;        //pozice palky od 0 do 5 => byte
byte bat2 = 2;        //pozice palky od 0 do 5 => byte
byte player1pts = 0;  //pocitadlo bodu
byte player2pts = 0;  //pocitadlo bodu
byte stopflag1=0;     //signalizace, ktery hrac dal gol 
byte stopflag2=0;     //signalizace, ktery hrac dal gol
char message1[] = ">>Player 1 win!<<";
char message2[] = ">>Player 2 win!<<";
/******************************************************************************
* Functions
******************************************************************************/

/******************************************************************************
* Interrupt Handlers
******************************************************************************/
/* RTC Interrupt Handler */
interrupt VectorNumber_Vrtc void rtc_interrupt(void)
{
  /* Clear the interrupt flag */
  RTCClearInterrupt();

{
  if(row==0){   //odraz micku od horni vodorovne hrany
     y=+1;
  }
  if(row==4){   //odraz micku od spodni vodorovne hrany
     y=-1;
  }
 
  if(col==1){                                      // pro prvni palku=bat1
     if((-1+bat1==row)||(bat1==row)){              //kdyz je micek odpalen rovnou hraci plochou
        x=+1;
        SounderOn(TONE_C1);
     }
     if(((row==0)&&(bat1==2))||((row==4)&&(bat1==3))){      //kdyz je micek odpalen hranou palky a zaroven byl v te chvili odrazen od spodni nebo horno vodorovne hrany(bylo nutno osetrit zvlast)
        x=+1;
        SounderOn(TONE_C1);
     }else{                                      // v else jsou ostatni pripady odpalu hranou palky
      
        if((((bat1-1)-row)==1)&&(y==+1)){      //osetreni odpalu horni hranou palky, jde-li micek zhora na horni hranu odpaleno(stejnym smerem kterym prisel - jako ve skutecnosti), jde-li ze spodu na horni hranu promachnuti
            x=+1;
            y=-1;
            SounderOn(TONE_C1);
        }
     
     
        if(((row-bat1)==1)&&(y==-1)){          //osetreni odpalu spodni hranou palky, jde-li micek z dola na spodni hranu odpaleno(stejnym smerem kterym prisel - jako ve skutecnosti), jde-li z hora na spodni hranu promachnuti
            x=+1;
            y=+1;
            SounderOn(TONE_C1);
        }
     }
  }
  
  if(col==14){                              // viz podminky nahore ale pro druhou palku=bat2
     if((-1+bat2==row)||(bat2==row)){
        x=-1;
        SounderOn(TONE_C1);                  
     }
     if(((row==0)&&(bat2==2))||((row==4)&&(bat2==3))){
        x=-1;
        SounderOn(TONE_C1);
     }else{ 
        if((((bat2-1)-row)==1)&&(y==+1)){      
            x=-1;
            y=-1;
            SounderOn(TONE_C1);
        }
        if(((row-bat2)==1)&&(y==-1)){
            x=-1;
            y=+1;
            SounderOn(TONE_C1);
        }
     }
  }                    
  
  if((col==2)||(col==13)){
     SounderOff();        //vypnuti tonu po odrazu micku od palky
  }             
  
  row=row+y;              //vypocteni nove pozice micku
  col=col+x;              //vypocteni nove pozice micku
  
  if(col==0){
     //LED_GRN = ON;
     SounderOff();
     player2pts = player2pts+1;    //pricteni bodu hraci 2 dostal-li hrac 1 gol
     x=+1;                         //nastaveni pocatecnich podminek pro micek po dostani golu
     y=+1;                         //nastaveni pocatecnich podminek pro micek po dostani golu
     bat1=2;                       //nastaveni pocatecnich podminek pro micek po dostani golu
     row=bat1;                     //nastaveni pocatecnich podminek pro micek po dostani golu
     col=1;                        //nastaveni aby se zastavil RTC a spustil az pri odpalu micku talcitkem odpalu u hrace dva = E8 u hrace 1 E4
     stopflag1=1;
     RTCStop();
     
  }
  if(col==15){                       // to same pro druheho hrace
     //LED_RED = ON;
     SounderOff();
     player1pts = player1pts+1;       
     x=-1;
     y=-1;
     bat2=2;
     row=bat2;
     col=14;
     stopflag2=1;
     RTCStop();
  }

  /* Clear the interrupt flag */
  RTCClearInterrupt();
}   

}/*end of RTC INTERRUPT*/

/* IRQ Interrupt Handler - Touch Pad */
interrupt VectorNumber_Virq void irq_interrupt(void) 
{
  /* On MPR084 interrupt */
  MPR084InterruptHandler();

if(KeyPressed())              //pri stisku se posune pouze o 1 pozici
  {  
switch (LastKeyNumPressed())  //testuje stisk prislusneho tlacitka
  {
  case 1:                      //pohyb palky 1 nahoru
    if(bat1>0) {
      bat1=bat1-1;
      if(stopflag1==1){        
        row=bat1;
        if(bat1==0){           //osetreni podminky sjede-li pod spodni vodorovnou hranu
          row=0;
        }
      }
    }
    break;
  case 2:
    if(bat1<5) {
    bat1=bat1+1;
      if(stopflag1==1){
        row=bat1;
        if(bat1==5){
          row=4;
        }
      }
    }
    break;
  case 4://odpal po promachnuti
    if(stopflag1==1){
      RTCStart(150);            //jestlize hrac 1 dostal gol tak rozehrava
      stopflag1=0;
    }
    break;
  
  case 5:
    if(bat2>0) {
      bat2=bat2-1;
      if(stopflag2==1){
        row=bat2;
        if(bat2==0){
          row=0;
        }
      }
    }
    break;
  case 6:
    if(bat2<5) {  
    bat2=bat2+1;
    if(stopflag2==1){
        row=bat2;
        if(bat2==5){
          row=4;
        }
      }
    }
    break;
  case 8://to smamy jako pro 4
    if(stopflag2==1) {
      RTCStart(150);
      stopflag2=0;
    }
    break;

  default:
    break;
  }
  }
}        

/******************************************************************************
* Main
******************************************************************************/
void main(void) 
{
  /* This needs to be here when running in bootloader framework */
  RedirectInterruptVectorsToRAM();

  /******************* Device and Board Initialization ***********************/
  /* Deafult low-power initialization of the JM Badge Board */
  JMBadgeBoardInit();  

  /* your initialization code here */

  /* Start 150ms periodic interrupts */
  RTCStart(150);
  MPR084Init();
  

  EnableInterrupts; 

  /*********************** Neverending background loop ***********************/
  while(1) 
  {
   if(player1pts==3){                      //HRA SE HRAJE NA TRI VYTEZNE BODY KDO JE DA DRIV VYHRAL!
      RTCStop();
      DisplayScrollingText(message1);
   } else if(player2pts==3){
      RTCStop();
      DisplayScrollingText(message2);
   } else {
    
  /* Display the ball */                         //Zobrazeni bodu - jelikoz je potreba body zobrazovat neustale nemuze byr pouzito uspani procesoru:(
   DisplayPoint(row, col);
   /* Display the bat1 */
   DisplayPoint(-1+bat1, 0);
   DisplayPoint(bat1, 0);
   /* Display the bat2 */
   DisplayPoint(-1+bat2, 15);
   DisplayPoint(bat2, 15);
   }
  }
  /* please make sure that you never leave main */
}
