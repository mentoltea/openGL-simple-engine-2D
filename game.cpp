#include "definitions.h"

Point p1, p2;
size_t tm = 0;
size_t pad = -50;

class OBJ: public DrawableObject {
public:
    Point p1, p2;
    size_t length =  -pad + (WINX > WINY ? WINY : WINX)/3;
    void call() {
        if (tm>60*6) RemoveFromVector(this);
        p1.x = WINX/2 + cos((float)tm/30.)*length;
        p1.y = WINY/2 + sin((float)tm/30.)*length;

        p2.x = WINX - p1.x;
        p2.y = WINX - p1.y;
    }
    void draw() override {
        draw_rect(p1, 100, 50);
        draw_circle(p2, 50, {0,0,255}, 3, 180);
        auto diff = p1-p2;
        diff = {-diff.y/4, diff.x/4};
        Point center = {(p1.x+p2.x)/2, (p1.y+p2.y)/2};
        draw_poly({p1, center+diff, p2, center-diff});
    }
};

OBJ object;
void prepare() {
    ObjectsToDraw.push_back(&object);
}

void call() {
    object.call();
    tm++;
}