#include "imp.hh"
#include "imp_visitor.hh"
#include "imp_value_visitor.hh"
#include "type_visitor.hh"

string Exp::binopToString(BinaryOp op) {
	switch (op) {
		case PLUS:
			return "+";
		case MINUS:
			return "-";
		case MULT:
			return "*";
		case DIV:
			return "/";
		case EXP:
			return "**";
		case LT:
			return "<";
		case LTEQ:
			return "<=";
		case EQ:
			return "==";
		case AND:
			return "and";
		case OR:
			return "or";
	}
	return "";
}


// ####################### Expressions #####################################

// Constructors
BinaryExp::BinaryExp(Exp *l, Exp *r, BinaryOp op) : left(l), right(r), op(op) {}

NumberExp::NumberExp(int v) : value(v) {}

IdExp::IdExp(string id) : id(id) {}

ParenthExp::ParenthExp(Exp *e) : e(e) {}

CondExp::CondExp(Exp *c, Exp *et, Exp *ef) : cond(c), etrue(et), efalse(ef) {}

BoolExp::BoolExp(bool b) : value(b) {}

Exp::~Exp() {}

BinaryExp::~BinaryExp() {
	delete left;
	delete right;
}

NumberExp::~NumberExp() {}

IdExp::~IdExp() {}

ParenthExp::~ParenthExp() { delete e; }

CondExp::~CondExp() {
	delete cond;
	delete etrue;
	delete efalse;
}

BoolExp::~BoolExp() {}

// ImpVisitor
int BinaryExp::accept(ImpVisitor *v) {
	return v->visit(this);
}

int NumberExp::accept(ImpVisitor *v) {
	return v->visit(this);
}

int IdExp::accept(ImpVisitor *v) {
	return v->visit(this);
}

int ParenthExp::accept(ImpVisitor *v) {
	return v->visit(this);
}

int CondExp::accept(ImpVisitor *v) {
	return v->visit(this);
}

int BoolExp::accept(ImpVisitor *v) {
	return v->visit(this);
}

// ImpValueVisitor
ImpValue BinaryExp::accept(ImpValueVisitor *v) {
	return v->visit(this);
}

ImpValue NumberExp::accept(ImpValueVisitor *v) {
	return v->visit(this);
}

ImpValue IdExp::accept(ImpValueVisitor *v) {
	return v->visit(this);
}

ImpValue ParenthExp::accept(ImpValueVisitor *v) {
	return v->visit(this);
}

ImpValue CondExp::accept(ImpValueVisitor *v) {
	return v->visit(this);
}

ImpValue BoolExp::accept(ImpValueVisitor *v) {
	return v->visit(this);
}

// TypeVisitor
ImpType BinaryExp::accept(TypeVisitor *v) {
	return v->visit(this);
}

ImpType NumberExp::accept(TypeVisitor *v) {
	return v->visit(this);
}

ImpType IdExp::accept(TypeVisitor *v) {
	return v->visit(this);
}

ImpType ParenthExp::accept(TypeVisitor *v) {
	return v->visit(this);
}

ImpType CondExp::accept(TypeVisitor *v) {
	return v->visit(this);
}

ImpType BoolExp::accept(TypeVisitor *v) {
	return v->visit(this);
}

// ############################# STATEMENTS ################################

Stm::Stm(bool ic): isComment(ic) {}

AssignStatement::AssignStatement(string id, Exp *e, Comment* cmt) : id(id), rhs(e), cmt(cmt) {}

PrintStatement::PrintStatement(Exp *e, Comment* cmt) : e(e), cmt(cmt) {}

IfStatement::IfStatement(Exp *c, Body *tb, Body *fb, Comment* cmt) : cond(c), tbody(tb), fbody(fb), cmt(cmt) {}

WhileStatement::WhileStatement(Exp *c, Body *b, Comment* cmt) : cond(c), body(b), cmt(cmt) {}

DoWhileStatement::DoWhileStatement(Exp *c, Body *b, Comment* cmt) : cond(c), body(b), cmt(cmt) {}

CommentStatement::CommentStatement(string comment): comment(comment), Stm(true) {}

StatementList::StatementList() : slist() {}

VarDec::VarDec(bool ic) : isComment(ic) {}

VarDeclaration::VarDeclaration(string type, list<string> vars, Comment* cmt) : type(type), vars(vars), cmt(cmt), VarDec(false) {}

CommentVarDec::CommentVarDec(string comment) : comment(comment), VarDec(true) {}

VarDecList::VarDecList() : vdlist() {}

