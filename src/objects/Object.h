#pragma once

#include <cstdlib>
#include <iostream>
#include "raylib.h"

struct Object {
protected:
	Texture2D object;

public:
	Object();
	std::string check;
	void Draw(int x, int y);
	bool blocked = true;
};