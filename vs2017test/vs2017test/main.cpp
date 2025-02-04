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
double heights[NUM_STARS];

double offset = 0;
double airplaneOffset = 0;

void init()
{
	int i, j;
	srand(time(0));

	for (i = 0; i < NUM_STARS; i++)
	{
		stars[i].x = -1 + (rand() % 1000) / 500.0; // random value in range (-1,1);
		stars[i].y = -1 + (rand() % 1000) / 500.0; // random value in range (-1,1);
		heights[i] = (0.2) * ((double)rand() / (double)RAND_MAX); //random values to add the buildings heights between 0 to 0.2
	}
	// prepare windows
	for (i = 0; i < NUM_STARS; i++)
		for (j = 0; j < NUM_STARS; j++)
			light[i][j] = (rand() % 20) <= 14;

	//			red  green  blue
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
		y = (cy + radius * sin(alpha + offset));
		glColor3d(0.7, 0.5, 0.4); // yellow booth
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
	double reflectionButtomY = leftButtomY + 0.25;

	for (cube = 0; cube < 4; cube++)
	{
		leftButtomY += 0.25;
		glBegin(GL_POLYGON);
		glColor3d(0.2, 0.4, 0.5);
		glVertex2d(leftBottomX + pipeSize, leftButtomY); // left bottom
		glVertex2d(leftBottomX + pipeSize + windowWidth, leftButtomY); // right bottom
		glVertex2d(leftBottomX + pipeSize + windowWidth, leftButtomY + cubeSize); // right top
		glVertex2d(leftBottomX + pipeSize, leftButtomY + cubeSize); // left top
		glEnd();

		//reflection
		glBegin(GL_POLYGON);
		glColor3d(0.1, 0.3, 0.4);
		glVertex2d(leftBottomX + pipeSize, reflectionButtomY); // left top
		glVertex2d(leftBottomX + pipeSize + windowWidth, reflectionButtomY); // right top
		glVertex2d(leftBottomX + pipeSize + windowWidth, reflectionButtomY - cubeSize); // right bottom
		glVertex2d(leftBottomX + pipeSize, reflectionButtomY - cubeSize); // left bottom
		glEnd();

		glLineWidth(1);
		for (window = 0; window <= windowWidth; window += windowWidth / 5)
		{
			glBegin(GL_LINES);
			glColor3d(0, 0, 0);
			glVertex2d(leftBottomX + pipeSize, leftButtomY + pipeSize + window); // left bottom
			glVertex2d(leftBottomX + cubeSize - pipeSize, leftButtomY + pipeSize + window); // right bottom
			glVertex2d(leftBottomX + pipeSize, reflectionButtomY - pipeSize - window); // left bottom
			glVertex2d(leftBottomX + cubeSize - pipeSize, reflectionButtomY - pipeSize - window); // right bottom
			glEnd();
		}

		for (window = 0; window < windowWidth; window += windowWidth / 10)
		{
			glBegin(GL_LINES);
			glColor3d(0, 0, 0);
			glVertex2d(leftBottomX + pipeSize + window, leftButtomY + pipeSize); // left bottom
			glVertex2d(leftBottomX + pipeSize + window, leftButtomY + cubeSize - pipeSize); // right bottom
			glVertex2d(leftBottomX + pipeSize + window, reflectionButtomY - pipeSize); // left bottom
			glVertex2d(leftBottomX + pipeSize + window, reflectionButtomY - cubeSize + pipeSize); // right bottom
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
		//reflection
		glLineWidth(3);
		glBegin(GL_LINE_LOOP);
		glColor3d(0.9, 0.9, 0.9);
		glVertex2d(leftBottomX, reflectionButtomY); // left bottom
		glVertex2d(leftBottomX + cubeSize, reflectionButtomY); // right bottom
		glVertex2d(leftBottomX + cubeSize, reflectionButtomY - cubeSize); // right top
		glVertex2d(leftBottomX, reflectionButtomY - cubeSize); // left top
		glEnd();

		for (pipe = windowWidth / 4; pipe < windowWidth; pipe += windowWidth / 4)
		{
			// bottom pipes
			glBegin(GL_LINES);
			glVertex2d(leftBottomX + pipe + pipeSize, leftButtomY);
			glVertex2d(leftBottomX + pipe + pipeSize, leftButtomY + pipeSize);
			glVertex2d(leftBottomX + pipe + pipeSize, reflectionButtomY);
			glVertex2d(leftBottomX + pipe + pipeSize, reflectionButtomY - pipeSize);
			glEnd();

			// top pipes
			glBegin(GL_LINES);
			glVertex2d(leftBottomX + pipe + pipeSize, leftButtomY + cubeSize);
			glVertex2d(leftBottomX + pipe + pipeSize, leftButtomY - pipeSize + cubeSize);
			glVertex2d(leftBottomX + pipe + pipeSize, reflectionButtomY - cubeSize);
			glVertex2d(leftBottomX + pipe + pipeSize, reflectionButtomY + pipeSize - cubeSize);
			glEnd();
		}

		for (pipe = windowWidth / 5; pipe < windowWidth; pipe += windowWidth / 5)
		{
			// left pipes
			glBegin(GL_LINES);
			glVertex2d(leftBottomX, leftButtomY + pipe + pipeSize);
			glVertex2d(leftBottomX + pipeSize, leftButtomY + pipe + pipeSize);
			glVertex2d(leftBottomX, reflectionButtomY - pipe - pipeSize);
			glVertex2d(leftBottomX + pipeSize, reflectionButtomY - pipe - pipeSize);
			glEnd();

			// right pipes
			glBegin(GL_LINES);
			glVertex2d(leftBottomX + cubeSize, leftButtomY + pipe + pipeSize);
			glVertex2d(leftBottomX - pipeSize + cubeSize, leftButtomY + pipe + pipeSize);
			glVertex2d(leftBottomX + cubeSize, reflectionButtomY - pipe - pipeSize);
			glVertex2d(leftBottomX - pipeSize + cubeSize, reflectionButtomY - pipe - pipeSize);
			glEnd();
		}

		reflectionButtomY -= 0.25;
	}
}

void Airplane(double cx, double cy, int n) {
	double alpha, teta = 2 * PI / n;
	double x, y, i;
	double radius = 0.125;

	glColor3d(1, 1, 1);
	glBegin(GL_POLYGON);
	for (alpha = PI / 2; alpha <= (3 * PI) / 2; alpha += teta)
	{
		x = cx + radius * cos(alpha) + airplaneOffset;
		y = cy + radius * sin(alpha);
		glVertex2d(x, y);
	}
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2d(cx + airplaneOffset, cy - 0.125); // left bottom
	glVertex2d(cx + 1 + airplaneOffset, cy - 0.125); // right bottom
	glVertex2d(cx + 1 + airplaneOffset, cy + 0.125); // right top
	glVertex2d(cx + airplaneOffset, cy + 0.125); // left top
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2d(cx + 1 + airplaneOffset, cy - 0.125);
	glVertex2d(cx + 1.25 + airplaneOffset, cy + 0.375);
	glVertex2d(cx + 1 + airplaneOffset, cy + 0.125);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2d(cx + 0.5 + airplaneOffset, cy - 0.125);
	glVertex2d(cx + 1 + airplaneOffset, cy - 0.375);
	glVertex2d(cx + 0.75 + airplaneOffset, cy - 0.125);
	glEnd();

	glColor3d(0, 0.7, 1);
	for (i = 0; i < 1; i += 0.05)
	{
		glBegin(GL_POLYGON);
		glVertex2d(cx + i + airplaneOffset, cy - 0.02); // left bottom
		glVertex2d(cx + i + 0.04 + airplaneOffset, cy - 0.02); // right bottom
		glVertex2d(cx + i + 0.04 + airplaneOffset, cy + 0.06); // right top
		glVertex2d(cx + i + airplaneOffset, cy + 0.06); // left top
		glEnd();
	}
}

// redraw function
void display()
{
	double x = 0, y = 0, step = 0.1;
	int i;
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawStarrySky();
	Airplane(1, 0.5, 20);
	DrawWheel(-0.5, 0, 0.45, 30);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
	DrawWheel(-0.5, -0.95, 0.45, 30);
	glDisable(GL_BLEND);

	ElectraTower(0.5, -0.75);

	// high buildings
	for (x = -1, i = 0; x <= 1; x += step, i++)
	{
		y = 0.2 * cos(3 * x) - 0.1 + 0.2 * sin(50 * x) + heights[i];
		// send the center x, height and bottom and red, green, blue color component
		DrawBuilding(x, y, -0.5, 0.04, 0.3 * (1 - fabs(y)), 0.4 * (fabs(x)), fabs(y));

		//reflection
		DrawBuilding(x, -0.5, -0.5 + (-0.5 - y), 0.04, 0.3 * (1 - fabs(y)) - 0.1, 0.4 * (fabs(x)) - 0.1, fabs(y) - 0.1);
	}

	// law buildings
	step = 0.2;
	for (x = -1, i = 0; x <= 1; x += step, i++)
	{
		y = 0.1 * cos(2 * x) * cos(35 * x) - 0.35 + heights[i];
		glColor3d(0.4 * (1 - fabs(y)), 0.4 * (fabs(x)), fabs(y));
		glVertex2d(x, y);

		// send the center x, height and bottom and red, green, blue color component
		DrawBuilding(x, y, -0.5, 0.06, 0.3, 0.4 * (1 - fabs(x)), fabs(y));

		//reflection
		DrawBuilding(x, -0.5, -0.5 + (-0.5 - y), 0.06, 0.3 - 0.05, 0.4 * (1 - fabs(x)) - 0.05, fabs(y) - 0.05);
	}

	glutSwapBuffers(); // show all
}

// runs when nothing happens
void idle()
{
	int i, j;

	// update moving stars
	int starNum = (rand() % 100);
	stars[starNum].x = -1 + (rand() % 1000) / 500.0; // random value in range (-1,1);
	stars[starNum].y = -1 + (rand() % 1000) / 500.0; // random value in range (-1,1);

	// switch light in randomly chosen window
	i = rand() % NUM_STARS;
	j = rand() % NUM_STARS;
	light[i][j] = !light[i][j];

	offset -= 0.001;

	airplaneOffset -= 0.001;
	if (airplaneOffset < -5) {
		airplaneOffset = 0;
	}

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