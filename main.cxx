#include <iostream>
#include <vector>

#if not defined LEX_IMPL
#define LEX_IMPL
#endif
#include "SimpleLexer.hxx"

int main(void){

	SimpleLexer lex;

	//std::vector<std::string> a = lex.lex("fn \n\t hello123 world(){}/*-+");
	std::vector<std::string> a = lex.lex_file("./test.txt");
	for(std::string &_ : a){
		std::cout << "    TOKEN: \'" << _ << "\'\n";
	}

	std::cout << "  RUN SUCCESSFULL" << std::endl;
}
