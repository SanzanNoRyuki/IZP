/******************************************************************************************************/
/***                  Roman Fulla 1.rocnik FIT 2018 - Projekt 2 - Iteracne vypocty                  ***/
/***                                       Verzia - 1.4                                             ***/
/******************************************************************************************************/

#include <stdio.h>                                                                                      //Deklaracia kniznic
#include <stdlib.h>
#include <string.h>
#include <math.h>

enum RETURNS{ALL_OK,ERROR_WRONG_INPUT};                                                                 //Chybove hlasenia

void help();                                                                                            //Deklar�cia funkcie "Napoveda"

double cfrac_log(double x, unsigned int n);                                                             //Deklar�cia funkcie "Logaritmus - Zretazeny zlomok"
double taylor_log(double x, unsigned int n);                                                            //Deklar�cia funkcie "Logaritmus - Taylorov polynom"

double taylor_pow(double x, double y, unsigned int n);                                                  //Deklar�cia funkcie "Exponencionalna - Taylorov polynom"
double taylorcf_pow(double x, double y, unsigned int n);                                                //Deklar�cia funkcie "Exponencionalna - Zretazeny zlomok"

int main(int argc, char *argv[]){                                                                       //Deklaracia hlavnej funkcie

  if (argc==4 && !strcmp(argv[1],"--log")){                                                             //Detekcia volanej funkcie: Logaritmus (kontrola poctu argumentov)
    double       x=strtod(argv[2],NULL);                                                                //Deklaracia hodnoty x (1. zadany argument - zadane cislo)
    unsigned int n=strtoul(argv[3],NULL,10);                                                            //Deklaracia hodnoty n (2. zadany argument - pocet iteracii)

    if (x<=0 || n==0 || argv[3][0]=='-') {                                                              //Kontrola: Zle zadany vstup
      help();                                                                                           //Volanie funkcie "Napoveda"
      fprintf(stderr, "\nerr: log - x value: %g | n value: %i\n",x,n);                                  //Vypis na standartny vstup
      return ERROR_WRONG_INPUT;                                                                         //Ukoncenie programu s chybou
    }

    double        log_output=log(x);                                                                    //Vysledok je funkcia "log" z matematickej kniznice
    double  cfrac_log_output=cfrac_log(x,n);                                                            //Vysledok je to co vypocitala volana funkcia "Logaritmus - Zretazeny zlomok"
    double taylor_log_output=taylor_log(x,n);                                                           //Vysledok je to co vypocitala volana funkcia "Logaritmus - Taylorov polynom"

    printf("       log (%g) = %.12g\n"                                                                  //Vypis vysledkov
           " cfrac_log (%g) = %.12g\n"
           "taylor_log (%g) = %.12g\n",
                         x,   log_output,
                         x,   cfrac_log_output,
                         x,   taylor_log_output);

    return ALL_OK;                                                                                      //Ukoncenie programu
  }

  else if (argc==5 && !strcmp(argv[1],"--pow")) {                                                       //Detekcia volanej funkcie: Exponencionalna funkcia (kontrola poctu argumentov)
    double       x=strtod(argv[2],NULL);                                                                //Deklaracia hodnoty x (1. zadany argument - zadane cislo)
    double       y=strtod(argv[3],NULL);                                                                //Deklaracia hodnoty y (2. zadany argument - zadana mocnina)
    unsigned int n=strtoul(argv[4],NULL,10);                                                            //Deklaracia hodnoty n (3. zadany argument - pocet iteracii)

    if ( (x<=0 && strcmp(argv[2],"0")) || (y==0 && strcmp(argv[3],"0")) || n==0 || argv[4][0]=='-') {   //Kontrola: Zle zadany vstup
      help();                                                                                           //Volanie funkcie "Napoveda"
      fprintf(stderr, "\nerr: pow - x value: %g | y value: %g | n value: %i\n",x,y,n);                  //Vypis na standartny vstup
      return ERROR_WRONG_INPUT;                                                                         //Ukoncenie programu s chybou
    }

    double          pow_output=pow(x,y);                                                                //Vysledok je funkcia "pow" z matematickej kniznice
    double   taylor_pow_output=taylor_pow(x,y,n);                                                       //Vysledok je to co vypocitala volana funkcia "Exponencionalna - Taylorov polynom"
    double taylorcf_pow_output=taylorcf_pow(x,y,n);                                                     //Vysledok je to co vypocitala volana funkcia "Exponencionalna - Zretazeny zlomok"

    if (!strcmp(argv[2],"0") && strcmp(argv[3],"0")){                                                   //Zadanie 0 ako vstupu (ak nie je na 0 mocninu)
        taylor_pow_output=0;
        taylorcf_pow_output=0;
    }

    printf("         pow (%g,%g) = %.12g\n"                                                             //Vypis vysledkov
           "  taylor_pow (%g,%g) = %.12g\n"
           "taylorcf_pow (%g,%g) = %.12g\n",
                           x,y,    pow_output,
                           x,y,    taylor_pow_output,
                           x,y,    taylorcf_pow_output);

    return ALL_OK;                                                                                      //Ukoncenie programu
  }

  else {                                                                                                //Kontrola: Zle zadany vstup
    help();                                                                                             //Volanie funkcie "Napoveda"
    fprintf(stderr, "\nerr: number of arguments: %i | First argument: %s\n", argc,argv[1]);             //Vypis na standartny vstup
    return ERROR_WRONG_INPUT;                                                                           //Ukoncenie programu s chybou
  }
}

