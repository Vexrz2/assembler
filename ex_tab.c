/* ================= ex_tab.c =======================*/ 

#include "asm.h"

static struct nlist *exttab[HASHSIZE] ;

/* lookForExt: looks for s in exttab */
struct nlist *lookForExt(char *s){
	struct nlist *np;
	for (np=exttab[hash(s)]; np != NULL; np = np -> next ){
		if (strcmp(s, np->name)== 0)
			(np->value)++;
			return np; /* found */
	}
	return NULL;
}

/* installExt: puts name in exttab */

int installExt(char *name){
	struct nlist *np;
	unsigned hashval;
	if ( (np=lookForExt(name))== NULL){ /* name was not found, assigning np=name */
		np = (struct nlist *) malloc(sizeof(*np));
		if ( np == NULL ||  (np->name =strDup(name)) == NULL){
			printf("Out of memory");
			return 2; /* out of memory. */
		}
		hashval = hash(name);
		np -> next = exttab[hashval];
		exttab[hashval]= np;
		(np->value)=0 ;
		return 1;  /* Successfully added */
	}
	else /* already there */
		return 0; 
}
	

/*------------- cleanExTab ----------------------------
cleanExTab: cleaning  the external symbol table.
-----------------------------------------------------
*/
void cleanExTab(){
	struct nlist *np, *u ;
	int i=0;
	for( i=0; i<HASHSIZE; i++){
		np=exttab[i]; 
		while(np != NULL){
			u=np->next ;
			free(np->name);
			free(np);
			np=u;
		}
		exttab[i]=NULL;
	}
}
