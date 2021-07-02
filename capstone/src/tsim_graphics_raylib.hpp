
#ifndef __GRAPHICS_RAYLIB_HPP__
#define __GRAPHICS_RAYLIB_HPP__

namespace raylib {
extern "C" {
#include "raylib.h"  // C API.
}
}  // namespace raylib

// #include "raylib.h"
#include <cstdlib>
#include <vector>

#include <glm/glm.hpp>

#include "map.hpp"

class RaylibGraphics {
   public:
    RaylibGraphics(std::vector<sim::Road> roads) : roads_(roads) {
        raylib::InitWindow(screenWidth_, screenHeight_, "OpenDrive Traffic Simulation");

        // Define the camera to look into our 3d world
        camera_.position = (raylib::Vector3){10.0f, 10.0f, 10.0f};  // Camera position
        camera_.target = (raylib::Vector3){0.0f, 0.0f, 0.0f};       // Camera looking at point
        camera_.up = (raylib::Vector3){0.0f, 1.0f, 0.0f};           // Camera up vector (rotation towards target)
        camera_.fovy = 45.0f;                                       // Camera field-of-view Y
        camera_.projection = raylib::CAMERA_PERSPECTIVE;            // Camera mode type

        raylib::SetCameraMode(camera_, raylib::CAMERA_FREE);  // Set a free camera mode

        raylib::SetTargetFPS(60);  // Set our game to run at 60 frames-per-second
        //--------------------------------------------------------------------------------------
    }
    ~RaylibGraphics() {
        raylib::CloseWindow();  // Close window and OpenGL context
    }
    const int screenWidth_ = 800;
    const int screenHeight_ = 450;
    void render() {
        while (!raylib::WindowShouldClose()) {
            raylib::UpdateCamera(&camera_);  // Update camera

            // Draw
            raylib::BeginDrawing();

            raylib::ClearBackground(raylib::RAYWHITE);

            raylib::BeginMode3D(camera_);

            // DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
            raylib::DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, raylib::MAROON);

            raylib::DrawGrid(10, 1.0f);

            std::vector<raylib::Vector3> linevec;
            linevec.push_back(raylib::Vector3{0.0f, 0.0f, 0.0f});

            linevec.push_back(raylib::Vector3{1.0f, 0.7f, 2.1f});
            linevec.push_back(raylib::Vector3{1.0f, 0.5f, 1.6f});
            glm::vec3 vector;

            // for (int i = 0; i < 100000; i++) {
            //     linevec.push_back(raylib::Vector3{(float)(std::rand() % 100) / 50, (float)(std::rand() % 100) / 50,
            //                                       (float)(std::rand() % 100) / 50});
            // }
            int linectr{0};
            raylib::Vector3 start;

            for (auto road : roads_) {
                // if (auto rep_it = m_RoadReps.find(line.type); rep_it != m_RoadReps.end()) {
                // auto& rep = rep_it->second;
                auto width = 2.f;
                for (auto pt : road.points())
                    for (int line = 0; line < road.points().size() - 1; line++) {
                        raylib::DrawPoint3D(
                            raylib::Vector3{road.points().at(line).x(), road.points().at(line).y(),
                                            road.points().at(line).z()},
                            raylib::BLUE);
                        linectr++;
                    }
            }
            std::cout << "drawing " << linectr << " lines" << std::endl;
            // for (int line = 0; line < linevec.size() - 1; line++) {
            //     raylib::DrawLine3D(linevec[line], linevec[line + 1], raylib::BLUE);
            // }

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
        }
    }

   private:
    std::vector<sim::Road> roads_;

    raylib::Camera3D camera_{0};
    raylib::Vector3 cubePosition = {0.0f, 0.0f, 0.0f};
};
#endif  // __GRAPHICS_RAYLIB_HPP__
