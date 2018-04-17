/*
 * File:   speaker_selector.c
 * 
 * Author: Dilshan R Jayakody.
 * E-mail: jayakody2000lk at gmail dot com
 *
 * Created on March 30, 2018, 8:18 PM
 */

#ifndef SPEAKER_SELECTOR_H
#define	SPEAKER_SELECTOR_H

#include <xc.h>

// CONFIG1
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTRC oscillator; port I/O function on both RA6/OSC2/CLKO pin and RA7/OSC1/CLKI pin)
#pragma config WDTE = OFF   // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON   // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF  // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = ON   // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF     // Low-Voltage Programming Enable bit (RB3/PGM pin has PGM function, Low-Voltage Programming enabled)
#pragma config CPD = OFF    // Data EE Memory Code Protection bit (Code protection off)
#pragma config WRT = OFF    // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CCPMX = RB0  // CCP1 Pin Selection bit (CCP1 function on RB0)
#pragma config CP = OFF     // Flash Program Memory Code Protection bit (Code protection off)

// CONFIG2
#pragma config FCMEN = ON   // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = ON    // Internal External Switchover bit (Internal External Switchover mode enabled)

#define _XTAL_FREQ 4000000  // System clock is set to 4MHz.

// Default E2PROM configuration.
__EEPROM_DATA(0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

#ifdef	__cplusplus
extern "C" {
#endif
    
signed char buttonHoldDownTime; 

unsigned char speakerChannel, lastSpeakerChannel;
unsigned char updateCounter;

void initSystem();
void setDisplay(unsigned char value);
    
#ifdef	__cplusplus
}
#endif

#endif	/* SPEAKER_SELECTOR_H */

