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

#define EPSILON 0.00001
#define ITERATIONS 20

using namespace std;
using Eigen::Matrix4d;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Vector4d;
using Eigen::Vector3d;

const Vector4d Bone::ORIGIN_VECTOR = Vector4d(0,0,0,1);


Bone::Bone(Vector3d axis, double length, double theta, int name, Bone *parent) {
    float parentlen;
    if (parent == NULL) {
        this->z = 0;
    } else {
        this->z = parent->length;
    }
    this->name = name;
    this->y = 0;
    this->x = 0;
    this->axis = axis / axis.norm();
    this->length = length;
    this->parent = parent;
    if (parent)
        parent->addChild(this);
    this->theta = theta;
}

Bone::Bone(double x, double y, double z, double length, double theta, int name, Bone *parent) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->length = length;
    this->parent = parent;
    if (parent)
        parent->addChild(this);
    this->theta = theta;
}

void Bone::addAngle(double theta) {
    this->theta += theta;
}

Matrix4d Bone::getTransformationMatrix() {
    return getTransformationMatrix(0.0f);
}

Matrix4d Bone::getTransformationMatrix(double deltheta) {
    Matrix4d t;
    double cosine = cos(this->theta + deltheta);
    double sine = sin(this->theta + deltheta);

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
        glLoadName(this->name);        
        gluCylinder(gluNewQuadric(), 10., 0., this->length, 3, this->length);
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

VectorXd Bone::dampedLeastSquares(VectorXd delpoints, float epsilon, int iterations) {
    int depth = getDepth();    
    VectorXd guess = VectorXd::Zero(depth + 1);
    MatrixXd jacobian;
    int i = 0;
    bool found = false;
    do {
        jacobian = this->jacobian(guess);
        guess = solveDamped(jacobian, delpoints);
        found = goodSolution(guess, jacobian, delpoints, epsilon);
        //cout << "guess" << endl << guess << endl << endl;
    } while (i++ <= iterations && !found);
    if (!found) cout<<"FUCK"<<endl;
    return found ? guess : VectorXd::Zero(depth + 1);
}

VectorXd Bone::solveDamped(MatrixXd jacobian, VectorXd delpoints) {
    MatrixXd jtj = jacobian.transpose() * jacobian;
    MatrixXd damper = EPSILON * MatrixXd::Identity(jtj.rows(), jtj.cols());
    return (jtj + damper).inverse() * jacobian.transpose() * delpoints;
}

MatrixXd Bone::jacobian(VectorXd deltheta) {
    Vector4d effector;//, v;
    int depth = getDepth();
    MatrixXd jacobian = MatrixXd::Zero(3, depth + 1);
    //Bone *cur = this;
    effector = this->getEffectorWorldCoords();
    for (Bone *cur = this; cur != NULL; cur = cur->parent, depth--) {
        //v = effector - cur->getWorldCoords();
        //cout << "v_" << i << ": " << endl << v;
        //Vector3d partial = bones[i]->getAxis().cross(Vector3d(v(0),v(1),v(2)));
        //cout << "ds/dtheta_" << i << ": " << endl << partial;
        jacobian.col(depth) = this->getEffectorDerivativeWRT(cur, deltheta);
    }
    return jacobian;

}

int Bone::getDepth() {
    return parent == NULL ? 0 : 1 + parent->getDepth();
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

void Bone::moveEffector(VectorXd delpoints) {
    VectorXd deltheta = this->dampedLeastSquares(delpoints, EPSILON, ITERATIONS);
    cout << getDepth() << " vs " << deltheta.size();
    this->addAngles(deltheta);
}

void Bone::addAngles(VectorXd deltheta) {
    this->addAngle(deltheta(deltheta.size() - 1));
    if (this->parent != NULL)
        this->parent->addAngles(deltheta.head(deltheta.size() - 1));
}

vector<Bone *> Bone::getChildren() {
    return this->children;
}

Bone *Bone::getChild(int i) {
    return i < children.size() ? children[i] : NULL;
}

double Bone::getLength() {
    return this->length;
}

double Bone::getTheta() {
    return this->theta;
}

Vector4d Bone::getVector() {
    return Vector4d(x, y, z, 1);
}

bool Bone::goodSolution(VectorXd guess, MatrixXd jacobian, VectorXd delpoints, float epsilon) {
    double r = (jacobian * guess - delpoints).norm();
    //cout << (r < epsilon) << r <<endl;
    return r < epsilon;
}
