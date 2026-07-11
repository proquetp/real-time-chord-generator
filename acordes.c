/* 
 * @file:       acordes.c
 * 
 * @author:     MAC,PFF,PRP
 *
 * @version:    1.0.0
 *
 * @date        24 de noviembre de 2025
 * 
 * @brief:      Driver de la máquina de estados
 */

#include <xc.h>
#include "acordes.h" 
#include "timer3.h"
#include "pwm.h"
#include "adc.h"
#include "config.h"
#include "microfono.h"

// ------------------------------------------------------
// --------------------- CONSTANTES ---------------------
// ------------------------------------------------------
#define K 4  // Número de ciclos para calcular el periodo

#define P3 4 //melodia
#define P2 3 //modo
#define ACORDE 2 //pulsador

#define NUM_ACORDES 2
#define LED 3
#define NOTAS 12

#define T1 1 //provisional en dms
#define TMAX 50000 //provisional en ticks
#define NUM_NOTAS 9 //maximo en cada melodia 

// ------------------------------------------------------
// ------------ VARIABLES GLOBALES AL MÓDULO ------------
// ------------------------------------------------------
static unsigned char intercambio = 0;
static unsigned int ticks = 0;
static unsigned char acorde = 1;
static unsigned char nota[] = {0, 0, 0};
static unsigned int notas_cancion[NUM_NOTAS];
static unsigned int tiempos_cancion[NUM_NOTAS];

static unsigned int cms = 0;

static unsigned char subida = 0;
static unsigned char indice = 0;

// ------------------------------------------------------
// ------------ FUNCIONES PRIVADAS ----------------------
// ------------------------------------------------------

/**
 * Detecta el flanco de subida en la medida de frecuencias del conversor AD
 * Y devuelve el periodo promedio de los ciclos establecidos por K para reducir 
 * el error
 */
void LeerNota(void);

/**
 * Reproduce en tiempo real el acorde sobre la nota que está sonando
 * Obtiene la nota del micrófono y reproduce el acorde mayor o menor
 * dando la señal apropiada por cada uno de los tres zumbadores
 */
void reproduceacorde(void);

/**
 * Máquina de estados dentro del estado 1 de "tareaAcorde" para gestionar los
 * cuatro subestados de este modo de funcionamiento: espera, escucha, 
 * reproducción o "Para Elisa".
 */
void melodia(void);

/**
 * Pone de nuevo a cero los contadores del timer 3
 */
void resetTiempo(void);

/**
 * detecta el flanco de bajada del pulsador indicado
 * 
 * @param pin introduce en la función el pin del pulsador del flanco que 
 *              se desea conocer
 * 
 * @return devuelve una variable binaria que informa de si ha sucedido
 *          el flanco o no
 */
unsigned char detectaflanco(unsigned char pin);

/**
 * Enciende el led para indicar con la frecuencia de parpadeo (8Hz 0 2HZ)
 * el tipo de acorde elegido
 */
void encenderLED(void);

/**
 * Lee el pulsador para fijar tipo de acorde que se desea reproducir:
 * mayor o menor
 */
void setAcorde(void);

/**
 * Forma el acorde mayor o menor a partir de la nota introducida
 * 
 * @param posicion indica la nota (por su ordinal en la escala dodecafónica) 
 *                  sobre la que se desea formar el acorde.
 */
void creaAcorde(unsigned int posicion);

/**
 * Registra nueve notas diferentes recibidas en el micrófono
 * en dos vectores: uno para los índices de las notas en la escala dodecafónica
 * y otro para los tiempos que dura cada nota 
 */
void escucha(void);

/**
 * Reproduce por cada zumbador la nota correspondiente del acorde formado a
 * partir de las nueve notas registradas por la función "escucha" o introducidas
 * por la función "llenaelisa"
 */
void reproduccion(void);

/**
 * Pone a cero todos los valores de la Melodía registrada 
 * para que no se reproduzca nada
 */
void resetMelodia(void);

