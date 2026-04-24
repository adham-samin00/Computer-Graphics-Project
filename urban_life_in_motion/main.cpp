#include <windows.h>
#include <GL/glut.h>
#include <math.h>

float cloudMove = 0.0f;
float dayValue = 1.0f;
float targetDayValue = 1.0f;

// Sky
void drawSky() {
    float r = 0.05f + dayValue * 0.40f;
    float g = 0.07f + dayValue * 0.68f;
    float b = 0.18f + dayValue * 0.82f;

    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    glColor3f(r * 0.7f, g * 0.8f, b);
    glVertex2f(-1.0f, 0.20f);
    glVertex2f( 1.0f, 0.20f);
    glColor3f(r, g, b);
    glVertex2f( 1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(r + 0.10f, g + 0.10f, b);
    glVertex2f(-1.0f, -0.10f);
    glVertex2f( 1.0f, -0.10f);
    glColor3f(r, g, b);
    glVertex2f( 1.0f, 0.20f);
    glVertex2f(-1.0f, 0.20f);
    glEnd();
}

// Sun and moon
void drawSunMoon() {
    float sunY = 0.80f - (1.0f - dayValue) * 0.35f;

    if(dayValue > 0.05f) {
        glColor3f(1.0f, 0.82f, 0.0f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++) {
            float pi = 3.1416;
            float A = (i * 2 * pi) / 200;
            float r = 0.08;
            float x = 0.75f + r * cos(A);
            float y = sunY + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();

        glColor3f(1.0f, 0.90f, 0.20f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++) {
            float pi = 3.1416;
            float A = (i * 2 * pi) / 200;
            float r = 0.055;
            float x = 0.75f + r * cos(A);
            float y = sunY + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();

        glColor3f(1.0f, 0.97f, 0.45f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++) {
            float pi = 3.1416;
            float A = (i * 2 * pi) / 200;
            float r = 0.030;
            float x = 0.75f + r * cos(A);
            float y = sunY + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();

        //Rays
        glColor3f(1.0f, 0.85f, 0.15f);
        glBegin(GL_LINES);
        glVertex2f(0.75f, sunY + 0.10f); glVertex2f(0.75f, sunY + 0.16f);
        glVertex2f(0.75f, sunY - 0.10f); glVertex2f(0.75f, sunY - 0.16f);

        glVertex2f(0.65f, sunY); glVertex2f(0.59f, sunY);
        glVertex2f(0.85f, sunY); glVertex2f(0.91f, sunY);

        glVertex2f(0.68f, sunY + 0.07f); glVertex2f(0.63f, sunY + 0.12f);
        glVertex2f(0.82f, sunY + 0.07f); glVertex2f(0.87f, sunY + 0.12f);

        glVertex2f(0.68f, sunY - 0.07f); glVertex2f(0.63f, sunY - 0.12f);
        glVertex2f(0.82f, sunY - 0.07f); glVertex2f(0.87f, sunY - 0.12f);
        glEnd();
}


    if(dayValue < 0.60f) {
        glColor3f(0.95f, 0.95f, 1.0f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++) {
            float pi = 3.1416;
            float A = (i * 2 * pi) / 200;
            float r = 0.06;
            float x = -0.75f + r * cos(A);
            float y = 0.78f + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();

        glColor3f(0.05f, 0.07f, 0.18f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++) {
            float pi = 3.1416;
            float A = (i * 2 * pi) / 200;
            float r = 0.05;
            float x = -0.72f + r * cos(A);
            float y = 0.80f + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();
    }
}

// Stars
void drawStars() {
    if(dayValue > 0.40f) return;

    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(2.0f);

    glBegin(GL_POINTS);
    glVertex2f(-0.90f, 0.90f);
    glVertex2f(-0.70f, 0.82f);
    glVertex2f(-0.45f, 0.92f);
    glVertex2f(-0.20f, 0.80f);
    glVertex2f( 0.05f, 0.88f);
    glVertex2f( 0.30f, 0.78f);
    glVertex2f( 0.55f, 0.90f);
    glVertex2f( 0.80f, 0.84f);
    glEnd();
}

// Moving Clouds
void drawClouds() {
    glPushMatrix();
    glTranslatef(cloudMove, 0.0f, 0.0f);

    float c = 0.75f + dayValue * 0.25f;
    glColor3f(c, c, c);

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

    glPopMatrix();
}

// Temporary buildings
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
    drawStars();
    drawSunMoon();
    drawClouds();
    drawBuildings();
    drawGround();
    drawRoad();

    glFlush();
}

void update(int value) {
    cloudMove += 0.001f;

    if(cloudMove > 1.30f) {
        cloudMove = -1.30f;
    }

    if(dayValue < targetDayValue) {
        dayValue += 0.005f;
        if(dayValue > targetDayValue) {
            dayValue = targetDayValue;
        }
    }

    if(dayValue > targetDayValue) {
        dayValue -= 0.005f;
        if(dayValue < targetDayValue) {
            dayValue = targetDayValue;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(20, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if(key == 'd' || key == 'D') {
        targetDayValue = 1.0f;
    }

    if(key == 'n' || key == 'N') {
        targetDayValue = 0.0f;
    }

    glutPostRedisplay();
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
    glutCreateWindow("Urban Life in Motion - Phase 3 Samin");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(20, update, 0);
    glutMainLoop();

    return 0;
}
