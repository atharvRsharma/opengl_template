#pragma once
#include <cstdint>
#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window(uint32_t height, uint32_t width, const char* title)
        : h(height), w(width), title(title), window(nullptr)
    {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        window = glfwCreateWindow(static_cast<int>(w), static_cast<int>(h), title, nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwSetWindowUserPointer(window, this);

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            glfwDestroyWindow(window);
            glfwTerminate();
            throw std::runtime_error("Failed to initialize GLAD");
        }

        int fbw, fbh;
        glfwGetFramebufferSize(window, &fbw, &fbh);
        glViewport(0, 0, fbw, fbh);

        glfwSetFramebufferSizeCallback(window, Window::framebuffer_size_callback_static);
    }

    ~Window() {
        if (window) {
            glfwDestroyWindow(window);
            window = nullptr;
        }
        glfwTerminate();
    }

    //singleton
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    GLFWwindow* getWindow() const { return window; }
    bool shouldClose() const { return window && glfwWindowShouldClose(window); }

    void processInput() {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    void swapBuffersAndPollEvents() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

private:
    void framebuffer_size_callback(int width, int height) {
        glViewport(0, 0, width, height);
    }

    static void framebuffer_size_callback_static(GLFWwindow* win, int width, int height) {
        Window* self = static_cast<Window*>(glfwGetWindowUserPointer(win));
        if (self) self->framebuffer_size_callback(width, height);
    }

private:
    const uint32_t h;       
    const uint32_t w;
    const char* title;
    GLFWwindow* window;
};

