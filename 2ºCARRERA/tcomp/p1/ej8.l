mult3		I(((H*VH*)(H*VH*)(H*VH*))+|H*)F
parNoMult3	I((H*VH*)(H*VH*)+|H*)F
imparNoMult3	I((H*V*)+|H*)F

%%
{mult3}		printf("++%s++", yytext);
{parNoMult3}	printf("+%s+", yytext);
{imparNoMult3}	printf("-%s-", yytext);
%%

int main(){
	printf("\n");
	int error = yylex();
	return error;
}