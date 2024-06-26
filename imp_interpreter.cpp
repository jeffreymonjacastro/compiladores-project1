#include "imp_interpreter.hh"


/* *************** Fin manejo del estado del programa ****** */

void ImpInterpreter::interpret(Program *p) {
	env.clear();
	p->accept(this);
	return;
}

void ImpInterpreter::visit(Program *p) {
	p->body->accept(this);
	return;
}

void ImpInterpreter::visit(Body *b) {
	env.add_level();
	b->var_decs->accept(this);
	b->slist->accept(this);
	env.remove_level();
	return;
}


void ImpInterpreter::visit(VarDecList *decs) {
	list<VarDec *>::iterator it;
	for (it = decs->vdlist.begin(); it != decs->vdlist.end(); ++it) {
		(*it)->accept(this);
	}
	return;
}

void ImpInterpreter::visit(VarDeclaration *vd) {
	list<string>::iterator it;
	ImpValue v;
	ImpType tt;
	tt.set_basic_type(vd->type);
	if (tt.ttype == NOTYPE) {
		cout << "Tipo invalido: " << vd->type << endl;
		exit(0);
	}
	v.set_default_value(tt.ttype);
	for (it = vd->vars.begin(); it != vd->vars.end(); ++it) {
		env.add_var(*it, v);
	}
	return;
}

void ImpInterpreter::visit(CommentVarDec *c) {
	return;
}

void ImpInterpreter::visit(StatementList *s) {
	list<Stm *>::iterator it;
	for (it = s->slist.begin(); it != s->slist.end(); ++it) {
		(*it)->accept(this);
	}
	return;
}

void ImpInterpreter::visit(AssignStatement *s) {
	ImpValue v = s->rhs->accept(this);
	if (!env.check(s->id)) {
		cout << "Variable " << s->id << " undefined" << endl;
		exit(0);
	}
	ImpValue lhs = env.lookup(s->id);
	env.update(s->id, v);
	return;
}

void ImpInterpreter::visit(PrintStatement *s) {
	ImpValue v = s->e->accept(this);
	cout << v << endl;
	return;
}

void ImpInterpreter::visit(IfStatement *s) {
	ImpValue v = s->cond->accept(this);
	if (v.bool_value) {
		s->tbody->accept(this);
	} else {
		if (s->fbody != NULL)
			s->fbody->accept(this);
	}
	return;
}

void ImpInterpreter::visit(WhileStatement *s) {
	ImpValue v = s->cond->accept(this);
	while (v.bool_value) {
		s->body->accept(this);
		v = s->cond->accept(this);
	}
	return;
}

void ImpInterpreter::visit(DoWhileStatement *s) {
	ImpValue v;
	do {
		s->body->accept(this);
		v = s->cond->accept(this);
	} while (v.bool_value);
	return;
}

void ImpInterpreter::visit(CommentStatement *s) {
	return;
}

ImpValue ImpInterpreter::visit(BinaryExp *e) {
	ImpValue result;
	ImpValue v1 = e->left->accept(this);
	ImpValue v2 = e->right->accept(this);

	if (v1.type != v2.type) {
		cout << "Error de tipos: operandos en operacion binaria deben de ser del mismo tipo" << endl;
		exit(0);
	}

	int iv, iv1, iv2;
	bool bv1, bv2, bv;

	TType type = NOTYPE;
	iv1 = v1.int_value;
	iv2 = v2.int_value;
	bv1 = v1.bool_value;
	bv2 = v2.bool_value;
	switch (e->op) {
		case PLUS:
			iv = iv1 + iv2;
			type = INT;
			break;
		case MINUS:
			iv = iv1 - iv2;
			type = INT;
			break;
		case MULT:
			iv = iv1 * iv2;
			type = INT;
			break;
		case DIV:
			iv = iv1 / iv2;
			type = INT;
			break;
		case EXP:
			iv = 1;
			while (iv2 > 0) {
				iv *= iv1;
				iv2--;
			}
			type = INT;
			break;
		case LT:
			bv = (iv1 < iv2);
			type = BOOL;
			break;
		case LTEQ:
			bv = (iv1 <= iv2);
			type = BOOL;
			break;
		case EQ:
			bv = (iv1 == iv2);
			type = BOOL;
			break;
		case AND:
			bv = (bv1 && bv2);
			type = BOOL;
			break;
		case OR:
			bv = (bv1 || bv2);
			type = BOOL;
			break;
	}
	if (type == INT) result.int_value = iv;
	else result.bool_value = bv;
	result.type = type;
	return result;
}

ImpValue ImpInterpreter::visit(UnaryExp* e) {
	ImpValue v = e->e->accept(this);
	ImpValue result;
	int iv = v.int_value;
	int bv = v.bool_value;

	if (e->op == NEG) {
		result.int_value = -iv;
		result.type = INT;
	} else {
		result.bool_value = !bv;
		result.type = BOOL;
	}

	return result;
}

ImpValue ImpInterpreter::visit(NumberExp *e) {
	ImpValue v;
	v.set_default_value(INT);
	v.int_value = e->value;
	return v;
}

ImpValue ImpInterpreter::visit(IdExp *e) {
	if (env.check(e->id))
		return env.lookup(e->id);
	else {
		cout << "Variable indefinida: " << e->id << endl;
		exit(0);
	}
}

ImpValue ImpInterpreter::visit(ParenthExp *ep) {
	return ep->e->accept(this);
}

ImpValue ImpInterpreter::visit(CondExp *e) {
	ImpValue v = e->cond->accept(this);

	if (v.bool_value == 0)
		return e->efalse->accept(this);
	else
		return e->etrue->accept(this);
}

ImpValue ImpInterpreter::visit(BoolExp* e) {
	ImpValue v;
	v.set_default_value(BOOL);
	v.bool_value = e->value;
	return v;
}
