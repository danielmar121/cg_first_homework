#include "glut.h"
#include <math.h>
#include <time.h>

typedef struct
{
	double x, y;
} STAR;

const int NUM_STARS = 100;
const double PI = 3.14;

STAR stars[NUM_STARS];
bool light[NUM_STARS][NUM_STARS]; // windows

double offset = 0;

void init()
{
	int i, j;
	srand(time(0));

	for (i = 0; i < NUM_STARS; i++)
	{
		stars[i].x = -1 + (rand() % 1000) / 500.0; // random value in range (-1,1);
		stars[i].y = -1 + (rand() % 1000) / 500.0; // random value in range (-1,1);
	}
	// prepare windows
	for (i = 0; i < NUM_STARS; i++)
		for (j = 0; j < NUM_STARS; j++)
			light[i][j] = (rand() % 20) <= 14;

	//             red  green  blue
	glClearColor(0.0, 0.0, 0.3, 0);// color of window background
	// setup orthographic projection
	glOrtho(-1, 1, -1, 1, 1, -1); // logical coordinates
}

void DrawMainAxes()
{
	glColor3d(1, 1, 1);

	// X
	glBegin(GL_LINES);
	glVertex2d(-1, 0);
	glVertex2d(1, 0);
	glEnd();
	// arrow
	glBegin(GL_POLYGON);
	glVertex2d(1, 0);
	glVertex2d(0.85, 0.05);
	glVertex2d(0.85, -0.05);
	glEnd();

	// Y
	glBegin(GL_LINES);
	glVertex2d(0, -1);
	glVertex2d(0, 1);
	glEnd();
	// arrow
	glBegin(GL_POLYGON);
	glVertex2d(0, 1);
	glVertex2d(0.05, 0.85);
	glVertex2d(-0.05, 0.85);
	glEnd();
}

void 	DrawStarrySky()
{
	int i;

	glBegin(GL_POLYGON);
	glColor3d(0, 0, 0.2); // dark
	glVertex2d(-1, 1);  // left- top
	glVertex2d(1, 1);  // right-top
	glColor3d(0, 0.2, 0.4);
	glVertex2d(1, -0.5);  // right-bottom
	glColor3d(0, 0.3, 0.6);// light blue
	glVertex2d(0, -0.5); // right-bottom
	glColor3d(0.1, 0.3, 0.4);
	glVertex2d(-1, -0.5);  // left-bottom
//	glVertex2d(-0, -0.5); // left-bottom
	glEnd();

	glBegin(GL_POLYGON);
	glColor3d(0, 0, 0.2); // dark
	glVertex2d(-1, -1);  // left- top
	glVertex2d(1, -1);  // right-top
	glColor3d(0, 0.2, 0.4);
	glVertex2d(1, -0.5);  // right-bottom
	glVertex2d(-1, -0.5);  // left-bottom
//	glVertex2d(-0, -0.5); // left-bottom
	glEnd();

	//	glPointSize(2);
	glColor3d(1, 1, 0); // yellow
	glBegin(GL_POINTS);

	for (i = 0; i < NUM_STARS; i++)
		glVertex2d(stars[i].x, stars[i].y);

	glEnd();
}

