#include "raylib.h"

//Um int werte in string umzuwandeln und diese mit DrawText schreiben zu lassen.
#include <sstream>

#include "objects/Path.h"
#include "objects/Water.h"
#include "objects/House.h"
#include "objects/Hafen.h"
#include "objects/Pit.h"

#include "items/Key.h"
#include "items/Sword.h"
#include "items/Food.h"
#include "items/Heart.h"
#include "items/Help.h"
#include "items/Hat.h"

#include "GameScreen.h"
#include <list>



void GameScreen::Init() {
	// Aufgabe 4)
	for (int i = 0; i < 15; i++) {
		for (int e = 0; e < 15; e++) {
			nodes[i][e] = new sNode;
			nodes[i][e]->x = i * 32;
			nodes[i][e]->y = e * 32;
			nodes[i][e]->bObstacle = false;
			nodes[i][e]->parent = nullptr;
			nodes[i][e]->bVisited = false;
		}
	}

	for (int x = 0; x < 15; x++) {
		for (int y = 0; y < 15; y++) {
			if (y > 0) {
				nodes[x][y]->vecNeighbours.push_back(nodes[x][y - 1]);
			}
			if (y < 15 - 1) {
				nodes[x][y]->vecNeighbours.push_back(nodes[x][y + 1]);
			}
			if (x > 0) {
				nodes[x][y]->vecNeighbours.push_back(nodes[x - 1][y]);
			}
			if (x < 15 - 1) {
				nodes[x][y]->vecNeighbours.push_back(nodes[x + 1][y]);
			}
		}
	}

	path = Path();
	house = House();
	hafen = Hafen();
	water = Water();
	pit = Pit();

	key = Key();
	sword = Sword();
	food = Food();
	heart = Heart();
	help = Help();
	hat = Hat();

	hatEqu = Hat();
	swordEqu = Sword();
	helpEqu = Help();

	//Aufgabe 1b)
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			map[i][j] = path;
		}
	}

	//generate a random value for the start and exit position 
	srand((unsigned)time(0));
	int rand1 = ((rand() % 14) + 1);
	int rand2 = ((rand() % 14) + 1);

	map[rand1][0] = house;
	nodeStart = nodes[rand1][0];
	map[rand2][14] = hafen;
	nodeEnd = nodes[rand2][14];

	SetMap();
	//Aufgabe 1b) ende

	//Aufgabe 1c)
	int pitRand = ((rand() % 7) + 5);
	for (int i = 0; i < pitRand; i++) {
		PutPit(pit);
	}

	//Items werden gesetzt
	PutItem(key);
	for (int i = 0; i < 5; i++) {
		int itemRand = ((rand() % 4) + 2);
		for (int j = 0; j < itemRand; j++) {
			switch (i) {
			case 0:
				PutItem(sword);
				break;
			case 1:
				PutItem(food);
				break;
			case 2:
				PutItem(heart);
				break;
			case 3:
				PutItem(hat);
				break;
			case 4:
				PutItem(help);
				break;
			default:
				break;
			}
		}
	}

	//Schlüssel ist zu beginn unsichtbar
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			if (item[i][j].getName().compare("Key") == 0) {
				item[i][j].setVisible(false);
			}
		}
	}
	//Aufgabe 1c) ende

	//Player wird generiert
	player = PlayerChar(rand1, 0);
	npc = NPC(rand1, 0);

}

//Aufgabe 1b)
void GameScreen::SetMap() {
	for (int i = 0; i < 13; i++) {
		int r2 = ((rand() % 4) + 2);
		for (int j = 0; j < r2; j++) {
			int r1 = rand() % 15;
			if (map[r1][i].check == "water" || map[r1][i].check == "house" || map[r1][i].check == "hafen") {
				j -= 1;
			}
			else {
				map[r1][i] = water;
				nodes[r1][i]->bObstacle = true;
			}
		}
	}

	//Aufgabe 4
	pathfinding();

	sNode* tempNode = nodeEnd;
	while (tempNode->parent != nullptr)
	{

		npcMap[tempNode->x / 32][tempNode->y / 32] = true;
		tempNode = tempNode->parent;
		if (tempNode == nullptr) {
			SetMap();
		}
	}
	//


	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			if (!npcMap[i][j]) {
				npcMap[i][j] = false;
			}
		}
	}
}
//Aufgabe 1b) ende


