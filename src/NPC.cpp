#include "raylib.h"
#include<iostream>

#include "NPC.h"

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

NPC::NPC() {
	this->player = player;
	this->playerTexture = playerTexture;
	this->inventory = inventory;
	this->strength = strength;
}

NPC::NPC(int x, int y) {
	this->player = { (float)x, (float)y };
	this->playerTexture = LoadTexture("assets/npc.png");
	this->inventory = Inventory<ItemBase>(10);
	this->strength = 15;
}

void NPC::MoveLeft() {
	this->player.x -= 1;
}

void NPC::MoveRight() {
	this->player.x += 1;
}

void NPC::MoveUp() {
	this->player.y -= 1;
}

void NPC::MoveDown() {
	this->player.y += 1;
}

Vector2 NPC::getPlayerPos() {
	return this->player;
}

void NPC::Draw() {
	DrawTexture(playerTexture, player.x * 32, player.y * 32, RAYWHITE);
}

Inventory<ItemBase> NPC::getItemBase() {
	return this->inventory;
}

bool NPC::pickUp(ItemBase item) {
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

int NPC::weight() {
	unsigned int weight = 0;
	for (int i = 0; i < inventory.GetSlots(); i++) {
		if (inventory.getItem(i).getName().compare("") != 0) {
			weight += inventory.getItem(i).getWeight();
		}
	}
	return weight;
}

int NPC::getStrength() {
	return this->strength;
}
