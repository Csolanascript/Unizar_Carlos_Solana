#include <iostream>
#include <istream>
#include "GSenku.hpp"
#include <unistd.h>
#include <string>
using namespace std;

//Pre: true
// Post: lee la configuración y el estado del tablero del fichero de configuración que se le pasa como argumento
//      inicializando tablero y devolviendo true si todo ha ido bien y false si ha habido algún error
bool inicializarTablero(const string nombreFichero, tpTablero &tablero){
    string lineas,columnas,casilla;
    ifstream f;
    f.open(nombreFichero);
    if (f.is_open()) {
        f>>lineas;
        f>>columnas;
        tablero.nfils=stoi(lineas);
        tablero.ncols=stoi(columnas);
        for (unsigned i = 0; i < tablero.nfils; i++)
        {
            for (unsigned j = 0; j < tablero.ncols; j++)
            {
                f>>casilla;
                if(casilla=="-"){
                    tablero.matriz[i][j]=NO_USADA;
                } 
                else if(casilla=="x"){
                    tablero.matriz[i][j]=VACIA;
                }  
                else if(casilla=="o"){
                    tablero.matriz[i][j]=OCUPADA;
                }
                else{
                    cerr << "No se ha podido leer un caracter de \""<< nombreFichero << "\"" << endl;
                return false;
                }   
            }  
        }
        f.close();
        return true;
    }
        
    else{
        cerr << "No se ha podido acceder a \""<< nombreFichero << "\"" << endl;
        return false;
    
    } 
} 




// Pre: true 
// Post: lee los movimientos válidos del fichero que se le pasa como argumento 
//      inicializando la estructura y devolviendo true si todo ha ido bien y false si ha habido algún error
bool inicializarMovimientosValidos (const string nombreFichero, tpMovimientosValidos &movimientos){
    string caracter;
    ifstream f;
    f.open(nombreFichero);
    if (f.is_open()) {
        for (unsigned i = 0; i < 9; i++){
            f >> caracter;
            if(i<4){
                if(caracter =="+"){
                    movimientos.validos[i]  = true;
                } 
                if(caracter =="-"){
                    movimientos.validos[i]  = false;
                } 
            }
            if(i>4){
                if(caracter =="+"){
                    movimientos.validos[i-1]  = true;
                } 
                if(caracter =="-"){
                    movimientos.validos[i-1]  = false;  
                } 
            }
        }
        
    f.close();
    return true;
    }
    else {
        cerr << " mecago en mi vida No se ha podido acceder a \""<< nombreFichero << "\"" << endl;
        return false;
    }
    
}





// Pre: tablero contiene el estado actual de la ejecución de la búsqueda de la solución
// Post: Se ha mostrado el tablero por pantalla
void mostrarTablero(const tpTablero & tablero){
    cout << endl;
    for (unsigned i = 0; i < tablero.nfils; i++)
    {
        for (unsigned j = 0; j < tablero.ncols; j++)
        {
            if (tablero.matriz[i][j]==0){
                cout << "-";
            }
            else if(tablero.matriz[i][j]==1){
                cout << "x";
            }
            else if(tablero.matriz[i][j]==2){
                cout << "o";
            }
        }
        cout << endl;
    }
}



bool DirEsValido(unsigned direccion,tpMovimientosValidos movimientos,tpTablero &tablero,unsigned i, unsigned j){
        if(direccion==0){
            if(tablero.matriz[i-1][j-1]==OCUPADA && tablero.matriz[i-2][j-2]==VACIA){
                
                return true;
            }
        }
        else if(direccion==1){
            if(tablero.matriz[i-1][j]==OCUPADA && tablero.matriz[i-2][j]==VACIA){
                
                return true;
            }
        }
        else if(direccion==2){
            if(tablero.matriz[i-1][j+1]==OCUPADA && tablero.matriz[i-2][j+2]==VACIA){
               
                return true;
            }
        }
        else if(direccion==3){
            if(tablero.matriz[i][j-1]==OCUPADA && tablero.matriz[i][j-2]==VACIA){
                
                return true;
            }
        }
        else if(direccion==4){
            if(tablero.matriz[i][j+1]==OCUPADA && tablero.matriz[i][j+2]==VACIA){
                
                return true;
            }
        }
        else if(direccion==5){
            if(tablero.matriz[i+1][j-1]==OCUPADA && tablero.matriz[i+2][j-2]==VACIA){
               
                return true;
            }
        }
        else if(direccion==6){
            if(tablero.matriz[i+1][j]==OCUPADA && tablero.matriz[i+2][j]==VACIA){
                
                return true;
            }
        }
        else if(direccion==7){
            if(tablero.matriz[i+1][j+1]==OCUPADA && tablero.matriz[i+2][j+2]==VACIA){
                
                return true;
            }
        } 
        else{
            return false;
        }   
    return false;
} 




