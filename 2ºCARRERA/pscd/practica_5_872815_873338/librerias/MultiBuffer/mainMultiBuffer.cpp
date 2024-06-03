//*****************************************************************
// File:   mainMultiBuffer.cpp
// Author: PSCD-Unizar
// Date:   septiembre 2018
// Coms:   Parte del práctica 3 de PSCD
//         Ejemplo de uso del TAD "MultiBuffer"
//*****************************************************************

#include <iostream>
#include <thread>
#include <string>

#include "MultiBuffer.hpp"

using namespace std;
const int NUM_BUFS = 5;
//-----------------------------------------------------
//Definimos un tipo tarea, de ejemplo
struct tarea {
    string id;
    int numVeces;
};

string to_string(tarea t ) {
    return "{" + t.id + "," + to_string(t.numVeces) + "}";
}

string to_string(MultiBuffer<tarea,NUM_BUFS>& bT) {
    const string sep = "   ";
    string res = "[\n";
    for (unsigned i=0;i<bT.size();i++) {
        res = res + sep + "(" + to_string(bT.isNew(i)) + "):";
        if (bT.isNew(i)) {
            res += to_string(bT.watch(i)) + "\n";
        }
        else {
            res += "???\n";
        }
    }
    res += "]";
    return res;
}
//-----------------------------------------------------
int main() {
    tarea t1 = {"casa",3};
    tarea t2 = {"perro",6};
    tarea t3 = {"bosque",1000};
    MultiBuffer<tarea,NUM_BUFS> bT1,bT2;

    bT1.put(t1,2);
    bT1.put(t2,4);
    bT1.put(t3,0);

    cout << "Trabajando" << endl;
    cout << "inicial:\n" + to_string(bT1) << endl;
    //-------------------------------------------------
    t3 = bT1.get(4);
    cout << "bT1.get(4):\n" + to_string(bT1) << endl;
    t1 = bT1.get(0);
    cout << "bT1.get(0):\n" + to_string(bT1) << endl;
    //-------------------------------------------------
    bT1.put(t2,0);
    cout << "bT1.put(t2,4):\n" << to_string(bT1) << endl;

    return 0;
}
