#include<stdio.h>  
#include<ctype.h>
#include<string.h>
#include<malloc.h>
#include <stdlib.h>
#include <math.h>



#define NAME 207  /*Max characters allowed for the name of the input file.*/

#define MAX 80	  /*Max characters allowed in one line.*/

#define DATA 0
#define STRING 1
#define ENTRY 2
#define EXTERN 3

#define no_ins 16
#define no_dir 4
#define no_arg 6
#define no_operand 4
#define no_reg 16
#define no_ext 2

#define forever for( ; ; ) /* infinite loop.*/

#define HASHSIZE 101
#define MAX_LABEL_LENGTH 30
#define MAX_MACRO_LENGTH 99


/*------------------------------------STRUCTURES------------------------------------------*/

struct keywords {
	char *word;
};
	 
struct boolian{
	unsigned int flag : 1;
};

struct nlist{
	struct nlist *next;
	char *name;
	int value;
};

struct mlist{
	struct mlist *next;
	char *name;
	char **macroString;

};



/*--------------------- GLOBAL VARIABLES ------------------------- */

int ic, dc, ac ;  /* Global variables for "instruction counter", "data counter" and "address counter" (respectively).*/
int er;
int error  ;  /* check errors  */
int macro;
int macroLine;
char* mstring[MAX];

char LABEL[MAX_LABEL_LENGTH];
char MACRO[MAX_MACRO_LENGTH];

int inscode   ;
int dirtype   ;
int regist    ;
int extFlag       ;





/* Function declaration */

/* preprocessor.c */
int preProcessor(char* filename);
int readMacroLine(char* line, FILE* am);

/* first_pass.c */
int assemble(char *);
int readLine(char *);
int checkExt(char *);
void asExtens(char *, char *);
void addExtens(char *, char *, char *);
int findDir(char *);
int findReg(char *);
int findIndexReg(char *p);
int isIndexLabel(char* line);
char *skipSpace(char *);
int lookForLabel(char *);
int lookForDir(char *);
int lookForIns(char *);
int dirToValue(char *, int );
int insToValue(char *, int );
int getAddressingMode(char *);
char *getComma(char *);
int isLabel(char *);
char *nextField(char *);
int isNumber(char *);
int lengthToBlank(char * );
int cont(char *);
int ignore(char *);
/* error.c */
void writeError(int);
int writeError2(int, char *);

/* second_pass.c  */
char* insertFlag(char* str, int flag);
int charToNum(char *line);
char* fullDecToBinary(int num, int);
int getFunct(int code);
char decToHex(int num);
int binCharToNum(char* str);
char* decToBinary(int num, int size);
int print(FILE* ob, int code, int flag);
int dirOb(FILE *,char *);
int writeToFiles(FILE *,FILE *,FILE *,char *,int);
int secondPass(char*,char*);
int writeToEnt(FILE *, char *);
char *cpyarg(char *,char);
int wrt(FILE *, FILE * , char *);
int addVal(FILE *, char *,int *,int);
int regFromIndex(char*);

/* MACRO_tab.c */
struct mlist *lookForMacro(char *s);
int installMacro(char *name);
void cleanMacroTab();

/* LAB_tab.c */
int install(char *, int);
struct nlist *lookup(char *);
unsigned hash(char *);
void cleanTab();
/* en_tab.c  */
int installEntry(char *);
struct nlist *lookForEntry(char *);
void cleanEnTab();
char *strDup(char *);
/* ex_tab.c  */
int installExt(char *);
struct nlist *lookForExt(char *);
void cleanExTab();
