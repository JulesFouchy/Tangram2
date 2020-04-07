#include "CellularLife.hpp"

#include "Systems/LayersManager.hpp"

CellularLife::CellularLife(LayersManager& layersM) {
	layersM.createFragmentLayer("res/shaders/testCellDistortion.frag");
}

void CellularLife::loopIteration(float dt) {

}