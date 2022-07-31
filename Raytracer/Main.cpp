#include <iostream>
#include <vector>

#include <chrono>
#include <thread>

#include <Model/Model.hpp>
#include <Objects/Sphere.hpp>
#include <Objects/Light.hpp>
#include <Utilities.hpp>
#include <Raytracing/SphereTriangleRayTrace.hpp>

int main(int argc, const char* argv[])
{
    auto start = std::chrono::high_resolution_clock::now();

    auto deps = Scene::setup();

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Setup Execution Time: "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.f
        << "ms"
        << std::endl;

    start = std::chrono::high_resolution_clock::now();

    Scene::run(deps);

    end = std::chrono::high_resolution_clock::now();
    std::cout << "Computations Execution Time: "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.f
        << "ms"
        << std::endl;


    if (!glfwInit())
        exit(-1);

    GLFWwindow* window = glfwCreateWindow(CW, CH, "Raytracer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glDrawPixels(CW, CH, GL_RGBA, GL_UNSIGNED_BYTE, deps.pixels);
    glfwSwapBuffers(window);
    while (!glfwWindowShouldClose(window))
        glfwPollEvents();
    glfwTerminate();
}