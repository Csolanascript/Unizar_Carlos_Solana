/******************************************************************************\
 * Curso de Programación 1. Práctica 3
 * Autores: ¡PON AQUÍ TU NOMBRE!
 * Última revisión: 21 de octubre de 2021
 * Resumen: Programa que escribe en la pantalla el calendario de un determinado
 *          mes de un año, que se solicitan al usuario. El programa se asegura 
 *          de que el mes introducido está comprendido entre 1 y 12 y de que el 
 *          año es igual o posterior a 1900.
 *          Clase de problemas correspondiente al tema 7 (diseño modular y
 *          descendente) y 3ª práctica.
 * Nota: Leer el fichero README.md, que contiene instrucciones para su 
 *       compilación y ejecución.
\******************************************************************************/
#include <iostream>
#include <string>
#include <iomanip>
#include <fechas.hpp>
using namespace std;

/*
 * Pre:  1 ≤ mes ≤ 12
 * Post: Ha escrito en la pantalla el nombre (en mayúsculas) del mes
 *       correspondiente al valor del parámetro «mes».
 * Nota: Este código lo podrás comprender adecuadamente cuando hayamos visto
 *       los temas 9 (vectores) y 10 (cadenas de caracteres).
 */
void escribirNombreMes(unsigned mes) {
    const string NOMBRES_MES[12] = { "ENERO",
            "FEBRERO", "MARZO", "ABRIL", "MAYO", "JUNIO", "JULIO", "AGOSTO",
            "SEPTIEMBRE", "OCTUBRE", "NOVIEMBRE", "DICIEMBRE" };
    cout << NOMBRES_MES[mes - 1];
}

/*
* Pre: 1 ≤ mes ≤ 12
Post: le pide al usuario el mes*/
void pedirMes(int& mes){
    cout << "Introduzca el mes [1-12]: ";
    cin >> mes;
    if (mes<1 || mes>12){
        cout << "El mes debe estar comprendido entre 1 y 12: ";
        cin >> mes;
        }
}

/*
Pre: agno <= 1900
* Post: Le pide al usuario un nummero igual o posterior a 1900
*/
void pedirAgno(int& agno){
    cout << "Introduzca un año igual o posterior a 1900: ";
    cin >> agno;
    while(agno<1900){
        cout << "El año debe ser igual o posterior a 1900: ";
        cin >> agno;
        }
}
/*
* Pre: 1 ≤ mes ≤ 12
* Post: Ha escrito en la pantalla el nombre (en mayúsculas) del mes
* correspondiente al valor del parámetro «mes».
* Nota: Este código lo podrás comprender adecuadamente cuando hayamos visto
* los temas 9 (vectores) y 10 (cadenas de caracteres).
*/

/*
* Pre: 
* Post: Escribe en pantalla el año introducido por el usuario.
*/
void escribirAgno(unsigned agno){
    cout << right << agno << endl;
}
/*
* Pre: ---
* Post: Escribe la cabecera del calendario 
*/
void escribirCabecera(int mes, int agno){
escribirNombreMes(mes);
escribirAgno(agno);
cout << setw(4) << "L"
    << setw(4) << "M"
    << setw(4) << "X"
    << setw(4) << "J"
    << setw(4) << "V"
    << setw(4) << "S"
    << setw(4) << "D"
    << endl;
cout << "-----------------------------" << endl;
}
/*
* Pre: ---
* Post: Muestra en pantalla la hoja del calendario correspondiente 
*/

void presentarCalendario(int mes, int agno){

    int calendario;
        for( int sem=0 ; sem<7; sem++){
            for( int d=1; d<8; d++){
                calendario = sem * 7 + d - diaDeLaSemana( 1, mes, agno);
                if(calendario<=0){
                    cout << "    ";
                }
                else if(calendario <= diasDelMes(mes, agno)){
                        cout << setw(4) << calendario;
                }
                }
                cout << endl;
}
}

/*
 * ¡ESCRIBE LA ESPECIFICACIÓN!
 */
int main() {
int mes;
int agno;
pedirMes(mes);
pedirAgno(agno);
escribirCabecera(mes, agno);
presentarCalendario(mes, agno);
return 0;
}