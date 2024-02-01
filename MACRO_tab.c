/* ================= MACRO_tab.c =======================*/ 

#include "asm.h"

static struct mlist *macrotab[HASHSIZE];

/* lookForMacro: Searching s in macrotab */
struct mlist *lookForMacro(char *s){
	struct mlist *np;
	for (np=macrotab[hash(s)]; np != NULL;	np = np -> next ){
		if (strcmp(s, np->name)== 0)
			return np; /* found */
	}
	return NULL;
}


/* installMacro: Inserting the macro name into the hashtab */

int installMacro(char *name){
	struct mlist *np;
	unsigned hashval;
	if ( (np=lookForMacro(name))== NULL){ /* name was not found, assigning np=name */
		np = (struct mlist *) malloc(sizeof(*np));
		if ( np == NULL ||  (np->name =strDup(name)) == NULL){
			printf("Out of memory");
			return 2; /* out of memory.*/
		}
		hashval = hash(name);
		np -> next = macrotab[hashval];
		macrotab[hashval]= np;
		return 1;  /* successfully added */
	}
	else /* already there */
		return 0; 
}
	

/*---------------- cleanMacroTab ------------------------
cleanMacroTab: Cleans the "macro table".
-----------------------------------------------------
*/
void cleanMacroTab(){
	struct mlist *np, *u ;
	int i=0;
	for( i=0; i<HASHSIZE; i++){
		np=macrotab[i]; 
		while(np != NULL){
			u=np->next ;
			free(np->name);
			free(np);
			np=u;
		}
		macrotab[i]=NULL;
	}
}
