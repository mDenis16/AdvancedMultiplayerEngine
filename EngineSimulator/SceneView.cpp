#include <raylib.h>

#include <functional>
#include <iostream>
#include <glm\glm.hpp>

#include "pch.h"

#include <Client/Network.hpp>
#include <Client/Client.hpp>
#include "Multiplayer.hpp"
#include <Network/NetworkPacket.hpp>
#include <Entities/Entities.hpp>
#include <Gridmap/Grid.hpp>

#include "SceneView.hpp"

void SceneView::Init()
{

	this->renderThread = new std::thread(std::bind(&SceneView::OnRender, this));


}
static float angleBetween(
	glm::vec3 a,
	glm::vec3 b,
	glm::vec3 origin
) {
	glm::vec3 da = glm::normalize(a - origin);
	glm::vec3 db = glm::normalize(b - origin);
	return glm::acos(glm::dot(da, db));
}
#define MAX_COLUMNS 20


void SceneView::OnRender()
{
	//--------------------------------------------------------------------------------------

    
    const int screenWidth = 500;
    const int screenHeight = 500;
    glm::vec3 gtaMins = glm::vec3(-5395.54, -5024.13, 0);
    glm::vec3 gtaMaxs = glm::vec3(5395.54, 9024.13, 0);

    auto center = (gtaMins + gtaMaxs) * 0.5f;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera first person");

    // Define the camera to look into our 3d world (position, target, up vector)
    Camera3D camera = { 0 };
    //-763.4022, 7.327758, 40.59016
    camera.position = Vector3{ 10.0f, 30.0f, 10.0f };
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera.fovy = 125.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    //auto gridvIew = new Grid<int>(glm::vec3(-250, -250, 0), glm::vec3(250, 250, 0), glm::vec3(30, 30, 0));

    SetCameraMode(camera, CAMERA_FREE); // Set a first person camera mode

    SetTargetFPS(60);                           // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose())                // Detect window close button or ESC key
    {

        camera.position = Vector3{ scene.localPosition.x ,scene.localPosition.z + 120, scene.localPosition.y };

        camera.target = Vector3{ scene.localPosition.x ,scene.localPosition.z, scene.localPosition.y };
 

        UpdateCamera(&camera);                  // Update camera
   
        BeginDrawing();

        ClearBackground(WHITE);

        BeginMode3D(camera);

  
         if (IsKeyDown(KEY_RIGHT)) localPosition.y += 0.2f;
         if (IsKeyDown(KEY_LEFT)) localPosition.y -= 0.2f;
         if (IsKeyDown(KEY_UP)) localPosition.x += 0.2f;
         if (IsKeyDown(KEY_DOWN)) localPosition.x -= 0.2f;


         std::clamp(localPosition.x, Multiplayer.grid->_min.x, Multiplayer.grid->_max.x);
         std::clamp(localPosition.z, Multiplayer.grid->_min.y, Multiplayer.grid->_max.y);

        DrawPlane( Vector3 { 0, 0, 0 }, Vector2 { 500, 500 }, BLUE);
        for (auto& cell : Multiplayer.grid->cells) {
            auto center = (cell.mins + cell.maxs) * 0.5f;
            DrawLine3D(Vector3{ cell.mins.x, 0, cell.mins.y }, Vector3{ cell.maxs.x, 0, cell.mins.y }, RED);
            DrawLine3D(Vector3{ cell.mins.x, 0, cell.mins.y }, Vector3{ cell.mins.x, 0, cell.maxs.y }, GREEN);
     
         //   RLAPI void DrawSphereEx(Vector3 centerPos, float radius, int rings, int slices, Color color);            // Draw sphere with extended parameters

            //DrawSphereEx(Vector3{ center.x, center.z, center.y }, 5.f, 5, 5, RED);
           
            

           
            
        }
        
        DrawSphere(Vector3{ localPosition.x, localPosition.z, localPosition.y }, 5.f, MAGENTA);
        {

           std::lock_guard<std::mutex> guard(Multiplayer.streamLock);
            
                for (auto& ent : Multiplayer.StreamedEntities)
                {
                  auto render =   ent->Interpolation();
               
                   std::cout << "Entity interpolated position X: " << ent->renderData.Position.x << " Y: " << ent->renderData.Position.y << " Z: " << ent->renderData.Position.z << std::endl;


                	//-763.4022, 7.327758, 40.59016
                    DrawSphereEx(Vector3{ ent->Position.x ,  ent->Position.z ,  ent->Position.y }, 5.f, 5, 5, BLACK);
                    DrawSphereEx(Vector3{ render.Position.x ,  render.Position.z ,  render.Position.y }, 5.f, 5, 5, RED);
                }
            
        }

        EndMode3D();
        localCell = Multiplayer.grid->getCell(glm::vec3(localPosition.x, localPosition.y, localPosition.z));

        if (localCell) {
            auto nearby = Multiplayer.grid->getNearby(localCell);
            for (auto& cell : nearby) {
                auto center = (cell->mins + cell->maxs) * 0.5f;

                auto pos = GetWorldToScreen(Vector3{ center.x, 0, center.y }, camera);

                DrawRayText(std::to_string(cell->_index).c_str(), pos.x, pos.y, 10, BLACK);
            }
        }

        Multiplayer.OnCreateMove();
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseRayWindow();        // Close window and OpenGL context
    //------------------------------------------------------------
}