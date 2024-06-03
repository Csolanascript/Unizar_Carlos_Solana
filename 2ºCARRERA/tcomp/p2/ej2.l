
%%
(((a|ba)a*b(aa*b)ba|bbb*a*bba|((a|ba)a*b(aa*b)bb|bbb*aa*bbb)(aa*bbb)*aa*bba)a*|
(((a|ba)a*b(aa*b)bb|bbb*aa*bbb)(aa*bbb)*b|(bbb*aa*ba|((a|ba)a*b(aa*b)bb|bbb*aa*bbb)
(aa*bbb)*aa*ba)a*))((bb|aa*bbb)(aa*bbb)*b|(aa*ba|(bb|aa*bbb)(aa*bbb)*aa*ba)a*)*(
ba|aa*bba|(bb|aa*bbb)(aa*bbb)*aa*bba)a b)b(anbb)b ((((aba)a*b(aa*b)baaa*bbba)
((aba)a*b(aa*b)bbb bbb*aa*bbb)(aa*bbb)aabba)a b)(((aba)a baa*b)bbb bbb aa*bbb)
(a*bbb)b)(bbb aa ba)((aba)a*b)aa bbbb aa bbb)(aa*bbb)aa*ba)a*))((bb|aa*bbb)(
aa*bbb)b](aa*ba](bblaa*bbb)(aa bbb)aa*ba)a*)*(babaa*bbab(bb)aa|bbb)(ae|bbb)a b)
b(a|bb)|aa|ba)(aba|aa|bbb)(aa|bbb)b(a|bb)b(a|bb(aa|bb)*b|(aaabaab|bb(aa|bb)*aa|ba)
(a|bab bb)an|ba)b(a|bb)b)*(b(a|bb)b)b(a|bb)aa|ba(a|bb|a|bb(aa|bb)b](aa|ba](bb|aa*bbb)
(aa bbb)aa|ba)a|bb)*(babaa*bbab(bb)ae|bbb)(ae|bbb)"a|bbba)a b)b { printf("**--%s", yytext); }
^(aa*b|bb*aa*b)(aa*b)*b(ba*b(aa*b)*b)*aa*ba*b((b|a)a*ba*b)*$	{ printf("**%s", yytext); }
^(b|aa*b)(aa*b)*bb*aa*b(bb*aa*b|aa*bb*aa*b)*$						{ printf("--%s", yytext); }

%%
int main() {
    int error = yylex();
}