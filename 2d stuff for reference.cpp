#include <windows.h>   // use as needed for your system
#include <gl/Gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265

const int height = 500;
const int width = 650;

// create a struct to hold the x and y values for points
struct point {
	double x;
	double y;
};

//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
 void myInit(void)
 {
    glClearColor(1.0, 1.0, 1.0, 0.0);      // set the bg color to a bright white
    glColor3f(0.0f, 0.0f, 0.0f);           // set the drawing color to black 
 	glPointSize(1.0);		            //set the point size to 4 by 4 pixels
	glMatrixMode(GL_PROJECTION);// set up appropriate matrices- to be explained 
	glLoadIdentity();// to be explained
	gluOrtho2D(0.0, float(width), 0.0, float(height));// to be explained
}

void matrixMath(double m1[3][3], double m2[3], double res[3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			res[i] += m1[i][j] * m2[j];
		}
	}
}

void myTranslate(point l[], double a, double b, int n) {
	double myMatrix[3][3] = {{1,0,a}, {0,1,b}, {0,0,1}};
	for (int i = 0; i < n; i++) {
		double pointMatrix[3] = {l[i].x,l[i].y,1};
		double resMatrix[3] = {0};
		matrixMath(myMatrix, pointMatrix, resMatrix);
		l[i].x = resMatrix[0];
		l[i].y = resMatrix[1];
	}
}

void myScale(point l[], double a, double b, int n) {
	double myMatrix[3][3] = {{a,0,0}, {0,b,0}, {0,0,1}};
	for (int i = 0; i < n; i++) {
		double pointMatrix[3] = {l[i].x,l[i].y,1};
		double resMatrix[3] = {0};
		matrixMath(myMatrix, pointMatrix, resMatrix);
		l[i].x = resMatrix[0];
		l[i].y = resMatrix[1];
	}
}

