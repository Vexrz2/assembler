/* ================= second_pass.c ===================*/ 

/* 
The purpose of this file is to make the "second pass" on the "as" file. 
*/

#include "asm.h"

/*--------------------- insertFlag --------------------------------
insertFlag: Takes a string and inserts the correct A,R,E flag
--------------------------------------------------------------*/
char* insertFlag(char* str, int flag) {
	switch (flag) {
		case 1: 
			*(str+1) = '1';
			break;
		case 2: 
			*(str+2) = '1';
			break;
		case 3: 
			*(str+3) = '1';
			break;
	
	
	
	
	
	}
	return str;

}

/*--------------------- charToNum --------------------------------
charToNum: Takes a string of the form [#][-]d where d is a string
		  of digits and the character between [] are optional, 
		  and returns the numeric value of [-]d.
--------------------------------------------------------------*/
int charToNum(char *line){
	int num=0;
	struct boolian minus={0};
	
	if( *line == '#' )
		line++ ;
	if(*line == '-'){
		minus.flag=1;
		line++;
	}
	
	if (line == NULL) {
		return 0;
	}
	
	do{		
		num=(num*10)+( *line - '0' );
		line++;
	} while( isdigit(*line) ); 
	
	if(minus.flag ==1) {
		return -num;
	}
	else {
		return num;
	}
}


/*--------------------- fullDecToBinary --------------------------------
fullDecToBinary: Takes a number and returns a string of its binary form with a flag.
--------------------------------------------------------------*/
char* fullDecToBinary(int num, int flag) {
	int k, c;
	int i = 4, m = 0;
	char* str = (char *) calloc (20, 1);
	for(m = 0; m < 20; m++) {   /*resetting the string to 0s*/
		*(str+m) = '0';
	
	}
	for (c = 15; c >= 0; c--)
  	{				/*filling up the integer part of the binary string*/
   		k = num >> c;

    		if (k & 1)
    			*(str+i) = '1';
    		i++;
  	}
  	insertFlag(str, flag);  /*inserting the appropriate flag(A,R,E), for data*/
  	return str;
}

/*--------------------- getFunct --------------------------------
getFunct: Returns the funct code of the input inscode
--------------------------------------------------------------*/

int getFunct(int code) {
   	switch (code) {
   		case 2: 
   			return 10;
   		case 3: 
   			return 11;
   		case 5: 
   			return 10;
   		case 6: 
   			return 11;
   		case 7: 
   			return 12;
   		case 8: 
   			return 13;
   		case 9: 
   			return 10;
   		case 10: 
   			return 11;
   		case 11: 
   			return 12;
   		default:
   			return 0;
   	
   	}
}

/*--------------------- decToHex --------------------------------
decToHex: Converts a decimal number into hexadecimal form (char)
--------------------------------------------------------------*/

char decToHex(int num) {
   	char hex;
	int i = 0;
     	int temp = 0;
	temp = num % 16;
        if(temp < 10) {
        	hex = temp + 48;
         	i++;
      	} 
      	else 	{
         	hex = temp + 87;
         	i++;
      	}
      	return hex;
}

/*--------------------- binCharToNum --------------------------------
binCharToNum: Converts a string of a binary number to a decimal number
--------------------------------------------------------------*/

int binCharToNum(char* str) {
   	int i;
   	int num =0;
   	for(i = 3; i>=0; i--) {
   		num+=(str[3-i] - '0')*pow(2, i);
   	}
   	return num;
}

/*--------------------- binToString --------------------------------
binToString: Takes a string and returns a string in the format specified in the project (A-B-C-D-E)
--------------------------------------------------------------*/
char* binToString(char* str) {
	char* hexStr = (char*) calloc (15,1);
	
	char strA[4];
	memcpy(strA, str, 4);
	char strB[4];
	memcpy(strB, str+4, 4);
	char strC[4];
	memcpy(strC, str+8, 4);
	char strD[4];
	memcpy(strD, str+12, 4);
	char strE[4];
	memcpy(strE, str+16, 4);
	
	int a = binCharToNum(strA);
	int b = binCharToNum(strB);
	int c = binCharToNum(strC);
	int d = binCharToNum(strD);
	int e = binCharToNum(strE);
	
	*(hexStr) = 'A';
	*(hexStr + 1) = decToHex(a);
	*(hexStr + 2) = '-';
	*(hexStr + 3) = 'B';
	*(hexStr + 4) = decToHex(b);
	*(hexStr + 5) = '-';
	*(hexStr + 6) = 'C';
	*(hexStr + 7) = decToHex(c);
	*(hexStr + 8) = '-';
	*(hexStr + 9) = 'D';
	*(hexStr + 10) = decToHex(d);
	*(hexStr + 11) = '-';
	*(hexStr + 12) = 'E';
	*(hexStr + 13) = decToHex(e);
	
	free(str);
	return hexStr;
}

