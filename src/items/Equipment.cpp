#include "raylib.h"
#include<iostream>

#include "Equipment.h"

Equipment::Equipment() {
	this->weight = weight;
	this->name = name;
	this->descr = descr;
	this->value = value;
	this->item = item;
	this->strength = strength;
}

int Equipment::getStrength() {
	return this->strength;
}