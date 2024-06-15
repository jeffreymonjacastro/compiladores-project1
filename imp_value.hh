#ifndef IMP_VALUE
#define IMP_VALUE

#include "imp_type.hh"

using TType = ImpType::TType;
const TType NOTYPE = ImpType::TType::NOTYPE;
const TType INT = ImpType::TType::INT;
const TType BOOL = ImpType::TType::BOOL;

class ImpValue {
public:
	ImpValue();

	TType type;

	// podemos ser mas eficientes con la memoria?
	int int_value;
	bool bool_value;

	void set_default_value(TType tt);

	static TType get_basic_type(string s);
};

std::ostream &operator<<(std::ostream &outs, const ImpValue &v);


#endif