/*--------------------- decToBinary --------------------------------
decToBinary: Takes a number and returns a string of its binary form.
--------------------------------------------------------------*/
char* decToBinary(int num, int size) {
	int k, c, m;
	int i = 0;
	char* str = (char *) calloc (size, 1);
	for(m = 0; m < size; m++) {   /*resetting the string to 0s*/
		*(str+m) = '0';
	
	}
	for (c = size-1; c >= 0; c--) {
   		k = num >> c;

    		if (k & 1) {
    			*(str+i) = '1';
    		}
    		
    		i++;
  	}
	return str;

}



/*--------- secondPass------------------------------- 
secondPass: The main function in this file.
             It makes the "second pass" on the "am" 
			 file, create the appropriate files and
			 uses other functions to write on these 
			 files. If the program detects error[s]
			 on the "am" file, then the function removes 
			 all file[s].
-------------------------------------------------------
*/
int secondPass(char *line, char *ch){
	
	FILE *am, *ob, *ex=NULL, *en=NULL;
	char am_name[NAME], ob_name[NAME], ex_name[NAME], en_name[NAME];
	int ln;		 /*to count line number in source. */
	addExtens(ch,am_name, ".am");
	am=fopen(am_name,"r");
	addExtens(ch,ob_name,".ob");
	addExtens(ch,en_name,".ent");
	addExtens(ch,ex_name,".ext");
	ob=fopen(ob_name,"w");
	fprintf(ob,"\t%d   %d\n",ic-100,dc);
	if( ob == NULL ){
		printf("Error: permission denied");
		return 0;
	}

	/*--- Creating ".ext" and ".ent" files. ---- */
			
	ex=fopen(ex_name,"w");
	en=fopen(en_name,"w");
	ln=1;

	while(1){
		
		/* initiating instruction codes and directive values*/
		inscode=no_ins;
		dirtype=no_dir;
		regist=no_reg;
		extFlag=no_ext;
		error=0;
		/*__________________________________________________*/
		/*------- Set a breaking point.------------------*/
		if((fgets(line,MAX,am))==NULL)					/*for some reason fgets isn't working here, if it will work the whole program will run well(check write code)*/
			break;		/* Finish reading file, exiting loop.*/
		/*_______________________________________________*/	
		if(!ignore(line)){
			writeToFiles(ob,en,ex,line,ln);
			writeError2(ln,line);
			if (error != 0)  /* Counting errors */
				er++;
			
		}
		ln++;
	}

	
	
	fclose(ex) ;
	fclose(en) ;
	fclose(ob);
	fclose(am);
	if(er>0){
		remove(ob_name);
		remove(en_name);
		remove(ex_name);
	}
	return 1; /*late*/
}


/*------------------getOpCode-------------------------------------------
getOpCode: Returns the correct opcode after funct adjustment
-------------------------------------------------------------------
*/
int getOpCode(int code){
	if (code >=2 && code<=3) 
		return 2;
	if (code >=5 && code <=8) 
		return 5;
	if (code >=9 && code <=11) 
		return 9;
	
	return code;
}

/*------------------print-------------------------------------------
print: To make notation easier.
-------------------------------------------------------------------
*/
int print(FILE* ob, int code, int flag){
	char* str = (char *) calloc (20, 1);
	
	int m;
	for(m = 0; m < 20; m++) {   /*resetting the string to 0s*/
		*(str+m) = '0';
	
	}
	code = getOpCode(code);
	*(str+(19-code)) = '1'; /*turning on the correct bit*/
	insertFlag(str, flag);
	char* hexString = binToString(str);
	fprintf(ob, "%s \n", hexString);
	free(hexString);
	return 0;
}

