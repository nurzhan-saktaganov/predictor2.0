#pragma once

#include <vector>

#include "Disposition.hpp"

namespace dvmpredictor {
	// Массив размещений шаблонов или массиов по узлам.
	// Индекс массива -- это идентификатор размещаемого шаблона или массива.
	using Dispositions = std::vector<Disposition>;
}
