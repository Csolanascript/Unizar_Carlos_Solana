#include "rational.h"
#include <iostream>

#define ever (;;)

using namespace std;

int main()
{
	Rational	a,b;
	Rational	zero(0,1);
	Rational 	r;
	char		op;

	for ever
	{
		cout << "? " << flush;

		cin >> skipws;
		a.read(cin);
		if (cin.fail())	break;

		cin >> skipws >> op;
		if (cin.fail())	break;

		cin >> skipws;
		b.read(cin);
		if (cin.fail())	break;

		cout << "= ";
		switch(op)
		{
			case '+':
				r = a.add(b);
				r.write(cout);
				break;
			case '-':
				r = a.sub(b);
				r.write(cout);
				break;
			case '*':
				r = a.mul(b);
				r.write(cout);
				break;
			case '/':
				r = a.div(b);
				r.write(cout);
				break;
			case '=':
				cout << (a.equal(b) ? "yes" : "no");
				break;
			case '<':
				cout << (a.lesser_than(b) ? "yes" : "no");
				break;
			case '>':
				cout << (a.greater_than(b) ? "yes" : "no");
				break;
			default:
				zero.write(cout);
		}
		cout << endl;
	}

	return 0;
}

