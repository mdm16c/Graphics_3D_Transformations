#include <windows.h>   // use as needed for your system
#include <gl/Gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <bits/stdc++.h>

using namespace std;

#define PI 3.14159265

const int height = 500;
const int width = 650;

// create a struct to hold the x and y values for points
struct point {
	double x;
	double y;
	double z;
};

struct myTransform {
	string transformType;
	double x;
	double y;
	double z;
};

point lineList[8] = {};
const int allPoints = 8;

stack <myTransform> st;

void initLineList() {
	// back face
	lineList[0].x = 0;lineList[0].y = 1;lineList[0].z = -1;
	lineList[1].x = 1;lineList[1].y = 1;lineList[1].z = -1;
	lineList[2].x = 1;lineList[2].y = 0;lineList[2].z = -1;
	lineList[3].x = 0;lineList[3].y = 0;lineList[3].z = -1;

	// front face
	lineList[4].x = 0;lineList[4].y = 1;lineList[4].z = -2;
	lineList[5].x = 1;lineList[5].y = 1;lineList[5].z = -2;
	lineList[6].x = 1;lineList[6].y = 0;lineList[6].z = -2;
	lineList[7].x = 0;lineList[7].y = 0;lineList[7].z = -2;
}

void matrixMath(double m1[4][4], double m2[4], double res[4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res[i] += m1[i][j] * m2[j];
		}
	}
}

void doTranslate(double x, double y, double z) {
	double myMatrix[4][4] = {{1,0,0,x}, {0,1,0,y}, {0,0,1,z}, {0,0,0,1}};
	for (int i = 0; i < allPoints; i++) {
		double pointMatrix[4] = {lineList[i].x,lineList[i].y,lineList[i].z,1};
		double resMatrix[4] = {0};
		matrixMath(myMatrix, pointMatrix, resMatrix);
		lineList[i].x = resMatrix[0];lineList[i].y = resMatrix[1];lineList[i].z = resMatrix[2];
	}
}

void doScaling(double x, double y, double z) {
	double myMatrix[4][4] = {{x,0,0,0}, {0,y,0,0}, {0,0,z,0}, {0,0,0,1}};
	double deltaX, deltaY, deltaZ;
	for (int i = 0; i < allPoints; i++) {
		double pointMatrix[4] = {lineList[i].x,lineList[i].y,lineList[i].z,1};
		double resMatrix[4] = {0};
		matrixMath(myMatrix, pointMatrix, resMatrix);
		if (i == 0) {
			deltaX = lineList[i].x - resMatrix[0];
			deltaY = lineList[i].y - resMatrix[1];
			deltaZ = lineList[i].z - resMatrix[2];
		}
		lineList[i].x = resMatrix[0] + deltaX;
		lineList[i].y = resMatrix[1] + deltaY;
		lineList[i].z = resMatrix[2] + deltaZ;
	}
}

void doRotateX(double a) {
	a *= (PI / 180);
	double myMatrix[4][4] = {{1,0,0,0}, {0,cos(a),-sin(a),0}, {0,sin(a),cos(a),0}, {0,0,0,1}};
	for (int i = 0; i < allPoints; i++) {
		double pointMatrix[4] = {lineList[i].x,lineList[i].y,lineList[i].z,1};
		double resMatrix[4] = {0};
		matrixMath(myMatrix, pointMatrix, resMatrix);
		lineList[i].x = resMatrix[0];lineList[i].y = resMatrix[1];lineList[i].z = resMatrix[2];
	}
}

void doRotateY(double a) {
	a *= (PI / 180);
	double myMatrix[4][4] = {{cos(a),0,sin(a),0}, {0,1,0,0}, {-sin(a),0,cos(a),0}, {0,0,0,1}};
	for (int i = 0; i < allPoints; i++) {
		double pointMatrix[4] = {lineList[i].x,lineList[i].y,lineList[i].z,1};
		double resMatrix[4] = {0};
		matrixMath(myMatrix, pointMatrix, resMatrix);
		lineList[i].x = resMatrix[0];lineList[i].y = resMatrix[1];lineList[i].z = resMatrix[2];
	}
}

