#include "rational.h"

// Auxiliares

int Rational::mcd(int a, int b)
{
	return ( b == 0 ? a : mcd(b,a%b));
}

void Rational::reduce()
{
	int a;
	while((a = mcd(numer,denom)) != 1){
		numer=numer / a; 
		denom=denom / a;	
	}
}

// Constructores

Rational::Rational()
{
	numer = 0;
	denom = 1;
	reduce();
}

Rational::Rational(int num, int den)
{
	numer = num;
	denom = den;
	reduce();
}

// Entrada/salida

void Rational::write(std::ostream& os) const
{
	os << numer;
	os << "/";
	os << denom;
}

void Rational::read(std::istream& is)
{
	char c;
	is >> numer;
	is >> c;
	is >> denom;
	reduce();
}

// Operaciones aritmeticas

Rational Rational::add(Rational b) const
{
	Rational sol;
	sol.denom = denom * b.denom;
	sol.numer = numer * b.denom + b.numer * denom;
	sol.reduce();
	return sol;
}

Rational Rational::sub(Rational b) const
{
	Rational sol;
	sol.denom = denom * b.denom;
	sol.numer = numer*b.denom - b.numer*denom;
	if(sol.numer < 0) {
		sol.numer = -sol.numer;
		sol.reduce();
		sol.numer = -sol.numer;
	}
	else {
	sol.reduce();
	}
	return sol;
}

Rational Rational::mul(Rational b) const
{
	Rational sol;
	sol.denom = denom*b.denom;
	sol.numer = numer*b.numer;
	sol.reduce();
	return sol;
}

Rational Rational::div(Rational b) const
{
	Rational sol;
	sol.denom = b.numer;
	sol.numer = b.denom;
	return mul(sol);
}

// Operaciones logicas

bool Rational::equal(Rational b) const
{
	return(numer == b.numer && denom == b.denom);	
}

bool Rational::lesser_than(Rational b) const
{
	return(numer*b.denom < b.numer*denom);
}

bool Rational::greater_than(Rational b) const
{
	return(numer*b.denom > b.numer*denom);
}
