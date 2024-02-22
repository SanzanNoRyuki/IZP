/******************************************************************************************************/
/*                    Roman Fulla 1.ro�n�k FIT 2018 - Projekt 1 - Pr�ca s textom                      */
/*                                         Verzia - 6.1                                               */
/******************************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


enum RETURNS{ALL_OK,ERROR_FILE_CANNOT_BE_OPENED,ERROR_GO_TO_VALUE,ERROR_UNEXPECTED_ARGUMENT,            //Chybov� hl�senia
             TOO_MANY_ARGUMENTS,TOO_LONG_SENTENCES,TOO_MANY_LINES};


void help(int argc);                                                                                    //Deklar�cia funkcie "N�poveda"

void presah_riadkov (int max_znaky_na_riadok);                                                          //Deklar�cia funkcie "Pr�li� dlh� riadky"

void string_connect(char *vystup, char *vstup,char *insert,char *before,char *after,bool remove_EOL);   //Deklar�cia funkcie "Spoj v�sledky"


int main(int argc ,char *argv[]) {

  if (argc!=2) {                                                                                        //Kontrola: Spr�vnos� po�tu argumentov
    help(argc);                                                                                         //Volanie funkcie "N�poveda"
    return ALL_OK;                                                                                      //Ukon�enie programu
  }


  FILE *arguments=fopen(argv[1],"r");                                                                   //Otvorenie s�boru z pr�kazmi
  if(arguments==NULL) {                                                                                 //Kontrola: Otvorenie s�boru
    printf("Chyba pri otvarani suboru.\n");                                                             //Hl�senie o chybe
    return ERROR_FILE_CANNOT_BE_OPENED;                                                                 //Ukon�enie programu s chybou
  }


  int max_riadky=111+1,max_znaky_na_riadok=1111+1;                                                      //Deklar�cia kon�t�nt
  char tmp;                                                                                             //Deklar�cia pomocnej premennej

  int riadok_prikazu=0,pismeno_prikazu=0;                                                               //Deklar�cia pr�kazov
  char prikaz[max_riadky][max_znaky_na_riadok];

  int riadok_vstupu=0,pismeno_vstupu=0;                                                                 //Deklar�cia vstupu
  char vstup[max_riadky][max_znaky_na_riadok];


  while(tmp!=EOF) {                                                                                     //��tanie obsahu zo s�boru pr�kazov
    tmp=fgetc(arguments);
    prikaz[riadok_prikazu][pismeno_prikazu]=tmp;
    pismeno_prikazu++;                                                                                  //Postup p�smeno po p�smene
    if (tmp=='\n') {                                                                                    //Ak nastane koniec riadku:
      prikaz[riadok_prikazu][pismeno_prikazu-1]='\0';                                                   //Vymazanie posledn�ho znaku "\n"
      pismeno_prikazu=0;                                                                                //Za�atie od prv�ho p�smena
      riadok_prikazu++;                                                                                 //Posunutie riadku
      if(riadok_prikazu==max_riadky-1) {                                                                //Kontrola: Po�et pr�kazov
        printf("Prilis vela prikazov.\nSkus ich pouzit menej.\n");                                      //Hl�senie o chybe
        return TOO_MANY_ARGUMENTS;                                                                      //Ukon�enie programu s chybou
      }
    }
  }
  prikaz[riadok_prikazu][pismeno_prikazu-1]='\0';                                                       //Vymazanie posledn�ho znaku "\n"
  tmp='\0';                                                                                             //Vynulovanie do�asnej premennej
  fclose(arguments);                                                                                    //Zatvorenie s�boru pr�kazov


 while(tmp!=EOF) {                                                                                      //��tanie vstupu
    tmp=getchar();
    vstup[riadok_vstupu][pismeno_vstupu]=tmp;
    pismeno_vstupu++;                                                                                   //Postup p�smeno po p�smene
    if (tmp=='\n') {                                                                                    //Ak nastane koniec riadku:
      vstup[riadok_vstupu][pismeno_vstupu-1]='\0';                                                      //Vymazanie posledn�ho znaku "\n"
      pismeno_vstupu=0;                                                                                 //Za�atie od prv�ho p�smena
      riadok_vstupu++;                                                                                  //Posunutie riadku
      if(riadok_vstupu==max_riadky-1) {                                                                 //Kontrola: Po�et pr�kazov
        printf("Prilis vela riadkov.\nSkus ich pouzit menej.\n");                                       //Hl�senie o chybe
        return TOO_MANY_LINES;                                                                          //Ukon�enie programu s chybou
      }
    }
  }
  vstup[riadok_vstupu][pismeno_vstupu-1]='\0';                                                          //Vymazanie posledn�ho znaku "\n"
  tmp='\0';                                                                                             //Vynulovanie do�asnej premennej


  int sucasny_riadok=0;                                                                                 //Deklar�cia premenn�ch pre �a��� cyklus
  char vystup[max_riadky][max_znaky_na_riadok];

  char insert[max_znaky_na_riadok];                                                                     //Deklar�cia pre pr�kaz "insert"
  char before[max_znaky_na_riadok];                                                                     //Deklar�cia pre pr�kaz "before"
  char after[max_znaky_na_riadok];                                                                      //Deklar�cia pre pr�kaz "after"
  int  vymaz;                                                                                           //Deklar�cia pre pr�kaz "delete"
  bool spoj;                                                                                            //Deklar�cia pre pr�kaz "remove EOL"
  int posun;                                                                                            //Deklar�cia pre pr�kaz "next"
  int skoc_na_riadok;                                                                                   //Deklar�cia pre pr�kaz "goto"
  bool skoc_na_riadok_kontrola[max_riadky];

  for (int sucasny_prikaz=0;sucasny_prikaz<=riadok_prikazu;sucasny_prikaz++){                           //��tanie pr�kazov postupne
    switch(prikaz[sucasny_prikaz][0]){                                                                  //Prep�na� pou�itia pr�kazu
      case 'i':
          strcat(insert,prikaz[sucasny_prikaz]+1);                                                      //Ukladanie hodnoty do premennej "insert"
          strcat(insert,"\n");
          break;
      case 'b':
          strcat(before,prikaz[sucasny_prikaz]+1);                                                      //Ukladanie hodnoty do premennej "before"
          break;
      case 'a':
          strcat(after,prikaz[sucasny_prikaz]+1);                                                       //Ukladanie hodnoty do premennej "after"
          break;
      case 'd':
          vymaz=strtol(prikaz[sucasny_prikaz]+1,NULL, 10);                                              //Riadky na vymazanie (po�et)
          if (vymaz==0) sucasny_riadok++;                                                               //Bez hodnoty => vymazanie 1 riadku
          else sucasny_riadok=sucasny_riadok+vymaz;                                                     //S hodnotou  => vymazanie "vymaz" riadkov
          break;
      case 'r':
          spoj=true;                                                                                    //Ozna�enie riadku, �e nem� by� ukon�en� "\n"
          break;
      case 'n':
          posun=strtol(prikaz[sucasny_prikaz]+1,NULL, 10);                                              //Riadky na vyp�sanie (po�et)
          if (posun==0){                                                                                //Bez hodnoty => v�pis 1 riadku
            string_connect(vystup[sucasny_riadok],vstup[sucasny_riadok],                                //Volanie funkcie "Spoj v�sledky"
                           insert,before,after,spoj);

            if (strlen(vystup[sucasny_riadok])>max_znaky_na_riadok){                                    //Kontrola: D�ka riadkov
              presah_riadkov(max_znaky_na_riadok);                                                      //Volanie funkcie "Spoj v�sledky"
              return TOO_LONG_SENTENCES;                                                                //Ukon�enie programu s chybou
            }

            printf("%s",vystup[sucasny_riadok++]);                                                      //V�pis riadku a n�sledn� posun
          }
          else {                                                                                        //S hodnotou  => v�pis "posun" riadkov
            int posunutie;
            for (posunutie=sucasny_riadok;posunutie<posun+sucasny_riadok;posunutie++) {                 //Cyklus od s��asn�ho po cie�ov� riadok
              string_connect(vystup[posunutie],vstup[posunutie],                                        //Volanie funkcie "Spoj v�sledky"
                             insert,before,after,spoj);

                if (strlen(vystup[sucasny_riadok])>max_znaky_na_riadok){                                //Kontrola: D�ka riadkov
                  presah_riadkov(max_znaky_na_riadok);                                                  //Volanie funkcie "Spoj v�sledky"
                  return TOO_LONG_SENTENCES;                                                            //Ukon�enie programu s chybou
                }

              printf("%s",vystup[posunutie]);                                                           //V�pis "posun" riadkov
            }
            sucasny_riadok=posunutie;                                                                   //Posun riadkov
          }
          strcpy(insert,"");                                                                            //Vynulovanie premennej "insert"
          strcpy(before,"");                                                                            //Vynulovanie premennej "before"
          strcpy(after,"");                                                                             //Vynulovanie premennej "after"
          if (spoj==true) spoj=false;                                                                   //Vynulovanie premennej "spoj"
          break;
      case 'q':
          return ALL_OK;                                                                                //Ukon�enie programu
      case 'g':
        skoc_na_riadok=strtol(prikaz[sucasny_prikaz]+1,NULL, 10)-1;                                     //Cie�ov� riadok
        if (skoc_na_riadok<0 || skoc_na_riadok>riadok_prikazu) {                                        //Kontrola: Hodnota pr�kazu "goto"
          printf("V prikaze 'goto' bola zadana nespravna hodnota.\n");                                  //Hl�senie o chybe
          return ERROR_GO_TO_VALUE;                                                                     //Ukon�enie programu s chybou
        }
        else if (skoc_na_riadok_kontrola[sucasny_prikaz]==true) break;                                  //Kontrola: zacyklenie
        else {
          skoc_na_riadok_kontrola[sucasny_prikaz]=true;                                                 //Zabr�nenie zacykleniu
          sucasny_prikaz=skoc_na_riadok-1;                                                              //Skok na dan� riadok(-1 pre inkrement�ciu cyklu)
        }
        break;
      case '\0':                                                                                        //Ignorovanie pr�zdnych riadkov v pr�kazoch
          break;
      default:                                                                                          //Kontrola: Nezn�my pr�kaz
          printf("Bol pouzity neznamy prikaz.\n");                                                      //Hl�senie o chybe
          return ERROR_UNEXPECTED_ARGUMENT;                                                             //Ukon�enie programu s chybou
    }
  }


  string_connect(vystup[sucasny_riadok],vstup[sucasny_riadok],                                          //Volanie funkcie "Spoj v�sledky"
                 insert,before,after,spoj);

  if (strlen(vystup[sucasny_riadok])>max_znaky_na_riadok){                                              //Kontrola: D�ka riadkov
    presah_riadkov(max_znaky_na_riadok);                                                                //Volanie funkcie "Spoj v�sledky"
    return TOO_LONG_SENTENCES;                                                                          //Ukon�enie programu s chybou
  }

  printf("%s",vystup[sucasny_riadok++]);                                                                //Vyp�s posledn�ho upravovan�ho riadku


  if (sucasny_riadok<riadok_vstupu){                                                                    //Kontrola: Vyp�san� v�etky riadky
    for (int vypis=sucasny_riadok;vypis<=riadok_vstupu;vypis++){
      printf("%s\n",vstup[vypis]);                                                                      //V�pis neupravovan�ch riadkov
    }
  }

  else {
    if(prikaz[riadok_prikazu-1][0]=='r') printf("\n");                                                  //Ukon�enie posledn�ho riadku "\n"
  }                                                                                                     //za ka�d�ch okolnost�


  return ALL_OK;                                                                                        //Ukon�enie programu
}

void help(int argc){                                                                                    //N�poveda
  if      (argc<2) printf("Skus pridat argumenty. Pozri napovedu:\n");                                  //Zis�ovanie d�vodu volania n�povedy
  else             printf("Skus odobrat argumenty. Pozri napovedu:\n");

  printf("\n---------------------------------------------------------------------------------\n"        //V�pis n�povedy
          "X                            Toto je textovy editor.                            X\n"
          "X             Pre spravne fungovanie ma vstup vyzerat nejako takto:             X\n"
          "X                        ./proj1 meno_suboru_s_prikazmi                         X\n"
          "X              meno_suboru_s_prikazmi znamena ze mame nejaky subor,             X\n"
          "X              v ktorom su prikazy potrebne pre fungovanie suboru.              X\n"
          "X                  Zoznam moznych prikazov najdete na stranke:                  X\n"
          "X  https://wis.fit.vutbr.cz/FIT/st/cwk.php?title=Projekt1&csid=678178&id=12816  X"
          "\n---------------------------------------------------------------------------------\n"
          "X            Roman Fulla 1.rocnik FIT 2018 - Projekt 1 - Praca s textom         X"
          "\n---------------------------------------------------------------------------------\n"
        );
}


void presah_riadkov (int max_znaky_na_riadok) {                                                         //Pr�li� dlh� riadky
    printf("Niektory riadok / riadky su prilis dlhe.\n"                                                 //Hl�senie o chybe
           "Zmente svoje parametre alebo vstup tak, aby na riadku\n"
           "nebolo viac znakov ako %i.\n", max_znaky_na_riadok-1);
}


void string_connect(char *vystup, char *vstup,char *insert,char *before,char *after,bool spoj){         //Spoj v�sledky
                    strcpy(vystup,insert);                                                              //Sp�janie jednotliv�ch pr�kazov
                    strcat(vystup,before);                                                              //do jedin�ho v�stupu
                    strcat(vystup,vstup);
                    strcat(vystup,after);
  if (spoj==false)  strcat(vystup,"\n");                                                                //Ak nebol pou�it� pr�kaz "remove", ukon�enie riadku
}
