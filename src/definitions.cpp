#include "definitions.h"

std::vector<DrawableObject*> ObjectsToDraw;

void RemoveFromVector(DrawableObject* item, std::vector<DrawableObject*>& vector) {
    for (auto it = vector.begin(); it != vector.end(); it++) {
        if (*it == item) {
            vector.erase(it);
            break;
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);//очищаем буфер

    int size = ObjectsToDraw.size();
    for (int i=0; i<size; i++) {
        ObjectsToDraw[i]->draw();
    }

    glutSwapBuffers();//прорисовываем буфер на экран
    glutPostRedisplay();
}

void setFPS(float nFPS) {
    if (nFPS <= 0) return;
    FPS = nFPS;
    unsigned int delaytime = 1000000/nFPS;
}

void reshape(int width, int height) {
    glutReshapeWindow(WINX, WINY);
}

void force_reshape(int width, int height) {
    glutReshapeWindow(width, height);
    WINX = width; WINY = height;
}

void update() {
    using namespace std::chrono;
    auto start = steady_clock::now();
    call();
    display();
    auto end = steady_clock::now();
    unsigned int dur = duration_cast<microseconds>(end-start).count();
    if (dur < delaytime)
        usleep(delaytime-dur);
}

void draw_line(Point start, Point end, Color color, int width) {
    glColor4f(color.r, color.g, color.b, color.a);//задаем цвет рисования
    glLineWidth(width);//задаем толщину линии
    glBegin(GL_LINES);//говорим что будем рисовать линию
    glVertex2d(start.x, start.y);//координата начала отрезка в формате (x,y)
    glVertex2d(end.x, end.y);//координата конца отрезка
    glEnd();//говорим что закончили рисовать линию  
}

void draw_circle(Point center, int radius, Color color, int width, int shapeness) {
    glColor4f(color.r, color.g, color.b, color.a);//задаем цвет рисования
    glLineWidth(width);
    Point p1 = {center.x + radius, center.y};
    double koef = (2*M_PI)/shapeness;
    glBegin(GL_LINES);
    for (int i=0; i<shapeness+1; i++) {
        glVertex2d(p1.x, p1.y);
        p1.x = center.x + cos((double)i*koef)*radius;
        p1.y = center.y + sin((double)i*koef)*radius;
        glVertex2d(p1.x, p1.y);
    }
    glEnd();
}

void draw_filled_circle(Point center, int radius, Color color, int shapeness) {
    glColor4f(color.r, color.g, color.b, color.a);//задаем цвет рисования
    Point p1 = {center.x + radius, center.y};
    double koef = (2*M_PI)/shapeness;
    glBegin(GL_TRIANGLE_FAN);
    for (int i=0; i<shapeness+1; i++) {
        glVertex2d(p1.x, p1.y);
        p1.x = center.x + cos((double)i*koef)*radius;
        p1.y = center.y + sin((double)i*koef)*radius;
        glVertex2d(p1.x, p1.y);
    }
    glEnd();
}

void draw_rect(Point point,  int width, int height, Color color, int linewidth) {
    glColor4f(color.r, color.g, color.b, color.a);//задаем цвет рисования
    glLineWidth(linewidth);
    glBegin(GL_LINES);
    glVertex2d(point.x, point.y);
    glVertex2d(point.x + width, point.y);glVertex2d(point.x + width, point.y);
    glVertex2d(point.x + width, point.y + height);glVertex2d(point.x + width, point.y + height);
    glVertex2d(point.x, point.y + height);glVertex2d(point.x, point.y + height);
    glVertex2d(point.x, point.y);
    glEnd();
}


void draw_filled_rect(Point point,  int width, int height, Color color) {
    glColor4f(color.r, color.g, color.b, color.a);//задаем цвет рисования
    glBegin(GL_QUADS);
    glVertex2d(point.x, point.y);
    glVertex2d(point.x + width, point.y);//glVertex2d(point.x + width, point.y);
    glVertex2d(point.x + width, point.y + height);//glVertex2d(point.x + width, point.y + height);
    glVertex2d(point.x, point.y + height);//glVertex2d(point.x, point.y + height);
    glVertex2d(point.x, point.y);
    glEnd();
}

void draw_poly(std::vector<Point> points, Color color, int linewidth) {
    glColor4f(color.r, color.g, color.b, color.a);//задаем цвет рисования
    glLineWidth(linewidth);//задаем толщину линии
    glBegin(GL_LINES);//говорим что будем рисовать линию
    int size = points.size();
    for (int i=1; i<size+1; i++) {
        glVertex2d(points[i-1].x, points[i-1].y);
        glVertex2d(points[i%size].x, points[i%size].y);
    }
    glEnd();//говорим что закончили рисовать линию  
}

void draw_filled_poly(std::vector<Point> points, Color color) {
    glColor4f(color.r, color.g, color.b, color.a);//задаем цвет рисования
    glBegin(GL_TRIANGLE_FAN);//говорим что будем рисовать линию
    int size = points.size();
    for (int i=1; i<size+1; i++) {
        glVertex2d(points[i-1].x, points[i-1].y);
        glVertex2d(points[i%size].x, points[i%size].y);
    }
    glEnd();//говорим что закончили рисовать линию  
}