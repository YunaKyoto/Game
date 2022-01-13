#pragma once

#include "raylib.h"
#include "objects/Object.h"
#include "items/ItemBase.h"
#include "PlayerChar.h"
#include "NPC.h"

class GameScreen{
protected:
    struct sNode
    {
        bool bObstacle = false;            // Is the node an obstruction?
        bool bVisited = false;            // Have we searched this node before?
        float fGlobalGoal;                // Distance to goal so far
        float fLocalGoal;                // Distance to goal if we took the alternative route
        int x;                            // Nodes position in 2D space
        int y;
        std::vector<sNode*> vecNeighbours;    // Connections to neighbours
        sNode* parent;                    // Node connecting to this node that offers shortest parent
    };

    sNode* nodes[15][15];
    sNode* nodeStart = nullptr;
    sNode* nodeEnd = nullptr;


    Object map[15][15];
    ItemBase item[15][15];
    ItemBase inv[10];
    Equipment equ[3];

    bool npcMap[15][15];

    Object path;
    Object house;
    Object hafen;
    Object water;
    Object pit;

    ItemBase key;
    ItemBase food;
    ItemBase heart;
    ItemBase sword;
    ItemBase help;
    ItemBase hat;

    Equipment hatEqu;
    Equipment swordEqu;
    Equipment helpEqu;

    PlayerChar player;

    NPC npc;

    bool invVisible = false;

    bool vis = false;
    
    bool win = false;

    bool info = true;

    bool p = false;
    
    ItemBase aufgabe3b[10];

    int test[15][15];


public:
    void Init();

    void SetMap();

    void PutItem(ItemBase iB);

    void PutPit(Object pit);

    void ProcessInput();

    void Update();

    void Draw();

    void pathfinding();
};
