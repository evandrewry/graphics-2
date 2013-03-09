#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include "lego.h"

static bool initialized = false;

//LEGO COLORS
const GLfloat green[4] = {40./255, 127./255, 70./255, 1};
const GLfloat yellow[4] = {1, 1, 0, 1};
const GLfloat white[4] = {1, 1, 1, 1};
const GLfloat cyan[4] = {0, 1, 1, 1};
const GLfloat red[4] = {1, 0, 0, 1};
const GLfloat blue[4] = {0, 0, 1, 1};
const GLfloat black[4] = {0.1, 0.1, 0.1, 1};

//LEGO VERTICES

//main eight
static GLfloat vertex_a[3] = {
    -LEGO_LENGTH/2,
    -LEGO_WIDTH/2,
    -LEGO_HEIGHT/2
};
static GLfloat vertex_b[3] = {
    -LEGO_LENGTH/2,
    -LEGO_WIDTH/2,
    LEGO_HEIGHT/2
};
static GLfloat vertex_c[3] = {
    LEGO_LENGTH/2,
    -LEGO_WIDTH/2,
    -LEGO_HEIGHT/2
};
static GLfloat vertex_d[3] = {
    LEGO_LENGTH/2,
    -LEGO_WIDTH/2,
    LEGO_HEIGHT/2
};
static GLfloat vertex_e[3] = {
    LEGO_LENGTH/2,
    LEGO_WIDTH/2,
    -LEGO_HEIGHT/2
};
static GLfloat vertex_f[3] = {
    LEGO_LENGTH/2,
    LEGO_WIDTH/2,
    LEGO_HEIGHT/2
};
static GLfloat vertex_g[3] = {
    -LEGO_LENGTH/2,
    LEGO_WIDTH/2,
    -LEGO_HEIGHT/2
};
static GLfloat vertex_h[3] = {
    -LEGO_LENGTH/2,
    LEGO_WIDTH/2,
    LEGO_HEIGHT/2
};
static GLfloat mini_vertex_a[3] = {
    -LEGO_UNIT/2,
    -LEGO_UNIT/2,
    -LEGO_HEIGHT/2
};
static GLfloat mini_vertex_b[3] = {
    -LEGO_UNIT/2,
    -LEGO_UNIT/2,
    LEGO_HEIGHT/2
};
static GLfloat mini_vertex_c[3] = {
    LEGO_UNIT/2,
    -LEGO_UNIT/2,
    -LEGO_HEIGHT/2
};
static GLfloat mini_vertex_d[3] = {
    LEGO_UNIT/2,
    -LEGO_UNIT/2,
    LEGO_HEIGHT/2
};
static GLfloat mini_vertex_e[3] = {
    LEGO_UNIT/2,
    LEGO_UNIT/2,
    -LEGO_HEIGHT/2
};
static GLfloat mini_vertex_f[3] = {
    LEGO_UNIT/2,
    LEGO_UNIT/2,
    LEGO_HEIGHT/2
};
static GLfloat mini_vertex_g[3] = {
    -LEGO_UNIT/2,
    LEGO_UNIT/2,
    -LEGO_HEIGHT/2
};
static GLfloat mini_vertex_h[3] = {
    -LEGO_UNIT/2,
    LEGO_UNIT/2,
    LEGO_HEIGHT/2
};


