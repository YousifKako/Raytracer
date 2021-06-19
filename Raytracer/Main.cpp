#include <iostream>
#include <vector>

#include <chrono>
#include <thread>

#include <Model/Model.hpp>
#include <Objects/Sphere.hpp>
#include <Objects/Light.hpp>
#include <Utilities.hpp>
#include <Raytracing/SphereRayTrace.hpp>
#include <Raytracing/TriangleRayTrace.hpp>

int main(int argc, const char* argv[])
{
    auto start = std::chrono::high_resolution_clock::now();
    
    //Model* model = new Model("C:\\Users\\PC\\Desktop\\Test.obj");
    //model->load();
    // Setup Sphere Scene
    //const auto deps = SphereRayTrace::Scene::setup();
    const auto deps = TriangleRayTrace::Scene::setup();
    
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Setup Execution Time: "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.f
        << "ms"
        << std::endl;

    start = std::chrono::high_resolution_clock::now();
    
    // Run Sphere Scene
    //SphereRayTrace::Scene::run(deps);
    TriangleRayTrace::Scene::run(deps);

    end = std::chrono::high_resolution_clock::now();
    std::cout << "Computations Execution Time: "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.f
        << "ms"
        << std::endl;
}