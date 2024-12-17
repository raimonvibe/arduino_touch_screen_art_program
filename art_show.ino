#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>   // Hardware-specific library

MCUFRIEND_kbv tft;           // Instantiate the display object

// Define commonly used colors
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// Increase show duration to 5 minutes and number of effects
const unsigned long showDuration = 300000; // 5 minutes in milliseconds
const int numEffects = 15;  // Increased from 5 to 15 effects

unsigned long startTime; // Start time of the art show

void setup() {
    Serial.begin(9600);

    uint16_t ID = tft.readID();
    if (ID == 0xD3D3) ID = 0x9486; // Fix for some screens
    tft.begin(ID);
    tft.setRotation(1);            // Screen rotation (0-3)
    tft.fillScreen(BLACK);         // Clear screen

    Serial.println("5-Minute Art Show Starting");
    Serial.print("Detected ID: 0x");
    Serial.println(ID, HEX);

    // Start the timer
    startTime = millis();
}

void loop() {
    // Check if 5 minutes have passed
    if (millis() - startTime >= showDuration) {
        tft.fillScreen(BLACK);
        tft.setTextColor(WHITE);
        tft.setTextSize(2);
        tft.setCursor(30, 150);
        tft.print("Art Show Finished!");
        while (1); // Stop execution
    }

    // Art show sequence
    artShow();
}

void artShow() {
    static int stage = 0; // Track the current stage of the art show

    // Clear screen with fade effect
    for (int i = 255; i > 0; i -= 51) {
        tft.fillScreen(interpolateColor(BLACK, WHITE, i/255.0));
        delay(10);
    }

    switch (stage) {
        case 0: drawCheckerboard(); break;
        case 1: drawRainbowLines(); break;
        case 2: drawExpandingCircles(); break;
        case 3: drawColorfulTriangles(); break;
        case 4: drawMovingRectangles(); break;
        case 5: drawSpiral(); break;
        case 6: drawFireworks(); break;
        case 7: drawKaleidoscope(); break;
        case 8: drawWaves(); break;
        case 9: drawMaze(); break;
        // Enhanced versions of original effects
        case 10: drawCheckerboard(); drawSpiral(); break;
        case 11: drawRainbowLines(); drawWaves(); break;
        case 12: drawExpandingCircles(); drawFireworks(); break;
        case 13: drawColorfulTriangles(); drawKaleidoscope(); break;
        case 14: drawMovingRectangles(); drawMaze(); break;
    }

    stage = (stage + 1) % numEffects;
    delay(5000);
}

void drawCheckerboard() {
    tft.fillScreen(BLACK);
    int size = 40;
    for (int x = 0; x < tft.width(); x += size) {
        for (int y = 0; y < tft.height(); y += size) {
            if ((x / size + y / size) % 2 == 0) {
                tft.fillRect(x, y, size, size, WHITE);
            } else {
                tft.fillRect(x, y, size, size, BLACK);
            }
        }
    }
}

void drawRainbowLines() {
    tft.fillScreen(BLACK);
    for (int i = 0; i < tft.height(); i += 10) {
        tft.drawLine(0, i, tft.width(), tft.height() - i, rainbowColor(i / 10));
    }
}

void drawExpandingCircles() {
    tft.fillScreen(BLACK);
    for (int r = 10; r < 150; r += 15) {
        tft.drawCircle(tft.width() / 2, tft.height() / 2, r, rainbowColor(r / 10));
        delay(100);
    }
}

void drawColorfulTriangles() {
    tft.fillScreen(BLACK);
    for (int i = 0; i < 10; i++) {
        tft.fillTriangle(random(0, 240), random(0, 320), random(0, 240), random(0, 320), random(0, 240), random(0, 320), rainbowColor(i));
        delay(300);
    }
}

void drawMovingRectangles() {
    tft.fillScreen(BLACK);
    for (int i = 0; i < tft.width(); i += 20) {
        tft.fillRect(i, i, 60, 60, rainbowColor(i / 20));
        delay(200);
    }
}

