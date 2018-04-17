/*
 * File:   speaker_selector.c
 * 
 * Author: Dilshan R Jayakody.
 * E-mail: jayakody2000lk at gmail dot com
 *
 * Created on March 30, 2018, 8:18 PM
 */

#include "speaker_selector.h"

void main(void) 
{
    // Initialize system.
    initSystem();
    __delay_ms(150);
    
    // Try to load last speaker selection from E2PROM and apply it to relays.
    speakerChannel = eeprom_read(0x00);
    if(speakerChannel > 6)
    {
        // E2PROM data is not valid and switch system to mute state.
        speakerChannel = 0;
    }
    else
    {
        PORTA = (unsigned char)(1 << (speakerChannel - ((speakerChannel == 6) ? 0 : 1)));
    }
    
    // Main service loop.
    while(1) 
    {
        setDisplay(speakerChannel);
        
        // Check for available updates and apply it to relays.
        if(updateCounter == 0xFF)
        {
            PORTA = 0x00;
            __delay_ms(500);
            PORTA = (unsigned char)(1 << (speakerChannel - ((speakerChannel == 6) ? 0 : 1)));
        }
        
        // Decrement update loop and write latest audio channel into E2PROM.
        if(updateCounter > 0)
        {
            if(((--updateCounter) == 1) && (speakerChannel > 0) && (eeprom_read(0x00) != speakerChannel))
            {
                eeprom_write(0x00, speakerChannel);
            }
        }
        
        __delay_ms(40);
    }
       
    return;
}

void interrupt interruptServiceHandler()
{
    // Handle button interrupt.
    if((INTCONbits.INT0IF) && (INTCONbits.INT0IE))
    {
        buttonHoldDownTime = 0;   
        
        // Disable external interrupts and enable timer1 to detect 
        // button states.
        INTCONbits.INT0IE = 0;
        PIR1bits.TMR1IF = 0;
        PIE1bits.TMR1IE = 1;  
        T1CONbits.TMR1ON = 1;
        
        return;
    }
    
    // Interrupt handler for timer1.
    if(PIR1bits.TMR1IF)
    {
        PIR1bits.TMR1IF = 0;
        buttonHoldDownTime = (buttonHoldDownTime > 0x0A) ? 0x10 : (buttonHoldDownTime + 1);
        
        // Check for long button press or button releases.
        if((buttonHoldDownTime > 0x0A) || (PORTBbits.RB0 == 1))
        {                                                                     
            // Long button press detected - mute all speaker lines.
            if(buttonHoldDownTime > 0x0A)
            {
                if(speakerChannel > 0)
                {
                    lastSpeakerChannel = speakerChannel;
                    speakerChannel = 0;
                    PORTA = 0x00;
                }                               
            }
            else
            {
                // Move to next audio input channel. Restore last input channel
                // if line is in mute state.
                if(speakerChannel == 0) 
                {
                    speakerChannel = lastSpeakerChannel;
                }
                else 
                {
                    speakerChannel = (unsigned char)((speakerChannel == 6) ? 1 : (speakerChannel + 1));                      
                }   
                
                // Notify line status in main service routine.
                updateCounter = 0xFF;
            }
            
            // External interrupts are not enable until user release the button.
            // This routine is to avoid button debounce and channel skips.
            if(PORTBbits.RB0)
            {
                __delay_ms(100);
                
                T1CONbits.TMR1ON = 0;
                INTCONbits.INT0IF = 0;
                INTCONbits.INT0IE = 1;
                PIE1bits.TMR1IE = 0;
                
                buttonHoldDownTime = 0;
            }            
        }                       
    }
}

void initSystem()
{
    buttonHoldDownTime = 0;
    speakerChannel = 0;
    lastSpeakerChannel = 1;
    updateCounter = 0;
    
    // Setup system registers and enable interrupts in MCU.
    OPTION_REG = 0xC7;
    OSCCON = 0x62;
    INTCON = 0xD0;
    
    // Disable unused peripherals.
    ANSEL = 0x00;
    ADCON0 = 0x00;
    ADCON1 = 0x00;
    CMCON = 0x07;
    SSPCON = 0x00;
    
    // Enable timer1 module in MCU to detect button events.
    T1CON = 0x50;
    
    // PORTB is mainly reserved for SSD unit. PORTB<0> is configured for 
    // selector button. 
    TRISB = 0x01;
    PORTB = 0x00; 
    
    TRISA = 0x00;
    PORTA = 0x00;
}

void setDisplay(unsigned char value) 
{
    switch(value)
    {
        case 0:
            PORTB = 0x80;   // Symbol "-".
            break;
        case 1:
            PORTB = 0x0C;   // Symbol "1".
            break;
        case 2:
            PORTB = 0xB6;   // Symbol "2".
            break;
        case 3:
            PORTB = 0x9E;   // Symbol "3".
            break;
        case 4:
            PORTB = 0xCC;   // Symbol "4".
            break;
        case 5:
            PORTB = 0xDA;   // Symbol "5".
            break;
        case 6:
            PORTB = 0xFA;   // Symbol "6".
            break;
    }
}
