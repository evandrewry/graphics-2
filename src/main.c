//#elif defined(__linux)
#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include "GLScreenCapturer.h"
#include "trackball.h"
#include "lego/lego.h"
#include "lego/models/fan.h"
#include "lego/models/pile.h"
#include "lego/models/horse.h"

#define BUFFER_LENGTH 64
#define GRID_WIDTH 1000
#define GRID_GRANULARITY 40
#define CAMERA_MOVEMENT_GRANULARITY 10

static GLfloat camRotX, camRotY, camRotZ, camPosX, camPosY, camPosZ;
static GLint viewport[4];
static GLdouble modelview[16];
static GLdouble projection[16];
static GLuint pickedObj = -1;
static bool grid_on = false;
static bool pile_on = true;
static bool tessellation_on = true;
static bool horse_on = true;
static bool lego_mode = false;

static GLuint legoDL;
static int howMany = 5;
static const GLfloat *tesselation_colors[4];

static char titleString[150];

// Lights & Materials
const static GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
const static GLfloat position[] = {200, 200, 200, 100};
const static GLfloat position2[] = {-200, -200, -200, -100};
const static GLfloat mat_diffuse[] = {0.6, 0.6, 0.6, 1.0};
const static GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
const static GLfloat mat_shininess[] = {20.0};
const static GLfloat global_ambient[] = { 0.4, 0.4, 0.4, 1 };


static GLScreenCapturer screenshot("screenshot-%d.ppm");

void initLights(void)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_diffuse);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

static void init()
{
    tbInit(GLUT_RIGHT_BUTTON);
    tbAnimate(GL_TRUE);

    // Place Camera
    camRotX = 120.0f;
    camRotY = 180.0f;
    camRotZ = 0.f;
    camPosX = 0.0f;
    camPosY = 0.0f;
    camPosZ = -200.0f;

    glEnable( GL_DEPTH_TEST );
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    initLights();

    legoDL = glGenLists (1);
    glNewList(legoDL, GL_COMPILE);
    {
        GLfloat color[] = {0, 1, 0, 1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);

        lego_pile(howMany);
    }
    glEndList();

    glClearColor(0.0, 0.0, 0.0, 0.0);
}

static void setCamera( void )
{
    glTranslatef(0, 0, camPosZ);
    glRotatef(camRotX, 1, 0, 0);
    glRotatef(camRotY, 0, 1, 0);
    glRotatef(camRotZ, 0, 0, 1);
}


static void grid()
{
    unsigned int x, y;
    glPushMatrix(); {
        glTranslatef(-GRID_WIDTH / 2, -GRID_WIDTH / 2, -1.5 * LEGO_HEIGHT);
        glBegin(GL_QUADS); {
            for (x = 0; x < GRID_WIDTH; ++x)
                for (y = 0; y < GRID_WIDTH; ++y) {
                    if ((x+y) & 0x00000001) //modulo 2
                        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white); 
                    else
                        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
         
                    glNormal3f(0., 0., 1);
                    glVertex3f(x * GRID_GRANULARITY, y * GRID_GRANULARITY, 0);
                    glNormal3f(0., 0., 1);
                    glVertex3f((x+1) * GRID_GRANULARITY, y * GRID_GRANULARITY, 0);
                    glNormal3f(0., 0., 1);
                    glVertex3f((x+1) * GRID_GRANULARITY, (y+1) * GRID_GRANULARITY, 0);
                    glNormal3f(0., 0., 1);
                    glVertex3f(x * GRID_GRANULARITY, (y+1) * GRID_GRANULARITY, 0);
                }
        } glEnd();
    } glPopMatrix();
}

