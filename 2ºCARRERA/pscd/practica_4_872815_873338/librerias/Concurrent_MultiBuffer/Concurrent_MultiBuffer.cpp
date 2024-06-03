//-------------------------------------------------------------------------------------------
// File:   Concurrent_MultiBuffer.cpp
// Author: Carlos Solana Melero (872815) - Diego Mateo Lorente (873338)
// Date:   11-11-2023
// Coms:   Monitor que gestiona las comunicaciones entre el proceso máster y el proceso controlador
//-------------------------------------------------------------------------------------------

//Operación que asigna una tarea a un controlador libre
template <typename T,unsigned N>
void Concurrent_MultiBuffer<T,N>::putTarea(const T tarea) {
    unique_lock<mutex> lck(mtxConcurrent_MultiBuffer);
    while(mB.numberNews() >= mB.size()) {       //await nNews < N
        bufferLibre.wait(lck);                  //bloquea el monitor hasta que haya algun proceso libre
    }
    unsigned i = 0;
    bool encontrado = false;
    while(!encontrado) {
        if(!mB.isNew(i)) {
            mB.put(tarea, i);                   //Colocamos la tarea en un controlador libre
            llegaTarea[i].notify_all();         //avisa al controlador i de que se le ha asignado una nueva tarea
            encontrado = true;
        }
        i++;
    }
}

//Operación que asigna la tarea final a todos los controladores cuando estén libres
template <typename T,unsigned N>
void Concurrent_MultiBuffer<T,N>::putTF(const T tarea) {
    unique_lock<mutex> lck(mtxConcurrent_MultiBuffer);
    while(mB.numberNews() > 0) {                //await nNews = 0
        zeroNews.wait(lck);                     //espera a que todos los controladores estén libres
    }
    for(unsigned i = 0; i < mB.size(); i++) {
        mB.put(tarea, i);                       //Colocamos la tarea final a todos los controladores
        llegaTarea[i].notify_all();             //Vamos avisando a cada controlador que ha llegado la tarea final
    }
}

//Operación que extrae la tarea del búffer cuando esta llega 
template <typename T,unsigned N>
T Concurrent_MultiBuffer<T,N>::getTarea(const unsigned pos) {
    unique_lock<mutex> lck(mtxConcurrent_MultiBuffer);
    while(!mB.isNew(pos)) {                     //await isNew(i)
        llegaTarea[pos].wait(lck);              //espera a que llegue la tarea
    }
    T tarea = mB.get(pos);                      //al estar en exclusion mutua ya sabemos que solo un proceso podra coger ese dato
    zeroNews.notify_all();                      //avisamos de que uno de los controladores está libre, y puede que ahora todos los controladores estén libres
    bufferLibre.notify_all();                   //avisamos de que el búffer está libre para meter otra tarea
    return tarea;
}

//Operación de espera hasta que todos los controladores estén libres
template <typename T,unsigned N>
void Concurrent_MultiBuffer<T,N>::espera() {
    unique_lock<mutex> lck(mtxConcurrent_MultiBuffer);
    while(mB.numberNews() > 0) {                //await nNews = 0
        zeroNews.wait(lck);                     //espera a que todos los controladores estén libres
    }
}