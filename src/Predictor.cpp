#include "expect.hpp"
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

	uint32_t Predictor::rank() const
	{
		expect(impl != nullptr);

		return impl->rank();
	}

	double Predictor::wtime(uint32_t node) const
	{
		expect(impl != nullptr);

		return impl->wtime(node);
	}

	Template Predictor::declare_template(Shape shape)
	{
		expect(impl != nullptr);

		return impl->declare_template(shape);
	}

	DArray Predictor::declare_darray(Shape shape, Shadow shadow, uint32_t elem_size)
	{
		expect(impl != nullptr);

		return impl->declare_darray(shape, shadow, elem_size);
	}

	void Predictor::distribute(Template t, DRule rule)
	{
		expect(impl != nullptr);

		return impl->distribute(t, rule);
	}

	void Predictor::distribute(DArray a, DRule rule)
	{
		expect(impl != nullptr);

		return impl->distribute(a, rule);
	}

	void Predictor::redistribute(Template t, DRule rule)
	{
		expect(impl != nullptr);

		return impl->redistribute(t, rule);
	}

	void Predictor::redistribute(DArray a, DRule rule)
	{
		expect(impl != nullptr);

		return impl->redistribute(a, rule);
	}

	void Predictor::align_with(DArray a, Template t, ARule rule)
	{
		expect(impl != nullptr);

		return impl->align_with(a, t, rule);
	}

	void Predictor::align_with(DArray a, DArray b, ARule rule)
	{
		expect(impl != nullptr);

		return impl->align_with(a, b, rule);
	}

	void Predictor::realign_with(DArray a, Template t, ARule rule)
	{
		expect(impl != nullptr);

		return impl->realign_with(a, t, rule);
	}

	void Predictor::realign_with(DArray a, DArray b, ARule rule)
	{
		expect(impl != nullptr);

		return impl->realign_with(a, b, rule);
	}
}
