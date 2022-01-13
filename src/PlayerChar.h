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

class PlayerChar : public Char {
private:
	Vector2 player;
	Texture2D playerTexture;
	Inventory<ItemBase> inventory;
	unsigned int strength;
	

public:
	PlayerChar();
	PlayerChar(int x, int y);
	void MoveLeft() override;
	void MoveRight() override;
	void MoveUp() override;
	void MoveDown() override;
	Vector2 getPlayerPos();
	void Draw();
	void Draw(bool b, ItemBase it[], Equipment equ[]);

	Inventory<ItemBase> getItemBase();

	void deleteItem(int slot);

	int weight();
	int getStrength();

	bool pickUp(ItemBase item);
	void setSlot(Equipment equi, int slot);

	void setToInventory(Equipment equip);


	void deleteSlot(int slot);

	void setItem(ItemBase item, int slot);
};