//Aufgabe 1c)
void GameScreen::PutItem(ItemBase iB) {
	int randItemX = rand() % 15;
	int randItemY = rand() % 15;
	if (map[randItemX][randItemY].blocked && map[randItemX][randItemY].check != "house" && map[randItemX][randItemY].check != "hafen" && item[randItemX][randItemY].getName().compare("") == 0) {
		item[randItemX][randItemY] = iB;
	}
	else {
		PutItem(iB);
	}
}

void GameScreen::PutPit(Object pit) {
	int randItemX = rand() % 15;
	int randItemY = rand() % 15;
	if (!map[randItemX][randItemY].blocked) {
		map[randItemX][randItemY] = pit;
	}
	else {
		PutPit(pit);
	}
}
//Aufgabe 1c) ende

//Aufgabe 4
void GameScreen::pathfinding() {
	// Reset Navigation Graph - default all node states
	for (int i = 0; i < 15; i++)
	{
		for (int e = 0; e < 15; e++)
		{
			nodes[i][e]->bVisited = false;
			nodes[i][e]->fGlobalGoal = INFINITY;
			nodes[i][e]->fLocalGoal = INFINITY;
			nodes[i][e]->parent = nullptr;
		}
	}
	auto distance = [](sNode* a, sNode* b) // For convenience
	{
		return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
	};

	auto heuristic = [distance](sNode* a, sNode* b) // So we can experiment with heuristic
	{
		return distance(a, b);
	};

	// Setup starting conditions
	sNode* nodeCurrent = nodeStart;
	nodeStart->fLocalGoal = 0.0f;
	nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

	// Add start node to not tested list - this will ensure it gets tested.
	// As the algorithm progresses, newly discovered nodes get added to this
	// list, and will themselves be tested later
	std::list<sNode*> listNotTestedNodes;
	listNotTestedNodes.push_back(nodeStart);

	// if the not tested list contains nodes, there may be better paths
	// which have not yet been explored. However, we will also stop 
	// searching when we reach the target - there may well be better
	// paths but this one will do - it wont be the longest.
	while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)// Find absolutely shortest path // && nodeCurrent != nodeEnd)
	{
		// Sort Untested nodes by global goal, so lowest is first
		listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

		// Front of listNotTestedNodes is potentially the lowest distance node. Our
		// list may also contain nodes that have been visited, so ditch these...
		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
			listNotTestedNodes.pop_front();

		// ...or abort because there are no valid nodes left to test
		if (listNotTestedNodes.empty())
			break;

		nodeCurrent = listNotTestedNodes.front();
		nodeCurrent->bVisited = true; // We only explore a node once


		// Check each of this node's neighbours...
		for (auto nodeNeighbour : nodeCurrent->vecNeighbours)
		{
			// ... and only if the neighbour is not visited and is 
			// not an obstacle, add it to NotTested List
			if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
				listNotTestedNodes.push_back(nodeNeighbour);

			// Calculate the neighbours potential lowest parent distance
			float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

			// If choosing to path through this node is a lower distance than what 
			// the neighbour currently has set, update the neighbour to use this node
			// as the path source, and set its distance scores as necessary
			if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
			{
				nodeNeighbour->parent = nodeCurrent;
				nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

				// The best path length to the neighbour being tested has changed, so
				// update the neighbour's score. The heuristic is used to globally bias
				// the path algorithm, so it knows if its getting better or worse. At some
				// point the algo will realise this path is worse and abandon it, and then go
				// and search along the next best path.
				nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
			}
		}
	}
}

