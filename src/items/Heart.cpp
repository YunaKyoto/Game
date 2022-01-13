#include "raylib.h"
#include<iostream>

#include "Heart.h"

Heart::Heart() {
	this->weight = 2;
	this->name = "Heart";
	this->descr = "You'll get more life!";
	this->value = 100;
	this->item = LoadTexture("assets/heart.png");
}