void doRotateZ(double a) {
	a *= (PI / 180);
	double myMatrix[4][4] = {{cos(a),-sin(a),0,0}, {sin(a),cos(a),0,0}, {0,0,1,0}, {0,0,0,1}};
	for (int i = 0; i < allPoints; i++) {
		double pointMatrix[4] = {lineList[i].x,lineList[i].y,lineList[i].z,1};
		double resMatrix[4] = {0};
		matrixMath(myMatrix, pointMatrix, resMatrix);
		lineList[i].x = resMatrix[0];lineList[i].y = resMatrix[1];lineList[i].z = resMatrix[2];
	}
}

void execStack() {
	while (!st.empty()) {
        myTransform temp = st.top();

        if (temp.transformType == "T") {
        	doTranslate(temp.x, temp.y, temp.z);
        }
        else if (temp.transformType == "S") {
        	doScaling(temp.x, temp.y, temp.z);
        }
        else if (temp.transformType == "R") {
        	if ((temp.x < 0 || temp.x > 0) && temp.y == 0 && temp.z == 0) {
        		doRotateX(temp.x);
        	}
        	else if ((temp.y < 0 || temp.y > 0) && temp.x == 0 && temp.z == 0) {
        		doRotateY(temp.y);
        	}
        	else if ((temp.z < 0 || temp.z > 0) && temp.x == 0 && temp.y == 0) {
        		doRotateZ(temp.z);
        	}
        }

        st.pop();
    }
}

// debugging
void keyboard(unsigned char key,int x,int y)
{
	myTransform command;
	// Q
	if(key==113) {
		command.transformType = "T";
		command.x = .25; command.y = .25; command.z = .25;
		st.push(command);
	}

	// W
	if(key==119) {
		command.transformType = "S";
		command.x = 1.2; command.y = 1.2; command.z = 1.2;
		st.push(command);
	}

	// E
	if(key==101) {
		command.transformType = "R";
		command.x = 5; command.y = 0; command.z = 0;
		st.push(command);
	}

	// R
	if(key==114) {
		command.transformType = "R";
		command.x = 0; command.y = 5; command.z = 0;
		st.push(command);
	}

	// T
	if(key==116) {
		command.transformType = "R";
		command.x = 0; command.y = 0; command.z = 5;
		st.push(command);
	}

	// Y
	if(key==121) {
		execStack();
	}

	// U
	if(key==117) {
		initLineList();
	}

	glutPostRedisplay();
}

// the redraw function
void myDisplay(void)
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glLoadIdentity();		// clear modelview

	// move origin away from camera
	glTranslatef(0,-2,-8);

	// vertices on 1 side
	int vSwitch = 4;
	glBegin(GL_LINES);
		// front face
		for (int i = 0; i < vSwitch; i++) {
			glVertex3d(lineList[i].x, lineList[i].y, lineList[i].z);
			glVertex3d(lineList[(i+1)%vSwitch].x, lineList[(i+1)%vSwitch].y, lineList[(i+1)%vSwitch].z);
		}
		// back face
		for (int i = vSwitch; i < vSwitch*2; i++) {
			glVertex3d(lineList[i].x, lineList[i].y, lineList[i].z);
			if (i == vSwitch*2-1) {
				glVertex3d(lineList[vSwitch].x, lineList[vSwitch].y, lineList[vSwitch].z);
			}
			else {
				glVertex3d(lineList[i+1].x, lineList[i+1].y, lineList[i+1].z);
			}
		}
		// connect the sides
		for (int i = 0; i < vSwitch; i++) {
			glVertex3d(lineList[i].x, lineList[i].y, lineList[i].z);
			glVertex3d(lineList[i+vSwitch].x, lineList[i+vSwitch].y, lineList[i+vSwitch].z);
		}
	glEnd();
	glutSwapBuffers();

	glFlush();		                 // send all output to display 
}

//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char **argv)
{
	glutInit(&argc, argv);          // initialize the toolkit
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); // set the display mode
	glutInitWindowSize(640,480);     // set the window size
	glutInitWindowPosition(100, 150); // set the window position on the screen
	glutCreateWindow("3D Transformations"); // open the screen window(with its exciting title)
	glutDisplayFunc(myDisplay);     // register the redraw function
	glutKeyboardFunc(keyboard);
	glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90,1.0,0.1,100);
	glMatrixMode(GL_MODELVIEW);
	initLineList();

	// myInit();                   
	glutMainLoop(); 		     // go into a perpetual loop
	
	return 0;
}