#pragma once
#include <glad/glad.h>
#include "VBO.hpp"

class VAO{
    private:
        GLuint id;
    public:
        VAO();
        void linkVBO(VBO vbo, GLuint posLayout, GLuint normalLayout, GLuint texLayout);
        void linkVBO(VBO vbo, GLuint posLayout, GLuint colorLayout);
        void linkVBO(VBO vbo, GLuint posLayout, GLuint texLayout, bool texture);//bool is just for being able to overload
        
        void bind();
        void unbind();
        void Delete();
};