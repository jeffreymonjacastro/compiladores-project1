# Projecto 1 - (Compiladores CS3025)

## Resumen
En este proyecto se implementaron mejoras al compilador del lenguaje IMP-DEC que se desarrolló en el curso de Compiladores CS3025. Las mejoras implementadas fueron las siguientes:

1. **Typechecker y Codegen**: Se implementó un `typechecker` y un `codegen` para la gramática IMP-DEC.

2. **Generación de Código I**: Se modificó el `Scanner` y `Parser` para que soporte la inclusión de comentarios en el código fuente. 

3. **Sentencia Do While**: Se implementó la sentencia `do while` en el lenguaje IMP-DEC.


## Tabla de Contenidos

- [Projecto 1 - (Compiladores CS3025)](#projecto-1---compiladores-cs3025)
  - [Resumen](#resumen)
  - [Tabla de Contenidos](#tabla-de-contenidos)
  - [Gramática del lenguaje IMP-DEC](#gramática-del-lenguaje-imp-dec)
  - [1. Typechecker y Codegen](#1-typechecker-y-codegen)
    - [1.1. Typechecker](#11-typechecker)
    - [1.2. Codegen](#12-codegen)
    - [Ejemplo de uso](#ejemplo-de-uso)
  - [2. Generación de Código I](#2-generación-de-código-i)
  - [3. Sentencia Do While](#3-sentencia-do-while)
  - [Autores](#autores)
  - [Referencias](#referencias)

## Gramática del lenguaje IMP-DEC
La gramática que se utilizó en el proyecto es la siguiente:

```js
Program         ::= Body
Body            ::= VarDecList StatementList  
VarDecList      ::= (VarDec)* 
VarDec          ::= "var" Type VarList ";" (Comment)?
Type            ::= id
VarList         ::= id ("," id)*
StatementList   ::= (Stm)+
Stm             ::= id "=" Exp ";" (Comment)?                           |
                    "print" "(" Exp ")" ";" (Comment)?                  |
                    "if" Exp "then" Body ("else" Body)? "endif" ";"     |
                    "while" Exp "do" Body "endwhile" ";"                |
                    "do" Body "while" Exp "enddo" ";"         
Exp             ::= BExp
BExp            ::= CEXP (("and" | "or") BExp)?
CEXP            ::= AExp (("==" | "<" | "<=" ) AExp)?
AExp            ::= Term (("+" | "-") Term)*
Term            ::= FExp (("*" | "/") FExp)*
FExp            ::= Factor ("**" FExp)?
Factor          ::= id                                                  | 
                    num                                                 | 
                    true                                                |
                    false                                               |
                    "(" Exp ")"                                         | 
                    "ifexp" "(" Exp "," Exp "," Exp ")"
Comment         ::= "//" (~("\n"))*
```

## 1. Typechecker y Codegen

En esta sección se describe la implementación del Typechecker y Codegen para el lenguaje IMP-DEC.

### 1.1. Typechecker
El Typechecker es un visitor que recorre el AST parseado y verifica que las variables utilizadas en el programa estén declaradas y que los tipos de las variables sean correctos. En caso de que haya un error, el Typechecker lanza una excepción con un mensaje de error. 

Los archivos modificados para implementar el Typechecker son:
+ `type_visitor.hh`: Interfaz **TypeVisitor** que funcionará como visitor para el Typechecker.
+ `imp_typecheker.hh`: Clase **ImpTypeChecker** que implementa la interfaz **TypeVisitor**. Este a su vez tiene un Environment de tipo **ImpType** que almacenará las variables (key) y sus tipos (value).
+ `imp_typecheker.cpp`: Definición de la clase **ImpTypeChecker**. Aquí se implementan los métodos de visitor para cada regla de la gramática.

#### Implementación del Typechecker

```cpp
cout << "Hello World" << endl;

 
```


### 1.2. Codegen

El Codegen es un visitor que recorre el AST parseado y genera el código intermedio en formato de tres direcciones. El código intermedio generado es almacenado en un vector de strings.


### Ejemplo de uso


> **⚠️ Reporte: ¿Cómo se calculó la memoria necesaria para las variables globales?**  \

Para calcular la memoria necesaria tanto para las variables globales como para las variables locales se utilizó el Typechecker.


## 2. Generación de Código I

En esta sección se describe la implementación de la inclusión de comentarios en el código fuente.

Como se ve en la [gramática](#gramática-del-lenguaje-imp-dec), se añadió la regla `Comment ::= "//" (~("\n"))` que permite la inclusión de comentarios en el código fuente. Esta regla permite que se incluyan comentarios de una sola línea al final de una **declaración de variable (`VarDec`), asignación (`AssignStm`) y de una impresión (`PrintStm`)**.

Para implementar esta funcionalidad se modificó ligeramente la gramática:

+ `VarDec`, `AssignStm` y `PrintStm` tienen al final un `Comment` opcional.
+ Se modificó el `StatementList` para que cada `Stm` esté seguido de un `;` y luego de un `Comment` opcional.
  
```js
StatementList ::= Stm (“;” Stm)*  ➡️  StatementList ::= (Stm)+
```

### Cambios en el Scanner

Para implementar la inclusión de comentarios en el código fuente, se modificó el Scanner para que asigne un token a los comentarios. Para ello, cuando el Scanner detecta un "//" en el input procede a saltarse todos los caracteres hasta encontrar un salto de línea "\n".

Primero, se agregó el token `COMMENT` en los archivos `imp_parser.hh` y `imp_parser.cpp`.

```cpp	
// imp_parser.hh
class Token {
  enum Type { 
    ...
    COMMENT,
    ...
  };
}

// imp_parser.cpp
const char *Token::token_names[34] = {
  ...
  "COMMENT",
  ...
};
```

Luego, se modificó el método `next_token()` en el archivo `imp_scanner.cpp` para que detecte los comentarios.

```cpp
Token *Scanner::nextToken(){
  char c;
  ...
  else if (strchr("()+-*/;=<,", c)) {
    switch(c) {
      ...
      case '/':
				c = nextChar();
				if (c == '/') {
					while (c != '\n') c = nextChar(); // Hasta encontrar un salto de línea
					rollBack();
					token = new Token(Token::COMMENT, getLexema()); // COMMENT
				} else {
					rollBack();
					token = new Token(Token::DIV);
				}
				break;
      ...
    }
  }
}
```


### Cambios en el Parser

Para que el Parser soporte la inclusión de comentarios en el código fuente, se creó una nueva clase `Comment`, la cual almacenará el comentario, en el archivo `imp.hh`. 

```cpp
// Comment class
class Comment {
public:
	string comment;
	Comment();
	Comment(string comment);
	void accept(ImpVisitor* v); // Solo se utiliza para el printer
	~Comment();
};
```

Además, se agregó como atributo de la clase `VarDec`, `AssignStm` y `PrintStm` un puntero a un objeto de tipo `Comment` en el archivo `imp.hh` y sus respectivas definiciones en el archivo `imp.cpp`.

```cpp

// imp.hh
class AssignStatement : public Stm {
public:
  ...
  Comment* cmt;
  AssignStatement(Var* var, Exp* exp, Comment* cmt);
  ...
};

class PrintStatement : public Stm {
public:
  ...
  Comment* cmt;
  PrintStatement(Exp* exp, Comment* cmt);
  ...
};

class VarDec {
public:
  ...
  Comment* cmt;
  VarDec(Type* type, VarList* varList, Comment* cmt);
  ...
};

// imp.cpp
// Se modificaron sus constructores 
```

Luego, se agregó un nuevo método para parsear el comentario en el archivo `imp_parser.cpp`.

```cpp
class Parser {
  ...
  Comment* parseComment();
  ...
};
```

Y finalmente, se modificó el parseo de las reglas `VarDec`, `AssignStm` y `PrintStm` para que puedan incluir comentarios de acuerdo a la [gramática](#gramática-del-lenguaje-imp-dec) en el archivo `imp_parser.cpp`.

```cpp
// parser de Comment
Comment *Parser::parseCommment() {
	Comment *c = NULL;
	if (match(Token::COMMENT)) {
		c = new Comment(previous->lexema);
	}
	return c;
}

// parser de VarDec
VarDec *Parser::parseVarDec() {
	VarDec *vd = NULL;

	if (match(Token::VAR)) {
		Comment* comment = NULL;
		if (!match(Token::ID))
			parserError("Expecting type in var declaration");

		string var, type = previous->lexema;
		list<string> vars;

		if (!match(Token::ID))
			parserError("Expecting id in var declaration");

		var = previous->lexema;
		vars.push_back(var);

		while (match(Token::COMMA)) {
			if (!match(Token::ID))
				parserError("Expecting id in comma in var declaration");
			var = previous->lexema;
			vars.push_back(var);
		}

		if (!match(Token::SEMICOLON))
			parserError("Expecting semicolon at end of var declaration");

		comment = parseCommment();

		vd = new VarDec(type, vars, comment);
	}
	return vd;
}

// parser de StatementList
StatementList *Parser::parseStatementList() {
	StatementList *p = new StatementList();
	Stm* stm;
	stm = parseStatement();

	while (stm != NULL) {
		p->add(stm);
		stm = parseStatement();
	}
	return p;
}

// parser de Stm (AssignStm y PrintStm)
Stm *Parser::parseStatement() {
	Stm *s = NULL;
	Exp *e;
	Body *tb, *fb;
	Comment *comment;

	// AssignStatement Parser
	if (match(Token::ID)) {
		string lex = previous->lexema;
		if (!match(Token::ASSIGN)) {
			parserError("Expecting assignment (=) operator");
		}

		e = parseBExp();

		if (!match(Token::SEMICOLON))
			parserError("Expecting semicolon at end of AssigmentStatement declaration");

		comment = parseCommment();

		s = new AssignStatement(lex, e, comment);

	// PrintStatement Parser
	} else if (match(Token::PRINT)) {
		if (!match(Token::LPAREN)) {
			parserError("Expecting left parenthesis '(' ");
		}
		e = parseBExp();
		if (!match(Token::RPAREN)) {
			parserError("Expecting right parenthesis ')' ");
		}
		if (!match(Token::SEMICOLON))
			parserError("Expecting semicolon at end of PrintStatement declaration");

		comment = parseCommment();

		s = new PrintStatement(e, comment);
  }
  ...
  return s;
}
```

### Cambios en el ImpPrinter

Como los comentarios no son necesarios para el análisis Semántico, se modificó solamente el `ImpVisitor` y `ImpPrinter` para que imprima los comentarios en el código fuente.

```cpp
// imp_visitor.cpp
class ImpVisitor {
public:
  ...
  virtual void visit(Comment* e) = 0;
  ...
}

// imp_printer.cpp
class ImpPrinter : public ImpVisitor {
public:
  ...
  void visit(Comment* e);
  ...
}
```

Y se 


> **⚠️ Reporte: ¿Qué cambios se hicieron al scanner y/o parser para lograr la inclusión de comentarios?**  \
Respuesta


## 3. Sentencia Do While 


> **⚠️ Reporte: Indicar el cambio a la gramática y los puntos donde se hicieron cambios al código. Además, proveer las definiciones de tcheck y codegen usadas**  \
Respuesta


## Autores

| **Benjamin Soto** | **Fabrizzio Vilchez** | **Jeffrey Monja** |
|:------------:|:------------:|:------------:|
| ![Benjamin Soto](https://avatars.githubusercontent.com/u/104233590?v=4) | ![Fabrizzio Vilchez](https://avatars.githubusercontent.com/u/115495332?v=4) | ![Jeffrey Monja](https://avatars.githubusercontent.com/u/104637993?v=4) |
| [https://github.com/SotoBenjamin](https://github.com/SotoBenjamin) | [https://github.com/Fabrizzio20k](https://github.com/Fabrizzio20k) | [https://github.com/jeffreymonjacastro](https://github.com/jeffreymonjacastro) |


## Referencias 

- W. Appel. (2002) Modern compiler implementation in Java. 2.a edición. CambridgeUniversity Press.
- Kenneth C. Louden. (2004) Compiler Construction: Principles and Practice.Thomson.
