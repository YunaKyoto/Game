#include "raylib.h"
#include<iostream>

#include "House.h"

House::House() {
	this->object = LoadTexture("assets/house.png");
	this->check = "house";
}

