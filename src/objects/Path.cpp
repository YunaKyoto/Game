#include "raylib.h"
#include<iostream>

#include "Path.h"

Path::Path() {
	this->object = LoadTexture("assets/path.png");
	this->check = "path";
}
