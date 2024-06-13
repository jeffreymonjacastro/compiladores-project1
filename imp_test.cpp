#include <sstream>
#include <iostream>
#include <iostream>

#include "imp.hh"
#include "imp_parser.hh"
#include "imp_printer.hh"
#include "imp_interpreter.hh"
#include "imp_typechecker.hh"

void testScanner(Scanner scanner){
	cout << "Testing Scanner: " << endl;
	Token *tk = scanner.nextToken();
	while (tk->type != Token::END) {
		cout << Token::token_names[tk->type] << " " << tk->lexema << endl;
		delete tk;
		tk = scanner.nextToken();
	}

	cout << "last token " << Token::token_names[tk->type] << endl;
	cout << endl;
	delete tk;
}

int main(int argc, const char *argv[]) {

	Program *program;

	if (argc != 2) {
		cout << "Incorrect number of arguments" << endl;
		exit(1);
	}

	std::ifstream t(argv[1]);
	std::stringstream buffer;
	buffer << t.rdbuf();
	Scanner scanner(buffer.str());

	// Testing Scanner
	testScanner(scanner);

	Parser parser(&scanner);
	program = parser.parse();  // el parser construye la aexp

	// Declaración de los visitors
	ImpPrinter printer;
	ImpInterpreter interpreter;
	ImpTypeChecker checker;

	printer.print(program);

	cout << endl << "Type checking:" << endl;
	checker.typecheck(program);
	cout << "Type checking OK" << endl;

	cout << endl << "Run program:" << endl;
	interpreter.interpret(program);

	delete program;
}
