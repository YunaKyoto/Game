#include "Inventory.h"
#include <vector>
#include <iostream>

//Aufgabe 2a)

template <typename T>
Inventory<T>::Inventory(int slot)
{
    this->slots = slot;
    this->itemBag.resize(slot);
}

template <typename T>
T Inventory<T>::getItem(int itemSlot)
{
    return this->itemBag[itemSlot];
}

template <typename T>
bool Inventory<T>::setItem(T item, int itemSlot)
{
    this->itemBag[itemSlot] = item;
    return true;
}

template <typename T>
void Inventory<T>::deleteItem(int itemSlot) {
    for (int i = 0; i < GetSlots(); i++) {
        T it;
        if (i < itemSlot) {
            it = getItem(i);
        }
        else if(i >= itemSlot && i < (GetSlots() - 1)){
            it = getItem(i+1);
        }
        this->itemBag[i] = it;
    }
}

template <typename T>
T Inventory<T>::getCategory(int slot) {
    return this->category[slot];
}

template <typename T>
bool Inventory<T>::setCategory(int slot, T item) {
    this->category[slot] = item;
    return true;
}

template <typename T>
void Inventory<T>::deleteCategory(int slot) {
    //category[slot] = ;
}

template <typename T>
int Inventory<T>::GetSlots() {
    return this->slots;
}