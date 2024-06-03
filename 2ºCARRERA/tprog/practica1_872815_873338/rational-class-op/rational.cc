#include "rational.h"

// Auxiliares

int Rational::mcd(int a, int b)
{
	return ( b == 0 ? a : mcd(b,a%b));
}

void Rational::reduce()
{
	...
}

// Constructores

Rational::Rational()
{
	...
}

Rational::Rational(int num, int den)
{
	...
}

// Operadores aritmeticos

... Rational::operator+(...) const
{
	...
}

... Rational::operator-(...) const
{
	...
}

... operator*(...)
{
	...
}

... operator/(...)
{
	...
}

// Operadores logicos

... Rational::operator==(...) const
{
	...
}

... Rational::operator<(...) const
{
	...
}

... Rational::operator>(...) const
{
	...
}

// Entrada/salida

... operator<<(...)
{
	...
}

... operator>>(...)
{
	...
}



