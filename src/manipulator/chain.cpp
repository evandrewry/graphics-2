#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include "../../Eigen/Core"
#include "chain.h"

#define BONE_RADIUS 10.0
#define BONE_FACES 30
#define BONE_LENGTH 50.0
#define END_LENGTH (BONE_LENGTH / 5.0) 


using Eigen::Matrix4f;


Chain::Chain(int length) {
    int i;
    Bone *tmp;

    this->length = length;
    this->root = new Bone(BONE_LENGTH, M_PI / 2, NULL);
    tmp = root;
    for (i = 1; i < length; i++) {
        Bone *b = new Bone(BONE_LENGTH, M_PI / 2, tmp);
        tmp = b;
    }
    this->end = new Bone(END_LENGTH, 0, tmp);
}

void Chain::draw() {
    Bone *current = this->root;
    int depth = 0;
    //Matrix4f transform;
    glPushMatrix();
    while (current != NULL) {
        glMultMatrixf(current->getTransformationMatrix().data());
        current->draw(BONE_RADIUS, BONE_FACES);
        //transform = current->getTransformationMatrix();
        current = current->getChild(0);
    }
    glPopMatrix();
}

void Chain::addAngle(int depth, float theta) {
    int i;
    Bone *target = this->root;
    for (i = 0; i < depth; i++) {
        /* avoid null pointer dereference */
        if (target == NULL) return;

        /* move to next link in chain */
        target = target->getChild(0);
    }

    /* do the rotation */
    target->addAngle(theta);
}


