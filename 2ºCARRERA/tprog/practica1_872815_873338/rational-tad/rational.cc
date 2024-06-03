#include "rational.h"

// Auxiliares, privadas

int mcd(int a, int b)
{
	return ( b==0 ? a : mcd(b,a%b) );
}

void reduce(Rational& r)
{
	int a;
	while((a = mcd(r.num,r.denom)) != 1){
		r.num=r.num / a; 
		r.denom=r.denom / a;	
	}
}

// Inicializadores

void init(Rational& r)
{
	r.num = 0;
	r.denom = 1;
}

void init(Rational& r,int num, int den)
{
	r.num = num;
	r.denom = den;
	reduce(r);
}

// Entrada/salida

void write(std::ostream& os, const Rational& r)
{
	os << r.num;
	os << "/";
	os << r.denom;
	}

void read(std::istream& is, Rational& r)
{
	char c;
	is >> r.num;
	is >> c;
	is >> r.denom;
	reduce(r);
}

// Operaciones aritmeticas

Rational add(const Rational& r1, const Rational& r2)
{
	Rational sol;
	sol.denom = r1.denom * r2.denom;
	sol.num = r1.num * r2.denom + r2.num * r1.denom;
	reduce(sol);
	return sol;
}

Rational sub(const Rational& r1, const Rational& r2)
{
	Rational sol;
	sol.denom = r1.denom * r2.denom;
	sol.num = r1.num*r2.denom - r2.num*r1.denom;
	if(sol.num < 0) {
		sol.num = -sol.num;
		reduce(sol);
		sol.num = -sol.num;
	}
	else {
	reduce(sol);
	}
	return sol;
}

Rational mul(const Rational& r1, const Rational& r2)
{
	Rational sol;
	sol.denom = r1.denom*r2.denom;
	sol.num = r1.num*r2.num;
	reduce(sol);
	return sol;
}

Rational div(const Rational& r1, const Rational& r2)
{
	Rational sol;
	sol.denom = r2.num;
	sol.num = r2.denom;
	return mul(r1,sol);
}

// Operaciones logicas

bool equal(const Rational& r1, const Rational& r2)
{
	return(r1.num == r2.num && r1.denom == r2.denom);	
}

bool lesser_than(const Rational& r1, const Rational& r2)
{
	return(r1.num*r2.denom < r2.num*r1.denom);
}

bool greater_than(const Rational& r1, const Rational& r2)
{
	return(r1.num*r2.denom > r2.num*r1.denom);
}


