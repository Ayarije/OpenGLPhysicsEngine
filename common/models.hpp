#ifndef MODELS_HPP
#define MODELS_HPP

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>
using namespace std;

#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;


struct Vertex
{
    vec3 pos;
    vec3 normal;
    vec2 tex;

    bool operator==(const Vertex& other) const {
        return pos[0] == other.pos[0] && pos[1] == other.pos[1] && pos[2] == other.pos[2] &&
               normal[0] == other.normal[0] && normal[1] == other.normal[1] && normal[2] == other.normal[2] &&
               tex[0] == other.tex[0] && tex[1] == other.tex[1];
    }
};

// Provide a hash specialization so unordered_map<Vertex, ...> is usable
namespace std {
template<>
struct hash<Vertex> {
    size_t operator()(Vertex const& v) const noexcept {
        size_t h1 = std::hash<float>{}(v.pos.x);
        size_t h2 = std::hash<float>{}(v.pos.y);
        size_t h3 = std::hash<float>{}(v.pos.z);
        size_t h4 = std::hash<float>{}(v.normal.x);
        size_t h5 = std::hash<float>{}(v.normal.y);
        size_t h6 = std::hash<float>{}(v.normal.z);
        size_t h7 = std::hash<float>{}(v.tex.x);
        size_t h8 = std::hash<float>{}(v.tex.y);

        size_t seed = h1;
        seed ^= h2 + 0x9e3779b9 + (seed<<6) + (seed>>2);
        seed ^= h3 + 0x9e3779b9 + (seed<<6) + (seed>>2);
        seed ^= h4 + 0x9e3779b9 + (seed<<6) + (seed>>2);
        seed ^= h5 + 0x9e3779b9 + (seed<<6) + (seed>>2);
        seed ^= h6 + 0x9e3779b9 + (seed<<6) + (seed>>2);
        seed ^= h7 + 0x9e3779b9 + (seed<<6) + (seed>>2);
        seed ^= h8 + 0x9e3779b9 + (seed<<6) + (seed>>2);
        return seed;
    }
};
}

class Model
{
private:
    vector< unsigned short > indices;

    mat4 TranslationMat;
    mat4 ScaleMat;
    mat4 RotationMat;
    mat4 MaterialMat;

    GLuint ModelID;
    GLuint MaterialID;
public:
    Model(GLuint programID, ModelLoader loader, string filepath);
    ~Model();

    int setPos(vec3 pos);
    int setScale(vec3 scale);
    int setRotation(vec3 omega);

    int draw();
};

class ModelLoader
{
public:
    ModelLoader();
    ~ModelLoader();

    vector< Model > ModelRegister;
    
    vector< Vertex > vertex_buffer;

    int drawModels(unordered_map<string, mat4> uniform_data);
    int loadOBJ(string filepath, vector< unsigned short > &out_indices);
};

#endif