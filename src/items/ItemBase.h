#pragma once

#include <cstdlib>
#include <iostream>
#include "raylib.h"

//Aufgabe 1c)

struct ItemBase {
protected:
	unsigned int weight;
	std::string name;
	std::string descr;
	unsigned int value;
	Texture2D item;
	bool visible = true;

public:
	ItemBase();
	void Draw(int x, int y);
	unsigned int getWeight();
	bool getVisible();
	void setVisible(bool b);
	std::string getName();
	std::string getDescr();
	unsigned int getValue();
	void Draw();
};