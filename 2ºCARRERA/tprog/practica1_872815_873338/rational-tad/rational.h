#pragma once

#include <iostream>

// Solucion con atributos privados.

struct Rational
{
private:
   // Atributos, a definir
   int num; 
   int denom;
   // Declaraciones de friends, en su caso...
   friend void init(Rational& r);
   friend void init(Rational& r,int num, int den);
   friend void write(std::ostream& os, const Rational& r);
   friend void read(std::istream& is,Rational& r);
   friend Rational add(const Rational& r1, const Rational& r2);
   friend Rational sub(const Rational& r1, const Rational& r2);
   friend Rational mul(const Rational& r1, const Rational& r2);
   friend Rational div(const Rational& r1, const Rational& r2);
   friend bool equal(const Rational& r1, const Rational& r2);
   friend bool lesser_than(const Rational& r1, const Rational& r2);
   friend bool greater_than(const Rational& r1, const Rational& r2);
   friend int num(Rational& r);
   friend int denom(Rational& r);
   friend void reduce(Rational& r);
};

// Inicializadores

void init(Rational& r);
void init(Rational& r,int num, int den);

// Entrada/salida

void write(std::ostream& os, const Rational& r);
void read(std::istream& is,Rational& r);

// Operaciones aritmeticas

Rational add(const Rational& r1, const Rational& r2);
Rational sub(const Rational& r1, const Rational& r2);
Rational mul(const Rational& r1, const Rational& r2);
Rational div(const Rational& r1, const Rational& r2);

// Operaciones logicas

bool equal(const Rational& r1, const Rational& r2);
bool lesser_than(const Rational& r1, const Rational& r2);
bool greater_than(const Rational& r1, const Rational& r2);

void reduce(Rational& r);