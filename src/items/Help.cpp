#include "raylib.h"
#include<iostream>

#include "Help.h"

Help::Help() {
	this->weight = 1;
	this->name = "Help";
	this->descr = "You'll see the key!";
	this->value = 50;
	this->item = LoadTexture("assets/help.png");
	this->strength = 5;
}