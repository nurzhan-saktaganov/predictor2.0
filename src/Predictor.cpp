#include <cassert>

#include "Predictor.hpp"
#include "PredictorImpl.hpp"

namespace dvmpredictor {
	Predictor::Predictor() :
		impl(nullptr)
	{}

	Predictor::Predictor(Shape pgrid, double latency, double bandwidth) :
		impl(new PredictorImpl(pgrid, latency, bandwidth))
	{}

	Predictor::Predictor(const Predictor &p) :
		impl(nullptr)
	{
		if (p.impl != nullptr) {
			impl = new PredictorImpl;
			*impl = *p.impl;
		}
	}

	Predictor &Predictor::operator=(const Predictor &p)
	{
		if (this == &p)
			return *this;

		if (p.impl == nullptr) {
			if (impl != nullptr)
				delete impl;
			impl = nullptr;
		} else {
			if (impl == nullptr)
				impl = new PredictorImpl;
			*impl = *p.impl;
		}

		return *this;
	}

	Predictor::~Predictor()
	{
		if (impl != nullptr) delete impl;
	}

	Template Predictor::declare_template(Shape shape)
	{
		assert(impl != nullptr);

		return impl->declare_template(shape);
	}

	DArray Predictor::declare_darray(Shape shape, uint32_t elem_size)
	{
		assert(impl != nullptr);

		return impl->declare_darray(shape, elem_size);
	}

	void Predictor::distribute(Template t, DRule rule)
	{
		assert(impl != nullptr);

		return impl->distribute(t, rule);
	}

	void Predictor::distribute(DArray a, DRule rule)
	{
		assert(impl != nullptr);

		return impl->distribute(a, rule);
	}

	void Predictor::redistribute(Template t, DRule rule)
	{
		assert(impl != nullptr);

		return impl->redistribute(t, rule);
	}

	void Predictor::redistribute(DArray a, DRule rule)
	{
		assert(impl != nullptr);

		return impl->redistribute(a, rule);
	}

	void Predictor::align_on(DArray a, Template t, ARule rule)
	{
		assert(impl != nullptr);

		return impl->align_on(a, t, rule);
	}

	void Predictor::align_on(DArray a, DArray b, ARule rule)
	{
		assert(impl != nullptr);

		return impl->align_on(a, b, rule);
	}

	void Predictor::realign_on(DArray a, Template t, ARule rule)
	{
		assert(impl != nullptr);

		return impl->realign_on(a, t, rule);
	}

	void Predictor::realign_on(DArray a, DArray b, ARule rule)
	{
		assert(impl != nullptr);

		return impl->realign_on(a, b, rule);
	}
}
