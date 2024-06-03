/******************************************************************************\
 * Programación 1. Práctica 4
 * Autores: ------------- carlos solana -------------
 * Ultima revisión: 
 * Resumen: Fichero de implementación «naturales-grandes.cpp» de un módulo
 *          denominado «naturales-grandes» para trabajar con números naturales 
 *          de gran magnitud en la 4ª práctica.
\******************************************************************************/

#include "naturales-grandes.hpp"
#include <cmath>
#include <iostream>
using namespace std;

/*
 * Pre:  «original» almacena la representación de un número natural. La
 *       componente original[0] representa   
 *       las unidades de «original», la componente original[1] las decenas de  
 *       «original» y así, sucesivamente. Las componentes de índices
 *       superiores al correspondiente al dígito más significativo
 *       almacenan el valor 0. «original» y «copia» tienen 
 *       «NUM_DIGITOS» componentes.
 * Post: Tras ejecutar este procedimiento, «copia» almacena una representación
 *       idéntica a la del número natural correspondiente a la de «original».
 */
void copiar(const unsigned original[], unsigned copia[]) {
     for (unsigned i = 0; i < NUM_DIGITOS; i++){
         copia[i] = original[i];
     }
    // Por completar
}


/*
 * Pre:  «a» y «b» almacenan las representaciones de sendos números naturales. 
 *       Las componentes a[0] y b[0] representan las unidades del natural «a» y 
 *       «b», respectivamente, las componentes a[1] y b[1], las decenas y así 
 *       sucesivamente. Las componentes de índices
 *       superiores a los correspondientes a los dígitos más significativos
 *       almacenan el valor 0. Las componentes de 
 *       índices superiores almacenan el valor 0. «a» y «b» tienen «NUM_DIGITOS»
 *       componentes.
 * Post: Devuelve el valor booleano true si «a» y «b» representan el mismo
 *       número natural y false en caso contrario.
 */
bool sonIguales(const unsigned a[], const unsigned b[]) {
    bool iguales = true;
    unsigned i = 0;
    while (iguales && i < NUM_DIGITOS){
        iguales = (a[i] == b[i]);
        i++;
    }
    return iguales;
}


/*
 * Pre:  «natural» almacena la representación de un número natural. La 
 *       componente natural[0] representa las unidades de «natural», la
 *       componente natural[1] las decenas de «natural» y así, sucesivamente. 
 *       Las componentes de índices superiores al correspondiente al dígito más
 *       significativo almacenan el valor 0. «natural» tiene «NUM_DIGITOS»
 *       componentes.
 * Post: Devuelve el número de cifras de «natural» cuando se escribe en base 10
 *       sin ceros a la izquierda.
 */
unsigned numCifras(const unsigned natural[]) {
    unsigned cifras = 0;
    for(unsigned i = NUM_DIGITOS - 1; natural[i] == 0 && i>0; i--) {
        cifras = i;
        if (cifras == 0){   
            cifras = 1;
        }

    }
    return cifras;
    // Por completar
}

/*
 * Pre:  «natural» almacena la representación de un número natural grande. Sea 
 *       «n» el número de dígitos de «natural». La componente natural[0] 
 *       representa las unidades de «natural», la componente natural[1] las
 *       decenas de «natural» y así, sucesivamente. Las componentes de índices
 *       superiores al correspondiente al dígito más significativo
 *       almacenan el valor 0. «natural» tiene «NUM_DIGITOS» componentes.
 * Post: Tras ejecutar este procedimiento, se ha escrito en la pantalla el
 *       número natural representado por «natural» (sin ceros a la izquierda).
 */
void escribirEnPantalla(const unsigned natural[]) {
   for (unsigned int i = 0; i < numCifras(natural); i++)
    {
        cout<<natural[i];
    }
        
    // Por completar
}


/*
 * Pre:  «naturalGrande» tiene «NUM_DIGITOS» componentes.
 * Post: Tras ejecutar este procedimiento, «naturalGrande» contiene la secuencia
 *       de dígitos equivalente a «numero». 
 *       La componente naturalGrande[0] almacena las unidades de «numero», la
 *       componente naturalGrande[1], las decenas de «numero» y así 
 *       sucesivamente. Las componentes de índices superiores al correspondiente
 *       al dígito más significativo almacenan el valor 0.
 */
