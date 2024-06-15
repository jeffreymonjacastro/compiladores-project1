#include "imp_value.hh"

ImpValue::ImpValue() : type(NOTYPE) {}

TType ImpValue::get_basic_type(string s) {
	TType tt;
	if (s.compare("int") == 0) tt = INT;
	else if (s.compare("bool") == 0) tt = BOOL;
	else tt = NOTYPE;
	return tt;
}

void ImpValue::set_default_value(TType tt) {
	type = tt;
	if (tt == INT) {
		int_value = 0;
	} else if (tt == BOOL) {
		bool_value = true;
	}
	return;
}

std::ostream &operator<<(std::ostream &outs, const ImpValue &v) {
	if (v.type == INT)
		outs << v.int_value;
	else if (v.type == BOOL) {
		if (v.bool_value)
			outs << "true";
		else
			outs << "false";
	} else {
		outs << "NOTYPE";
	}
	return outs;
}


