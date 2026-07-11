/* 
 * @file:       main.c
 * 
 * @author:     MAC,PFF,PRP
 *
 * @version:    1.0.0
 *
 * @date        24 de noviembre de 2025
 * 
 * @brief:      Código principal del proyecto final de laboratorio
 */

#include <xc.h>
#include "config.h"
#include "pwm.h"
#include "acordes.h"
#include "adc.h"

int main(void) {
    inicializarReloj();
    InicializarPWM();
    configurarPinADC(0);
    inicializarADC();
    inicializarAcorde();
    
    while(1){
        tareaAcorde();
    }
    return 0;
}