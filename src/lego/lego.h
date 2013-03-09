#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#define LEGO_HEIGHT (LEGO_WIDTH * 0.6)
#define LEGO_WIDTH 18.0
#define LEGO_UNIT (LEGO_WIDTH / 2.0)
#define WALL_WIDTH (LEGO_WIDTH / 18.0)
#define LEGO_LENGTH (LEGO_WIDTH * 1.5)
#define PEG_HEIGHT (LEGO_HEIGHT / 6.0)
#define PEG_RADIUS (LEGO_WIDTH / 6.0)
#define INNER_PEG_RADIUS (LEGO_WIDTH * 0.2)
#define PEG_EDGE_TO_EDGE_DISTANCE \
    ((LEGO_LENGTH - (6.0 * PEG_RADIUS)) / 3.0)
#define PEG_CENTER_TO_CENTER_DISTANCE \
    (2.0 * PEG_RADIUS + PEG_EDGE_TO_EDGE_DISTANCE)
#define STRUT_WIDTH WALL_WIDTH
#define STRUT_LENGTH  \
    ((LEGO_WIDTH - 2.0 * WALL_WIDTH - 2.0 * INNER_PEG_RADIUS) / 2.0)
#define NUM_PEG_VERTICES 100
#define STRUT_HEIGHT (LEGO_HEIGHT * 0.75)

//LEGO COLORS
extern const GLfloat green[4]; 
extern const GLfloat yellow[4];
extern const GLfloat white[4]; 
extern const GLfloat cyan[4];
extern const GLfloat red[4];
extern const GLfloat blue[4];
extern const GLfloat black[4];

//DRAW 3X2 LEGO
void lego();

//DRAW 1X1 LEGO
void mini_lego();

