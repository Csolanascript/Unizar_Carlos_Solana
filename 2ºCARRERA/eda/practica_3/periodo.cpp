//-------------------------------------------------------------------------------------------
// File:   periodo.cpp
// Author: Carlos Solana Melero (872815) -- Diego Mateo Lorente (873338)
// Date:   14-11-2023
//-------------------------------------------------------------------------------------------

#include <iostream>
#include "periodo.hpp"
#include "instante.hpp"

using namespace std;

/*Dado un instante de tiempo i y un instante f igual o posterior a i, devuelve un periodo
  cronológicamente correcto y finalizado, igual al resultante de: iniciarse en i y finalizar en f.
  Parcial: la operación no está definida si los instantes i y f no pueden constituir un periodo
  cronológicamente correcto, es decir, el instante f es anterior al instante i, o bien no pertenecen
  al mismo año y mes (PeriodoNoVálido).
*/
bool crearPeriodo(const instante& i, const instante& f, periodo& p) {
  if(anterior(f,i) || mes(i) != mes(f) || anio(i) != anio(f)) {
      return false;
  }
  else {
    p.inicio = i;
    p.final = f;
    p.duracion = diferencia(i,f);
    if(p.duracion < 0) {
      p.duracion = -p.duracion;
    }
    return true;
  }
}

//Dado un periodo p, devuelve el instante de tiempo inicio del periodo.
instante obtenerInicio(const periodo& p) {
  return p.inicio;
}

//Dado un periodo p, devuelve el instante de tiempo final del periodo.
instante obtenerFinal(const periodo& p) {
  return p.final;
}

/*Dado un periodo p, devuelve el número de minutos transcurridos desde el instante de tiempo inicio
  del periodo, hasta su instante final.
*/
double obtenerDuracion(const periodo& p) {
  return p.duracion;
}

/*Dado un periodo p1 y un periodo p2, devuelve verdad si y solo si ambos periodos tienen instantes
  de inicio iguales, y también tienen instantes de final iguales.
*/
bool iguales(const periodo& p1, const periodo& p2) {
  return(iguales(p1.inicio, p2.inicio) && iguales(p1.final, p2.final));
}

/*Dado un periodo p1 y un periodo p2, devuelve verdad si y solo si el instante de inicio del
  periodo p1 es estricta y cronológicamente anterior al instante de inicio del periodo p2, o si
  sus instantes de inicio son iguales pero el instante de final del periodo p1 es estricta y
  cronológicamente anterior al instante de final del periodo p2.
*/
bool anterior(const periodo& p1, const periodo& p2) {
  return(anterior(p1.inicio, p2.inicio) || (iguales(p1.inicio,p2.inicio) && anterior(p1.final,p2.final)));
}

/*Dado un periodo p1 y un periodo p2, devuelve verdad si y solo si ambos periodos de tiempo se
  solapan: ambos periodos son iguales o, incluyen instantes de tiempo en común distintos de los
  respectivos inicio y final (es decir, la intersección de los periodos como intervalos abiertos
  no es vacía, si el inicio de un periodo es igual al final del otro periodo no hay solape).
*/
bool solapados(const periodo& p1, const periodo& p2) {
  return(iguales(p1,p2) || (anterior(p1.inicio,p2.final) && posterior(p1.inicio,p2.inicio)) 
        || (anterior(p2.inicio,p1.final) && posterior(p2.inicio,p1.inicio)) 
        ||iguales(p1.inicio,p2.inicio) || iguales(p1.final,p2.final));
}