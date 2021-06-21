#include <msp430x14x.h>
#define         PRZYCISK                 BIT4&P4IN         // przycisk B1

void mruganie(int licznik)
{
  unsigned int  i,j;
  for (j = 0; j < licznik ; j++) {
    P2OUT &= ~ BIT1;
    for (i = 0x1FFF; i > 0; i--) WDTCTL=WDTPW +WDTCNTCL;;
    P2OUT |= BIT1;
    
    P1OUT |= BIT5;       
    for (i = 0x1FFF; i > 0; i--) WDTCTL=WDTPW +WDTCNTCL;;
    P1OUT &= ~ BIT5;
    
    P1OUT |= BIT6;       
    for (i = 0x1FFF; i > 0; i--) WDTCTL=WDTPW +WDTCNTCL;;
    P1OUT &= ~ BIT6;
  }
}

void main( void )
{
  unsigned int i;
  WDTCTL  = WDTPW + WDTHOLD;                // wyłączenie układu WDT
  BCSCTL1 |= XTS + DIVA1 + DIVA0;           // (ACLK = LFXT1 = HF XTAL)/8 
  
  P2DIR |= BIT1;
  P1DIR |= BIT5;
  P1DIR |= BIT6;
  P1OUT &= ~ BIT5;
  P1OUT &= ~ BIT6;
  
  mruganie(2);
  
  do 
  {
    IFG1 &= ~OFIFG;                         // czyszczenie flgi OSCFault
    for (i = 0xFF; i > 0; i--);             // odczekanie 
  }
  while ((IFG1 & OFIFG));                   //  dopóki OSCFault jest ciągle ustawiona                
  WDTCTL = WDTPW + WDTCNTCL + WDTSSEL;    // tryb watchdog ACLK
  
  if(WDTIFG & IFG1)
  { 
    IFG1 &= ~WDTIFG;
    mruganie(10);
  }


  for(;;) {
    if ((PRZYCISK) == 0) {                   // przycisk B1 został wciśnięty
      for(;;);                               // pętla nieskończona - zawieszenie programu
    } else {
      WDTCTL=WDTPW +WDTCNTCL;                // zerowanie WDT      
    }
  } 
}


