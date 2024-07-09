#include "definitions.h"

size_t WINX=700, WINY=700;
float FPS = 60;
unsigned int delaytime = 1000000/FPS;


Point p1, p2;
size_t tm = 0;
size_t pad = -50;

class OBJ: public DrawableObject {
public:
    Point p1, p2;
    size_t length =  -pad + (WINX > WINY ? WINY : WINX)/3;
    OBJ() {
        std::cout << "Const!" << std::endl;
        ObjectsToDraw.push_back(this);
    }
    ~OBJ() {
        std::cout << "Dest!" << std::endl;
        RemoveFromVector(this);
    }
    void call() {
        //if (tm>FPS*6) RemoveFromVector(this);
        p1.x = WINX/2 + cos((float)tm/30.)*length;
        p1.y = WINY/2 + sin((float)tm/30.)*length;

        p2.x = WINX - p1.x;
        p2.y = WINX - p1.y;
    }
    void draw() override {
        auto diff = p1-p2;
        diff = {-diff.y/4, diff.x/4};
        Point center = {(p1.x+p2.x)/2, (p1.y+p2.y)/2};
        draw_filled_poly({p1, center+diff, p2, center-diff}, {150,0,0});
        draw_rect(p1, 100, 50);
        draw_circle(p2, 50, {0,0,255}, 3, 180);
        draw_filled_circle({100,100}, 100);
    }
};

OBJ *object;
void prepare() {
}

void call() {
    if (object) object->call();
    tm++;
    if (tm%(6*(int)FPS) == 0) {
        if (object) delete object;
        object = nullptr;
    }
    else if (tm%(6*(int)FPS) == (int)3*FPS) {
        if (!object) object = new OBJ();
    }
}

void end() {
    if (object) delete object;
}