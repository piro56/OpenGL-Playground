#include "ShaderProgram.h"


ShaderProgram::ShaderProgram(std::string vertex_path, std::string fragment_path) {
    this->load(vertex_path.c_str(), fragment_path.c_str());
}
ShaderProgram::ShaderProgram(std::string srcPath) {
    this->load(srcPath.c_str());
}
// Inspired by LearnOpenGL.com
ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragPath) {
    this->load(vertexPath, fragPath);
}
ShaderProgram::ShaderProgram() {

}


void ShaderProgram::use() {
    glUseProgram(this->shaderProgramID);
}


//void ShaderProgram::loadShaders(std::string vertexShaderPath, std::string fragShaderPath) {}

void ShaderProgram::compileCheck(unsigned int shader, bool vertex) {
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if(vertex) {
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        } else {
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

    }
}

void ShaderProgram::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(this->shaderProgramID, name.c_str()), (int) value);
}

void ShaderProgram::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(this->shaderProgramID, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(this->shaderProgramID, name.c_str()), value);
}

void ShaderProgram::setVec4Float(const std::string &name, float value_one, float value_two, float value_three, float value_four) const {
    glUniform4f(glGetUniformLocation(this->shaderProgramID, name.c_str()), value_one, value_two, value_three, value_four);
}

void ShaderProgram::setVec2Float(const std::string &name, float value_one, float value_two) const {
    glUniform2f(glGetUniformLocation(this->shaderProgramID, name.c_str()), value_one, value_two);
}
void ShaderProgram::setVec3Float(const std::string &name, float value_one, float value_two, float value_three) const {
    glUniform3f(glGetUniformLocation(this->shaderProgramID, name.c_str()), value_one, value_two, value_three);
}

void ShaderProgram::setMatrix4f(const std::string &name, GLboolean transpose, const GLfloat* value) const {
    glUniformMatrix4fv(glGetUniformLocation(this->shaderProgramID, name.c_str()), 1, transpose, value);
}
void ShaderProgram::load(const char* vertexPath, const char* fragPath) {
    unsigned int vertexShader;
    unsigned int fragmentShader;
    std::string vertexCode;
    std::string fragCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    loaded = true;
    // Ensure ifstream can throw exceptions
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    vShaderFile = std::ifstream(vertexPath);
    fShaderFile = std::ifstream(fragPath);
    if(vShaderFile.is_open() && fShaderFile.is_open()) {
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragCode = fShaderStream.str();
    } else {
        std::cout << "ERROR::SHADERPROGRAM::FILE_READ_ERROR......" << std::endl;
        loaded = false;
    }
    //std::cout << "-------------Fragment---------------\n" << fragCode << "\n--------\n"; 
    //std::cout << "-------------Vertex---------------\n" << vertexCode << "\n--------\n"; 

/*     try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragCode);
        std::stringstream vShaderStream, fShaderStream;
        // read file buffer into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close files
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADERPROGRAM::FILE_READ_ERROR..." << std::endl;
    } */

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragCode.c_str();
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);

    // COMPILE
    glCompileShader(vertexShader);
    compileCheck(vertexShader, true);
    glCompileShader(fragmentShader);
    compileCheck(fragmentShader, false);

    // GENERATE PROGRAM
    this->shaderProgramID = glCreateProgram();
    // ATTACH
    glAttachShader(this->shaderProgramID, vertexShader);
    glAttachShader(this->shaderProgramID, fragmentShader);
    glLinkProgram(this->shaderProgramID);
    // Check Program Link Errors
    int success;
    char infoLog[512];
    glGetProgramiv(this->shaderProgramID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(this->shaderProgramID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADERPROGRAM::LINKING_FAILURE\n" << infoLog << std::endl;
        loaded = false;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
// LOADS COMPUTE SHADER
void ShaderProgram::load(const char* srcPath) {
    unsigned int srcShader;
    std::string srcCode;
    std::ifstream sShaderFile;
    loaded = true;
    // Ensure ifstream can throw exceptions
    sShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    sShaderFile = std::ifstream(srcPath);
    if(sShaderFile.is_open()) {
        std::stringstream sShaderStream;
        sShaderStream << sShaderFile.rdbuf();
        sShaderFile.close();
        srcCode = sShaderStream.str();
    } else {
        std::cout << "ERROR::SHADERPROGRAM::FILE_READ_ERROR......" << std::endl;
        loaded = false;
    }
    //std::cout << "-------------Fragment---------------\n" << fragCode << "\n--------\n"; 
    //std::cout << "-------------Vertex---------------\n" << vertexCode << "\n--------\n"; 

/*     try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragCode);
        std::stringstream vShaderStream, fShaderStream;
        // read file buffer into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close files
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADERPROGRAM::FILE_READ_ERROR..." << std::endl;
    } */

    const char* sShaderCode = srcCode.c_str();
    srcShader = glCreateShader(GL_COMPUTE_SHADER);

    glShaderSource(srcShader, 1, &sShaderCode, NULL);

    // COMPILE
    glCompileShader(srcShader);
    compileCheck(srcShader, true);

    // GENERATE PROGRAM
    this->shaderProgramID = glCreateProgram();
    // ATTACH
    glAttachShader(this->shaderProgramID, srcShader);
    glLinkProgram(this->shaderProgramID);
    // Check Program Link Errors
    int success;
    char infoLog[512];
    glGetProgramiv(this->shaderProgramID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(this->shaderProgramID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADERPROGRAM::LINKING_FAILURE\n" << infoLog << std::endl;
        loaded = false;
    }
    glDeleteShader(srcShader);
}

std::string ShaderProgram::get_shader_file(std::string fileName) {
    std::string s = ShaderProgram::getexepath();
    s = s.substr(0, s.find("builds"));
    s = s + "src\\shaders\\" + fileName;
    //std::cout << "OUTPUT S:" << s << "\n\n";
    return s;
}

void ShaderProgram::loadCompute(const char* path) {

}