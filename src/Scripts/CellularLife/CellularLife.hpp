#pragma once

class LayersManager;

class CellularLife {
public:
	CellularLife() = default;
	CellularLife(LayersManager& layersM);
	~CellularLife() = default;

	void loopIteration(float dt);
};