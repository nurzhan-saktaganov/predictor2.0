#pragma once

#include <cstdint>

#include "PGrid.hpp"
#include "Shape.hpp"
#include "Template.hpp"
#include "DArray.hpp"
#include "DRule.hpp"
#include "ARule.hpp"
#include "Shadow.hpp"

namespace dvmpredictor {
	class PredictorImpl {
	public:
		PredictorImpl();
		PredictorImpl(Shape pgrid, double latency, double bandwidth);

		Template declare_template(Shape shape);
		DArray declare_darray(Shape shape, Shadow shadow, uint32_t elem_size);

		void distribute(Template t, DRule rule);
		void distribute(DArray a, DRule rule);

		void redistribute(Template t, DRule rule);
		void redistribute(DArray a, DRule rule);

		void align_on(DArray a, Template t, ARule rule);
		void align_on(DArray a, DArray b, ARule rule);

		void realign_on(DArray a, Template t, ARule rule);
		void realign_on(DArray a, DArray b, ARule rule);

	private:
		PGrid _pgrid;
	};
}
