#include <iostream>
#include <GL/glut.h>
#include <vector>

// Define camera position and movement speed
float cameraX = 0.0f, cameraY = -20.0f, cameraZ = 5.0f;
float cameraSpeed = 0.1f;

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
}

void createCube(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glutWireCube(1.0);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Create a 20x20x20 grid of cubes
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            for (int k = 0; k < 20; ++k) {
                createCube(i - 4.5f, j - 4.5f, k);
            }
        }
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            cameraY += cameraSpeed;
            break;
        case 's':
            cameraY -= cameraSpeed;
            break;
        case 'a':
            cameraX -= cameraSpeed;
            break;
        case 'd':
            cameraX += cameraSpeed;
            break;
        case 'q':
            cameraZ -= cameraSpeed;
            break;
        case 'e':
            cameraZ += cameraSpeed;
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Cube Grid");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}
