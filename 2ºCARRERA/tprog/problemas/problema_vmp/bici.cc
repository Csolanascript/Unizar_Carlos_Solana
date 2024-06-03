/* Autores: Diego Mateo Lorente - 873338
*           Carlos Solana Melero - 872815
*/

#include "bici.h"
using namespace std;
Bici::Bici(int id, bool esta_en_base, bool dispo, double x = 0, double y = 0)
        :Vmp(id,esta_en_base,dispo,x,y)
{}