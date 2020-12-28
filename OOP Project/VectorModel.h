#pragma once

#include "Components.h"
#include <iostream>
#include <vector>

class VectorModel{
private:
	std::vector<std::pair<float, float>> cordinateVector;

	VectorModel(TranformComponent& _transform ,const std::vector<std::pair<float, float>> _cordinateVector);
};

