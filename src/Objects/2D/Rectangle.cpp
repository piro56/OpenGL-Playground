#include <Rectangle.h>
using namespace shp;
Rectangle::Rectangle() :
x{0.0f}, y{0.0f}, xLength{0.1f}, yLength{0.1f}
{
    randomColor();
    initializePoints();
}
Rectangle::Rectangle(float xLength, float yLength, float x, float y) 
    : xLength{xLength}, yLength{yLength}, x{x}, y{y}
{
    randomColor();
    initializePoints();
}

Rectangle::Rectangle(float xLength, float yLength, ShaderProgram* sp) 
    : xLength{xLength}, yLength{yLength}, x{0}, y{0}
{
    this->shaderProgram = sp;
    sp->use();
    randomColor();
    initializePoints();
}

void Rectangle::randomColor() {
    r = (rand() % 255) / 255.0f;
    g = (rand() % 255) / 255.0f;
    b = (rand() % 255) / 255.0f;
    transUpdated = true;
}

void Rectangle::setColor(int r, int g, int b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

void Rectangle::initializePoints() {
    float halfX = xLength / 2.0f;
    float halfY = yLength / 2.0f;
    // we translate this later.
    float vertices[12] = { halfX,  halfY, 0.0f, // TR
                           halfX, -halfY, 0.0f, // BR
                          -halfX, -halfY, 0.0f, // BL 
                          -halfX,  halfY, 0.0f, // TL
                         };
    float colors[12] = {
        r, g, b, r, g, b, r, g, b, r, g, b
    };

    //    float indices[6] = {3, 0, 1, 3, 2, 0};
    this->vao.bind();
    this->vbo.bind();
    this->vbo.setBufferData(sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);    
    this->veb.bind();
    this->veb.setData(sizeof(indices), indices, GL_DYNAMIC_DRAW);
    this->vbo.setVertexAttributePointer(0, 3, GL_FLOAT, 3 * sizeof(float));
    this->vbo.enableAttribArray(0);
}
void Rectangle::scale(float scaleX, float scaleY, float scaleZ) {
    if (this->scaleX != scaleX || this->scaleY != scaleY || this->scaleZ != scaleZ) {
        this->scaleX = scaleX;
        this->scaleY = scaleY;
        this->scaleZ = scaleZ;
        this->transUpdated = true;
    }
}
void Rectangle::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(x, y, 0.0f));
    this->transUpdated = true;
}
void Rectangle::setRotation(float rotation) {
    this->rotation = rotation;
    this->transUpdated = true;
}
void Rectangle::draw() {
    this->preDraw();
    vao.bind();
    vbo.bind();
    veb.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Rectangle::preDraw() {
    this->shaderProgram->use();
    if (transUpdated) {
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(x, y, 0.0f));
        transform = glm::rotate(transform, rotation, glm::vec3(0.0,0.0,1.0));
        transform = glm::scale(transform, glm::vec3(scaleX, scaleY, scaleZ));
        this->shaderProgram->setVec3Float("selectedColor", r, g, b);
        this->shaderProgram->setMatrix4f("rectTrans", GL_FALSE, glm::value_ptr(transform));
    }
    transUpdated = false;
    #ifdef DEBUG
    GLenum err = glGetError();
    if (err != 0) {
        std::cout << "[GL_ERROR]: " <<  err << "\n";
    }
    #endif
}

void Rectangle::setShader(ShaderProgram* shaderProgram) {
     {
        this->shaderProgram = shaderProgram;
        this->shaderProgram->use();
        preDraw();
    }    
}

void Rectangle::setColor(float r, float g, float b) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->shaderProgram->use();
    this->shaderProgram->setVec3Float("selectedColor", r, g, b);
}