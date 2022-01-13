#include "raylib.h"
#include<iostream>

#include "Sword.h"

Sword::Sword() {
	this->weight = 10;
	this->name = "Sword";
	this->descr = "You'll be strong now!";
	this->value = 70;
	this->item = LoadTexture("assets/sword.png");
	this->strength = 10;

}