void Mover(tpListaMovimientos &solucionParcial,tpTablero &tablero,unsigned direccion,unsigned i, unsigned j,unsigned& dx, unsigned& dy){
        
        tablero.matriz[i][j]=VACIA;
        if(direccion==0){
            tablero.matriz[i-1][j-1]=VACIA;
            tablero.matriz[i-2][j-2]=OCUPADA;
            dx=j-2;
            dy=i-2;
           
            }
        else if(direccion==1){
            tablero.matriz[i-1][j]=VACIA;
            tablero.matriz[i-2][j]=OCUPADA;
            dx=j;
            dy=i-2;

    
        }
        else if(direccion==2){
            tablero.matriz[i-1][j+1]=VACIA;
            tablero.matriz[i-2][j+2]=OCUPADA;
            dx=j+2;
            dy=i-2;
            }
        else if(direccion==3){
            tablero.matriz[i][j-1]=VACIA;
            tablero.matriz[i][j-2]=OCUPADA;
            dx=j-2;
            dy=i;
                
            }
        
        else if(direccion==4){
            tablero.matriz[i][j+1]=VACIA;
            tablero.matriz[i][j+2]=OCUPADA;  
            dx=j+2;
            dy=i;
            }
        
        else if(direccion==5){
            tablero.matriz[i+1][j-1]=VACIA;
            tablero.matriz[i+2][j-2]=OCUPADA;
            dx=j-2;
            dy=i+2;
            }
        
        else if(direccion==6){
            tablero.matriz[i+1][j]=VACIA;
            tablero.matriz[i+2][j]=OCUPADA;
            dx=j;
            dy=i+2;
            }
        
        else if(direccion==7){
            tablero.matriz[i+1][j+1]=VACIA;
            tablero.matriz[i+2][j+2]=OCUPADA;
            dx=j+2;
            dy=i+2;
               
            }
        solucionParcial.movs[solucionParcial.numMovs].origen.y=j;
        solucionParcial.movs[solucionParcial.numMovs].origen.x=i;
        solucionParcial.movs[solucionParcial.numMovs].destino.y=dx;
        solucionParcial.movs[solucionParcial.numMovs].destino.x=dy;
        solucionParcial.numMovs++;
        }


void deshacerMov(tpTablero& tablero,unsigned direccion,unsigned i,unsigned j, tpListaMovimientos &solucionParcial){
        tablero.matriz[i][j]=OCUPADA;
        if(direccion==0){
            tablero.matriz[i-1][j-1]=OCUPADA;
            tablero.matriz[i-2][j-2]=VACIA;
            }
        else if(direccion==1){
            tablero.matriz[i-1][j]=OCUPADA;
            tablero.matriz[i-2][j]=VACIA;
    
        }
        else if(direccion==2){
            tablero.matriz[i-1][j+1]=OCUPADA;
            tablero.matriz[i-2][j+2]=VACIA;
            }
        else if(direccion==3){
            tablero.matriz[i][j-1]=OCUPADA;
            tablero.matriz[i][j-2]=VACIA;
                
            }
        
        else if(direccion==4){
            tablero.matriz[i][j+1]=OCUPADA;
            tablero.matriz[i][j+2]=VACIA;
               
            }
        
        else if(direccion==5){
            tablero.matriz[i+1][j-1]=OCUPADA;
            tablero.matriz[i+2][j-2]=VACIA;
               
            }
        
        else if(direccion==6){
            tablero.matriz[i+1][j]=OCUPADA;
            tablero.matriz[i+2][j]=VACIA;
               
            }
        
        else if(direccion==7){
            tablero.matriz[i+1][j+1]=OCUPADA;
            tablero.matriz[i+2][j+2]=VACIA;
               
            }
        solucionParcial.numMovs--;
    }


