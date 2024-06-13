#include "imp_printer.hh"

void ImpPrinter::print(Program *p) {
	cout << "Program : " << endl;
	p->accept(this);
	return;
}

void ImpPrinter::visit(Program *p) {
	p->body->accept(this);
	return;
}

void ImpPrinter::visit(Body *b) {
	b->var_decs->accept(this);
	b->slist->accept(this);
	return;
}

void ImpPrinter::visit(VarDecList *s) {
	list<VarDec *>::iterator it;
	for (it = s->vdlist.begin(); it != s->vdlist.end(); ++it) {
		(*it)->accept(this);
	}
	return;
}

void ImpPrinter::visit(VarDec *vd) {
	bool first = true;
	cout << "var " << vd->type << " ";
	list<string>::iterator it;
	for (it = vd->vars.begin(); it != vd->vars.end(); ++it) {
		if (!first) cout << ", ";
		first = false;
		cout << *it;
	}

	if (vd->cmt == NULL){
		cout << ";" << endl;
		return;
	}

	cout << "; " << vd->cmt->comment << endl;
}

void ImpPrinter::visit(StatementList *s) {
//	cout << "{" << endl;
	list<Stm *>::iterator it;
	for (it = s->slist.begin(); it != s->slist.end(); ++it) {
		(*it)->accept(this);
	}
//	cout << "}" << endl;
}

void ImpPrinter::visit(AssignStatement *s) {
	cout << s->id << " = ";
	s->rhs->accept(this);

	if (s->cmt == NULL){
		cout << ";" << endl;
		return;
	}

	cout << "; " << s->cmt->comment << endl;
}

void ImpPrinter::visit(PrintStatement *s) {
	cout << "print(";
	s->e->accept(this);
	cout << ")";

	if (s->cmt == NULL){
		cout << ";" << endl;
		return;
	}

	cout << "; " << s->cmt->comment << endl;
}

void ImpPrinter::visit(IfStatement *s) {
	cout << "if (";
	s->cond->accept(this);
	cout << ") then {" << endl;;
	s->tbody->accept(this);
	if (s->fbody != NULL) {
		cout << "}\nelse {" << endl;
		s->fbody->accept(this);
	}

	cout << "}\nendif;" << endl;
}

void ImpPrinter::visit(WhileStatement *s) {
	cout << "while (";
	s->cond->accept(this);
	cout << ") do" << endl;;
	s->body->accept(this);
	cout << "endwhile";
	return;
}


int ImpPrinter::visit(BinaryExp *e) {
	e->left->accept(this);
	cout << ' ' << Exp::binopToString(e->op) << ' ';
	e->right->accept(this);
	return 0;
}

int ImpPrinter::visit(NumberExp *e) {
	cout << e->value;
	return 0;
}

int ImpPrinter::visit(IdExp *e) {
	cout << e->id;
	return 0;
}

int ImpPrinter::visit(ParenthExp *ep) {
	cout << '(';
	ep->e->accept(this);
	cout << ')';
	return 0;
}

int ImpPrinter::visit(CondExp *e) {
	cout << "ifexp(";
	e->cond->accept(this);
	cout << ",";
	e->etrue->accept(this);
	cout << ",";
	e->efalse->accept(this);
	cout << ')';
	return 0;
}

int ImpPrinter::visit(BoolExp *e) {
	cout << (e->value ? "true" : "false");
	return 0;
}

// Comment
void ImpPrinter::visit(Comment* c) {
	cout << c->comment << endl;
}