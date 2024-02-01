/* ================= first_pass.c ===================*/ 

/*
The purpose of this source file is to complete the "first pass" as specified on the assignment project.
*/

#include "asm.h"

struct keywords instruction[] ={
  {"mov"},{"cmp"},{"add"},{"sub"},{"lea"},{"clr"},{"not"},{"inc"},
  {"dec"},{"jmp"},{"bne"},{"jsr"},{"red"},{"prn"},{"rts"},{"stop"},
};

struct keywords directive[] = {
  {"data"},{"string"},
  {"entry"},{"extern"},
};

struct keywords reg[]={
  {"r0"},{"r1"},{"r2"},{"r3"},{"r4"},{"r5"},{"r6"},{"r7"}, {"r8"},{"r9"},{"r10"},{"r11"},{"r12"},{"r13"},{"r14"},{"r15"},
};


/*------------------------- ASSEMBLE ---------------------
assemble: This is the main function of this file.
          It activates the "first pass" on the input file (the file ending with "*.am") 
		  file. It then calls readLine, and writeError on
		  each iteration.
*/
int assemble(char *ch){
	FILE *am ;
	char am_name[NAME] ;
	char as_name[NAME] ;
	char line[MAX];
		        
	int  ln=1;      /* ln counts line number in the source */
		
	struct boolian longline={0};
	

        er=0;	
	ic=100; /* initiating instruction counter. */
	dc=0; /* initiating data counter. */
	ac=100; /* initiating address counter. */
	
	asExtens(ch, as_name);
	addExtens(ch, am_name, ".am");
	
	am=fopen(am_name,"r");
	
	if(am == NULL){
		printf("\n\n Either \"%s\" does not exist or",am_name);
		printf("\n you have no permission to read it.\n\n");
		return 0;
	}
		               
	while(1){
		
		/* initiating instruction codes and directive values*/
		inscode=no_ins;
		dirtype=no_dir;		
		/*________________________________________________*/
		error=0;
		longline.flag=0;
		
		/*------- Set a breaking point.------------------*/
		if(fgets(line,MAX,am)==NULL)
			break;		/* Finish reading file, exiting loop. */
		/*_______________________________________________*/

		if(! ignore(line)){
			/*------- Checking line's length ---------------*/
			if(strlen(line)>MAX-2){
				longline.flag=1;  /* Marks long line.*/
				error=9;
				er++;
				writeError(ln);
				ln--;
			}			
			/*_____________________________________________*/

			if(longline.flag != 1){  /* If line is not to long, proceed...	*/
				readLine(line);  
				if (error != 0)  /* Count errors */
					er++;             
				writeError(ln);			
			}
		}
		ln++;	/*count line numbers in the source code.*/
	}
	/* End of loop. */
	fclose(am);
	secondPass(line,ch); /* executing the second pass on "am" file.*/
	
	if( er > 1 )
		printf("\nFile name %s contains %d errors.\n",as_name,er);
	if( er == 1 )
		printf("\nFile name %s contains %d error.",as_name,er); 
	if (er == 0)
		printf("\nFile successfully compiled.");
	return 1; 
}



/*----------------------- readLine  -------------------------------
readLine: Read a line from "am" file determine whether a label exists
		   determine whether the line contains a directive or 
		   instruction and calls the appropriate function[s] 
		   accordingly.
--------------------------------------------------------------------
*/
int readLine(char *line){
	
	/*char *p=line  ;*/
	int temp=0;
	int label=0;
	
	/*----- Remarks and blank lines -----*/
	if (*line == ';')
		return 0 ;
	line=skipSpace(line); 
	if(line == NULL)
		return 0;
	/*___________________________________*/
	

	/* first character can either be a dot or a letter. */
	if( isalpha(*line)==0 && *line != '.'  ){   
		error=1;
		return 0;
	}
	/*__________________________________________________ */

	/*If there is a label, "lookForLabel" will detect it, 
 	  put it in the symbol table, mark it with label.
	  flag and move to the next field. */	
	if( lookForLabel(line) == 1 ){ 
		label=1 ;			/* Mark label.*/
		line=nextField(line) ; /* Go to the next field.*/
	}
	
	if (line == NULL){
		error=11;
		return 0;
	}

	/* Looking for directive...-------------*/
	if(*line == '.' ){
		lookForDir(line);
		if(dirtype == no_dir){ /*findDir(line) == 0){*/
			error=6;    
			return 0;
		}
		dirToValue(line,label);
		return 1;
	}
	/*--------------------------------------*/
	
	/* Looking for instruction...*/
	temp=lookForIns(line);
	if(temp==0 && error == 0){
		error=7;
		return 0;
	}
	if( temp==1 ){
		insToValue(line,label);
	}
	return -13; /*late*/
}