bool existe(const tpTablero tablero, const int direccion, const unsigned i, const unsigned j) {
    if((direccion==0 && (i<2 || j < 2) ) || 
        (direccion==1 && i<2) ||
        (direccion==2 && (i<2 || j>tablero.ncols-3)) || 
        (direccion==3 && j<2) || 
        (direccion==4 && j>tablero.ncols-3) ||
        (direccion==5 && (i>tablero.nfils-3 || j<2)) || 
        (direccion==6 && i>tablero.nfils-3) || 
        (direccion==7 && (i>tablero.ncols-3 ||j>tablero.ncols-3))){ 
        return false;    
    }    
    return true;
}


  
bool solucionado(tpTablero tablero){
    unsigned cont = 0;
    for (unsigned i = 0; i < tablero.ncols; i++){
        for (unsigned j = 0; j < tablero.nfils; j++){
            if(tablero.matriz[i][j]==OCUPADA){;
                cont++;
            }
        }
    }
    if(cont==1){
        return true;
    }
    else{
        return false;
    }
}
   





// Pre: tablero contiene el estado inicial del que se parte para la búsqueda, 
//      movimientosValidos contiene los movimientos que están permitidos, 
//      solucionParcial contiene la solución actual como lista de movimientos, En el tablero se han colocada las n primeras piezas de vEntrada, en la columnas indicadas respectivamente en vSalida
// Post: solucionParcial contendrá la lista de movimientos completa (si no se llega a una solución, estará vacía, numMovs == 0)
int buscaSolucion(tpTablero &tablero, const tpMovimientosValidos &movValidos, tpListaMovimientos &solucionParcial, const int retardo){
    unsigned dx,dy;
    cout << endl;
    if (solucionado(tablero)){
        //cout<<"esta solucionado";
        return 0; 
    }
    else{   
        for (unsigned i = 0; i < tablero.nfils; i++){
            for (unsigned j = 0; j < tablero.ncols; j++){   
                if (tablero.matriz[i][j]==OCUPADA){
                    
                    for (unsigned direccion = 0; direccion < 8; direccion++){
                        if(movValidos.validos[direccion]){
                            
                             if(existe(tablero,direccion,i,j)){
                                
                                if(DirEsValido(direccion,movValidos,tablero,i,j)){ 
                                                                        
                                    Mover(solucionParcial,tablero,direccion,i,j,dx,dy);  
                                    if(retardo>0){
                                    sleep(retardo);
                                    mostrarTablero(tablero);   
                                    }
                                    if(buscaSolucion(tablero,movValidos,solucionParcial,retardo)==-7){
                                        deshacerMov(tablero,direccion,i,j,solucionParcial);
                                    } 
                                    else{
                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                }                 
            }     
        }
        return -7;
    }
}           





// Pre: listaMovimientos contiene la lista de movimientos con la solucion 
// Post: escribe la lista de movimientos en el fichero que se le pasa como argumento siguiendo el 
//      formato especificado en el guión (si está vacía, se escribe un -1 en el fichero)
void escribeListaMovimientos (string nombreFichero, const tpListaMovimientos &solucion){
    ofstream f;
    f.open(nombreFichero);
    if (f.is_open()){
        if(solucion.numMovs != 0){
            for (unsigned i = 0; i < solucion.numMovs; i++){
                f<<solucion.movs[i].origen.x<<","<<solucion.movs[i].origen.y<<":"<<solucion.movs[i].destino.x<<","<<solucion.movs[i].destino.y << endl;   
            }   
        }
        else{
            f << -1;
        }
        f.close();
    }
}

