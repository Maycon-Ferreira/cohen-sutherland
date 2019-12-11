#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>

#define INSIDE 0U
#define LEFT 1U
#define RIGHT 2U
#define BOTTOM 4U
#define TOP 8U

typedef struct {
    double x1;
    double y1;
    double x2;
    double y2;
} line_t;

double x_min = 0;
double x_max = 0;
double y_min = 0;
double y_max = 0;

double x1 = 0;
double y1 = 0;
double x2 = 0;
double y2 = 0;

int num_lines = 0;
line_t *lines = NULL;

unsigned char area_code(double x, double y) {
    unsigned char code = INSIDE;

    if (x < x_min) { code |= LEFT; }
    if (x > x_max) { code |= RIGHT; }
    if (y < y_min) { code |= BOTTOM; }
    if (y > y_max) { code |= TOP; }

    return code;
}

int cohen_sutherland(line_t *line) {
    unsigned int area_p1 = area_code(line->x1, line->y1);
    unsigned int area_p2 = area_code(line->x2, line->y2);

    if (area_p1 & area_p2) {
        return -1;
    } else {
        if (area_p1 == INSIDE) {
            x1 = line->x1;
            y1 = line->y1;
        }
        if (area_p2 == INSIDE) {
            x2 = line->x2;
            y2 = line->y2;
        }
        if (area_p1 & LEFT) {
            y1 = line->y1 + (line->y2 - line->y1) * (x_min - line->x1) / (line->x2 - line->x1);
            x1 = x_min;
        }
        if (area_p1 & RIGHT) {
            y1 = line->y1 + (line->y2 - line->y1) * (x_max - line->x1) / (line->x2 - line->x1);
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
            y2 = line->y1 + (line->y2 - line->y1) * (x_min - line->x1) / (line->x2 - line->x1);
            x2 = x_min;
        }
        if (area_p2 & RIGHT) {
            y2 = line->y1 + (line->y2 - line->y1) * (x_max - line->x1) / (line->x2 - line->x1);
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

    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_LINE_STRIP);
    {
        glVertex3d(x_min, y_min, 10.0);
        glVertex3d(x_min, y_max, 10.0);
        glVertex3d(x_max, y_max, 10.0);
        glVertex3d(x_max, y_min, 10.0);
        glVertex3d(x_min, y_min, 10.0);
    }
    glEnd();

    glColor3f(1.0, 0.0, 0.0);

    for (int i = 0; i < num_lines; ++i) {
        glBegin(GL_LINES);
        {
            glVertex3d(lines[i].x1, lines[i].y1, 10.0);
            glVertex3d(lines[i].x2, lines[i].y2, 10.0);
        }
        glEnd();
    }

    glColor3f(0.0, 1.0, 0.0);

    for (int i = 0; i < num_lines; ++i) {
        if (cohen_sutherland(lines + i) == 0) {
            glBegin(GL_LINES);
            {
                glVertex3d(x1, y1, 9.9);
                glVertex3d(x2, y2, 9.9);
            }
            glEnd();
        }
    }

    glutSwapBuffers();
    glFlush();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        puts("Usage: ./cohen-sutherland FILE");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        perror("Could not open file");
        return 2;
    }

    fscanf(file, "%lf\n"
                 "%lf\n"
                 "%lf\n"
                 "%lf\n"
                 "%d\n", &x_min, &x_max, &y_min, &y_max, &num_lines);

    lines = calloc(num_lines, sizeof(line_t));

    for (int i = 0; i < num_lines; ++i) {
        fscanf(file, "%lf %lf\n"
                     "%lf %lf\n", &lines[i].x1, &lines[i].y1, &lines[i].x2, &lines[i].y2);
    }

    fclose(file);

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

#pragma clang diagnostic pop
