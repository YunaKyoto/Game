#include "raylib.h"
#include<iostream>

#include "Object.h"
#include "Path.h"
#include "Water.h"
#include "Hafen.h"
#include "House.h"

Object::Object() {
	this->object = object;
}

void Object::Draw(int x, int y) {

	DrawTexture(object, 32 * x, 32 * y, RAYWHITE);
}