void help(){                                                                                            //Napoveda
  printf("-------------------------------------------------------------------------------------------------------------------\n"
         "X                      Tento program pocita prirodzene logaritmi a exponencionalne funkcie.                       X\n"
         "-------------------------------------------------------------------------------------------------------------------\n"
         "X             Ak chcete vypocitat prirodzeny logaritmus daneho cisla, vas vstup by mal vyzerat takto:             X\n"
         "X                                                ./proj2 --log X N                                                X\n"
         "X            X je vase cislo (vacsie ako 0) a N (vacsie ako 0) urcuje ako presne ho chcete vypocitat.             X\n"
         "X                    (cim je N vacsie, tym presnejsi je vysledok, no tym dlhsie bude vypocet trvat)               X\n"
         "-------------------------------------------------------------------------------------------------------------------\n"
         "X                  Ak chcete vypocitat exponencionalnu funkciu, vas vstup by mal vyzerat takto:                   X\n"
         "X                                               ./proj2 --pow X Y N                                               X\n"
         "X    X je vase cislo, Y je na ktoru ho chcete umocnit a N (vacsie ako 0) urcuje ako presne ho chcete vypocitat.   X\n"
         "X                   (cim je N vacsie, tym presnejsi je vysledok, no tym dlhsie bude vypocet trvat)                X\n"
         "X                                                                                                                 X\n"
         "X                      Disclaimer: program bohuzial nie je schopny pocitat s X mensimi ako 0                      X\n"
         "-------------------------------------------------------------------------------------------------------------------\n"
         "X                           Roman Fulla 1.rocnik FIT 2018 - Projekt 2 - Iteracne vypocty                          X\n"
         "-------------------------------------------------------------------------------------------------------------------\n");
}

double cfrac_log(double x, unsigned int n){                                                             //Logaritmus - Zretazeny zlomok
  double z=(x-1)/(x+1);                                                                                 //Deklaracia pomocnej premennej 'z' potrebnej pre vzorce
  double previous=(2*n-1)-(n*n*z*z);                                                                    //Deklaracia predosleho clena a vlozenie hodnoty
  double current=1;                                                                                     //Deklaracia sucasneho clena a vlozenie hodnoty

  for (unsigned int i=n;i>1;i--){                                                                       //Opakovanie od n-tej iteracie po predposlednu iteraciu
    current=(2*i-3)-((i-1)*(i-1)*z*z)/previous;                                                         //Pouzitie vzorca pre vypocet sucasneho clena
    previous=current;                                                                                   //Vlozenie sucasneho clena do minuleho clena
  }

  double output=(2*z)/current;                                                                          //Deklaracia vysledku a jeho posledna uprava podla vzorca
  return output;                                                                                        //Vratenie vysledku
}

