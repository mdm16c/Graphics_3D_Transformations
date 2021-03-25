#include <windows.h>   // use as needed for your system
#include <gl/Gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <stdio.h>
#include <stdlib.h>

const int height = 500;
const int width = 650;

// debugging
int rot = 0;

// create a struct to hold the x and y values for points
struct point {
	double x;
	double y;
	double z;
};

// debugging
void keyboard(unsigned char key,int x,int y)
{
	if(key==27) exit(1);
	rot+=5;				// increment the rotation angle by 5 degrees
	rot%=360;			// ... and keep it within 0-359
	glutPostRedisplay();
}

// the redraw function
void myDisplay(void)
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glLoadIdentity();		// clear modelview

	// debugging
	glTranslatef(0,-2,-8);
	glRotatef(rot,0,1,0);

	point lineList1[8] = {};

	// back face
	lineList1[0].x = 0;lineList1[0].y = 1;lineList1[0].z = -1;
	lineList1[1].x = 1;lineList1[1].y = 1;lineList1[1].z = -1;
	lineList1[2].x = 1;lineList1[2].y = 0;lineList1[2].z = -1;
	lineList1[3].x = 0;lineList1[3].y = 0;lineList1[3].z = -1;

	// front face
	lineList1[4].x = 0;lineList1[4].y = 1;lineList1[4].z = -2;
	lineList1[5].x = 1;lineList1[5].y = 1;lineList1[5].z = -2;
	lineList1[6].x = 1;lineList1[6].y = 0;lineList1[6].z = -2;
	lineList1[7].x = 0;lineList1[7].y = 0;lineList1[7].z = -2;

	// vertices on 1 side
	int sides1 = 4;

	glBegin(GL_LINES);
		// front face
		for (int i = 0; i < sides1; i++) {
			glVertex3d(lineList1[i].x, lineList1[i].y, lineList1[i].z);
			glVertex3d(lineList1[(i+1)%sides1].x, lineList1[(i+1)%sides1].y, lineList1[(i+1)%sides1].z);
		}
		// back face
		for (int i = sides1; i < sides1*2; i++) {
			glVertex3d(lineList1[i].x, lineList1[i].y, lineList1[i].z);
			if (i == sides1*2-1) {
				glVertex3d(lineList1[sides1].x, lineList1[sides1].y, lineList1[sides1].z);
			}
			else {
				glVertex3d(lineList1[i+1].x, lineList1[i+1].y, lineList1[i+1].z);
			}
		}
		// connect the sides
		for (int i = 0; i < sides1; i++) {
			glVertex3d(lineList1[i].x, lineList1[i].y, lineList1[i].z);
			glVertex3d(lineList1[i].x, lineList1[i].y, lineList1[i+sides1].z);
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

	// myInit();                   
	glutMainLoop(); 		     // go into a perpetual loop
	
	return 0;
}