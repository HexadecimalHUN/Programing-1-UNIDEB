%{
  #include <string.h>
  int kbetu = 0, nbetu = 0, szo = 0, szam = 0, sor = 0,spec = 0, osszeg = 0;
%}
%%
\n {sor++;szo++;}
[\t ' '] szo++;
[A-Z] nbetu++;
[a-z] kbetu++;
[0-9] szam++;
. spec++;
%%

main (void)
{
yyin = fopen("pelda.txt","r");
yylex();
osszeg = kbetu + nbetu + spec;
printf("\nEbben a fileban van:");
printf("%d sor\n", sor);
printf("%d szo\n", szo);
printf("%d kisbetu\n", kbetu);
printf("%d nagybetu\n", nbetu);
printf("%d szamjegy\n", szam);
printf("%d specialis karakter\n", spec);
printf("%d Karakter osszesen \n", osszeg );
}

int yywrap()
{
return(1);
}
// magyarazatok :
// https://stackoverflow.com/questions/22106034/how-to-use-lex-with-c-source-file
// https://arato.inf.unideb.hu/batfai.norbert/UDPROG/deprecated/Prog1_1.pdf
// http://xahlee.info/parser/lex_tutorial.html
// https://stackoverflow.com/questions/8859710/lex-how-to-run-compile-a-lex-program-on-commandline
