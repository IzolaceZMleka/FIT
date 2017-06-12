/*Projekt c.2 pro predmet IJC - Jazyk C
*Autor: Jan Pavlica - 1BIT, VUT FIT
*       xpavli78@stud.fit.vutbr.cz
*Datum: 27.4.2015
*Prelozeno: gcc 4.5.2
*Popis: opsano ze zadani, hashovaci funkce
*/

unsigned int hash_function(const char *str, unsigned htab_size)
{
  unsigned int h=0;
  const unsigned char *p;
  for(p=(const unsigned char*)str; *p!='\0'; p++)
  {
    h = 65599*h + *p;  
  }
  return h % htab_size;
}
