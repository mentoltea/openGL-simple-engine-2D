#ifndef S7K_OPENGL_LIB
#define S7K_OPENGL_LIB

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

#include <math.h>
#include <unistd.h>
#include <vector>

// STRUCTURES
class DrawableObject {
public:
    virtual void draw() = 0;
};

extern std::vector<DrawableObject*> ObjectsToDraw;


void RemoveFromVector(DrawableObject* item, std::vector<DrawableObject*>& vector = ObjectsToDraw);

struct Point {
    int x, y;
    Point() = default;
    Point(int x, int y): x(x), y(y) {}
    Point operator+(Point p) {
        return Point(x+p.x, y+p.y);
    }
    Point operator-(Point p) {
        return Point(x-p.x, y-p.y);
    }
    Point operator-() {
        return Point(-x, -y);
    }
};
struct Color {
    int r, g, b;
    int a = 0;
};


// GRAPHIC & READY FUNCS
extern size_t WINX, WINY;
extern float FPS;
extern unsigned int delaytime;

//prevents user from window size changing
void reshape(int width, int height);
// sets new window shape
void force_reshape(int width, int height);
void setFPS(float FPS);
void display();
// updates the game (needs to be provided)
extern void call();
void update();
// prepares the game (needs to be provided)
extern void prepare();


// DRAWING FUNCTIONS

// Draws line
void draw_line(Point start, Point end, Color color = {0,0,0}, int width=3);

void draw_circle(Point center, int radius, Color color = {0,0,0}, int width=3, int shapeness=180);

void draw_rect(Point leftbottom,  int width, int height, Color color = {0,0,0}, int linewidth=3);

void draw_poly(std::vector<Point> points, Color color = {0,0,0}, int linewidth=3);

#endif