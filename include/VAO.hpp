#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "VBO.hpp"

class VAO{
    private:
        GLuint id;
    public:
        VAO();
        void linkVBO(VBO vbo, GLuint posLayout);
        void linkVBO(VBO vbo, GLuint posLayout, GLuint normalLayout, GLuint texLayout);
        void linkVBO(VBO vbo, GLuint posLayout, GLuint colorLayout);
        void linkVBO(VBO vbo, GLuint posLayout, GLuint texLayout, bool texture);//bool is just for being able to overload
        void linkVBO(VBO vbo1, VBO vbo2, GLuint posLayout, GLuint texLayout, GLuint instanced1Layout, GLuint instanced2Layout, GLuint instanced3Layout, GLuint instanced4Layout);
        
        void bind();
        void unbind();
        void Delete();
};