#include <cassert>
#include <stack>

#include "MPIImpl.hpp"

namespace mpisimulator {
	MPIImpl::MPIImpl() :
		_size(0), _latency(0), _bandwidth(0)
	{}

	MPIImpl::MPIImpl(uint32_t size, double latency, double bandwidth) :
		_size(size), _latency(latency), _bandwidth(bandwidth), _nodes(size)
	{
		assert(size > 0);
		assert(latency > 0);
		assert(bandwidth > 0);
	}

	void MPIImpl::send(uint32_t at, uint32_t to, uint64_t bytes)
	{
		assert(at < _size);

		struct operation op = {};

		op.type = OperationType::send;
		op.at = at;
		op.send.to = to;
		op.send.bytes = bytes;

		OperationQueue &ops = _nodes[at].ops;
		ops.push(op);
	}

	void MPIImpl::recv(uint32_t at, uint32_t from, uint64_t bytes)
	{
		assert(at < _size);

		struct operation op = {};

		op.type = OperationType::recv;
		op.at = at;
		op.recv.from = from;
		op.recv.bytes = bytes;

		OperationQueue &ops = _nodes[at].ops;
		ops.push(op);
	}

	void MPIImpl::skip(uint32_t at, double time)
	{
		assert(at < _size);
		assert(time > 0);

		struct operation op = {};

		op.type = OperationType::skip;
		op.at = at;
		op.skip.time = time;

		OperationQueue &ops = _nodes[at].ops;
		ops.push(op);
	}

	double MPIImpl::wtime(uint32_t at)
	{
		assert(at < _size);

		std::vector<bool> waiting(_size);
		std::stack<struct operation> stack, stack_helper;

		OperationQueue &ops = _nodes[at].ops;
		while (!ops.empty()) {
			struct operation op = ops.front();
			ops.pop();
			stack_helper.push(op);
		}

		while (!stack_helper.empty()) {
			stack.push(stack_helper.top());
			stack_helper.pop();
		}

		while (!stack.empty()) {
			struct operation op1 = stack.top();
			uint32_t at = op1.at;

			assert(op1.type != OperationType::undefined);

			if (op1.type == OperationType::skip) {
				stack.pop();
				_nodes[at].local_time += op1.skip.time;
				continue;
			}

			// To detect deadlocks
			waiting[at] = true;

			uint32_t with = (op1.type == OperationType::send) ? op1.send.to : op1.recv.from;

			assert(with < _size);

			// To detect deadlocks
			assert(waiting[with] == false);

			OperationQueue &ops = _nodes[with].ops;

			// To detect an incorrect usage
			assert(!ops.empty());

			struct operation op2 = ops.front();
			ops.pop();

			assert(op2.type != OperationType::undefined);

			// match two operations
			bool match;

			if (op1.type == OperationType::send) {
				match = op2.type == OperationType::recv &&
						op1.at == op2.recv.from;
			} else {
				match = op2.type == OperationType::send &&
						op1.at == op2.send.to;
			}

			if (!match) {
				stack.push(op2);
				continue;
			}

			uint64_t bytes;

			if (op1.type == OperationType::send) {
				assert(op1.send.bytes == op2.recv.bytes);
				bytes = op1.send.bytes;
			} else {
				assert(op1.recv.bytes == op2.send.bytes);
				bytes = op1.recv.bytes;
			}

			double now = std::max(_nodes[at].local_time, _nodes[with].local_time);

			now += _latency + bytes / _bandwidth;

			_nodes[at].local_time = now;
			_nodes[with].local_time = now;

			waiting[at] = false;

			stack.pop();
		}

		return _nodes[at].local_time;
	}

	void MPIImpl::compact()
	{
		for (uint32_t node = 0; node < _size; node++)
			wtime(node);
	}

	void MPIImpl::barrier()
	{
		// TODO not implemented yet
	}
}
