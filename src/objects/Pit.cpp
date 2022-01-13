#include "raylib.h"
#include<iostream>

#include "Pit.h"

Pit::Pit() {
	this->object = LoadTexture("assets/pit.png");
	this->check = "pit";
	this->blocked = false;
}
