#pragma once

#include <cstdlib>
#include <iostream>
#include "raylib.h"

#include "ItemBase.h"

struct Equipment : public ItemBase {
protected:
	int strength;
	//bool visible = true;
public:
	Equipment();
	int getStrength();
};