/******************************************************************************************************************************************************************************/
/***                                                 Roman Fulla 1.rocnik FIT 2018 - Projekt 3 - Jednoducha shlukova analyza                                                ***/
/***                                                                           Verzia - 2.2                                                                                 ***/
/******************************************************************************************************************************************************************************/

/***/
///#define NDEBUG                                                               //DEBUG alebo NDEBUG (alebo pouzitim argumentu pri preklade suboru)
/***/

/***INCLUDE***/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>                                                               //"Z dovodu sqrtf"
#include <limits.h>                                                             //"Z dovodu INT_MAX"

/***DEFINE***/

enum ERROR {                                                                    //Mozne predpokladane vystupy:
  ALL_OK,                                                                       //Spravny priebeh programu
  ARGC_ERR,                                                                     //Nespravny pocet argumentov
  LOADCF_ERR,                                                                   //Chyba pri funkcii 'load_clusters'
};

#ifdef NDEBUG                                                                   //NoDebug:
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)

#else                                                                           //Debug:
#define debug(s) printf("- %s\n", s)                                            //"Vypise ladiaci retazec"
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)     //"Vypise formatovany ladiaci vystup - pouzitie podobne ako printf"
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)        //"Vypise ladiacu informaciu o premennej - pouzitie dint (identifikator premennej)"
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)      //"Vypise ladiacu informaciu o premennej typu float - pouzitie dfloat (identifikator premennej)"
#endif

/***DATOVE TYPY***/

const int CLUSTER_CHUNK = 10;                                                   //"Kus zhluku objektov - odporucana hodnota pre realokaciu"

struct obj_t {                                                                  //"Struktura objektu:"
  int  id;                                                                      //"identifikator"
  float x;                                                                      //"suradnica X"
  float y;                                                                      //"suradnica Y"
};

struct cluster_t {                                                              //"Struktura zhluku objektov:"
  int          size;                                                            //"Pocet objektov v zhluku"
  int      capacity;                                                            //"Kapacita zhluku (pre kolko objektov je rezervovane miesto v poli)"
  struct obj_t *obj;                                                            //"Ukazatel na pole objektov"
};

/***DEKLARACIA FUNKCII***/

void init_cluster(struct cluster_t *c, int cap);                                //Funkcia na inicializaciu zhluku
void clear_cluster(struct cluster_t *c);                                        //Funkcia na vycistenie zhluku
void clear_clusters(struct cluster_t *c, int n);                                //Funkcia na vycistenie viacerych zhlukov
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);             //Funkcia na rozsirenie kapacity zhluku
void append_cluster(struct cluster_t *c, struct obj_t obj);                     //Funkcia na rozsirenie zhluku o objekt
void sort_cluster(struct cluster_t *c);                                         //Funkcia na zoradenie objektov v zhluku
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);                //Funkcia na spojenie dvoch zhlukov

/***DEKLARACIA FUNKCII PRE PRACU S POLOM ZHLUKOV***/

int remove_cluster(struct cluster_t *carr, int narr, int idx);                  //Funkcia na odstranenie zhluku z pola
float obj_distance(struct obj_t *o1, struct obj_t *o2);                         //Funkcia na vypocet vzdialenosti dvoch objektov
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);             //Funkcia na vypocet vzdialenosti dvoch zhlukov
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);       //Funkcia na najdenie dvoch najblizsich zhlukov
static int obj_sort_compar(const void *a, const void *b);                       //Pomocnná funkcia pre radenie zhlukov
void print_cluster(struct cluster_t *c);                                        //Funkcia na vypisanie zhluku
int load_clusters(char *filename, struct cluster_t **arr);                      //Funkcia na nacitanie zhlukov zo suboru
void print_clusters(struct cluster_t *carr, int narr);                          //Funkcia na vypisanie viacerych zhlukov

/***FUNKCIE***/

