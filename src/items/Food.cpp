#include "raylib.h"
#include<iostream>

#include "Food.h"

Food::Food() {
	this->weight = 5;
	this->name = "Food";
	this->descr = "Makes you full.";
	this->value = 30;
	this->item = LoadTexture("assets/food.png");
}