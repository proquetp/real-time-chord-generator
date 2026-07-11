/* 
 * @file:       microfono.c
 * 
 * @author:     MAC,PFF,PRP
 *
 * @version:    1.0.0
 *
 * @date        10 de noviembre de 2025
 * 
 * @brief:      Driver que toma la medida del conversor A/D y devuleve la nota
 *              que está sonando
 */

#include <xc.h>
#include "microfono.h"
#include "acordes.h" 

//-----------------------------------------------------------------------------
//------------------FUNCIONES PÚBLICAS-----------------------------------------
//-----------------------------------------------------------------------------

unsigned char Nota(unsigned int per) {
    unsigned int diff = 0xFFFF, min_diff = 0xFFFF;
    unsigned char index = 0;
    unsigned char tam = sizeof (PERIODO_NOTAS) / sizeof (PERIODO_NOTAS[0]);

    for (unsigned char i = 0; i < tam; i++) {
        if (per > PERIODO_NOTAS[i])
            diff = per - PERIODO_NOTAS[i];
        else
            diff = PERIODO_NOTAS[i] - per;

        if (diff < min_diff) {
            min_diff = diff;
            index = i;
        }
    }
    
    if (min_diff > 20)
        return 50;
    
    return index;
}