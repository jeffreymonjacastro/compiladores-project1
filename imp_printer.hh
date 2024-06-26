#ifndef IMP_PRINTER
#define IMP_PRINTER

#include "imp.hh"
#include "imp_visitor.hh"

class ImpPrinter : public ImpVisitor {
public:
	void print(Program*);
	void visit(Program*);
	void visit(Body*);
	void visit(VarDecList*);
	void visit(VarDeclaration*);
	void visit(CommentVarDec*); // Comment
	void visit(StatementList*);
	void visit(AssignStatement*);
	void visit(PrintStatement*);
	void visit(IfStatement*);
	void visit(WhileStatement*);
	void visit(DoWhileStatement*);
	void visit(CommentStatement*); // Comment
	void visit(Comment*); // Comment


	int visit(BinaryExp* e);
	int visit(UnaryExp* e);
	int visit(NumberExp* e);
	int visit(IdExp* e);
	int visit(ParenthExp* e);
	int visit(CondExp* e);
	int visit(BoolExp* e);
};


#endif

