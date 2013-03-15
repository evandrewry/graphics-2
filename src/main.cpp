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
#include "manipulator/chain.h"
#include "../Eigen/Core"

#define BUFFER_LENGTH 64
#define GRID_WIDTH 1000
#define GRID_GRANULARITY 40
#define CAMERA_MOVEMENT_GRANULARITY 10
using Eigen::Vector3d;

static Chain *chain;

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
static bool drag = false;
int dragx, dragy, selected;


static char titleString[150];

// Lights & Materials
const static GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
const static GLfloat position[] = {200, 200, 200, 100};
const static GLfloat position2[] = {-200, -200, -200, -100};
const static GLfloat mat_diffuse[] = {0.6, 0.6, 0.6, 1.0};
const static GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
const static GLfloat mat_shininess[] = {20.0};
const static GLfloat global_ambient[] = { 0.4, 0.4, 0.4, 1 };

//LEGO COLORS
extern const GLfloat green[4]; 
extern const GLfloat yellow[4];
extern const GLfloat white[4]; 
extern const GLfloat cyan[4];
extern const GLfloat red[4];
extern const GLfloat blue[4];
extern const GLfloat black[4];

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
    chain = (new Chain(3));
    tbInit(GLUT_RIGHT_BUTTON);
    tbAnimate(GL_TRUE);

    // Place Camera
    camRotX = -90.0f;
    camRotY = 0.0f;
    camRotZ = 0.f;
    camPosX = 0.0f;
    camPosY = 0.0f;
    camPosZ = -200.0f;

    glEnable( GL_DEPTH_TEST );
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    initLights();


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
        
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellow); 

            chain->draw();

            if (grid_on) grid();
            
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
    gluPerspective(60.0f, (GLfloat)w / (GLfloat)h, 0.1f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}
#define D 6.

void keyboard( unsigned char key, int x, int y )
{
    switch(key)
    {
        case 27: // Escape key
            exit(0);
            break;
        case '0':
            chain->moveEffector(Vector3d(0.,0.,D));
            glutPostRedisplay();
            break;
        case ')':
            chain->moveEffector(Vector3d(0.,0.,-D));
            glutPostRedisplay();
            break;
        case '9':
            chain->moveEffector(Vector3d(0.,D,0.));
            glutPostRedisplay();
            break;
        case '(':
            chain->moveEffector(Vector3d(0.,-D,0.));
            glutPostRedisplay();
            break;
        case '8':
            chain->moveEffector(Vector3d(D,0.,0.));
            glutPostRedisplay();
            break;
        case '*':
            chain->moveEffector(Vector3d(-D,0.,0.));
            glutPostRedisplay();
            break;
        case '1':
            chain->addAngle(0, M_PI / 10.0);
            glutPostRedisplay();
            break;
        case '!':
            chain->addAngle(0, -M_PI / 10.0);
            glutPostRedisplay();
            break;        
        case '2':
            chain->addAngle(1, M_PI / 10.0);
            glutPostRedisplay();
            break;
        case '@':
            chain->addAngle(1, -M_PI / 10.0);
            glutPostRedisplay();
            break;
        case '3':
            chain->addAngle(2, M_PI / 10.0);
            glutPostRedisplay();
            break;
        case '#':
            chain->addAngle(2, -M_PI / 10.0);
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
        case 'r':
            printf("save current screen\n");
            screenshot.capture();
            return;
    }
    glutPostRedisplay();
}


void processHits (GLint hits, GLuint buffer[])
{
    unsigned int i, j;
    GLuint names, *ptr;

    if (hits > 0) drag = true;

    printf ("hits = %d\n", hits);
    ptr = (GLuint *) buffer;
    for (i = 0; i < hits; i++) { /*  for each hit  */
        names = *ptr;
        printf (" number of names for hit = %d\n", names); ptr++;
        printf("  z1 is %g;", (float) *ptr/0x7fffffff); ptr++;
        printf(" z2 is %g\n", (float) *ptr/0x7fffffff); ptr++;
        printf ("   the name is ");
        for (j = 0; j < names; j++) {     /*  for each name */
            printf ("%d ", *ptr); 
            selected = *ptr;            
            ptr++;
        }
        printf ("\n");
    }
}

void processSelection(int xPos, int yPos)
{
    dragx = xPos;
    dragy = yPos;

    GLfloat fAspect;

    // Space for selection buffer
    static GLuint selectBuff[BUFFER_LENGTH];

    // Hit counter and viewport storage
    GLint hits, viewport[4];

    // Setup selection buffer
    glSelectBuffer(BUFFER_LENGTH, selectBuff);

    // Get the viewport
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    // Switch to projection and save the matrix
    glMatrixMode(GL_PROJECTION);
    
    glPushMatrix();
    {
        // Change render mode
        glRenderMode(GL_SELECT);
        
        // Establish new clipping volume to be unit cube around
        // mouse cursor point (xPos, yPos) and extending two pixels
        // in the vertical and horizontal direction
        glLoadIdentity();
        gluPickMatrix(xPos, viewport[3] - yPos + viewport[1], 2.,2., viewport);
        
        // Apply perspective matrix 
        fAspect = (float)viewport[2] / (float)viewport[3];
        gluPerspective(60.0f, fAspect, 1.0, 1000.0);
        
        // Render only those needed for selection
        glPushMatrix();    
        {
            setCamera();
            chain->draw();
        }
        glPopMatrix();
        
        // Collect the hits
        hits = glRenderMode(GL_RENDER);
        processHits(hits, selectBuff);
        // Restore the projection matrix
        glMatrixMode(GL_PROJECTION);
    }
    glPopMatrix();
    
    // Go back to modelview for normal rendering
    glMatrixMode(GL_MODELVIEW);
    
    glutPostRedisplay();
}

void processDrag(int x, int y) {

}

void mouse(int button, int state, int x, int y)
{
    tbMouse(button, state, x, y);
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        processSelection(x, y);
        
    }

    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        drag = false;
        glutPostRedisplay();
    }

}


void motion(int x, int y)
{
    tbMotion(x, y);
    
    if (drag) {
        GLfloat winX, winY, winZ;
        GLdouble posX, posY, posZ, x0, y0, z0;

        winX = (float)x;
        winY = (float)viewport[3] - (float)y;
        glReadPixels( x, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
        cout << "WINZ: " << winZ << endl << endl;
        gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

        winX = (float)dragx;
        winY = (float)viewport[3] - (float)dragy;
        //glReadPixels( dragx, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
        gluUnProject( winX, winY, winZ, modelview, projection, viewport, &x0, &y0, &z0);
        
        Vector3d v = Vector3d(posX, posY, posZ) - Vector3d(x0, y0, z0);
        cout << "down " << dragx << ", " <<dragy << "  , up " << x << ", " << y << endl<<endl;
        cout << Vector3d(x0, y0, z0) << endl <<endl;
        cout << Vector3d(posX, posY, posZ) << endl <<endl;
        cout << v << endl <<endl;
        chain->moveEffector(selected, v);
        dragx = x;
        dragy = y;
    }

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