void GameScreen::ProcessInput() {
	//Aufgabe 2b)
	Vector2 playerPos = player.getPlayerPos();
	ItemBase iB = item[(int)playerPos.x][(int)playerPos.y];

	//Bewegung
	if ((player.weight() <= player.getStrength()) && !invVisible && !win && !info && !p) {
		if (IsKeyPressed(KEY_W)) {
			if (playerPos.y > 0 && map[(int)(playerPos.x)][(int)(playerPos.y - 1)].blocked) {
				player.MoveUp();
			}
		}
		else if (IsKeyPressed(KEY_A)) {
			if (playerPos.x > 0 && map[(int)(playerPos.x - 1)][(int)(playerPos.y)].blocked) {
				player.MoveLeft();
			}
		}
		else if (IsKeyPressed(KEY_S)) {
			if (playerPos.y < 14 && map[(int)(playerPos.x)][(int)(playerPos.y + 1)].blocked) {
				player.MoveDown();
			}
		}
		else if (IsKeyPressed(KEY_D)) {
			if (playerPos.x < 14 && map[(int)(playerPos.x + 1)][(int)(playerPos.y)].blocked) {
				player.MoveRight();
			}
		}
		else if (IsKeyPressed(KEY_SPACE)) {
			if (player.pickUp(iB) && iB.getVisible()) {
				item[(int)playerPos.x][(int)playerPos.y].setVisible(false);
				for (int i = 0; i < 10; i++) {
					inv[i] = player.getItemBase().getItem(i);
				}
			}
		}
	}


	//Wenn Inventory geöffnet ist
	if (invVisible && !vis) {
		Vector2 mouse = GetMousePosition();

		//Item Information ausgeben mit Mausposition
		for (int i = 0; i < 10; i++) {
			int j = i / 4;
			int k = i % 4;
			if (inv[i].getName().compare("") != 0) {
				if ((mouse.x > ((4 + (k * 2)) * 32)) && (mouse.x < ((4 + (k * 2)) * 32) + 32) && (mouse.y > ((6 + (j * 2)) * 32)) && (mouse.y < (((6 + (j * 2)) * 32) + 32))) {
					if (inv[i].getName().compare("Food") == 0) {
						inv[i].Draw();
						if (IsKeyPressed(KEY_D)) {
							//item[(int)playerPos.x][(int)playerPos.y] = inv[i];
							player.deleteItem(i);
							for (int i = 0; i < 10; i++) {
								inv[i] = player.getItemBase().getItem(i);
							}

						}
					}
					else if (inv[i].getName().compare("Hat") == 0) {
						inv[i].Draw();
						if (IsKeyPressed(KEY_D)) {

							for (int i = 0; i < 2; i++) {
								for (int j = 0; j < 2; j++) {
									if (item[(int)playerPos.x + i][(int)playerPos.y + j].getName().compare("") == 0) {
										item[(int)playerPos.x + i][(int)playerPos.y + j] = inv[i];
										std::cout << "Hier" << std::endl;
										break;
									}
								}
							}
							//item[(int)playerPos.x][(int)playerPos.y] = inv[i];
							player.deleteItem(i);
							for (int i = 0; i < 10; i++) {
								inv[i] = player.getItemBase().getItem(i);
							}

						}
					}
					else if (inv[i].getName().compare("Heart") == 0) {
						inv[i].Draw();
						if (IsKeyPressed(KEY_D)) {

							for (int i = 0; i < 2; i++) {
								for (int j = 0; j < 2; j++) {
									if (item[(int)playerPos.x + i][(int)playerPos.y + j].getName().compare("") == 0) {
										item[(int)playerPos.x + i][(int)playerPos.y + j] = inv[i];
										std::cout << "Hier" << std::endl;
										break;
									}
								}
							}
							//item[(int)playerPos.x][(int)playerPos.y] = inv[i];
							player.deleteItem(i);
							for (int i = 0; i < 10; i++) {
								inv[i] = player.getItemBase().getItem(i);
							}

						}
					}
					else if (inv[i].getName().compare("Help") == 0) {
						inv[i].Draw();
						DrawText("Can't drop", 10, 100, 20, GRAY);
						DrawText("this item!", 10, 130, 20, GRAY);

					}
					else if (inv[i].getName().compare("Key") == 0) {
						inv[i].Draw();
						DrawText("Can't drop", 10, 100, 20, GRAY);
						DrawText("this item!", 10, 130, 20, GRAY);

					}
					else if (inv[i].getName().compare("Sword") == 0) {
						inv[i].Draw();
						if (IsKeyPressed(KEY_D)) {

							for (int i = 0; i < 2; i++) {
								for (int j = 0; j < 2; j++) {
									if (item[(int)playerPos.x + i][(int)playerPos.y + j].getName().compare("") == 0) {
										item[(int)playerPos.x + i][(int)playerPos.y + j] = inv[i];
										std::cout << "Hier" << std::endl;
										break;
									}
								}
							}
							//item[(int)playerPos.x][(int)playerPos.y] = inv[i];
							player.deleteItem(i);
							for (int i = 0; i < 10; i++) {
								inv[i] = player.getItemBase().getItem(i);
							}

						}
					}

				}
			}
		}

		//Equipment droppen mit "D"
		for (int i = 0; i < 3; i++) {
			if ((mouse.x > ((4 + (i * 3)) * 32)) && (mouse.x < ((4 + (i * 3)) * 32) + 32) && (mouse.y > (12 * 32)) && (mouse.y < ((12 * 32) + 32))) {
				if (equ[i].getName().compare("Hat") == 0 && equ[i].getVisible()) {
					equ[i].Draw();
					if (IsKeyPressed(KEY_D)) {
						equ[i].setVisible(false);
					}
				}
				else if (equ[i].getName().compare("Sword") == 0 && equ[i].getVisible()) {
					equ[i].Draw();
					if (IsKeyPressed(KEY_D)) {
						equ[i].setVisible(false);
					}
				}
				else if (equ[i].getName().compare("Help") == 0 && equ[i].getVisible()) {
					equ[i].Draw();
					DrawText("Can't drop", 10, 100, 20, GRAY);
					DrawText("this item!", 10, 130, 20, GRAY);
				}
			}
		}

		//Equipment ausrüsten
		if (IsMouseButtonPressed(0)) {
			for (int i = 0; i < 10; i++) {
				int j = i / 4;
				int k = i % 4;
				if (inv[i].getName().compare("") != 0) {
					if ((mouse.x > ((4 + (k * 2)) * 32)) && (mouse.x < ((4 + (k * 2)) * 32) + 32) && (mouse.y > ((6 + (j * 2)) * 32)) && (mouse.y < (((6 + (j * 2)) * 32) + 32))) {
						if (inv[i].getName().compare("Hat") == 0 && (equ[0].getName().compare("") == 0 || !equ[0].getVisible())) {
							equ[0] = hatEqu;
							player.setSlot(hatEqu, 0);
							player.deleteItem(i);
							for (int i = 0; i < 10; i++) {
								inv[i] = player.getItemBase().getItem(i);
							}
						}
						else if (inv[i].getName().compare("Sword") == 0 && (equ[1].getName().compare("") == 0 || !equ[1].getVisible())) {
							equ[1] = swordEqu;
							player.setSlot(swordEqu, 1);
							player.deleteItem(i);
							for (int i = 0; i < 10; i++) {
								inv[i] = player.getItemBase().getItem(i);
							}
						}
						else if (inv[i].getName().compare("Help") == 0 && (equ[2].getName().compare("") == 0 || !equ[2].getVisible())) {
							equ[2] = helpEqu;
							player.setSlot(helpEqu, 2);
							player.deleteItem(i);
							for (int i = 0; i < 10; i++) {
								inv[i] = player.getItemBase().getItem(i);
							}
							for (int i = 0; i < 15; i++) {
								for (int j = 0; j < 15; j++) {
									if (item[i][j].getName().compare("Key") == 0) {
										item[i][j].setVisible(true);
									}
								}
							}
						}
					}
				}
			}

			//Equipment entrüsten
			for (int i = 0; i < 3; i++) {
				if ((mouse.x > ((4 + (i * 3)) * 32)) && (mouse.x < ((4 + (i * 3)) * 32) + 32) && (mouse.y > (12 * 32)) && (mouse.y < ((12 * 32) + 32))) {
					if (equ[i].getName().compare("Hat") == 0 && player.pickUp(equ[i])) {
						for (int j = 0; j < 10; j++) {
							inv[j] = player.getItemBase().getItem(j);
						}
						equ[0].setVisible(false);
					}
					else if (equ[i].getName().compare("Sword") == 0 && player.pickUp(equ[i])) {
						for (int j = 0; j < 10; j++) {
							inv[j] = player.getItemBase().getItem(j);
						}
						equ[1].setVisible(false);
					}
					else if (equ[i].getName().compare("Help") == 0 && player.pickUp(equ[i])) {
						for (int j = 0; j < 10; j++) {
							inv[j] = player.getItemBase().getItem(j);
						}
						equ[2].setVisible(false);
						for (int i = 0; i < 15; i++) {
							for (int j = 0; j < 15; j++) {
								if (item[i][j].getName().compare("Key") == 0) {
									item[i][j].setVisible(false);
								}
							}
						}
					}
				}
			}

			//Aufgabe 3a)
			//Sortieren nach
			//Gewicht
			if ((mouse.x > 20) && (mouse.x < 100) && (mouse.y > 240) && (mouse.y < 263)) {
				bool sortiert = false;
				while (!sortiert)
				{
					sortiert = true;
					for (int i = 0; i < 9; i++)
					{
						if ((player.getItemBase().getItem(i).getWeight() > player.getItemBase().getItem(i + 1).getWeight()) && player.getItemBase().getItem(i).getName().compare("") != 0)
						{
							ItemBase it = player.getItemBase().getItem(i);
							player.setItem(player.getItemBase().getItem(i + 1), i);
							player.setItem(it, i + 1);
							sortiert = false;
						}
					}
				}
				for (int j = 0; j < 10; j++) {
					inv[j] = player.getItemBase().getItem(j);
				}
			}


			//Name
			if ((mouse.x > 20) && (mouse.x < 100) && (mouse.y > 270) && (mouse.y < 293)) {
				for (int i = 0; i < 9; ++i) {
					for (int j = 0; j < 10 - i - 1; ++j) {
						int length;
						bool swap = false;
						bool in = true;
						if (player.getItemBase().getItem(j).getName().length() < player.getItemBase().getItem(j + 1).getName().length()) {
							length = player.getItemBase().getItem(j).getName().length();
						}
						else {
							length = player.getItemBase().getItem(j + 1).getName().length();
						}

						int k = 0;
						while (in && k < length) {
							int a = std::tolower(static_cast<unsigned char>(player.getItemBase().getItem(j).getName().at(k)));
							int b = std::tolower(static_cast<unsigned char>(player.getItemBase().getItem(j + 1).getName().at(k)));
							if (a == b) {
								k++;
							}
							if (a > b) {
								swap = true;
								in = false;
							}
							if (a < b) {
								in = false;
							}
						}
						if (swap) {
							ItemBase it = player.getItemBase().getItem(j);
							player.setItem(player.getItemBase().getItem(j + 1), j);
							player.setItem(it, j + 1);
						}
					}
				}
				for (int j = 0; j < 10; j++) {
					inv[j] = player.getItemBase().getItem(j);
				}
			}

			//Wert(Preis)
			if ((mouse.x > 20) && (mouse.x < 100) && (mouse.y > 300) && (mouse.y < 320)) {
				bool sortiert = false;

				while (!sortiert)
				{
					sortiert = true;
					for (int i = 0; i < 9; i++)
					{
						if ((player.getItemBase().getItem(i).getValue() > player.getItemBase().getItem(i + 1).getValue()) && player.getItemBase().getItem(i).getName().compare("") != 0)
						{
							ItemBase it = player.getItemBase().getItem(i);
							player.setItem(player.getItemBase().getItem(i + 1), i);
							player.setItem(it, i + 1);
							sortiert = false;
						}
					}
				}
				for (int j = 0; j < 10; j++) {
					inv[j] = player.getItemBase().getItem(j);
				}
			}
			//Aufgabe 3a) ende
		}
	}
	//Aufgabe 2b) ende

	//Aufgabe 3b)
	Vector2 mouse = GetMousePosition();
	if (IsMouseButtonPressed(0)) {
		if (mouse.x > 385 && mouse.x < 480 && mouse.y > 0 && mouse.y < 25) {
			for (int i = 0; i < 10; i++) {
				int itemRand = rand() % 6;
				switch (itemRand) {
				case 0:
					aufgabe3b[i] = food;
					break;
				case 1:
					aufgabe3b[i] = sword;
					break;
				case 2:
					aufgabe3b[i] = heart;
					break;
				case 3:
					aufgabe3b[i] = hat;
					break;
				case 4:
					aufgabe3b[i] = help;
					break;
				case 5:
					aufgabe3b[i] = key;
					break;
				default:
					break;
				}
			}
			vis = !vis;
		}

		//Sortieren nach 
		//Gewicht
		if ((mouse.x > 20) && (mouse.x < 100) && (mouse.y > 240) && (mouse.y < 263)) {
			bool sortiert = false;
			while (!sortiert)
			{
				sortiert = true;
				for (int i = 0; i < 9; i++)
				{
					if ((aufgabe3b[i].getWeight() > aufgabe3b[i + 1].getWeight()) && aufgabe3b[i].getName().compare("") != 0)
					{
						ItemBase it = aufgabe3b[i];
						aufgabe3b[i] = aufgabe3b[i + 1];
						aufgabe3b[i + 1] = it;

						sortiert = false;
					}
				}
			}
		}

		//Name
		if ((mouse.x > 20) && (mouse.x < 100) && (mouse.y > 270) && (mouse.y < 293)) {
			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 10 - i - 1; ++j) {
					int length;
					bool swap = false;
					bool in = true;
					if (aufgabe3b[j].getName().length() < aufgabe3b[j + 1].getName().length()) {
						length = aufgabe3b[j].getName().length();
					}
					else {
						length = aufgabe3b[j + 1].getName().length();
					}
					int k = 0;
					while (in && k < length) {
						int a = std::tolower(static_cast<unsigned char>(aufgabe3b[j].getName().at(k)));
						int b = std::tolower(static_cast<unsigned char>(aufgabe3b[j + 1].getName().at(k)));
						if (a == b) {
							k++;
						}
						if (a > b) {
							swap = true;
							in = false;
						}
						if (a < b) {
							in = false;
						}
					}
					if (swap) {
						ItemBase it = aufgabe3b[j];
						aufgabe3b[j] = aufgabe3b[j + 1];
						aufgabe3b[j + 1] = it;
					}
				}
			}
		}

		//Wert
		if ((mouse.x > 20) && (mouse.x < 100) && (mouse.y > 300) && (mouse.y < 320)) {
			bool sortiert = false;

			while (!sortiert)
			{
				sortiert = true;
				for (int i = 0; i < 9; i++)
				{
					if ((aufgabe3b[i].getValue() > aufgabe3b[i + 1].getValue()))
					{
						ItemBase it = aufgabe3b[i];
						aufgabe3b[i] = aufgabe3b[i + 1];
						aufgabe3b[i + 1] = it;

						sortiert = false;
					}
				}
			}
		}
	}

	//Information anzeigen lassen
	if (vis) {
		for (int i = 0; i < 10; i++) {
			int j = i / 4;
			int k = i % 4;
			if (aufgabe3b[i].getName().compare("") != 0) {
				if ((mouse.x > ((4 + (k * 2)) * 32)) && (mouse.x < ((4 + (k * 2)) * 32) + 32) && (mouse.y > ((6 + (j * 2)) * 32)) && (mouse.y < (((6 + (j * 2)) * 32) + 32))) {
					if (aufgabe3b[i].getName().compare("Food") == 0) {
						aufgabe3b[i].Draw();
					}
					else if (aufgabe3b[i].getName().compare("Hat") == 0) {
						aufgabe3b[i].Draw();
					}
					else if (aufgabe3b[i].getName().compare("Heart") == 0) {
						aufgabe3b[i].Draw();
					}
					else if (aufgabe3b[i].getName().compare("Help") == 0) {
						aufgabe3b[i].Draw();
					}
					else if (aufgabe3b[i].getName().compare("Key") == 0) {
						aufgabe3b[i].Draw();
					}
					else if (aufgabe3b[i].getName().compare("Sword") == 0) {
						aufgabe3b[i].Draw();
					}

				}
			}
		}
	}
	//Aufgabe 3b) ende
}

