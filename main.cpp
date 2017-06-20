#include <stdio.h>

#include "parser_aid.h"
#include "op.h"
#include "parser.hpp"

int main(int argc, char **argv) {
	extern FILE *yyin;
	if(argc != 2) {
		printf("Uso: %s arquivo_de_entrada\n", argv[0]);
		return 1;
	}
	
	if ((yyin = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Erro ao tentar abrir o arquivo %s!\n", argv[1]);
		return 1;
	}
	
	if (yyparse()) {
		fprintf(stderr, "Não foi possível compilar %s!\n", argv[1]);
		return 1;
	}
  
  return asmvm::parser::StaticHolder::instance().vm().Run();;
}