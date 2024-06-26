#ifndef IMP_AST
#define IMP_AST

#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>

#include <list>

#include "imp_value.hh"

using namespace std;

class ImpVisitor;
class ImpValueVisitor;
class TypeVisitor;

enum BinaryOp { PLUS, MINUS, MULT, DIV, EXP, LT, LTEQ, EQ, AND, OR};
enum UnaryOp { NEG, NOT };

class Exp {
public:
	virtual int accept(ImpVisitor* v) = 0;
	virtual ImpValue accept(ImpValueVisitor* v) = 0;
	virtual ImpType accept(TypeVisitor* v) = 0;
	static string binopToString(BinaryOp op);
	static string unopToString(UnaryOp op);
	virtual ~Exp() = 0;
};

class BinaryExp : public Exp {
public:
	Exp *left, *right;
	BinaryOp op;
	BinaryExp(Exp* l, Exp* r, BinaryOp op);
	int accept(ImpVisitor* v);
	ImpValue accept(ImpValueVisitor* v);
	ImpType accept(TypeVisitor* v);
	~BinaryExp();
};

class UnaryExp : public Exp {
public:
	Exp* e;
	UnaryOp op;
	UnaryExp(Exp* e, UnaryOp op);
	int accept(ImpVisitor* v);
	ImpValue accept(ImpValueVisitor* v);
	ImpType accept(TypeVisitor* v);
	~UnaryExp();
};

class NumberExp : public Exp {
public:
	int value;
	NumberExp(int v);
	int accept(ImpVisitor* v);
	ImpValue accept(ImpValueVisitor* v);
	ImpType accept(TypeVisitor* v);
	~NumberExp();
};

class IdExp : public Exp {
public:
	string id;
	IdExp(string id);
	int accept(ImpVisitor* v);
	ImpValue accept(ImpValueVisitor* v);
	ImpType accept(TypeVisitor* v);
	~IdExp();
};

class ParenthExp : public Exp {
public:
	Exp *e;
	ParenthExp(Exp *e);
	int accept(ImpVisitor* v);
	ImpValue accept(ImpValueVisitor* v);
	ImpType accept(TypeVisitor* v);
	~ParenthExp();
};

class CondExp : public Exp {
public:
	Exp *cond, *etrue, *efalse;
	CondExp(Exp* c, Exp* et, Exp* ef);
	int accept(ImpVisitor* v);
	ImpValue accept(ImpValueVisitor* v);
	ImpType accept(TypeVisitor* v);
	~CondExp();
};

class BoolExp : public Exp {
public:
	bool value;
	BoolExp(bool b);
	int accept(ImpVisitor* v);
	ImpValue accept(ImpValueVisitor* v);
	ImpType accept(TypeVisitor* v);
	~BoolExp();
};

class Stm {
public:
	bool isComment = false;
	Stm(){};
	Stm(bool ic);
	virtual void accept(ImpVisitor* v) = 0;
	virtual void accept(ImpValueVisitor* v)=0;
	virtual void accept(TypeVisitor* v)=0;
	virtual ~Stm() = 0;
};

class StatementList;
class Body;
class Comment;

class AssignStatement : public Stm {
public:
	string id;
	Exp* rhs;
	Comment* cmt;
	AssignStatement(string id, Exp* e, Comment* cmt);
	void accept(ImpVisitor* v);
	void accept(ImpValueVisitor* v);
	void accept(TypeVisitor* v);
	~AssignStatement();
};

class PrintStatement : public Stm {
public:
	Exp* e;
	Comment* cmt;
	PrintStatement(Exp* e, Comment* cmt);
	void accept(ImpVisitor* v);
	void accept(ImpValueVisitor* v);
	void accept(TypeVisitor* v);
	~PrintStatement();
};

class IfStatement : public Stm {
public:
	Exp* cond;
	Body *tbody, *fbody;
	Comment* cmt;
	IfStatement(Exp* c, Body* tbody, Body *fbody, Comment* cmt);
	void accept(ImpVisitor* v);
	void accept(ImpValueVisitor* v);
	void accept(TypeVisitor* v);
	~IfStatement();
};

class WhileStatement : public Stm {
public:
	Exp* cond;
	Body *body;
	Comment* cmt;
	WhileStatement(Exp* c, Body* b, Comment* cmt);
	void accept(ImpVisitor* v);
	void accept(ImpValueVisitor* v);
	void accept(TypeVisitor* v);
	~WhileStatement();
};

class DoWhileStatement : public Stm {
public:
	Body *body;
	Exp* cond;
	Comment* cmt;
	DoWhileStatement(Exp* c, Body *b, Comment* cmt);
	void accept(ImpVisitor* v);
	void accept(ImpValueVisitor* v);
	void accept(TypeVisitor* v);
	~DoWhileStatement();
};

class CommentStatement : public Stm {
public:
	string comment;
	CommentStatement(string comment);
	void accept(ImpVisitor* v);
	void accept(ImpValueVisitor* v);
	void accept(TypeVisitor* v);
	~CommentStatement();
};


class StatementList {
public:
	list<Stm*> slist;
	StatementList();
	void add(Stm* s);
	void accept(ImpVisitor* v);
	void accept(ImpValueVisitor* v);
	void accept(TypeVisitor* v);
	~StatementList();
};

class VarDec {
public:
	bool isComment;
	VarDec(bool ic);
	virtual void accept(ImpVisitor* v) = 0;
	virtual void accept(ImpValueVisitor* v) = 0;
	virtual void accept(TypeVisitor* v) = 0;
	virtual ~VarDec() = 0;
};

class VarDeclaration : public VarDec {
public:
	string type;
	list<string> vars;
	Comment* cmt;
	VarDeclaration(string type, list<string> vars, Comment* cmt);
	void accept(ImpVisitor* v);
	void accept(ImpValueVisitor* v);
	void accept(TypeVisitor* v);
	~VarDeclaration();
};

class CommentVarDec : public VarDec {
public:
	string comment;
	CommentVarDec(string comment);
	void accept(ImpVisitor* v);
	void accept(ImpValueVisitor* v);
	void accept(TypeVisitor* v);
	~CommentVarDec();
};


class VarDecList {
public:
	list<VarDec*> vdlist;
	VarDecList();
	void add(VarDec* s);
	void accept(ImpVisitor* v);
	void accept(ImpValueVisitor* v);
	void accept(TypeVisitor* v);
	~VarDecList();
};


class Body {
public:
	VarDecList* var_decs;
	StatementList* slist;
	Body(VarDecList* vdl, StatementList* sl);
	void accept(ImpVisitor* v);
	void accept(ImpValueVisitor* v);
	void accept(TypeVisitor* v);
	~Body();
};

class Program {
public:
	Body* body;
	Program(Body* body);
	void accept(ImpVisitor* v);
	void accept(ImpValueVisitor* v);
	void accept(TypeVisitor* v);
	~Program();
};

// Comment class
class Comment {
public:
	string comment;
	Comment();
	Comment(string comment);
	void accept(ImpVisitor* v);
	~Comment();
};


#endif

