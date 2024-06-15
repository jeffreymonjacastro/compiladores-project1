#ifndef IMP_VALUE_VISITOR
#define IMP_VALUE_VISITOR

#include "imp.hh"
#include "imp_value.hh"

class ImpValueVisitor {
public:
	virtual void visit(Program* p) = 0;
	virtual void visit(Body* b) = 0;
	virtual void visit(VarDecList* e) = 0;
	virtual void visit(VarDeclaration* e) = 0;
	virtual void visit(CommentVarDec* e) = 0; // Comment
	virtual void visit(StatementList* e) = 0;
	virtual void visit(AssignStatement* e) = 0;
	virtual void visit(PrintStatement* e) = 0;
	virtual void visit(IfStatement* e) = 0;
	virtual void visit(WhileStatement* e) = 0;
	virtual void visit(DoWhileStatement* e) = 0;
	virtual void visit(CommentStatement* e) = 0; // Comment

	virtual ImpValue visit(BinaryExp* e) = 0;
	virtual ImpValue visit(UnaryExp* e) = 0;
	virtual ImpValue visit(NumberExp* e) = 0;
	virtual ImpValue visit(IdExp* e) = 0;
	virtual ImpValue visit(ParenthExp* e) = 0;
	virtual ImpValue visit(CondExp* e) = 0;
	virtual ImpValue visit(BoolExp* e) = 0;
};


#endif