/*---------------------lookForLabel---------------------------------
lookForLabel: return 1 on label appearance. Else return 0.
*/
int lookForLabel(char *field ){
	char *p=field;
	
	int  i=0, c=0;
	
	while( *p != ':' && *p != '\n' && i<30 ){
		LABEL[i]=*p;
		p++;
		i++;
	}
	LABEL[i]='\0';
	
	while( *p != ':' && *p != '\n' && *p!='\0')  /* problem here 1 if p is newline or end of string but still no colon*/
		p++;
	if ( *p == ':' ){						   
		if (i >= 30){
			error=5;		/* label is too long.*/
			return 0;
		}
		for(c=0;c<i;c++){ /* checking label characters.*/
			if(!isdigit(LABEL[c]) && !isalpha(LABEL[c])){
			error=1;
			return 0;
			}
		}
			
		if ( *field == '.' || isdigit(*field)  ){ /* label cannot start with a dot.*/
			error=8;			
			return 0;
		}		
		if (lookForIns(LABEL)==1){   /* label cannot be an instruction.*/
			error=2 ;
			return 0;
		}
		if (findDir(LABEL)==1){   /* label cannot be a directive.*/
			error = 3 ;
			return 0;
		}
		if(findReg(LABEL) == 1){  /* label cannot be a register.*/
			error=13;
			return 0;
		}
		if( error == 0 )
			return 1;
	}
	return -13; /*late*/
}



/*----------- skipSpace --------------------------------
skipSpace: Skips on blank character[s] and returns the
		   a pointer to the first non blank character.
--------------------------------------------------------
*/
char *skipSpace(char *line){
	if(line == NULL)
		return NULL;
	if(strlen(line) <= 1)       
		return NULL;
	while(*line==' ' || *line=='\t'){
		if (*line == '\0' || *line==10)
			break;
		line++;
		
	} 
	return line;
}
/* ----------------------  checkExt - checks extension   ----------------------------------- */

int checkExt(char *ch){
	while(*ch != '\0'){
		ch++;
	}
	ch--; 
	if(*ch !='s'){ 
		return 0;}
	ch--;
	if(*ch !='a')
		return 0;
	ch--;
	if(*ch !='.')
		return 0;
	return 1;

}
	


/*------------------ addExtens ------------------------
addExtens: Generates the appropriate extension to the 
             files.
---------------------------------------------------------
*/
void addExtens(char *ch, char *s, char *ext){
	strcpy(s,ch) ;
	strcat(s,ext) ;	
}

/*------------------ MAKE_EXTENS ------------------------
asExtens: Add extension for the *.as filetype. 
---------------------------------------------------------
*/
void asExtens(char *ch, char *s){
	strcpy(s,ch) ;
	if(checkExt(ch) == 0)
		strcat(s,".as") ;	
}
/*----------------- nextField -------------------------------
nextField: Returns a pointer to the next "field" of the line
		   i.e. it choose the first character after "blank".		
*/
char *nextField(char *line){
	if( line == NULL)
		return NULL;
	while( *line != ' ' && *line != '\t' && *line != 10 && *line != '\0' ){
		line++;
	}
	line++;
	line=skipSpace(line);
	return line;
}


