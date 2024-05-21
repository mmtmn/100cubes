#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define M_PI 3.14159265358979323846

GLFWwindow* window;
bool keys[1024];
double lastX = WINDOW_WIDTH / 2, lastY = WINDOW_HEIGHT / 2;
float cameraX = 0, cameraY = 0, cameraZ = 20;
float cameraYaw = -90.0f, cameraPitch = 0.0f;

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
    float cameraSpeed = 0.2f;

    float yaw = cameraYaw * M_PI / 180.0f;
    float pitch = cameraPitch * M_PI / 180.0f;

    float frontX = cos(yaw) * cos(pitch);
    float frontY = sin(pitch);
    float frontZ = sin(yaw) * cos(pitch);

    if (keys[GLFW_KEY_W]) {
        cameraX += frontX * cameraSpeed;
        cameraY += frontY * cameraSpeed;
        cameraZ += frontZ * cameraSpeed;
    }
    if (keys[GLFW_KEY_S]) {
        cameraX -= frontX * cameraSpeed;
        cameraY -= frontY * cameraSpeed;
        cameraZ -= frontZ * cameraSpeed;
    }
    if (keys[GLFW_KEY_A]) {
        float rightX = sin(yaw);
        float rightZ = -cos(yaw);
        cameraX += rightX * cameraSpeed;
        cameraZ += rightZ * cameraSpeed;
    }
    if (keys[GLFW_KEY_D]) {
        float rightX = sin(yaw);
        float rightZ = -cos(yaw);
        cameraX -= rightX * cameraSpeed;
        cameraZ -= rightZ * cameraSpeed;
    }
    if (keys[GLFW_KEY_Q]) {
        cameraY -= cameraSpeed;
    }
    if (keys[GLFW_KEY_E]) {
        cameraY += cameraSpeed;
    }
}

void draw_cube(float r, float g, float b) {
    glColor3f(r, g, b);
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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float yaw = cameraYaw * M_PI / 180.0f;
    float pitch = cameraPitch * M_PI / 180.0f;

    float targetX = cameraX + cos(yaw) * cos(pitch);
    float targetY = cameraY + sin(pitch);
    float targetZ = cameraZ + sin(yaw) * cos(pitch);

    gluLookAt(cameraX, cameraY, cameraZ, targetX, targetY, targetZ, 0.0f, 1.0f, 0.0f);

    // Draw cubes with different colors based on depth
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            for (int k = 0; k < 30; k++) {
                float r = (float)i / 20.0f;
                float g = (float)j / 20.0f;
                float b = (float)k / 20.0f;

                glPushMatrix();
                glTranslatef(i - 9.5, j - 9.5, k - 9.5);
                glScalef(0.5, 0.5, 0.5);
                draw_cube(r, g, b);
                glPopMatrix();
            }
        }
    }

    // Draw a grid on the XZ plane
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    for (int i = -10; i <= 10; i++) {
        glVertex3f(i, 0.0f, -10.0f);
        glVertex3f(i, 0.0f, 10.0f);
        glVertex3f(-10.0f, 0.0f, i);
        glVertex3f(10.0f, 0.0f, i);
    }
    glEnd();

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