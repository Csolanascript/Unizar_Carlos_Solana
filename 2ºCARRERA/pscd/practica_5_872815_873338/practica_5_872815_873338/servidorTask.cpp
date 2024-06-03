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
#include <Concurrent_MultiBuffer.hpp>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <iomanip>
#include <vector>
#include <Socket.hpp>

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
void masterTask(Concurrent_MultiBuffer<tarea,N_CONTROLLERS>& CMB, tarea& task, Socket& sock) {
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
        string msg;
        for(unsigned i = 0; i < N_CONTROLLERS; i++) {
            sock.Receive(i,msg,3);
            if(msg != "END") {
                cerr << sock.error("Error en la llamada Receive");
                sock.Close(i);
                exit(1);
            }
        }
        f.close();
    }
    else {
        cerr << "Error al abrir el fichero \"" + nombreFichero + "\".\n";
    }
}
//---------------------------------------------------- 
//Pre:  0 <= id < N_CONTROLLERS
void controllerTask(unsigned int id, Concurrent_MultiBuffer<tarea,N_CONTROLLERS>& CMB, Socket& sock) {
    bool seguir = true;
    tarea task_local;
    string msg;
    while (seguir) {
        task_local = CMB.getTarea(id);
        msg = task_local.tipoTarea + "," + to_string(task_local.cargaDeTrabajo);
        sock.Send(id,msg);

        if(task_local.tipoTarea == "TF"){          //Comprobamos que la tarea que recibe el controlador es la final para salir del bucle y terminar el proceso
            seguir = false;
        }
    }
}

int main(int argc, char* argv[]) {
    Concurrent_MultiBuffer<tarea,N_CONTROLLERS> CMB;
    vector<thread> task;
    tarea tarea;
    Socket sock(stoi(argv[1]));
        // bind
    int socket_fd = sock.Bind();
    if (socket_fd == -1) {
        cerr << sock.error("Error en el bind");
        exit(1);
    }

    //listen
    int error_code = sock.Listen();
    if (error_code == -1) {
        cerr << sock.error("Error en el listen");
        // Cerramos el socket
        sock.Close();
        exit(1);
    }
    
    for(unsigned i = 0; i < N_CONTROLLERS; i++) {
        int new_client_fds = sock.Accept();
        if(new_client_fds == -1) {
            cerr << sock.error("Error en el accept");
            // Cerramos el socket
            sock.Close();
            exit(1);
        }
        else {
            task.push_back(thread(&controllerTask, i, ref(CMB), ref(sock)));
            cout << "ACEPTADO\n";
        }
    }
    task.push_back(thread(&masterTask, ref(CMB), ref(tarea), ref(sock)));

    for(unsigned i = 0; i <= N_CONTROLLERS; i++) {
        task[i].join();
    }
    error_code = sock.Close();
    if (error_code == -1) {
        cerr << sock.error("Error cerrando el socket del servidor");
    }
    return error_code;
}