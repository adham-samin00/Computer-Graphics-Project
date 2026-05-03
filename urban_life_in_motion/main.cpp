#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")


float cloudMove = 0.0f;
float dayValue = 1.0f;
float targetDayValue = 1.0f;

int rainOn = 0;
float rainIntensity = 0.0f;
int rainSoundPlaying = 0;


int windDirection = 0;
float windWave = 0.0f;


int showStartScreen = 1;

const int RAIN_COUNT = 250;
float rainX[RAIN_COUNT];
float rainY[RAIN_COUNT];
float rainSpeed[RAIN_COUNT];


char lightState = 'G';
int running = 1;

float normalSpeed = 0.008f;
float slowSpeed = 0.003f;

//int citySoundPlaying = 0;

float car1X = -1.00f;
float car2X = -0.40f;
float car3X = 0.80f;
float car4X = 0.20f;
float car5X = -1.30f;
float car6X = -0.85f;
float car7X = 1.05f;
float car8X = 0.55f;

const float carWidth = 0.26f;
const float carGap = 0.10f;
const float wrapRight = 1.30f;
const float wrapLeft = -1.50f;
const float zebraLeft = -0.06f;
const float zebraRight = 0.06f;
const float rightStopX = zebraLeft - 0.03f;
const float leftStopX = zebraRight + 0.03f;

// Function prototypes
void drawText(float x, float y, const char *text);
void drawStartScreen();
void initRain();
void drawSky();
void drawStars();
void drawSunMoon();
void drawClouds();
void drawRainClouds();
void drawRain();
void drawBuildings();
void drawGround();
void drawWindEffect();
float getWindBend();
void drawTrees();
void drawLampPosts();
void drawFlags();
void drawRoad();
void drawTrafficLight();
void drawSingleCar(float carX, float y, float r, float g, float b, int direction);
void drawCars();
void display();
void update(int value);
void handleKeypress(unsigned char key, int x, int y);
void init();
void drawCircle(float cx, float cy, float r);
void drawZebraLane(float topY, float bottomY);
void moveRightLane(float *frontCar, float *backCar, float speed);
void moveLeftLane(float *frontCar, float *backCar, float speed);
float getGreenOrYellowSpeed();