void DrawWheel(double cx, double cy, double radius, int n)
{
	double alpha, teta = 2 * PI / n;
	double x, y;

	glColor3d(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	for (alpha = 0; alpha <= 2 * PI; alpha += teta)
	{
		x = cx + radius * cos(alpha + offset);
		y = cy + radius * sin(alpha + offset);
		glVertex2d(x, y);
	}
	glEnd();

	glColor3d(0.5, 0.5, 0.5);
	glBegin(GL_LINES);
	for (alpha = 0; alpha <= 2 * PI; alpha += 2 * teta)
	{
		x = cx + radius * cos(alpha + offset);
		y = cy + radius * sin(alpha + offset);
		glVertex2d(x, y);
		glVertex2d(cx, cy);
	}
	glEnd();

	for (alpha = 0; alpha <= 2 * PI; alpha += teta)
	{
		x = cx + radius * cos(alpha + offset);
		y = cy + radius * sin(alpha + offset);
		glColor3d(1, 1, 0); // yellow
		glBegin(GL_POLYGON);
		glVertex2d(x + 0.02, y + 0.02);
		glVertex2d(x - 0.02, y + 0.02);
		glVertex2d(x - 0.02, y - 0.02);
		glVertex2d(x + 0.02, y - 0.02);
		glEnd();
	}

	glLineWidth(2);
	glColor3d(1, 1, 1);

	glBegin(GL_LINES);
	glVertex2d(cx, cy);
	glVertex2d(cx - 0.2, -0.5);
	glVertex2d(cx, cy);
	glVertex2d(cx + 0.2, -0.5);
	glEnd();
	glLineWidth(1);
}

void DrawBuilding(double cx, double top, double bottom, double w, double r, double g, double b)
{
	//	double w = 0.035;
	double row, col;
	int i, j;

	glBegin(GL_POLYGON);
	glColor3d(r, g, b);
	glVertex2d(cx - w, top);
	glVertex2d(cx + w, top);
	glColor3d(2 * r, 2 * g, 2 * b);
	glVertex2d(cx + w, bottom);
	glVertex2d(cx - w, bottom);
	glEnd();

	// windows
	glPointSize(2);
	glColor3d(0.7, 0.7, 0);
	glBegin(GL_POINTS);
	for (row = bottom + 0.01, i = 0; row < top; row += 0.02, i++)
		for (col = cx - w + 0.01, j = 0; col < cx + w - 0.01; col += 0.015, j++)
		{
			if (light[i][j])
				glColor3d(0.7, 0.7, 0);
			else
				glColor3d(0, 0, 0);

			glVertex2d(col, row);
		}
	glEnd();
}

// redraw function
void display()
{
	double x, y, step = 0.1;
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawStarrySky();

	//	DrawMainAxes();
		// draw graph of function
	glPointSize(2);
	glLineWidth(2);
	//	glBegin(GL_LINE_STRIP);
	for (x = -1; x <= 1; x += step)
	{
		y = 0.2 * cos(3 * x) - 0.1 + 0.2 * sin(50 * x);
		//		glColor3d(0.4*(1-fabs(y)), 0.4*(fabs(x)), fabs(y));
		//		glVertex2d(x, y);
				// send the center x, height and bottom and red, green, blue color component
		DrawBuilding(x, y, -0.5, 0.04, 0.3 * (1 - fabs(y)), 0.4 * (fabs(x)), fabs(y));
	}
	//	glEnd();

	//	glBegin(GL_LINE_STRIP);
	step = 0.2;
	for (x = -1; x <= 1; x += step)
	{
		y = 0.1 * cos(2 * x) * cos(35 * x) - 0.35;
		glColor3d(0.4 * (1 - fabs(y)), 0.4 * (fabs(x)), fabs(y));
		glVertex2d(x, y);
		// send the center x, height and bottom and red, green, blue color component
		DrawBuilding(x, y, -0.5, 0.06, 0.3, 0.4 * (1 - fabs(x)), fabs(y));
	}
	//	glEnd();

	DrawWheel(-0.5, 0, 0.45, 30);

	glutSwapBuffers(); // show all
}

// runs when nothing happens
void idle()
{
	int i, j;

	// update moving stars
	/*
	for (i = 0; i < NUM_STARS; i++)
	{
		stars[i].x *= 1.003;
		stars[i].y *= 1.003;

		if (fabs(stars[i].x) > 1 || fabs(stars[i].y) > 1) // pick new random position
		{
			stars[i].x = -0.5 + (rand() % 1000) / 1000.0; // random value in range (-1,1);
			stars[i].y = -0.5 + (rand() % 1000) / 1000.0; // random value in range (-1,1);
		}
	}
	*/

	// switch light in randomly choosen window
	i = rand() % NUM_STARS;
	j = rand() % NUM_STARS;
	light[i][j] = !light[i][j];

	offset -= 0.001;
	glutPostRedisplay(); // indirect call to display
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv); //
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // defines matrices: 1. Color matrix, 2. video buffer
	glutInitWindowSize(600, 600); // physical size of window (in pixels)
	glutInitWindowPosition(200, 100);
	glutCreateWindow("First Example");

	glutDisplayFunc(display); //  display is a refresh window function
	glutIdleFunc(idle); // kind of timer function

	init();

	glutMainLoop();
}