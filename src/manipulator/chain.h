#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include "../../Eigen/Core"
#include "bone.h"
using Eigen::Matrix4d;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Vector4d;

class Chain {
    public:
        Chain(int length);
        void draw();
        void addAngle(int name, float theta);
        void moveEffector(VectorXd delpoints);
        void moveEffector(int name, VectorXd delpoints);        
    private:
        Bone *root;
        vector<Bone *> bones;
};
