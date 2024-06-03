//-------------------------------------------------------------------------------------------
// File:   practica_3.cpp
// Author: Carlos Solana Melero (872815) -- Diego Mateo Lorente (873338)
// Date:   14-11-2023
//-------------------------------------------------------------------------------------------

#include "actividad.hpp"
#include "periodo.hpp"
#include "coleccionSeg.hpp"
#include <fstream>
#include <iomanip>

//lee una fecha del fichero f y crea el dato
bool leerFecha(ifstream &f, instante &inicio){
                string dia,mes,agno,hora,minuto;
                getline(f,agno,'-');
                getline(f,mes,'-');  
                getline(f,dia,' '); 
                getline(f,hora,':');
                getline(f,minuto);
                return crearInstante(stoi(dia),stoi(mes),stoi(agno),stoi(hora),stoi(minuto),inicio);        
}
//lee una actividad del fichero f y crea el dato
void leerActividad(ifstream &f, actividad &a){
                string nombre,tEstimado,descripcion;
                getline(f,nombre);
                getline(f,tEstimado);
                getline(f,descripcion);
                crearActividad(nombre,descripcion, stod(tEstimado),a);

}
int main(){
    const string nombreFicheroEntrada = "entrada_prueba.txt";
    const string nombreFicheroSalida = "salida.txt";
    coleccionSeg<periodo,actividad> h; //usa coleccionSeg
    instante inicio,final;
    periodo p;
    string nombre;
    string act_name;
    actividad a;
    crear(h);
    ifstream f (nombreFicheroEntrada);
    if(f.is_open()){ //abre fichero
        ofstream s (nombreFicheroSalida);
        if(s.is_open()){
            string instruccion,basura;
            while(getline(f,instruccion,'\n')){
                if(instruccion == "A"){ //instrucción A
                    if(leerFecha(f,inicio)){
                        if(leerFecha(f,final)){
                            if(crearPeriodo(inicio,final,p)){
                                leerActividad(f,a);
                                if(!aniadir(h,p,a)){//añade a la colección
                                     s << "NO ";
                                } 
                                //escribe en el fichero de salida los resultados
                                s << "INTRODUCIDO: ";
                                s << "[ " << anio(inicio) << "-" << setw(2) << setfill('0') <<  mes(inicio) << "-" << setw(2) << setfill('0')<< dia(inicio) << " " << setw(2) << setfill('0')<< hora(inicio) << ":" << setw(2) << setfill('0')<< minuto(inicio);
                                s << " ;;; " << anio(final) << "-" << setw(2) << setfill('0')<< mes(final) << "-" << setw(2) << setfill('0')<< dia(final) << " " << setw(2) << setfill('0')<< hora(final) << ":" << setw(2) << setfill('0')<< minuto(final) << " ]";
                                s << " ::: " << obtenerNombre(a) << " ::: (" << obtenerTiempoEstimado(a) << ") ::: " <<obtenerDescripcion(a) << endl;
                            }
                            else{
                                s << "PERIODO NO VALIDO" << endl;
                            }
                        }
                        else{
                            s << "INSTANTE NO VALIDO" << endl << "PERIODO NO VALIDO" << endl;
                        }
                    }
                    else{
                        s << "INSTANTE NO VALIDO" << endl << "PERIODO NO VALIDO" << endl;
                    }
                }
                else if(instruccion == "C"){
                    if (leerFecha(f,inicio)){
                        if(leerFecha(f,final)){
                            if(crearPeriodo(inicio,final,p)){
                                leerActividad(f,a);
                                if(!actualizar(h,p,a)){//actualiza la colección
                                     s << "NO ";
                                }
                                //escribe en el fichero de salida los resultados 
                                s << "CAMBIADO: ";
                                s << "[ " << anio(inicio) << "-" << setw(2) << setfill('0') << mes(inicio) << "-" << setw(2) << setfill('0') << dia(inicio) << " " << setw(2) << setfill('0') << hora(inicio) << ":" << setw(2) << setfill('0') << minuto(inicio);
                                s << " ;;; " << anio(final) << "-" << setw(2) << setfill('0') << mes(final) << "-" << setw(2) << setfill('0') << dia(final) << " " << setw(2) << setfill('0') << hora(final) << ":" << setw(2) << setfill('0') << minuto(final) << " ]";
                                s << " ::: " << obtenerNombre(a) << " ::: (" << obtenerTiempoEstimado(a) << ") ::: " <<obtenerDescripcion(a) << endl;
                            }
                            else{
                                s << "PERIODO NO VALIDO" << endl;
                            }
                        }
                        else{
                            s << "INSTANTE NO VALIDO" << endl << "PERIODO NO VALIDO" << endl;
                        }
                    }
                    else{
                        s << "INSTANTE NO VALIDO" << endl << "PERIODO NO VALIDO" << endl;
                    }
                }
                else if(instruccion == "O"){
                    if(leerFecha(f,inicio)) {
                        if(leerFecha(f,final)) {
                            if(crearPeriodo(inicio,final,p)) {
                                if(obtener(h,p,a)) {//si hay un segmento en la colección igual que el que ha leído lo muestra por pantalla 
                                    //escribe en el fichero de salida los resultados
                                    s << "LOCALIZADO: [ " << anio(inicio) << "-" << setw(2) << setfill('0')<< mes(inicio) << "-" << setw(2) << setfill('0')<< dia(inicio) << " " << setw(2) << setfill('0')<< hora(inicio) << ":" << setw(2) << setfill('0')<< minuto(inicio);
                                    s << " ;;; " << anio(final) << "-" << setw(2) << setfill('0')<< mes(final) << "-" << setw(2) << setfill('0')<< dia(final) << " " << setw(2) << setfill('0')<< hora(final) << ":" << setw(2) << setfill('0')<< minuto(final) << " -> " << obtenerDuracion(p);
                                    s << " ] ::: " << obtenerNombre(a) << " ::: (" << obtenerTiempoEstimado(a) << ") ::: " << obtenerDescripcion(a) << endl; 
                                }
                                else {
                                    s << "NO LOCALIZADO: [ " << anio(inicio) << "-" << setw(2) << setfill('0')<< mes(inicio) << "-" << setw(2) << setfill('0')<< dia(inicio) << " " << setw(2) << setfill('0')<< hora(inicio) << ":" << setw(2) << setfill('0')<< minuto(inicio);
                                    s << " ;;; " << anio(final) << "-" << setw(2) << setfill('0')<< mes(final) << "-" << setw(2) << setfill('0')<< dia(final) << " " << setw(2) << setfill('0')<< hora(final) << ":" << setw(2) << setfill('0')<< minuto(final) << " ]" << endl;
                                }
                            }
                            else {
                                s << "PERIODO NO VALIDO" << endl;
                            }
                        }
                        else {
                            s << "INSTANTE NO VALIDO" << endl << "PERIODO NO VALIDO" << endl;
                        }
                    }
                    else {
                        s << "INSTANTE NO VALIDO" << endl << "PERIODO NO VALIDO" << endl;
                    }
                }
                else if(instruccion == "S"){
                    if(leerFecha(f,inicio)) {
                        if(leerFecha(f,final)) {
                            if(crearPeriodo(inicio,final,p)) {
                                if(!seSolapa(h,p)) {//si hay algún elemento solapado con el leído lo escribe en el fichero
                                    s << "NO ";
                                }
                                //escribe en el fichero de salida los resultados
                                s << "SOLAPE con: [ " << anio(inicio) << "-" << setw(2) << setfill('0')<< mes(inicio) << "-" << setw(2) << setfill('0')<< dia(inicio) << " " << setw(2) << setfill('0')<< hora(inicio) << ":" << setw(2) << setfill('0')<< minuto(inicio);
                                s << " ;;; " << anio(final) << "-" << setw(2) << setfill('0')<< mes(final) << "-"<< setw(2) << setfill('0') << dia(final) << " " << setw(2) << setfill('0')<< hora(final) << ":" << setw(2) << setfill('0')<< minuto(final) << " ]" << endl;
                            }
                            else {
                                s << "PERIODO NO VALIDO" << endl;
                            }
                        }
                        else {
                            s << "INSTANTE NO VALIDO" << endl << "PERIODO NO VALIDO" << endl;
                        }
                    }
                    else {
                        s << "INSTANTE NO VALIDO" << endl << "PERIODO NO VALIDO" << endl;
                    }
                }
                else if(instruccion == "B"){
                    if(leerFecha(f,inicio)) {
                        if(leerFecha(f,final)) {
                            if(crearPeriodo(inicio,final,p)) {
                                if(!borrar(h,p)) {
                                    s << "NO ";
                                }
                                //escribe en el fichero de salida los resultados
                                s << "BORRADO: [ " << anio(inicio) << "-" << setw(2) << setfill('0')<< mes(inicio) << "-" << setw(2) << setfill('0')<< dia(inicio) << " " << setw(2) << setfill('0')<< hora(inicio) << ":" << setw(2) << setfill('0')<< minuto(inicio);
                                s << " ;;; " << anio(final) << "-"<< setw(2) << setfill('0') << mes(final) << "-" << setw(2) << setfill('0')<< dia(final) << " " << setw(2) << setfill('0')<< hora(final) << ":" << setw(2) << setfill('0')<< minuto(final) << " ]" << endl;
                            }
                            else {
                                s << "PERIODO NO VALIDO" << endl;
                            }
                        }
                        else {
                            s << "INSTANTE NO VALIDO" << endl << "PERIODO NO VALIDO" << endl;
                        }
                    }
                    else {
                        s << "INSTANTE NO VALIDO" << endl << "PERIODO NO VALIDO" << endl;
                    }

                }
                else if(instruccion == "LA"){
                    int suma = 0;
                    getline(f,act_name);
                    s << "****ACTIVIDAD: " << act_name << endl;
                    iniciarIterador(h);
                    do {
                        if(siguienteVal(h,a)) {
                            if(act_name == obtenerNombre(a)) {
                                if(siguienteSegm(h,p)) {
                                    inicio = obtenerInicio(p);
                                    final = obtenerFinal(p);
                                    //escribe en el fichero de salida los resultados
                                    s << "[ " << anio(inicio) << "-" << setw(2) << setfill('0')<< mes(inicio) << "-" << setw(2) << setfill('0')<< dia(inicio) << " " << setw(2) << setfill('0')<< hora(inicio) << ":" << setw(2) << setfill('0')<< minuto(inicio);
                                    s << " ;;; " << anio(final) << "-" << setw(2) << setfill('0')<< mes(final) << "-" << setw(2) << setfill('0')<< dia(final) << " " << setw(2) << setfill('0')<< hora(final) << ":" << setw(2) << setfill('0')<< minuto(final);
                                    s << " -> " << obtenerDuracion(p);
                                    s << " ]" << endl;
                                    suma += obtenerDuracion(p);
                                }
                            }
                        }    
                    }while(avanza(h));
                    s << "****TOTAL dedicados: " << suma << endl;
                }
                else if(instruccion == "LT"){
                    s << "-----LISTADO: " << tamagno(h) << endl;
                    iniciarIterador(h);
                    do{
                        if(siguienteSegm(h,p)){
                            if(siguienteVal(h,a)){
                                inicio = obtenerInicio(p);
                                final = obtenerFinal(p);
                                //escribe en el fichero de salida los resultados
                                s << "[ " << anio(inicio) << "-" << setw(2) << setfill('0')<< mes(inicio) << "-" << setw(2) << setfill('0')<< dia(inicio) << " " << setw(2) << setfill('0')<< hora(inicio) << ":" << setw(2) << setfill('0')<< minuto(inicio);
                                s << " ;;; " << anio(final) << "-" << setw(2) << setfill('0')<< mes(final) << "-" << setw(2) << setfill('0')<< dia(final) << " " << setw(2) << setfill('0')<< hora(final) << ":" << setw(2) << setfill('0')<< minuto(final);
                                s << " -> " << obtenerDuracion(p);
                                s << " ] ::: " << obtenerNombre(a) << " ::: (" << obtenerTiempoEstimado(a) << ") ::: " << obtenerDescripcion(a) << endl;  
                            }
                        }
                    } while(avanza(h));
                    s << "-----" << endl;
                }
            }
            s.close();
        }
        else{
            cerr << "Error al abrir el fichero de salida \"" + nombreFicheroSalida + "\".\n";   
        }
        f.close();
    }
    else {
        cerr << "Error al abrir el fichero de entrada\"" + nombreFicheroEntrada + "\".\n";
    }

}