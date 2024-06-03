//-------------------------------------------------------------------------------------------
// File:   practica_4.hpp
// Author: Carlos Solana Melero (872815) - Diego Mateo Lorente (873338)
// Date:   11-11-2023
// Coms:   Programa que, mediante la ayuda de monitores, consigue coger una tarea del búffer, asignarla a un controlador, procesarla y, finalmente,
//         muestra los resultados de la ejecución de las tareas por pantalla
//-------------------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <fstream>
#include <MultiBuffer.hpp>
#include <Matrix.hpp>
#include <Concurrent_MultiBuffer.hpp>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <iomanip>

using namespace std;
const int N_CONTROLLERS = 10;
const string nombreFichero = "tareas.txt";
const unsigned N_TIPOS_TAREA = 3;
//----------------------------------------------------
struct tarea {
    string tipoTarea;
    float cargaDeTrabajo;
};
//Pre: Necesita el struct MultiBuffer, todos los semáforos y la matriz de resultados
//Post: Lee del fichero y va repartiendo tareas, al final manda la tarea final y muestra por pantalla la matriz
void masterTask(Concurrent_MultiBuffer<tarea,N_CONTROLLERS>& CMB, Matrix<N_TIPOS_TAREA>& M, tarea& task) {
    ifstream f (nombreFichero);
    if(f.is_open()){    //Mientras pueda leer manda tareas
        string cargaTrabajo;
        while (getline(f,task.tipoTarea,',')){
            getline(f,cargaTrabajo,'\n');       //Lee y asigna cada componente
            task.cargaDeTrabajo = stod(cargaTrabajo);
            CMB.putTarea(task);
        }
        task.tipoTarea = "TF";                      //Cuando acabe de leer manda la tarea final
        CMB.putTF(task);                            //Asignamos la tarea final a todos los controladores
        CMB.espera();                               //Esperamos a que todos los controladores reciban la tarea final
        M.muestra();                                //Mostramos la matriz de resultados por pantalla
        f.close();
    }
    else {
        cerr << "Error al abrir el fichero \"" + nombreFichero + "\".\n";
    }
}
//---------------------------------------------------- 
//Pre:  0 <= id < N_CONTROLLERS
void controllerTask(unsigned int id, Concurrent_MultiBuffer<tarea,N_CONTROLLERS>& CMB, Matrix<N_TIPOS_TAREA>& M, tarea& task, int& n,int& i) {
    bool seguir = true;
    while (seguir) {
        tarea task_local = CMB.getTarea(id);
        if(task_local.tipoTarea == "TF"){          //Comprobamos que la tarea que recibe el controlador es la final para salir del bucle y terminar el proceso
            seguir = false;
        }
        else {
            srand(id + time(NULL) + i);
            i++;
            int numGen = rand() % (100 + 1);        //Generamos un número aleatorio entre 0 y 100 para aplicar probabilidad
            M.escribe(task_local.tipoTarea,numGen, task_local.cargaDeTrabajo,n,id);    //Escribimos los resultados en la matriz de resultados
        }
    }
}
//----------------------------------------------------
int main(int argc, char *argv[]) {
    Concurrent_MultiBuffer<tarea,N_CONTROLLERS> CMB;
    Matrix<N_TIPOS_TAREA> M;
    tarea task;
    int j = 0;
    thread P[N_CONTROLLERS + 1];        //De momento no inicializamos los threads
    int n = 0;
    for(unsigned i = 0; i < N_CONTROLLERS; i++) {
        P[i] = thread(&controllerTask, i, ref(CMB),ref(M), ref(task),ref(n),ref(j));
    }
    P[N_CONTROLLERS] = thread(&masterTask, ref(CMB),ref(M), ref(task));
    for(unsigned i = 0; i <= N_CONTROLLERS; i++) {
        P[i].join();
    }
    return 0;
}