/**
 * Inicializa y pone en marcha el Timer 1 para contar centésimas de milisegundo
 */
void InicializarTimer(void);

/**
 * Introduce el inicio de la melodía de Beethoven "Für Elise" en la cadena a
 * reproducir
 */
void llenaelisa(void);

//-----------------------------------------------------------------------------
//------------------FUNCIONES PÚBLICAS-----------------------------------------
//-----------------------------------------------------------------------------

void inicializarAcorde(void) {

    TRISB |= (1 << P2);
    AD1PCFGL |= (1 << (P2 + 2));

    TRISB |= (1 << ACORDE);
    AD1PCFGL |= (1 << (ACORDE + 2));

    TRISB |= (1 << P3);
    //AD1PCFGL |= (1 << P3);

    InicializarTimer();
    inicializarTimer3(T1); //dms

    subida = getMedidaADC();

}

void tareaAcorde(void) {

    static unsigned char estado = 0;
    static unsigned char flanco = 0;
    static unsigned char cambio_estado = 0;

    setAcorde();
    encenderLED();

    if (cambio_estado) {
        resetTiempo();
        cambio_estado = 0;
    }

    switch (estado) {

        case 0:
            reproduceacorde();
            flanco = detectaflanco(P2);
            if (flanco) {
                cambio_estado = 1;
                intercambio = 1;
                estado = 1;
                for (int j = 0; j < 3; j++) {
                    periodoPWM_B(j, 0);
                }
            }
            break;
        case 1:
            melodia();
            flanco = detectaflanco(P2);
            if (flanco) {
                cambio_estado = 1;
                estado = 0;
            }
            break;
    }
}

//-----------------------------------------------------------------------------
//------------------FUNCIONES PRIVADAS-----------------------------------------
//-----------------------------------------------------------------------------

void setAcorde(void) {

    unsigned char flanco = 0;

    flanco = detectaflanco(ACORDE);

    if (flanco) {
        acorde++;
        if (acorde >= NUM_ACORDES) {
            acorde = 0;
        }
        resetTiempo();
    }

}

void creaAcorde(unsigned int posicion) {
    unsigned char nota3 = 0, nota2 = 0, nota1 = 0;

    nota1 = posicion;
    nota2 = posicion + 3 + acorde;
    nota3 = posicion + 7;

    if (nota2 >= NOTAS) {
        nota2 = nota2 - NOTAS + 1;
        nota3 = nota3 - NOTAS + 1;
    }
    else if (nota3 >= NOTAS)
        nota3 = nota3 - NOTAS + 1;

    nota[0] = nota1;
    nota[1] = nota2;
    nota[2] = nota3;

}

unsigned char detectaflanco(unsigned char pin) {
    if (pin > P3 || pin < ACORDE) {
        return 0;
    }

    static unsigned char pulsador[] = {1, 1, 1};
    unsigned char pulsador_ant = pulsador[pin - 2];
    unsigned char flanco = 0;

    pulsador[pin - 2] = (PORTB >> pin)&0x1;
    if (pulsador[pin - 2] < pulsador_ant) {
        flanco = 1;
    }

    return flanco;

}

void reproduceacorde(void) {
    unsigned int j = 0;

    LeerNota();

    if (indice < 50)
        creaAcorde(indice);
    //}

    for (j = 0; j < 3; j++) {
        periodoPWM_B(j, PERIODO_NOTAS[nota[j]]);
    }
}