void myRotate(point l[], double a, int n) {
	a *= (PI / 180);
	double myMatrix[3][3] = {{cos(a),-sin(a),0}, {sin(a),cos(a),0}, {0,0,1}};
	for (int i = 0; i < n; i++) {
		double pointMatrix[3] = {l[i].x,l[i].y,1};
		double resMatrix[3] = {0};
		matrixMath(myMatrix, pointMatrix, resMatrix);
		l[i].x = resMatrix[0];
		l[i].y = resMatrix[1];
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
// the redraw function
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);     // clear the screen

	// Grid
	glColor3f(0.85f, 0.85f, 0.85f);
	glBegin(GL_LINES);
		for (int i = 0; i <= width; i += 25) {
			glVertex2i(i, 0);
			glVertex2i(i, height);
		}
		for (int i = 0; i <= height; i += 25) {
			glVertex2i(0, i);
			glVertex2i(width, i);
		}
	glEnd();

	// original triangle
	glColor3f(0.0f, 0.0f, 0.0f);
	point lineList1[3] = {};
	lineList1[0].x = 95;
	lineList1[0].y = 200;
	lineList1[1].x = 125;
	lineList1[1].y = 300;
	lineList1[2].x = 155;
	lineList1[2].y = 200;

	glBegin(GL_LINES);
		glVertex2i(lineList1[0].x, lineList1[0].y);
		glVertex2i(lineList1[1].x, lineList1[1].y);
		glVertex2i(lineList1[1].x, lineList1[1].y);
		glVertex2i(lineList1[2].x, lineList1[2].y);
		glVertex2i(lineList1[2].x, lineList1[2].y);
		glVertex2i(lineList1[0].x, lineList1[0].y);
	glEnd();

	// new triangle
	myScale(lineList1, 1.5, 1.5, 3);
	myRotate(lineList1, 15, 3);
	myTranslate(lineList1, 150, -250, 3);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
		glVertex2i(lineList1[0].x, lineList1[0].y);
		glVertex2i(lineList1[1].x, lineList1[1].y);
		glVertex2i(lineList1[1].x, lineList1[1].y);
		glVertex2i(lineList1[2].x, lineList1[2].y);
		glVertex2i(lineList1[2].x, lineList1[2].y);
		glVertex2i(lineList1[0].x, lineList1[0].y);
	glEnd();

	// original square
	glColor3f(0.0f, 0.0f, 0.0f);
	point lineList2[4] = {};
	lineList2[0].x = 350;
	lineList2[0].y = 50;
	lineList2[1].x = 350;
	lineList2[1].y = 150;
	lineList2[2].x = 450;
	lineList2[2].y = 150;
	lineList2[3].x = 450;
	lineList2[3].y = 50;

	glBegin(GL_LINES);
		glVertex2i(lineList2[0].x, lineList2[0].y);
		glVertex2i(lineList2[1].x, lineList2[1].y);
		glVertex2i(lineList2[1].x, lineList2[1].y);
		glVertex2i(lineList2[2].x, lineList2[2].y);
		glVertex2i(lineList2[2].x, lineList2[2].y);
		glVertex2i(lineList2[3].x, lineList2[3].y);
		glVertex2i(lineList2[3].x, lineList2[3].y);
		glVertex2i(lineList2[0].x, lineList2[0].y);
	glEnd();

	// new square
	myScale(lineList2, .5, .5, 4);
	myTranslate(lineList2, -100, 300, 4);
	myRotate(lineList2, -15, 4);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
		glVertex2i(lineList2[0].x, lineList2[0].y);
		glVertex2i(lineList2[1].x, lineList2[1].y);
		glVertex2i(lineList2[1].x, lineList2[1].y);
		glVertex2i(lineList2[2].x, lineList2[2].y);
		glVertex2i(lineList2[2].x, lineList2[2].y);
		glVertex2i(lineList2[3].x, lineList2[3].y);
		glVertex2i(lineList2[3].x, lineList2[3].y);
		glVertex2i(lineList2[0].x, lineList2[0].y);
	glEnd();

	// original pentagon
	glColor3f(0.0f, 0.0f, 0.0f);
	point lineList3[5] = {};
	lineList3[0].x = 250;
	lineList3[0].y = 350;
	lineList3[1].x = 225;
	lineList3[1].y = 430;
	lineList3[2].x = 300;
	lineList3[2].y = 490;
	lineList3[3].x = 375;
	lineList3[3].y = 430;
	lineList3[4].x = 350;
	lineList3[4].y = 350;

	glBegin(GL_LINES);
		glVertex2i(lineList3[0].x, lineList3[0].y);
		glVertex2i(lineList3[1].x, lineList3[1].y);
		glVertex2i(lineList3[1].x, lineList3[1].y);
		glVertex2i(lineList3[2].x, lineList3[2].y);
		glVertex2i(lineList3[2].x, lineList3[2].y);
		glVertex2i(lineList3[3].x, lineList3[3].y);
		glVertex2i(lineList3[3].x, lineList3[3].y);
		glVertex2i(lineList3[4].x, lineList3[4].y);
		glVertex2i(lineList3[4].x, lineList3[4].y);
		glVertex2i(lineList3[0].x, lineList3[0].y);
	glEnd();

	// new pentagon
	myScale(lineList3, .75, .75, 5);
	myRotate(lineList3, 30, 5);
	myTranslate(lineList3, 400, -100, 5);
	glColor3f(0.0f, 0.0f, 1.0f);

	glBegin(GL_LINES);
		glVertex2i(lineList3[0].x, lineList3[0].y);
		glVertex2i(lineList3[1].x, lineList3[1].y);
		glVertex2i(lineList3[1].x, lineList3[1].y);
		glVertex2i(lineList3[2].x, lineList3[2].y);
		glVertex2i(lineList3[2].x, lineList3[2].y);
		glVertex2i(lineList3[3].x, lineList3[3].y);
		glVertex2i(lineList3[3].x, lineList3[3].y);
		glVertex2i(lineList3[4].x, lineList3[4].y);
		glVertex2i(lineList3[4].x, lineList3[4].y);
		glVertex2i(lineList3[0].x, lineList3[0].y);
	glEnd();

	glFlush();		                 // send all output to display 
}
//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char **argv)
{
	glutInit(&argc, argv);          // initialize the toolkit
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set the display mode
	glutInitWindowSize(640,480);     // set the window size
	glutInitWindowPosition(100, 150); // set the window position on the screen
	glutCreateWindow("2D Transformations"); // open the screen window(with its exciting title)
	glutDisplayFunc(myDisplay);     // register the redraw function
	myInit();                   
	glutMainLoop(); 		     // go into a perpetual loop
	
	return 0;
}