//inner vertices
static GLfloat inner_vertex_a[3] = {
    -(LEGO_LENGTH/2 - WALL_WIDTH),
    -(LEGO_WIDTH/2 - WALL_WIDTH),
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_ag[3] = {
    -LEGO_LENGTH/2,
    -(LEGO_WIDTH/2 - WALL_WIDTH),
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_ac[3] = {
    -(LEGO_LENGTH/2 - WALL_WIDTH),
    -LEGO_WIDTH/2,
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_c[3] =  {
    LEGO_LENGTH/2 - WALL_WIDTH,
    -(LEGO_WIDTH/2 - WALL_WIDTH),
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_ca[3] = {
    LEGO_LENGTH/2 - WALL_WIDTH,
    -LEGO_WIDTH/2,
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_ce[3] = {
    LEGO_LENGTH/2,
    -(LEGO_WIDTH/2 - WALL_WIDTH),
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_e[3] = {
    LEGO_LENGTH/2 - WALL_WIDTH,
    LEGO_WIDTH/2 - WALL_WIDTH,
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_ec[3] = {
    LEGO_LENGTH/2,
    LEGO_WIDTH/2 - WALL_WIDTH,
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_eg[3] = {
    LEGO_LENGTH/2 - WALL_WIDTH,
    LEGO_WIDTH/2,
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_g[3] =  {
    -(LEGO_LENGTH/2 - WALL_WIDTH),
    LEGO_WIDTH/2 - WALL_WIDTH,
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_ge[3] = {
    -(LEGO_LENGTH/2 - WALL_WIDTH),
    LEGO_WIDTH/2,
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_ga[3] = {
    -LEGO_LENGTH/2,
    LEGO_WIDTH/2 - WALL_WIDTH,
    -LEGO_HEIGHT/2
};
static GLfloat inner_vertex_b[3] =  {
    -(LEGO_LENGTH/2 - WALL_WIDTH),
    -(LEGO_WIDTH/2 - WALL_WIDTH),
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_bd[3] = {
   -(LEGO_LENGTH/2 - WALL_WIDTH),
   -LEGO_WIDTH/2,
   LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_bh[3] = {
    -LEGO_LENGTH/2,
    -(LEGO_WIDTH/2 - WALL_WIDTH),
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_h[3] =  {
    -(LEGO_LENGTH/2 - WALL_WIDTH),
    LEGO_WIDTH/2 - WALL_WIDTH,
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_hd[3] = {
    -(LEGO_LENGTH/2 - WALL_WIDTH),
    -LEGO_WIDTH/2,
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_hh[3] = {
    -LEGO_LENGTH/2,
    -(LEGO_WIDTH/2 - WALL_WIDTH),
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_d[3] =  {
    LEGO_LENGTH/2 - WALL_WIDTH,
    -(LEGO_WIDTH/2 - WALL_WIDTH),
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_db[3] = {
    LEGO_LENGTH/2 - WALL_WIDTH,
    -LEGO_WIDTH/2,
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_df[3] = {
    LEGO_LENGTH/2,
    -(LEGO_WIDTH/2 - WALL_WIDTH),
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_f[3] =  {
    LEGO_LENGTH/2 - WALL_WIDTH,
    LEGO_WIDTH/2 - WALL_WIDTH,
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_fh[3] = {
    LEGO_LENGTH/2 - WALL_WIDTH,
    LEGO_WIDTH/2,
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat inner_vertex_fd[3] = {
    LEGO_LENGTH/2,
    LEGO_WIDTH/2 - WALL_WIDTH,
    LEGO_HEIGHT/2 - WALL_WIDTH
};

static GLfloat mini_inner_vertex_a[3] = {
    -(LEGO_UNIT/2 - WALL_WIDTH),
    -(LEGO_UNIT/2 - WALL_WIDTH),
    -LEGO_HEIGHT/2
};
static GLfloat mini_inner_vertex_ag[3] = {
    -LEGO_UNIT/2,
    -(LEGO_UNIT/2 - WALL_WIDTH),
    -LEGO_HEIGHT/2
};
static GLfloat mini_inner_vertex_ac[3] = {
    -(LEGO_UNIT/2 - WALL_WIDTH),
    -LEGO_UNIT/2,
    -LEGO_HEIGHT/2
};
static GLfloat mini_inner_vertex_c[3] =  {
    LEGO_UNIT/2 - WALL_WIDTH,
    -(LEGO_UNIT/2 - WALL_WIDTH),
    -LEGO_HEIGHT/2
};
static GLfloat mini_inner_vertex_ca[3] = {
    LEGO_UNIT/2 - WALL_WIDTH,
    -LEGO_UNIT/2,
    -LEGO_HEIGHT/2
};
static GLfloat mini_inner_vertex_ce[3] = {
    LEGO_UNIT/2,
    -(LEGO_UNIT/2 - WALL_WIDTH),
    -LEGO_HEIGHT/2
};
static GLfloat mini_inner_vertex_e[3] = {
    LEGO_UNIT/2 - WALL_WIDTH,
    LEGO_UNIT/2 - WALL_WIDTH,
    -LEGO_HEIGHT/2
};
static GLfloat mini_inner_vertex_ec[3] = {
    LEGO_UNIT/2,
    LEGO_UNIT/2 - WALL_WIDTH,
    -LEGO_HEIGHT/2
};
static GLfloat mini_inner_vertex_eg[3] = {
    LEGO_UNIT/2 - WALL_WIDTH,
    LEGO_UNIT/2,
    -LEGO_HEIGHT/2
};
static GLfloat mini_inner_vertex_g[3] =  {
    -(LEGO_UNIT/2 - WALL_WIDTH),
    LEGO_UNIT/2 - WALL_WIDTH,
    -LEGO_HEIGHT/2
};
static GLfloat mini_inner_vertex_ge[3] = {
    -(LEGO_UNIT/2 - WALL_WIDTH),
    LEGO_UNIT/2,
    -LEGO_HEIGHT/2
};
static GLfloat mini_inner_vertex_ga[3] = {
    -LEGO_UNIT/2,
    LEGO_UNIT/2 - WALL_WIDTH,
    -LEGO_HEIGHT/2
};
static GLfloat mini_inner_vertex_b[3] =  {
    -(LEGO_UNIT/2 - WALL_WIDTH),
    -(LEGO_UNIT/2 - WALL_WIDTH),
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat mini_inner_vertex_bd[3] = {
   -(LEGO_UNIT/2 - WALL_WIDTH),
   -LEGO_UNIT/2,
   LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat mini_inner_vertex_bh[3] = {
    -LEGO_UNIT/2,
    -(LEGO_UNIT/2 - WALL_WIDTH),
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat mini_inner_vertex_h[3] =  {
    -(LEGO_UNIT/2 - WALL_WIDTH),
    LEGO_UNIT/2 - WALL_WIDTH,
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat mini_inner_vertex_hd[3] = {
    -(LEGO_UNIT/2 - WALL_WIDTH),
    -LEGO_UNIT/2,
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat mini_inner_vertex_hh[3] = {
    -LEGO_UNIT/2,
    -(LEGO_UNIT/2 - WALL_WIDTH),
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat mini_inner_vertex_d[3] =  {
    LEGO_UNIT/2 - WALL_WIDTH,
    -(LEGO_UNIT/2 - WALL_WIDTH),
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat mini_inner_vertex_db[3] = {
    LEGO_UNIT/2 - WALL_WIDTH,
    -LEGO_UNIT/2,
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat mini_inner_vertex_df[3] = {
    LEGO_UNIT/2,
    -(LEGO_UNIT/2 - WALL_WIDTH),
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat mini_inner_vertex_f[3] =  {
    LEGO_UNIT/2 - WALL_WIDTH,
    LEGO_UNIT/2 - WALL_WIDTH,
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat mini_inner_vertex_fh[3] = {
    LEGO_UNIT/2 - WALL_WIDTH,
    LEGO_UNIT/2,
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat mini_inner_vertex_fd[3] = {
    LEGO_UNIT/2,
    LEGO_UNIT/2 - WALL_WIDTH,
    LEGO_HEIGHT/2 - WALL_WIDTH
};

static GLfloat *lego_side_vertices[8] = {
    &vertex_a[0], &vertex_b[0],
    &vertex_c[0], &vertex_d[0],
    &vertex_e[0], &vertex_f[0],
    &vertex_g[0], &vertex_h[0]
};
static GLfloat *lego_base_corner_vertices[4][4] = {
    {   &inner_vertex_a[0],
        &inner_vertex_ac[0],
        &vertex_a[0],
        &inner_vertex_ag[0]
    },{ &inner_vertex_c[0],
        &inner_vertex_ce[0],
        &vertex_c[0],
        &inner_vertex_ca[0]
    },{ &inner_vertex_e[0],
        &inner_vertex_eg[0],
        &vertex_e[0],
        &inner_vertex_ec[0]
    },{ &inner_vertex_g[0],
        &inner_vertex_ga[0],
        &vertex_g[0],
        &inner_vertex_ge[0]
    }
};

static GLfloat *lego_base_edge_vertices[4][4] = {
    {   &inner_vertex_a[0],
        &inner_vertex_ac[0],
        &inner_vertex_ca[0],
        &inner_vertex_c[0]
    },{ &inner_vertex_c[0],
        &inner_vertex_ce[0],
        &inner_vertex_ec[0],
        &inner_vertex_e[0]
    },{ &inner_vertex_e[0],
        &inner_vertex_eg[0],
        &inner_vertex_ge[0],
        &inner_vertex_g[0]
    },{ &inner_vertex_g[0],
        &inner_vertex_ga[0],
        &inner_vertex_ag[0],
        &inner_vertex_a[0]
    }
};

static GLfloat *lego_inner_face_vertices[8] = {
    &inner_vertex_a[0],
    &inner_vertex_b[0],
    &inner_vertex_c[0],
    &inner_vertex_d[0],
    &inner_vertex_e[0],
    &inner_vertex_f[0],
    &inner_vertex_g[0],
    &inner_vertex_h[0]
};
static GLfloat *mini_lego_side_vertices[8] = {
    &mini_vertex_a[0], &mini_vertex_b[0],
    &mini_vertex_c[0], &mini_vertex_d[0],
    &mini_vertex_e[0], &mini_vertex_f[0],
    &mini_vertex_g[0], &mini_vertex_h[0]
};
static GLfloat *mini_lego_base_corner_vertices[4][4] = {
    {   &mini_inner_vertex_a[0],
        &mini_inner_vertex_ac[0],
        &mini_vertex_a[0],
        &mini_inner_vertex_ag[0]
    },{ &mini_inner_vertex_c[0],
        &mini_inner_vertex_ce[0],
        &mini_vertex_c[0],
        &mini_inner_vertex_ca[0]
    },{ &mini_inner_vertex_e[0],
        &mini_inner_vertex_eg[0],
        &mini_vertex_e[0],
        &mini_inner_vertex_ec[0]
    },{ &mini_inner_vertex_g[0],
        &mini_inner_vertex_ga[0],
        &mini_vertex_g[0],
        &mini_inner_vertex_ge[0]
    }
};

static GLfloat *mini_lego_base_edge_vertices[4][4] = {
    {   &mini_inner_vertex_a[0],
        &mini_inner_vertex_ac[0],
        &mini_inner_vertex_ca[0],
        &mini_inner_vertex_c[0]
    },{ &mini_inner_vertex_c[0],
        &mini_inner_vertex_ce[0],
        &mini_inner_vertex_ec[0],
        &mini_inner_vertex_e[0]
    },{ &mini_inner_vertex_e[0],
        &mini_inner_vertex_eg[0],
        &mini_inner_vertex_ge[0],
        &mini_inner_vertex_g[0]
    },{ &mini_inner_vertex_g[0],
        &mini_inner_vertex_ga[0],
        &mini_inner_vertex_ag[0],
        &mini_inner_vertex_a[0]
    }
};

static GLfloat *mini_lego_inner_face_vertices[8] = {
    &mini_inner_vertex_a[0],
    &mini_inner_vertex_b[0],
    &mini_inner_vertex_c[0],
    &mini_inner_vertex_d[0],
    &mini_inner_vertex_e[0],
    &mini_inner_vertex_f[0],
    &mini_inner_vertex_g[0],
    &mini_inner_vertex_h[0]
};


static GLfloat peg_base_center[3] = {
    0,
    0,
    LEGO_HEIGHT/2
};

static GLfloat peg_top_center[3] = {
    0,
    0,
    LEGO_HEIGHT/2 + PEG_HEIGHT
};

static GLfloat inner_peg_base_center[3] = {
    0,
    0,
    -LEGO_HEIGHT/2
};

static GLfloat inner_peg_top_center[3] = {
    0,
    0,
    LEGO_HEIGHT/2 - WALL_WIDTH
};
static GLfloat peg_base_vertices[NUM_PEG_VERTICES][3];
static GLfloat peg_top_vertices[NUM_PEG_VERTICES][3];
static GLfloat peg_face_vertices[NUM_PEG_VERTICES * 2][3];

static GLfloat inner_peg_base_vertices[NUM_PEG_VERTICES][3];
static GLfloat inner_peg_top_vertices[NUM_PEG_VERTICES][3];
static GLfloat inner_peg_face_vertices[NUM_PEG_VERTICES * 2][3];
static GLfloat inner_inner_peg_base_vertices[NUM_PEG_VERTICES][3];
static GLfloat inner_inner_peg_top_vertices[NUM_PEG_VERTICES][3];
static GLfloat inner_inner_peg_face_vertices[NUM_PEG_VERTICES * 2][3];
static GLfloat inner_peg_base_face_vertices[NUM_PEG_VERTICES * 2][3];


//draw outer faces of mini lego
static void mini_lego_sides()
{
    int i;
    GLfloat normal[3] = {0, 0, 0};

    glBegin(GL_QUADS); {
        for (i = 0; i < 8; i += 2) {
            switch (i) {
                case 0: normal[1] = -1; break;
                case 2: normal[0] = 1; break;
                case 4: normal[1] = 1; break;
                case 6: normal[0] = -1; break;
                default: exit(1);
            }
            glNormal3fv(&normal[0]);
            glVertex3fv(&mini_lego_side_vertices[i][0]);
            glNormal3fv(&normal[0]);
            glVertex3fv(&mini_lego_side_vertices[(i + 1) % 8][0]);
            glNormal3fv(&normal[0]);
            glVertex3fv(&mini_lego_side_vertices[(i + 3) % 8][0]);
            glNormal3fv(&normal[0]);
            glVertex3fv(&mini_lego_side_vertices[(i + 2) % 8][0]);
        }
    } glEnd();
}

//draw outer faces of lego
static void lego_sides()
{
    int i;
    GLfloat normal[3] = {0, 0, 0};

    glBegin(GL_QUADS); {
        for (i = 0; i < 8; i += 2) {
            switch (i) {
                case 0: normal[1] = -1; break;
                case 2: normal[0] = 1; break;
                case 4: normal[1] = 1; break;
                case 6: normal[0] = -1; break;
                default: exit(1);
            }
            glNormal3fv(&normal[0]);
            glVertex3fv(&lego_side_vertices[i][0]);
            glNormal3fv(&normal[0]);
            glVertex3fv(&lego_side_vertices[(i + 1) % 8][0]);
            glNormal3fv(&normal[0]);
            glVertex3fv(&lego_side_vertices[(i + 3) % 8][0]);
            glNormal3fv(&normal[0]);
            glVertex3fv(&lego_side_vertices[(i + 2) % 8][0]);
        }
    } glEnd();
}

//draw top face of mini lego
static void mini_lego_top()
{
    glBegin(GL_QUADS); {
        glNormal3f(0.,0.,1.0);
        glVertex3fv(mini_vertex_b);
        glNormal3f(0.,0.,1.0);
        glVertex3fv(mini_vertex_d);
        glNormal3f(0.,0.,1.0);
        glVertex3fv(mini_vertex_f);
        glNormal3f(0.,0.,1.0);
        glVertex3fv(mini_vertex_h);
    } glEnd();
}

//draw inner top face of mini lego
static void mini_lego_top_inner()
{
    glBegin(GL_QUADS); {
        glNormal3f(0.,0.,-1.0);
        glVertex3fv(mini_inner_vertex_b);
        glNormal3f(0.,0.,-1.0);
        glVertex3fv(mini_inner_vertex_d);
        glNormal3f(0.,0.,-1.0);
        glVertex3fv(mini_inner_vertex_f);
        glNormal3f(0.,0.,-1.0);
        glVertex3fv(mini_inner_vertex_h);
    } glEnd();
}


//draw top face of lego
static void lego_top()
{
    glBegin(GL_QUADS); {
        glNormal3f(0.,0.,1.0);
        glVertex3fv(vertex_b);
        glNormal3f(0.,0.,1.0);
        glVertex3fv(vertex_d);
        glNormal3f(0.,0.,1.0);
        glVertex3fv(vertex_f);
        glNormal3f(0.,0.,1.0);
        glVertex3fv(vertex_h);
    } glEnd();
}

//draw inner top face of lego
static void lego_top_inner()
{
    glBegin(GL_QUADS); {
        glNormal3f(0.,0.,-1.0);
        glVertex3fv(inner_vertex_b);
        glNormal3f(0.,0.,-1.0);
        glVertex3fv(inner_vertex_d);
        glNormal3f(0.,0.,-1.0);
        glVertex3fv(inner_vertex_f);
        glNormal3f(0.,0.,-1.0);
        glVertex3fv(inner_vertex_h);
    } glEnd();
}



//subtracts vectors
static GLfloat *subtractv(GLfloat *v1, GLfloat *v2, int len)
{
    int i;
    GLfloat *out = (GLfloat *) malloc(len * sizeof(GLfloat));
    for (i = 0; i < len; i++) {
        out[i] = v1[i] - v2[i];
    }
    return out;
}

//negates vector
static GLfloat *negatev(GLfloat *vector, int len)
{
    int i;
    GLfloat *out = (GLfloat *) malloc(len * sizeof(GLfloat));
    for (i = 0; i < len; i++) {
        out[i] = -vector[i];
    }
    return out;
}

//draw inner faces of lego
static void lego_inner_faces()
{   int i;
    GLfloat normal[3] = {0, 0, 0};

    glBegin(GL_QUADS); {
        for (i = 0; i < 8; i += 2) {
            //calculate normal vector
            switch (i) {
                case 0: normal[1] = 1; break;
                case 2: normal[0] = -1; break;
                case 4: normal[1] = -1; break;
                case 6: normal[0] = 1; break;
                default: exit(1);
            }
            glNormal3fv(&normal[0]);
            glVertex3fv(&lego_inner_face_vertices[i][0]);
            glNormal3fv(&normal[0]);
            glVertex3fv(&lego_inner_face_vertices[(i + 1) % 8][0]);
            glNormal3fv(&normal[0]);
            glVertex3fv(&lego_inner_face_vertices[(i + 3) % 8][0]);
            glNormal3fv(&normal[0]);
            glVertex3fv(&lego_inner_face_vertices[(i + 2) % 8][0]);
        }
    } glEnd();
}


//draw inner faces of mini lego
static void mini_lego_inner_faces()
{   
    int i;
    GLfloat normal[3] = {0, 0, 0};

    glBegin(GL_QUADS); {
        for (i = 0; i < 8; i += 2) {
            switch (i) {
                case 0: normal[1] = 1; break;
                case 2: normal[0] = -1; break;
                case 4: normal[1] = -1; break;
                case 6: normal[0] = 1; break;
                default: exit(1);
            }
            glNormal3fv(&normal[0]);
            glVertex3fv(&mini_lego_inner_face_vertices[i][0]);
            glNormal3fv(&normal[0]);
            glVertex3fv(&mini_lego_inner_face_vertices[(i + 1) % 8][0]);
            glNormal3fv(&normal[0]);
            glVertex3fv(&mini_lego_inner_face_vertices[(i + 3) % 8][0]);
            glNormal3fv(&normal[0]);
            glVertex3fv(&mini_lego_inner_face_vertices[(i + 2) % 8][0]);
        }
    } glEnd();
}

//draw strut thing connecting the inner peg to the inner faces
static void strut()
{
    int i;
    glBegin(GL_QUADS); {
        //front face
        glNormal3f(-1,0,0);
        glVertex3f(-STRUT_WIDTH / 2, -STRUT_LENGTH / 2, LEGO_HEIGHT / 2 - WALL_WIDTH);
        glNormal3f(-1,0,0);
        glVertex3f(-STRUT_WIDTH / 2, STRUT_LENGTH / 2, LEGO_HEIGHT / 2 - WALL_WIDTH);
        glNormal3f(-1,0,0);
        glVertex3f(-STRUT_WIDTH / 2, STRUT_LENGTH / 2, LEGO_HEIGHT / 2 - WALL_WIDTH - STRUT_HEIGHT);
        glNormal3f(-1,0,0);
        glVertex3f(-STRUT_WIDTH / 2, -STRUT_LENGTH / 2, LEGO_HEIGHT / 2 - WALL_WIDTH - STRUT_HEIGHT);

        
        //skinny edge face
        glNormal3f(0,0,-1);
        glVertex3f(-STRUT_WIDTH / 2, STRUT_LENGTH / 2, LEGO_HEIGHT / 2 - WALL_WIDTH - STRUT_HEIGHT);
        glNormal3f(0,0,-1);
        glVertex3f(-STRUT_WIDTH / 2, -STRUT_LENGTH / 2, LEGO_HEIGHT / 2 - WALL_WIDTH - STRUT_HEIGHT);
        glNormal3f(0,0,-1);
        glVertex3f(STRUT_WIDTH / 2, -STRUT_LENGTH / 2, LEGO_HEIGHT / 2 - WALL_WIDTH - STRUT_HEIGHT);
        glNormal3f(0,0,-1);
        glVertex3f(STRUT_WIDTH / 2, STRUT_LENGTH / 2, LEGO_HEIGHT / 2 - WALL_WIDTH - STRUT_HEIGHT);
        
        //back face
        glNormal3f(1,0,0);
        glVertex3f(STRUT_WIDTH / 2, STRUT_LENGTH / 2, LEGO_HEIGHT / 2 - WALL_WIDTH - STRUT_HEIGHT);
        glNormal3f(1,0,0);
        glVertex3f(STRUT_WIDTH / 2, -STRUT_LENGTH / 2, LEGO_HEIGHT / 2 - WALL_WIDTH - STRUT_HEIGHT);
        glNormal3f(1,0,0);
        glVertex3f(STRUT_WIDTH / 2, -STRUT_LENGTH / 2, LEGO_HEIGHT / 2 - WALL_WIDTH);
        glNormal3f(1,0,0);
        glVertex3f(STRUT_WIDTH / 2, STRUT_LENGTH / 2, LEGO_HEIGHT / 2 - WALL_WIDTH);
    } glEnd();
}

//draws peg inside lego
static void inner_peg()
{
    //outer faces
    int i;
    glBegin(GL_QUAD_STRIP); {
        for (i = 0; i < NUM_PEG_VERTICES * 2; i += 2) {
            glNormal3fv(subtractv(&inner_peg_face_vertices[i][0], &inner_peg_base_center[0], 3));
            glVertex3fv(&inner_peg_face_vertices[i][0]);
            glNormal3fv(subtractv(&inner_peg_face_vertices[i + 1][0], &inner_peg_top_center[0], 3));
            glVertex3fv(&inner_peg_face_vertices[i + 1][0]);
        }
        glNormal3fv(subtractv(&inner_peg_face_vertices[0][0], &inner_peg_base_center[0], 3));
        glVertex3fv(&inner_peg_face_vertices[0][0]);
        glNormal3fv(subtractv(&inner_peg_face_vertices[1][0], &inner_peg_top_center[0], 3));
        glVertex3fv(&inner_peg_face_vertices[1][0]);
    } glEnd();

    //inner faces
    glBegin(GL_QUAD_STRIP); {
        for (i = 0; i < NUM_PEG_VERTICES * 2; i += 2) {
            glNormal3fv(negatev(subtractv(&inner_inner_peg_face_vertices[i][0],
                        &inner_peg_base_center[0], 3), 3));
            glVertex3fv(&inner_inner_peg_face_vertices[i][0]);

            glNormal3fv(negatev(subtractv(&inner_inner_peg_face_vertices[i + 1][0],
                        &inner_peg_top_center[0], 3), 3));
            glVertex3fv(&inner_inner_peg_face_vertices[i + 1][0]);
        }
        glNormal3fv(negatev(subtractv(&inner_inner_peg_face_vertices[0][0],
                    &inner_peg_base_center[0], 3), 3));
        glVertex3fv(&inner_inner_peg_face_vertices[0][0]);

        glNormal3fv(negatev(subtractv(&inner_inner_peg_face_vertices[1][0],
                    &inner_peg_top_center[0], 3), 3));
        glVertex3fv(&inner_inner_peg_face_vertices[1][0]);
    } glEnd();

    //base faces
    glBegin(GL_TRIANGLE_STRIP); {
        for (i = 0; i < NUM_PEG_VERTICES; i++) {
            glNormal3f(0.,0.,-1.);
            glVertex3fv(&inner_inner_peg_base_vertices[i][0]);
            glNormal3f(0.,0.,-1.);
            glVertex3fv(&inner_peg_base_vertices[i][0]);
        }
        glNormal3f(0.,0.,-1.);
        glVertex3fv(&inner_inner_peg_base_vertices[0][0]);
        glNormal3f(0.,0.,-1.);
        glVertex3fv(&inner_peg_base_vertices[0][0]);
    } glEnd();
}

//Draws peg cylinder
static void lego_peg_faces() {
    int i;
    GLfloat *normal;
    glBegin(GL_QUAD_STRIP); {
        for (i = 0; i < NUM_PEG_VERTICES * 2; i += 2) {
            normal = subtractv(&peg_face_vertices[i][0], &peg_base_center[0], 3);
            glNormal3fv(normal);
            glVertex3fv(&peg_face_vertices[i][0]);
            glNormal3fv(normal);
            glVertex3fv(&peg_face_vertices[i + 1][0]);
        }
        glNormal3fv(subtractv(&peg_face_vertices[0][0], &peg_base_center[0], 3));
        glVertex3fv(&peg_face_vertices[0][0]);
        glNormal3fv(subtractv(&peg_face_vertices[1][0], &peg_top_center[0], 3));
        glVertex3fv(&peg_face_vertices[1][0]);
    } glEnd();
    //QUAD_CYCLE(peg_face_vertices, NUM_PEG_VERTICES * 2);
}


//draws the base faces of mini lego
static void mini_lego_base_edges()
{
    int i, j;
    glBegin(GL_QUADS);{
        for (i = 0; i < 4; i++){
            for (j = 0; j < 4; j++){
                glNormal3f(0.,0.,-1.0);
                glVertex3fv(&mini_lego_base_edge_vertices[i][j][0]);
            }
        }
    } glEnd();
}

//draws base face of lego
static void lego_base_edges()
{
    int i, j;
    glBegin(GL_QUADS);{
        for (i = 0; i < 4; i++){
            for (j = 0; j < 4; j++){
                glNormal3f(0.,0.,-1.0);
                glVertex3fv(&lego_base_edge_vertices[i][j][0]);
            }
        }
    } glEnd();
}

//shear matrix for LEGO font
static float font_shear[] = { 
    1,   0, 0, 0, 
    0.8, 1, 0, 0,
    0,   0, 1, 0,
    0,   0, 0, 1 
};

//draws top face of lego peg with LEGO text
static void lego_peg_top() {
    int i;
    char *c;
    glBegin(GL_TRIANGLE_FAN); {
        glNormal3f(0.,0.,1.0);
        glVertex3fv(peg_top_center);
        for (i = 0; i < NUM_PEG_VERTICES; i++) {
            glNormal3f(0.,0.,1.0);
            glVertex3fv(peg_top_vertices[i]);
        }
        glNormal3f(0.,0.,1.0);
        glVertex3fv(peg_top_vertices[0]);
    } glEnd();
    glPushMatrix();
    {
        glRotatef(-90., 0., 0., 1.);
        glTranslatef(peg_top_center[0] - PEG_RADIUS * 0.8,
                peg_top_center[1] - PEG_RADIUS * 0.2, peg_top_center[2] + .05);
        glScalef(0.01, 0.01, 0.01);
        glMultMatrixf(font_shear);
        for (c = (char *) "LEGO"; *c != '\0'; c++) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
            glTranslatef(25, 0, 0);
        }
    }
    glPopMatrix();
}

//draws corners of base
static void mini_lego_base_corners()
{
    int i, j;
    for (i = 0; i < 4; i++) {
        glBegin(GL_TRIANGLE_FAN); {
            for (j = 0; j < 4; j++) {
                glNormal3f(0.,0.,-1.0);
                glVertex3fv(&mini_lego_base_corner_vertices[i][j][0]);
            }
        } glEnd();
    }
}


//draws corners of base
static void lego_base_corners()
{
    int i, j;
    for (i = 0; i < 4; i++) {
        glBegin(GL_TRIANGLE_FAN); {
            for (j = 0; j < 4; j++) {
                glNormal3f(0.,0.,-1.0);
                glVertex3fv(&lego_base_corner_vertices[i][j][0]);
            }
        } glEnd();
    }
}

//draws base face of mini lego
static void mini_lego_base()
{
    mini_lego_base_corners();
    mini_lego_base_edges();
}
//draws base face of lego
static void lego_base()
{
    lego_base_corners();
    lego_base_edges();
}

//draws lego peg with LEGO stroked on top
static void peg()
{
    lego_peg_faces();
    lego_peg_top();
}


static void initLego() {
    //lazy initialization
    if (initialized) return;

    int i;
    GLfloat x, y, *bfv, *bv, *tfv, *tv, *ibv, *itv, *itfv, *ibfv;

    //peg vertices
    for (i = 0; i < NUM_PEG_VERTICES; i++) {
        x = peg_base_center[0] + PEG_RADIUS * cos(2 * M_PI * i / NUM_PEG_VERTICES);
        y = peg_base_center[1] + PEG_RADIUS * sin(2 * M_PI * i / NUM_PEG_VERTICES);

        bfv = peg_face_vertices[2 * i];
        bv = peg_base_vertices[i];
        tfv = peg_face_vertices[2 * i + 1];
        tv = peg_top_vertices[i];

        bfv[0] = bv[0] = tfv[0] = tv[0] = x; 
        bfv[1] = bv[1] = tfv[1] = tv[1] = y; 
        bfv[2] = bv[2] = LEGO_HEIGHT/2;
        tfv[2] = tv[2] = LEGO_HEIGHT/2 + PEG_HEIGHT;
    }

    //inner peg vertices
    for (i = 0; i < NUM_PEG_VERTICES; i++) {
        x = INNER_PEG_RADIUS * cos(2 * M_PI * i / NUM_PEG_VERTICES);
        y = INNER_PEG_RADIUS * sin(2 * M_PI * i / NUM_PEG_VERTICES);

        bfv = inner_peg_face_vertices[2 * i];
        bv = inner_peg_base_vertices[i];
        tfv = inner_peg_face_vertices[2 * i + 1];
        tv = inner_peg_top_vertices[i];

        bfv[0] = bv[0] = tfv[0] = tv[0] = x; 
        bfv[1] = bv[1] = tfv[1] = tv[1] = y; 
        bfv[2] = bv[2] = -LEGO_HEIGHT/2 * 0.95;
        tfv[2] = tv[2] = LEGO_HEIGHT/2 - WALL_WIDTH;

        x = (INNER_PEG_RADIUS - WALL_WIDTH) * cos(2 * M_PI * i / NUM_PEG_VERTICES);
        y = (INNER_PEG_RADIUS - WALL_WIDTH) * sin(2 * M_PI * i / NUM_PEG_VERTICES);

        ibfv = inner_inner_peg_face_vertices[2 * i];
        ibv = inner_inner_peg_base_vertices[i];
        itfv = inner_inner_peg_face_vertices[2 * i + 1];
        itv = inner_inner_peg_top_vertices[i];

        ibfv[0] = ibv[0] = itfv[0] = itv[0] = x; 
        ibfv[1] = ibv[1] = itfv[1] = itv[1] = y; 
        ibfv[2] = ibv[2] = -LEGO_HEIGHT/2 * 0.95;
        itfv[2] = itv[2] = LEGO_HEIGHT/2 - WALL_WIDTH;
    }
    initialized = true;
}

//draws lego
void lego()
{
    initLego();
    lego_sides();
    lego_base();
    lego_inner_faces();
    lego_top();
    lego_top_inner();
    glPushMatrix();
        glTranslatef(-PEG_CENTER_TO_CENTER_DISTANCE / 2, 0, 0);
        inner_peg();
        glPushMatrix();
            glTranslatef(0, INNER_PEG_RADIUS + STRUT_LENGTH / 2, 0);
            strut();
            glTranslatef(0, -(INNER_PEG_RADIUS * 2 + STRUT_LENGTH), 0);
            strut();
        glPopMatrix();
        glTranslatef(PEG_CENTER_TO_CENTER_DISTANCE, 0, 0);
        inner_peg();
        glPushMatrix();
            glTranslatef(0, INNER_PEG_RADIUS + STRUT_LENGTH / 2, 0);
            strut();
            glTranslatef(0, -(INNER_PEG_RADIUS * 2 + STRUT_LENGTH), 0);
            strut();
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-PEG_CENTER_TO_CENTER_DISTANCE, -PEG_CENTER_TO_CENTER_DISTANCE / 2, 0);
        peg(); 
        glPushMatrix();
            glTranslatef(PEG_CENTER_TO_CENTER_DISTANCE, 0, 0);
            peg();
            glTranslatef(PEG_CENTER_TO_CENTER_DISTANCE, 0, 0);
            peg();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0, PEG_CENTER_TO_CENTER_DISTANCE, 0);
            peg();
            glTranslatef(PEG_CENTER_TO_CENTER_DISTANCE, 0, 0);
            peg();
            glTranslatef(PEG_CENTER_TO_CENTER_DISTANCE, 0, 0);
            peg();
        glPopMatrix();
    glPopMatrix();
}

//draws mini lego
void mini_lego()
{
    initLego();
    mini_lego_sides();
    mini_lego_base();
    mini_lego_inner_faces();
    mini_lego_top();
    mini_lego_top_inner();
    peg();
}
