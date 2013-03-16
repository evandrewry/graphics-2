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

    this->root = new Bone(Vector3d(0.,0.,1.), BONE_LENGTH, 0, 1, NULL);
    tmp = root;
    this->bones.push_back(tmp);
    for (i = 1; i < length; i++) {
        tmp = new Bone(Vector3d(i%2,(i+1)%2,0.), BONE_LENGTH, M_PI / 4, i + 1, tmp);
        this->bones.push_back(tmp);
    }
    
    tmp = root;
    for (i = 1; i < length; i++) {
        tmp = new Bone(Vector3d(i%2,(i+1)%2,0.), BONE_LENGTH, -M_PI / 4, i + length, tmp);
        this->bones.push_back(tmp);
    }

}

//void Chain::drawChain() {
    //int i;
    //glPushMatrix();
    //for (i = 0; i < bones.size(); i++) {
        //glMultMatrixd(bones[i]->getTransformationMatrix().data());
        //bones[i]->draw(BONE_RADIUS, BONE_FACES);
    //}
    //glPopMatrix();
//}

void Chain::draw() {
    glPushMatrix();
    this->root->drawSelectable();
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
    VectorXd deltheta = bones[bones.size() - 1]->dampedLeastSquares(delpoints, 0.00001, 20);
    addAngles(deltheta);
}


void Chain::moveEffector(int name, VectorXd delpoints) {
    bones[name - 1]->moveEffectorIncrementally(delpoints);
}
