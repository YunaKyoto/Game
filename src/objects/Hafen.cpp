#include "raylib.h"
#include<iostream>

#include "Hafen.h"

Hafen::Hafen() {
	this->object = LoadTexture("assets/hafen.png");
	this->check = "hafen";
}