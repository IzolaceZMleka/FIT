/*Projekt c.2 pro predmet IJC - Jazyk C
*Autor: Jan Pavlica - 1BIT, VUT FIT
*       xpavli78@stud.fit.vutbr.cz
*Datum: 27.4.2015
*Prelozeno: gcc 4.5.2
*Popis: vlastni knihovna
*/

#ifndef MAP
#define MAP

struct htab_listitem {
  char *key;
  int data;
  struct htab_listitem *next;
};

typedef struct {   
  int htab_size;
  struct htab_listitem **list;
} htab_t;

htab_t* htab_init(int htab_size);
struct htab_listitem * htab_lookup(htab_t *hash_table, const char *key);
void htab_foreach(htab_t * hash_table, void (*function)(const char* key, const int value));
void htab_remove(htab_t * hash_table, const char *key);
void htab_clear(htab_t * hash_table);
void htab_free(htab_t * hash_table);
void htab_statistics(htab_t * hash_table);

#endif
