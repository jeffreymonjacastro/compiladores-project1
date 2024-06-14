#ifndef IMP_AST_VISITOR
#define IMP_AST_VISITOR

#include "imp.hh"

class ImpVisitor {
public:
	virtual void visit(Program* p) = 0;
	virtual void visit(Body* b) = 0;
	virtual void visit(VarDecList* e) = 0;
	virtual void visit(VarDeclaration* e) = 0;
	virtual void visit(CommentVarDec* e) = 0;
	virtual void visit(StatementList* e) = 0;
	virtual void visit(AssignStatement* e) = 0;
	virtual void visit(PrintStatement* e) = 0;
	virtual void visit(IfStatement* e) = 0;
	virtual void visit(WhileStatement* e) = 0;
	virtual void visit(CommentStatement* e) = 0;
	virtual void visit(Comment* e) = 0;

	virtual int visit(BinaryExp* e) = 0;
	virtual int visit(NumberExp* e) = 0;
	virtual int visit(IdExp* e) = 0;
	virtual int visit(ParenthExp* e) = 0;
	virtual int visit(CondExp* e) = 0;
	virtual int visit(BoolExp* e) = 0;
};


#endif
