//-------------------------------------------------------------------------------------------
// File:   instante.cpp
// Author: Carlos Solana Melero (872815) -- Diego Mateo Lorente (873338)
// Date:   14-11-2023
//-------------------------------------------------------------------------------------------

#include <iostream>
#include "instante.hpp"

using namespace std;

/*Dados 5 valores enteros, devuelve un dato instante compuesto con dichos valores utilizados como
  día, mes, año, hora y minuto respectivamente.
  Parcial: la operación no está definida si los valores día, mes, año, hora y minuto no pueden
  formar un instante válido según las reglas del calendario gregoriano (InstanteNoVálido).
*/
bool crearInstante(const int dia, const int mes, const int anio, const int hora, const int minuto, instante& i){
    bool ok = false;
    if(hora>=0 && minuto>=0 && hora<=24 && minuto<=60 && anio >= 1582)
    {
        switch (mes)
            {
            case  1 :
            case  3 :
            case  5 :
            case  7 :
            case  8 :
            case 10 :
            case 12 : if ( dia >= 1 && dia <= 31 )
                          ok= true;
                      break;

            case  4 :
            case  6 :
            case  9 :
            case 11 : if ( dia >= 1 && dia <= 30 )
                           ok= true;
                      break;

            case  2 : if( anio % 4 == 0 && anio % 100 != 0 || anio % 400 == 0 )
                            if ( dia >= 1 && dia <= 29 )
                                ok= true;
                            else
                                ok= true;
        }
        if(ok){
            i.dia = dia;
            i.mes = mes;
            i.anio = anio;
            i.minuto = minuto;
            i.hora = hora;
        }
        return ok;
    }
    else{
        return false;
    }
}



//Dado un instante i, devuelve el entero que corresponde al día en el instante i.
int dia(const instante& i){
    return i.dia;
}

//Dado un instante i, devuelve el entero que corresponde al mes en el instante i.
int mes(const instante& i){
    return i.mes;
}
//Dado un instante i, devuelve el entero que corresponde al año en el instante i.
int anio(const instante& i){
    return i.anio;
}
//Dado un instante i, devuelve el entero que corresponde a la hora en el instante i.
int hora(const instante& i){
    return i.hora;
}
//Dado un instante i, devuelve el entero que corresponde al minuto en el instante i.
int minuto(const instante& i){
    return i.minuto;
}
/*Dados dos instantes i1 e i2, devuelve un booleano con valor verdad si y solo si el instante i1
  es igual que el instante i2, es decir, corresponden al mismo día, mes, año, hora y minuto.
*/
bool iguales(const instante& i1, const instante& i2){
    return (i1.dia == i2.dia && i1.mes == i2.mes && i1.anio == i2.anio && i1.minuto == i2.minuto && i1.hora == i2.hora);
    }
/*Dados dos instantes i1 e i2, devuelve un booleano con valor verdad si y solo si el instante i1
  es estricta y cronológicamente anterior al instante i2.
*/
bool anterior(const instante& i1, const instante& i2){
    if(iguales(i1,i2)){
        return false;
    }
    else if (i1.anio > i2.anio)
    {
        return false;
    }
    else if (i1.mes > i2.mes)
    {
        return false;
    }
    else if (i1.dia > i2.dia)
    {
        return false;
    }
    else if (i1.hora > i2.hora)
    {
        return false;
    }    
    else if (i1.minuto > i2.minuto)
    {
        return false;
    }
    else{
        return true;
    }
    

}

/*Dados dos instantes i1 e i2, devuelve un booleano con valor verdad si y solo si el instante i1
  es estricta y cronológicamente posterior al instante i2.
*/
bool posterior(const instante& i1, const instante& i2){
    if (anterior(i1,i2) || iguales(i1,i2)){
        return false;
    }
    return true;
}

/*Dados dos instantes i1 e i2, correspondientes al mismo año y mes, devuelve un entero con el
  número total de minutos comprendidos desde el instante i1 hasta el instante i2. Si ambos instantes
  i1 e i2 no pertenecen al mismo año y mes, entonces devolverá el entero de valor absoluto 100.000.
  El resultado será un número positivo si anterior(i1,i2) o será negativo si posterior(i1,i2).
*/
int diferencia(const instante& i1, const instante& i2){
    if(i1.mes == i2.mes && i1.anio == i2.anio){
        int min_1 = i1.minuto + i1.dia*24*60 + i1.hora*60; 
        int min_2 = i2.minuto + i2.dia*24*60 + i2.hora*60; 
        return (min_2 -  min_1);
   }
   else{
        return 100000;
   } 
}