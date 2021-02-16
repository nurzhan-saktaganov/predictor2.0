#pragma once

#include "Shape.hpp"

namespace dvmpredictor {
	// Массив размещений шаблона или массива по узлам.
	// Индекс массива -- это идентификатор узла.
	using Disposition = std::vector<Shape>;
}
