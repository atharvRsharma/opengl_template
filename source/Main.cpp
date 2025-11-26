#include <iostream>
#include <array>

#include "Window.h"
#include "Shader.h"
#include "Mesh.h"



//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



int main()
{
    std::array<float, 12> verts = {
         0.5f,  0.5f, 0.0f,  //top right
         0.5f, -0.5f, 0.0f,  //bottom right
        -0.5f, -0.5f, 0.0f,  //bottom left
        -0.5f,  0.5f, 0.0f   //top left 
    };

    std::array<unsigned int, 6> idx = { 0, 1, 3, 1, 2, 3 };

    std::vector<float> v(verts.begin(), verts.end());
    std::vector<unsigned int> i(idx.begin(), idx.end());

    Window window(SCR_HEIGHT, SCR_WIDTH, "window");
    Shader shader("resources/default.vert", "resources/default.frag");
    Mesh quad(v, i, 3); 

    while (!window.shouldClose())
    {
        window.processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        quad.draw();

        window.swapBuffersAndPollEvents();
    }

    return 0;
}