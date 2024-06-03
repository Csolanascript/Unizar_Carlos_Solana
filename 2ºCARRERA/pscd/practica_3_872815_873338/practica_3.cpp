//Autores: Carlos Solana Melero 872815
//	       Diego Mateo Lorente 873338
#include <iostream>
#include <string>
#include <fstream>
#include <librerias/Semaphore_V4/Semaphore_V4.hpp>
#include <librerias/MultiBuffer/MultiBuffer.hpp>
#include <thread>
#include <random>
#include <chrono>
#include <iomanip>

using namespace std;
const int N_CONTROLLERS = 10;
const string nombreFichero = "tareas.txt";
//----------------------------------------------------
struct tarea {
    string tipoTarea;
    float cargaDeTrabajo;
};
//Pre: Necesita el struct MultiBuffer, y todos los semáforos
//La función pasarTestigo comprueba si se ha cumplido la condicición de algún semáforo y si es cierta y tine permiso pasa el testigo a esa función, en caso contrario desbloquea un proceso cualquiera
void pasarTestigo(MultiBuffer<tarea,N_CONTROLLERS>& mBT,Semaphore& testigo, Semaphore& s1, int& d1, 
                Semaphore& s2, int& d2, Semaphore* s3[], int d3[]){
    //comprueba la condición de s1
    if(d1>0 && mBT.numberNews() < N_CONTROLLERS){
        d1--;
        s1.signal();
    }
    //comprueba la condición de s2
    else if(d2>0 && mBT.numberNews() == 0){
        d2--;
        s2.signal();
    }
    //comprueba la condición de s3 buscando entre todas sus componentes
    else {
        int i = 0;
        bool salir = false;
        while(!salir && i < N_CONTROLLERS) {
            if(d3[i] > 0 && mBT.isNew(i)) {
                salir = true;
                d3[i]--;
                s3[i]->signal();
            }
            else {
                i++;
            }
        }
        //si no se cumple ninguna condición devuelve el control a los controladores para que esperen hasta cumplir una condición y poner un permiso en ese caso
        if(!salir) {
            testigo.signal();
        }
    }
}
//Pre: Necesita el struct MultiBuffer, todos los semáforos y la matriz de resultados
//Post: Lee del fichero y va repartiendo tareas, al final manda la tarea final y muestra por pantalla la matriz
void masterTask(MultiBuffer<tarea,N_CONTROLLERS>& mBT, tarea& task, Semaphore& testigo, Semaphore& s1, int& d1, 
                Semaphore& s2, int& d2, Semaphore* s3[], int d3[], double resultados[][3]) {
    ifstream f (nombreFichero);
    if(f.is_open()){    //Mientras pueda leer manda tareas
        string cargaTrabajo;
        while (getline(f,task.tipoTarea,',')){
            getline(f,cargaTrabajo,'\n');       //Lee y asigna cada componente
            task.cargaDeTrabajo = stod(cargaTrabajo);
            testigo.wait();                             //Espera a recibir el testigo
            if(mBT.numberNews() >= N_CONTROLLERS) {     //Si hay algún proceso libre
                d1++;                                   //Aumenta el numero de permisos
                testigo.signal();                       //Da el testigo a otro proceso
                s1.wait();                              //Espera a recibir un signal de pasar testigo
            }
            unsigned i = 0;
            bool encontrado = false;
            while(!encontrado) {                        //Busca un proceso libre y manda el dadto a través del multi buffer
                if(!mBT.isNew(i)) {
                    mBT.put(task,i);
                    encontrado = true;
                }
                i++;
            }
            pasarTestigo(mBT, testigo, s1, d1, s2, d2, s3, d3);
        }
        task.tipoTarea = "TF";                      //Cuando acabe de leer manda la tarea final
        testigo.wait();                             //Espera a recibir el testigo
        if(mBT.numberNews() != 0) {                 //Si hay algún proceso libre
            d2++;                                   //Aumenta el numero de permisos
            testigo.signal();                       //Da el testigo a otro proceso
            s2.wait();                              //Espera a recibir un signal de pasar testigo
        }
        for(unsigned i = 0; i < N_CONTROLLERS; i++) {//Manda la tarea final
            mBT.put(task,i);
        }
        pasarTestigo(mBT, testigo, s1, d1, s2, d2, s3, d3);
        testigo.wait();                              //Espera a recibir el testigo
        if(mBT.numberNews() != 0) {                  //Si hay algún proceso libre
            d2++;                                    //Aumenta el número de procesos esperando a la guarda
            testigo.signal();                        //Da el testigo a otro proceso
            s2.wait();                               //Espera a recibir un signal de pasarTestigo
        }
        pasarTestigo(mBT, testigo, s1, d1, s2, d2, s3, d3);
        cout << endl;
        for (int i = 0; i < 3; i++){                //Muestra la matriz por pantalla
            for (int t = 0; t < 3; t++)
            {
            cout<< left << setw(6) << resultados[i][t];
            }
            cout << endl;
        }
        f.close();
    }
    else {
        cerr << "Error al abrir el fichero \"" + nombreFichero + "\".\n";
    }
}
//---------------------------------------------------- 
//Pre:  0 <= id < N_CONTROLLERS
void controllerTask(unsigned int id, MultiBuffer<tarea,N_CONTROLLERS>& mBT, tarea& task, Semaphore& testigo, 
                    Semaphore& s1, int& d1, Semaphore& s2, int& d2, Semaphore* s3[], int d3[], Semaphore& s4, double resultados[][3]) {
    bool seguir = true;
    while (seguir) {
        testigo.wait();                             //Espera a recibir el testigo
        if(!mBT.isNew(id)) {                        //Si el proceso id del controlador está libre
            d3[id]++;                               //Aumenta el número de procesos esperando a la guarda
            testigo.signal();                       //Da el testigo a otro proceso
            s3[id]->wait();                         //Espera a recibir un signal de pasarTestigo
        }
        tarea task_local = mBT.get(id);             //Guardamos en task_local la tarea que debe cumplir el controlador
        pasarTestigo(mBT, testigo, s1, d1, s2, d2, s3, d3);
        if(task_local.tipoTarea == "TF"){           //Comprobamos que la tarea que recibe el controlador es la final para salir del bucle y terminar el proceso
            seguir = false;
        }
        else {
            int numGen = rand() % (100 + 1);        //Generamos un número aleatorio entre 0 y 100 para aplicar probabiidad
            if (task_local.tipoTarea =="t1"){
                s4.wait();                          //Esperamos a tener permiso para escribir en la matriz
                resultados[0][0]++;                 //Aumentamos en 1 el número de veces ejecutado t1
                if (numGen>=5){                     //5% de fallo
                    this_thread::sleep_for(chrono::milliseconds(int(task_local.cargaDeTrabajo)));
                    resultados[0][2] = resultados[0][2] + task_local.cargaDeTrabajo;        //Sumamos el tiempo de carga de trabajo de t1
                    resultados[0][1]++;                                                     //Aumentamos en 1 el número de éxitos en t1
                }
                s4.signal();                        //Devolvemos el permiso de semáforo
            }
            if (task_local.tipoTarea =="t2"){
                s4.wait();                          //Esperamos a tener permiso para escribir en la matriz
                resultados[1][0]++;                 //Aumentamos en 1 el número de veces ejecutado t2
                if (numGen>=7){                     //7% de fallo
                    this_thread::sleep_for(chrono::milliseconds(int(task_local.cargaDeTrabajo)));
                    resultados[1][1]++;                                                     //Aumentamos en 1 el número de éxitos en t2
                    resultados[1][2] = resultados[1][2] + task_local.cargaDeTrabajo;        //Sumamos el tiempo de carga de trabajo de t2
                }
                s4.signal();                        //Devolvemos el permiso de semáforo
            }
            if (task_local.tipoTarea =="t3"){
                s4.wait();                          //Esperamos a tener permiso para escribir en la matriz
                resultados[2][0]++;                 //Aumentamos en 1 el número de veces ejecutado t3
                if (numGen>=10){                    //10% de fallo
                    this_thread::sleep_for(chrono::milliseconds(int(task_local.cargaDeTrabajo)));
                    resultados[2][1]++;                                                     //Aumentamos en 1 el número de éxitos en t3
                    resultados[2][2] = resultados[2][2] + task_local.cargaDeTrabajo;        //Sumamos el tiempo de carga de trabajo de t3
                }
                s4.signal();                        //Devolvemos el permiso de semáforo
            }
        }
    }
}
//----------------------------------------------------
int main(int argc, char *argv[]) {

    srand(time(NULL));
    MultiBuffer<tarea,N_CONTROLLERS> mBT;   //multi-buffer de tareas
    Semaphore testigo(1);                  //testigo
    Semaphore s1(0);                       //s1 : await nNews < N
    int d1 = 0;
    Semaphore s2(0);                       //s2 : await nNews = 0
    int d2 = 0;
    Semaphore *s3[N_CONTROLLERS];           //s3 : await isNew(i)
    int d3[N_CONTROLLERS];
    Semaphore s4(1);                       //semáforo para dar permisos de escritura en la matriz de resultados en controllerTask
    for(unsigned i = 0; i < N_CONTROLLERS; i++) {
        d3[i] = 0;
        s3[i] = new Semaphore(0);
    }
    tarea task;
    thread P[N_CONTROLLERS + 1];            //Declaramos los hilos que se van a usar (de momento no los inicializamos)
    double resultados[3][3];
    for (int i = 0; i < 3; i++)
    {
        for (int t = 0; t < 3; t++)
        {
            resultados[i][t]=0;
        }
    }
    for(unsigned i = 0; i < N_CONTROLLERS; i++) {
        P[i] = thread(&controllerTask, i, ref(mBT), ref(task), ref(testigo), ref(s1), ref(d1), ref(s2), ref(d2), s3, d3, ref(s4), resultados);
    }
    P[N_CONTROLLERS] = thread(&masterTask, ref(mBT), ref(task), ref(testigo), ref(s1), ref(d1), ref(s2), ref(d2), s3, d3, resultados);
    for(unsigned i = 0; i <= N_CONTROLLERS; i++) {
        P[i].join();
    }
    for(unsigned i = 0; i < N_CONTROLLERS; i++) {
        delete s3[i];
    }
    return 0;
}