Body::Body(VarDecList *vdl, StatementList *sl) : var_decs(vdl), slist(sl) {}

Program::Program(Body *b) : body(b) {}

Comment::Comment() {}

Comment::Comment(string comment): comment(comment) {}

Stm::~Stm() {}

AssignStatement::~AssignStatement() { delete rhs; }

PrintStatement::~PrintStatement() { delete e; }

IfStatement::~IfStatement() {
	delete fbody;
	delete tbody;
	delete cond;
}

WhileStatement::~WhileStatement() {
	delete body;
	delete cond;
}

DoWhileStatement::~DoWhileStatement() {
	delete body;
	delete cond;
}

CommentStatement::~CommentStatement() {}

StatementList::~StatementList() {}

VarDeclaration::~VarDeclaration() {}

CommentVarDec::~CommentVarDec() {}

VarDec::~VarDec() {}

VarDecList::~VarDecList() {}

Body::~Body() {
	delete slist;
	delete var_decs;
}

Program::~Program() { delete body; }

Comment::~Comment() {}

// ImpVisitor
void AssignStatement::accept(ImpVisitor *v) {
	return v->visit(this);
}

void PrintStatement::accept(ImpVisitor *v) {
	return v->visit(this);
}

void IfStatement::accept(ImpVisitor *v) {
	return v->visit(this);
}

void WhileStatement::accept(ImpVisitor *v) {
	return v->visit(this);
}

void DoWhileStatement::accept(ImpVisitor *v) {
	return v->visit(this);
}

void CommentStatement::accept(ImpVisitor *v) {
	return v->visit(this);
}

void StatementList::add(Stm *s) { slist.push_back(s); }

void StatementList::accept(ImpVisitor *v) {
	return v->visit(this);
}

void VarDeclaration::accept(ImpVisitor *v) {
	return v->visit(this);
}

void CommentVarDec::accept(ImpVisitor *v) {
	return v->visit(this);
}

void VarDecList::add(VarDec *vd) { vdlist.push_back(vd); }

void VarDecList::accept(ImpVisitor *v) {
	return v->visit(this);
}

void Body::accept(ImpVisitor *v) {
	return v->visit(this);
}

void Program::accept(ImpVisitor *v) {
	return v->visit(this);
}

void Comment::accept(ImpVisitor *v) {
	return v->visit(this);
}


// ImpValueVisitor
void AssignStatement::accept(ImpValueVisitor *v) {
	return v->visit(this);
}

void PrintStatement::accept(ImpValueVisitor *v) {
	return v->visit(this);
}

void IfStatement::accept(ImpValueVisitor *v) {
	return v->visit(this);
}

void WhileStatement::accept(ImpValueVisitor *v) {
	return v->visit(this);
}

void DoWhileStatement::accept(ImpValueVisitor *v) {
	return v->visit(this);
}

void CommentStatement::accept(ImpValueVisitor *v) {
	return v->visit(this);
}

void StatementList::accept(ImpValueVisitor *v) {
	return v->visit(this);
}

void VarDeclaration::accept(ImpValueVisitor *v) {
	return v->visit(this);
}

void CommentVarDec::accept(ImpValueVisitor *v) {
	return v->visit(this);
}

void VarDecList::accept(ImpValueVisitor *v) {
	return v->visit(this);
}

void Body::accept(ImpValueVisitor *v) {
	return v->visit(this);
}

void Program::accept(ImpValueVisitor *v) {
	return v->visit(this);
}

// TypeVisitor
void AssignStatement::accept(TypeVisitor *v) {
	return v->visit(this);
}

void PrintStatement::accept(TypeVisitor *v) {
	return v->visit(this);
}

void IfStatement::accept(TypeVisitor *v) {
	return v->visit(this);
}

void WhileStatement::accept(TypeVisitor *v) {
	return v->visit(this);
}

void DoWhileStatement::accept(TypeVisitor *v) {
	return v->visit(this);
}

void CommentStatement::accept(TypeVisitor *v) {
	return v->visit(this);
}

void StatementList::accept(TypeVisitor *v) {
	return v->visit(this);
}

void VarDeclaration::accept(TypeVisitor *v) {
	return v->visit(this);
}

void CommentVarDec::accept(TypeVisitor *v) {
	return v->visit(this);
}

void VarDecList::accept(TypeVisitor *v) {
	return v->visit(this);
}

void Body::accept(TypeVisitor *v) {
	return v->visit(this);
}

void Program::accept(TypeVisitor *v) {
	return v->visit(this);
}