/*------------------printIns-------------------------------------------
printIns: To make notation easier. Prints an instruction to file "ob" 
-------------------------------------------------------------------
*/
int printIns(FILE* ob, int code, int originReg, int originMode, int destReg, int destMode){
	char* str = (char *) calloc (20, 1);
	int m;
	for(m = 0; m < 20; m++) {   /*resetting the string to 0s*/
		*(str+m) = '0';
	
	}
	insertFlag(str, 1);
	int funct = getFunct(code);
	
	char* functString = decToBinary(funct, 4);
	char* originRegString = decToBinary(originReg, 4);
	char* originModeString = decToBinary(originMode, 2);
	char* destRegString = decToBinary(destReg, 4);
	char* destModeString = decToBinary(destMode, 2);
	
	memcpy(str+4, functString, 4);
	memcpy(str+8, originRegString, 4);
	memcpy(str+12, originModeString, 2);
	memcpy(str+14, destRegString, 4);
	memcpy(str+18, destModeString, 2);
	
	free(functString);
	free(originRegString);
	free(originModeString);
	free(destRegString);
	free(destModeString);
	
	
	char* hexString = binToString(str);
	fprintf(ob, "%s \n", hexString);
	free(hexString);
	
	return 0;
}

/*------------------printNum-------------------------------------------
printNum: To make notation easier. Prints a number to file "ob" 
-------------------------------------------------------------------
*/
int printNum(FILE* ob, int num, int flag){
	char* hexString = binToString(fullDecToBinary(num, flag)); 
	fprintf(ob, "%s \n", hexString);
	free(hexString);
	return 0;
}

/*----------------------- writeToFiles -----------------------------
writeToFiles: Takes a line from the "am" file and write output to 
			 "ob", "ex", and "en".   
*/
int writeToFiles(FILE *ob,FILE *ent,FILE *ext,char *line, int ln){
	/* ----- skip label ----------------- */
	line=skipSpace(line);
	if( isLabel(line) == 1 ){
		line=nextField(line);
	}
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	dirOb(ob,line);
	writeToEnt(ent,line);
	wrt(ob,ext,line);	
	
	return 0;
}

/*-------------------- dirOb ---------------------------------
dirOb: if a line contains .data or .string directive, then
		dirOb function writes the appropriate output line[s]
		to the "ob" file.
---------------------------------------------------------------
*/
int dirOb(FILE *ob,char *line){
	int i=0;
	
	lookForDir(line);  /* change dirtype value.*/
	/*--- DATA*/
	if( dirtype == DATA ){
		line=nextField(line) ;
		line=skipSpace(line); 
		if( line == NULL){
			return 0;
		}
		do{
			if(line == NULL ) 
				break;
			fprintf(ob,"%.4d\t",ac);  
			char* hexString = binToString(fullDecToBinary(charToNum(line), 1));  
			fprintf(ob,"%s \n",hexString); 
			free(hexString);   
			ac++;
		line=getComma(line);
			if(line == NULL ){
				
				break;
			}
			line++;
			line=skipSpace(line);
			
		}
		while(line!=NULL);
	}
	/*--- STRING*/
	if( dirtype == STRING ){
		line=nextField(line);
		line=skipSpace(line);
		if( line == NULL ){
			return 0;
		}
		line++; /* skip on quotation mark.*/
		
		do{
			fprintf(ob,"%.4d\t",ac);
			char* hexString = binToString(fullDecToBinary(*line, 1));  
			fprintf(ob,"%s \n",hexString);   
			free(hexString);   
			line++;
			ac++;
			i++;
		}
		while(*line != '"' && i<MAX);
		
		fprintf(ob,"%.4d\t",ac);    /*end with '0' to mark end of string*/
		char* hexString = binToString(fullDecToBinary(0, 1));  
		fprintf(ob,"%s \n",hexString);   
		free(hexString);   
		ac++;
		i++;
		
		if( i == MAX){
			error=1000;
			error=4;
		}
	}
	return 0;
}

