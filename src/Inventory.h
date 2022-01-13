#pragma once

#include <vector>

//Aufgabe 2a)

template <typename T>

class Inventory {
private:
	std::vector<T> itemBag;
	int slots;
	T category[3];

public:
	Inventory() {};
	Inventory(int slot);
	~Inventory() {};

	T getItem(int itemSlot);
	bool setItem(T item, int itemSlot);
	void deleteItem(int itemSlot);

	T getCategory(int slot);
	bool setCategory(int slot, T item);
	void deleteCategory(int slot);
	int GetSlots();
};