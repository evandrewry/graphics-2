#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "../../Eigen/Core"

using namespace std;
using Eigen::Matrix4f;

class Bone {
    public:
        Bone(float length, float theta, Bone *parent);
        Matrix4f getTransformationMatrix();
        void addChild(Bone *child);
        void draw(float radius, int vertices);
        Bone *getChild(int i);
        vector<Bone *> getChildren();
        void addAngle(float theta);
    private:
        float length,   /* length of bone */
              theta;    /* angle at joint (radians) */
        Bone *parent;   /* parent bone */
        vector<Bone *> children;    /* child bone */
};

void bones();