// Draw Text
void drawText(float x, float y, const char *text) {
    glRasterPos2f(x, y);

    for(int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

// start scree
void drawStartScreen() {
    glClearColor(0.05f, 0.07f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Border
    glColor3f(0.90f, 0.90f, 0.90f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.90f, -0.90f);
    glVertex2f( 0.90f, -0.90f);
    glVertex2f( 0.90f,  0.90f);
    glVertex2f(-0.90f,  0.90f);
    glEnd();


    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(-0.30f, 0.78f, "American International University - Bangladesh");

    glColor3f(0.90f, 0.90f, 0.30f);
    drawText(-0.16f, 0.68f, "Computer Graphics");

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(-0.08f, 0.58f, "Section: L");


    glColor3f(0.20f, 0.90f, 1.0f);
    drawText(-0.28f, 0.42f, "PROJECT ON: URBAN LIFE IN MOTION");


    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(-0.35f, 0.22f, "Adham Samin [ID: 23-53329-3]");
    drawText(-0.35f, 0.12f, "Topu Roy [ID: 23-53318-3]");
    drawText(-0.35f, 0.02f, "Al Rahit [ID: 23-53316-3]");
    drawText(-0.35f,-0.08f, "Utsho Karmakar [ID: 23-53327-3]");


    glColor3f(0.90f, 0.90f, 0.30f);
    drawText(-0.18f, -0.25f, "Animation Controls");

    glColor3f(0.85f, 0.85f, 0.85f);
    drawText(-0.55f, -0.35f, "Press R, Y, G : Control traffic light");
    drawText(-0.55f, -0.43f, "Press D       : Day mode");
    drawText(-0.55f, -0.51f, "Press N       : Night mode");
    drawText(-0.55f, -0.59f, "Press T       : Rain on/off");
    drawText(-0.55f, -0.67f, "Press L       : Wind left to right");
    drawText(-0.55f, -0.75f, "Press K       : Wind right to left");
    drawText(-0.55f, -0.83f, "Press O       : Stop wind");


    glColor3f(0.20f, 1.0f, 0.30f);
    drawText(-0.20f, -0.95f, "Press S to START");

    glFlush();
}

// Rain initialization
void initRain() {
    for(int i = 0; i < RAIN_COUNT; i++) {
        rainX[i] = -1.0f + (rand() % 200) / 100.0f;
        rainY[i] = -1.0f + (rand() % 200) / 100.0f;
        rainSpeed[i] = 0.025f + (rand() % 100) / 5000.0f;
    }
}

//Rain sound
void startRainSound() {
    if (!rainSoundPlaying) {
        PlaySound(TEXT("C:/Spring 25-26/Computer Graphics/FINAL/Project/urban_life_in_motion/boons_freak-rain-sound-188158.wav"), NULL, SND_ASYNC | SND_LOOP | SND_NODEFAULT);
        rainSoundPlaying = 1;
    }
}

void stopRainSound() {
    if (rainSoundPlaying) {
        PlaySound(NULL, NULL, 0);   // stops any currently playing sound
        rainSoundPlaying = 0;
    }
}

//void startCitySound() {
//    if (!citySoundPlaying) {
//        if (!PlaySound(TEXT("C:/Spring 25-26/Computer Graphics/FINAL/Project/urban_life_in_motion/baranova_n-city-birds-444667.wav"), NULL, SND_ASYNC | SND_LOOP | SND_NODEFAULT)) {
//            // If file not found, fallback to a simple beep or ignore
//            // MessageBeep(MB_ICONASTERISK);
//        }
//        citySoundPlaying = 1;
//    }
//}
//
//void stopCitySound() {
//    if (citySoundPlaying) {
//        PlaySound(NULL, NULL, 0);
//        citySoundPlaying = 0;
//    }
//}

// sky with gradient
void drawSky() {
    float r = 0.05f + dayValue * 0.40f;
    float g = 0.07f + dayValue * 0.68f;
    float b = 0.18f + dayValue * 0.82f;

    if(rainIntensity > 0.0f) {
        r = r * (1.0f - rainIntensity) + 0.35f * rainIntensity;
        g = g * (1.0f - rainIntensity) + 0.38f * rainIntensity;
        b = b * (1.0f - rainIntensity) + 0.42f * rainIntensity;
    }

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
    glColor3f(r + 0.05f, g + 0.05f, b + 0.05f);
    glVertex2f(-1.0f, -0.10f);
    glVertex2f( 1.0f, -0.10f);
    glColor3f(r, g, b);
    glVertex2f( 1.0f, 0.20f);
    glVertex2f(-1.0f, 0.20f);
    glEnd();
}

// sun/moon with day/night
void drawSunMoon() {
    float sunY = 0.80f - (1.0f - dayValue) * 0.55f;

    if(dayValue > 0.05f && rainIntensity < 0.55f) {
        glColor3f(1.0f, 0.82f, 0.0f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++) {
            float pi = 3.1416f;
            float A = (i * 2 * pi) / 200;
            float r = 0.08f;
            float x = 0.75f + r * cos(A);
            float y = sunY + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();

        glColor3f(1.0f, 0.90f, 0.20f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++) {
            float pi = 3.1416f;
            float A = (i * 2 * pi) / 200;
            float r = 0.055f;
            float x = 0.75f + r * cos(A);
            float y = sunY + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();

        glColor3f(1.0f, 0.97f, 0.45f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++) {
            float pi = 3.1416f;
            float A = (i * 2 * pi) / 200;
            float r = 0.030f;
            float x = 0.75f + r * cos(A);
            float y = sunY + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();

        // sun rays
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

    if(dayValue < 0.30f && rainIntensity < 0.85f) {
        glColor3f(0.95f, 0.95f, 1.0f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++) {
            float pi = 3.1416f;
            float A = (i * 2 * pi) / 200;
            float r = 0.06f;
            float x = -0.75f + r * cos(A);
            float y = 0.78f + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();

        glColor3f(0.05f, 0.07f, 0.18f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++) {
            float pi = 3.1416f;
            float A = (i * 2 * pi) / 200;
            float r = 0.05f;
            float x = -0.72f + r * cos(A);
            float y = 0.80f + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();
    }
}

// stars
void drawStars() {
    if(dayValue > 0.00f || rainIntensity > 0.0f) return;

    glColor3f(1.0f, 1.0f, 1.0f);
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

// moving clouds
void drawClouds() {
    glPushMatrix();
    glTranslatef(cloudMove, 0.0f, 0.0f);

    float c = 0.75f + dayValue * 0.25f;
    c = c * (1.0f - rainIntensity) + 0.28f * rainIntensity;
    glColor3f(c, c, c);

    // Cloud 1
    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.05f + rainIntensity * 0.015f;
        float x = -0.75f + r * cos(A);
        float y = 0.78f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.06f + rainIntensity * 0.015f;
        float x = -0.70f + r * cos(A);
        float y = 0.80f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.05f + rainIntensity * 0.015f;
        float x = -0.64f + r * cos(A);
        float y = 0.78f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    // Cloud 2
    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.05f + rainIntensity * 0.015f;
        float x = -0.15f + r * cos(A);
        float y = 0.70f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.06f + rainIntensity * 0.015f;
        float x = -0.10f + r * cos(A);
        float y = 0.72f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.05f + rainIntensity * 0.015f;
        float x = -0.04f + r * cos(A);
        float y = 0.70f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    // Cloud 3
    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.05f + rainIntensity * 0.015f;
        float x = 0.35f + r * cos(A);
        float y = 0.76f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.06f + rainIntensity * 0.015f;
        float x = 0.40f + r * cos(A);
        float y = 0.78f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.05f + rainIntensity * 0.015f;
        float x = 0.46f + r * cos(A);
        float y = 0.76f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    glPopMatrix();
}

// rain clouds
void drawRainClouds() {
    if(rainIntensity <= 0.0f) return;

    glPushMatrix();
    glTranslatef(cloudMove * 1.0f, 0.0f, 0.0f);

    float c = 0.28f - rainIntensity * 0.06f;
    glColor3f(c, c, c);

    // upper continuous cloud
    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.12f;
        glVertex2f(-0.95f + r * cos(A), 0.92f + r * sin(A));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.14f;
        glVertex2f(-0.75f + r * cos(A), 0.94f + r * sin(A));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.13f;
        glVertex2f(-0.55f + r * cos(A), 0.92f + r * sin(A));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.15f;
        glVertex2f(-0.32f + r * cos(A), 0.94f + r * sin(A));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.13f;
        glVertex2f(-0.08f + r * cos(A), 0.91f + r * sin(A));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.15f;
        glVertex2f(0.18f + r * cos(A), 0.94f + r * sin(A));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.13f;
        glVertex2f(0.43f + r * cos(A), 0.91f + r * sin(A));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.15f;
        glVertex2f(0.68f + r * cos(A), 0.94f + r * sin(A));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.13f;
        glVertex2f(0.92f + r * cos(A), 0.91f + r * sin(A));
    }
    glEnd();

    // lower dark cloud layer
    glColor3f(c + 0.03f, c + 0.03f, c + 0.03f);

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.09f;
        glVertex2f(-0.80f + r * cos(A), 0.78f + r * sin(A));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.10f;
        glVertex2f(-0.55f + r * cos(A), 0.76f + r * sin(A));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.11f;
        glVertex2f(-0.25f + r * cos(A), 0.78f + r * sin(A));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.10f;
        glVertex2f(0.08f + r * cos(A), 0.76f + r * sin(A));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.11f;
        glVertex2f(0.38f + r * cos(A), 0.78f + r * sin(A));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.10f;
        glVertex2f(0.68f + r * cos(A), 0.76f + r * sin(A));
    }
    glEnd();

    glPopMatrix();
}
// rain drops
void drawRain() {
    if(rainIntensity <= 0.0f) return;

    int dropsToDraw = (int)(RAIN_COUNT * rainIntensity);

    glLineWidth(1.5f);
    glBegin(GL_LINES);

    for(int i = 0; i < dropsToDraw; i++) {
        float length = 0.08f + (i % 5) * 0.01f;
        float shade = 0.55f + (i % 4) * 0.08f;

        glColor3f(0.45f * shade, 0.55f * shade, 0.75f * shade);

        glVertex2f(rainX[i], rainY[i]);
        glVertex2f(rainX[i] + 0.025f, rainY[i] - length);
    }

    glEnd();
    glLineWidth(1.0f);
}

// Wind bending
float getWindBend() {
    if(windDirection == 1) {
        return 0.018f + 0.008f * sin(windWave);   // bend to right
    }
    if(windDirection == -1) {
        return -0.018f - 0.008f * sin(windWave);  // bend to left
    }
    return 0.0f;
}

// Wind effect lines
void drawWindEffect() {
    if(windDirection == 0) return;

    glColor3f(0.85f, 0.85f, 0.85f);
    glLineWidth(1.0f);

    glBegin(GL_LINES);

    if(windDirection == 1) {
        // wind blowing left to right
        glVertex2f(-0.90f, 0.65f); glVertex2f(-0.65f, 0.65f);
        glVertex2f(-0.70f, 0.55f); glVertex2f(-0.45f, 0.55f);
        glVertex2f(-0.20f, 0.68f); glVertex2f( 0.05f, 0.68f);
        glVertex2f( 0.25f, 0.58f); glVertex2f( 0.50f, 0.58f);
        glVertex2f( 0.60f, 0.70f); glVertex2f( 0.85f, 0.70f);
    }
    else if(windDirection == -1) {
        // wind blowing right to left
        glVertex2f(-0.65f, 0.65f); glVertex2f(-0.90f, 0.65f);
        glVertex2f(-0.45f, 0.55f); glVertex2f(-0.70f, 0.55f);
        glVertex2f( 0.05f, 0.68f); glVertex2f(-0.20f, 0.68f);
        glVertex2f( 0.50f, 0.58f); glVertex2f( 0.25f, 0.58f);
        glVertex2f( 0.85f, 0.70f); glVertex2f( 0.60f, 0.70f);
    }

    glEnd();
}

void drawWindTree(float tx, float baseY) {

    float bend = getWindBend();

    // bottom fixed, top bends with wind
    glColor3f(0.45f, 0.25f, 0.08f);
    glBegin(GL_QUADS);
    glVertex2f(tx - 0.015f, baseY);
    glVertex2f(tx + 0.015f, baseY);
    glVertex2f(tx + 0.015f + bend * 0.45f, baseY + 0.12f);
    glVertex2f(tx - 0.015f + bend * 0.45f, baseY + 0.12f);
    glEnd();

    // Main leaf circle
    glColor3f(0.02f, 0.50f, 0.08f);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.05f;
        float x = tx + bend * 0.70f + r * cos(A);
        float y = baseY + 0.17f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    // Left leaf circle
    glColor3f(0.04f, 0.58f, 0.10f);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.04f;
        float x = tx - 0.03f + bend * 0.55f + r * cos(A);
        float y = baseY + 0.15f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    // Right leaf circle
    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.04f;
        float x = tx + 0.03f + bend * 0.85f + r * cos(A);
        float y = baseY + 0.15f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    // Top leaf circle
    glColor3f(0.08f, 0.65f, 0.15f);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.025f;
        float x = tx + bend * 1.00f + r * cos(A);
        float y = baseY + 0.22f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawTrees() {

    // Tree 1
    drawWindTree(-0.90f, -0.12f);

    // Tree 2
    drawWindTree(-0.70f, -0.12f);

    // Tree 3
    drawWindTree(-0.48f, -0.12f);

    // Tree 4
    drawWindTree(-0.20f, -0.12f);

    // Tree 5
    drawWindTree(0.16f, -0.12f);

    // Tree 6
    drawWindTree(0.46f, -0.12f);

    // Tree 7
    drawWindTree(0.72f, -0.12f);

    // Tree 8
    drawWindTree(0.92f, -0.12f);
}

void drawLampPosts() {

    // Left side lamp post 1
    glColor3f(0.25f, 0.25f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(-0.82f, -0.18f);
    glVertex2f(-0.80f, -0.18f);
    glVertex2f(-0.80f, 0.12f);
    glVertex2f(-0.82f, 0.12f);
    glEnd();


    glBegin(GL_QUADS);
    glVertex2f(-0.82f, 0.12f);
    glVertex2f(-0.75f, 0.12f);
    glVertex2f(-0.75f, 0.10f);
    glVertex2f(-0.82f, 0.10f);
    glEnd();


    if(dayValue <= 0.4f || rainOn == 1.0f)
    {
        glColor3f(0.8f, 0.8f, 0.8f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++)
        {
            float pi = 3.1416f;
            float A = (i * 2 * pi) / 200;
            float r = 0.025f;
            float x = -0.75f + r * cos(A);
            float y = 0.095f + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++)
        {
            float pi = 3.1416f;
            float A = (i * 2 * pi) / 200;
            float r = 0.015f;
            float x = -0.75f + r * cos(A);
            float y = 0.095f + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();
    }

    else
    {
        glColor3f(1.0f, 0.95f, 0.60f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++) {
            float pi = 3.1416f;
            float A = (i * 2 * pi) / 200;
            float r = 0.015f;
            float x = -0.75f + r * cos(A);
            float y = 0.095f + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();
    }

    // Left side lamp post 2
    glColor3f(0.25f, 0.25f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(-0.55f, -0.18f);
    glVertex2f(-0.53f, -0.18f);
    glVertex2f(-0.53f, 0.12f);
    glVertex2f(-0.55f, 0.12f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(-0.55f, 0.12f);
    glVertex2f(-0.48f, 0.12f);
    glVertex2f(-0.48f, 0.10f);
    glVertex2f(-0.55f, 0.10f);
    glEnd();


    if(dayValue <= 0.4f || rainOn == 1.0f)
    {
        glColor3f(0.8f, 0.8f, 0.8f);
        glBegin(GL_POLYGON);
            for(int i = 0; i < 200; i++) {
            float pi = 3.1416f;
            float A = (i * 2 * pi) / 200;
            float r = 0.025f;
            float x = -0.48f + r * cos(A);
            float y = 0.095f + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_POLYGON);
            for(int i = 0; i < 200; i++) {
            float pi = 3.1416f;
            float A = (i * 2 * pi) / 200;
            float r = 0.015f;
            float x = -0.48f + r * cos(A);
            float y = 0.095f + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();
    }

    else
    {
        glColor3f(1.0f, 0.95f, 0.60f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++) {
            float pi = 3.1416f;
            float A = (i * 2 * pi) / 200;
            float r = 0.015f;
            float x = -0.48f + r * cos(A);
            float y = 0.095f + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();
    }

    // Right side lamp post 1
    glColor3f(0.25f, 0.25f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(0.52f, -0.18f);
    glVertex2f(0.54f, -0.18f);
    glVertex2f(0.54f, 0.12f);
    glVertex2f(0.52f, 0.12f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(0.52f, 0.12f);
    glVertex2f(0.59f, 0.12f);
    glVertex2f(0.59f, 0.10f);
    glVertex2f(0.52f, 0.10f);
    glEnd();


    if(dayValue <= 0.4f || rainOn == 1.0f)
    {
        glColor3f(0.8f, 0.8f, 0.8f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++) {
            float pi = 3.1416f;
            float A = (i * 2 * pi) / 200;
            float r = 0.025f;
            float x = 0.59f + r * cos(A);
            float y = 0.095f + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++) {
            float pi = 3.1416f;
            float A = (i * 2 * pi) / 200;
            float r = 0.015f;
            float x = 0.59f + r * cos(A);
            float y = 0.095f + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();
    }

    else
    {
        glColor3f(1.0f, 0.95f, 0.60f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++) {
            float pi = 3.1416f;
            float A = (i * 2 * pi) / 200;
            float r = 0.015f;
            float x = 0.59f + r * cos(A);
            float y = 0.095f + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();
    }
    glColor3f(1.0f, 0.95f, 0.60f);

    // Right side lamp post 2
    glColor3f(0.25f, 0.25f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(0.80f, -0.18f);
    glVertex2f(0.82f, -0.18f);
    glVertex2f(0.82f, 0.12f);
    glVertex2f(0.80f, 0.12f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(0.80f, 0.12f);
    glVertex2f(0.87f, 0.12f);
    glVertex2f(0.87f, 0.10f);
    glVertex2f(0.80f, 0.10f);
    glEnd();


    if(dayValue <= 0.4f || rainOn == 1.0f)
    {
        glColor3f(0.8f, 0.8f, 0.8f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++) {
            float pi = 3.1416f;
            float A = (i * 2 * pi) / 200;
            float r = 0.025f;
            float x = 0.87f + r * cos(A);
            float y = 0.095f + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++) {
            float pi = 3.1416f;
            float A = (i * 2 * pi) / 200;
            float r = 0.015f;
            float x = 0.87f + r * cos(A);
            float y = 0.095f + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();
    }

    else
    {
        glColor3f(1.0f, 0.95f, 0.60f);
        glBegin(GL_POLYGON);
        for(int i = 0; i < 200; i++) {
            float pi = 3.1416f;
            float A = (i * 2 * pi) / 200;
            float r = 0.015f;
            float x = 0.87f + r * cos(A);
            float y = 0.095f + r * sin(A);
            glVertex2f(x, y);
        }
        glEnd();
    }
}

// Bangladesh flags
void drawFlags() {

    float bend = getWindBend();

    // Flag pole 1
    glColor3f(0.45f, 0.45f, 0.45f);
    glBegin(GL_QUADS);
    glVertex2f(-0.10f, -0.18f);
    glVertex2f(-0.09f, -0.18f);
    glVertex2f(-0.09f, 0.18f);
    glVertex2f(-0.10f, 0.18f);
    glEnd();

    // Flag 1 body
    glColor3f(0.00f, 0.42f, 0.18f);
    glBegin(GL_QUADS);
    glVertex2f(-0.09f, 0.12f);
    glVertex2f( 0.02f + bend, 0.10f);
    glVertex2f( 0.02f + bend, 0.04f);
    glVertex2f(-0.09f, 0.04f);
    glEnd();

    // Flag 1 circle
    glColor3f(0.85f, 0.10f, 0.10f);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.018f;
        float x = -0.03f + bend * 0.5f + r * cos(A);
        float y = 0.08f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();

    // Flag pole 2
    glColor3f(0.45f, 0.45f, 0.45f);
    glBegin(GL_QUADS);
    glVertex2f(0.18f, -0.18f);
    glVertex2f(0.19f, -0.18f);
    glVertex2f(0.19f, 0.18f);
    glVertex2f(0.18f, 0.18f);
    glEnd();

    // Flag 2 body
    glColor3f(0.00f, 0.42f, 0.18f);
    glBegin(GL_QUADS);
    glVertex2f(0.19f, 0.12f);
    glVertex2f(0.30f + bend, 0.10f);
    glVertex2f(0.30f + bend, 0.04f);
    glVertex2f(0.19f, 0.04f);
    glEnd();

    // Flag 2 circle
    glColor3f(0.85f, 0.10f, 0.10f);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        float r = 0.018f;
        float x = 0.25f + bend * 0.5f + r * cos(A);
        float y = 0.08f + r * sin(A);
        glVertex2f(x, y);
    }
    glEnd();
}

// buildings
void drawBuildings() {
    // Determine window color based on rain or night
    float winR, winG, winB;
    if (rainIntensity > 0.4f || dayValue < 0.3f) {
        // Yellowish (lights on)
        winR = 1.0f;
        winG = 0.85f;
        winB = 0.40f;
    } else {
        // Light blue (daytime, no rain)
        winR = 0.68f;
        winG = 0.85f;
        winB = 1.0f;
    }


    // Background Building 1
    glColor3f(0.45f, 0.45f, 0.50f);
    glBegin(GL_QUADS);
    glVertex2f(-1.00f, -0.15f);
    glVertex2f(-0.70f, -0.15f);
    glVertex2f(-0.70f, 0.45f);
    glVertex2f(-1.00f, 0.45f);
    glEnd();
    // Roof
    glColor3f(0.35f, 0.35f, 0.40f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-1.00f, 0.45f);
    glVertex2f(-0.70f, 0.45f);
    glVertex2f(-0.85f, 0.54f);
    glEnd();
    glColor3f(0.25f, 0.25f, 0.30f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.95f, 0.45f);
    glVertex2f(-0.75f, 0.45f);
    glVertex2f(-0.85f, 0.50f);
    glEnd();
    // Windows
    glColor3f(winR, winG, winB);
    glBegin(GL_QUADS);
    glVertex2f(-0.93f, 0.32f); glVertex2f(-0.89f, 0.32f); glVertex2f(-0.89f, 0.37f); glVertex2f(-0.93f, 0.37f);
    glVertex2f(-0.85f, 0.32f); glVertex2f(-0.81f, 0.32f); glVertex2f(-0.81f, 0.37f); glVertex2f(-0.85f, 0.37f);
    glVertex2f(-0.77f, 0.32f); glVertex2f(-0.73f, 0.32f); glVertex2f(-0.73f, 0.37f); glVertex2f(-0.77f, 0.37f);
    glVertex2f(-0.93f, 0.20f); glVertex2f(-0.89f, 0.20f); glVertex2f(-0.89f, 0.25f); glVertex2f(-0.93f, 0.25f);
    glVertex2f(-0.85f, 0.20f); glVertex2f(-0.81f, 0.20f); glVertex2f(-0.81f, 0.25f); glVertex2f(-0.85f, 0.25f);
    glVertex2f(-0.77f, 0.20f); glVertex2f(-0.73f, 0.20f); glVertex2f(-0.73f, 0.25f); glVertex2f(-0.77f, 0.25f);
    glVertex2f(-0.93f, 0.08f); glVertex2f(-0.89f, 0.08f); glVertex2f(-0.89f, 0.13f); glVertex2f(-0.93f, 0.13f);
    glVertex2f(-0.85f, 0.08f); glVertex2f(-0.81f, 0.08f); glVertex2f(-0.81f, 0.13f); glVertex2f(-0.85f, 0.13f);
    glVertex2f(-0.77f, 0.08f); glVertex2f(-0.73f, 0.08f); glVertex2f(-0.73f, 0.13f); glVertex2f(-0.77f, 0.13f);
    glEnd();
    // Door
    glColor3f(0.40f, 0.25f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(-0.87f, -0.15f); glVertex2f(-0.83f, -0.15f); glVertex2f(-0.83f, -0.05f); glVertex2f(-0.87f, -0.05f);
    glEnd();

    // Background Building 2
    glColor3f(0.65f, 0.35f, 0.30f);
    glBegin(GL_QUADS);
    glVertex2f(-0.70f, -0.15f);
    glVertex2f(-0.40f, -0.15f);
    glVertex2f(-0.40f, 0.42f);
    glVertex2f(-0.70f, 0.42f);
    glEnd();
    // Flat roof
    glColor3f(0.50f, 0.25f, 0.22f);
    glBegin(GL_QUADS);
    glVertex2f(-0.71f, 0.42f); glVertex2f(-0.39f, 0.42f); glVertex2f(-0.39f, 0.45f); glVertex2f(-0.71f, 0.45f);
    glEnd();
    glColor3f(0.40f, 0.20f, 0.18f);
    glBegin(GL_QUADS);
    glVertex2f(-0.69f, 0.45f); glVertex2f(-0.41f, 0.45f); glVertex2f(-0.41f, 0.47f); glVertex2f(-0.69f, 0.47f);
    glEnd();
    // Windows
    glColor3f(winR, winG, winB);
    glBegin(GL_QUADS);
    glVertex2f(-0.66f, 0.30f); glVertex2f(-0.62f, 0.30f); glVertex2f(-0.62f, 0.35f); glVertex2f(-0.66f, 0.35f);
    glVertex2f(-0.58f, 0.30f); glVertex2f(-0.54f, 0.30f); glVertex2f(-0.54f, 0.35f); glVertex2f(-0.58f, 0.35f);
    glVertex2f(-0.50f, 0.30f); glVertex2f(-0.46f, 0.30f); glVertex2f(-0.46f, 0.35f); glVertex2f(-0.50f, 0.35f);
    glVertex2f(-0.66f, 0.18f); glVertex2f(-0.62f, 0.18f); glVertex2f(-0.62f, 0.23f); glVertex2f(-0.66f, 0.23f);
    glVertex2f(-0.58f, 0.18f); glVertex2f(-0.54f, 0.18f); glVertex2f(-0.54f, 0.23f); glVertex2f(-0.58f, 0.23f);
    glVertex2f(-0.50f, 0.18f); glVertex2f(-0.46f, 0.18f); glVertex2f(-0.46f, 0.23f); glVertex2f(-0.50f, 0.23f);
    glVertex2f(-0.66f, 0.06f); glVertex2f(-0.62f, 0.06f); glVertex2f(-0.62f, 0.11f); glVertex2f(-0.66f, 0.11f);
    glVertex2f(-0.58f, 0.06f); glVertex2f(-0.54f, 0.06f); glVertex2f(-0.54f, 0.11f); glVertex2f(-0.58f, 0.11f);
    glVertex2f(-0.50f, 0.06f); glVertex2f(-0.46f, 0.06f); glVertex2f(-0.46f, 0.11f); glVertex2f(-0.50f, 0.11f);
    glEnd();
    // Door
    glColor3f(0.40f, 0.25f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(-0.57f, -0.15f); glVertex2f(-0.53f, -0.15f); glVertex2f(-0.53f, -0.05f); glVertex2f(-0.57f, -0.05f);
    glEnd();

    // Background Building 3
    glColor3f(0.70f, 0.65f, 0.55f);
    glBegin(GL_QUADS);
    glVertex2f(-0.40f, -0.15f);
    glVertex2f(-0.15f, -0.15f);
    glVertex2f(-0.15f, 0.48f);
    glVertex2f(-0.40f, 0.48f);
    glEnd();
    // Dome roof
    glColor3f(0.55f, 0.50f, 0.42f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.40f, 0.48f);
    glVertex2f(-0.15f, 0.48f);
    glVertex2f(-0.275f, 0.58f);
    glEnd();
    glColor3f(0.45f, 0.40f, 0.35f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.38f, 0.48f);
    glVertex2f(-0.17f, 0.48f);
    glVertex2f(-0.275f, 0.54f);
    glEnd();
    // Windows
    glColor3f(winR, winG, winB);
    glBegin(GL_QUADS);
    glVertex2f(-0.36f, 0.36f); glVertex2f(-0.32f, 0.36f); glVertex2f(-0.32f, 0.41f); glVertex2f(-0.36f, 0.41f);
    glVertex2f(-0.28f, 0.36f); glVertex2f(-0.24f, 0.36f); glVertex2f(-0.24f, 0.41f); glVertex2f(-0.28f, 0.41f);
    glVertex2f(-0.20f, 0.36f); glVertex2f(-0.16f, 0.36f); glVertex2f(-0.16f, 0.41f); glVertex2f(-0.20f, 0.41f);
    glVertex2f(-0.36f, 0.24f); glVertex2f(-0.32f, 0.24f); glVertex2f(-0.32f, 0.29f); glVertex2f(-0.36f, 0.29f);
    glVertex2f(-0.28f, 0.24f); glVertex2f(-0.24f, 0.24f); glVertex2f(-0.24f, 0.29f); glVertex2f(-0.28f, 0.29f);
    glVertex2f(-0.20f, 0.24f); glVertex2f(-0.16f, 0.24f); glVertex2f(-0.16f, 0.29f); glVertex2f(-0.20f, 0.29f);
    glVertex2f(-0.36f, 0.12f); glVertex2f(-0.32f, 0.12f); glVertex2f(-0.32f, 0.17f); glVertex2f(-0.36f, 0.17f);
    glVertex2f(-0.28f, 0.12f); glVertex2f(-0.24f, 0.12f); glVertex2f(-0.24f, 0.17f); glVertex2f(-0.28f, 0.17f);
    glVertex2f(-0.20f, 0.12f); glVertex2f(-0.16f, 0.12f); glVertex2f(-0.16f, 0.17f); glVertex2f(-0.20f, 0.17f);
    glEnd();
    // Door
    glColor3f(0.40f, 0.25f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(-0.30f, -0.15f); glVertex2f(-0.26f, -0.15f); glVertex2f(-0.26f, -0.05f); glVertex2f(-0.30f, -0.05f);
    glEnd();

    // Background Building 4
    glColor3f(0.55f, 0.55f, 0.60f);
    glBegin(GL_QUADS);
    glVertex2f(-0.15f, -0.15f);
    glVertex2f(0.10f, -0.15f);
    glVertex2f(0.10f, 0.44f);
    glVertex2f(-0.15f, 0.44f);
    glEnd();
    // Roof
    glColor3f(0.42f, 0.42f, 0.47f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.15f, 0.44f);
    glVertex2f(0.10f, 0.44f);
    glVertex2f(-0.025f, 0.53f);
    glEnd();
    glColor3f(0.35f, 0.35f, 0.40f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.12f, 0.44f);
    glVertex2f(0.07f, 0.44f);
    glVertex2f(-0.025f, 0.49f);
    glEnd();
    // Windows
    glColor3f(winR, winG, winB);
    glBegin(GL_QUADS);
    glVertex2f(-0.11f, 0.32f); glVertex2f(-0.07f, 0.32f); glVertex2f(-0.07f, 0.37f); glVertex2f(-0.11f, 0.37f);
    glVertex2f(-0.03f, 0.32f); glVertex2f(0.01f, 0.32f); glVertex2f(0.01f, 0.37f); glVertex2f(-0.03f, 0.37f);
    glVertex2f(0.05f, 0.32f); glVertex2f(0.09f, 0.32f); glVertex2f(0.09f, 0.37f); glVertex2f(0.05f, 0.37f);
    glVertex2f(-0.11f, 0.20f); glVertex2f(-0.07f, 0.20f); glVertex2f(-0.07f, 0.25f); glVertex2f(-0.11f, 0.25f);
    glVertex2f(-0.03f, 0.20f); glVertex2f(0.01f, 0.20f); glVertex2f(0.01f, 0.25f); glVertex2f(-0.03f, 0.25f);
    glVertex2f(0.05f, 0.20f); glVertex2f(0.09f, 0.20f); glVertex2f(0.09f, 0.25f); glVertex2f(0.05f, 0.25f);
    glVertex2f(-0.11f, 0.08f); glVertex2f(-0.07f, 0.08f); glVertex2f(-0.07f, 0.13f); glVertex2f(-0.11f, 0.13f);
    glVertex2f(-0.03f, 0.08f); glVertex2f(0.01f, 0.08f); glVertex2f(0.01f, 0.13f); glVertex2f(-0.03f, 0.13f);
    glVertex2f(0.05f, 0.08f); glVertex2f(0.09f, 0.08f); glVertex2f(0.09f, 0.13f); glVertex2f(0.05f, 0.13f);
    glEnd();
    // Door
    glColor3f(0.40f, 0.25f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(-0.02f, -0.15f); glVertex2f(0.02f, -0.15f); glVertex2f(0.02f, -0.05f); glVertex2f(-0.02f, -0.05f);
    glEnd();

    // Background Building 5
    glColor3f(0.50f, 0.35f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(0.10f, -0.15f);
    glVertex2f(0.40f, -0.15f);
    glVertex2f(0.40f, 0.47f);
    glVertex2f(0.10f, 0.47f);
    glEnd();
    // Flat roof
    glColor3f(0.38f, 0.28f, 0.20f);
    glBegin(GL_QUADS);
    glVertex2f(0.09f, 0.47f); glVertex2f(0.41f, 0.47f); glVertex2f(0.41f, 0.50f); glVertex2f(0.09f, 0.50f);
    glEnd();
    glColor3f(0.30f, 0.22f, 0.16f);
    glBegin(GL_QUADS);
    glVertex2f(0.12f, 0.50f); glVertex2f(0.38f, 0.50f); glVertex2f(0.38f, 0.52f); glVertex2f(0.12f, 0.52f);
    glEnd();
    // Windows
    glColor3f(winR, winG, winB);
    glBegin(GL_QUADS);
    glVertex2f(0.14f, 0.35f); glVertex2f(0.18f, 0.35f); glVertex2f(0.18f, 0.40f); glVertex2f(0.14f, 0.40f);
    glVertex2f(0.22f, 0.35f); glVertex2f(0.26f, 0.35f); glVertex2f(0.26f, 0.40f); glVertex2f(0.22f, 0.40f);
    glVertex2f(0.30f, 0.35f); glVertex2f(0.34f, 0.35f); glVertex2f(0.34f, 0.40f); glVertex2f(0.30f, 0.40f);
    glVertex2f(0.14f, 0.23f); glVertex2f(0.18f, 0.23f); glVertex2f(0.18f, 0.28f); glVertex2f(0.14f, 0.28f);
    glVertex2f(0.22f, 0.23f); glVertex2f(0.26f, 0.23f); glVertex2f(0.26f, 0.28f); glVertex2f(0.22f, 0.28f);
    glVertex2f(0.30f, 0.23f); glVertex2f(0.34f, 0.23f); glVertex2f(0.34f, 0.28f); glVertex2f(0.30f, 0.28f);
    glVertex2f(0.14f, 0.11f); glVertex2f(0.18f, 0.11f); glVertex2f(0.18f, 0.16f); glVertex2f(0.14f, 0.16f);
    glVertex2f(0.22f, 0.11f); glVertex2f(0.26f, 0.11f); glVertex2f(0.26f, 0.16f); glVertex2f(0.22f, 0.16f);
    glVertex2f(0.30f, 0.11f); glVertex2f(0.34f, 0.11f); glVertex2f(0.34f, 0.16f); glVertex2f(0.30f, 0.16f);
    glEnd();
    // Door
    glColor3f(0.40f, 0.25f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(0.22f, -0.15f); glVertex2f(0.26f, -0.15f); glVertex2f(0.26f, -0.05f); glVertex2f(0.22f, -0.05f);
    glEnd();

    // Background Building 6
    glColor3f(0.40f, 0.45f, 0.50f);
    glBegin(GL_QUADS);
    glVertex2f(0.40f, -0.15f);
    glVertex2f(0.70f, -0.15f);
    glVertex2f(0.70f, 0.49f);
    glVertex2f(0.40f, 0.49f);
    glEnd();
    // Roof
    glColor3f(0.30f, 0.35f, 0.40f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.40f, 0.49f);
    glVertex2f(0.70f, 0.49f);
    glVertex2f(0.55f, 0.58f);
    glEnd();
    glColor3f(0.22f, 0.27f, 0.32f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.43f, 0.49f);
    glVertex2f(0.67f, 0.49f);
    glVertex2f(0.55f, 0.54f);
    glEnd();
    // Windows
    glColor3f(winR, winG, winB);
    glBegin(GL_QUADS);
    glVertex2f(0.44f, 0.37f); glVertex2f(0.48f, 0.37f); glVertex2f(0.48f, 0.42f); glVertex2f(0.44f, 0.42f);
    glVertex2f(0.52f, 0.37f); glVertex2f(0.56f, 0.37f); glVertex2f(0.56f, 0.42f); glVertex2f(0.52f, 0.42f);
    glVertex2f(0.60f, 0.37f); glVertex2f(0.64f, 0.37f); glVertex2f(0.64f, 0.42f); glVertex2f(0.60f, 0.42f);
    glVertex2f(0.44f, 0.25f); glVertex2f(0.48f, 0.25f); glVertex2f(0.48f, 0.30f); glVertex2f(0.44f, 0.30f);
    glVertex2f(0.52f, 0.25f); glVertex2f(0.56f, 0.25f); glVertex2f(0.56f, 0.30f); glVertex2f(0.52f, 0.30f);
    glVertex2f(0.60f, 0.25f); glVertex2f(0.64f, 0.25f); glVertex2f(0.64f, 0.30f); glVertex2f(0.60f, 0.30f);
    glVertex2f(0.44f, 0.13f); glVertex2f(0.48f, 0.13f); glVertex2f(0.48f, 0.18f); glVertex2f(0.44f, 0.18f);
    glVertex2f(0.52f, 0.13f); glVertex2f(0.56f, 0.13f); glVertex2f(0.56f, 0.18f); glVertex2f(0.52f, 0.18f);
    glVertex2f(0.60f, 0.13f); glVertex2f(0.64f, 0.13f); glVertex2f(0.64f, 0.18f); glVertex2f(0.60f, 0.18f);
    glEnd();
    // Door
    glColor3f(0.40f, 0.25f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(0.52f, -0.15f); glVertex2f(0.56f, -0.15f); glVertex2f(0.56f, -0.05f); glVertex2f(0.52f, -0.05f);
    glEnd();

    // Background Building 7
    glColor3f(0.60f, 0.58f, 0.55f);
    glBegin(GL_QUADS);
    glVertex2f(0.70f, -0.15f);
    glVertex2f(1.00f, -0.15f);
    glVertex2f(1.00f, 0.43f);
    glVertex2f(0.70f, 0.43f);
    glEnd();
    // roof
    glColor3f(0.48f, 0.46f, 0.44f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.70f, 0.43f);
    glVertex2f(1.00f, 0.43f);
    glVertex2f(0.85f, 0.52f);
    glEnd();
    glColor3f(0.38f, 0.36f, 0.34f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.73f, 0.43f);
    glVertex2f(0.97f, 0.43f);
    glVertex2f(0.85f, 0.48f);
    glEnd();
    // Windows
    glColor3f(winR, winG, winB);
    glBegin(GL_QUADS);
    glVertex2f(0.74f, 0.31f); glVertex2f(0.78f, 0.31f); glVertex2f(0.78f, 0.36f); glVertex2f(0.74f, 0.36f);
    glVertex2f(0.82f, 0.31f); glVertex2f(0.86f, 0.31f); glVertex2f(0.86f, 0.36f); glVertex2f(0.82f, 0.36f);
    glVertex2f(0.90f, 0.31f); glVertex2f(0.94f, 0.31f); glVertex2f(0.94f, 0.36f); glVertex2f(0.90f, 0.36f);
    glVertex2f(0.74f, 0.19f); glVertex2f(0.78f, 0.19f); glVertex2f(0.78f, 0.24f); glVertex2f(0.74f, 0.24f);
    glVertex2f(0.82f, 0.19f); glVertex2f(0.86f, 0.19f); glVertex2f(0.86f, 0.24f); glVertex2f(0.82f, 0.24f);
    glVertex2f(0.90f, 0.19f); glVertex2f(0.94f, 0.19f); glVertex2f(0.94f, 0.24f); glVertex2f(0.90f, 0.24f);
    glVertex2f(0.74f, 0.07f); glVertex2f(0.78f, 0.07f); glVertex2f(0.78f, 0.12f); glVertex2f(0.74f, 0.12f);
    glVertex2f(0.82f, 0.07f); glVertex2f(0.86f, 0.07f); glVertex2f(0.86f, 0.12f); glVertex2f(0.82f, 0.12f);
    glVertex2f(0.90f, 0.07f); glVertex2f(0.94f, 0.07f); glVertex2f(0.94f, 0.12f); glVertex2f(0.90f, 0.12f);
    glEnd();
    // Door
    glColor3f(0.40f, 0.25f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(0.82f, -0.15f); glVertex2f(0.86f, -0.15f); glVertex2f(0.86f, -0.05f); glVertex2f(0.82f, -0.05f);
    glEnd();

    // Foreground

    // Building 1
    glColor3f(0.55f, 0.30f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(-0.98f, -0.15f);
    glVertex2f(-0.78f, -0.15f);
    glVertex2f(-0.78f, 0.22f);
    glVertex2f(-0.98f, 0.22f);
    glEnd();
    // Roof
    glColor3f(0.44f, 0.24f, 0.20f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.98f, 0.22f);
    glVertex2f(-0.78f, 0.22f);
    glVertex2f(-0.88f, 0.31f);
    glEnd();
    glColor3f(0.35f, 0.18f, 0.15f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.95f, 0.22f);
    glVertex2f(-0.81f, 0.22f);
    glVertex2f(-0.88f, 0.27f);
    glEnd();
    // Windows
    glColor3f(winR, winG, winB);
    glBegin(GL_QUADS);
    glVertex2f(-0.94f, 0.12f); glVertex2f(-0.90f, 0.12f); glVertex2f(-0.90f, 0.17f); glVertex2f(-0.94f, 0.17f);
    glVertex2f(-0.86f, 0.12f); glVertex2f(-0.82f, 0.12f); glVertex2f(-0.82f, 0.17f); glVertex2f(-0.86f, 0.17f);
    glVertex2f(-0.94f, 0.02f); glVertex2f(-0.90f, 0.02f); glVertex2f(-0.90f, 0.07f); glVertex2f(-0.94f, 0.07f);
    glVertex2f(-0.86f, 0.02f); glVertex2f(-0.82f, 0.02f); glVertex2f(-0.82f, 0.07f); glVertex2f(-0.86f, 0.07f);
    glEnd();
    // Door
    glColor3f(0.40f, 0.25f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(-0.90f, -0.15f); glVertex2f(-0.86f, -0.15f); glVertex2f(-0.86f, -0.05f); glVertex2f(-0.90f, -0.05f);
    glEnd();

    // Building 2
    glColor3f(0.48f, 0.50f, 0.52f);
    glBegin(GL_QUADS);
    glVertex2f(-0.73f, -0.15f);
    glVertex2f(-0.53f, -0.15f);
    glVertex2f(-0.53f, 0.32f);
    glVertex2f(-0.73f, 0.32f);
    glEnd();
    // Roof
    glColor3f(0.38f, 0.40f, 0.42f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.73f, 0.32f);
    glVertex2f(-0.53f, 0.32f);
    glVertex2f(-0.63f, 0.41f);
    glEnd();
    glColor3f(0.28f, 0.30f, 0.32f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.70f, 0.32f);
    glVertex2f(-0.56f, 0.32f);
    glVertex2f(-0.63f, 0.37f);
    glEnd();
    // Windows
    glColor3f(winR, winG, winB);
    glBegin(GL_QUADS);
    glVertex2f(-0.69f, 0.20f); glVertex2f(-0.65f, 0.20f); glVertex2f(-0.65f, 0.25f); glVertex2f(-0.69f, 0.25f);
    glVertex2f(-0.61f, 0.20f); glVertex2f(-0.57f, 0.20f); glVertex2f(-0.57f, 0.25f); glVertex2f(-0.61f, 0.25f);
    glVertex2f(-0.69f, 0.08f); glVertex2f(-0.65f, 0.08f); glVertex2f(-0.65f, 0.13f); glVertex2f(-0.69f, 0.13f);
    glVertex2f(-0.61f, 0.08f); glVertex2f(-0.57f, 0.08f); glVertex2f(-0.57f, 0.13f); glVertex2f(-0.61f, 0.13f);
    glEnd();
    // Door
    glColor3f(0.40f, 0.25f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(-0.65f, -0.15f); glVertex2f(-0.61f, -0.15f); glVertex2f(-0.61f, -0.05f); glVertex2f(-0.65f, -0.05f);
    glEnd();

    // Hospital
    glColor3f(0.92f, 0.90f, 0.88f);
    glBegin(GL_QUADS);
    glVertex2f(-0.48f, -0.15f);
    glVertex2f(-0.28f, -0.15f);
    glVertex2f(-0.28f, 0.35f);
    glVertex2f(-0.48f, 0.35f);
    glEnd();
    // Flat roof
    glColor3f(0.45f, 0.60f, 0.45f);
    glBegin(GL_QUADS);
    glVertex2f(-0.49f, 0.35f); glVertex2f(-0.27f, 0.35f); glVertex2f(-0.27f, 0.38f); glVertex2f(-0.49f, 0.38f);
    glEnd();
    glColor3f(0.35f, 0.48f, 0.35f);
    glBegin(GL_QUADS);
    glVertex2f(-0.47f, 0.38f); glVertex2f(-0.29f, 0.38f); glVertex2f(-0.29f, 0.40f); glVertex2f(-0.47f, 0.40f);
    glEnd();
    // Red cross
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.40f, 0.26f); glVertex2f(-0.36f, 0.26f); glVertex2f(-0.36f, 0.32f); glVertex2f(-0.40f, 0.32f);
    glVertex2f(-0.43f, 0.28f); glVertex2f(-0.33f, 0.28f); glVertex2f(-0.33f, 0.30f); glVertex2f(-0.43f, 0.30f);
    glEnd();
    // Windows
    glColor3f(winR, winG, winB);
    glBegin(GL_QUADS);
    glVertex2f(-0.44f, 0.18f); glVertex2f(-0.40f, 0.18f); glVertex2f(-0.40f, 0.23f); glVertex2f(-0.44f, 0.23f);
    glVertex2f(-0.36f, 0.18f); glVertex2f(-0.32f, 0.18f); glVertex2f(-0.32f, 0.23f); glVertex2f(-0.36f, 0.23f);
    glVertex2f(-0.44f, 0.06f); glVertex2f(-0.40f, 0.06f); glVertex2f(-0.40f, 0.11f); glVertex2f(-0.44f, 0.11f);
    glVertex2f(-0.36f, 0.06f); glVertex2f(-0.32f, 0.06f); glVertex2f(-0.32f, 0.11f); glVertex2f(-0.36f, 0.11f);
    glEnd();
    // Door
    glColor3f(0.40f, 0.25f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(-0.41f, -0.15f); glVertex2f(-0.35f, -0.15f); glVertex2f(-0.35f, -0.05f); glVertex2f(-0.41f, -0.05f);
    glEnd();

    // Building 3
    glColor3f(0.60f, 0.45f, 0.35f);
    glBegin(GL_QUADS);
    glVertex2f(-0.23f, -0.15f);
    glVertex2f(-0.03f, -0.15f);
    glVertex2f(-0.03f, 0.30f);
    glVertex2f(-0.23f, 0.30f);
    glEnd();
    // roof
    glColor3f(0.48f, 0.36f, 0.28f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.23f, 0.30f);
    glVertex2f(-0.03f, 0.30f);
    glVertex2f(-0.13f, 0.39f);
    glEnd();
    glColor3f(0.38f, 0.28f, 0.22f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.20f, 0.30f);
    glVertex2f(-0.06f, 0.30f);
    glVertex2f(-0.13f, 0.35f);
    glEnd();
    // Windows
    glColor3f(winR, winG, winB);
    glBegin(GL_QUADS);
    glVertex2f(-0.19f, 0.18f); glVertex2f(-0.15f, 0.18f); glVertex2f(-0.15f, 0.23f); glVertex2f(-0.19f, 0.23f);
    glVertex2f(-0.11f, 0.18f); glVertex2f(-0.07f, 0.18f); glVertex2f(-0.07f, 0.23f); glVertex2f(-0.11f, 0.23f);
    glVertex2f(-0.19f, 0.06f); glVertex2f(-0.15f, 0.06f); glVertex2f(-0.15f, 0.11f); glVertex2f(-0.19f, 0.11f);
    glVertex2f(-0.11f, 0.06f); glVertex2f(-0.07f, 0.06f); glVertex2f(-0.07f, 0.11f); glVertex2f(-0.11f, 0.11f);
    glEnd();
    // Door
    glColor3f(0.40f, 0.25f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(-0.15f, -0.15f); glVertex2f(-0.11f, -0.15f); glVertex2f(-0.11f, -0.05f); glVertex2f(-0.15f, -0.05f);
    glEnd();

    // SCHOOL
    glColor3f(0.85f, 0.80f, 0.65f);
    glBegin(GL_QUADS);
    glVertex2f(0.02f, -0.15f);
    glVertex2f(0.27f, -0.15f);
    glVertex2f(0.27f, 0.35f);
    glVertex2f(0.02f, 0.35f);
    glEnd();
    // roof
    glColor3f(0.68f, 0.60f, 0.48f);
    glBegin(GL_QUADS);
    glVertex2f(0.01f, 0.35f); glVertex2f(0.28f, 0.35f); glVertex2f(0.28f, 0.38f); glVertex2f(0.01f, 0.38f);
    glEnd();
    glColor3f(0.58f, 0.50f, 0.40f);
    glBegin(GL_QUADS);
    glVertex2f(0.04f, 0.38f); glVertex2f(0.25f, 0.38f); glVertex2f(0.25f, 0.40f); glVertex2f(0.04f, 0.40f);
    glEnd();


    // School Windows
    glColor3f(winR, winG, winB);
    glBegin(GL_QUADS);
    glVertex2f(0.05f, 0.25f); glVertex2f(0.09f, 0.25f); glVertex2f(0.09f, 0.30f); glVertex2f(0.05f, 0.30f);
    glVertex2f(0.12f, 0.25f); glVertex2f(0.16f, 0.25f); glVertex2f(0.16f, 0.30f); glVertex2f(0.12f, 0.30f);
    glVertex2f(0.19f, 0.25f); glVertex2f(0.23f, 0.25f); glVertex2f(0.23f, 0.30f); glVertex2f(0.19f, 0.30f);
    glVertex2f(0.05f, 0.12f); glVertex2f(0.09f, 0.12f); glVertex2f(0.09f, 0.17f); glVertex2f(0.05f, 0.17f);
    glVertex2f(0.12f, 0.12f); glVertex2f(0.16f, 0.12f); glVertex2f(0.16f, 0.17f); glVertex2f(0.12f, 0.17f);
    glVertex2f(0.19f, 0.12f); glVertex2f(0.23f, 0.12f); glVertex2f(0.23f, 0.17f); glVertex2f(0.19f, 0.17f);
    glEnd();

    // School Door
    glColor3f(0.40f, 0.25f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(0.12f, -0.15f); glVertex2f(0.17f, -0.15f); glVertex2f(0.17f, -0.05f); glVertex2f(0.12f, -0.05f);
    glEnd();

    // Fire Service Station
    glColor3f(0.70f, 0.25f, 0.20f);
    glBegin(GL_QUADS);
    glVertex2f(0.32f, -0.15f);
    glVertex2f(0.54f, -0.15f);
    glVertex2f(0.54f, 0.28f);
    glVertex2f(0.32f, 0.28f);
    glEnd();
    // Roof
    glColor3f(0.56f, 0.20f, 0.16f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.31f, 0.28f);
    glVertex2f(0.55f, 0.28f);
    glVertex2f(0.43f, 0.37f);
    glEnd();
    glColor3f(0.45f, 0.16f, 0.12f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.34f, 0.28f);
    glVertex2f(0.52f, 0.28f);
    glVertex2f(0.43f, 0.33f);
    glEnd();
    // Emblem
    glColor3f(0.85f, 0.72f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(0.41f, 0.20f); glVertex2f(0.45f, 0.20f); glVertex2f(0.45f, 0.24f); glVertex2f(0.41f, 0.24f);
    glEnd();
    // Garage door
    glColor3f(0.40f, 0.25f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(0.37f, -0.15f); glVertex2f(0.49f, -0.15f); glVertex2f(0.49f, 0.05f); glVertex2f(0.37f, 0.05f);
    glEnd();
    // Windows
    glColor3f(winR, winG, winB);
    glBegin(GL_QUADS);
    glVertex2f(0.35f, 0.12f); glVertex2f(0.39f, 0.12f); glVertex2f(0.39f, 0.16f); glVertex2f(0.35f, 0.16f);
    glVertex2f(0.47f, 0.12f); glVertex2f(0.51f, 0.12f); glVertex2f(0.51f, 0.16f); glVertex2f(0.47f, 0.16f);
    glEnd();

    // Police Station
    glColor3f(0.25f, 0.35f, 0.55f);
    glBegin(GL_QUADS);
    glVertex2f(0.58f, -0.15f);
    glVertex2f(0.82f, -0.15f);
    glVertex2f(0.82f, 0.33f);
    glVertex2f(0.58f, 0.33f);
    glEnd();
    // roof
    glColor3f(0.20f, 0.28f, 0.44f);
    glBegin(GL_QUADS);
    glVertex2f(0.57f, 0.33f); glVertex2f(0.83f, 0.33f); glVertex2f(0.83f, 0.36f); glVertex2f(0.57f, 0.36f);
    glVertex2f(0.61f, 0.36f); glVertex2f(0.79f, 0.36f); glVertex2f(0.79f, 0.39f); glVertex2f(0.61f, 0.39f);
    glVertex2f(0.64f, 0.39f); glVertex2f(0.76f, 0.39f); glVertex2f(0.76f, 0.41f); glVertex2f(0.64f, 0.41f);
    glEnd();
    // Badge
    glColor3f(0.85f, 0.72f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(0.68f, 0.24f); glVertex2f(0.72f, 0.24f); glVertex2f(0.72f, 0.28f); glVertex2f(0.68f, 0.28f);
    glEnd();
    // Windows
    glColor3f(winR, winG, winB);
    glBegin(GL_QUADS);
    glVertex2f(0.62f, 0.16f); glVertex2f(0.66f, 0.16f); glVertex2f(0.66f, 0.21f); glVertex2f(0.62f, 0.21f);
    glVertex2f(0.74f, 0.16f); glVertex2f(0.78f, 0.16f); glVertex2f(0.78f, 0.21f); glVertex2f(0.74f, 0.21f);
    glVertex2f(0.62f, 0.04f); glVertex2f(0.66f, 0.04f); glVertex2f(0.66f, 0.09f); glVertex2f(0.62f, 0.09f);
    glVertex2f(0.74f, 0.04f); glVertex2f(0.78f, 0.04f); glVertex2f(0.78f, 0.09f); glVertex2f(0.74f, 0.09f);
    glEnd();
    // Main door
    glColor3f(0.40f, 0.25f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(0.68f, -0.15f); glVertex2f(0.72f, -0.15f); glVertex2f(0.72f, -0.05f); glVertex2f(0.68f, -0.05f);
    glEnd();

    // Last building
    glColor3f(0.70f, 0.62f, 0.52f);
    glBegin(GL_QUADS);
    glVertex2f(0.86f, -0.15f);
    glVertex2f(1.00f, -0.15f);
    glVertex2f(1.00f, 0.24f);
    glVertex2f(0.86f, 0.24f);
    glEnd();
    // Roof
    glColor3f(0.56f, 0.50f, 0.42f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.86f, 0.24f);
    glVertex2f(1.00f, 0.24f);
    glVertex2f(0.93f, 0.33f);
    glEnd();
    glColor3f(0.46f, 0.40f, 0.34f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.88f, 0.24f);
    glVertex2f(0.98f, 0.24f);
    glVertex2f(0.93f, 0.29f);
    glEnd();
    // Windows
    glColor3f(winR, winG, winB);
    glBegin(GL_QUADS);
    glVertex2f(0.89f, 0.12f); glVertex2f(0.93f, 0.12f); glVertex2f(0.93f, 0.17f); glVertex2f(0.89f, 0.17f);
    glVertex2f(0.95f, 0.12f); glVertex2f(0.99f, 0.12f); glVertex2f(0.99f, 0.17f); glVertex2f(0.95f, 0.17f);
    glVertex2f(0.89f, 0.00f); glVertex2f(0.93f, 0.00f); glVertex2f(0.93f, 0.05f); glVertex2f(0.89f, 0.05f);
    glVertex2f(0.95f, 0.00f); glVertex2f(0.99f, 0.00f); glVertex2f(0.99f, 0.05f); glVertex2f(0.95f, 0.05f);
    glEnd();
    // Door
    glColor3f(0.40f, 0.25f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(0.92f, -0.15f); glVertex2f(0.96f, -0.15f); glVertex2f(0.96f, -0.05f); glVertex2f(0.92f, -0.05f);
    glEnd();
}

// grass ground
void drawGround() {
    glColor3f(0.10f, 0.50f, 0.16f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.10f);
    glVertex2f( 1.0f, -0.10f);
    glVertex2f( 1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();
}

// draw circle (for traffic light and wheels)
void drawCircle(float cx, float cy, float r) {
    glBegin(GL_POLYGON);
    for(int i = 0; i < 200; i++) {
        float pi = 3.1416f;
        float A = (i * 2 * pi) / 200;
        glVertex2f(cx + r * cos(A), cy + r * sin(A));
    }
    glEnd();
}

// zebra lane
void drawZebraLane(float topY, float bottomY) {
    float y = topY;
    glBegin(GL_QUADS);
    while(y > bottomY) {
        glVertex2f(zebraLeft, y);
        glVertex2f(zebraRight, y);
        glVertex2f(zebraRight, y - 0.025f);
        glVertex2f(zebraLeft, y - 0.025f);
        y -= 0.055f;
    }
    glEnd();
}

// road with lanes, divider, zebra crossing
void drawRoad() {
    // Footpath
    glColor3f(0.55f, 0.55f, 0.55f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.18f);
    glVertex2f( 1.0f, -0.18f);
    glVertex2f( 1.0f, -0.28f);
    glVertex2f(-1.0f, -0.28f);
    glEnd();

    // Main road
    glColor3f(0.13f, 0.13f, 0.14f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.28f);
    glVertex2f( 1.0f, -0.28f);
    glVertex2f( 1.0f, -0.95f);
    glVertex2f(-1.0f, -0.95f);
    glEnd();

    // Road borders
    glColor3f(0.90f, 0.90f, 0.90f);
    glBegin(GL_LINES);
    glVertex2f(-1.0f, -0.28f); glVertex2f( 1.0f, -0.28f);
    glVertex2f(-1.0f, -0.95f); glVertex2f( 1.0f, -0.95f);
    glEnd();

    // Middle divider
    glColor3f(0.90f, 0.82f, 0.10f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.61f);
    glVertex2f( 1.0f, -0.61f);
    glVertex2f( 1.0f, -0.63f);
    glVertex2f(-1.0f, -0.63f);
    glEnd();

    // Lane markers
    glColor3f(1.0f, 1.0f, 0.85f);
    glBegin(GL_LINES);
    for(float x = -0.95f; x < 0.95f; x += 0.20f) {
        glVertex2f(x, -0.45f); glVertex2f(x + 0.10f, -0.45f);
        glVertex2f(x, -0.79f); glVertex2f(x + 0.10f, -0.79f);
    }
    glEnd();

    // Zebra crossing across all four lanes
    glColor3f(1.0f, 1.0f, 1.0f);
    drawZebraLane(-0.31f, -0.45f);  // upper lane 1
    drawZebraLane(-0.48f, -0.60f);  // upper lane 2
    drawZebraLane(-0.66f, -0.79f);  // lower lane 1
    drawZebraLane(-0.82f, -0.94f);  // lower lane 2
}

// traffic light
void drawTrafficLight() {
    // Pole
    glColor3f(0.25f, 0.25f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(-0.01f, -0.28f);
    glVertex2f( 0.01f, -0.28f);
    glVertex2f( 0.01f,  0.12f);
    glVertex2f(-0.01f,  0.12f);
    glEnd();

    // Box
    glColor3f(0.12f, 0.12f, 0.12f);
    glBegin(GL_QUADS);
    glVertex2f(-0.04f, 0.00f);
    glVertex2f( 0.04f, 0.00f);
    glVertex2f( 0.04f, 0.16f);
    glVertex2f(-0.04f, 0.16f);
    glEnd();

    // Red
    if(lightState == 'R') glColor3f(1.0f, 0.0f, 0.0f);
    else glColor3f(0.35f, 0.0f, 0.0f);
    drawCircle(0.00f, 0.12f, 0.015f);

    // Yellow
    if(lightState == 'Y') glColor3f(1.0f, 1.0f, 0.0f);
    else glColor3f(0.35f, 0.35f, 0.0f);
    drawCircle(0.00f, 0.08f, 0.015f);

    // Green
    if(lightState == 'G') glColor3f(0.0f, 1.0f, 0.0f);
    else glColor3f(0.0f, 0.35f, 0.0f);
    drawCircle(0.00f, 0.04f, 0.015f);
}

// single car drawing
void drawSingleCar(float carX, float y, float r, float g, float b, int direction) {
    // Body
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(carX + 0.00f, y);
    glVertex2f(carX + 0.26f, y);
    glVertex2f(carX + 0.26f, y + 0.08f);
    glVertex2f(carX + 0.00f, y + 0.08f);
    glEnd();

    // Cabin
    glBegin(GL_POLYGON);
    glVertex2f(carX + 0.04f, y + 0.08f);
    glVertex2f(carX + 0.22f, y + 0.08f);
    glVertex2f(carX + 0.19f, y + 0.15f);
    glVertex2f(carX + 0.07f, y + 0.15f);
    glEnd();

    // Windows
    glColor3f(0.75f, 0.90f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(carX + 0.07f, y + 0.09f); glVertex2f(carX + 0.12f, y + 0.09f);
    glVertex2f(carX + 0.11f, y + 0.14f); glVertex2f(carX + 0.08f, y + 0.14f);
    glVertex2f(carX + 0.14f, y + 0.09f); glVertex2f(carX + 0.20f, y + 0.09f);
    glVertex2f(carX + 0.18f, y + 0.14f); glVertex2f(carX + 0.13f, y + 0.14f);
    glEnd();

    // Headlight and tail-light
    glBegin(GL_QUADS);
    if(direction == 1) {
        glColor3f(1.0f, 1.0f, 0.70f);
        glVertex2f(carX + 0.24f, y + 0.02f); glVertex2f(carX + 0.26f, y + 0.02f);
        glVertex2f(carX + 0.26f, y + 0.06f); glVertex2f(carX + 0.24f, y + 0.06f);
        glColor3f(0.90f, 0.10f, 0.10f);
        glVertex2f(carX + 0.00f, y + 0.02f); glVertex2f(carX + 0.02f, y + 0.02f);
        glVertex2f(carX + 0.02f, y + 0.06f); glVertex2f(carX + 0.00f, y + 0.06f);
    } else {
        glColor3f(1.0f, 1.0f, 0.70f);
        glVertex2f(carX + 0.00f, y + 0.02f); glVertex2f(carX + 0.02f, y + 0.02f);
        glVertex2f(carX + 0.02f, y + 0.06f); glVertex2f(carX + 0.00f, y + 0.06f);
        glColor3f(0.90f, 0.10f, 0.10f);
        glVertex2f(carX + 0.24f, y + 0.02f); glVertex2f(carX + 0.26f, y + 0.02f);
        glVertex2f(carX + 0.26f, y + 0.06f); glVertex2f(carX + 0.24f, y + 0.06f);
    }
    glEnd();

    // Wheels
    glColor3f(0.05f, 0.05f, 0.05f);
    drawCircle(carX + 0.06f, y, 0.030f);
    drawCircle(carX + 0.20f, y, 0.030f);

    glColor3f(0.60f, 0.60f, 0.65f);
    drawCircle(carX + 0.06f, y, 0.014f);
    drawCircle(carX + 0.20f, y, 0.014f);
}

// cars (8 vehicles)
void drawCars() {
    drawSingleCar(car1X, -0.42f, 0.10f, 0.35f, 0.90f,  1); // blue
    drawSingleCar(car2X, -0.57f, 0.92f, 0.55f, 0.10f,  1); // orange
    drawSingleCar(car3X, -0.77f, 0.15f, 0.75f, 0.25f, -1); // green
    drawSingleCar(car4X, -0.91f, 0.65f, 0.15f, 0.80f, -1); // purple
    drawSingleCar(car5X, -0.42f, 0.90f, 0.10f, 0.10f,  1); // red
    drawSingleCar(car6X, -0.57f, 0.20f, 0.80f, 0.85f,  1); // cyan
    drawSingleCar(car7X, -0.77f, 0.95f, 0.85f, 0.10f, -1); // yellow
    drawSingleCar(car8X, -0.91f, 0.85f, 0.85f, 0.85f, -1); // silver
}



// Traffic logic
float getGreenOrYellowSpeed() {
    if(lightState == 'G') return normalSpeed;
    if(lightState == 'Y') return slowSpeed;
    return normalSpeed;   // during red, cars only move up to stop line
}

void moveRightLane(float *frontCar, float *backCar, float speed) {
    float first = *frontCar;
    float second = *backCar;

    if(second > first) {
        float temp = first;
        first = second;
        second = temp;
    }

    if(lightState == 'R') {
        if(first + carWidth > rightStopX) {
            first += speed;
        } else {
            float stopX = rightStopX - carWidth;
            first += speed;
            if(first > stopX) first = stopX;
        }

        if(second + carWidth > rightStopX) {
            second += speed;
        } else {
            float stopX = first - carWidth - carGap;
            second += speed;
            if(second > stopX) second = stopX;
        }
    } else {
        first += speed;
        second += speed;

        if(second + carWidth + carGap > first && second < first) {
            second = first - carWidth - carGap;
        }
    }

    if(first > wrapRight) first = wrapLeft;
    if(second > wrapRight) second = wrapLeft - carWidth - carGap;

    *frontCar = first;
    *backCar = second;
}

void moveLeftLane(float *frontCar, float *backCar, float speed) {
    float first = *frontCar;
    float second = *backCar;

    if(second < first) {
        float temp = first;
        first = second;
        second = temp;
    }

    if(lightState == 'R') {
        if(first < leftStopX) {
            first -= speed;
        } else {
            float stopX = leftStopX;
            first -= speed;
            if(first < stopX) first = stopX;
        }

        if(second < leftStopX) {
            second -= speed;
        } else {
            float stopX = first + carWidth + carGap;
            second -= speed;
            if(second < stopX) second = stopX;
        }
    } else {
        first -= speed;
        second -= speed;

        if(first + carWidth + carGap > second && first < second) {
            second = first + carWidth + carGap;
        }
    }

    if(first < wrapLeft) first = wrapRight;
    if(second < wrapLeft) second = wrapRight + carWidth + carGap;

    *frontCar = first;
    *backCar = second;
}


void display() {

    if(showStartScreen == 1) {
        drawStartScreen();
        return;
    }
    drawSky();
    drawStars();
    drawSunMoon();
    drawClouds();
    drawRainClouds();
    drawBuildings();
    drawGround();
    drawTrees();
    drawLampPosts();
    drawFlags();
    drawRoad();
    drawTrafficLight();
    drawCars();
    drawRain();
    drawWindEffect();

    glFlush();
}


void update(int value) {

    if(showStartScreen == 1) {
    glutPostRedisplay();
    glutTimerFunc(20, update, 0);
    return;
    }
    // Cloud movement with wind
    if(windDirection == 1) {
        cloudMove += 0.004f;      // wind left to right, clouds move faster right
    }
    else if(windDirection == -1) {
        cloudMove -= 0.004f;      // wind right to left, clouds move faster left
    }
    else {
        cloudMove += 0.001f;      // normal cloud movement
    }

    if(cloudMove > 1.80f) {
        cloudMove = -1.80f;
    }
    if(cloudMove < -1.80f) {
        cloudMove = 1.80f;
    }

    if(rainOn == 1) {
        if(rainIntensity < 1.0f) rainIntensity += 0.01f;
    } else {
        if(rainIntensity > 0.0f) rainIntensity -= 0.01f;
    }

    for(int i = 0; i < RAIN_COUNT; i++) {
        rainY[i] -= rainSpeed[i];
        rainX[i] += 0.001f;
        if(rainY[i] < -1.0f || rainX[i] > 1.05f) {
            rainY[i] = 1.05f;
            rainX[i] = -1.0f + (rand() % 200) / 100.0f;
        }
    }

    // Night to Day
    if(dayValue < targetDayValue) {
        dayValue += 0.005f;
        if(dayValue > targetDayValue) dayValue = targetDayValue;
    }

    // Day to Night
    if(dayValue > targetDayValue) {
        dayValue -= 0.005f;
        if(dayValue < targetDayValue) dayValue = targetDayValue;
    }


    if(running == 1) {
        float speed = getGreenOrYellowSpeed();
        moveRightLane(&car1X, &car5X, speed);
        moveRightLane(&car2X, &car6X, speed);
        moveLeftLane(&car3X, &car7X, speed);
        moveLeftLane(&car4X, &car8X, speed);
    }

    // Wind
    if(windDirection != 0) {
        windWave += 0.15f;
        if(windWave > 6.28f) {
            windWave = 0.0f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(20, update, 0);
}


void handleKeypress(unsigned char key, int x, int y) {
    // Start screen control
    if(showStartScreen == 1) {
        if(key == 's' || key == 'S') {
            showStartScreen = 0;
        }

        glutPostRedisplay();
        return;
    }
    // Traffic light controls (R, G, Y)
    switch(key) {
        case 'r':
        case 'R':
            lightState = 'R';
            break;
        case 'g':
        case 'G':
            lightState = 'G';
            break;
        case 'y':
        case 'Y':
            lightState = 'Y';
            break;
        case 13:  // Enter
            running = !running;
            break;
    }


    if(key == 'd' || key == 'D') {
        targetDayValue = 1.0f;
    }
    if(key == 'n' || key == 'N') {
        targetDayValue = 0.0f;
    }
    if(key == 't' || key == 'T') {
        rainOn = !rainOn;
        if (rainOn)
        {
            startRainSound();
        }
        else
        {
            stopRainSound();
            //startCitySound();
        }
    }
    // Wind controls
    if(key == 'l' || key == 'L') {
        windDirection = 1;     // wind blows left to right
    }
    if(key == 'k' || key == 'K') {
        windDirection = -1;    // wind blows right to left
    }
    if(key == 'o' || key == 'O') {
        windDirection = 0;     // wind stops
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
    glutCreateWindow("Urban Life in Motion");

    init();
    initRain();
    //startCitySound();

    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeypress);
    glutTimerFunc(20, update, 0);
    glutMainLoop();

    return 0;
}
