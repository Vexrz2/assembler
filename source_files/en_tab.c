/* ================= en_tab.c =======================*/ 

#include "asm.h"

static struct nlist *entrytab[HASHSIZE] ;

/*------------- strDup ----------------------------------
 * strDup: String Duplicate: The purpose of this function
 * is to return a duplicated string of its argument. 
 * -----------------------------------------------------*/
char *strDup(char *s) {
	char *p;
	p = (char *) malloc(strlen(s)+1); /* The  +1 is for the string ending char '\0' */
	if (p != NULL)
		strcpy(p, s);
	return p;
}



/*------------ lookForEntry ---------------
lookForEntry: looks for s in entrytab 
-----------------------------------------
*/
struct nlist *lookForEntry(char *s){
	struct nlist *np;
	for (np=entrytab[hash(s)]; np != NULL; np = np -> next ){
		if (strcmp(s, np->name)== 0)
			return np; /* found */
	}
	return NULL;
}

/*-------------------------------------------
 installEntry: puts name in entrytab 
---------------------------------------------
*/
int installEntry(char *name){
	struct nlist *np;
	unsigned hashval;
	if ( (np=lookForEntry(name))== NULL){ /* name was not found, assigning np=name */
		np = (struct nlist *) malloc(sizeof(*np));
		if ( np == NULL ||  (np->name =strDup(name)) == NULL){
			printf("Out of memory");
			return 2; /* out of memory.*/
		}
		hashval = hash(name);
		np -> next = entrytab[hashval];
		entrytab[hashval]= np;
		return 1;  /* succesfully added */
	}
	else /* already there */
		return 0; 
}



/*---------------------- cleanEnTab--------------------
cleanEnTab: cleans the entry symbol table.
----------------------------------------------------- */
void cleanEnTab(){
	struct nlist *np, *u ;
	int i=0;
	for( i=0; i<HASHSIZE; i++){
		np=entrytab[i]; 
		while(np != NULL){
			u=np->next ;
			free(np->name);
			free(np);
			np=u;
		}
		entrytab[i]=NULL;
	}
}
