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
        Bone(Vector3d axis, double length, double theta, int name, Bone *parent);
        Bone(double x, double y, double z, double length, double theta, int name, Bone *parent);
        Matrix4d getTransformationMatrix();
        Matrix4d getTransformationMatrix(double deltheta);
        Matrix4d getWorldTransformationMatrix();
        Matrix4d getWorldTransformationMatrix(VectorXd deltheta);
        void addChild(Bone *child);
        void draw();
        void draw(float radius, int vertices);
        Bone *getChild(int i);
        vector<Bone *> getChildren();
        void addAngle(double theta);
        void addAngles(VectorXd deltheta);
        void moveEffector(VectorXd delpoints);
        double getLength();
        double getTheta();
        int getDepth();
        MatrixXd jacobian(VectorXd deltheta);
        Vector3d getAxis() {return axis;};
        Vector3d getEffectorDerivativeWRT(Bone *joint);
        Vector3d getEffectorDerivativeWRT(Bone *joint, VectorXd deltheta);
        Vector4d getVector();        
        Vector4d getWorldCoords();
        Vector4d getWorldCoords(VectorXd deltheta);
        Vector4d getEffectorWorldCoords();
        Vector4d getEffectorWorldCoords(VectorXd deltheta);
        bool goodSolution(VectorXd guess, MatrixXd jacobian, VectorXd delpoints, float epsilon);
        VectorXd dampedLeastSquares(VectorXd delpoints, float epsilon, int iterations);
        VectorXd solveDamped(MatrixXd jacobian, VectorXd delpoints); 
    private:
        const static Vector4d ORIGIN_VECTOR;
        int name;
        double x,
               y,
               z,
               length,   /* length of bone */
               theta;    /* angle at joint (radians) */
        Vector3d axis;
        Bone *parent;   /* parent bone */
        vector<Bone *> children;    /* child bone */

};
