/* 
 * @file:    timer3.h
 * 
 * @author:  MAC,PFF,PRP
 * 
 * @version: 1.0.0
 *
 * @date    21 de noviembre de 2025
 * 
 * @brief:  Interfaz del driver del Timer3
 */

#ifndef _TIMER3_H
#define _TIMER3_H

/**
 * Configura el Timer 3 para que cuente ajustando los parámetros del periférico.
 * 
 * @param dms Introduce en la configuración del Timer 3 el tiempo (en décimas de
 *              milisegundo) para el que se desea configurar el periférico
 */
void inicializarTimer3(unsigned int dms);

#endif	/* _TIMER3_H */