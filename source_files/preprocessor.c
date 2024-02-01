#include "asm.h"
int preProcessor(char* filename) {
	char am_name[NAME] ;
	char as_name[NAME] ;
	char line[MAX];
	FILE* as; 
	FILE* am;
	
	macro = 0; /*macro flag*/
	macroLine = 0;
	
	asExtens(filename, as_name);
	as=fopen(as_name, "r");
	if(as == NULL) {
		printf("Incorrect file name. \n");
		return 0;
	}
	addExtens(filename, am_name, ".am");
	am = fopen(am_name, "w+");

	while(1) {
		/*------- Set a breaking point.------------------*/
		if(fgets(line,MAX,as)==NULL)
			break;		/* Finish reading file, exiting loop. */
		/*_______________________________________________*/

		readMacroLine(line, am);
	
	}
	fclose(as);
	fclose(am);
	return 1;
}




int readMacroLine(char* line, FILE* am) {
	
	/*----- Remarks and blank lines -----*/
	if (*line == ';') {
		fprintf(am, "%s", line);
		return 0 ;
	}
	line=skipSpace(line); 
	if (line == NULL) {
		fprintf(am, "\n");
		return 0;
	}
	/*___________________________________*/
	/*looking for macro*/
	char* u = cpyarg(line, ' ');
	if (lookForMacro(u)) {
		int i = 0;
		for (i = 0; i<macroLine; i++) {
			fprintf(am, "%s", mstring[i]);
			free(mstring[i]);
		}
		free(u);
		return 1;
	
	}
	else if (strlen(line) > 4 && !strncmp(line, "macro", 5)) {  /*found new macro definition */
		macroLine = 0;
		int i =0;
		macro = 1;
		line = nextField(line);
		while(!isspace(*(line+i))) {
			MACRO[i] = *(line+i);
			i++;
		}
		MACRO[i] = '\0';
		installMacro(MACRO);
		free(u);
		return 1;
	
	}
	else if(strlen(line) > 3 && strncmp(line, "endm", 4) && macro) {   /* in macro*/
		
		char* curLine = strDup(line);
		mstring[macroLine] = curLine;
		macroLine++;
		free(u);
		return 1;
	
	}
	else if (strlen(line) > 3 && !strncmp(line, "endm", 4)) {    /*end of macro reached*/
		
		macro  = 0;
		free(u);
		return 1;
	}
	else {
		fprintf(am, "%s", line);
		free(u);
		return 1;
	}
	
	

	


}


