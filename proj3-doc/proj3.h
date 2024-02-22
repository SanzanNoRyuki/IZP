/**
 * @mainpage
 * Dokumentácia zdrojového kódu:\n
 * Projekt 3 - Jednoduchá zhluková analýza\n
 * \n
 * Roman Fulla 1.ročník FIT 2018\n
 */

/**
 * @file Proj3.h
 * @brief Hlavičkový súbor k tretiemu projektu
 * 
 * Súbor, v ktorom sú popísané jednotlivé súčasti tretieho projektu.
 */

/** 
 * @defgroup ERRORS Výstupy
 * @brief Všetky predpokladané výstupné hodnoty programu.
 * @{
 */
 
enum ERROR {
  ALL_OK,						/**<Program úspešne skončil.*/
  ARGC_ERR,						/**<Bol zadaný nesprávny počet argumentov.*/
  LOADCF_ERR,						/**<Nastala chyba vo funkcii <a href="group__FUNCTIONS.html#ga9d928923d43120cb53ddff210a087061">load_clusters</a>.*/
};

/**@}*/

/**
 * @defgroup CONSTANTS Konštanty
 * @brief Konštantné hodnoty programu.
 * @{
 */

extern const int CLUSTER_CHUNK;				/**<Kus zhluku <a href="structobj__t.html">objektov</a> - odporúčaná hodnota pre realokáciu.*/

/**@}*/

/** 
 * @defgroup STRUCTS Dátové štruktúry
 * @brief Vlastné dátové štruktúry používané v programe.
 * @{
 */

/**
 * @brief Objekt
 * 
 * Štruktúra objektu.
 */
struct obj_t {
    int id;						/**<identifikátor*/
    float x;						/**<súradnica X*/
    float y;						/**<súradnica Y*/
};

/**
 * @brief Zhluk <a href="structobj__t.html">objektov</a>.
 * 
 * Štruktúra zhluku objektov.
 */
struct cluster_t {
    int size;						/**<počet objektov v zhluku*/
    int capacity;					/**<kapacita zhluku (pre koľko objektov je rezervované miesto v zhluku)*/
    struct obj_t *obj;					/**<ukazateľ na pole objektov*/
};

/**@}*/

/**
 * @defgroup FUNCTIONS Funkcie
 * @brief Vlastné funkcie používané v programe.
 * @{
 */

/**
 * @brief Funkcia na inicializáciu <a href="structcluster__t.html">zhluku</a>.
 * 
 * Alokuje potrebnú pamäť pre nový zhluk.
 * 
 * @param *c - ukazateľ na zhluk, ktorý inicializujeme
 * @param cap - požadovaná kapacita zhluku
 * 
 * @return Funkcia nič nevracia.
 * 
 * @pre Ukazateľ na zhluk nie je NULL ( *c != NULL )
 * @post Funkcia alokuje pamäť pre "cap" <a href="structobj__t.html">objektov</a> a kapacitu nastaví na "cap".
 */
void init_cluster(struct cluster_t *c, int cap);
/**
 * @brief Funkcia na vyčistenie <a href="structcluster__t.html">zhluku</a>.
 * 
 * Odstráni všetky <a href="structobj__t.html">objekty</a> v zhluku a <a href="group__FUNCTIONS.html#ga96db0862471d90abb3d80103ef3695f7">inicializuje</a> ho na kapacitu 0.
 * 
 * @param *c - ukazateľ na zhluk, ktorý čistíme
 * 
 * @returns Funkcia nič nevracia.
 */
void clear_cluster(struct cluster_t *c);
/**
 * @brief Funkcia na vyčistenie viacerých <a href="structcluster__t.html">zhlukov</a>.
 * 
 * Vyčistí "n" zhlukov tým, že opakovane volá <a href="group__FUNCTIONS.html#ga8289f092f205baeb13cf33cfffa15324">funkciu na vyčistenie zhluku</a>.\n
 * Následne odstráni samotné zhluky.
 * 
 * @param *c - ukazateľ na zhluky, ktoré čistíme
 * @param n - počet zhlukov, ktoré čistíme
 * 
 * @returns Funkcia nič nevracia.
 */
void clear_clusters(struct cluster_t *c, int n);
/**
 * @brief Funkcia na zväčšenie kapacity <a href="structcluster__t.html">zhluku</a>.
 * 
 * Ak má zhluk kapacitu menšiu ako "new_cap" rozšíri ju na "new_cap".
 * 
 * @param *c - ukazateľ na zhluk, ktorého kapacitu rozširujeme
 * @param new_cap - nová požadovaná kapacita zhluku
 * 
 * @return Funkcia vracia NULL ak realokácia zlyhá.
 * @return Funkcia vracia ukazateľ na rozšírený zhluk.
 * 
 * @pre Kapacita zhluku je menšia ako "new_cap"
 * @post Kapacita zhluku je rozšírená na "new_cap".
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);
/**
 * @brief Funkcia na rozšírenie <a href="structcluster__t.html">zhluku</a> o jeden <a href="structobj__t.html">objekt</a>.
 * 
 * Pridá objekt "obj" na koniec zhluku. Ak je kapacita nedostatočná, <a href="group__FUNCTIONS.html#ga0d8702f8bee3bccb81380e012a615a3d">rozšíri ju</a>.
 * 
 * @param *c - ukazateľ na zhluk, ku ktorému objekt pridávame
 * @param obj - objekt, ktorý ku zhluku pridávame
 * 
 * @return Funkcia nič nevracia.
 * 
 * @pre Zhluk nemá dostatočnú kapacitu
 * @post Kapacita zhluku je rozšírená.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);
/**
 * @brief Funkcia na zoradenie <a href="structobj__t.html">objektov</a> v <a href="structcluster__t.html">zhluku</a>.
 * 
 * Zoradí objekty v zhluku vzostupne podľa ich identifikátorov.
 * 
 * @param *c - ukazateľ na zhluk, ktorý usporiadavame
 * 
 * @return Funkcia nič nevracia.
 */
