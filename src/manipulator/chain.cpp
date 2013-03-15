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
#define EPSILON 0.00001

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

    this->root = new Bone(Vector3d(0.,0.,1.), BONE_LENGTH, 0, NULL);
    tmp = root;
    this->bones.push_back(tmp);
    for (i = 1; i < length; i++) {
        tmp = new Bone(Vector3d(i%2,(i+1)%2,0.), BONE_LENGTH, M_PI / 4, tmp);
        this->bones.push_back(tmp);
    }
    //this->bones.push_back(new Bone(Vector3d(0.,1.,0.), 0, M_PI / 30.0, tmp));
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

void Chain::addAngles(VectorXd deltheta) {
    for (int i = 0; i < bones.size(); i++) {
        bones[i]->addAngle(deltheta(i));
    }
}

void Chain::addAngle(int depth, float theta) {
    bones[depth]->addAngle(theta);
}


MatrixXd Chain::jacobian(VectorXd deltheta) {
    return bones[bones.size() - 1]->jacobian(deltheta);
}

Vector4d Chain::getEndVector(int bone) {
    return bones[bone]->getEffectorWorldCoords();
}

Vector4d Chain::getOriginVector(int bone) {
    return bones[bone]->getWorldCoords();
}

void Chain::moveEffector(VectorXd delpoints) {
    VectorXd deltheta = dampedLeastSquares(delpoints, 0.00001, 20);
    addAngles(deltheta);
}

VectorXd Chain::dampedLeastSquares(VectorXd delpoints, float epsilon, int iterations) {
    VectorXd guess = VectorXd::Zero(bones.size());
    MatrixXd jacobian;
    int i = 0;
    bool found = false;
    do {
        jacobian = this->jacobian(guess);
        guess = solveDamped(jacobian, delpoints);
        found = goodSolution(guess, jacobian, delpoints, epsilon);
        cout << "guess" << endl << guess << endl << endl;
    } while (i++ <= iterations && !found);
    if (!found) cout<<"FUCK"<<endl;
    return found ? guess : VectorXd::Zero(bones.size());
}

VectorXd Chain::solveDamped(MatrixXd jacobian, VectorXd delpoints) {
    MatrixXd jtj = jacobian.transpose() * jacobian;
    MatrixXd damper = EPSILON * MatrixXd::Identity(jtj.rows(), jtj.cols());
    return (jtj + damper).inverse() * jacobian.transpose() * delpoints;
}

bool Chain::goodSolution(VectorXd guess, MatrixXd jacobian, VectorXd delpoints, float epsilon) {
    double r = (jacobian * guess - delpoints).norm();
    cout << (r < epsilon) << r <<endl;
    return r < epsilon;
}



