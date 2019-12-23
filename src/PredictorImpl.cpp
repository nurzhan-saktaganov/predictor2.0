#include "PredictorImpl.hpp"

namespace dvmpredictor {
	PredictorImpl::PredictorImpl() {}

	PredictorImpl::PredictorImpl(Shape pgrid, double latency, double bandwidth) :
		_pgrid(pgrid, latency, bandwidth)
	{}

	Template PredictorImpl::declare_template(Shape shape)
	{
		return _pgrid.declare_template(shape);
	}

	DArray PredictorImpl::declare_darray(Shape shape, Shadow shadow, uint32_t elem_size)
	{
		return _pgrid.declare_darray(shape, shadow, elem_size);
	}

	void PredictorImpl::distribute(Template t, DRule rule)
	{
		return _pgrid.distribute(t, rule);
	}

	void PredictorImpl::distribute(DArray a, DRule rule)
	{
		return _pgrid.distribute(a, rule);
	}

	void PredictorImpl::redistribute(Template t, DRule rule)
	{
		return _pgrid.redistribute(t, rule);
	}

	void PredictorImpl::redistribute(DArray a, DRule rule)
	{
		return _pgrid.redistribute(a, rule);
	}

	void PredictorImpl::align_with(DArray a, Template t, ARule rule)
	{
		return _pgrid.align_with(a, t, rule);
	}

	void PredictorImpl::align_with(DArray a, DArray b, ARule rule)
	{
		return _pgrid.align_with(a, b, rule);
	}

	void PredictorImpl::realign_with(DArray a, Template t, ARule rule)
	{
		return _pgrid.realign_with(a, t, rule);
	}

	void PredictorImpl::realign_with(DArray a, DArray b, ARule rule)
	{
		return _pgrid.realign_with(a, b, rule);
	}
}
