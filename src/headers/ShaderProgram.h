#include <glad/glad.h>  // Manages function pointers
#include <glfw3.h>      // Manages window
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
// TODO: Derived classes!!
// Combine Vertex and Fragment into one.


class ShaderProgram {
private:
    char* shaderData;
    unsigned int shaderProgramID;
    bool loaded;
public:
    // Detach shader and recompile program?
    //void loadVertexShader(std::string vertexShader);
    //void loadFragmentShader(std::string fragShader);
    
    // Compile shaders and link
    //void loadShaders(std::string vertexShaderPath, std::string fragShaderPath);
    void compileCheck(unsigned int vertexShader, bool vertex);
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    //ShaderProgram(std::string vertexShaderPath, std::string fragShaderPath);
    ShaderProgram(const char* vertexPath, const char* fragPath);
    ShaderProgram();
    
};