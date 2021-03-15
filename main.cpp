#include <iostream>

#include "Predictor.hpp"

static void example1(void)
{
	using namespace dvmpredictor;

	const uint32_t int_size = 4, double_size = 8;
	const uint64_t M = 1000000, N = 100000;

	// initialize a processors grid of 10 x 10 with 10 ms latency and 100 Mb bandwidth
	Predictor p({Range(0, 10), Range(0, 10)}, 0.01, 100 * 1000000);

	// declare an integers array of size M to N
	auto A = p.declare_darray({Range(0, M), Range(0, N)}, {ShadowEdge(), ShadowEdge()}, int_size);
	// declare a doubles array of size N to M
	auto B = p.declare_darray({Range(0, N), Range(0, M)}, {ShadowEdge(), ShadowEdge()}, double_size);

	DRule drule(2);
	for (DFormat &dformat: drule)
		dformat.block();

	// distribute A[block][block]
	p.distribute(A, drule);

	// align B on A via [i][j] on A[j][i]
	ARule arule = {AFormat(1, 0, 1), AFormat(1, 0, 0)};
	p.align_with(B, A, arule);

	drule[1] = DFormat();
	drule[1].none();

	std::cout<<"Before redistribute"<<std::endl;
	for (uint32_t node = 0; node < p.rank(); node++) {
		std::cout<<"\ttime at node "<<node<<": "<<p.wtime(node)<<std::endl;
	}

	// redistribute A [block][]
	p.redistribute(A, drule);

	std::cout<<"After redistribute"<<std::endl;
	for (uint32_t node = 0; node < p.rank(); node++) {
		std::cout<<"\ttime at node "<<node<<": "<<p.wtime(node)<<std::endl;
	}
}

int main(int argc, const char *argv[])
{
	example1();

	std::cout<<"Examples finished"<<std::endl;

	return 0;
}
