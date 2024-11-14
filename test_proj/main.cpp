#define GLEW_STATIC

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <cstdlib>
#include <cmath>

using namespace std;

// Rocket position and rotation
float rocketY = -0.8f;
float rocketX = -0.9f;
float rocketRotation = 0.0f;
float cloudX = 0.5f;
float buildingScale = 1.0f;
float sunRotation = 0.0f;
bool isMousePressed = false;
bool isRightMousePressed = false;

// Vertex Buffer Object
GLuint vbo;

void drawBackground() {
    // Draw a simple background with gradient
    glBegin(GL_QUADS);
    // Sky
    glColor3f(0.2f, 0.6f, 1.0f); // Light blue
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glColor3f(0.0f, 0.0f, 0.5f); // Dark blue
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();
}

void drawRocket() {
    glPushMatrix();

    // Translate and rotate the rocket
    glTranslatef(rocketX, rocketY, 0.0f);
    glRotatef(rocketRotation, 0.0f, 0.0f, 1.0f);

    // Draw the rocket
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.05f, 0.0f);
    glVertex2f(0.05f, 0.0f);
    glVertex2f(0.0f, 0.2f);
    glEnd();

    glPopMatrix();
}

void drawCloud() {
    glPushMatrix();

    // Move the cloud
    glTranslatef(cloudX, 0.7f, 0.0f);

    // Draw the cloud
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = -2; i <= 2; i++) {
        glBegin(GL_POLYGON);
        for (int j = 0; j < 360; j += 30) {
            float angle = j * 3.14159f / 180.0f;
            glVertex2f(0.1f * cos(angle) + i * 0.1f, 0.05f * sin(angle));
        }
        glEnd();
    }

    glPopMatrix();
}

void drawBuilding() {
    glPushMatrix();

    // Scale the building
    glScalef(buildingScale, buildingScale, 1.0f);

    // Draw the building
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, -1.0f);
    glVertex2f(0.2f, -1.0f);
    glVertex2f(0.2f, -0.2f);
    glVertex2f(-0.2f, -0.2f);
    glEnd();

    // Windows
    glColor3f(0.0f, 0.0f, 0.0f);
    for (float x = -0.15f; x <= 0.15f; x += 0.1f) {
        for (float y = -0.9f; y <= -0.3f; y += 0.2f) {
            glBegin(GL_QUADS);
            glVertex2f(x - 0.03f, y - 0.03f);
            glVertex2f(x + 0.03f, y - 0.03f);
            glVertex2f(x + 0.03f, y + 0.03f);
            glVertex2f(x - 0.03f, y + 0.03f);
            glEnd();
        }
    }

    glPopMatrix();
}

void drawSun() {
    glPushMatrix();

    // Rotate the sun
    glTranslatef(-0.8f, 0.8f, 0.0f);
    glRotatef(sunRotation, 0.0f, 0.0f, 1.0f);

    // Draw the sun
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(0.1f * cos(angle), 0.1f * sin(angle));
    }
    glEnd();

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the background
    drawBackground();

    // Draw the rocket
    drawRocket();

    // Draw the cloud
    drawCloud();

    // Draw the building
    drawBuilding();

    // Draw the sun
    drawSun();

    glutSwapBuffers();
}

void update(int value) {
    if (isRightMousePressed) {
        rocketRotation += 5.0f; // Rotate when right mouse is pressed
        if (rocketRotation > 360.0f) rocketRotation -= 360.0f;
    }

    // Vertical movement
    if (isMousePressed) {
        rocketY += 0.02f; // Move up when left mouse is pressed
        if (rocketY > 0.8f) {
            rocketY = -0.8f; // Reset to bottom left when reaching the top
            rocketX = -0.9f;
        }
    } else {
        rocketY -= 0.01f; // Fall down when mouse is not pressed
        if (rocketY < -0.8f) rocketY = -0.8f; // Restrict rocket's lowest point
    }

    // Constant horizontal movement
    rocketX += 0.01f;
    if (rocketX > 1.0f) rocketX = -1.0f; // Wrap around horizontally

    // Move the cloud
    cloudX -= 0.01f;
    if (cloudX < -1.2f) cloudX = 1.2f;

    // Rotate the sun
    sunRotation += 1.0f;
    if (sunRotation > 360.0f) sunRotation -= 360.0f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // Approximately 60 FPS
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        isMousePressed = (state == GLUT_DOWN);
    }
    if (button == GLUT_RIGHT_BUTTON) {
        isRightMousePressed = (state == GLUT_DOWN);
    }
    if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
        buildingScale += 0.1f;
    } else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) {
        buildingScale -= 0.1f;
        if (buildingScale < 0.1f) buildingScale = 0.1f;
    }
}

void initVBO() {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float vertices[] = {
        -0.1f, -0.1f, 0.1f, -0.1f, 0.1f, 0.1f, -0.1f, 0.1f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void init() {
    glewExperimental = GL_TRUE;
    glewInit();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    initVBO();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Rocket Flight Game");

    glewExperimental = GL_TRUE;
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        cerr << "Error initializing GLEW: " << glewGetErrorString(glewStatus) << "\\n";
        return -1;
    }

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}


