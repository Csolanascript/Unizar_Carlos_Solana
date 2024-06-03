/******************************************************************************\
 * Programación 1. Práctica 4
 * Autores: ------------- carlos solana -------------
 * Ultima revisión: 
 * Resumen: 
\******************************************************************************/
#include<iostream>
#include<naturales-grandes.hpp>
#include<cmath>
using namespace std;
int main() {
    unsigned numDigs = 0, digsAux = 0, potencia[NUM_DIGITOS], contador = 0, resultado[NUM_DIGITOS];
    //pedimos los datos
    cout << "Numero de dígitos (0 o negativo para acabar): ";
    cin >> numDigs;
    // iteramos hasta que el numero de cifras sea >0
    while (numDigs > 0){
    contador = 0;
    digsAux = 0;
    // convertimos el numero a 1
    convertir(1,potencia);
    // iteramos hasta que nuestra potencia de 2 el numero de cifras que necesitamos
    while(digsAux < numDigs){
        //sumamos las potencias anteriores para conseguir la siguiente
          sumar(potencia,potencia,resultado);
          digsAux = numCifras(resultado);
          copiar(resultado,potencia);
          contador++;
    }

escribirEnPantalla(resultado);
cout << " es 2 elevado a la " << contador << " potencia" << endl;
cout << "y es la primera potencia de 2 de " << numDigs << endl;
cout << "Numero de dígitos (0 o negativo para acabar): ";
cin >> numDigs;}
    return 0;
}
