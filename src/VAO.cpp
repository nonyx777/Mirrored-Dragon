#include "../include/VAO.hpp"
#include "../include/VBO.hpp"

VAO::VAO()
{
    glGenVertexArrays(1, &id);
}

void VAO::linkVBO(VBO vbo, GLuint posLayout)
{
    vbo.bind();

    glEnableVertexAttribArray(posLayout);
    glVertexAttribPointer(posLayout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void *)0);
    vbo.unbind();
}

void VAO::linkVBO(VBO vbo, GLuint posLayout, GLuint normalLayout, GLuint texLayout)
{
    vbo.bind();

    // position
    glVertexAttribPointer(posLayout, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)0);
    glEnableVertexAttribArray(posLayout);
    // normal
    glVertexAttribPointer(normalLayout, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(normalLayout);
    // texture coordinate
    glVertexAttribPointer(texLayout, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)(6 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(texLayout);

    vbo.unbind();
}
void VAO::linkVBO(VBO vbo, GLuint posLayout, GLuint colorLayout)
{
    vbo.bind();

    // position
    glVertexAttribPointer(posLayout, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void *)0);
    glEnableVertexAttribArray(posLayout);
    // color
    glVertexAttribPointer(colorLayout, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(colorLayout);

    vbo.unbind();
}

void VAO::linkVBO(VBO vbo, GLuint posLayout, GLuint texLayout, bool texture)
{
    vbo.bind();
    glEnableVertexAttribArray(posLayout);
    glVertexAttribPointer(posLayout, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void *)0);
    glEnableVertexAttribArray(texLayout);
    glVertexAttribPointer(texLayout, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void *)(3 * sizeof(float)));
    vbo.unbind();
}

void VAO::linkVBO(VBO vbo1, VBO vbo2, GLuint posLayout, GLuint texLayout, GLuint instanced1Layout, GLuint instanced2Layout, GLuint instanced3Layout, GLuint instanced4Layout)
{
    vbo1.bind();
    glEnableVertexAttribArray(posLayout);
    glVertexAttribPointer(posLayout, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void *)0);
    glEnableVertexAttribArray(texLayout);
    glVertexAttribPointer(texLayout, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void *)(3 * sizeof(float)));
    vbo1.unbind();
    vbo2.bind();
    glEnableVertexAttribArray(instanced1Layout);
    glVertexAttribPointer(instanced1Layout, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)0);
    glEnableVertexAttribArray(instanced2Layout);
    glVertexAttribPointer(instanced2Layout, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(instanced3Layout);
    glVertexAttribPointer(instanced3Layout, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(instanced4Layout);
    glVertexAttribPointer(instanced4Layout, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(3 * sizeof(glm::vec4)));
    vbo2.unbind();

    glVertexAttribDivisor(instanced1Layout, 1);
    glVertexAttribDivisor(instanced2Layout, 1);
    glVertexAttribDivisor(instanced3Layout, 1);
    glVertexAttribDivisor(instanced4Layout, 1);
}

void VAO::bind()
{
    glBindVertexArray(this->id);
}
void VAO::unbind()
{
    glBindVertexArray(0);
}
void VAO::Delete()
{
    glDeleteVertexArrays(1, &id);
}