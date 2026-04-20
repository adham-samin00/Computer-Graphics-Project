#include <windows.h>
#include <GL/glut.h>
#include <math.h>

// Sky
void drawSky() {
    glClearColor(0.45f, 0.75f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    glBegin(GL_QUADS);
    glColor3f(0.30f, 0.60f, 0.92f);
    glVertex2f(-1.0f, 0.20f);
    glVertex2f( 1.0f, 0.20f);
    glColor3f(0.45f, 0.75f, 1.0f);
    glVertex2f( 1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();


    glBegin(GL_QUADS);
    glColor3f(0.65f, 0.85f, 1.0f);
    glVertex2f(-1.0f, -0.10f);
    glVertex2f( 1.0f, -0.10f);
    glColor3f(0.50f, 0.78f, 1.0f);
    glVertex2f( 1.0f, 0.20f);
    glVertex2f(-1.0f, 0.20f);
    glEnd();
}

// Sun
void drawSun() {
    // Outer layer
    glColor3f(1.0f, 0.82f, 0.0f);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 200;
        float r = 0.08;
        float x = 0.75f + r * cos(A);
        float y = 0.80f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    // Middle layer
    glColor3f(1.0f, 0.90f, 0.20f);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 200;
        float r = 0.055;
        float x = 0.75f + r * cos(A);
        float y = 0.80f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    // Inner layer
    glColor3f(1.0f, 0.97f, 0.45f);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 200;
        float r = 0.030;
        float x = 0.75f + r * cos(A);
        float y = 0.80f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    // Rays
    glColor3f(1.0f, 0.85f, 0.15f);
    glBegin(GL_LINES);
    glVertex2f(0.75f, 0.90f); glVertex2f(0.75f, 0.96f);
    glVertex2f(0.75f, 0.70f); glVertex2f(0.75f, 0.64f);

    glVertex2f(0.65f, 0.80f); glVertex2f(0.59f, 0.80f);
    glVertex2f(0.85f, 0.80f); glVertex2f(0.91f, 0.80f);

    glVertex2f(0.68f, 0.87f); glVertex2f(0.63f, 0.92f);
    glVertex2f(0.82f, 0.87f); glVertex2f(0.87f, 0.92f);

    glVertex2f(0.68f, 0.73f); glVertex2f(0.63f, 0.68f);
    glVertex2f(0.82f, 0.73f); glVertex2f(0.87f, 0.68f);
    glEnd();
}

// Clouds
void drawClouds() {
    glColor3f(1.0f, 1.0f, 1.0f);

    // Cloud 1
    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 200;
        float r = 0.05;
        float x = -0.75f + r * cos(A);
        float y = 0.78f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 200;
        float r = 0.06;
        float x = -0.70f + r * cos(A);
        float y = 0.80f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 200;
        float r = 0.05;
        float x = -0.64f + r * cos(A);
        float y = 0.78f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 200;
        float r = 0.04;
        float x = -0.69f + r * cos(A);
        float y = 0.75f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    // Cloud 2
    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 200;
        float r = 0.05;
        float x = -0.15f + r * cos(A);
        float y = 0.70f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 200;
        float r = 0.06;
        float x = -0.10f + r * cos(A);
        float y = 0.72f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 200;
        float r = 0.05;
        float x = -0.04f + r * cos(A);
        float y = 0.70f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 200;
        float r = 0.04;
        float x = -0.09f + r * cos(A);
        float y = 0.67f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    // Cloud 3
    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 200;
        float r = 0.05;
        float x = 0.35f + r * cos(A);
        float y = 0.76f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 200;
        float r = 0.06;
        float x = 0.40f + r * cos(A);
        float y = 0.78f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 200;
        float r = 0.05;
        float x = 0.46f + r * cos(A);
        float y = 0.76f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 200;
        float r = 0.04;
        float x = 0.40f + r * cos(A);
        float y = 0.73f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();
}


void drawBuildings() {
    glColor3f(0.35f, 0.36f, 0.42f);
    glBegin(GL_QUADS);
    glVertex2f(-0.95f, -0.10f);
    glVertex2f(-0.75f, -0.10f);
    glVertex2f(-0.75f, 0.35f);
    glVertex2f(-0.95f, 0.35f);
    glEnd();

    glColor3f(0.38f, 0.30f, 0.38f);
    glBegin(GL_QUADS);
    glVertex2f(-0.25f, -0.10f);
    glVertex2f(0.00f, -0.10f);
    glVertex2f(0.00f, 0.40f);
    glVertex2f(-0.25f, 0.40f);
    glEnd();

    glColor3f(0.28f, 0.30f, 0.36f);
    glBegin(GL_QUADS);
    glVertex2f(0.10f, -0.10f);
    glVertex2f(0.35f, -0.10f);
    glVertex2f(0.35f, 0.55f);
    glVertex2f(0.10f, 0.55f);
    glEnd();
}

void drawGround() {
    glColor3f(0.10f, 0.50f, 0.16f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.10f);
    glVertex2f( 1.0f, -0.10f);
    glVertex2f( 1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();
}

void drawRoad() {
    glColor3f(0.55f, 0.55f, 0.55f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.18f);
    glVertex2f( 1.0f, -0.18f);
    glVertex2f( 1.0f, -0.28f);
    glVertex2f(-1.0f, -0.28f);
    glEnd();

    glColor3f(0.13f, 0.13f, 0.14f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.28f);
    glVertex2f( 1.0f, -0.28f);
    glVertex2f( 1.0f, -0.95f);
    glVertex2f(-1.0f, -0.95f);
    glEnd();
}


void display() {
    drawSky();
    drawSun();
    drawClouds();
    drawBuildings();
    drawGround();
    drawRoad();

    glFlush();
}


void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(40, 30);
    glutCreateWindow("Urban Life in Motion");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