void sort_cluster(struct cluster_t *c);
/**
 * @brief Funkcia na spojenie dvoch <a href="structcluster__t.html">zhlukov</a> do jedného.
 * 
 * <a href="group__FUNCTIONS.html#gab6269b64b2c7f4842a51c58b2d86a2e5">Pripojí</a> všetky <a href="structobj__t.html">objekty</a> z druhého zhluku do prvého.\n
 * Náslende objekty v zhluku <a href="group__FUNCTIONS.html#ga12718af9e3e31b61bf703ec260e22450">zoradí</a>.\n
 * Druhý zhluk ostane nezmenený.
 * 
 * @param *c1 - ukazateľ na zhluk, do ktorého ukladáme všetky objekty
 * @param *c2 - ukazateľ na nezmenený zhluk
 * 
 * @return Funkcia nič nevracia.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);
/**
 * @brief Funkcia na odstránenie <a href="structcluster__t.html">zhluku</a>.
 * 
 * Odstráni zhluk z poľa zhlukov. 
 * 
 * @param *carr - ukazateľ na pole zhlukov
 * @param narr - počet zhlukov v poli
 * @param idx - index zhluku ktorý odstraňujeme
 * 
 * @return Funkcia vracia nový počet zhlukov v poli.
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);
/**
 * @brief Funkcia na vypočítanie vzdialenosti medzi dvomi <a href="structobj__t.html">objektmi</a>.
 * 
 * Vypočíta Euklidovskú vzdialenosť medzi dvomi objektmi.
 * 
 * @param *o1 - ukazateľ na prvý objekt
 * @param *o2 - ukazateľ na druhý objekt
 * 
 * @returns Funkcia vracia vzdialenosť medzi objektmi "o1" a "o2".
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);
/**
 * @brief Funkcia na vypočítanie vzdialenosti medzi dvomi <a href="structcluster__t.html">zhlukmi</a>.
 * 
 * Vypočíta vzdialenosť medzi dvomi zhlukmi tým, že <a href="group__FUNCTIONS.html#ga703ce6afddf65026b68645bc5394405b">porovnáva vzdialenosti medzi ich objektmi</a>.
 * 
 * @param *c1 - ukazateľ na prvý zhluk
 * @param *c2 - ukazateľ na druhý zhluk
 * 
 * @returns Funkcia vracia vzdialenosť medzi zhlukmi "c1" a "c2".
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);
/**
 * @brief Funkcia na nájdenie dvoch najbližších <a href="structcluster__t.html">zhlukov</a>.
 * 
 * Nájde dva najbližšie zhluky v poli.
 * 
 * @param *carr - ukazateľ na pole zhlukov
 * @param narr - počet zhlukov v poli
 * @param *c1 - ukazateľ na prvý najbližší zhluk
 * @param *c2 - ukazateľ na druhý najbližší zhluk
 * 
 * @return Funkcia nič nevracia.
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);
/**
 * @brief Funkcia na vypísanie <a href="structcluster__t.html">zhluku</a>.
 * 
 * Vytlačí zhluk na štandartný výstup.
 * 
 * @param *c - ukazateľ na zhluk, ktorý tlačíme
 * 
 * @return Funkcia nič nevracia.
 */
void print_cluster(struct cluster_t *c);
/**
 * @brief Funkcia na načítanie <a href="structobj__t.html">objektov</a> zo súboru.
 * 
 * Načíta zadané objekty zo súboru a uloží ich do samostatných <a href="structcluster__t.html">zhlukov</a>. 
 * 
 * @param filename - názov súboru, z ktorého chceme objekty načítame
 * @param **arr - ukazateľ na pole zhlukov kam výslendé objekty ukladáme
 * 
 * @returns Funkcia vracia počet načítaných objektov.
 * 
 * @pre Nastala nejaká chyba
 * @post Ukazateľ na pole zhlukov je nastavený na NULL a je načítaných 0 objektov.
 */
int load_clusters(char *filename, struct cluster_t **arr);
/**
 * @brief Funkcia na vypísanie viacerých <a href="structcluster__t.html">zhlukov</a>.
 * 
 * Vytlačí "narr" zhlukov na štandartný výstup.
 * 
 * @param *carr - ukazateľ na zhluky, ktoré tlačíme
 * @param narr - počet zhlukov ktoré tlačíme
 * 
 * @return Funkcia nič nevracia.
 */
void print_clusters(struct cluster_t *carr, int narr);

/**@}*/

