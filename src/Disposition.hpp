#pragma once

#include "Node.hpp"
#include "SubShape.hpp"

namespace dvmpredictor {
	// Массив размещений шаблона или массива по узлам.
	// Индекс массива -- это идентификатор узла.
	using Disposition = std::vector<Shape>;
}
