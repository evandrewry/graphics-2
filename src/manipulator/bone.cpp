#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "../../Eigen/Core"
#include "bone.h"

using namespace std;
using Eigen::Matrix4f;

Bone::Bone(float length, float theta, Bone *parent) {
    this->length = length;
    this->parent = parent;
    if (parent)
        parent->addChild(this);
    this->theta = theta;
}

void Bone::addAngle(float theta) {
    this->theta += theta;
}

Matrix4f Bone::getTransformationMatrix() {
    Matrix4f t;
    float cosine = cos(this->theta);
    float sine = sin(this->theta);
    float parentlen;
    if (this->parent == NULL) {
        parentlen = 0;
    } else {
        parentlen = this->parent->length;
    }
    t << cosine, 0,   sine, parentlen,
              0, 1,      0,         0,
          -sine, 0, cosine,         0,
              0, 0,      0,         1;
    return t;
}

void Bone::addChild(Bone *child) {
    this->children.push_back(child);
}

void Bone::draw(float radius, int vertices) {
    glPushMatrix(); {
        glBegin(GL_QUAD_STRIP); {
            int i;
            for (i = 0; i < vertices; i ++) {
                float y = radius * cos(2 * M_PI * i / (float) vertices);
                float z = radius * sin(2 * M_PI * i / (float) vertices);
                glNormal3f(0, y, z);
                glVertex3f(0, y, z);
                glNormal3f(0, y, z);
                glVertex3f(this->length, y, z);
                //printf("len: %f y: %f z: %f" , this->length, y, z);
            }
            float y = radius * cos(0);
            float z = radius * sin(0);
            glNormal3f(0, y, z);
            glVertex3f(0, y, z);
            glNormal3f(0, y, z);
            glVertex3f(this->length, y, z);
        } glEnd();
    } glPopMatrix();
}

vector<Bone *> Bone::getChildren() {
    return this->children;
}

Bone *Bone::getChild(int i) {
    return i < children.size() ? children[i] : NULL;
}