uint16_t rainbowColor(int index) {
    switch (index % 6) {
        case 0: return RED;
        case 1: return YELLOW;
        case 2: return GREEN;
        case 3: return CYAN;
        case 4: return BLUE;
        case 5: return MAGENTA;
    }
    return WHITE;
}

uint16_t interpolateColor(uint16_t color1, uint16_t color2, float ratio) {
    int r1 = (color1 & 0xF800) >> 11;
    int g1 = (color1 & 0x07E0) >> 5;
    int b1 = color1 & 0x001F;

    int r2 = (color2 & 0xF800) >> 11;
    int g2 = (color2 & 0x07E0) >> 5;
    int b2 = color2 & 0x001F;

    int r = r1 + (r2 - r1) * ratio;
    int g = g1 + (g2 - g1) * ratio;
    int b = b1 + (b2 - b1) * ratio;

    return ((r << 11) | (g << 5) | b);
}

uint16_t hsvToRgb565(float h, float s, float v) {
    float r, g, b;
    int i = floor(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    switch(i % 6) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }

    return ((int(r * 31) << 11) | (int(g * 63) << 5) | int(b * 31));
}

void drawSpiral() {
    tft.fillScreen(BLACK);
    int centerX = tft.width() / 2;
    int centerY = tft.height() / 2;
    float angle = 0;
    float r = 0;
    while (r < min(tft.width(), tft.height()) / 2) {
        int x = centerX + r * cos(angle);
        int y = centerY + r * sin(angle);
        tft.drawPixel(x, y, hsvToRgb565(angle / (2 * PI), 1.0, 1.0));
        angle += 0.1;
        r += 0.1;
    }
}

void drawFireworks() {
    tft.fillScreen(BLACK);
    for (int i = 0; i < 5; i++) {
        int centerX = random(50, tft.width() - 50);
        int centerY = random(50, tft.height() - 50);
        int particles = 50;

        for (int j = 0; j < particles; j++) {
            float angle = 2 * PI * j / particles;
            for (int r = 0; r < 40; r++) {
                int x = centerX + r * cos(angle);
                int y = centerY + r * sin(angle);
                if (x >= 0 && x < tft.width() && y >= 0 && y < tft.height()) {
                    tft.drawPixel(x, y, rainbowColor(i));
                }
            }
        }
        delay(200);
    }
}

void drawKaleidoscope() {
    tft.fillScreen(BLACK);
    int centerX = tft.width() / 2;
    int centerY = tft.height() / 2;
    int segments = 8;

    for (int r = 0; r < min(tft.width(), tft.height()) / 2; r += 2) {
        for (int i = 0; i < segments; i++) {
            float angle = 2 * PI * i / segments;
            int x = centerX + r * cos(angle);
            int y = centerY + r * sin(angle);
            uint16_t color = hsvToRgb565(float(r) / min(tft.width(), tft.height()), 1.0, 1.0);
            tft.drawLine(centerX, centerY, x, y, color);
        }
        delay(50);
    }
}

void drawWaves() {
    tft.fillScreen(BLACK);
    for (int y = 0; y < tft.height(); y++) {
        for (int x = 0; x < tft.width(); x++) {
            float val = sin(x * 0.1) + sin(y * 0.1);
            float hue = (val + 2) / 4.0;
            tft.drawPixel(x, y, hsvToRgb565(hue, 1.0, 1.0));
        }
    }
}

void drawMaze() {
    tft.fillScreen(BLACK);
    int cellSize = 20;
    for (int x = 0; x < tft.width(); x += cellSize) {
        for (int y = 0; y < tft.height(); y += cellSize) {
            if (random(2) == 0) {
                tft.drawLine(x, y, x + cellSize, y + cellSize, WHITE);
            } else {
                tft.drawLine(x + cellSize, y, x, y + cellSize, WHITE);
            }
        }
    }
}
