#include <iostream>
using namespace std;
/* programa que introduce un numero de segundos y los transorma en horas,minutos y sugundos*/
int main(){
    unsigned hor,min,seg;
    int tiempo;                  
    cout<<"Duracion en segundos: ";
    cin>>tiempo;
    if (tiempo<=0){
        cout<<"la fecha debe ser positiva";
    }
    else{
        min = tiempo / 60;
        seg = tiempo % 60;
        hor = min / 60;
        if(hor<0){
        min = hor % 60;}
        cout<<"Este tiempo equivale a " << hor <<" horas, "<< min <<" minutos y " << seg << " segundos "; 
    }
}