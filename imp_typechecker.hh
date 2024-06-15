#ifndef IMP_TYPECHECKER
#define IMP_TYPECHECKER

#include <unordered_map>

#include "imp.hh"
#include "type_visitor.hh"
#include "environment.hh"
#include "imp_type.hh"

using namespace std;

using TType = ImpType::TType;

class ImpTypeChecker : public TypeVisitor {
public:
  	ImpTypeChecker();
	int sp, max_sp, next_direc, mem_locals;
private:
  	Environment<ImpType> env;
	ImpType booltype;
	ImpType inttype;
	void sp_incr(int n);
	void sp_decr(int n);

public:
	void typecheck(Program*);
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
	void visit(CommentStatement*); //Comment

	ImpType visit(BinaryExp* e);
	ImpType visit(UnaryExp* e);
	ImpType visit(NumberExp* e);
	ImpType visit(IdExp* e);
	ImpType visit(ParenthExp* e);
	ImpType visit(CondExp* e);
	ImpType visit(BoolExp* e);
};


#endif

