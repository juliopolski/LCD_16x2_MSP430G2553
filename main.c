/* By Julio Opolski Netto
 * Conexoes (Connections)
 *  Pinos de dados (Pinout)
 *   P2.0 - D4 Pin11
 *   P2.1 - D5 Pin12
 *   P2.2 - D6 Pin13
 *   P2.3 - D7 Pin14
 *
 *  Pinos de controle
 *   P1.4 - RS Pin4
 *   P1.5 - R/W GND
 *   P1.6 - E Pin6
 */

#include <msp430.h>
#include "lcd.h"

void main(void)
{
    int tempInt = 13;
    int pressaoInt = 17;
    
    long tempLong = 273;
    long pressaoLong = 1234;
    
    WDTCTL = WDTPW + WDTHOLD;
    inicializarLcd();
    enviaString("Temp: ");
    longToLcd(tempLong, 2);
    //integerToLcd(tempInt);
    enviaComando(0xC0);
    enviaString("Pressao: ");
    longToLcd(pressaoLong, 1);
    //integerToLcd(pressaoInt);
    
    while(1){}
}
