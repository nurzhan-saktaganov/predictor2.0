#pragma once

#include <cstdint>

#include "Shape.hpp"
#include "Template.hpp"
#include "DArray.hpp"
#include "DRule.hpp"
#include "ARule.hpp"
#include "Shadow.hpp"

namespace dvmpredictor {
	class PredictorImpl;

	class Predictor {
	public:
		Predictor();
		Predictor(Shape pgrid, double latency, double bandwidth);
		Predictor(const Predictor &p);
		Predictor& operator=(const Predictor &p);
		~Predictor();

		Template declare_template(Shape shape);
		// elem size -- size of each element of array
		DArray declare_darray(Shape shape, Shadow shadow, uint32_t elem_size);

		void distribute(Template t, DRule rule);
		void distribute(DArray a, DRule rule);

		// TODO: perform nested redistribution
		void redistribute(Template t, DRule rule);
		void redistribute(DArray a, DRule rule);

		void align_on(DArray a, Template t, ARule rule);
		void align_on(DArray a, DArray b, ARule rule);

		void realign_on(DArray a, Template t, ARule rule);
		void realign_on(DArray a, DArray b, ARule rule);
	private:
		PredictorImpl *impl;
	};
};
