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
};

void myInit(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);      // set the bg color to a bright white
	glColor3f(0.0f, 0.0f, 0.0f);           // set the drawing color to black 
	glPointSize(1.0);		            //set the point size to 4 by 4 pixels
	glMatrixMode(GL_PROJECTION);// set up appropriate matrices- to be explained 
	glLoadIdentity();// to be explained
	gluOrtho2D(0.0, float(width), 0.0, float(height));// to be explained
}

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
	glClearColor( 0, 0, 0, 0 );
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glLoadIdentity();		// clear modelview
	glTranslatef( 0,-3,-8 );

	// debugging
	glRotatef(rot,0,1,0);

	glBegin(GL_LINES);
		// back face
		glVertex3i(0, 1, -1);
		glVertex3i(1, 1, -1);

		glVertex3i(1, 1, -1);
		glVertex3i(1, 0, -1);

		glVertex3i(1, 0, -1);
		glVertex3i(0, 0, -1);

		glVertex3i(0, 0, -1);
		glVertex3i(0, 1, -1);

		// front face
		glVertex3i(0, 1, -2);
		glVertex3i(1, 1, -2);

		glVertex3i(1, 1, -2);
		glVertex3i(1, 0, -2);

		glVertex3i(1, 0, -2);
		glVertex3i(0, 0, -2);

		glVertex3i(0, 0, -2);
		glVertex3i(0, 1, -2);

		// connect sides
		glVertex3i(1, 1, -2);
		glVertex3i(1, 1, -1);

		glVertex3i(0, 0, -2);
		glVertex3i(0, 0, -1);

		glVertex3i(0, 1, -2);
		glVertex3i(0, 1, -1);

		glVertex3i(1, 0, -2);
		glVertex3i(1, 0, -1);
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

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 90,1.0,0.1,100 );
	glMatrixMode( GL_MODELVIEW );

	// myInit();                   
	glutMainLoop(); 		     // go into a perpetual loop
	
	return 0;
}