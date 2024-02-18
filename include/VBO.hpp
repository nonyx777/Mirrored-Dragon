#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

class VBO{
    private:
        GLuint id;
    public:
        VBO(GLfloat* vertices, GLuint size);
        VBO(glm::mat4* vertices, GLuint size);

        void bind();
        void unbind();
        void Delete();
};