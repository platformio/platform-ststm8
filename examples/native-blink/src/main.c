/**********************
  Simple blink project w/o interrupts
   
  supported hardware:
    - STM8S Discovery board (https://www.st.com/en/evaluation-tools/stm8s-discovery.html)
    - NUCLEO-8S207K8 (https://www.st.com/en/evaluation-tools/nucleo-8s207k8.html)
    - NUCLEO-8S208RB (https://www.st.com/en/evaluation-tools/nucleo-8s208rb.html)
    - STM8L Discovery board (https://www.st.com/en/evaluation-tools/stm8l-discovery.html)
    - STM8-SO8 Discovery board (https://www.st.com/en/evaluation-tools/stm8-so8-disco.html)
    - Sduino Uno (https://github.com/roybaer/sduino_uno)
    - muBoard (http://www.cream-tea.de/presentations/160305_PiAndMore.pdf)
  
  Functionality:
    - blink LED w/o ISR. Mainly for testing toolchain
    - pass port structure to function
**********************/

/*----------------------------------------------------------
    SELECT BOARD
----------------------------------------------------------*/
//Not needed for PlatformIO since the board definition file
//automatically activates macros by which we can identify it,
//like STM8S_NUCLEO_207K8 etc.

//#define STM8S_DISCOVERY
//#define NUCLEO_8S207K8
//#define NUCLEO_8S208RB
//#define STM8L_DISCOVERY
//#define STM8_SO8_DISCO_STM8S001
//#define SDUINO
//#define MUBOARD

/*----------------------------------------------------------
    INCLUDE FILES
----------------------------------------------------------*/
#if defined(STM8S_DISCOVERY)
  #include "STM8S105C6.h"
  #define LED_PORT   sfr_PORTD
  #define LED_PIN    PIN0
  //print this out as an info during compilation
  #warning "[INFO] USING STM8S_DISCOVERY PIN MAP"
#elif defined(STM8S_NUCLEO_207K8)
  #include "STM8S207K8.h"
  #define LED_PORT   sfr_PORTC
  #define LED_PIN    PIN5
  //print this out as an info during compilation
  #warning "[INFO] USING NUCLEO_8S207K8 PIN MAP PORTC PIN5"
#elif defined(STM8S_NUCLEO_208RB)
  #include "STM8S208RB.h"
  #define LED_PORT   sfr_PORTC
  #define LED_PIN    PIN5
  //print this out as an info during compilation
  #warning "[INFO] USING NUCLEO_8S208RB PIN MAP PORTC PIN5"
#elif defined(STM8L_DISCOVERY)
  #include "STM8L152C6.h"
  #define LED_PORT   sfr_PORTC
  #define LED_PIN    PIN7
#elif defined(STM8_SO8_DISCO_STM8S001)
  #include "STM8S001J3.h"
  #define LED_PORT   sfr_PORTA
  #define LED_PIN    PIN3
#elif defined(SDUINO)
  #include "STM8S105K6.h"
  #define LED_PORT   sfr_PORTC
  #define LED_PIN    PIN5
#elif defined(MUBOARD)
  #include "STM8S207MB.h"
  #define LED_PORT   sfr_PORTH
  #define LED_PIN    PIN2
#else
  #include "STM8S103F3.h"
  #define LED_PORT   sfr_PORTB
  #define LED_PIN    PIN5
  #warning "[INFO] USING STM8S103F3 PINMAP PORTB, PIN 5"
#endif

// toggle specified pin. Pass port struct as pointer
void toggle_pin(PORT_t *port, uint8_t pin) {
  
  port->ODR.byte ^= pin;
  
} // toggle_pin

/////////////////
//    main routine
/////////////////
void main (void) {

  uint32_t  i = 0;
    
  // switch to 16MHz (default is 2MHz)
  sfr_CLK.CKDIVR.byte = 0x00;
    
  // configure LED pin as output
  LED_PORT.DDR.byte = LED_PIN;    // input(=0) or output(=1)
  LED_PORT.CR1.byte = LED_PIN;    // input: 0=float, 1=pull-up; output: 0=open-drain, 1=push-pull
  LED_PORT.CR2.byte = LED_PIN;    // input: 0=no exint, 1=exint; output: 0=2MHz slope, 1=10MHz slope
  
  // main loop
  while(1) {
	
    // toggle LED. Pass port struct as pointer
    toggle_pin(&LED_PORT, LED_PIN);
    
    // simple wait ~500ms @ 16MHz
    for (i=0; i<300000L; i++)
      NOP();
	  
  } // main loop

} // main

/*-----------------------------------------------------------------------------
    END OF MODULE
-----------------------------------------------------------------------------*/
