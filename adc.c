/* 
 * @file:       adc.c
 * 
 * @author:     MAC,PFF,PRP
 *
 * @version:    1.0.0
 *
 * @date        10 de noviembre de 2025
 * 
 * @brief:      Driver del conversor A/D
 */

#include <xc.h>
#include "adc.h"
#include "microfono.h"


#define NUM_PINES_AD 6  // Número de pines A/D 

//-----------------------------------------------------------------------------
//------------------VARIABLES GLOBALES AL MÓDULO-------------------------------
//-----------------------------------------------------------------------------

static unsigned char pin_an;
static unsigned char subida; 


//-----------------------------------------------------------------------------
//------------------FUNCIONES PRIVADAS-----------------------------------------
//-----------------------------------------------------------------------------

/**
 * Rutina de interrupción del ADC
 */
void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void);

//-----------------------------------------------------------------------------
//------------------FUNCIONES PÚBLICAS-----------------------------------------
//-----------------------------------------------------------------------------

void configurarPinADC(unsigned int pin_analogico) { 
// Comprobar que el argumento es válido 
    if (pin_analogico >= NUM_PINES_AD) 
        return; 
// Configurar como entrada 
    if (pin_analogico <= 1) // 0 ó 1
        TRISA |= 1 << pin_analogico; 
    else 
        TRISB |= 1 << (pin_analogico-2);
    AD1PCFGL &= ~(1 << pin_analogico);  // Pin analógico 2-7 aquí 0-5
    pin_an = pin_analogico;
}
//------------------------------------------------------------------------------

void inicializarADC(void) { 
    AD1CON3 = 0x0105;    // Muestreo: 1 ciclo, ADCS = 5 
    IFS0bits.AD1IF = 0;  // Borrar la bandera 
    IEC0bits.AD1IE = 1;  // Habilitar interrupciones 
    IPC3bits.AD1IP = 4;  // Prioridad interrupciones 
    AD1CON1 = 0x80E0;    // ON, conversión automática 
    AD1CHS0 = pin_an; 
    AD1CON1 |= 1 << 1;   // Empezar a muestrear 
} 
//------------------------------------------------------------------------------

unsigned char getMedidaADC(void) { 
    return subida; 
}

//-----------------------------------------------------------------------------
//------------------FUNCIONES PRIVADAS-----------------------------------------
//-----------------------------------------------------------------------------

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void) { 
    IFS0bits.AD1IF = 0;             // Borrar la bandera 
    unsigned int medida_adc = ADC1BUF0; // Guardar la medida
    
    // Esperar primer cruce ascendente real con histéresis
    if (medida_adc > TH_UP)
        subida = 1;
    else if (medida_adc < TH_DOWN)
        subida = 0;
    
    AD1CON1 |= 1 << 1;  // Empezar a muestrear 
}