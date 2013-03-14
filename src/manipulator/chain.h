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
        void drawChain();
        void addAngle(int depth, float theta);
        void addAngles(VectorXd deltheta);
        VectorXd getAngles();
        MatrixXd jacobian(VectorXd deltheta);
        Vector4d getEndVector(int bone);
        Vector4d getOriginVector(int bone);
        VectorXd dampedLeastSquares(VectorXd delpoints, float epsilon, int iterations);
        VectorXd solveDamped(MatrixXd jacobian, VectorXd delpoints);
        VectorXd getEffectorCoords(VectorXd angles);
        void moveEffector(VectorXd delpoints);
    private:
        Bone *root;
        vector<Bone *> bones;
        const static Vector4d ORIGIN_VECTOR;
        bool goodSolution(VectorXd guess, MatrixXd jacobian, VectorXd delpoints, float epsilon);
};
