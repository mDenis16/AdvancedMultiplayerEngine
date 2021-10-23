#include "pch.h"
#include <iostream>
#include <EngineSimulator\SceneView.hpp>
#include <raylib.h>
#include <glm\glm.hpp>
#include "pch.h"

#include <Client/Network.hpp>
#include <Client/Client.hpp>

#include "Multiplayer.hpp"

//#include <Client/Network.hpp>
//#include <Client/Client.hpp>
//#include "Multiplayer.hpp"

int main()
{

     Multiplayer.Connect("192.168.88.252", 5555);


     scene.Init();

     while (true) {

     }

     std::cout << "Hello World!\n";


     // Initialization
    // //--------------------------------------------------------------------------------------
    //const int screenWidth = 768;
    //const int screenHeight = 768;

    //InitWindow(screenWidth, screenHeight, "QuadTree raylib implementation");

    //Vector2 ballPosition = { (float)screenWidth / 2, (float)screenHeight / 2 };

    //SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    ////--------------------------------------------------------------------------------------


    //std::cout << "Generated cells " << grid->cells.size() << std::endl;

    //// Main game loop
    //while (!WindowShouldClose())    // Detect window close button or ESC key
    //{
    //    // Update
    //    //----------------------------------------------------------------------------------
    //    if (IsKeyDown(KEY_RIGHT)) ballPosition.x += 5.0f;
    //    if (IsKeyDown(KEY_LEFT)) ballPosition.x -= 5.0f;
    //    if (IsKeyDown(KEY_UP)) ballPosition.y -= 5.0f;
    //    if (IsKeyDown(KEY_DOWN)) ballPosition.y += 5.0f;
    //    //----------------------------------------------------------------------------------

    //    // Draw
    //    //----------------------------------------------------------------------------------
    //    BeginDrawing();

    //    ClearBackground(RAYWHITE);


    //    GridCell<int> cellFond = grid->getCell(glm::vec3(ballPosition.x, ballPosition.y, 0));

    //    std::cout << "Currently in cell " << cellFond._index << std::endl;

    //    for (auto& cell : grid->cells)
    //    {
    //        DrawRectangleLines(cell.mins.x, cell.mins.y, grid->_cellSize.x, grid->_cellSize.y, BLACK);
    //       // DrawRayText(std::to_string(cell._index).c_str(), cell.mins.x, cell.mins.y, 25, RED);
    //    }

    //    const auto& nearby = grid->getNearby(cellFond);

    //    for (auto& cell : nearby)
    //    {
    //        DrawRectangle(cell->mins.x, cell->mins.y, grid->_cellSize.x, grid->_cellSize.y, BLUE);
    //   //    DrawRayText("nearby", cell.mins.x, cell.mins.y, 25, RED);
    //    }

    //    DrawCircleV(ballPosition, 25, MAROON);

    //    EndDrawing();
    //    //----------------------------------------------------------------------------------
    //}

    //// De-Initialization
    ////--------------------------------------------------------------------------------------
    //CloseRayWindow();        // Close window and OpenGL context
    ////--------------------------------------------------------------------------------------

    return 0;
}
