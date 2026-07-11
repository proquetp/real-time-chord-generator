/* 
 * @file:       adc.h
 * 
 * @author:     MAC,PFF,PRP
 *
 * @version:    1.0.0
 *
 * @date        10 de noviembre de 2025
 * 
 * @brief:      Interfaz del driver de la máquina de estados 
 */

//------------------------------------------------------------------------------

#ifndef _ACORDES_H
#define _ACORDES_H

// Constantes globales

static const unsigned int PERIODO_NOTAS[] = {382, 361, 341, 321, 303, 286, 270, 255, 241, 227, 214, 202};

//-----------------------------------------------------------------------------
//------------------FUNCIONES PÚBLICAS-----------------------------------------
//-----------------------------------------------------------------------------

/**
 * Máquina de estados global para gestionar los estados de funcionamiento:
 * automático o melodía
 */
void tareaAcorde(void);

/**
 * Inicializa los puertos de los pulsadores y los timers 1 y 3.
 */
void inicializarAcorde(void);

#endif	/* ACORDES_H */


