#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include "bone.h"

class Chain {
    public:
        Chain(int length);
        void draw();
        void addAngle(int depth, float theta);
    private:
        Bone *root;
        Bone *end;
        unsigned int length;
};
