#include "raylib.h"
#include<iostream>

#include "Hat.h"

Hat::Hat() {
	this->weight = 3;
	this->name = "Hat";
	this->descr = "You're amazing now!";
	this->value = 60;
	this->item = LoadTexture("assets/hat.png");
	this->strength = 3;
}