#include "Predictor.hpp"

namespace dvmpredictor {
	Predictor::Predictor() {};

	void Predictor::init(PGrid pgrid)
	{
		_pgrid = pgrid;
	}

	Template Predictor::declare_template(Shape shape)
	{
		return _pgrid.declare_template(shape);
	}

	DArray Predictor::declare_darray(Shape shape, uint32_t elem_size)
	{
		return _pgrid.declare_darray(shape, elem_size);
	}

	void Predictor::distribute(Template t, DRule rule)
	{
		return _pgrid.distribute(t, rule);
	}

	void Predictor::distribute(DArray a, DRule rule)
	{
		return _pgrid.distribute(a, rule);
	}

	void Predictor::redistribute(Template t, DRule rule)
	{
		return _pgrid.redistribute(t, rule);
	}

	void Predictor::redistribute(DArray a, DRule rule)
	{
		return _pgrid.redistribute(a, rule);
	}

	void Predictor::align_on(DArray a, Template t, ARule rule)
	{
		return _pgrid.align_on(a, t, rule);
	}

	void Predictor::align_on(DArray a, DArray b, ARule rule)
	{
		return _pgrid.align_on(a, b, rule);
	}

	void Predictor::realign_on(DArray a, Template t, ARule rule)
	{
		return _pgrid.realign_on(a, t, rule);
	}

	void Predictor::realign_on(DArray a, DArray b, ARule rule)
	{
		return _pgrid.realign_on(a, b, rule);
	}
}
