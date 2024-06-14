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
    - [Cambios en el Scanner](#cambios-en-el-scanner)
    - [Cambios en el Parser](#cambios-en-el-parser)
      - [Comentarios al final de una declaración de variable o statement](#comentarios-al-final-de-una-declaración-de-variable-o-statement)
      - [Comentarios aislados en cualquier parte del código](#comentarios-aislados-en-cualquier-parte-del-código)
    - [Cambios en el ImpPrinter](#cambios-en-el-impprinter)
  - [3. Sentencia Do While](#3-sentencia-do-while)
  - [Autores](#autores)
  - [Referencias](#referencias)

## Gramática del lenguaje IMP-DEC
La gramática que se utilizó en el proyecto es la siguiente:

```js
Program         ::= Body
Body            ::= VarDecList StatementList  
VarDecList      ::= (VarDec | Comment)* 
VarDec          ::= "var" Type VarList ";" (Comment)?
Type            ::= id
VarList         ::= id ("," id)*
StatementList   ::= (Stm ";" (Comment)? | Comment)*
Stm             ::= id "=" Exp                               |
                    "print" "(" Exp ")"                      |
                    "if" Exp "then" Body "else" Body "endif" |
                    "while" Exp "do" Body "endwhile"         |
                    "do" Body "while" Exp "enddo"                    
Exp             ::= BExp
BExp            ::= CEXP (("and" | "or") BExp)?
CEXP            ::= AExp (("==" | "<" | "<=" ) AExp)?
AExp            ::= Term (("+" | "-") Term)*
Term            ::= FExp (("*" | "/") FExp)*
FExp            ::= Factor ("**" FExp)?
Factor          ::= id                                       | 
                    num                                      | 
                    true                                     |
                    false                                    |
                    "(" Exp ")"                              | 
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

Como se ve en la [gramática](#gramática-del-lenguaje-imp-dec), se añadió la regla `Comment ::= "//" (~("\n"))` que permite la inclusión de comentarios en el código fuente. Esta regla permite que se incluyan comentarios de una sola línea al final de una **declaración de variable (`VarDec`), o statement (`Stm`)**, así como comentarios aislados **en cualquier parte del código**.

Para implementar esta funcionalidad se modificó ligeramente la gramática:

+ Se modificó el `VarDecList` para que pueda aceptar un `VarDec` o un `Comment`
  
```js
// Antes                       Después
VarDecList ::= (VarDec)*  ➡️  VarDecList ::= (VarDec | Comment)*
```

+ `VarDec` tienen al final un `Comment` opcional.

```js
// Antes                              Después
VarDec ::= "var" Type VarList ";"  ➡️  VarDec ::= "var" Type VarList ";" (Comment)?
```

+ Se modificó el `StatementList` para que cada `Stm` esté seguido de un `;` y luego de un `Comment` opcional o sea un `Comment` solo.
  
```js
// Antes                              Después
StatementList ::= Stm (“;” Stm)*  ➡️  StatementList ::= (Stm ";" (Comment)? | Comment)*
```

+ Se añadió la regla `Comment ::= "//" (~("\n"))*` que permite la inclusión de comentarios en el código fuente.


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

Para que el Parser soporte la inclusión de comentarios de una sola línea en el código fuente, se consideraron dos casos:
1. Comentarios al final de una declaración de variable (`VarDec`) o statement (`Stm`).
2. Comentarios aislados en cualquier parte del código.

#### Comentarios al final de una declaración de variable o statement

Después de cada `VarDec` o `Stm` se puede incluir un comentario, como en la [gramática](#gramática-del-lenguaje-imp-dec). Para ello, se creó una nueva clase `Comment` la cual almacenará el comentario al final de un `VarDec` o `Stm`. 


```cpp
/* imp.hh */
// Clase Comment para los comentarios en línea
class Comment {
public:
	string comment;
	Comment();
	Comment(string comment);
	void accept(ImpVisitor* v); // Solo se utiliza para el printer
	~Comment();
};
```

Además, se agregó como atributo de las clases derivadas de `Stm` y de `VarDec` un puntero a un objeto de tipo `Comment` para almacenar comentarios en línea. 

```cpp
/* imp.hh */
// Modificación del AssignStatement
class AssignStatement : public Stm {
public:
  ...
  Comment* cmt;
  AssignStatement(Var* var, Exp* exp, Comment* cmt);
  ...
};

// La misma modificación se realizó en cada Stm y VarDeclaration

/* imp.cpp */
// Se modificaron los constructores 
```

Luego, se agregó un nuevo método para parsear el comentario 

```cpp
/* imp_parser.hh */
class Parser {
  ...
  Comment* parseComment();
  ...
};

/* imp_parser.cpp */
Comment *Parser::parseCommment() {
	Comment *c = NULL;
	if (match(Token::COMMENT)) {
		c = new Comment(previous->lexema);
	}
	return c;
}
```

Y finalmente, se modificó el parseo de las reglas de `Stm` y `VarDec` para que puedan incluir comentarios de acuerdo a la [gramática](#gramática-del-lenguaje-imp-dec).

```cpp
/* imp_parser.cpp */

// Modificación al parser de VarDec
VarDec *Parser::parseVarDec() {
	VarDec *vd = NULL;

	if (match(Token::VAR)) {
		Comment* comment = NULL;
        ...
		comment = parseCommment();

		vd = new VarDec(type, vars, comment);
	}
	return vd;
}

// Modificación al parser de StatementList
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

// Modificación al parser de Stm
Stm *Parser::parseStatement() {
	Stm *s = NULL;
	Exp *e;
	Body *tb, *fb;
	Comment *comment;

	// AssignStatement Parser
	if (match(Token::ID)) {
		...
		comment = parseCommment();
		s = new AssignStatement(lex, e, comment);

	// PrintStatement Parser
	} else if (match(Token::PRINT)) {
		...
		comment = parseCommment();
		s = new PrintStatement(e, comment);
  }
  
  // Se realizó lo mismo para los demás Stm

  return s;
}
```

#### Comentarios aislados en cualquier parte del código

Los comentarios aislados pueden ir en cualquier parte del código. Para ello, se crearon dos nuevas clases que solo contendrán un comentario: 
  + `CommentStatement` que hereda de `Stm`
  + `CommentVarDec` que hereda de `VarDec`

> **Nota:** Se definió a `VarDec` como una interfaz que hereda a `VarDeclaration` (que contiene la declaración de variables) y `CommentVarDec` (que contiene el comentario). Con el fin de que `VarDecList` pueda aceptar ambos.

```cpp
/* imp.hh */
// Comentarios aislados en los statements
class CommentStatement : public Stm {
public:
	string comment;
	CommentStatement(string comment);
	void accept(ImpVisitor* v);
	void accept(ImpValueVisitor* v);
	void accept(TypeVisitor* v);
	~CommentStatement();
};

// Interfaz VarDec
class VarDec {
public:
	bool isComment;
	VarDec(bool ic);
	virtual void accept(ImpVisitor* v) = 0;
	virtual void accept(ImpValueVisitor* v) = 0;
	virtual void accept(TypeVisitor* v) = 0;
	virtual ~VarDec() = 0;
};

// Comentarios aislados en la declaración de variables
class CommentVarDec : public VarDec {
public:
	string comment;
	CommentVarDec(string comment);
	void accept(ImpVisitor* v);
	void accept(ImpValueVisitor* v);
	void accept(TypeVisitor* v);
	~CommentVarDec();
};
```

Luego, se modificó el parser para que pueda parsear los comentarios aislados en cualquier parte del código.

```cpp
/* imp_parser.cpp */
// Modificación al parser de VarDec
VarDec *Parser::parseVarDec() {
	VarDec *vd = NULL;

	if (match(Token::COMMENT)){
		vd = new CommentVarDec(previous->lexema);
	} else if (match(Token::VAR)) {
    ...
    vd = new VarDeclaration(type, vars, comment);
  }
  return vd;
}

// Modificación al parser de Stm
Stm *Parser::parseStatement() {
	Stm *s = NULL;
  ...
  else if(match(Token::COMMENT)){
    s = new CommentStatement(previous->lexema);
  }
}
```


### Cambios en los visitors

#### ImpVisitor

Se modificó el `ImpVisitor` y `ImpPrinter` para que imprima los comentarios en el código fuente.

```cpp
/* imp_visitor.hh */
class ImpVisitor {
public:
  ...
  virtual void visit(VarDeclaration* e) = 0;
  virtual void visit(CommentVarDec* e) = 0;
  virtual void visit(CommentStatement* e) = 0;
  virtual void visit(Comment* e) = 0;
  ...
}

/* imp_printer.hh */
class ImpPrinter : public ImpVisitor {
public:
  ...
  void visit(VarDeclaration*);
	void visit(CommentVarDec*);
  void visit(CommentStatement*);
  void visit(Comment* e);
  ...
}
```

Y se establecieron algunas reglas de printeo para los comentarios en el `ImpPrinter`.

```cpp
/* imp_printer.cpp */
// Modificación del VarDeclaration
void ImpPrinter::visit(VarDeclaration *vd) {
  ...
  if (vd->cmt == NULL){
		cout << ";" << endl;
		return;
	}

	cout << ";\t";
	vd->cmt->accept(this);
}

// Se realizó lo mismo para los demás Stm que tienen comentarios en línea como atributo 

void ImpPrinter::visit(CommentVarDec *c) {
	cout << c->comment << endl;
}

void ImpPrinter::visit(CommentStatement *s) {
	cout << s->comment << endl;
}

void ImpPrinter::visit(Comment* c) {
	cout << c->comment << endl;
}
```

#### ImpValueVisitor e ImpTypeVisitor

Como los comentarios no afectan la semántica del programa, no se realizaron cambios sustanciales en estos visitors. No obstante, para que no se genere un error al visitar un comentario, se añadió un método `accept` en las nuevas clases que solo se ignorarán.

```cpp
/* imp_interpreter.hh */
void ImpInterpreter::visit(CommentStatement *s) {
	return;
}

void ImpInterpreter::visit(CommentVarDec *c) {
	return;
}

/* imp_typechecker.hh */
// Se realizó lo mismo para el TypeChecker
```

### Ejemplo de funcionamiento

La inclusión de comentarios en el código fuente se puede ver en el siguiente ejemplo:

Input: 
```txt
var int x, y; // Enteros
// Booleana
var bool b;
x = 3; // Asignacion
b = x < 5; // Comparacion
if b then
    y = x*2; // Multiplicacion
else // Multiplicacion 2
    y = x*3;
endif; // Fin del if
// Print
print(y);

```

Output:
```txt
Program :
var int x, y;   // Enteros
// Booleana
var bool b;
x = 3;  // Asignacion
b = x < 5;      // Comparacion
if (b) then {
y = x * 2;      // Multiplicacion
}
else {
// Multiplicacion 2
y = x * 3;
}
endif;  // Fin de if
// Print
print(y);

Type checking:
Type checking OK

Run program:
6
```

### Consideraciones finales

Es complicado manejar los comentarios en el código fuente, ya que estos pueden estar en cualquier parte del código. Por ello, se decidió implementar dos tipos de comentarios: los comentarios en línea y los comentarios aislados. Los comentarios en línea se pueden incluir al final de una declaración de variable o statement, mientras que los comentarios aislados pueden ir en cualquier parte del código. Aun así, hay casos en los que los comentarios no se imprimen correctamente, como en el caso de los comentarios aislados que están al final de un `if` o `else`. Finalmente, se puede mejorar la implementación de los comentarios para que se puedan incluir comentarios de bloque.


> **⚠️ Reporte: ¿Qué cambios se hicieron al scanner y/o parser para lograr la inclusión de comentarios?**  \
Como se vió en la sección anterior [2. Generación de Código I](#2-generación-de-código-i), se modificó el Scanner para que asigne un token a los comentarios y el Parser para que pueda parsear los comentarios de acuerdo a la gramática para así poder imprimirlos.


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
