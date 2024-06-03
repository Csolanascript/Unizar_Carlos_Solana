/* Autores: Diego Mateo Lorente - 873338
*           Carlos Solana Melero - 872815
*/

#pragma once

class ElectricVmp{
    private:
        bool cargando;
        int carga;
    public:
        ElectricVmp(bool cargando, int carga = 0);
        //getters
        bool _cargando();
        int _carga();
        //setters
        void mod_cargando(bool enchufado);
        void mod_carga(int carga_restante);
};