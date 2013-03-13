#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "../../Eigen/LU"
#include "chain.h"

#define BONE_RADIUS 10.0
#define BONE_FACES 30
#define BONE_LENGTH 50.0
#define END_LENGTH (BONE_LENGTH / 5.0) 
#define EPSILON 0.001

using namespace std;
using Eigen::Matrix4d;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Vector4d;
using Eigen::ColPivHouseholderQR;


const Vector4d Chain::ORIGIN_VECTOR = Vector4d(0,0,0,1);

Chain::Chain(int length) {
    int i;
    Bone *tmp;

    this->root = new Bone(Vector3d(BONE_LENGTH, M_PI / 2, NULL);
    tmp = root;
    this->bones.push_back(tmp);
    for (i = 1; i < length; i++) {
        tmp = new Bone(BONE_LENGTH, M_PI / 2, tmp);
        this->bones.push_back(tmp);
    }
    this->bones.push_back(new Bone(END_LENGTH, M_PI / 30.0, tmp));
}

void Chain::drawChain() {
    int i;
    glPushMatrix();
    for (i = 0; i < bones.size(); i++) {
        glMultMatrixd(bones[i]->getTransformationMatrix().data());
        bones[i]->draw(BONE_RADIUS, BONE_FACES);
    }
    glPopMatrix();
}

void Chain::draw() {
    glPushMatrix();
    this->root->draw();
    glPopMatrix();
}


void Chain::addAngle(int depth, float theta) {
    bones[depth]->addAngle(theta);
}


MatrixXd Chain::jacobian(VectorXd deltheta) {
    int i;
    MatrixXd jacobian = MatrixXd::Zero(3,bones.size());
    Vector4d s_j, p_i;
    s_j = this->getEndVector(bones.size() - 1);
    for (i = 0; i < bones.size(); i++) {
        p_i = this->getOriginVector(i);
        Vector3d partial = bones[i].getAxis().cross(s_j - p_i).norm() * sin(bones[i]->getTheta() + deltheta(i))
        jacobian(0, i) = 0;
        jacobian(1, i) = 0;
        jacobian(2, i) = ;
    }
    return jacobian;
}

Vector4d Chain::getEndVector(int bone) {
    return getOriginVector(bone) + bones[bone]->getVector();
}

Vector4d Chain::getOriginVector(int bone) {
    int i;
    Matrix4d t = Matrix4d::Identity();
    for (i = 0; i < bone; i++) {
        t = t * bones[i]->getTransformationMatrix();
    }
    return t * ORIGIN_VECTOR;
}

VectorXd Chain::dampedLeastSquares(VectorXd delpoints, float epsilon, int iterations) {
    VectorXd guess = VectorXd::Zero(bones.size());
    MatrixXd jacobian;
    VectorXd solution;
    int i = 0;
    do {
        jacobian = this->jacobian(guess);
        solution = solveDamped(jacobian, delpoints);
        cout << solution << endl << endl << jacobian * solution << endl << endl;
    } while (i++ <= iterations && !goodSolution(solution, jacobian, delpoints, epsilon));
    return solution;
}

VectorXd Chain::solveDamped(MatrixXd jacobian, VectorXd delpoints) {
    MatrixXd jtj = jacobian.transpose() * jacobian;
    MatrixXd damper = EPSILON * MatrixXd::Identity(jtj.rows(), jtj.cols());
    return (jtj + damper).inverse() * jacobian.transpose() * delpoints;
}

bool Chain::goodSolution(VectorXd guess, MatrixXd jacobian, VectorXd delpoints, float epsilon) {
    float r = (jacobian * guess - delpoints).norm();
    return r * r < epsilon;
}



