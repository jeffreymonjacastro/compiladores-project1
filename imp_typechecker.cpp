#include "imp_typechecker.hh"

void ImpTypeChecker::sp_incr(int n) {
	sp += n;
	if (sp > max_sp) max_sp = sp;
}

void ImpTypeChecker::sp_decr(int n) {
	sp -= n;
	if (sp < 0) {
		cout << "stack less than 0" << endl;
		exit(0);
	}
}

ImpTypeChecker::ImpTypeChecker() {
	inttype.set_basic_type("int");
	booltype.set_basic_type("bool");
	sp = max_sp = next_direc = mem_locals = 0;
}

void ImpTypeChecker::typecheck(Program *p) {
	env.clear();
	p->accept(this);
//	cout << "Max stack size: " << max_sp << endl;
//	cout << "Memory for local variables: " << mem_locals << endl;
	return;
}

void ImpTypeChecker::visit(Program *p) {
	p->body->accept(this);
	return;
}

void ImpTypeChecker::visit(Body *b) {
	env.add_level();
	b->var_decs->accept(this);
	b->slist->accept(this);
	env.remove_level();
	if (next_direc > mem_locals) mem_locals = next_direc;
	return;
}

void ImpTypeChecker::visit(VarDecList *decs) {
	list<VarDec *>::iterator it;
	for (it = decs->vdlist.begin(); it != decs->vdlist.end(); ++it) {
		(*it)->accept(this);
	}
	return;
}

void ImpTypeChecker::visit(VarDeclaration *vd) {
	ImpType type;
	type.set_basic_type(vd->type);
	if (type.ttype==ImpType::NOTYPE || type.ttype==ImpType::VOID) {
		cout << "Tipo invalido: " << vd->type << endl;
		exit(0);
	}

	list<string>::iterator it;
	for (it = vd->vars.begin(); it != vd->vars.end(); ++it) {
		env.add_var(*it, type);
		next_direc++;
	}
	return;
}

void ImpTypeChecker::visit(CommentVarDec *c) {
	return;
}

void ImpTypeChecker::visit(StatementList *s) {
	list<Stm *>::iterator it;
	for (it = s->slist.begin(); it != s->slist.end(); ++it) {
		(*it)->accept(this);
	}
	return;
}

void ImpTypeChecker::visit(AssignStatement *s) {
	ImpType type = s->rhs->accept(this);
	// typecheck
	if (!env.check(s->id)) {
		cout << "Variable " << s->id << " undefined" << endl;
		exit(0);
	}

	sp_decr(1);
	ImpType var_type = env.lookup(s->id);

	if (!type.match(var_type)) {
		cout << "Type mismatch in assignment" << endl;
		exit(0);
	}

	return;
}

void ImpTypeChecker::visit(PrintStatement *s) {
	s->e->accept(this);
	sp_decr(1);
	return;
}

void ImpTypeChecker::visit(IfStatement *s) {
	ImpType tcond = s->cond->accept(this);
	if (tcond.ttype != BOOL) {
		cout << "Type error en If: esperaba bool en condicional" << endl;
		exit(0);
	}

	sp_decr(1);
	s->tbody->accept(this);

	if (s->fbody != NULL)
		s->fbody->accept(this);
	return;
}

void ImpTypeChecker::visit(WhileStatement *s) {
	ImpType tcond = s->cond->accept(this);
	if (tcond.ttype != BOOL) {
		cout << "Type error en While: esperaba bool en condicional" << endl;
		exit(0);
	}

	sp_decr(1);

	s->body->accept(this);
	return;
}

void ImpTypeChecker::visit(DoWhileStatement* s) {
	s->body->accept(this);

	ImpType tcond = s->cond->accept(this);
	if (tcond.ttype != BOOL) {
		cout << "Type error en DoWhile: esperaba bool en condicional" << endl;
		exit(0);
	}

	sp_decr(1); // OJO
	return;
}

void ImpTypeChecker::visit(CommentStatement *s) {
	return;
}

ImpType ImpTypeChecker::visit(BinaryExp *e) {
	ImpType t1 = e->left->accept(this);
	ImpType t2 = e->right->accept(this);
	ImpType result;

	if (t1.ttype != t2.ttype) {
		cout << "Los tipos de los operandos no coinciden" << endl;
		exit(0);
	}

	switch (e->op) {
		case PLUS: result.ttype = INT; break;
		case MINUS: result.ttype = INT; break;
		case MULT: result.ttype = INT; break;
		case DIV: result.ttype = INT; break;
		case EXP: result.ttype = INT; break;
		case LT: result.ttype = BOOL; break;
		case LTEQ: result.ttype = BOOL; break;
		case EQ: result.ttype = BOOL; break;
		case AND: result.ttype = BOOL; break;
		case OR: result.ttype = BOOL; break;
	}

	sp_decr(1);
	return result;
}

ImpType ImpTypeChecker::visit(UnaryExp* e) {
	ImpType etype = e->e->accept(this);
	ImpType rtype;
	if (e->op == NEG) rtype.set_basic_type("int");
	else rtype.set_basic_type("bool");
	if (!etype.match(rtype)) {
		cout << "Tipo incorrecto para operacion binaria" << endl;
		exit(0);
	}
	return rtype;
}

ImpType ImpTypeChecker::visit(NumberExp *e) {
	sp_incr(1);
	return inttype;
}

ImpType ImpTypeChecker::visit(IdExp *e) {
	sp_incr(1);
	if (env.check(e->id))
		return env.lookup(e->id);
	else {
		cout << "Variable indefinida: " << e->id << endl;
		exit(0);
	}
}

ImpType ImpTypeChecker::visit(ParenthExp *ep) {
	return ep->e->accept(this);
}

ImpType ImpTypeChecker::visit(CondExp *e) {
	ImpType booltype;
	booltype.set_basic_type("bool");

	if (!e->cond->accept(this).match(booltype)) {
		cout << "Tipo en ifexp debe de ser bool" << endl;
		exit(0);
	}

	sp_decr(1);
	int sp_start = sp;
	ImpType ttype =  e->etrue->accept(this);
	sp = sp_start;

	if (!ttype.match(e->efalse->accept(this))) {
		cout << "Tipos en ifexp deben de ser iguales" << endl;
		exit(0);
	}
	return ttype;
}

ImpType ImpTypeChecker::visit(BoolExp *e) {
	sp_incr(1);
	return booltype;
}
