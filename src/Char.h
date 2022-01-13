#pragma once

#include <cstdlib>
#include <iostream>
#include "raylib.h"

//Aufgabe 2b)

class Char {
private:

public:
	virtual void MoveLeft() = 0;
	virtual void MoveRight() = 0;
	virtual void MoveUp() = 0;
	virtual void MoveDown() = 0;
};