/*------------------- writeToEnt ----------------------------
writeToEnt: if a line contains .entry directive, then
		   writeToEnt function writes the appropriate output line
		   to the "en" file. 
------------------------------------------------------------
*/
int writeToEnt(FILE *ent, char *line){
	
	struct nlist *en;
	char *temp;
	
	/* to change dirtype value, writeToFiles function calls to */
	/* dirOb first and then it calls to the current function. */
	line=skipSpace(line);
	/* ----- skip label ----------------- */
	if( isLabel(line) == 1 ){
		line=nextField(line);
	}
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	if( dirtype == ENTRY){
		line=skipSpace(line);
		line=nextField(line);
		if( line == NULL ){
			return 0;
		}
		temp=cpyarg(line,' ');
		en=lookup(temp); 
		if(en == NULL){
			error=5; 
			return 0;
		}
		int address = en->value;
		int baseAddress = address - address %16;
		int offset = address % 16;
		fprintf(ent,"%s, %d, %d \n",temp, baseAddress, offset); 
		free(temp); 
	}
	return 1;


}

/*-------------------- wrt -------------------------------
wrt: If a line contains instruction, then wrt function
     writes the appropriate output line[s] to the "ob" file.
----------------------------------------------------------
*/
int wrt(FILE *ob, FILE *ext, char *line){ 
	int reg1, reg2 ; 
	char *u;
	int first_arg, second_arg ;
	int code ;
	int addtype1, addtype2;
	
	addtype1=no_operand;
	addtype2=no_operand;

	line=skipSpace(line);  
	if(line==NULL || *line=='\0' || *line==10) {
		return 0;
	}
	
	lookForIns(line); /* change inscode value.*/
	code=inscode;
	/* Instruction with no arguments.*/
	if(code==14 || code == 15){
		fprintf(ob,"%.4d\t",ac);
		print(ob, code, 1);
		ac++;
		return 0;
	}
	line=nextField(line);
	
	if(line == NULL){
		error=2;
		return 0;
	}
	
	u=cpyarg(line,' ');
	/* Instruction with one argument.*/
	if(code >= 5 && code <= 13){
		addtype1=getAddressingMode(line);
		if(addtype1 == no_operand){
			error=2;
			return 0;
		}
		fprintf(ob,"%.4d\t",ac);
		findReg(u);
		
		
		if(regist == no_reg)
			regist=0;
		print(ob, code, 1);
		ac++;
		fprintf(ob,"%.4d\t",ac);
		addVal(ext, line,& first_arg, addtype1);
		printIns(ob,code,0,0,regist,addtype1);
		
		if(first_arg>-1){
			ac++;
			fprintf(ob,"%.4d\t",ac);
			if (addtype1 == 0) {
				printNum(ob, first_arg, 1);
			
			}
			if (addtype1 == 1 || addtype1 == 2) {
				int address = first_arg;
				int baseAddress = address - address %16;
				int offset = address % 16;
				printNum(ob, baseAddress, extFlag);
				ac++;
				fprintf(ob,"%.4d\t",ac);
				printNum(ob, offset, extFlag);
			}
			
		}
		ac++;
	}
	/*   Instruction with two arguments.*/
	if( code >= 0 && code < 5 ){
		addtype1=getAddressingMode(line);
		if(addtype1 == no_operand){
			error=2 ;
			return 0 ;
		}
		fprintf(ob,"%.4d\t",ac);
		print(ob, code, 1);
		ac++;
		/*__________________________________________________________*/
		findReg(u);
		
		addVal(ext, line,&first_arg, addtype1);
		if(regist == no_reg)
			regist=0;
		reg1=regist;
		
		free(u);
		/*__________________________________________________________*/
		if(line != NULL)
			line=getComma(line);
		
		if(line != NULL)
			line++; /* skip comma.*/
		if(line != NULL)
			line=skipSpace(line);
		if(line == NULL){
			error=3;
			return 0;
		}
		addtype2=getAddressingMode(line);
		u=cpyarg(line,' ');	
		/*____________________________________________________________*/
		findReg(u);
		addVal(ext, line,&second_arg,addtype2);
		if(regist == no_reg) {
			regist=0;
		}
		
		reg2=regist;     
		/*____________________________________________________*/
		fprintf(ob,"%.4d\t",ac);
		printIns(ob,code,reg1,addtype1,reg2,addtype2);
		ac++;
		if(first_arg!=-1){
			fprintf(ob,"%.4d\t",ac);
			if (addtype1 == 0) {
				printNum(ob, first_arg, 1);
			
			}
			if (addtype1 == 1 || addtype1 == 2) {
				int address = first_arg;
				int baseAddress = address - address %16;
				int offset = address % 16;
				printNum(ob, baseAddress, extFlag);
				ac++;
				fprintf(ob,"%.4d\t",ac);
				printNum(ob, offset, extFlag);
			}
			ac++;
		}

		if(second_arg!=-1){
			fprintf(ob,"%.4d\t",ac);
			if (addtype2 == 0) {
				printNum(ob, second_arg, 1);
			}
			if (addtype2 == 1 || addtype2 == 2) {
				int address = second_arg;
				int baseAddress = address - address %16;
				int offset = address % 16;
				printNum(ob, baseAddress, extFlag);
				ac++;
				fprintf(ob,"%.4d\t",ac);
				printNum(ob, offset, extFlag);
			}
			ac++;
		}
		
	}
	free(u);
	return 1; /*late	*/
}



