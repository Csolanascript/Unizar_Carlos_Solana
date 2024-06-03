//-------------------------------------------------------------------------------------------
// File:   Matrix.cpp
// Author: Carlos Solana Melero (872815) - Diego Mateo Lorente (873338)
// Date:   11-11-2023
// Coms:   Monitor que gestiona las operaciones de escritura de la matriz y su muestre por pantalla
//-------------------------------------------------------------------------------------------

//constructor del monitor Matrix, que inicializa el la matriz de resultados
template <unsigned N>
Matrix<N>::Matrix(){
    for(unsigned i = 0; i < N; i++) {
        for(unsigned j = 0; j < 3; j++) {
            resultados[i][j] = 0;
        }
    }
}

//Operación de escritura de las tareas ejecutadas en la matriz de resultados
template <unsigned N>
void Matrix<N>::escribe(string tarea, const int numGen, double cargaTrabajo, int& n, int id){
    unique_lock<mutex> lck(mtxMatrix);          //Hago que toda la función esté en exclusión mutua
    if(tarea == "t1") {                         //Esperamos a tener permiso para escribir en la matriz
        n++;
        resultados[0][0]++;                     //Aumentamos en 1 el número de veces ejecutado t1
        if (numGen>=5){                         //5% de fallo
            this_thread::sleep_for(chrono::milliseconds(int(cargaTrabajo)));
            resultados[0][2] += cargaTrabajo;   //Sumamos el tiempo de carga de trabajo de t1
            resultados[0][1]++;                 //Aumentamos en 1 el número de éxitos en t1
        }
    }
    if(tarea == "t2") {
        resultados[1][0]++;                     //Aumentamos en 1 el número de veces ejecutado t2
        if (numGen>=7){                         //7% de fallo
            this_thread::sleep_for(chrono::milliseconds(int(cargaTrabajo)));
            resultados[1][2] += cargaTrabajo;   //Sumamos el tiempo de carga de trabajo de t2
            resultados[1][1]++;                 //Aumentamos en 1 el número de éxitos en t2
        }
    }
    if(tarea == "t3") {
        resultados[2][0]++;                     //Aumentamos en 1 el número de veces ejecutado t3
        if (numGen>=10){                        //10% de fallo
            this_thread::sleep_for(chrono::milliseconds(int(cargaTrabajo)));
            resultados[2][2] += cargaTrabajo;   //Sumamos el tiempo de carga de trabajo de t3
            resultados[2][1]++;                 //Aumentamos en 1 el número de éxitos en t3
        }
    }
}

//Operación que muestra por pantalla la matriz de resultados
template <unsigned N>
void Matrix<N>::muestra(){
    unique_lock<mutex> lck(mtxMatrix);          //Hago que toda la función esté en exclusión mutua
        cout << endl;
        for (int i = 0; i < 3; i++){            //Muestra la matriz por pantalla
            for (int t = 0; t < 3; t++)
            {
            cout<< left << setw(6) << resultados[i][t];
            }
            cout << endl;
        }
}