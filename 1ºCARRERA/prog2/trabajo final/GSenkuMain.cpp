#include "iostream"
#include <GSenku.hpp>
//#include <GSenku.cpp>
#include <string>
using namespace std;

int main(int argc, char* argv[]){
    
    
    string ficheroTablero=argv[1];
    string ficheroMovimientos=argv[2];

    int retardo=stoi(argv[3]);
    
    
    string ficheroSolucion="resultado.txt";
    
    if(argc==5){
        ficheroSolucion=argv[4];
    }
    


    tpTablero tablero;
    tpMovimientosValidos validos;
    tpListaMovimientos solucion;
    
    
    inicializarTablero(ficheroTablero,tablero);

    mostrarTablero(tablero);
    
    inicializarMovimientosValidos(ficheroMovimientos,validos);
    
    buscaSolucion(tablero,validos,solucion,retardo);
    
    escribeListaMovimientos(ficheroSolucion,solucion);

}
