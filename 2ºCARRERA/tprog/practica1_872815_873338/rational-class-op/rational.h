#pragma once

#include <iostream>

// Solucion con atributos privados.

class Rational
{
private:
	// Atributos

private:
	// Auxiliares

	static int mcd(int a, int b);
	void reduce();

public:
	// Constructores

	Rational();
	Rational(int num, int den);

	// Operadores aritmeticos

	... operator+(...) const;
	... operator-(...) const;
	friend ... operator*(...);
	friend ... operator/(...);

	// Operadores logicos

	... operator==(...) const;
	... operator<(...) const;
	... operator>(...) const;

	// Friends, en su caso
	friend ... operator<<(...);
	...
};

// Operadores aritmeticos

... operator*(...);
... operator/(...);

// Entrada/salida

... operator<<(...);
... operator>>(...);
