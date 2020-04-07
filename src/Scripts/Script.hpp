#pragma once

class Script {
public:
	Script() = default;
	virtual ~Script() = default;
	virtual void loopIteration(float dt) = 0;
};