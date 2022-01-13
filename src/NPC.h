#pragma once

#include <cstdlib>
#include <iostream>
#include "raylib.h"

#include "Char.h"
#include "Inventory.h"
#include "items/ItemBase.h"

#include "items/Food.h"
#include "items/Heart.h"
#include "items/Help.h"
#include "items/Key.h"
#include "items/Sword.h"
#include "items/Hat.h"

//Aufgabe 2b)

class NPC : public Char {
private:
	Vector2 player;
	Texture2D playerTexture;
	Inventory<ItemBase> inventory;
	unsigned int strength;

public:
	NPC();
	NPC(int x, int y);

	void MoveLeft() override;
	void MoveRight() override;
	void MoveUp() override;
	void MoveDown() override;

	Vector2 getPlayerPos();
	void Draw();
	Inventory<ItemBase> getItemBase();
	bool pickUp(ItemBase item);
	int weight();
	int getStrength();
};