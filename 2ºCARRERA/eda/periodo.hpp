//-------------------------------------------------------------------------------------------
// File:   periodo.hpp
// Author: Carlos Solana Melero (872815) -- Diego Mateo Lorente (873338)
// Date:   14-11-2023
//-------------------------------------------------------------------------------------------

#ifndef PERIODO_HPP
#define PERIODO_HPP

#include <iostream>
#include "instante.hpp"
using namespace std;

// PREDECLARACIÓN DEL TAD periodo

/*Los valores del TAD representan periodos de tiempo
  cronológicamente correctos y finalizados. Es decir, valores que incluyen siempre un instante de
  tiempo como inicio del periodo y un instante de tiempo final del periodo cronológicamente válido,
  es decir, igual o posterior al instante de inicio del periodo. Las fechas de los instantes de
  inicio y final podrán ser distintas, pero deberán corresponder al mismo año y mes
*/
struct periodo;

/*Dado un instante de tiempo i y un instante f igual o posterior a i, devuelve un periodo
  cronológicamente correcto y finalizado, igual al resultante de: iniciarse en i y finalizar en f.
  Parcial: la operación no está definida si los instantes i y f no pueden constituir un periodo
  cronológicamente correcto, es decir, el instante f es anterior al instante i, o bien no pertenecen
  al mismo año y mes (PeriodoNoVálido).
*/
bool crearPeriodo(const instante& i, const instante& f, periodo& p);

//Dado un periodo p, devuelve el instante de tiempo inicio del periodo.
instante obtenerInicio(const periodo& p);

//Dado un periodo p, devuelve el instante de tiempo final del periodo.
instante obtenerFinal(const periodo& p);

/*Dado un periodo p, devuelve el número de minutos transcurridos desde el instante de tiempo inicio
  del periodo, hasta su instante final.
*/
double obtenerDuracion(const periodo& p);

/*Dado un periodo p1 y un periodo p2, devuelve verdad si y solo si ambos periodos tienen instantes
  de inicio iguales, y también tienen instantes de final iguales.
*/
bool iguales(const periodo& p1, const periodo& p2);

/*Dado un periodo p1 y un periodo p2, devuelve verdad si y solo si el instante de inicio del
  periodo p1 es estricta y cronológicamente anterior al instante de inicio del periodo p2, o si
  sus instantes de inicio son iguales pero el instante de final del periodo p1 es estricta y
  cronológicamente anterior al instante de final del periodo p2.
*/
bool anterior(const periodo& p1, const periodo& p2);

/*Dado un periodo p1 y un periodo p2, devuelve verdad si y solo si ambos periodos de tiempo se
  solapan: ambos periodos son iguales o, incluyen instantes de tiempo en común distintos de los
  respectivos inicio y final (es decir, la intersección de los periodos como intervalos abiertos
  no es vacía, si el inicio de un periodo es igual al final del otro periodo no hay solape).
*/
bool solapados(const periodo& p1, const periodo& p2);

// DECLARACIÓN DEL TAD periodo

struct periodo{
    friend bool crearPeriodo(const instante& i, const instante& f, periodo& p);
    friend instante obtenerInicio(const periodo& p);
    friend instante obtenerFinal(const periodo& p);
    friend double obtenerDuracion(const periodo& p);
    friend bool iguales(const periodo& p1, const periodo& p2);
    friend bool anterior(const periodo& p1, const periodo& p2);
    friend bool solapados(const periodo& p1, const periodo& p2);
    private:
        instante inicio;
        instante final;
        int duracion;
};

#endif