int main(int argc, char *argv[]) {                                              //HLAVNA funkcia

  if (argc<2 || argc>3) {                                                       //Kontrola: Nespravny pocet argumentov
    fprintf(stderr, "err: wrong argument input\n");                             //Hlasenie o chybe
    return ARGC_ERR;                                                            //Ukoncenie programu s chybou
  }

  struct cluster_t *clusters;                                                   //Pole zhlukov
  int cl_1,cl_2;                                                                //Pomocne premenne pre niektore funkcie

  int cluster_count = load_clusters(argv[1],&clusters);                         //Nacitanie zhlukov zo suboru
  if (clusters == NULL) {                                                       //Kontrola: Nieco sa pri nacitani pokazilo
    return LOADCF_ERR;                                                          //Ukoncenie programu s chybou
  }

  int wanted_clusters = 1;                                                      //Citanie poziadavky poctu vyslednych zhlukov
  if (argc == 3 && argv[2][0] != '-'){                                          //Kontrola: Spravne zadanny vstup
    wanted_clusters = strtoul(argv[2],NULL,10);                                 //Zapis do premennej 'wanted_clusters'
    if      (wanted_clusters > cluster_count) wanted_clusters = cluster_count;  //Kontrola: Pozadovanie viacerych zhlukov ako je v subore
    else if (wanted_clusters == 0) wanted_clusters = 1;                         //Kontrola: Pri nespravnom zapise sa pokracuje s poziadavkov jedineho zhluku
  }

  while (cluster_count > wanted_clusters){                                      //Kym nemame pozadovany pocet zhlukov:
                    find_neighbours(clusters,cluster_count,&cl_1,&cl_2);        //Najdenie dvoch najblizsich zhlukov
                    merge_clusters(&(clusters[cl_1]),&(clusters[cl_2]));        //Spojenie dvoch najblizsich zhlukov
    cluster_count = remove_cluster(clusters,cluster_count,cl_2);                //Odstranenie prebytocneho zhluku a vratenie noveho poctu zhlukov
  }

  print_clusters(clusters,cluster_count);                                       //Vypisanie zhlukov
  clear_clusters(clusters,cluster_count);                                       //Vycistenie zhlukov

  return ALL_OK;                                                                //Ukoncenie programu
}

/***POMOCNE FUNKCIE***/

void init_cluster(struct cluster_t *c, int cap) {                               //Inicializacia zhluku 'c'

  assert(c != NULL);
  assert(cap >= 0);

  if (c != NULL && cap > 0) {                                                   //Kontrola: Spravny vstup
    c->obj      = malloc (sizeof(struct obj_t) * cap);                          //Alokacia pamate pre 'cap' objektov
    c->capacity = cap;                                                          //Kapacita zhluku je 'cap'
    c->size = 0;                                                                //Pociatocna velkost zhluku je 0

    if (c->obj == NULL) {                                                       //Kontrola: Alokacia sa nepodarila
      c->capacity = 0;                                                          //Ziadna kapacita
    }
  }

  else {                                                                        //Nespravny vstup:
    c->obj = NULL;                                                              //Ziadne objekty
    c->capacity = 0;                                                            //Ziadna kapacita
    c->size = 0;                                                                //Ziadna velkost
  }
}

void clear_cluster(struct cluster_t *c) {                                       //Vycistenie zhluku

  free(c->obj);                                                                 //Dealokacia pola objektov
  init_cluster(c,0);                                                            //Inicializacia na prazdny zhluk
}

void clear_clusters(struct cluster_t *c, int n) {                               //Vycistenie 'n' zhlukov

  for (int i=0;i<n;i++){                                                        //Opakovanie 'n'-krat
    clear_cluster(&c[i]);                                                       //Vycistenie zhluku
  }
  free(c);                                                                      //Vycistenie pola zhlukov
}

struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap) {            //"Zmena kapacity zhluku 'c' na kapacitu 'new_cap'"

  assert(c);
  assert(c->capacity >= 0);
  assert(new_cap >= 0);

  if (c->capacity >= new_cap) return c;

  size_t size = sizeof(struct obj_t) * new_cap;

  void *arr = realloc(c->obj, size);
  if (arr == NULL) return NULL;

  c->obj = (struct obj_t*)arr;
  c->capacity = new_cap;
  return c;
}

void append_cluster(struct cluster_t *c, struct obj_t obj) {                    //Pridanie objektu 'obj' na koniec zhluku 'c'

  if (c->capacity == c->size) {                                                 //Kontrola: Kapacita je nedostatocna pre dalsi objekt
    struct cluster_t *err = c;                                                  //Uchovanie adresy 'c' pre pripad ze sa alokacia nepodari do pomocnej premennej 'err'
    c = resize_cluster(c,c->capacity+CLUSTER_CHUNK);                            //Rozsirenie kapacity o 'CLUSTER_CHUNK'
    if (c == NULL) {                                                            //Kontrola: Realokacia sa nepodarila
      c = err;                                                                  //Zhluk sa nezmeni
      fprintf(stderr,"err: object ID: %i - lost (realloc error)\n", obj.id);    //Sprava o stratenom objekte
      return;                                                                   //Ukoncenie funkcie
    }
  }

  c->obj[c->size] = obj;                                                        //Pridanie objektu na koniec zhluku
  c->size++;                                                                    //Zvacsenie poctu objektov o jeden
}

