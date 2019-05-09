#pragma once

#include <cstdint>

#include "Shape.hpp"
#include "Template.hpp"
#include "DArray.hpp"
#include "DRule.hpp"

namespace dvmpredictor {
	using namespace std;

	class Predictor {
	public:
		Predictor();

		// void init(PGrid pgrid);
		Template declare_template(Shape shape);
		// elem size -- size of each element of array
		DArray declare_darray(Shape shape, uint32_t elem_size);

		void distribute(Template t, DRule rule);
		void distribute(DArray a, DRule rule);

		void redistribute(Template t, DRule rule);
		void redistribute(DArray a, DRule rule);

		// TODO add align rule
		void align_on(DArray a, Template t);
		void align_on(DArray a, DArray b);

		// TODO add align rule
		void realign_on(DArray a, Template t);
		void realign_on(DArray a, DArray b);
		

	private:
	};
}
