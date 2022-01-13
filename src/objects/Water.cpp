#include "raylib.h"
#include <iostream>

#include "Water.h"

Water::Water() {
	this->object = LoadTexture("assets/water.png");
	this->check = "water";
	this->blocked = false;
	
}
