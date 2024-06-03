#include "rational.h"
#include <iostream>

#define ever (;;)

using namespace std;

int main()
{
	Rational	a,b;
	Rational	zero(0,1);
	char		op;

	for ever
	{
		cout << "? " << flush;

		cin >> skipws >> a >> skipws >> op >> skipws >> b;
		if (cin.fail())	break;

		cout << "= ";
		switch(op)
		{
			case '+':
				cout << a+b;
				break;
			case '-':
				cout << a-b;
				break;
			case '*':
				cout << a*b;
				break;
			case '/':
				cout << a/b;
				break;
			case '=':
				cout << (a==b ? "yes" : "no");
				break;
			case '<':
				cout << (a<b ? "yes" : "no");
				break;
			case '>':
				cout << (a>b ? "yes" : "no");
				break;
			default:
				cout << zero;
		}
		cout << endl;
	}

	return 0;
}

