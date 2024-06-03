//-------------------------------------------------------------------------------------------
// File:   instante.hpp
// Author: Carlos Solana Melero (872815) -- Diego Mateo Lorente (873338)
// Date:   14-11-2023
//-------------------------------------------------------------------------------------------

#ifndef INSTANTE_HPP
#define INSTANTE_HPP

#include <iostream>
using namespace std;

// PREDECLARACIÓN DEL TAD instante 

/*Los valores del TAD representan instantes de tiempo correctos,es decir, 
  valores que incluyen una fecha y hora válidos según las reglas del calendario gregoriano.
*/
struct instante;
/*Dados 5 valores enteros, devuelve un dato instante compuesto con dichos valores utilizados como
  día, mes, año, hora y minuto respectivamente.
  Parcial: la operación no está definida si los valores día, mes, año, hora y minuto no pueden
  formar un instante válido según las reglas del calendario gregoriano (InstanteNoVálido).
*/
bool crearInstante(const int dia, const int mes, const int anio, const int hora, const int minuto, instante& i);

//Dado un instante i, devuelve el entero que corresponde al día en el instante i.
int dia(const instante& i);

//Dado un instante i, devuelve el entero que corresponde al mes en el instante i.
int mes(const instante& i);

//Dado un instante i, devuelve el entero que corresponde al año en el instante i.
int anio(const instante& i);

//Dado un instante i, devuelve el entero que corresponde a la hora en el instante i.
int hora(const instante& i);

//Dado un instante i, devuelve el entero que corresponde al minuto en el instante i.
int minuto(const instante& i); 

/*Dados dos instantes i1 e i2, devuelve un booleano con valor verdad si y solo si el instante i1
  es igual que el instante i2, es decir, corresponden al mismo día, mes, año, hora y minuto.
*/
bool iguales(const instante& i1, const instante& i2);

/*Dados dos instantes i1 e i2, devuelve un booleano con valor verdad si y solo si el instante i1
  es estricta y cronológicamente anterior al instante i2.
*/
bool anterior(const instante& i1, const instante& i2);

/*Dados dos instantes i1 e i2, devuelve un booleano con valor verdad si y solo si el instante i1
  es estricta y cronológicamente posterior al instante i2.
*/
bool posterior(const instante& i1, const instante& i2);

/*Dados dos instantes i1 e i2, correspondientes al mismo año y mes, devuelve un entero con el
  número total de minutos comprendidos desde el instante i1 hasta el instante i2. Si ambos instantes
  i1 e i2 no pertenecen al mismo año y mes, entonces devolverá el entero de valor absoluto 100.000.
  El resultado será un número positivo si anterior(i1,i2) o será negativo si posterior(i1,i2).
*/
int diferencia(const instante& i1, const instante& i2);

// DECLARACIÓN DEL TAD instante

struct instante {
    friend bool crearInstante(const int dia, const int mes, const int anio, const int hora, const int minuto, instante& i);
    friend int dia(const instante& i);
    friend int mes(const instante& i);
    friend int anio(const instante& i);
    friend int hora(const instante& i);
    friend int minuto(const instante& i);
    friend bool iguales(const instante& i1, const instante& i2);
    friend bool anterior(const instante& i1, const instante& i2);
    friend bool posterior(const instante& i1, const instante& i2);
    friend int diferencia(const instante& i1, const instante& i2);
    private:
        int dia;
        int mes;
        int anio;
        int hora;
        int minuto;
};

#endif