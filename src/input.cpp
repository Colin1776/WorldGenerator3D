void process_input(GLFWwindow* window)
{
    auto cam = (struct Camera*)glfwGetWindowUserPointer(window);

    if (cam->disabled)
        return;

    float cam_speed = 2.5f * delta_time;

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cam_speed = 250.0f * delta_time;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam->move_forward(cam_speed);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam->move_forward(-cam_speed);


    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam->move_right(-cam_speed);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam->move_right(cam_speed);


    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam->move_up(cam_speed);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cam->move_up(-cam_speed);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    auto cam = (struct Camera*)glfwGetWindowUserPointer(window);

    if (cam->disabled)
    {
        firstMouse = true;
        return;
    }


    float xpos = (float)xposIn;
    float ypos = (float)yposIn;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cam->yaw += xoffset;
    cam->pitch += yoffset;

    if (cam->pitch > 89.0f)
        cam->pitch = 89.0f;

    if (cam->pitch < -89.0f)
        cam->pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(cam->yaw)) *cos(glm::radians(cam->pitch));
    front.y = sin(glm::radians(cam->pitch));
    front.z = sin(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));
    cam->front = glm::normalize(front);

    if (cam->yaw > 180.0f)
        cam->yaw -= 360.0f;
    if (cam->yaw < -180.0f)
        cam->yaw += 360.0f;

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    auto cam = (struct Camera*)glfwGetWindowUserPointer(window);

    cam->fov -= (float)yoffset;
    if (cam->fov < 1.0f)
        cam->fov = 1.0f;

    if (cam->fov > 90.0f)
        cam->fov = 90.0f;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto cam = (struct Camera*) glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        if (cam->disabled)
        {
            cam->disabled = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            cam->disabled = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    else if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        cam->print();
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        if (wire_frame)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        wire_frame = !wire_frame;
    }
    else if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        print_fps = !print_fps;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    WIN_WIDTH = width;
    WIN_HEIGHT = height;
    glViewport(0, 0, width, height);
}