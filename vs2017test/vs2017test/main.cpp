#include "glut.h"
#include <math.h>
#include <time.h>
#include <iostream>
#include <string>
using namespace std;

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

void 	DrawStarrySky()
{
	int i;

	// sky
	glBegin(GL_POLYGON);
	// fourth 1/4 in left purple
	glColor3d(0.6, 0, 0.4); // dark
	glVertex2d(-1, 1);  // right-top
	// third 1/4 left in red
	glColor3d(0.9, 0.1, 0);
	glVertex2d(-1, 0.5);
	// second 1/4 left in orange
	glColor3d(0.9, 0.7, 0);
	glVertex2d(-1, 0);
	// first 1/4 left in yellow
	glColor3d(1, 1, 0);
	glVertex2d(-1, -0.5);
	// first 1/4 right in yellow
	glColor3d(1, 1, 0);
	glVertex2d(1, -0.5);
	// second 1/4 right in orange
	glColor3d(0.9, 0.7, 0);
	glVertex2d(1, 0);
	// third 1/4 right in red
	glColor3d(0.9, 0.1, 0);
	glVertex2d(1, 0.5);
	// fourth 1/4 in right purple
	glColor3d(0.6, 0, 0.4); // dark
	glVertex2d(1, 1);  // left- top
	glEnd();

	glBegin(GL_POLYGON);
	glColor3d(0.4, 0.7, 1); // light blue
	glVertex2d(1, -0.5);  // right-top
	glVertex2d(-1, -0.5);  // left- top
	glColor3d(0.4, 0.1, 0.8); // dark blue
	glVertex2d(-1, -1);  // left-bottom
	glVertex2d(1, -1);  // right-bottom
	glEnd();

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
		glColor3d(0.7, 0.5, 0.4); // yellow both
		glBegin(GL_POLYGON);
		glVertex2d(x + 0.02, y + 0.02);
		glVertex2d(x - 0.02, y + 0.02);
		glVertex2d(x - 0.02, y - 0.02);
		glVertex2d(x + 0.02, y - 0.02);
		glEnd();

		glColor3d(0, 0.7, 1); // blue window
		glBegin(GL_POLYGON);
		glVertex2d(x - 0.005, y + 0.01);
		glVertex2d(x - 0.015, y + 0.01);
		glVertex2d(x - 0.015, y - 0.01);
		glVertex2d(x - 0.005, y - 0.01);
		glEnd();

		glColor3d(0, 0.7, 1); // blue window
		glBegin(GL_POLYGON);
		glVertex2d(x + 0.015, y + 0.01);
		glVertex2d(x + 0.005, y + 0.01);
		glVertex2d(x + 0.005, y - 0.01);
		glVertex2d(x + 0.015, y - 0.01);
		glEnd();

		glColor3d(0.5, 0.1, 0.2); // red roof
		glBegin(GL_POLYGON);
		glVertex2d(x + 0.02, y + 0.02);
		glVertex2d(x, y + 0.04);
		glVertex2d(x - 0.02, y + 0.02);
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

void ElectraTower(double leftBottomX, double leftButtomY) {
	double windowWidth = 0.1875, cubeSize = 0.25, pipeSize = 0.03125;
	int cube;
	double pipe, window;

	for (cube = 0; cube < 3; cube++)
	{
		leftButtomY += 0.25;
		glBegin(GL_POLYGON);
		glColor3d(0.2, 0.4, 0.5);
		glVertex2d(leftBottomX + pipeSize, leftButtomY); // left bottom
		glVertex2d(leftBottomX + pipeSize + windowWidth, leftButtomY); // right bottom
		glVertex2d(leftBottomX + pipeSize + windowWidth, leftButtomY + cubeSize); // right top
		glVertex2d(leftBottomX + pipeSize, leftButtomY + cubeSize); // left top
		glEnd();

		glLineWidth(1);
		for (window = 0; window <= windowWidth; window += windowWidth / 5)
		{
			glBegin(GL_LINES);
			glColor3d(0, 0, 0);
			glVertex2d(leftBottomX + pipeSize, leftButtomY + pipeSize + window); // left bottom
			glVertex2d(leftBottomX + cubeSize - pipeSize, leftButtomY + pipeSize + window); // right bottom
			glEnd();
		}

		for (window = 0; window < windowWidth; window += windowWidth / 10)
		{
			glBegin(GL_LINES);
			glColor3d(0, 0, 0);
			glVertex2d(leftBottomX + pipeSize + window, leftButtomY + pipeSize); // left bottom
			glVertex2d(leftBottomX + pipeSize + window, leftButtomY + cubeSize - pipeSize); // right bottom
			glEnd();
		}

		glLineWidth(3);
		glBegin(GL_LINE_LOOP);
		glColor3d(1, 1, 1);
		glVertex2d(leftBottomX, leftButtomY); // left bottom
		glVertex2d(leftBottomX + cubeSize, leftButtomY); // right bottom
		glVertex2d(leftBottomX + cubeSize, leftButtomY + cubeSize); // right top
		glVertex2d(leftBottomX, leftButtomY + cubeSize); // left top
		glEnd();

		for (pipe = windowWidth / 4; pipe < windowWidth; pipe += windowWidth / 4)
		{
			// bottom pipes
			glBegin(GL_LINES);
			glVertex2d(leftBottomX + pipe + pipeSize, leftButtomY);
			glVertex2d(leftBottomX + pipe + pipeSize, leftButtomY + pipeSize);
			glEnd();

			// top pipes
			glBegin(GL_LINES);
			glVertex2d(leftBottomX + pipe + pipeSize, leftButtomY + cubeSize);
			glVertex2d(leftBottomX + pipe + pipeSize, leftButtomY - pipeSize + cubeSize);
			glEnd();
		}

		for (pipe = windowWidth / 5; pipe < windowWidth; pipe += windowWidth / 5)
		{
			// left pipes
			glBegin(GL_LINES);
			glVertex2d(leftBottomX, leftButtomY + pipe + pipeSize);
			glVertex2d(leftBottomX + pipeSize, leftButtomY + pipe + pipeSize);
			glEnd();

			// right pipes
			glBegin(GL_LINES);
			glVertex2d(leftBottomX + cubeSize, leftButtomY + pipe + pipeSize);
			glVertex2d(leftBottomX - pipeSize + cubeSize, leftButtomY + pipe + pipeSize);
			glEnd();
		}
	}
}

// redraw function
void display()
{
	double x = 0, y = 0, step = 0.1;
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawStarrySky();
	DrawWheel(-0.5, 0, 0.45, 30);
	ElectraTower(0.5,-0.75);

	
	for (x = -1; x <= 1; x += step)
	{
		y = 0.2 * cos(3 * x) - 0.1 + 0.2 * sin(50 * x);
		// send the center x, height and bottom and red, green, blue color component
		DrawBuilding(x, y, -0.5, 0.04, 0.3 * (1 - fabs(y)), 0.4 * (fabs(x)), fabs(y));
	}

	step = 0.2;
	for (x = -1; x <= 1; x += step)
	{
		y = 0.1 * cos(2 * x) * cos(35 * x) - 0.35;
		glColor3d(0.4 * (1 - fabs(y)), 0.4 * (fabs(x)), fabs(y));
		glVertex2d(x, y);
		// send the center x, height and bottom and red, green, blue color component
		DrawBuilding(x, y, -0.5, 0.06, 0.3, 0.4 * (1 - fabs(x)), fabs(y));
	}
	

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