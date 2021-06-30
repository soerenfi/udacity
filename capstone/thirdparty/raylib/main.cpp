
namespace raylib {
extern "C" {
#include "raylib.h"  // C API.
}
}  // namespace raylib

// #include "raylib.h"
#include <cstdlib>
#include <vector>

#include <glm/glm.hpp>
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    raylib::InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera free");

    // Define the camera to look into our 3d world
    raylib::Camera3D camera = {0};
    camera.position = (raylib::Vector3){10.0f, 10.0f, 10.0f};  // Camera position
    camera.target = (raylib::Vector3){0.0f, 0.0f, 0.0f};       // Camera looking at point
    camera.up = (raylib::Vector3){0.0f, 1.0f, 0.0f};           // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                       // Camera field-of-view Y
    camera.projection = raylib::CAMERA_PERSPECTIVE;            // Camera mode type

    raylib::Vector3 cubePosition = {0.0f, 0.0f, 0.0f};

    raylib::SetCameraMode(camera, raylib::CAMERA_FREE);  // Set a free camera mode

    raylib::SetTargetFPS(60);  // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!raylib::WindowShouldClose())  // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        raylib::UpdateCamera(&camera);  // Update camera

        if (raylib::IsKeyDown('Z')) camera.target = (raylib::Vector3){0.0f, 0.0f, 0.0f};
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        raylib::BeginDrawing();

        raylib::ClearBackground(raylib::RAYWHITE);

        raylib::BeginMode3D(camera);

        // DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
        raylib::DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, raylib::MAROON);

        raylib::DrawGrid(10, 1.0f);

        std::vector<raylib::Vector3> linevec;
        linevec.push_back(raylib::Vector3{0.0f, 0.0f, 0.0f});

        linevec.push_back(raylib::Vector3{1.0f, 0.7f, 2.1f});
        linevec.push_back(raylib::Vector3{1.0f, 0.5f, 1.6f});
        glm::vec3 vector;

        for (int i = 0; i < 100000; i++) {
            linevec.push_back(raylib::Vector3{(float)(std::rand() % 100) / 50, (float)(std::rand() % 100) / 50,
                                              (float)(std::rand() % 100) / 50});
        }

        for (int line = 0; line < linevec.size() - 1; line++) {
            raylib::DrawLine3D(linevec[line], linevec[line + 1], raylib::BLUE);
        }

        raylib::EndMode3D();

        raylib::DrawRectangle(10, 10, 320, 133, raylib::Fade(raylib::SKYBLUE, 0.5f));
        raylib::DrawRectangleLines(10, 10, 320, 133, raylib::BLUE);

        raylib::DrawText("Free camera default controls:", 20, 20, 10, raylib::BLACK);
        raylib::DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, raylib::DARKGRAY);
        raylib::DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, raylib::DARKGRAY);
        raylib::DrawText("- Alt + Mouse Wheel Pressed to Rotate", 40, 80, 10, raylib::DARKGRAY);
        raylib::DrawText("- Alt + Ctrl + Mouse Wheel Pressed for Smooth Zoom", 40, 100, 10, raylib::DARKGRAY);
        raylib::DrawText("- Z to zoom to (0, 0, 0)", 40, 120, 10, raylib::DARKGRAY);

        raylib::EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    raylib::CloseWindow();  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}