#include "VectorModel.h"

using namespace std;

VectorModel::VectorModel(TranformComponent& _transform , const vector<pair<float, float>> _cordinateVector){
	cordinateVector = _cordinateVector;
}

