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

#include "EntityPrediction.h"

static const int INTERPOLATION_TIME_MS = 300;


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

static float game_time_ms_ = 0.f;
float LerpDegrees(float start, float end, float amount) {
	float difference = std::abs(end - start);
	if (difference > 180) {
		// We need to add on to one of the values.
		if (end > start) {
			// We'll add it on to start...
			start += 360;
		}
		else {
			// Add it on to end.
			end += 360;
		}
	}

	// Interpolate it.
	float value = (start + ((end - start) * amount));

	// Wrap it..
	float rangeZero = 360;

	if (value >= 0 && value <= 360)
		return value;

	return fmod(value, rangeZero);
}
struct TextView2D
{
	std::string str;
	glm::vec3 position;
};

uint64_t get_interpolation_time_point_ms() {
	uint64_t time_point = 0;

	if (game_time_ms_ > INTERPOLATION_TIME_MS)
		time_point = game_time_ms_ - INTERPOLATION_TIME_MS;

	return time_point;
}

static const float Unlerp(const double fStart, const double fPos, const double fEnd)
{
	// Avoid dividing by 0 (results in INF values)
	if (fStart == fEnd) return 1.0f;

	return (float)((fPos - fStart) / (fEnd - fStart));
}
double get_time_fraction(uint64_t start_ms, uint64_t stop_ms, uint64_t between_ms) {
	return static_cast<double>(between_ms - start_ms) / static_cast<double>(stop_ms - start_ms);
}
unsigned int timeGetTime()
{
	return Clock::now().time_since_epoch().count();
}


