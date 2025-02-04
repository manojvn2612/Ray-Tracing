#include <iostream>
#include <SDL3/SDL.h>
#include <cmath>
#include <algorithm>
#include <vector>

#define M_PI 3.14159265358979323846
const int WIDTH = 900;
const int HEIGHT = 600;

struct Circle {
    double x, y, r;
};

struct Point {
    double x, y;
};

// Function to draw a pixel
void drawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        SDL_Rect pixel = { x, y, 1, 1 };
        SDL_FillSurfaceRect(surface, &pixel, color);
    }
}

// Function to draw a filled circle using Midpoint Circle Algorithm
void drawFilledCircle(SDL_Surface* surface, Circle c, Uint32 color) {
    int x = 0;
    int y = c.r;
    int p = 1 - c.r;
    while (x <= y) {
        for (int i = c.x - x; i <= c.x + x; i++) {
            drawPixel(surface, i, c.y + y, color);
            drawPixel(surface, i, c.y - y, color);
        }
        for (int i = c.x - y; i <= c.x + y; i++) {
            drawPixel(surface, i, c.y + x, color);
            drawPixel(surface, i, c.y - x, color);
        }
        if (p < 0) {
            p += 2 * x + 3;
        }
        else {
            p += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

// Function to draw a line
void drawLine(SDL_Surface* surface, double x1, double y1, double x2, double y2, Uint32 color) {
    double dx = x2 - x1, dy = y2 - y1;
    int steps = std::max(abs(dx), abs(dy));
    double x_inc = dx / steps, y_inc = dy / steps;
    double x = x1, y = y1;
    for (int i = 0; i <= steps; i++) {
        drawPixel(surface, (int)x, (int)y, color);
        x += x_inc;
        y += y_inc;
    }
}

// Function to fill area between extended lines
void fillBetweenLines(SDL_Surface* surface, const Point& p1, const Point& p2, 
                     const Point& p3, const Point& p4, double startX, Uint32 color) {
    // Calculate the slope and y-intercept for both lines
    double m1 = (p2.y - p1.y) / (p2.x - p1.x);
    double b1 = p1.y - m1 * p1.x;
    
    double m2 = (p4.y - p3.y) / (p4.x - p3.x);
    double b2 = p3.y - m2 * p3.x;
    
    // Fill the area between lines
    for (int x = startX; x < WIDTH; x++) {
        int y1 = m1 * x + b1;
        int y2 = m2 * x + b2;
        
        // Ensure y1 is always the smaller value
        if (y1 > y2) std::swap(y1, y2);
        
        // Draw vertical line between the two points
        for (int y = y1; y <= y2; y++) {
            drawPixel(surface, x, y, color);
        }
    }
}

// Function to draw extended external tangents and fill area
void drawTangents(SDL_Surface* surface, Circle c1, Circle c2, Uint32 lineColor, Uint32 fillColor) {
    double d = sqrt(pow(c2.x - c1.x, 2) + pow(c2.y - c1.y, 2));
    if (d < fabs(c1.r - c2.r)) return;
    
    double theta = atan2(c2.y - c1.y, c2.x - c1.x);
    double alpha = asin((c2.r - c1.r) / d);
    
    const double extension = 1000.0;
    
    // Store points for both tangent lines
    std::vector<Point> points;
    
    for (int i = -1; i <= 1; i += 2) {
        double beta = theta + i * alpha + i * M_PI / 2;
        
        // Calculate points on circles
        double x1 = c1.x + c1.r * cos(beta);
        double y1 = c1.y + c1.r * sin(beta);
        double x2 = c2.x + c2.r * cos(beta);
        double y2 = c2.y + c2.r * sin(beta);
        
        // Calculate direction vector
        double dx = x2 - x1;
        double dy = y2 - y1;
        double length = sqrt(dx * dx + dy * dy);
        double unit_x = dx / length;
        double unit_y = dy / length;
        
        // Calculate extended end point
        double x3 = x2 + unit_x * extension;
        double y3 = y2 + unit_y * extension;
        
        // Store points for filling
        points.push_back({x1, y1});
        points.push_back({x3, y3});
        
        // Draw the extended line
        drawLine(surface, x1, y1, x3, y3, lineColor);
    }
    
    // Fill area between extended lines after circle c2
    if (points.size() == 4) {
        fillBetweenLines(surface, points[0], points[1], points[2], points[3], 
                        c2.x + c2.r, fillColor);
    }
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    SDL_Window* window = SDL_CreateWindow("Circle Tangents", WIDTH, HEIGHT, 0);
    if (!window) {
        std::cerr << "Window Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    Uint32 white = SDL_MapSurfaceRGB(surface, 255, 255, 255);
    Uint32 black = SDL_MapSurfaceRGB(surface, 0, 0, 0);
    Uint32 fillColor = SDL_MapSurfaceRGB(surface, 100, 100, 255); // Light blue fill
    
    Circle c1 = { 300, 300, 80 };
    Circle c2 = { 600, 400, 100 };
    
    bool is_running = true;
    SDL_Event event;
    
    while (is_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) is_running = false;
            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                c1.x = event.motion.x;
                c1.y = event.motion.y;
            }
        }
        
        SDL_FillSurfaceRect(surface, nullptr, black);
        drawFilledCircle(surface, c1, white);
        drawFilledCircle(surface, c2, white);
        drawTangents(surface, c1, c2, white, fillColor);
        SDL_UpdateWindowSurface(window);
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}