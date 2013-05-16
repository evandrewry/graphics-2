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
#define EPSILON 0.000001

using namespace std;
using Eigen::Matrix4d;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Vector4d;
using Eigen::ColPivHouseholderQR;

/*
 *
 */
Chain::Chain(int length) {
    int i;
    Bone *tmp;

    this->root = new Bone(Vector3d(0.,1.,0.), BONE_LENGTH, 0, 1, NULL);
    tmp = root;
    this->bones.push_back(tmp);
    for (i = 1; i < length; i++) {
        tmp = new Bone(Vector3d(0.,1.,0.), BONE_LENGTH, M_PI / 4, bones.size() + 1, tmp);
        this->bones.push_back(tmp);
    }
}

/*
 *
 */
void Chain::draw() {
    glPushMatrix();
    this->root->drawSelectable();
    glPopMatrix();
}

/*
 *
 */
void Chain::addAngle(int name, float theta) {
    bones[name - 1]->addAngle(theta);
}

/*
 *
 */
void Chain::moveEffector(VectorXd delpoints) {
    bones[bones.size() - 1]->moveEffectorIncrementally(delpoints);
}


/*
 *
 */
void Chain::moveEffector(int name, VectorXd delpoints) {
    bones[name - 1]->moveEffector(delpoints);
}