/*---------------------- addVal ----------------------------------------
addVal: If an additional word should be written to file name "ob", then
		 addVal function puts the numeric value of this word in integer
		 name "first". Otherwise "first" gets -1. The addVal function 
		 also, writes the appropriate output to the "ex" file.
-------------------------------------------------------------------------
*/
int addVal(FILE *ext, char *line,int *first, int operand){
	
	struct nlist *temp;
	char *u;
	int addm = getAddressingMode(line);
	if(addm == no_operand){
		error=2 ;
		return 0;
	}
	u=cpyarg(line,',');
	/*if( addm != 1) {
		u++;
	}*/
	if( ( temp=lookup(u) ) != NULL){ /* find out whether temp is in label symbol table.*/
		*first=temp->value ;
		free(u);
		return 0;
	} 
	if( ( temp = lookForExt(u) ) != NULL){ /* finds out  whether temp is in external symbol table.*/
		
		/*if( operand>3 && operand != no_operand)
			fprintf(ext,"%s\t%d \n",temp->name,ac);
		if( operand<3 && operand>=0 )
			fprintf(ext,"%s\t%d \n",temp->name,ac+1); */
			
		fprintf(ext,"%s BASE %d \n",temp->name, ac); 
		fprintf(ext,"%s OFFSET %d \n \n",temp->name, ac+1); 
		extFlag = 3;
		*first=0 ;
		free(u);
		return 0;
	}
	if( addm == 0 ){
		if (isNumber(u) != 0) {
			*first=charToNum(u);
		}
		if (isNumber(u) == 0) {
			error=1;
		}
		free(u);	
		return 0;	
	}
	if (addm == 2) {
		char* label = cpyarg(u, '[');
		temp = lookup(label);
		free(label);
		if (temp!= NULL) {
			
			regist = regFromIndex(u);
			*first = temp->value;
			free(u);
			return 0;
		}
		
	}	
	if( addm == 3 ){
		*first = -1; /* no additional word.*/
		free(u);
		return 0;
	}		
	error=1;
	free(u);
	return 0 ;
} 

int regFromIndex(char* index) {
	while (*index != '[') {
		index++;
	}
	index+=2;
	
	char* num = cpyarg(index, ']');
	int reg = atoi(num);
	free(num);
	return reg;

}

/*------------------ cpyarg -------------------------------
cpyarg: Copy all the character in the input string up to
		c, space, \t, \0 character, which ever comes first.   
----------------------------------------------------------
*/	
char *cpyarg(char *line,char c){
	/*char *arg; 		
	char *t;
	
	arg=(char*) malloc(sizeof(char));
	if(arg == NULL ){
		printf("Error: out of memory");
		return NULL;
	}
	t=arg;
	strcpy(arg,line);
	if( arg == NULL )
		return NULL;
	while( cont(arg) &&  *arg != c ){
		arg++ ;
	}
	*arg='\0' ;
	return t ; */
	
	char *arg; 
	char *t;
	
	arg=(char*) malloc(strlen(line)*sizeof(char) + 1);
	if(arg == NULL ){
		printf("Error: out of memory");
		return NULL;
	}
	t=arg;
	strcpy(arg,line);
	if( arg == NULL )
		return NULL;
	while( cont(arg) &&  *arg != c ){
		arg++ ;
	}
	*arg='\0' ;
	return t ; 
	
}
