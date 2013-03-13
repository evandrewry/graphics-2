#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "../../Eigen/Dense"

using namespace std;
using Eigen::Matrix4d;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Vector4d;
using Eigen::Vector3d;

class Bone {
    public:
        Bone(Vector3d axis, float length, float theta, Bone *parent);
        Bone(float x, float y, float z, float length, float theta, Bone *parent);
        Matrix4d getTransformationMatrix();
        void addChild(Bone *child);
        void draw();
        void draw(float radius, int vertices);
        Bone *getChild(int i);
        vector<Bone *> getChildren();
        void addAngle(float theta);
        float getLength();
        float getTheta();
        Vector4d getVector();        
    private:
        float x,
              y,
              z,
              length,   /* length of bone */
              theta;    /* angle at joint (radians) */
        Vector3d axis;
        Bone *parent;   /* parent bone */
        vector<Bone *> children;    /* child bone */
};
