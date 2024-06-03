/* Autores: Diego Mateo Lorente - 873338
*           Carlos Solana Melero - 872815
*/

#pragma once

class Vmp{
    private:
        int id;
        double x;
        double y;
        bool esta_en_base;
        bool dispo;
    public:
        Vmp(int id, bool esta_en_base, bool dispo, double x = 0, double y = 0);
        double distancia();
        virtual void alquilar();
        virtual void devolver();
        virtual void liberar();
        void bloquear();
    private:
        //setters
        int identificador();
        double coord_x();
        double coord_y();
        bool enBase();
        bool disponible();
        //getters
        void mod_identificador(int identificador);
        void mod_coord_x(double coord_x);
        void mod_coord_y(double coord_y);
        void mod_enBase(bool enBase);
        void mod_disponible(bool disponible);
};