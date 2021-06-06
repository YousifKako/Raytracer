#include <iostream>
#include <vector>

#include <chrono>
#include <thread>
#include <windows.h>

#include <Objects/Sphere.hpp>
#include <Objects/Light.hpp>
#include <Utilities.hpp>
#include <Raytracing/SphereRayTrace.hpp>

int main(int argc, const char* argv[])
{
    auto start = std::chrono::high_resolution_clock::now();

    // Setup Sphere Scene
    const auto deps = SphereRayTrace::Scene::setup();
    
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Setup Execution Time: "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.f
        << "ms"
        << std::endl;

    start = std::chrono::high_resolution_clock::now();
    
    // Run Sphere Scene
    SphereRayTrace::Scene::run(deps);

    end = std::chrono::high_resolution_clock::now();
    std::cout << "Computations Execution Time: "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.f
        << "ms"
        << std::endl;
}