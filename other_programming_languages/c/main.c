#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

GLFWwindow* window;
bool keys[1024];
double lastX = WINDOW_WIDTH / 2, lastY = WINDOW_HEIGHT / 2;
float cameraX = 0, cameraY = 0, cameraZ = 5;
float cameraYaw = 0, cameraPitch = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cameraYaw += xoffset;
    cameraPitch += yoffset;

    if (cameraPitch > 89.0f) {
        cameraPitch = 89.0f;
    } else if (cameraPitch < -89.0f) {
        cameraPitch = -89.0f;
    }
}

void move_camera() {
    float cameraSpeed = 0.1f;
    float cameraSensitivity = 0.1f;

    if (keys[GLFW_KEY_W]) {
        cameraX += cameraSpeed * sin(cameraYaw * M_PI / 180.0);
        cameraZ -= cameraSpeed * cos(cameraYaw * M_PI / 180.0);
    }
    if (keys[GLFW_KEY_S]) {
        cameraX -= cameraSpeed * sin(cameraYaw * M_PI / 180.0);
        cameraZ += cameraSpeed * cos(cameraYaw * M_PI / 180.0);
    }
    if (keys[GLFW_KEY_A]) {
        cameraX -= cameraSpeed * cos(cameraYaw * M_PI / 180.0);
        cameraZ -= cameraSpeed * sin(cameraYaw * M_PI / 180.0);
    }
    if (keys[GLFW_KEY_D]) {
        cameraX += cameraSpeed * cos(cameraYaw * M_PI / 180.0);
        cameraZ += cameraSpeed * sin(cameraYaw * M_PI / 180.0);
    }
    if (keys[GLFW_KEY_Q]) {
        cameraY -= cameraSpeed;
    }
    if (keys[GLFW_KEY_E]) {
        cameraY += cameraSpeed;
    }
}

void draw_cube() {
    glBegin(GL_QUADS);
    // Front face
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // Back face
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    // Left face
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // Right face
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    // Top face
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    // Bottom face
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set up camera
    glRotatef(-cameraPitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-cameraYaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-cameraX, -cameraY, -cameraZ);

    // Draw cubes
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            for (int k = 0; k < 20; k++) {
                glPushMatrix();
                glTranslatef(i - 4.5, j - 4.5, k);
                glScalef(0.5, 0.5, 0.5);
                draw_cube();
                glPopMatrix();
            }
        }
    }

    glfwSwapBuffers(window);
}

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3D Cubes", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        move_camera();
        display();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}