/*------------------- findDir --------------------------
findDir: Finds out whether p is a legitimate directive 
          name.
---------------------------------------------------------
*/
int findDir(char *p){

	int i; 
	for(i=0;i<4;i++){
		if ( strcmp(directive[i].word , p) == 0 ){
			return 1 ;
		}
	}
	return 0 ;
}


/*--------------------- lookForIns --------------------------- 
lookForIns: Returns 1 if p is a legitimate instruction name, else
		  it returns zero.  
-------------------------------------------------------------
*/
int lookForIns(char *p){
	int i; 
	for(i=0;i<16;i++){
		if (i == 15) {
			if (strncmp(instruction[i].word , p,4) == 0 && lengthToBlank(p)==4 ){
				inscode=i;
				return 1 ;
			}
		
		}
		if (strncmp(instruction[i].word , p,3) == 0 && lengthToBlank(p)==3 ){
			inscode=i;
			return 1 ;
		}
	}
	return 0 ;
}


/*----------------------- lookForDir --------------------------
lookForDir: Returns 1 if p is a legitimate directive name, else
		  it returns zero.  
-------------------------------------------------------------
*/
int lookForDir(char *p){
	if(p == NULL)
		return 0;
	if ( strncmp("data",p+1,4)==0 && lengthToBlank(p+1)==4){  
		dirtype=0;  /* Mark directive type. */
		return 1 ;
	}
	if (strncmp("string",p+1,6)==0 && lengthToBlank(p+1)==6 ){
		dirtype=1;
		return 1;
	}
	if( strncmp("entry",p+1,5)==0 &&  lengthToBlank(p+1)==5){
		dirtype=2;
		return 1;
	}
	if( strncmp("extern",p+1,6)==0 && lengthToBlank(p+1)==6){
		dirtype=3;
		return 1;
	}
	
	return 0 ;
}


/*-------------------- lengthToBlank-------------------------
lengthToBlank: Returns the length of the string up to "blank"
		  character.	 
*/
int lengthToBlank(char *line){
	int i=0;
	while ( *line>32 && *line!=10 && *line!='\0'){
		line++;
		i++;
	}
	return i;
}



/*---------------- dirToValue ---------------------
dirToValue: Inserts value to label and updates 
           the value of dc.
--------------------------------------------------
*/
int dirToValue(char *line, int i){
	int k=0 ;
	char *xlab;
	line=nextField(line);
	
	if(i == 1){ /* label exists.*/
		if(dirtype==DATA || dirtype == STRING){ 
			if(lookForExt(LABEL) != NULL ){  
				error=16;
				return 0;
			}
			if( install(LABEL,(dc+ic)) == 0 ){ /* insert label into the label symbol table.*/
				error = 4;		/* redefinition of label.*/
				return 0;
			}
		}
	}
	
	line=skipSpace(line);
	if( line == NULL ){
		error=18; 
		return 0;
	}
	
	if(dirtype == DATA){
		while( line != NULL && *line!=10 && *line!='\0'){
			if(  *line == ','){
				error=10; 
				return 0;
			}
			if(*line == '-')
				line++;
			while( isdigit(*line) ){
				line++;
				k++;
			}
			dc++;
			line=skipSpace(line);
			if( line==NULL || *(line+1)=='\0')
				return 0;
			if(  *line != ',' || k==0 ){
				error=10; 
				return 0;
			}
			
			line++;
			line=skipSpace(line);
			if(!cont(line)) {
				error =10;
				return 0;
			}
			if(  *line == ','){
				error=10; 
				return 0;
			}
			if( line==NULL)
				return 0;
			k=0;
		}
	}
	if(dirtype == STRING){
		if(line==NULL){
			error=10;
			return 0;
		}
		if(*line!='"'){
			error=10;
			return 0;
		}
	
		line++;

		if( *line == '"' ){
			error=14;
			return 0;
		}
		while(line!=NULL && *line != '"' && *line!=10 && *line!='\0'){
			/*count++;*/
			line++;
			dc++;
		}
		if( line==NULL || *line != '"'){
			error=10;
			return 0;
		}
		line++;
		line = skipSpace(line);
		
		if(line==NULL)
			return 0;
		
		if( *line!=' ' && *line!='\t' && *line!=10 && *line!='\0'){
			error=10;
			return 0;
		}
		dc++; /* for the line that marks the end of string.*/
	} 
	if(dirtype == EXTERN){
		xlab=cpyarg(line,' ');
		if( lookup(xlab) != NULL){ 
			error=17; /* already defined as a non ext label.*/
			return 0;
		}
		installExt(xlab); /* put label in external symbol tabel.*/
		free(xlab);
	}
	if(dirtype == ENTRY ){
		xlab=cpyarg(line,' ');
		installEntry(xlab);
		free(xlab);
	}
	return -13; /*late*/
}


