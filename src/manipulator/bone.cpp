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
using Eigen::Matrix4d;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Vector4d;
using Eigen::Vector3d;

Bone::Bone(Vector3d axis, float length, float theta, Bone *parent) {
    float parentlen;
    if (parent == NULL) {
        this->z = 0;
    } else {
        this->z = parent->length;
    }
    this->y = 0;
    this->x = 0;
    this->axis = axis / axis.norm();
    this->length = length;
    this->parent = parent;
    if (parent)
        parent->addChild(this);
    this->theta = theta;
}

Bone::Bone(float x, float y, float z, float length, float theta, Bone *parent) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->length = length;
    this->parent = parent;
    if (parent)
        parent->addChild(this);
    this->theta = theta;
}

void Bone::addAngle(float theta) {
    this->theta += theta;
}

Matrix4d Bone::getTransformationMatrix() {
    Matrix4d t;
    float cosine = cos(this->theta);
    float sine = sin(this->theta);

    t << cosine, 0,   sine, this->x,
          -sine, 0, cosine, this->y,
              0, 0,      1, this->z,
              0, 0,      0,       1;
    return t;
}

void Bone::addChild(Bone *child) {
    this->children.push_back(child);
}

void Bone::draw() {
    glPushMatrix();
        glMultMatrixd(getTransformationMatrix().data()); 
        gluCylinder(gluNewQuadric(), 10., 10., this->length, 30, this->length);
        for (int i = 0; i < children.size(); i++) {
            children[i]->draw();
        }
    glPopMatrix();
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

float Bone::getLength() {
    return this->length;
}

float Bone::getTheta() {
    return this->theta;
}

Vector4d Bone::getVector() {
    return Vector4d(x, y, z, 1);
}
