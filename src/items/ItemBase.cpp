#include "raylib.h"
#include <sstream>
#include <string.h>
#include<iostream>

#include "ItemBase.h"

//Aufgabe 1c)

ItemBase::ItemBase() {
	this->weight = weight;
	this->name = name;
	this->descr = descr;
	this->value = value;
	this->item = item;
	this->visible = visible;
}

void ItemBase::Draw(int x, int y) {
	if (visible) {
		DrawTexture(item, 32 * x, 32 * y, RAYWHITE);
	}
}

unsigned int ItemBase::getWeight() {
	return this->weight;
}

bool ItemBase::getVisible() {
	return this->visible;
}

void ItemBase::setVisible(bool b) {
	this->visible = b;
}

std::string ItemBase::getName() {
	return this->name;
}

std::string ItemBase::getDescr() {
	return this->descr;
}

unsigned int ItemBase::getValue() {
	return this->value;
}

void ItemBase::Draw() {
	DrawRectangle(125, 10, 230, 150, GRAY);
	const char* cstr;

	cstr = name.c_str();
	DrawText("Name: ", 130, 10, 20, BLACK);
	DrawText(cstr, 210, 10, 20, BLACK);
	
	cstr = descr.c_str();
	DrawText("Description: ", 130, 40, 20, BLACK);
	DrawText(cstr, 130, 70, 20, BLACK);

	std::string wert = std::to_string(weight);
	cstr = wert.c_str(); 
	DrawText("Weight: ", 130, 100, 20, BLACK);
	DrawText(cstr, 210, 100, 20, BLACK);

	wert = std::to_string(value);
	cstr = wert.c_str();
	DrawText("Value: " , 130, 130, 20, BLACK);
	DrawText(cstr, 210, 130, 20, BLACK);
}