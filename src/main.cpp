#include <iostream>

#include "glad.c"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "types.h"
#include "file.cpp"
#include "camera.cpp"
#include "shader.cpp"

float WIN_WIDTH = 800.0f;
float WIN_HEIGHT = 600.0f;

float delta_time = 0.0f;
bool firstMouse = true;
float lastX = 800.0f / 2.0f;
float lastY = 600.0f / 2.0f;

bool wire_frame;
bool print_fps;

#include "input.cpp"

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "WorldGenerator", nullptr, nullptr);
    if (window == nullptr)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf(("Failed to initialize GLAD\n"));
        return -1;
    }

    glViewport(0, 0, 800, 600);

    struct Camera cam{};
    cam.init();

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window, &cam);

    glEnable(GL_DEPTH_TEST);

    struct Shader basic{};
    basic.build("res/shaders/basic.vert", "res/shaders/basic.frag");

    float vertices[] =
    {
        // verts            // normals        // text UVs
        -0.5, -0.5, -0.5,   0.0,  0.0, -1.0,   0.0,  0.0,
        -0.5,  0.5, -0.5,   0.0,  0.0, -1.0,   0.0,  1.0,
         0.5,  0.5, -0.5,   0.0,  0.0, -1.0,   1.0,  1.0,
         0.5,  0.5, -0.5,   0.0,  0.0, -1.0,   1.0,  1.0,
         0.5, -0.5, -0.5,   0.0,  0.0, -1.0,   1.0,  0.0,
        -0.5, -0.5, -0.5,   0.0,  0.0, -1.0,   0.0,  0.0,
         0.5, -0.5,  0.5,   0.0,  0.0,  1.0,   0.0,  0.0,
         0.5,  0.5,  0.5,   0.0,  0.0,  1.0,   0.0,  1.0,
        -0.5,  0.5,  0.5,   0.0,  0.0,  1.0,   1.0,  1.0,
        -0.5,  0.5,  0.5,   0.0,  0.0,  1.0,   1.0,  1.0,
        -0.5, -0.5,  0.5,   0.0,  0.0,  1.0,   1.0,  0.0,
         0.5, -0.5,  0.5,   0.0,  0.0,  1.0,   0.0,  0.0,
        -0.5, -0.5,  0.5,  -1.0, -0.0, -0.0,   0.0,  0.0,
        -0.5,  0.5,  0.5,  -1.0, -0.0, -0.0,   0.0,  1.0,
        -0.5,  0.5, -0.5,  -1.0, -0.0, -0.0,   1.0,  1.0,
        -0.5,  0.5, -0.5,  -1.0, -0.0, -0.0,   1.0,  1.0,
        -0.5, -0.5, -0.5,  -1.0, -0.0, -0.0,   1.0,  0.0,
        -0.5, -0.5,  0.5,  -1.0, -0.0, -0.0,   0.0,  0.0,
         0.5, -0.5, -0.5,   1.0,  0.0, -0.0,   0.0,  0.0,
         0.5,  0.5, -0.5,   1.0,  0.0, -0.0,   0.0,  1.0,
         0.5,  0.5,  0.5,   1.0,  0.0, -0.0,   1.0,  1.0,
         0.5,  0.5,  0.5,   1.0,  0.0, -0.0,   1.0,  1.0,
         0.5, -0.5,  0.5,   1.0,  0.0, -0.0,   1.0,  0.0,
         0.5, -0.5, -0.5,   1.0,  0.0, -0.0,   0.0,  0.0,
        -0.5,  0.5, -0.5,  -0.0,  1.0,  0.0,   0.0,  0.0,
        -0.5,  0.5,  0.5,  -0.0,  1.0,  0.0,   0.0,  1.0,
         0.5,  0.5,  0.5,  -0.0,  1.0,  0.0,   1.0,  1.0,
         0.5,  0.5,  0.5,  -0.0,  1.0,  0.0,   1.0,  1.0,
         0.5,  0.5, -0.5,  -0.0,  1.0,  0.0,   1.0,  0.0,
        -0.5,  0.5, -0.5,  -0.0,  1.0,  0.0,   0.0,  0.0,
         0.5, -0.5, -0.5,  -0.0, -1.0, -0.0,   0.0,  0.0,
         0.5, -0.5,  0.5,  -0.0, -1.0, -0.0,   0.0,  1.0,
        -0.5, -0.5,  0.5,  -0.0, -1.0, -0.0,   1.0,  1.0,
        -0.5, -0.5,  0.5,  -0.0, -1.0, -0.0,   1.0,  1.0,
        -0.5, -0.5, -0.5,  -0.0, -1.0, -0.0,   1.0,  0.0,
         0.5, -0.5, -0.5,  -0.0, -1.0, -0.0,   0.0,  0.0
    };

    glm::vec3 cube_position[] =
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 0.0f, 0.0f),
        glm::vec3(3.0f, 0.0f, 0.0f),
        glm::vec3(4.0f, 0.0f, 0.0f),
        glm::vec3(5.0f, 0.0f, 0.0f),
        glm::vec3(6.0f, 0.0f, 0.0f),
        glm::vec3(7.0f, 0.0f, 0.0f),
        glm::vec3(8.0f, 0.0f, 0.0f),
        glm::vec3(9.0f, 0.0f, 0.0f),
        glm::vec3(10.0f, 0.0f, 0.0f),
        glm::vec3(11.0f, 0.0f, 0.0f),
        glm::vec3(12.0f, 0.0f, 0.0f),
        glm::vec3(13.0f, 0.0f, 0.0f),
        glm::vec3(14.0f, 0.0f, 0.0f),
        glm::vec3(15.0f, 0.0f, 0.0f),
        glm::vec3(16.0f, 0.0f, 0.0f),
        glm::vec3(17.0f, 0.0f, 0.0f),
        glm::vec3(18.0f, 0.0f, 0.0f),
        glm::vec3(19.0f, 0.0f, 0.0f)
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr);
//    glEnableVertexAttribArray(0);
//
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);

    u32 texture = load_texture("res/textures/stone.png");

    basic.use();
    basic.setInt("textureAtlas", 0);

    float last_frame = 0.0f;

    glEnable(GL_CULL_FACE);

    glfwSwapInterval(0);

    float frametime_total = 0.0;

    while (!glfwWindowShouldClose(window))
    {
        float current_frame = (float)glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        frametime_total += delta_time;
        if (frametime_total > 0.5 && print_fps)
        {
            printf("FPS: %f\n", 1.0 / delta_time);
            frametime_total = 0;
        }

        // input here :skull:
        process_input(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glm::mat4 projection = glm::perspective(glm::radians(cam.fov), WIN_WIDTH / WIN_HEIGHT, 0.1f, 100.0f);
        basic.setMat4("projection", projection);

        glm::mat4 view = glm::lookAt(cam.pos, cam.pos + cam.front, cam.up);
        basic.setMat4("view", view);

        glBindVertexArray(VAO);

        for (glm::vec3 &i : cube_position)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, i);
            basic.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
