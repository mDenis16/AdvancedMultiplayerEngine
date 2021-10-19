#include <raylib.h>
#include "SceneView.hpp"
#include <functional>
#include <iostream>
#include <glm\glm.hpp>


Camera camera = { 0 };

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


    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera first person");

    // Define the camera to look into our 3d world (position, target, up vector)
    Camera camera = { 0 };
    camera.position = Vector3{ 4.0f, 2.0f, 4.0f };
    camera.target = Vector3{ 0.0f, 1.8f, 0.0f };
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

 
    SetCameraMode(camera, CAMERA_FIRST_PERSON); // Set a first person camera mode

    SetTargetFPS(60);                           // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())                // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);                  // Update camera
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawPlane(Vector3 { 0.0f, 0.0f, 0.0f }, Vector2 { 32.0f, 32.0f }, LIGHTGRAY); // Draw ground
        DrawCube(Vector3 { -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
        DrawCube(Vector3 { 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
        DrawCube(Vector3 { 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall

        DrawCube(Vector3{ camera.target.x,camera.target.y - 2,camera.target.z }, 2.0f, 2.0f, 2.f, RED);

     

        EndMode3D();

        DrawRectangle(10, 10, 220, 70, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(10, 10, 220, 70, BLUE);

       // DrawText("First person camera default controls:", 20, 20, 10, BLACK);
       // DrawText("- Move with keys: W, A, S, D", 40, 40, 10, DARKGRAY);
        //DrawText("- Mouse move to look around", 40, 60, 10, DARKGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseRayWindow();        // Close window and OpenGL context
    //------------------------------------------------------------
}