void merge_clusters(struct cluster_t *c1, struct cluster_t *c2) {               //Pridanie objektov zo zhluku 'c2' do zhluku 'c1'

  assert(c1 != NULL);
  assert(c2 != NULL);

  for (int i = 0; i < c2->size; i++){                                           //Opakovanie od prveho az po posledny objekt zhluku 'c2'
    append_cluster(c1,c2->obj[i]);                                              //Pridanie objektu do prveho zhluku
  }

  sort_cluster(c1);                                                             //Zoradenie objektov v zhluku 'c1'
}

/***FUNKCIE PRE PRACU S POLOM ZHLUKOV***/

int remove_cluster(struct cluster_t *carr, int narr, int idx) {                 //Odstranenie zhluku na indexe 'idx' z pola zhlukov 'carr', ktore obsahuje 'narr' zhlukov

  assert(idx < narr);
  assert(narr > 0);

  clear_cluster(&carr[idx]);                                                    //Vycistenie nechceneho zhluku

                                                                                //Posunutie ostatnych prvkov v poli
  for (int i = idx; i < narr - 1; i++){                                         //Opakovanie od indexu az po predposledny prvok pola
    carr[i] = carr[i+1];                                                        //Presunutie zhluku z vyssieho indexu do nizsieho
  }

  return narr-1;                                                                //Vratenie noveho poctu zhlukov
}

float obj_distance(struct obj_t *o1, struct obj_t *o2) {                        //Vypocet Euklidovskej vzdialenosti medzi dvomi objektmi

  assert(o1 != NULL);
  assert(o2 != NULL);

  return sqrtf (powf ((o1->x - o2->x),2.0) +  powf ((o1->y - o2->y),2.0));      //Vzdialenost = ((X1 - X2) na 2 + (Y1 - Y2) na 2) na 1/2
}

float cluster_distance(struct cluster_t *c1, struct cluster_t *c2) {            //Vypocet vzdialenosti medzi dvomi zhlukmi

  assert(c1 != NULL);
  assert(c1->size > 0);
  assert(c2 != NULL);
  assert(c2->size > 0);

  float min_distance = INT_MAX;                                                 //Minimalna vzdialenost

  for (int i = 0; i < c1->size; i++) {                                          //Opakovanie pre vsetky objekty 'c1'
      for (int j = 0; j < c2->size; j++){                                       //Opakovanie pre vsetky objekty 'c2'
        float distance = obj_distance(&c1->obj[i],&c2->obj[j]);                 //Vypocet vzdialenosti

        if (distance < min_distance) {                                          //Kontrola: Vzdialenost je mensia ako doteraz minimalna vzdialenost
          min_distance = distance;                                              //Aktualizacia minimalnej vzialenosti
        }
      }
  }
  return min_distance;                                                          //Vratenie minimalnej vzialenosti
}

void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2) {      //Hladanie dvoch najblizsich zhlukov z pola 'carr' o velkosti 'narr'

  assert(narr > 0);

  float min_distance=INT_MAX;                                                   //Minimalna vzdialenost zhlukov

  for (int i = 0; i < narr-1; i++) {                                            //Opakuj od prveho po predposledny zhluk
    for (int j = i+1; j < narr; j++){                                           //Opakuj od druheho po posledny zhluk
      float distance = cluster_distance(&carr[i],&carr[j]);                     //Zistovanie vzdialenosti

      if (distance < min_distance){                                             //Kontrola: Vzdialenost je mensia ako doteraz minimalna vzdialenost
        min_distance = distance;                                                //Aktualizacia minimalnej vzdialenosti
        *c1 = i;                                                                //Zaznamenanie indexu prveho z 2 najblizsich zhlukov
        *c2 = j;                                                                //Zaznamenanie indexu druheho z 2 najblizsich zhlukov
      }
    }
  }
}

static int obj_sort_compar(const void *a, const void *b) {                      //"Pomocna funkcia pre radenie zhlukov"

  const struct obj_t *o1 = (const struct obj_t *)a;
  const struct obj_t *o2 = (const struct obj_t *)b;
  if (o1->id < o2->id) return -1;
  if (o1->id > o2->id) return 1;
  return 0;
}