/*------------------ getComma ---------------------------
getComma: Takes a string and return a pointer, pointing 
			to where the comma appears in the string. If 
			no comma in the string, it returns zero.
*/
char *getComma(char *line){
	if(line==NULL || *line == '\0')
		return NULL;
	if(*line==',')
		return line;
	line++;
	return getComma(line);
}

/*--------------------- getAddressingMode --------------------------
getAddressingMode: returns the code number for each 
         operand.
---------------------------------------------------------- 
*/
int getAddressingMode(char *line){
	if(line == NULL )
		return no_operand; 	
	if(*line == '#') { /*instant addressing*/
			line++;
			if( *line == '-')
				line++;
			if( isNumber(line) == 0 )
				error=12;
			return 0;
	}
	if(isLabel(line) == 1) { 
		return 1;
		
	}
	if(isIndexLabel(line) == 1) {
		return 2;
	}
	if(findReg(line) == 1) { /*register addressing*/
		return 3;
	}
	
	else {
		error=12;
	}
	return 4;	
	
}

	
/*---------------- insToValue ------------------------
insToValue: Inserting  a value to label and updating the 
           value of ic. 
-----------------------------------------------------
*/
int insToValue(char *line, int lab_in_tab){
	int first=no_arg, second=no_arg;
	int code=inscode; 
	
  	line=nextField(line);
	if( line == NULL && code <14 ){
		error=12;
		return 0;
	}
	if(lab_in_tab == 1){ /* label exists.*/
		if(lookForExt(LABEL) != NULL ){
			error=16;
			return 0;
		}
		if( install(LABEL,(ic+dc)) == 0 ){ /* put label in the label symbol table.   */
			error = 4;			/* redefinition of label.*/
			return 0;
		}
	}

	/*-Instruction with no operands.-*/
	if( code == 14 || code == 15){
		ic++;
	}
	/*_____________________________*/
	
	/*-Instruction with one operand.-*/
	if(code >= 5 && code <= 13){
		second=getAddressingMode(line);
		if (second == 4){
			error=12;
			return 0;
		}
		if( ((code >= 5 && code <= 8) || code == 12) && second == 0){
			error=12;
			return 0;
		}
		if( ( code >=9 && code <=11 ) && (second ==0 || second ==3 )){
			error=12;
			return 0;
		}
		ic+=2 ;
		if(second >= 1 && second<=2) /*additional words if addressing mode isn't register addressing*/
			ic+=2;
		if (second == 0)
			ic++;
	}
	/*______________________________*/

	/*-Instruction with two operands.-*/
	if( code >=0 && code<=4){
		first=getAddressingMode(line);
		line=getComma(line);
		if(line == NULL){
			error=12;
			return 0;
		}
		line++;
		if(line == NULL){
			error=12;
			return 0;
		}
		line=skipSpace(line);
		if(line == NULL){
			error=12;
			return 0;
		}
		second=getAddressingMode(line);
		if (code == 4 ){
			if( first == 0 || first == 3 ){
				error=12;
				return 0;
			}
			if( second == 0  ){
				error=12;
				return 0;
			}
		}
		if((code >= 0 && code <= 3) && code != 1){
			if (second == 0) {
				error=12;
				return 0;
			}
		}
		ic+=2;
		
		if(first >= 1 && first<=2) /*additional words if addressing mode isn't register addressing*/
			ic+=2;
		if (first == 0)
			ic++;
		if(second >= 1 && second<=2) /*additional words if addressing mode isn't register addressing*/
			ic+=2;
		if (second == 0)
			ic++;
	}
	return -13; /*late*/
}
	