void convertir(const unsigned numero, unsigned naturalGrande[]) {

    unsigned n = numero;
    unsigned aux;
    unsigned contador = 0;

    for (unsigned int i = 0; i < NUM_DIGITOS; i++){
        naturalGrande[i] = 0;
    }
    while (n != 0){
        aux = n % 10;
        naturalGrande[contador] = aux;
        n /= 10;
        contador++;
    }
    // Por completar
}


/*
 * Pre:  «naturalGrande» almacena los dígitos de un número natural lo 
 *       suficientemente pequeño como para ser representado por un dato de tipo
 *       «unsigned». En la componente naturalGrande[0] se almacena el dígito
 *       que representa las unidades de ese número, en la componente 
 *       naturalGrande[1], las decenas y así sucesivamente. Las componentes de
 *       índices superiores al correspondiente al dígito más significativo
 *       almacenan el valor 0. «naturalGrande» tiene «NUM_DIGITOS» componentes.
 * Post: Devuelve el valor numérico del natural almacenado en «naturalGrande».
 */
unsigned valor(const unsigned naturalGrande[]) {
    unsigned natural = 0;
    unsigned contador = 1;
    for(unsigned i = 0 ; i < 10; i++){
        natural += naturalGrande[i] * contador;
        contador = contador * 10;
    }
    return natural;
    // Por completar
}


/*
 * Pre:  «a» y «b» almacenan las representaciones de sendos números naturales. 
 *       Las componentes a[0] y b[0] representan las unidades del natural «a» y 
 *       «b», respectivamente, las componentes a[1] y b[1], las decenas y así 
 *       sucesivamente. Las componentes de índices superiores a los
 *       correspondientes a los dígitos más significativos almacenan el valor 0.
 *       La suma de los números representados por «a» y «b» tiene menos de
 *       «NUM_DIGITOS» dígitos.
 *       Los vectores «a», «b» y «suma» tienen «NUM_DIGITOS» componentes.
 * Post: Tras ejecutar este procedimiento, «suma» almacena la representación de
 *       un número natural equivalente a la suma de los naturales representados
 *       por «a» y «b». La componente suma[0] representa las unidades de la
 *       suma, la componente suma[1] representa las decenas y así sucesivamente.
 */
void sumar(const unsigned a[], const unsigned b[], unsigned suma[]) {
        unsigned llevada=0;

        for( unsigned i = 0; i < NUM_DIGITOS; i++){
            suma[i] = (a[i] + b[i] + llevada)%10;
            if((a[i] + b[i]+ llevada) >= 10){
                llevada = 1;
            }
            else{
                llevada = 0;
            }

        }
    // Por completar
}


/*
 * Pre:  «natural» almacena la representación de un número natural. La
 *       componente natural[0] representa las unidades de «natural», la
 *       componente natural[1] las decenas de «natural» y así sucesivamente. Las
 *       componentes de índices superiores al correspondiente al dígito más
 *       significativo almacenan el valor 0. «natural» e «imagen» tienen
 *       «NUM_DIGITOS» componentes.
 * Post: Tras ejecutar este procedimiento, «imagen» almacena la representación
 *        del número natural correspondiente a la imagen especular de «natural».
 */
void calcularImagen(const unsigned natural[], unsigned imagen[]) {
    for (unsigned int i = 0; i < NUM_DIGITOS; i++){
        imagen[i] = 0;
    }
    unsigned int digitos = numCifras(natural);
    
    for (unsigned int i = 0; i < digitos; i++){
        imagen[i] = natural[digitos - i - 1];
    }
    // Por completar
}


/*
 * Pre:  «natural» almacena la representación de un número natural. La
 *       componente natural[0] representa las unidades de «natural», la
 *       componente natural[1] las decenas de «natural» y así sucesivamente. Las
 *       componentes de índices superiores al correspondiente al dígito más
 *       significativo almacenan el valor 0. «natural» tiene «NUM_DIGITOS»
 *       componentes.
 * Post: Devuelve «true» si y solo si «natural» es un número capicúa.
 */
bool esCapicua(const unsigned natural[]) {
    unsigned int inversa [NUM_DIGITOS];
    unsigned int digitos = numCifras(natural);
    bool capicua = true;
    calcularImagen(natural, inversa);
    while (digitos < NUM_DIGITOS)
    {
        if (natural[digitos] != inversa[digitos])
        {
            capicua = false;
    
        }
        digitos--;
    }
    return capicua;
    // Por completar
}

