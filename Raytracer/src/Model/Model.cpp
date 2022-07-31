#include <Model/Model.hpp>

#include <iostream>
#include <fstream>
#include <string>

typedef struct Vert3
{
    float data[3];
} Vert3;

typedef struct Vert2
{
    float data[2];
} Vert2;

const std::vector<std::string>
Model::split(std::string const& original, char separator)
{
    std::vector<std::string> results  = { };
    std::string::const_iterator start = original.begin();
    std::string::const_iterator end   = original.end();
    std::string::const_iterator next  = std::find(start, end, separator);

    while (next != end)
    {
        results.push_back(std::string(start, next));
        start = next + 1;
        next  = std::find(start, end, separator);
    }

    results.push_back(std::string(start, next));
    return results;
}

Model::Model(const std::string& model_name)
{
    std::ifstream model(model_name, std::ios::in);
    std::string buffer = "";

    std::vector<Vector3D<double>> temp_vertices = { };
    std::vector<Vector3D<double>> temp_normals = { };
    std::vector<Vector3D<double>> temp_texture = { };

    Mesh mesh = Mesh();
    std::string mesh_name = "";
    uint16_t num_objs = 0;

    while (std::getline(model, buffer))
    {
        if (buffer[0] == 'o')
        {
            num_objs++;
            if (num_objs > 1)
            {
                this->model.push_back(mesh);
                mesh = Mesh();
            }
            mesh_name.assign(buffer.begin() + 2, buffer.end());
        }
        else if (buffer[0] == 'v')
        {
            auto result = this->split(buffer, ' ');
            if (buffer[1] == ' ')
                temp_vertices.push_back(Vector3D<double>{ std::stof(result[1]), std::stof(result[2]), std::stof(result[3]) });
            else if (buffer[1] == 't')
                temp_texture.push_back(Vector3D<double>{ std::stof(result[1]), std::stof(result[2]), 0.f });
            else if (buffer[1] == 'n')
                temp_normals.push_back(Vector3D<double>{ std::stof(result[1]), std::stof(result[2]), std::stof(result[3]) });
        }
        else if (buffer[0] == 'f')
        {
            auto result = this->split(buffer, ' ');
            auto vertex1 = this->split(result[1], '/');
            auto vertex2 = this->split(result[2], '/');
            auto vertex3 = this->split(result[3], '/');
            Triangle triangle = { };

            triangle.set_specular(500);

            triangle.set_vertices(temp_vertices[std::stoi(vertex1[0]) - 1],
                temp_vertices[std::stoi(vertex2[0]) - 1],
                temp_vertices[std::stoi(vertex3[0]) - 1]);

            triangle.set_normals(temp_normals[std::stoi(vertex1[2]) - 1]);

            if (num_objs == 1)
            {
                triangle.set_reflective(0.4);
                triangle.set_vertices_rgb({ 0, 1, 0.87 }, { 0, 1, 0.87 }, { 0, 1, 0.87 });
            }
            else if (num_objs == 2)
            {
                triangle.set_reflective(0.2);
                triangle.set_vertices_rgb({ 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 });
            }
            else
            {
                triangle.set_reflective(0.1);
                triangle.set_vertices_rgb({ 0.18, 0.31, 0.31 }, { 0.18, 0.31, 0.31 }, { 0.18, 0.31, 0.31 });
            }

            mesh.triangles->push_back(triangle);
        }
    }

    this->model.push_back(mesh);
    this->num_objs = num_objs;
}

const std::vector<Mesh>&
Model::get()
{
    return this->model;
}