/*------------cont---------------------------
cont: To make notation easier.
*/
int cont(char *line){
	if(line==NULL)
		return 0;
	if(*line != ' ' && *line != '\t'){
		if(*line != 10 &&  *line !='\0' && line != NULL)
			return 1;
	}
	return 0;
}


/*--------------- isNumber ----------------------
isNumber: Check to see whether a string is a positive 
	  number or not. Return 1 if the string is a 
	  positive number 0 otherwise.       
-------------------------------------------------
*/
int isNumber(char *line){
	if(line == NULL)
		return 0;
		
	if( *line == '#' )
		line++ ;
	if(*line == '-'){
		line++;
	}
	
	if(!cont(line))
		return 0;
	
	while( cont(line) && *line != ',' ){
		if( ! isdigit(*line) )
			return 0;
		line++;
	}
	return 1;
}

/*-------------------- isLabel ----------------------
isLabel: returns 1 if the syntax of the string compliance  with the syntax of label.
----------------------------------------------------
				
*/
int isLabel(char *line){
	int i=0;
	if(line == NULL)
		return 0;
	if(findReg(line))
		return 0;
	if(lookForIns(line)) {
		return 0;
	}
	while( cont(line) && *line != ',' ){
		if(i==0 &&  isalpha(*line) == 0 ) {
			return 0;
		}
		
		i++;
		if( isdigit(*line) == 0 &&  isalpha(*line) == 0 && *line!= ':') {
			return 0;
		}
		if(i == 30){
			error=5;
			return 0;
		}
		line++;
	}
	return 1;
}

/*----------------------- findReg ------------------------------
findReg: Returns 1, if register appears, 0 otherwise.
----------------------------------------------------------------
*/
int findReg(char *p){
	int i; 
	for(i=10;i<16;i++){
		if ( strncmp(reg[i].word, p ,3) == 0 ){
			regist=i;
			return 1 ;
		}
		
	}
	for(i=0;i<10;i++){
		if ( strncmp(reg[i].word, p ,2) == 0 ){
			regist=i;
			return 1 ;
		}
		
	}
	regist = no_reg;
	return 0 ;
}

/*----------------------- findIndexReg ------------------------------
findIndexReg: Returns 1, if index register appears, 0 otherwise.
----------------------------------------------------------------
*/
int findIndexReg(char *p){
	int i; 
	for(i=10;i<16;i++){
		if ( strncmp(reg[i].word, p ,3) == 0 ){
			regist=i;
			return 1 ;
		}
	}
	return 0 ;
}

/*----------------------- isIndexLabel ------------------------------
isIndexLabel: Returns 1, if the label is an index label, 0 otherwise.
----------------------------------------------------------------
*/
int isIndexLabel(char* line) {
	while (*line != '[' && !isspace(*line) && (isalpha(*line) || isdigit(*line))) {
		line++;
	}
	
	if(*line == '[') {
		line++;
		if(findIndexReg(line)) {
			line+=3;
			if (*line == ']') {
				return 1;
			}
			else error = 1;
		}
		else error = 19;
	
	}
	return 0;
	
	




}

/*-------------------------ignore----------------------------------
 * */
int ignore(char *line){
	if(line == NULL)
		return 1; /* ignore the line it doesn't exist.*/
	if(*line==';')
		return 1;
	while(line!=NULL && *line<=32 && *line!='\0' && *line=='\t'){
		line++;
		if(*line==';')
			return 1;
	}
	if(*line=='\0')
		return  1;
	line++;
	if(*line=='\0')
		return  1;
	else 
		return 0;
}