float LOW_LIMIT = 0.0167f;          // Keep At/Below 60fps
float HIGH_LIMIT = 0.1f;            // Keep At/Above 10fps
void SceneView::OnRender()
{
	//--------------------------------------------------------------------------------------


	const int screenWidth = 800;
	const int screenHeight = 800;
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
	camera.fovy = 60.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	//auto gridvIew = new Grid<int>(glm::vec3(-250, -250, 0), glm::vec3(250, 250, 0), glm::vec3(30, 30, 0));

	SetCameraMode(camera, CAMERA_FREE); // Set a first person camera mode

	SetTargetFPS(60);                           // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------
	Multiplayer.StreamedEntities.reserve(6);

	// Main game loop
	while (!WindowShouldClose())                // Detect window close button or ESC key
	{
		Multiplayer.CurrentFrameTime = enet_time_get();

		std::vector< TextView2D > Texts;



	

		camera.position = Vector3{ scene.localPosition.x ,scene.localPosition.z + 5, scene.localPosition.y };

		camera.target = Vector3{ scene.localPosition.x ,scene.localPosition.z, scene.localPosition.y };


		UpdateCamera(&camera);                  // Update camera

		BeginDrawing();

		ClearBackground(WHITE);

		BeginMode3D(camera);

		static int HeadingSpeed = 100;

		static float TargetHeading = 0.f;
		static float LastHeading = 0.f;
		static std::uint32_t lastChange = 0.f;
		float Speed = 0.5f;

		float cameraAngle = angleBetween(glm::vec3(camera.position.x, camera.position.z, camera.position.y), scene.localPosition, glm::vec3(0, 0, 0));

		bool move = false;
		if (IsKeyDown(KEY_RIGHT)) {
			TargetHeading = -90.f;
			move = true;
		}
		if (IsKeyDown(KEY_LEFT)) {
			TargetHeading = 90.f;
			move = true;
		}
		if (IsKeyDown(KEY_UP)) {
			TargetHeading = 180;
			move = true;
		}
		if (IsKeyDown(KEY_DOWN)) {
			TargetHeading = 360;
			move = true;
		}
	
		 

		localHeading = LerpDegrees(localHeading, TargetHeading, 0.025f);

		
		float radius = 5.f;
		float a = glm::radians(localHeading);

		glm::vec3 location(radius * cos(a) + scene.localPosition.x, radius * sin(a) + scene.localPosition.y, scene.localPosition.z);

		DrawLine3D(Vector3{ scene.localPosition.x, scene.localPosition.z, scene.localPosition.y }, Vector3{ location.x, location.z, location.y }, Color(255, 0, 0, 255));
		if (move)
		{
			double a = glm::radians(localHeading);

		
			glm::vec3 dir(cos(a) ,  sin(a), 0);


			localPosition += dir * Speed;
			
		}



		std::clamp(localPosition.x, Multiplayer.grid->_min.x, Multiplayer.grid->_max.x);
		std::clamp(localPosition.z, Multiplayer.grid->_min.y, Multiplayer.grid->_max.y);

		DrawPlane(Vector3{ 0, 0, 0 }, Vector2{ 500, 500 }, BLUE);
		//for (auto& cell : Multiplayer.grid->cells) {
		//	auto center = (cell.mins + cell.maxs) * 0.5f;
		//	DrawLine3D(Vector3{ cell.mins.x, 0, cell.mins.y }, Vector3{ cell.maxs.x, 0, cell.mins.y }, RED);
		//	DrawLine3D(Vector3{ cell.mins.x, 0, cell.mins.y }, Vector3{ cell.mins.x, 0, cell.maxs.y }, GREEN);

		//	//   RLAPI void DrawSphereEx(Vector3 centerPos, float radius, int rings, int slices, Color color);            // Draw sphere with extended parameters

		//	   //DrawSphereEx(Vector3{ center.x, center.z, center.y }, 5.f, 5, 5, RED);





		//}
		//
		std::uint32_t EntityTime = -1;
		std::uint32_t streamCount = 0;


		std::lock_guard<std::mutex> lock_guard(Multiplayer.streamLock); {

			std::uint32_t delayedTime = Network::CurrentFrameTime - TIME_DELAY;


			for (auto entity : Multiplayer.StreamedEntities) {



			//if (entity->previousTickcount == 0)
			//	entity->previousTickcount = Network::CurrentFrameTime + 1;

			//	//if (Network::CurrentFrameTime > entity->previousTickcount)
			//	{
			//		std::uint32_t deltaError = TIME_DELAY;

			//		entity->lastPosition = entity->Position;
			//		glm::vec3 predictedVelocity = (entity->ShadowPosition - entity->Position);// ((entity->ShadowPosition - entity->Position) + (entity->Velocity)) / 2.f;
			//		entity->lastVelocity = entity->Velocity;
			//		if (entity->Velocity != glm::vec3(0, 0, 0)) {
			//			entity->Position += glm::normalize(entity->Velocity ) * (float)TIME_DELAY * 1000.f/60.f;// *(float)(entity->EntitySpeed);// *entity->EntitySpeed * (float)TIME_DELAY) / 60.f;
			//		}

			//	
			//		if (entity->HeadingPerTick != 0.f) {
			//			entity->PreviousHeading = entity->Heading;

			//			float ab = (entity->ShadowHeading - entity->PreviousHeading) / TIME_DELAY;

			//			ab += entity->HeadingPerTick;
			//			ab /= 2.f;

			//			entity->Heading += ab * TIME_DELAY;
			//		}
			//		entity->previousTickcount = Network::CurrentFrameTime + TIME_DELAY;
			//		entity->lastPreUpdate = entity->lastUpdate;
			//		entity->lastUpdate = Network::CurrentFrameTime;
			//	}

			//	float progress = (float)(Network::CurrentFrameTime - entity->lastUpdate) / (entity->lastUpdate - entity->lastPreUpdate);
			//	bool isVelNull = entity->Velocity == glm::vec3(0, 0, 0);
			//	glm::vec3 pos = entity->Position; //glm::mix(entity->lastPosition, entity->Position, progress);
			//	float interpolatedHeading = LerpDegrees(entity->PreviousHeading, entity->Heading, progress);
			//	if (isVelNull)
			//	{
			//		if (entity->PreviousVelocity != glm::vec3(0, 0, 0))
			//			entity->TimeWhenVelocityStopped = Network::CurrentFrameTime;

			//		float realProgress = (float)(Network::CurrentFrameTime - entity->TimeWhenVelocityStopped) / (entity->lastUpdate - entity->lastPreUpdate);

			//		std::cout << "Has stopped real Progress => " << realProgress << std::endl;
			//	//	pos = glm::mix(entity->lastPosition, entity->ShadowPosition, glm::clamp(realProgress, 0.f, 1.f));
			//	}

				EntityPrediction::OnPreRender(entity);


				DrawCylinder(Vector3{ entity->Position.x, 	entity->Position.z, 	entity->Position.y }, 2.0f, 2.0f, 4.0f, 6, RED);

				

				/*{
					float radius = 5.f;
					float a = glm::radians(interpolatedHeading);

					glm::vec3 location(radius * cos(a) + pos.x, radius * sin(a) + pos.y, pos.z);

					DrawLine3D(Vector3{ pos.x, pos.z, pos.y }, Vector3{ location.x, location.z, location.y }, Color(5, 255, 0, 255));
				}*/
			//	if (!entity->lagRecords.empty())

				DrawCylinder(Vector3{ entity->ShadowPosition.x, entity->ShadowPosition.z, entity->ShadowPosition.y }, 2.0f, 2.0f, 4.0f, 6, Color{0,0,0,100});

				/*if (entity->Velocity != glm::vec3(0, 0, 0)) {
					glm::vec3 arrow = pos + entity->Velocity * 10.f;

					float velocityProgress = (float)(Network::CurrentFrameTime - entity->lastTickUpdate) / (entity->lastUpdate - entity->lastPreUpdate);

					glm::vec3 interpolatedVelocity = glm::mix(entity->PreviousVelocity, entity->Velocity, progress);

					double angle = std::atan2(interpolatedVelocity.y, interpolatedVelocity.x);
					double a = angle;

					glm::vec3 location(10.f * cos(a) + pos.x, 10.f * sin(a) + pos.y, pos.z);

					
					DrawLine3D(Vector3{ pos.x, pos.z, pos.y }, Vector3{ location.x, location.z, location.y }, Color(255, 0, 255, 255));


				}*/
				
				

			}
		}
		DrawCylinderWires(Vector3{ localPosition.x, 	localPosition.z, 	localPosition.y }, 2.0f, 2.0f, 4.0f, 6, BLACK);

		EndMode3D();

		DrawRayText(std::to_string(streamCount).c_str(), 50, 50, 20, BLACK);

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

		for(auto& text : Texts)
		{
			auto pos = GetWorldToScreen(Vector3{ center.x, 0, center.y }, camera);

			DrawRayText("SAMP", pos.x, pos.y, 10, BLACK);
		}

		EndDrawing();
		
		ClockTime end = Clock::now();
		//THREAD_WAKE(Multiplayer.HandleFlow);
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseRayWindow();        // Close window and OpenGL context
	//------------------------------------------------------------
}