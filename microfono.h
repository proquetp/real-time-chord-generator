/* 
 * @file:    pwm.h
 * 
 * @author:  MAC,PFF,PRP
 * 
 * @version: 1.0.0
 *
 * @date    10 de noviembre de 2025
 * 
 * @brief:  Interfaz del driver para obtener la nota que está sonando
 */

#ifndef _MICROFONO_H
#define	_MICROFONO_H

#define THRESHOLD 900  // Umbral de cruce (1.65V si Vref = 3.3V)
#define HYST 40                     // Ancho de histéresis en cuentas ADC
#define TH_UP   (THRESHOLD + HYST)  // Umbral superior
#define TH_DOWN (THRESHOLD - HYST)  // Umbral inferior

/**
 * Devuelve la nota a partir del valor del periodo
 * @param per Periodo de la nota que se desea conocer en centésimas de
 *              milisegundo
 * @return Devuelve el ordinal en la escala dodecafónica de la nota con el 
 *          periodo dado
 */
unsigned char Nota(unsigned int per);

#endif	/* MICROFONO_H */