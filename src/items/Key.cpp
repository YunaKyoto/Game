#include "raylib.h"
#include<iostream>

#include "Key.h"

Key::Key() {
	this->weight = 8;
	this->name = "Key";
	this->descr = "Got to the port!";
	this->value = 1000;
	this->item = LoadTexture("assets/key.png");
}
