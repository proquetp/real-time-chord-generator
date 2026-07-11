/* 
 * @file:    pwm.c
 * 
 * @author:  MAC,PFF,PRP
 * 
 * @version: 1.0.0
 *
 * @date    10 de noviembre de 2025
 * 
 * @brief:  Driver para crear cuatro señales pwm a frecuencias independientes 
 *          para cada pin de salida; tres correpondiendo a los tres altavoces 
 *          y otra para el led
 */

#include <xc.h>
#include "pwm.h"
#include "config.h"

#define NUM_PINES_PWM_B 3
#define pin_menor 7
#define pin_led 12

//-----------------------------------------------------------------------------
//------------------VARIABLES GLOBALES AL MÓDULO-------------------------------
//-----------------------------------------------------------------------------

static unsigned int periodo_B[] = {0,0,0,0};
static unsigned int dc_B[] = {0,0,0,0};
static unsigned int ticks_B[] = {0,0,0,0};

//-----------------------------------------------------------------------------
//------------------FUNCIONES PRIVADAS-----------------------------------------
//-----------------------------------------------------------------------------

/**
 * Rutina de interrupción del Timer2
 */
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);

//-----------------------------------------------------------------------------
//------------------FUNCIONES PÚBLICAS-----------------------------------------
//-----------------------------------------------------------------------------

void InicializarPWM(void) {
    TRISB &= ~(7 << pin_menor);
    PORTB &= ~(7 << pin_menor); // fija los puertos 7 a 9 a nivel bajo
    TRISB &= ~(1 << pin_led);
    PORTB |= 1 << pin_led; 
    T2CON=0;
    PR2=FCY/100000; // centésimas de milisegundo
    TMR2=0;
    IFS0bits.T2IF=0;
    IEC0bits.T2IE=1;
    IPC1bits.T2IP=5;
    T2CON |= 1<<15;
}

void periodoPWM_B(unsigned char pin, unsigned int cms){
    if (pin > NUM_PINES_PWM_B)
        return;
    periodo_B[pin]=cms;
    dc_B[pin]=cms/2;
}


void dcPWM_B(unsigned char pin, unsigned int cms){
    if (pin > NUM_PINES_PWM_B)
        return;
    dc_B[pin]=cms;
}

//-----------------------------------------------------------------------------
//------------------FUNCIONES PRIVADAS-----------------------------------------
//-----------------------------------------------------------------------------

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    IFS0bits.T2IF=0;
    unsigned char i;
    for (i=0; i < NUM_PINES_PWM_B; i++) {
        if (periodo_B[i]>0) {
            if (ticks_B[i]<dc_B[i])
                PORTB |= 1 << (i+pin_menor);
            else
                PORTB &= ~(1 << (i+pin_menor));
            ticks_B[i]++;
            if (ticks_B[i] >= periodo_B[i])
                ticks_B[i]=0;
        }
    }
    if (periodo_B[3]>0) {
        if (ticks_B[3]<dc_B[3])
            PORTB |= 1 << pin_led;
        else
            PORTB &= ~(1 << pin_led);
        ticks_B[3]++;
        if (ticks_B[3] >= periodo_B[3])
            ticks_B[3]=0;
    }
    
}