void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushMatrix();
    {
        setCamera();
        tbMatrix();
        
        //in lego mode we display only a single lego for viewing
        if(lego_mode) { 
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellow); 
            lego();
        } else {

            //display checkered floor
            if (grid_on) grid();

            //display tessellation of lego pinwheel shapes
            if (tessellation_on) {
                tesselation_colors[0] = &blue[0]; tesselation_colors[1] = &yellow[0];
                tesselation_colors[2] = &white[0]; tesselation_colors[3] = &green[0];
                lego_fan_tessellation(2, 2, tesselation_colors);
                glPushMatrix();
                    glTranslatef(0, 0, -LEGO_HEIGHT);
                    tesselation_colors[0] = &red[0]; tesselation_colors[1] = &blue[0];
                    tesselation_colors[2] = &black[0]; tesselation_colors[3] = &green[0];
                    lego_fan_tessellation(6,6,tesselation_colors);
                glPopMatrix();
            }

            //display pile of legos
            if (pile_on) {
                glPushMatrix();
                    glTranslatef(LEGO_WIDTH / 4, 0, LEGO_HEIGHT * 2.5);
                    lego_pile(howMany);
                glPopMatrix();
            }

            //display animal thing
            if (horse_on) {
                glTranslatef(-LEGO_UNIT * 8.5, -LEGO_UNIT * 14, LEGO_HEIGHT / 2);
                horse();
            }
        }
        // Retrieve current matrice before they popped.
        glGetDoublev( GL_MODELVIEW_MATRIX, modelview );        // Retrieve The Modelview Matrix
        glGetDoublev( GL_PROJECTION_MATRIX, projection );    // Retrieve The Projection Matrix
        glGetIntegerv( GL_VIEWPORT, viewport );                // Retrieves The Viewport Values (X, Y, Width, Height)
    }
    glPopMatrix();

    glFlush();
    // End Drawing calls
    glutSwapBuffers();

}

void reshape( int w, int h )
{
    tbReshape(w, h);

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the clipping volume
    gluPerspective(60.0f, (GLfloat)w / (GLfloat)h, 1.0f, 500.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void keyboard( unsigned char key, int x, int y )
{
    switch(key)
    {
        case 27: // Escape key
            exit(0);
            break;
        case 'q':
            howMany++;
            printf("draw more\n");
            glutPostRedisplay();
            break;
        case 'Q':
            howMany--;
            printf("draw less\n");
            glutPostRedisplay();
            break;
        case 'k':
            camRotX += CAMERA_MOVEMENT_GRANULARITY;
            glutPostRedisplay();
            break;
        case 'j':
            camRotX -= CAMERA_MOVEMENT_GRANULARITY;
            glutPostRedisplay();
            break;
        case '+':
        case '=':
            camPosZ += CAMERA_MOVEMENT_GRANULARITY;
            glutPostRedisplay();
            break;
        case '-':
        case '_':
            camPosZ -= CAMERA_MOVEMENT_GRANULARITY;
            glutPostRedisplay();
            break;
        case 'h':
            camRotZ += CAMERA_MOVEMENT_GRANULARITY;
            glutPostRedisplay();
            break;
        case 'l':
            camRotZ -= CAMERA_MOVEMENT_GRANULARITY;
            glutPostRedisplay();
            break;
        case 'y':
        case '/':
            camRotY += CAMERA_MOVEMENT_GRANULARITY;
            glutPostRedisplay();
            break;
        case 'Y':
        case '.':
            camRotY -= CAMERA_MOVEMENT_GRANULARITY;
            glutPostRedisplay();
            break;
        case 'i':
        case 'I':
            glLoadIdentity();
            gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);
            break;
        case '4':
            grid_on = !grid_on;
            break;
        case '2':
            pile_on = !pile_on;
            break;
        case '1':
            tessellation_on = !tessellation_on;
            break;
        case '3':
            horse_on = !horse_on;
            break;
        case '5':
            lego_mode = !lego_mode;
            break;
        case 'r':
            printf("save current screen\n");
            screenshot.capture();
            return;
    }
    glutPostRedisplay();
}

void mouse( int button, int state, int x, int y)
{
    tbMouse(button, state, x, y);
}

void motion(int x, int y)
{
    tbMotion(x, y);

    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
    glutPostRedisplay();

}

int main (int argc, char *argv[])
{
    int win_width = 512;
    int win_height = 512;

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize( win_width, win_height );

    glutCreateWindow( "lego shit" );
    init();

    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutMotionFunc( motion );

    glutMainLoop();
}
