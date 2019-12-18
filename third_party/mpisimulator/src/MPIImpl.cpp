#include "expect.hpp"
#include "MPIImpl.hpp"

namespace mpisimulator {
	MPIImpl::MPIImpl() :
		_size(0), _latency(0), _bandwidth(0)
	{}

	MPIImpl::MPIImpl(uint32_t size, double latency, double bandwidth) :
		_size(size), _latency(latency), _bandwidth(bandwidth), _nodes(size)
	{
		expect(size > 0);
		expect(latency > 0);
		expect(bandwidth > 0);
	}

	void MPIImpl::send(uint32_t at, uint32_t to, uint64_t bytes)
	{
		expect(at < _size);

		struct operation op = {};

		op.type = OperationType::send;
		op.at = at;
		op.send.to = to;
		op.send.bytes = bytes;

		_put(at, op);
		_execute(at);
	}

	void MPIImpl::recv(uint32_t at, uint32_t from, uint64_t bytes)
	{
		expect(at < _size);

		struct operation op = {};

		op.type = OperationType::recv;
		op.at = at;
		op.recv.from = from;
		op.recv.bytes = bytes;

		_put(at, op);
		_execute(at);
	}

	void MPIImpl::skip(uint32_t at, double time)
	{
		expect(at < _size);
		expect(time > 0);

		struct operation op = {};

		op.type = OperationType::skip;
		op.at = at;
		op.skip.time = time;

		_put(at, op);
		_execute(at);
	}

	double MPIImpl::wtime(uint32_t at)
	{
		expect(at < _size);

		OperationQueue &ops = _nodes[at].ops;

		expect(ops.empty());

		return _nodes[at].local_time;
	}

	void MPIImpl::_execute(uint32_t at)
	{
		while (true) {
			struct node &node = _nodes[at];

			if (node.waiting || node.ops.empty())
				return;

			OperationQueue &stack = node.ops;
			struct operation op = stack.front();

			must(op.at == at);

			switch (op.type) {
			case OperationType::skip: {
				node.local_time += op.skip.time;
				break;
			}
			case OperationType::send:
				// fallthrough
			case OperationType::recv: {
				uint32_t partner_id;

				if (op.type == OperationType::send)
					partner_id = op.send.to;
				else
					partner_id = op.recv.from;

				must(partner_id < _size);

				struct node &partner = _nodes[partner_id];
				OperationQueue &ops = partner.ops;

				if (ops.empty()) {
					node.waiting = true;
					return;
				}

				struct operation op2 = ops.front();

				must(op2.at == partner_id);

				bool match;

				if (op.type == OperationType::send) {
					match = op2.type == OperationType::recv &&
							op2.recv.from == op.at;
				} else {
					match = op2.type == OperationType::send &&
							op2.send.to == op.at;
				}

				if (!match) {
					node.waiting = true;
					at = op2.at;
					continue;
				}

				ops.pop_front();

				uint64_t bytes;

				if (op.type == OperationType::send) {
					expect(op.send.bytes == op2.recv.bytes);
					bytes = op.send.bytes;
				} else {
					expect(op.recv.bytes == op2.send.bytes);
					bytes = op.recv.bytes;
				}

				double now = std::max(node.local_time, partner.local_time);

				now += _latency + bytes / _bandwidth;

				node.local_time = partner.local_time = now;

				if (partner.waiting) {
					partner.waiting = false;
					at = op2.at;
				}

				break;
			}
			default:
				abort();
			}

			stack.pop_front();
		}
	}

	void MPIImpl::_put(uint32_t at, struct operation op)
	{
		expect(at < _size);

		_nodes[at].ops.push_back(op);
	}

	void MPIImpl::barrier(uint32_t at)
	{
		const uint64_t barrier_package_size = 1;

		expect(at < _size);

		// Note: This is a tree-based barrier is copied from
		// https://github.com/nurzhan-saktaganov/cmc_master/blob/master/parallel_prac/2017autumn/task0/barrier.c

		uint32_t divider = 1;

		// Phase 1. Gather from all into process with rank = 0
		while (divider < _size) {
			divider *= 2;

			bool i_am_sender = at % divider != 0;

			if (i_am_sender) {
				uint32_t send_to = at - divider / 2;
				send(at, send_to, barrier_package_size);
				break;
			}

			uint32_t receive_from = at + divider / 2;
			if (receive_from < _size)
				recv(at, receive_from, barrier_package_size);
		}

		// Phase 2. Propogate back
		while (divider > 1) {
			bool i_am_receiver = at % divider != 0;

			if (i_am_receiver) {
				uint32_t receive_from = at - divider / 2;
				recv(at, receive_from, barrier_package_size);
			} else {
				uint32_t send_to = at + divider / 2;
				if (send_to < _size)
					send(at, send_to, barrier_package_size);
			}

			divider /= 2;
		}
	}
}
