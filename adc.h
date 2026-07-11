/* 
 * @file:       adc.h
 * 
 * @author:     MAC,PFF,PRP
 *
 * @version:    1.0.0
 *
 * @date        10 de noviembre de 2025
 * 
 * @brief:      Interfaz del driver del conversor A/D
 */

//------------------------------------------------------------------------------

#ifndef _ADC_H
#define	_ADC_H

//------------------------------------------------------------------------------

/**
 * Recibe en el driver el pin analógico que se desea usar para leer la señal
 * del micrófono y configura el puerto correspondiente al pin analógico indicado 
 * como entrada analógica.
 *  
 * @param pin_analogico índice del pin analógico que se va a utilizar
 */
void configurarPinADC(unsigned int pin_analogico);

//------------------------------------------------------------------------------

/**
 * Inicializa el conversor AD a 500 Ksps en el pin indicado por la función 
 * ConfigurarPinADC configurando la interrupción
 */
void inicializarADC(void);

//------------------------------------------------------------------------------

/**
 * Función Getter para devolver la medida del ADC en forma binaria
 * 
 * @return Señal binaria ajustada a la frecuencia de la señal analógica recibida
 *          por el puerto configurado para poder ser utilizada en forma de flanco
 */
unsigned char getMedidaADC(void);

//------------------------------------------------------------------------------

#endif	/* ADC_H */

