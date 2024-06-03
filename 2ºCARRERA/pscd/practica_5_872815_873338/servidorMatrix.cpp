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
#include <Matrix.hpp>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <vector>
#include <iomanip>
#include <sstream>
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
//---------------------------------------------------- 
//Pre:  0 <= id < N_CONTROLLERS
void escribir(unsigned int id, Matrix<N_TIPOS_TAREA>& M,Socket& chan) {
    string MENS_FIN = "TF";
    // Buffer para recibir el mensaje
    int length = 100;
    string buffer;
    string resultado;
    string cargaTrabajo;
    string tipoTarea;
    int rcv_bytes;
    int send_bytes;


    bool out = false; // Inicialmente no salir del bucle
    while(!out) {
        // Recibimos el mensaje del cliente
        rcv_bytes = chan.Receive(id,buffer,length);

        if (rcv_bytes == -1) {
            cerr << chan.error("Error al recibir datos1");
            // Cerramos los sockets
        }
        if(buffer != MENS_FIN){      
            out = true;
        }
        else{
            istringstream ss(buffer);
            // Iterar hasta que no haya más trozos
            getline(ss, tipoTarea, ',');   
            getline(ss, resultado, ',');
            getline(ss,cargaTrabajo);
            M.escribe(tipoTarea, resultado, stod(cargaTrabajo));    //Escribimos los resultados en la matriz de resultados
        }
    }
    send_bytes = chan.Send(id, "END");
    if(send_bytes == -1) {
        cerr << chan.error("Error al enviar datos2");
        // Cerramos los sockets
    }
    chan.Close(id);
}


int main(int argc, char* argv[]) {
    Matrix<N_TIPOS_TAREA> M;
    int SERVER_PORT = 3269; 
    vector<thread> cliente;
    cout << SERVER_PORT;
    bool fin = false;

    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket chan(SERVER_PORT);

    // bind
    int socket_fd = chan.Bind();
    if (socket_fd == -1) {
        cerr << chan.error("Error en el bind");
        exit(1);
    }
    cout << "A\n";

    //listen
    int error_code = chan.Listen();
    if (error_code == -1) {
        cerr << chan.error("Error en el listen");
        // Cerramos el socket
        chan.Close();
        exit(1);
    }
    //para desbloquear servidor y terminar
    cout << "B\n";
    for(unsigned i = 0; i < N_CONTROLLERS; i++) {
        cout << "d\n";
        int new_client_fds = chan.Accept();
        cout << new_client_fds;
        cout << "c\n";
        if(new_client_fds == -1) {
            cerr << chan.error("Error en el accept");
            // Cerramos el socket
            chan.Close();
            exit(1);
        }
        else {
            cout <<"e\n";
            cliente.push_back(thread(&escribir,ref(i),ref(M),ref(chan)));
            cout <<"g\n";
            cout << "Nuevo cliente " + to_string(i) + " aceptado" + "\n";
        }
    }


    //¿Qué pasa si algún thread acaba inesperadamente?
    for (int i=0; i<cliente.size(); i++) {
        cliente[i].join();
    }
    M.muestra();
    //FALTA ENVIAR EL MENSAJE END A CADA CONTROLADOR.
    // Cerramos el socket del servidor
    error_code = chan.Close();
    if (error_code == -1) {
        cerr << chan.error("Error cerrando el socket del servidor");
    }


    return error_code;
}
//-------------------------------------------------------------
