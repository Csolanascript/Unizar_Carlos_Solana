/*#include "elemento.h"

Elemento::Elemento(const string& nom, double vol, double pes)
                    :_nombre(nom),_volumen(vol),_peso(pes)
{
}
    
string Elemento::nombre(){
    return _nombre;
}

double Elemento::volumen() const{
    return _volumen;
}

double Elemento::peso()const {
    return _peso; 
}
void Elemento::mostrar(ostream& os, const string& sep)const{
    os << sep << _nombre << " [" << _volumen << " m3] [" << _peso << " kg]\n";
    // Longaniza de Graus [0.5 m3] [0.8 kg]
}

ostream& operator<<(ostream& os, const Elemento* item){
    item->mostrar(os,"");
}
*/