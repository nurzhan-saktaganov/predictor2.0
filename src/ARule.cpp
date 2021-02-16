#include "ARule.hpp"

#include "expect.hpp"

namespace dvmpredictor {

	void combine_arule(const ARule &r1, const ARule &r2, ARule &out)
	{
		std::vector<bool> seen_axis(r1.size());

		expect(r1.size() > 0);
		expect(r2.size() > 0);

		out.resize(r2.size());

		for (uint32_t dim2 = 0; dim2 < r2.size(); dim2++) {
			AFormat e2 = r2[dim2];

			expect(e2.defined());

			// See doc about align superposision, case a.
			// e2 has the format '*' or is some constant 'C'.
			if (e2.is_asterisk() || e2.is_constant()) {
				AFormat e3 = e2;
				out[dim2] = e3;
				continue;
			}

			// See doc about align superposision, case b.
			// e2 has the next format: a * I + b

			uint32_t dim1 = e2.dimension();
			expect(dim1 < r1.size());

			expect(seen_axis[dim1] == false);
			seen_axis[dim1] = true;

			AFormat e1 = r1[dim1];

			expect(e1.defined());

			// See case b.1 in the doc
			if (e1.is_asterisk()) {
				AFormat e3 = e1;
				out[dim2] = e3;
				continue;
			}

			if (e1.is_constant()) {
				int32_t constant = e2.a() * e1.b() + e2.b();
				AFormat e3(constant);
				out[dim2] = e3;
				continue;
			}

			// See case b.2 in the doc
			int32_t a = e2.a() * e1.a();
			int32_t b = e2.a() * e1.b() + e2.b();

			AFormat e3 = AFormat(a, b, dim1);

			out[dim2] = e3;
		}
	}
}
