#include "models.hpp"

/*----[ Model Class ]----*/
Model::Model(GLuint programID, ModelLoader loader, string filepath)
{
    this->TranslationMat = mat4(1.0f);
    this->RotationMat = mat4(1.0f);
    this->ScaleMat = mat4(1.0f);

    this->ModelID = glGetUniformLocation(programID, "ModelMat");
    this->MaterialID = glGetUniformLocation(programID, "MaterialData");

    loader.loadOBJ(filepath, this->indices);
}

Model::~Model() {}

int Model::draw()
{
    mat4 ModelMat = this->TranslationMat * this->RotationMat * this->ScaleMat;

    glUniformMatrix4fv(this->ModelID, 1, GL_FALSE, &ModelMat[0][0]);
    glUniformMatrix4fv(this->MaterialID, 1, GL_FALSE, &this->MaterialMat[0][0]);
    return 1;
}

int Model::setPos(vec3 pos) {
    this->TranslationMat = translate(mat4(1.0f), pos);
}

int Model::setScale(vec3 scaleVec) {
    this->ScaleMat = scale(mat4(1.0f), scaleVec);
}

int Model::setRotation(vec3 omega) {
    this->RotationMat = rotate(mat4(1.0f), omega);
}

/*----[ ModelLoader Class ]----*/
int ModelLoader::drawModels(unordered_map<GLuint, mat4> uniform_data, GLuint programID)
{
    for (auto it = uniform_data.begin(); it != uniform_data.end(); it++) {
        glUniformMatrix4fv(it->first, 1, GL_FALSE, &it->second[0][0]);
    }

    int drawn = 0;
    for (Model &m : ModelRegister)
    {
        if (m.draw()) ++drawn;
    }
    return drawn;
}

// .obj parser
vector< float > _extract_vertex_data(const std::string &s, char delimiter) {
    std::vector< float > tokens;
    std::string token;
    std::istringstream tokenStream(s);
    
    // Read from tokenStream into 'token', stopping at 'delimiter'
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(stof(token));
    }
    return tokens;
}

vector< vec3 > _extract_face_data(const string &s, char delimiter) {
    std::vector< vec3 > tokens;
    std::string token;
    std::istringstream tokenStream(s);
    
    // Read from tokenStream into 'token', stopping at 'delimiter'
    while (std::getline(tokenStream, token, delimiter)) {
        vector<float> data = _extract_vertex_data(token, '/');
        while (data.size() < 3) data.push_back(-1);

        tokens.push_back(vec3((int) data[0], (int) data[1], (int) data[2]));
    }
    return tokens;
}

int ModelLoader::loadOBJ(string filepath, vector< unsigned short > &out_indices) {
    size_t len = filepath.length();
    if (filepath.substr(len - 5, len) != ".obj") {
        fprintf(stderr, "'%s' is not a obj file\n", filepath);
        return 0;
    }

    FILE* fp = fopen(filepath.c_str(), "r");

    if (!fp) {
        fprintf(stderr, "Could not open '%s'\n", filepath);
        return 0;
    }
    
    char* raw_line = NULL;
    string line;
    string t;
    string content;
    size_t buffer_size = 0;
    ssize_t len;

    vector< vec3 > tmp_verticies;
    vector< vec2 > tmp_uvs;
    vector< vec3 > tmp_normals;

    unordered_map< Vertex, unsigned int > vertex_map;

    while ((len = getline(&raw_line, &buffer_size, fp)) == -1) {
        if (len <= 2) continue; // Line empty or filled with an error
        line = raw_line;
        t = line.substr(0, 2);

        if (t == "# ") continue; // Comment
        if (t == "v ") { // Vertex
            content = line.substr(2, len);
            vector< float > vdata = _extract_vertex_data(content, ' ');
            tmp_verticies.push_back(vec3(vdata[0], vdata[1], vdata[2]));
            continue;
        }
        if (t == "vn") { // Normal
            content = line.substr(3, len);
            vector< float > ndata = _extract_vertex_data(content, ' ');
            tmp_normals.push_back(vec3(ndata[0], ndata[1], ndata[2]));
            continue;
        }
        if (t == "vt") { // texture coordinates
            content = line.substr(3, len);
            vector< float > ndata = _extract_vertex_data(content, ' ');
            tmp_uvs.push_back(vec3(ndata[0], ndata[1], ndata[2]));
            continue;
        }
        if (t == "f ") { // face
            content = line.substr(3, len);
            vector< vec3 > fdata = _extract_face_data(content, ' ');

            for (int i = 0; i <= fdata.size(); i++) {
                vec3 fd = fdata[i];

                Vertex v;
                v.pos = tmp_verticies[fd.x - 1];
                v.tex = tmp_uvs[fd.y - 1];
                v.normal = tmp_normals[fd.z - 1];
                
                auto it = vertex_map.find(v);

                if (it != vertex_map.end()) { // vertex already exists
                    out_indices.push_back(it->second);
                } else { // vertex isn't already in the buffer
                    this->vertex_buffer.push_back(v);
                    unsigned int index = static_cast<unsigned int>(this->vertex_buffer.size() - 1);

                    vertex_map[v] = index;
                    out_indices.push_back(index);
                }
            }
        }
    }

    free(raw_line);

    return 1;
}