void GameScreen::Update() {
	//Öffnet das Inventory mit I
	if (IsKeyPressed(KEY_I) && !info) {
		invVisible = !invVisible;
	}

	if (IsKeyPressed(KEY_P) && !info && !invVisible) {
		p = !p;
	}

	/*if (IsKeyPressed(KEY_L)) {
		vis = !vis;
	}*/


	Vector2 pos = player.getPlayerPos();
	if (map[(int)pos.x][(int)pos.y].check == "hafen") {
		for (int i = 0; i < 10; i++) {
			if (player.getItemBase().getItem(i).getName().compare("Key") == 0) {
				win = true;
				DrawText("YOU WON!", 100, 200, 50, BLACK);
			}
		}
	}
}


void GameScreen::Draw() {
	ClearBackground(BLUE);
	if (!invVisible && !win) {

		for (int x = 0; x < 15; x++) {
			for (int y = 0; y < 15; y++) {
				Object to = map[x][y];
				to.Draw(x, y);
				if (!p) {
					ItemBase iBase = item[x][y];
					iBase.Draw(x, y);
				}
			}
		}

		if (!p) {
			player.Draw();
		}

		//Aufgabe 4 + Aufgabe 5 NPC
		if (p) {
			sNode* tempNode = nodeEnd;
			while (tempNode->parent != nullptr)
			{
				DrawRectangle(tempNode->x + 8, tempNode->y + 8, 16, 16, RED);
				tempNode = tempNode->parent;
			}
		}

		if (p) {
			if (IsKeyPressed(KEY_L) && (npc.weight() <= npc.getStrength())) {
				Vector2 npcPos = npc.getPlayerPos();
				int x = (int)npcPos.x;
				int y = (int)npcPos.y;

				if (npcMap[x][y + 1] && y < 14) {
					npcMap[x][y] = false;
					npc.MoveDown();

				}
				else if (npcMap[x + 1][y] && x < 14) {
					npcMap[x][y] = false;
					npc.MoveRight();

				}
				else if (npcMap[x - 1][y] && x > 1) {
					npcMap[x][y] = false;
					npc.MoveLeft();

				}
				else if (npcMap[x][y - 1] && y > 1) {
					npcMap[x][y] = false;
					npc.MoveUp();

				}
				if (item[x][y].getVisible()) {
					npc.pickUp(item[x][y]);
					item[x][y].setVisible(false);
				}
				
			}
			npc.Draw();

			for (int x = 0; x < 15; x++) {
				for (int y = 0; y < 15; y++) {
					ItemBase iBase = item[x][y];
					iBase.Draw(x, y);
				}
			}
		}
		//Aufgabe 4 + 5 ende

		std::string wert;
		const char* cstr;

		wert = std::to_string(player.getStrength());
		cstr = wert.c_str();
		DrawText("Strength: ", 10, 490, 15, BLACK);
		DrawText(cstr, 90, 490, 15, BLACK);

		wert = std::to_string(player.weight());
		cstr = wert.c_str();
		DrawText("Total Weight: ", 300, 490, 15, BLACK);
		DrawText(cstr, 405, 490, 15, BLACK);
	}

	if (invVisible && !info) {
		ClearBackground(BLACK);
		if (!vis) {
			player.Draw(invVisible, inv, equ);
		}
		else {
			Equipment a[3];
			player.Draw(invVisible, aufgabe3b, a);
		}

		DrawRectangle(10, 190, 105, 140, GRAY);
		DrawText("Sort", 40, 195, 20, BLACK);
		DrawText("ascending:", 11, 215, 20, BLACK);


		DrawRectangle(20, 240, 80, 23, WHITE);
		DrawText("Weight", 30, 240, 20, BLACK);

		DrawRectangle(20, 270, 80, 23, WHITE);
		DrawText("Name", 30, 270, 20, BLACK);

		DrawRectangle(20, 300, 80, 20, WHITE);
		DrawText("Value", 30, 300, 20, BLACK);

		DrawText("Task 3b)", 385, 10, 20, WHITE);
		DrawText("Equip and disarm with left click", 80, 450, 20, WHITE);
		DrawText("Drop item with \"D\"", 80, 480, 20, WHITE);
		DrawText("Hat", 135, 360, 15, GRAY);
		DrawText("Sword", 218, 360, 15, GRAY);
		DrawText("?-Box", 310, 360, 15, GRAY);
	}

	if (IsKeyPressed(KEY_H) && !invVisible) {
		info = !info;
	}

	if (info && !win) {
		DrawRectangle(70, 70, 340, 340, RAYWHITE);
		DrawText("Walk with \"W\", \"A\", \"S\", \"D\"", 75, 80, 20, BLACK);
		DrawText("Pick up item with \"Space\"", 75, 120, 20, BLACK);
		DrawText("Open inventory with \"I\"", 75, 160, 20, BLACK);
		DrawText("Close/Open this window with \"H\"", 75, 200, 20, BLACK);
		DrawText("Draw pathfinding with \"P\"", 75, 240, 20, BLACK);
		DrawText("Move NPC with \"L\"", 75, 280, 20, BLACK);
		DrawText("Goal:", 75, 320, 20, BLACK);
		DrawText("Find the key with the help", 75, 350, 20, BLACK);
		DrawText("of an item", 75, 380, 20, BLACK);
	}
}