void melodia(void) {
    static unsigned char estado = 0;
    static unsigned char flanco = 0;
    static unsigned char cambio_estado = 0;

    if (cambio_estado) {
        resetTiempo();
        cambio_estado = 0;
    }

    if (intercambio) {
        intercambio = 0;
        estado = 0;
    }

    switch (estado) {

        case 0:
            resetMelodia();
            flanco = detectaflanco(P3);
            for (int j = 0; j < 3; j++) {
                periodoPWM_B(j, 0);
            }
            if (flanco) {
                cambio_estado = 1;
                estado = 1;
            }
            break;
        case 1:
            escucha();
            flanco = detectaflanco(P3);
            if (flanco) {
                cambio_estado = 1;
                estado = 2;
            }
            break;
        case 2:
            reproduccion();
            flanco = detectaflanco(P3);
            if (flanco) {
                cambio_estado = 1;
                estado = 3;
                resetMelodia();
                llenaelisa();
            }
            break;
        case 3:
            reproduccion();
            flanco = detectaflanco(P3);
            if (flanco) {
                cambio_estado = 1;
                estado = 0;
            }
            break;

    }

}

void escucha(void) {
    static unsigned int k = 0;
    static unsigned int nota = 12;
    unsigned int nota_ant = nota;

    LeerNota();

    if (indice < 50) {
        nota_ant = nota;
        nota = indice;

        if (nota_ant != nota) {

            tiempos_cancion[k] = ticks;
            notas_cancion[k] = nota_ant;
            resetTiempo();
            nota_ant = nota;
            k++;
        }
    }
}

void resetMelodia(void) {
    unsigned int i = 0;

    for (i = 0; i < NUM_NOTAS; i++) {
        notas_cancion[i] = 0;
        tiempos_cancion[i] = 0;
    }
}

void reproduccion(void) {

    static unsigned int i = 0;
    unsigned int j = 0;

    if (ticks > tiempos_cancion[i]) {
        resetTiempo();
        i++;
        if (i >= NUM_NOTAS) {
            i = 0;
        }
        creaAcorde(notas_cancion[i]);

    } else {
        for (j = 0; j < 3; j++) {
            periodoPWM_B(j, PERIODO_NOTAS[nota[j]]);
        }
    }

}

void encenderLED(void) {

    static unsigned int t = 0;

    t = 12500 * (1 + 3 * acorde);

    periodoPWM_B(3, t);

}

void resetTiempo(void) {
    ticks = 0;
    TMR3 = 0;
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    IFS0bits.T3IF = 0;
    ticks++;
    if (ticks >= TMAX) {
        ticks = 0;
    }
}

void LeerNota(void) {
    static unsigned char cont = 0;
    unsigned int temp;
    static unsigned char subida = 0;
    unsigned char subida_ant = subida;
    subida = getMedidaADC();

    if (subida > subida_ant) {
        if (cont == 0) {
            TMR1 = 0;
            cms = 0;
        }

        cont++;

        if (cont >= K) {
            cont = 0;
            temp = (cms + K / 2) / K;
            indice = Nota(temp);
        }
    }
}

void InicializarTimer(void) {
    T1CON = 0;
    PR1 = FCY / 100000; // centésimas de milisegundo
    TMR1 = 0;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    IPC0bits.T1IP = 5;
    T1CON |= 1 << 15;
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0;
    cms++;
}

void llenaelisa(void) {
    //const unsigned int elise_notas[] = {4, 3, 4, 3, 4, 11, 2, 0, 9};      //Für Elisa
    //const unsigned int elise_tiempos[] = {2500, 2500, 2500, 2500, 5000, 2500, 2500, 2500, 5000};
    
    //const unsigned int elise_notas[] = {2, 7, 11, 9, 7, 6, 4, 6, 2};        //Probar otra canción: Cascanueces
    //const unsigned int elise_tiempos[] = {5000, 5000, 5000, 2500, 5000, 2500, 2500, 2500, 5000};
    
    const unsigned int elise_notas[] = {4, 11, 0, 2, 4, 5, 2, 0, 11}; //Probar otra canción: Tetris
    const unsigned int elise_tiempos[] = {5000, 5000, 5000, 5000, 5000, 5000, 2500, 2500, 2500};

    unsigned int i = 0;
    unsigned int n = 9;

    for (i = 0; i < n; i++) {
        tiempos_cancion[i] = elise_tiempos[i];
        notas_cancion[i] = elise_notas[i];
    }
}