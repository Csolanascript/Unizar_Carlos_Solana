#include <iostream>
using namespace std;
/* programa que introduce una cantidad de dinero en formato decimal y devuelve la
cantidad de centimos y euros equivalentes, asi como su valor en pesetas*/
int main(){
    double eurosdec;
    unsigned euros,centimos,pesetas;
    cout << "Escriba una cantidad no negativa de dinero en euros: ";
    cin >> eurosdec;
    if(eurosdec >= 0){
    euros = int(eurosdec);
    centimos = (( eurosdec - euros) * 100)  +   0.5;
    pesetas=eurosdec* 166.386   +  0.5;
    cout<<"Son "<<  euros   <<  " euros y " <<  centimos  <<  " centimos, que equivalen a "   <<pesetas<< " pesetas.";
    }
    else{
        cout << " el numero debe ser negativo";
    }
}