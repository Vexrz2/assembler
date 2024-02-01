/*==================== MAIN ========================= */
/* The purpose of this program is to create an assembler program
   
*/

#include "asm.h"





int main(int argc, char *argv[]){
	int i;
	for(i=1;i<argc;i++){
		preProcessor(argv[i]);
		assemble(argv[i]);
		
		cleanMacroTab();
		cleanTab();
		cleanExTab();
		cleanEnTab();
	}
	printf("\n");
	return 0; 
}

