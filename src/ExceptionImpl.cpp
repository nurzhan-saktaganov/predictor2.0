#include "ExceptionImpl.hpp"

namespace dvmpredictor {
	MisusageImpl::MisusageImpl()
		: MisusageImpl(nullptr)
	{}

	MisusageImpl::MisusageImpl(const char *what)
		: _what(what)
	{}

	const char *MisusageImpl::what() const noexcept
	{
		return _what;
	}
}
