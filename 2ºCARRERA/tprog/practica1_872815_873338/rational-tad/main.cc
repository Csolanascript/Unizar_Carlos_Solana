#include "rational.h"
#include <iostream>

#define ever (;;)

using namespace std;

int main()
{
	Rational	a,b;
	Rational	zero;
	Rational 	r;
	char		op;

	init(a);
	init(b);
	init(zero,0,1);
	init(r);

	for ever
	{
		cout << "? " << flush;

		cin >> skipws;
		read(cin,a);
		if (cin.fail())	break;
		cin >> skipws >> op;
		if (cin.fail())	break;
		cin >> skipws;
		read(cin,b);
		if (cin.fail())	break;
		cout << "= ";
		switch(op)
		{
			case '+':
				r = add(a,b);
				write(cout,r);
				break;
			case '-':
				r = sub(a,b);
				write(cout,r);
				break;
			case '*':
				r = mul(a,b);
				write(cout,r);
				break;
			case '/':
				r = div(a,b);
				write(cout,r);
				break;
			case '=':
				cout << (equal(a,b) ? "yes" : "no");
				break;
			case '<':
				cout << (lesser_than(a,b) ? "yes" : "no");
				break;
			case '>':
				cout << (greater_than(a,b) ? "yes" : "no");
				break;
			default:
				write(cout,zero);
		}
		cout << endl;
	}

	return 0;
}

