
%{
	int TL1=0;
	int TL2=0;
	int TL3=0;
	int TC=0;
	int TR=0;
	int TI=0;
%}
%x DECLARACION
%x REGLAS
%x CODIGO
%x USUARIO
%x STRING
%%
^[%]\{ 		{BEGIN DECLARACION;}
[/][/]		{TC++;}
main		{BEGIN USUARIO;}					
<DECLARACION>\n	{TL1++;}
<DECLARACION>\%\%	{BEGIN REGLAS;TL2--;}
<DECLARACION>[/][/]		{TC++;}
<REGLAS>\n	{TL2++;}		
<REGLAS>[/][/]		{TC++;TR--;}	
<REGLAS>^.	{TR++;}
<REGLAS>\{	{BEGIN CODIGO;}
<REGLAS>\%\%	{BEGIN INITIAL;}
<CODIGO>\{	{BEGIN CODIGO;}
<CODIGO>\n	{TL2++;}
<CODIGO>\"	{BEGIN STRING;}
<CODIGO>\;	{TI++;}
<CODIGO>\}	{BEGIN REGLAS;}
<STRING>\n	{TL2++;}
<STRING>'	{BEGIN CODIGO;}
<USUARIO>\n	{TL3++;}
<USUARIO>[/][/]	{TC++;}


%%
int main () {
	int error = yylex ();
	printf (" L1 : %d\n" , TL1);
	printf (" L2 : %d\n" , TL2);
	printf (" L3 : %d\n" , TL3);
	printf (" C : %d\n" , TC);
	printf (" R : %d\n" , TR);
	printf (" I : %d\n" , TI);
}