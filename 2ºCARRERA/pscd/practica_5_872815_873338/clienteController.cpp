//-------------------------------------------------------------------------------------------
// File:   practica_4.hpp
// Author: Carlos Solana Melero (872815) - Diego Mateo Lorente (873338)
// Date:   11-11-2023
// Coms:   Programa que, mediante la ayuda de monitores, consigue coger una tarea del búffer, asignarla a un controlador, procesarla y, finalmente,
//         muestra los resultados de la ejecución de las tareas por pantalla
//-------------------------------------------------------------------------------------------

#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <iomanip>
#include <fstream>
#include <vector>
#include <Socket.hpp>

using namespace std;

const int N_CONTROLLERS = 10;
const string nombreFichero = "tareas.txt";
const unsigned N_TIPOS_TAREA = 3;
const string MENS_FIN = "TF";
const string PEDIR_T = "GET_TASK";
//----------------------------------------------------
struct tarea {
    string tipoTarea;
    float cargaDeTrabajo;
};
//Pre:  0 <= id < N_CONTROLLERS
void controllerTask(unsigned int id,Socket& sock1,Socket& sock2, int& socket_fd_1, int& socket_fd_2) {
    
    // Buffer para recibir el mensaje
    int length = 100;
    int i = 0;
    string buffer;
    string msg;
    string cargaTrabajo;
    string resultado;
    string tipoTarea;
    bool out = false; // Inicialmente no salir del bucle
    int numGen;
    while(!out) {
        int snd_bytes = sock1.Send(id,PEDIR_T); // en vez de client_fd_1 seria id??
        if (snd_bytes == -1) {
            cerr << sock1.error("Error al mandar datos");
            // Cerramos los sockets
            sock1.Close(id); // en vez de client_fd_1 seria id??
        }

        int rcv_bytes = sock1.Receive(id,buffer,length); // en vez de client_fd_1 seria id??
        if (rcv_bytes == -1) {
            cerr << sock1.error("Error al recibir datos");
            // Cerramos los sockets
            sock1.Close(id); // en vez de client_fd_1 seria id??
        }
        
        istringstream ss(buffer);
        // Iterar hasta que no haya más trozos
        getline(ss, tipoTarea,',');
        if(tipoTarea != MENS_FIN){      
            getline(ss, cargaTrabajo);
            srand(id + time(NULL) + i);
            numGen = rand() % (100 + 1);
            i++;
            if(tipoTarea == "t1") {                         //Esperamos a tener permiso para escribir en la matriz
                if (numGen>=5){                         //5% de fallo
                    this_thread::sleep_for(chrono::milliseconds(stoi(cargaTrabajo)));
                    resultado = "OK";
                }
                else{
                    resultado = "NO_OK";
                }
            }
            if(tipoTarea == "t2") {
                if (numGen>=7){                         //7% de fallo
                    this_thread::sleep_for(chrono::milliseconds(stoi(cargaTrabajo)));
                    resultado = "OK";
                }
                else{
                    resultado = "NO_OK";
                }
            }
            if(tipoTarea == "t3") {                    //Aumentamos en 1 el número de veces ejecutado t3
                if (numGen>=10){                        //10% de fallo
                    this_thread::sleep_for(chrono::milliseconds(stoi(cargaTrabajo)));
                    resultado = "OK";
                }
                else{
                    resultado = "NO_OK";
                }
            }
            msg = tipoTarea + "," + resultado + "," + cargaTrabajo;
           
            snd_bytes = sock2.Send(id,msg);

            if (snd_bytes == -1) {
                cerr << sock2.error("Error al mandar datos");
                // Cerramos los sockets
                sock2.Close(id);
            }
        }
        else{
            out = true;
            snd_bytes = sock2.Send(id,MENS_FIN);
            if (snd_bytes == -1) {
                cerr << sock2.error("Error al mandar datos");
                // Cerramos los sockets
                sock2.Close(id);
            }
            rcv_bytes = sock2.Receive(id,buffer,length);
            if (rcv_bytes == -1) {
                cerr << sock2.error("Error al recibir datos");
                // Cerramos los sockets
                sock2.Close(id);// en vez de client_fd_1 seria id??
            }
            if(buffer != "END") {
                cerr << sock2.error("No ha llegado el mensaje esperado. Adios.");
                sock2.Close(id);
            }
            else {
                snd_bytes = sock1.Send(id,buffer);
                if(snd_bytes == -1) {
                    cerr << sock1.error("Error al mandar mensaje. Adios.");
                }
            }
        }
    }    
    sock1.Close(id);
    sock2.Close(id);
}

int main(int argc, char* argv[]) {
    const string MENS_FIN = "TF";
    // Dirección y número donde escucha el proceso servidor
    string SERVER_ADDRESS_1 = argv[1]; //alternativamente "127.0.0.1"
    string SERVER_ADDRESS_2 = argv[3];
    int SERVER_PORT_1 = stoi(argv[2]);
    int SERVER_PORT_2 = stoi(argv[4]);

    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket chan1(SERVER_ADDRESS_1, SERVER_PORT_1);
    Socket chan2(SERVER_ADDRESS_2, SERVER_PORT_2);
    // Conectamos con el servidor. Probamos varias conexiones
    int count = 0;
    int socket_fd_1;
    int socket_fd_2;
    do {
        // Conexión con el servidor
        socket_fd_1 = chan1.Connect();

        // Si error --> esperamos 1 segundo para reconectar
        if(socket_fd_1 == -1) {
            cout <<"No se ha podido conectar";
            this_thread::sleep_for(chrono::seconds(1));
        }
        else{
            cout << "HOLAS"<<count;
            do{
                cout <<"llega";
                socket_fd_2 = chan2.Connect();
                cout << socket_fd_2;
                // Si error --> esperamos 1 segundo para reconectar
                if(socket_fd_2 == -1) {
                    cout <<"ERROR\n";
                    this_thread::sleep_for(chrono::seconds(1));
                }
                else {
                  cout << "SUMO LA CUENTA\n";
                  count++;
                }
            }while(socket_fd_2 == -1);
        }
    }while(socket_fd_1 == -1 || count < N_CONTROLLERS);
    for (int i = 0; i < N_CONTROLLERS  ; i++)
    {
        thread(&controllerTask,i,ref(chan1),ref(chan2),ref(socket_fd_1),ref(socket_fd_2));
    }
    for(unsigned i = 0; i <= N_CONTROLLERS; i++) {
        task[i].join();
    }
    int error_code = chan1.Close();
    if (error_code == -1) {
        cerr << chan1.error("Error cerrando el socket del servidor");
        return error_code;
    }
    error_code = chan2.Close();
    if (error_code == -1) {
        cerr << chan2.error("Error cerrando el socket del servidor");
    }
    return error_code;
}