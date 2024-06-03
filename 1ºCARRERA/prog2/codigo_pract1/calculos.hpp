//*****************************************************************
// File:   calculos.hpp
// Author: Programación II. Universidad de Zaragoza
// Date:   febrero 2022
// Coms:   Fichero de interfaz de calculos.cpp
//         Para la práctica 1
//*****************************************************************

#ifndef CALCULOS_HPP
#define CALCULOS_HPP

// Pre:  n >= 0 ∧ 2 <= b <= 10
// Post: (n = 0 -> numCifras(n,b) = 1) ∧
//       (n > 0 -> numCifras(n,b) = NC ∧  n >= b^(NC-1) ∧ n < b^NC)
int numCifras(const int n, const int b = 10);


// Pre:  n >= 0 ∧ i >= 1 ∧ 2 <= b <= 10
// Post: cifra(n,i,b) = (n / b^(i-1)) % b
int cifra(const int n, const int i, const int b = 10);

// Pre:  n >= 0 ∧ 2 <= b <= 10
// Post: cifraMayor(n,b) = (Max α∊[1,∞].cifra(n,α,b))
int cifraMayor(const int n, const int b = 10);


// Pre:  n >= 0 ∧ 2 <= b <= 10
// Post: cifraMasSignificativa(n) = n / b^(NC-1)
//       ∧ (n >= b^(NC-1) ∧ n < b^NC)
int cifraMasSignificativa(const int n, const int b = 10);


// Pre:  n >= 0 ∧ 2 <= b <= 10
// Post: sumaCifras(n,b) = (∑ α∊[1,∞].cifra(n,α,b))
int sumaCifras(const int n, const int b = 10);

#endif