double taylor_log(double x, unsigned int n){                                                            //Logaritmus - Taylorov polynom
  double output;                                                                                        //Deklaracia vysledku

  if (x<=1){                                                                                            //Ak je zadane cislo mensie ako 1 pouziva sa tento vypocet:
    double y=1-x;                                                                                       //Deklaracia pomocnej premennej 'y' potrebnej pre vzorce
    double previous=-y;                                                                                 //Deklaracia predosleho clena a vlozenie prvej hodnoty
    double current;                                                                                     //Deklaracia sucasneho clena
           output=previous;                                                                             //Vlozenie prvej hodnoty do vysledku

    for (unsigned int i=2;i<=n;i++){                                                                    //Opakovanie od druhej po n iteracii
      current=(previous*(i-1)*y)/i;                                                                     //Pouzitie vzorca pre vypocet sucasneho clena
      output=output+current;                                                                            //Pripocitanie sucasneho clena k vysledku
      previous=current;                                                                                 //Vlozenie sucasneho clena do minuleho clena
    }
  }

  else{                                                                                                 //Inak sa pouziva tento vypocet:
    double previous=((x-1)/x);                                                                          //Deklaracia predosleho clena a vlozenie prvej hodnoty
    double current;                                                                                     //Deklaracia sucasneho clena
           output=previous;                                                                             //Vlozenie prvej hodnoty do vysledku

    for(unsigned int i=2;i<=n;i++){                                                                     //Opakovanie od druhej po n iteracii
      current=((previous*(i-1))*((x-1)/x))/i;                                                           //Pouzitie vzorca pre vypocet sucasneho clena
      output=output+current;                                                                            //Pripocitanie sucasneho clena k vysledku
      previous=current;                                                                                 //Vlozenie sucasneho clena do minuleho clena
    }
  }

  return output;                                                                                        //Vratenie vysledku
}

double taylor_pow(double x, double y, unsigned int n){                                                  //Exponencionalna funkcia - Taylorov polynom
  double previous=1;                                                                                    //Deklaracia predosleho clena a vlozenie prvej hodnoty
  double current;                                                                                       //Deklaracia sucasneho clena
  double output=previous;                                                                               //Deklaracia vysledku a vlozenie prvej hodnoty
  double log_x=taylor_log(x,n);                                                                         //Vypocet Taylorovho logaritmu pre 'x'

  for(unsigned int i=1;i<=n-1;i++){                                                                     //Opakovanie po n-1 iteracii
    current=(previous*y*log_x)/i;                                                                       //Pouzitie vzorca pre vypocet sucasneho clena
    output=output+current;                                                                              //Pripocitanie sucasneho clena k vysledku
    previous=current;                                                                                   //Vlozenie sucasneho clena do minuleho clena
  }

  return output;                                                                                        //Vratenie vysledku
}

double taylorcf_pow(double x, double y, unsigned int n){                                                //Exponencionalna funkcia - Zretazeny zlomok
  double previous=1;                                                                                    //Deklaracia predosleho clena a vlozenie prvej hodnoty
  double current;                                                                                       //Deklaracia sucasneho clena
  double output=previous;                                                                               //Deklaracia vysledku a vlozenie prvej hodnoty
  double log_x=cfrac_log(x,n);                                                                          //Vypocet Logaritmu zretazeneho zlomku pre 'x'

  for(unsigned int i=1;i<=n-1;i++){                                                                     //Opakovanie po n-1 iteracii
    current=(previous*y*log_x)/i;                                                                       //Pouzitie vzorca pre vypocet sucasneho clena
    output=output+current;                                                                              //Pripocitanie sucasneho clena k vysledku
    previous=current;                                                                                   //Vlozenie sucasneho clena do minuleho clena
  }

  return output;                                                                                        //Vratenie vysledku
}
