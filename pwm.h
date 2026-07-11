/* 
 * @file:    pwm.h
 * 
 * @author:  MAC,PFF,PRP
 * 
 * @version: 1.0.0
 *
 * @date    10 de noviembre de 2025
 * 
 * @brief:  Interfaz del driver para crear los acordes: tres señales pwm a 
 *          frecuencias independientes para cada pin de salida correpondiendo 
 *          a un altavoz
 */

#ifndef _PWM_H
#define	_PWM_H

/**
 * Función de configuración de los puertos de salida, del Timer2 y las 
 * interrupciones
 */
void InicializarPWM(void);

/**
 * Setter para la frecuencia (en forma de periodo) de cada salida
 * 
 * @param pin índice de la señal de salida para asignarlo a un pin del controlador
 * 
 * @param cms periodo de la señal de salida en centéssimas de milisegundo
 */
void periodoPWM_B(unsigned char pin, unsigned int cms);

/**
 * Setter para el factor de servicio de cada salida por si se desea uno distinto del 50%
 * 
 * @param pin índice de la señal de salida para asignarlo a un pin del controlador
 * 
 * @param cms factor de servicio de la señal de salida en centésimas de milisegundo
 */
void dcPWM_B(unsigned char pin, unsigned int cms);

#endif	/* _PWM_H */