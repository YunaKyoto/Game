#include "raylib.h"
#include<iostream>

#include "PlayerChar.h"

#include "Inventory.h"
#include "Inventory.cpp"

#include "items/ItemBase.h"
#include "items/Food.h"
#include "items/Heart.h"
#include "items/Help.h"
#include "items/Key.h"
#include "items/Sword.h"
#include "items/Hat.h"

//Aufgabe 2b)

PlayerChar::PlayerChar() {
	this->player = player;
	this->playerTexture = playerTexture;
	this->inventory = inventory;
	this->strength = strength;
}

PlayerChar::PlayerChar(int x, int y) {
	this->player = { (float) x, (float) y };
	this->playerTexture = LoadTexture("assets/player.png");
	this->inventory = Inventory<ItemBase>(10);
	this->strength = 15;
}

void PlayerChar::MoveLeft() {
	this->player.x -= 1;
}

void PlayerChar::MoveRight() {
	this->player.x += 1;
}

void PlayerChar::MoveUp() {
	this->player.y -= 1;
}

void PlayerChar::MoveDown() {
	this->player.y += 1;
}

void PlayerChar::Draw() {
	DrawTexture(playerTexture, player.x * 32, player.y * 32, RAYWHITE);
}

Vector2 PlayerChar::getPlayerPos() {
	return this->player;
}

int PlayerChar::weight() {
	unsigned int weight = 0;
	for (int i = 0; i < inventory.GetSlots(); i++) {
		if (inventory.getItem(i).getName().compare("") != 0) {
			weight += inventory.getItem(i).getWeight();
		}
	}
	return weight;
}

int PlayerChar::getStrength() {
	return this->strength;
}

bool PlayerChar::pickUp(ItemBase item) {
	bool stillPlace = false;
	for (int i = 0; i < inventory.GetSlots(); i++) {
		if (!stillPlace) {
			if (inventory.getItem(i).getName().compare("") == 0 && item.getVisible()) {

				inventory.setItem(item, i);
				stillPlace = true;

			}
		}
	}
	return stillPlace;
}

void PlayerChar::setSlot(Equipment equi, int slot) {
	if (inventory.getCategory(slot).getName().compare("") == 0) {
		inventory.setCategory(slot, equi);
		strength += equi.getStrength();
	}
}

void PlayerChar::Draw(bool b, ItemBase it[], Equipment equ[]) {
	if (b) {
		DrawRectangle(125, 165, 230, 190, WHITE);
		DrawText("Inventory", 185, 170, 23, BLACK);
		for (int i = 0; i < 10; i++) {
			if (it[i].getVisible()) {
				int j = i / 4;
				int k = i % 4;
				it[i].Draw(4 + (k * 2), 6 + (j * 2));
			}
		}
		
		DrawRectangle(125, 380, 45, 45, WHITE);
		DrawRectangle(218, 380, 45, 45, WHITE);
		DrawRectangle(310, 380, 45, 45, WHITE);

		for (int i = 0; i < 3; i++) {
			if (equ[i].getVisible()) {
				equ[i].Draw(4 + (i * 3), 12);
			}
		}
	}
}

void PlayerChar::setToInventory(Equipment equip) {
	if (equip.getName().compare("Hat") == 0) {
		inventory.setCategory(0, equip);
	}
	else if (equip.getName().compare("Help") == 0) {
		inventory.setCategory(1, equip);
	}
	else if (equip.getName().compare("Sword") == 0) {
		inventory.setCategory(2, equip);
	}
}

Inventory<ItemBase> PlayerChar::getItemBase() {
	return this->inventory;
}

void PlayerChar::deleteItem(int slot) {
	inventory.deleteItem(slot);
}

void PlayerChar::setItem(ItemBase item, int slot) {
	inventory.setItem(item, slot);
}

void PlayerChar::deleteSlot(int slot) {
	inventory.deleteCategory(slot);
}