void sort_cluster(struct cluster_t *c) {                                        //"Radenie objektov v zhluku vzostupne podla ich identifikatoru"

  qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

void print_cluster(struct cluster_t *c) {                                       //"Tlacenie zhluku 'c' na vystup"

  for (int i = 0; i < c->size; i++) {
    if (i) putchar(' ');
    printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
  }
  putchar('\n');
}

int load_clusters(char *filename, struct cluster_t **arr) {                     //Nacitanie objektov zo suboru 'filename'

  assert(arr != NULL);

  FILE* input_file = fopen(filename,"r");                                       //Otvorenie vstupneho suboru
  if (input_file == NULL) {                                                     //Kontrola: Subor sa nepodarilo otvorit
    fprintf(stderr, "err: %s cannot be opened (input file)\n", filename);       //Hlasenie o chybe
    *arr = NULL;                                                                //Vratenie hodnoty NULL do 'arr'
    return 0;                                                                   //Ukoncenie funkcie (ziadny nacitany objekt)
  }

  int scan_value;                                                               //Pomocna premenna pre kontrolu nacitania vstupov
  int count_value;                                                              //Poctu zhlukov podla suboru
  int loaded_clusters;                                                          //Pocet nacitanych zhlukov
  struct obj_t tmp;                                                             //Pomocny objekt

  scan_value = fscanf(input_file,"count=%i",&count_value);                      //Zistovanie kolko zhlukov je v subore
  if (scan_value == 0 || count_value < 0) {                                     //Kontrola: Nespravny vstup
    fprintf(stderr, "err: input file is written incorrectly (count)\n");        //Hlasenie o chybe
    *arr = NULL;                                                                //Vratenie hodnoty NULL do 'arr'
    fclose(input_file);                                                         //Zatvorenie vstupneho suboru
    return 0;                                                                   //Ukoncenie funkcie (ziadny nacitany objekt)
  }

  *arr = malloc (sizeof(struct cluster_t) * count_value);                       //Alokacia miesta pre 'count_value' zhlukov
  if (*arr == NULL) {                                                           //Kontrola: Alokacia sa nepodarila
    fprintf(stderr, "err: input memory allocation failed\n");                   //Hlasenie o chybe
    fclose(input_file);                                                         //Zatvorenie vstupneho suboru
    return 0;                                                                   //Ukoncenie funkcie s chybou (ziadny nacitany objekt)
  }

  for (loaded_clusters = 0;loaded_clusters < count_value; loaded_clusters++){   //Opakovanie 'count_value'-krat
    init_cluster(*arr + loaded_clusters,CLUSTER_CHUNK);                         //Inicializacia 'count_value' zhlukov
    if ((*arr + loaded_clusters)->obj == NULL) {                                //Kontrola: Inicializacia sa nepodarila
      fprintf(stderr, "err: initialization error\n");                           //Hlasenie o chybe
      clear_clusters(*arr,loaded_clusters+1);                                   //Vycistenie zhlukov
      *arr = NULL;                                                              //Vratenie hodnoty NULL do 'arr'
      fclose(input_file);                                                       //Zatvorenie vstupneho suboru
      return 0;                                                                 //Ukoncenie funkcie s chybou (ziadny nacitany objekt)
    }

    scan_value = fscanf(input_file,"%i %g %g",&tmp.id,&tmp.x,&tmp.y);           //Vlozenie hodnot objektu do docasneho objektu
    if (scan_value == 3) {                                                      //Kontrola: Spravny format objektu
      append_cluster(*arr + loaded_clusters,tmp);                               //Pridanie objektu 'tmp' do zhluku
    }
    else {                                                                      //Kontrola: Nespravne zadany vstup
      fprintf(stderr, "err: input file is written incorrectly (clusters)\n");   //Hlasenie o chybe
      clear_clusters(*arr,loaded_clusters+1);                                   //Vycistenie zhlukov
      *arr = NULL;                                                              //Vratenie hodnoty NULL do 'arr'
      fclose(input_file);                                                       //Zatvorenie vstupneho suboru
      return 0;                                                                 //Ukoncenie funkcie s chybou (ziadny nacitany objekt)
    }
  }

  fclose(input_file);                                                           //Zatvorenie vstupneho suboru
  return loaded_clusters;                                                       //Vratenie poctu nacitanych objektov
}

void print_clusters(struct cluster_t *carr, int narr) {                         //"Tlacenie 'narr' zhlukov pola 'carr'"

  printf("Clusters:\n");

  for (int i = 0; i < narr; i++) {
    printf("cluster %d: ", i);
    print_cluster(&carr[i]);
  }
}
