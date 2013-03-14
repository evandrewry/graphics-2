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

const Vector4d Bone::ORIGIN_VECTOR = Vector4d(0,0,0,1);


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
    return getTransformationMatrix(0.0f);
}

Matrix4d Bone::getTransformationMatrix(float deltheta) {
    Matrix4d t;
    float cosine = cos(this->theta + deltheta);
    float sine = sin(this->theta + deltheta);

    //I*cos(theta) + sin(theta)[u]_x + (1 - cos(theta))u(tensor)u
    t.row(0) << cosine + (axis(0) * axis(0) * (1 - cosine)), 
         (axis(0) * axis(1) * (1 - cosine)) - (axis(2) * sine),  
         (axis(0) * axis(2) * (1 - cosine)) + (axis(1) * sine),
         this->x;
    t.row(1) << (axis(1) * axis(0) * (1 - cosine)) + (axis(2) * sine),
         cosine + (axis(1) * axis(1) * (1 - cosine)),
         (axis(1) * axis(2) * (1 - cosine)) - (axis(0) * sine),
         this->y;
    t.row(2) << (axis(2) * axis(0) * (1 - cosine)) - (axis(1) * sine),
         (axis(2) * axis(1) * (1 - cosine)) + (axis(0) * sine),
         cosine + (axis(2) * axis(2) * (1 - cosine)), 
         this->z;
    t.row(3) << 0, 0, 0, 1; //row 4

    //cout << "sin: " << sine << "  cosine: " << cosine << endl 
    //     << "transform: " <<endl << t << endl << endl;
    return t;
}


void Bone::addChild(Bone *child) {
    this->children.push_back(child);
    child->parent = this;
}

void Bone::draw() {
    float radius = 10.f;
    int vertices = 30;
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

Vector3d Bone::getEffectorDerivativeWRT(Bone *joint) {
    return joint->getAxis().cross(Vector3d(getEffectorWorldCoords().head(3)) - Vector3d(joint->getWorldCoords().head(3)));
}


Vector3d Bone::getEffectorDerivativeWRT(Bone *joint, VectorXd deltheta) {
    return joint->getAxis().cross(Vector3d(getEffectorWorldCoords(deltheta).head(3)) - Vector3d(joint->getWorldCoords(deltheta).head(3)));
}

Vector4d Bone::getWorldCoords() {
    return getWorldTransformationMatrix() * ORIGIN_VECTOR;
}


Vector4d Bone::getEffectorWorldCoords() {
    return getWorldTransformationMatrix() * getVector();
}

Vector4d Bone::getWorldCoords(VectorXd deltheta) {
    return getWorldTransformationMatrix(deltheta) * ORIGIN_VECTOR;
}


Vector4d Bone::getEffectorWorldCoords(VectorXd deltheta) {
    return getWorldTransformationMatrix(deltheta) * getVector();
}

Matrix4d Bone::getWorldTransformationMatrix() {
    Bone *cur = this;
    Matrix4d t = Matrix4d::Identity();
    while (cur != NULL) {
        t = cur->getTransformationMatrix() * t;
        cur = cur->parent;
    }
    return t;
}


Matrix4d Bone::getWorldTransformationMatrix(VectorXd deltheta) {
    Bone *cur = this;
    Matrix4d t = Matrix4d::Identity();
    while (cur != NULL) {
        t = cur->getTransformationMatrix(deltheta(deltheta.size() - 1)) * t;        
        deltheta = VectorXd(deltheta.head(deltheta.size() - 1));
        cur = cur->parent;
    }
    return t;
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
