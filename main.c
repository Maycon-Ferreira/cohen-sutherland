#include <GL/gl.h>
#include <GL/glut.h>

#define INSIDE 0U
#define LEFT 1U
#define RIGHT 2U
#define BOTTOM 4U
#define TOP 8U

double x_min = 0;
double x_max = 0;
double y_min = 0;
double y_max = 0;

double x1 = 0;
double y1 = 0;
double x2 = 0;
double y2 = 0;

typedef struct {
    double x1;
    double y1;
    double x2;
    double y2;
} line_t;

unsigned char area_code(double x, double y) {
    unsigned char code = INSIDE;

    if (x < x_min) { code |= LEFT; }
    if (x > x_max) { code |= RIGHT; }
    if (y < y_min) { code |= BOTTOM; }
    if (y < y_max) { code |= TOP; }

    return code;
}

int cohen_sutherland(line_t *line) {
    unsigned int area_p1 = area_code(line->x1, line->x2);
    unsigned int area_p2 = area_code(line->x2, line->x2);

    if (area_p1 & area_p2) {
        return -1;
    } else if (area_p1 == INSIDE && area_p2 == INSIDE) {
        return 0;
    } else {
        if (area_p1 & LEFT) {
            y1 = line->x1 + (line->y2 - line->y1) * (x_min - line->x1) / (line->x2 - line->x1);
            x1 = x_min;
        }
        if (area_p1 & RIGHT) {
            y1 = line->x1 + (line->y2 - line->y1) * (x_max - line->x1) / (line->x2 - line->x1);
            x1 = x_max;
        }
        if (area_p1 & BOTTOM) {
            x1 = line->x1 + (line->x2 - line->x1) * (y_min - line->y1) / (line->y2 - line->y1);
            y1 = y_min;
        }
        if (area_p1 & TOP) {
            x1 = line->x1 + (line->x2 - line->x1) * (y_max - line->y1) / (line->y2 - line->y1);
            y1 = y_max;
        }
        if (area_p2 & LEFT) {
            y2 = line->x1 + (line->y2 - line->y1) * (x_min - line->x1) / (line->x2 - line->x1);
            x2 = x_min;
        }
        if (area_p2 & RIGHT) {
            y2 = line->x1 + (line->y2 - line->y1) * (x_max - line->x1) / (line->x2 - line->x1);
            x2 = x_max;
        }
        if (area_p2 & BOTTOM) {
            x2 = line->x1 + (line->x2 - line->x1) * (y_min - line->y1) / (line->y2 - line->y1);
            y2 = y_min;
        }
        if (area_p2 & TOP) {
            x2 = line->x1 + (line->x2 - line->x1) * (y_max - line->y1) / (line->y2 - line->y1);
            y2 = y_max;
        }
        return 0;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // TODO implement here

    glFlush();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cohen-Sutherland");
    init();
    glutDisplayFunc(display);
    glFrontFace(GL_CCW);
    glutMainLoop();
    return 0;
}
