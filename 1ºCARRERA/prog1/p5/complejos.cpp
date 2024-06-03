/******************************************************************************\
 * Programación 1. Práctica 5
 * Autor: carlos solana
 * Ultima revisión: ¡¡¡¡¡¡¡¡¡¡¡!!!!!!!!!
 * Resumen: Fichero de implementación del módulo principal del programa que 
 *          trabaja con números complejos.
 * Nota: El código de este programa no está repartido en varios módulos, por lo
 *       que se puede utilizar la extensión Code Runner de VSC para ejecutarlo.
 *       No obstante, se ha configurado el fichero «Makefile» para poder 
 *       compilarlo y se han definido tareas de VSC para compilarlo, ejecutarlo 
 *       y depurarlo.
 *       Para compilarlo, se puede ejecutar el comando
 *           make complejos
 *       o, en Windows,
 *           mingw32-make complejos
 *       o ejecutar la tarea "Compilar «complejos»" de VSC
 * 
 *       Para ejecutarlo, una vez compilado, se puede ejecutar el comando
 *           bin/complejos
 *       o, en Windows,
 *           bin\complejos.exe
 *       o ejecutar la tarea "Ejecutar «complejos»" de VSC
 *       o compilarlo y ejecutarlo con la extensión Code Runner (Ctrl+Alt+N).
\******************************************************************************/
#include <iostream>
#include <cmath>
using namespace std;
//declaro las estructura "complejos"
struct complejos{
        double real; //parte real
        double imaginaria; //parte imaginaria
    };
/*
 * Pre:  tengo un numero de tipo complejo en la variable num
 * Post: muestro el numero complejo en pantalla
 */
void mostrar(complejos num){
    
            if(num.imaginaria > 0){
                cout<<num.real<<" +"<<num.imaginaria<<"i";}
            else{
                cout<<num.real<<" "<<num.imaginaria<<"i";}

     
} 
/*
 * Pre:  tengo un vector de 5 componentes de tipo complejo
 * Post: calcula y muestra en pantalla el maximo y el minimo de las 5 componetes, itera calculando el módulo de cada una de ellas y muestra la mayor y la menor respeectivamente
 */
void maxMin(complejos p[5]){
    complejos max = p[0];
    complejos min = p[0];
    double mod,modAux = sqrt(pow(p[0].real,2) + pow(p[0].imaginaria,2));

    cout<<"Numeros complejos de mayor y menor modulo, respectivamente:"<<endl;

    for(unsigned i = 1;i < 5;i++){
        mod = sqrt(pow(p[i].real,2) + pow(p[i].imaginaria,2));
        if( mod > modAux){
            modAux = mod;
            max = p[i];}
        }
    mostrar(max);
    cout<<" y ";
    modAux = sqrt(pow(p[0].real,2) + pow(p[0].imaginaria,2));

    for(unsigned i = 1;i < 5;i++){
        mod = sqrt(pow(p[i].real,2) + pow(p[i].imaginaria,2));
        if( mod < modAux){
            modAux = mod;
            min = p[i];}
        }
    mostrar(min);
        
}
/*
 * Pre: tengo un vector de 5 componentes de tipo complejo
 * Post: itera entre las 5 generando un nuevo numero complejo que es la suma de todas las componentes y la muestra en pantalla
 */
void suma(complejos p[5]){
    complejos suma;
     for(int i = 0; i<5 ; i++){
        suma.real += p[i].real;
        suma.imaginaria += p[i].imaginaria;
     }
     cout<<"Suma:"<<endl;
     mostrar(suma);

}
int main(){
    double X = 1,Y = 0, random[9];
    complejos num[5];
    //pide los limites del intervalo y comprueba que son correctos
    cout <<"Introduce los limites del intervalo (x < y): ";
    cin >> X >> Y;

    while (X > Y){
    cout <<"Introduce los limites del intervalo (x < y): ";
    cin >> X >> Y;
    cout<<endl<<endl;
    }
    //genera 5 numeros aleatorios dentro de los limites y los almacena en el vector
    for(unsigned i=0; i<5; i++){
        num[i].real = double(rand())/(RAND_MAX*(Y - X))+X;
        num[i].imaginaria = double(rand())/(RAND_MAX*(Y - X))+X;
    }
    //itera el vector para mostrar sus componentes
    cout<<"Numeros complejos generados:"<<endl;

    for( int i = 0; i<4; i++){
        mostrar(num[i]);
        cout<<",  ";
    }
    mostrar(num[4]);
    cout<<endl<<endl;
    // usa las funciones maxMin y suma con el vector
    maxMin(num);

    cout<<endl<<endl;
    suma(num);
    return 0;

}

