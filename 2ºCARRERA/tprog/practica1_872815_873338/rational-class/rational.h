#pragma once

#include <iostream>

// Solucion con atributos privados.

class Rational
{
private:
	// Atributos
	int numer;
	int denom;

	//Auxiliar
	static int mcd(int a, int b);
	void reduce();

public:
	// Constructores

	Rational();
	Rational(int num, int den);

	// Entrada/salida

	void write(std::ostream& os) const;
	void read(std::istream& is);

	// Operaciones aritmeticas

	Rational add(Rational b) const;
	Rational sub(Rational b) const;
	Rational mul(Rational b) const;
	Rational div(Rational b) const;

	// Operaciones logicas

	bool equal(Rational b) const;
	bool lesser_than(Rational b) const;
	bool greater_than(Rational b) const;
};
