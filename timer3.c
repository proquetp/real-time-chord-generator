/* 
 * @file:    timer3.c
 * 
 * @author:  MAC,PFF,PRP
 * 
 * @version: 1.0.0
 *
 * @date    10 de noviembre de 2025
 * 
 * @brief:  Driver para configurar el timer3 con la medida de tiempo aportada
 */

#include <xc.h> 
#include "config.h" 
#include "timer3.h" 

void inicializarTimer3(unsigned int dms) {
    static unsigned long ticks;
    unsigned int preescalado;
    unsigned int t1con;

    // Calcular el número de ticks que corresponde al retardo 
    // dms * 0.1 ms * FCY = dms * FCY / 10000 
    ticks = dms * (FCY / 10000UL); //primero se realiza la división para no perder los valores mas significativos. 

    // Selección de preescalado 
    if (ticks <= 65535UL) {
        preescalado = 1;
        t1con = 0x0000;
    }
    else if (ticks / 8 <= 65535UL) {
        preescalado = 8;
        t1con = 0x0010;
    }
    else if (ticks / 64 <= 65535UL) {
        preescalado = 64;
        t1con = 0x0020;
    }
    else {
        preescalado = 256;
        t1con = 0x0030;
    }

    // Configurar Timer3
    T3CON = t1con; // OFF, preescalado 
    PR3 = (unsigned int) (ticks / preescalado);
    TMR3 = 0; // Reset contador 
    IFS0bits.T3IF = 0; // Borrar bandera
    IEC0bits.T3IE = 1; // Habilitar interrupciones
    IPC2bits.T3IP = 6; // Prioridad interrupciones
    T3CON |= (1 << 15); // Encender Timer3
}