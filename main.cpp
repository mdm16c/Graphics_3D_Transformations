#include <windows.h>   // use as needed for your system
#include <gl/Gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <bits/stdc++.h>
#include <sstream>

using namespace std;

#define PI 3.14159265

// screen vars
const int height = 500;
const int width = 650;

// create a struct to hold the x and y values for points
struct point {
	double x;
	double y;
	double z;
};

// struct to hold transformation details
struct myTransform {
	string transformType;
	string transformName;
	int id;
	double x;
	double y;
	double z;
};

// id counter variable to make sure each transform has a unique id
int idCounter = 0;

// line list + length
point lineList[41] = {};
const int allPoints = 41;

// stack to hold transformations
stack <myTransform> st;

// string to hold transformations
string allTransformationNames = "";

// debug function to print text to screen
void printString() {
	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(-7,-4);
	//get the length of the string to display
	int len = allTransformationNames.length();
	//loop to display character by character
	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,allTransformationNames[i]);
	}
}

// initialize the line list array
void initLineList() {
	// init stack
	while (!st.empty()) {
		st.pop();
	}
	allTransformationNames = "";

	// base of house
	// back face
	lineList[0].x = -1;lineList[0].y = -1;lineList[0].z = 0;
	lineList[1].x = 1;lineList[1].y = -1;lineList[1].z = 0;
	lineList[2].x = 1;lineList[2].y = -3;lineList[2].z = 0;
	lineList[3].x = -1;lineList[3].y = -3;lineList[3].z = 0;

	// front face
	lineList[4].x = -1;lineList[4].y = -1;lineList[4].z = -2;
	lineList[5].x = 1;lineList[5].y = -1;lineList[5].z = -2;
	lineList[6].x = 1;lineList[6].y = -3;lineList[6].z = -2;
	lineList[7].x = -1;lineList[7].y = -3;lineList[7].z = -2;

	// top of house
	lineList[8].x = 0;lineList[8].y = 1;lineList[8].z = -1;

	// left tree trunk
	// back face
	lineList[9].x = -3.5;lineList[9].y = 1;lineList[9].z = 0;
	lineList[10].x = -2.5;lineList[10].y = 1;lineList[10].z = 0;
	lineList[11].x = -2.5;lineList[11].y = -3;lineList[11].z = 0;
	lineList[12].x = -3.5;lineList[12].y = -3;lineList[12].z = 0;

	// front face
	lineList[13].x = -3.5;lineList[13].y = 1;lineList[13].z = -1;
	lineList[14].x = -2.5;lineList[14].y = 1;lineList[14].z = -1;
	lineList[15].x = -2.5;lineList[15].y = -3;lineList[15].z = -1;
	lineList[16].x = -3.5;lineList[16].y = -3;lineList[16].z = -1;

	// left tree top
	// back face
	lineList[17].x = -4.5;lineList[17].y = 4;lineList[17].z = 0;
	lineList[18].x = -1.5;lineList[18].y = 4;lineList[18].z = 0;
	lineList[19].x = -1.5;lineList[19].y = 1;lineList[19].z = 0;
	lineList[20].x = -4.5;lineList[20].y = 1;lineList[20].z = 0;

	// front face
	lineList[21].x = -4.5;lineList[21].y = 4;lineList[21].z = -1;
	lineList[22].x = -1.5;lineList[22].y = 4;lineList[22].z = -1;
	lineList[23].x = -1.5;lineList[23].y = 1;lineList[23].z = -1;
	lineList[24].x = -4.5;lineList[24].y = 1;lineList[24].z = -1;

	// right tree trunk
	// back face
	lineList[25].x = 3.5;lineList[25].y = 1;lineList[25].z = 0;
	lineList[26].x = 2.5;lineList[26].y = 1;lineList[26].z = 0;
	lineList[27].x = 2.5;lineList[27].y = -3;lineList[27].z = 0;
	lineList[28].x = 3.5;lineList[28].y = -3;lineList[28].z = 0;

	// front face
	lineList[29].x = 3.5;lineList[29].y = 1;lineList[29].z = -1;
	lineList[30].x = 2.5;lineList[30].y = 1;lineList[30].z = -1;
	lineList[31].x = 2.5;lineList[31].y = -3;lineList[31].z = -1;
	lineList[32].x = 3.5;lineList[32].y = -3;lineList[32].z = -1;

	// right tree top
	// back face
	lineList[33].x = 4.5;lineList[33].y = 4;lineList[33].z = 0;
	lineList[34].x = 1.5;lineList[34].y = 4;lineList[34].z = 0;
	lineList[35].x = 1.5;lineList[35].y = 1;lineList[35].z = 0;
	lineList[36].x = 4.5;lineList[36].y = 1;lineList[36].z = 0;

	// front face
	lineList[37].x = 4.5;lineList[37].y = 4;lineList[37].z = -1;
	lineList[38].x = 1.5;lineList[38].y = 4;lineList[38].z = -1;
	lineList[39].x = 1.5;lineList[39].y = 1;lineList[39].z = -1;
	lineList[40].x = 4.5;lineList[40].y = 1;lineList[40].z = -1;
}

// do matrix math for a 4x4 and 1x4 matrix
void matrixMath(double m1[4][4], double m2[4], double res[4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res[i] += m1[i][j] * m2[j];
		}
	}
}

// translate everything in line list by x, y, and z
void doTranslate(double x, double y, double z) {
	double myMatrix[4][4] = {{1,0,0,x}, {0,1,0,y}, {0,0,1,z}, {0,0,0,1}};
	for (int i = 0; i < allPoints; i++) {
		double pointMatrix[4] = {lineList[i].x,lineList[i].y,lineList[i].z,1};
		double resMatrix[4] = {0};
		matrixMath(myMatrix, pointMatrix, resMatrix);
		lineList[i].x = resMatrix[0];lineList[i].y = resMatrix[1];lineList[i].z = resMatrix[2];
	}
}

// scale all points in line list by x, y, and z
void doScaling(double x, double y, double z) {
	double myMatrix[4][4] = {{x,0,0,0}, {0,y,0,0}, {0,0,z,0}, {0,0,0,1}};
	double deltaX, deltaY, deltaZ;
	for (int i = 0; i < allPoints; i++) {
		double pointMatrix[4] = {lineList[i].x,lineList[i].y,lineList[i].z,1};
		double resMatrix[4] = {0};
		matrixMath(myMatrix, pointMatrix, resMatrix);

		// force the scaling to be from the first point in the shape rather than screen space
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

// do a rotation on the x axis for every point in linelist
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

// do a rotation on the y axis for every point in linelist
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

// do a rotation on the z axis for every point in linelist
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

int findLastIndex(string str, char x) {
	// Traverse from right
	for (int i = str.length() - 1; i >= 0; i--)
		if (str[i] == x)
			return i;
	return -1;
}

void addTransformName(string s) {
	if (allTransformationNames != "") {
		allTransformationNames += ", ";
	}
	allTransformationNames += s;
}

// perform all transformations in the stack at once
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

        	// make sure we are only given one x, y, or z input
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
    // reset id values
    idCounter = 0;
    allTransformationNames = "";
}

// perform top transformations in the stack
void execTopStack() {
	if (!st.empty()) {
        myTransform temp = st.top();

        if (temp.transformType == "T") {
        	doTranslate(temp.x, temp.y, temp.z);
        }
        else if (temp.transformType == "S") {
        	doScaling(temp.x, temp.y, temp.z);
        }
        else if (temp.transformType == "R") {
        	if (temp.x < 0 || temp.x > 0) {
        		doRotateX(temp.x);
        	}
        	if (temp.y < 0 || temp.y > 0) {
        		doRotateY(temp.y);
        	}
        	if (temp.z < 0 || temp.z > 0) {
        		doRotateZ(temp.z);
        	}
        }

        st.pop();
    }
    // decrement id counter
    idCounter--;
    int lastI = findLastIndex(allTransformationNames, ',');
    if (lastI != -1) {
    	allTransformationNames = allTransformationNames.erase(lastI);
    }
    else if (st.empty()) {
    	allTransformationNames = "";
    }
}

// remove transformation with given id from stack
void removeFromStack(int idNumber) {
	stack <myTransform> tempStack;
	idCounter = 0;
	allTransformationNames = "";
	// move everything to new stack pulling out the one we want to remove
	while (!st.empty()) {
        myTransform temp = st.top();

        if (temp.id != idNumber) {;
        	tempStack.push(temp);
        }

        st.pop();
    }

    // put everything back in our stack to keep order
    while (!tempStack.empty()) {
        myTransform temp = tempStack.top();
        temp.id = idCounter;
        idCounter++;
        allTransformationNames += temp.transformName + ", ";
        st.push(temp);
        tempStack.pop();
    }

    // trim last comma and space
    int lastI = findLastIndex(allTransformationNames, ',');
    if (lastI != -1) {
    	allTransformationNames = allTransformationNames.erase(lastI);
    }
    else if (st.empty()) {
    	allTransformationNames = "";
    }
}

// debugging using keyboard for commands instead of ui
// the if key statements can be replaced with if botton pressed then we can take the values in te fields for the x,y,z
// dont forget to defualt to zero if a field is left blank
// ignore the command.xyz != 0 for now. later we will want to check that
void keyboard(unsigned char key,int x,int y)
{
	myTransform command;
	// Q
	if(key==113 && (command.x != 0 || command.y != 0 || command.z != 0)) {
		command.transformType = "T";
		command.x = .25; command.y = 0; command.z = .25;
		command.id = idCounter;
		idCounter++;
		command.transformName = "Trans";
		ostringstream ss1, ss2, ss3;
		if (command.x > 0 || command.x < 0) {
			ss1 << command.x;
			command.transformName += "X" + ss1.str();
		}
		if (command.y > 0 || command.y < 0) {
			ss2 << command.y;
			command.transformName += "Y" + ss2.str();
		}
		if (command.z > 0 || command.z < 0) {
			ss3 << command.z;
			command.transformName += "Z" + ss3.str();
		}
		st.push(command);
		addTransformName(command.transformName);
	}

	// W
	if(key==119 && (command.x != 0 || command.y != 0 || command.z != 0)) {
		command.transformType = "S";
		command.x = 1.2; command.y = 1.2; command.z = 1.2;
		command.id = idCounter;
		idCounter++;
		command.transformName = "Scale";
		ostringstream ss1, ss2, ss3;
		if (command.x > 0 || command.x < 0) {
			ss1 << command.x;
			command.transformName += "X" + ss1.str();
		}
		if (command.y > 0 || command.y < 0) {
			ss2 << command.y;
			command.transformName += "Y" + ss2.str();
		}
		if (command.z > 0 || command.z < 0) {
			ss3 << command.z;
			command.transformName += "Z" + ss3.str();
		}
		st.push(command);
		addTransformName(command.transformName);
	}

	// E
	if(key==101 && (command.x != 0 || command.y != 0 || command.z != 0)) {
		command.transformType = "R";
		command.x = 5; command.y = 0; command.z = 0;
		command.id = idCounter;
		idCounter++;
		command.transformName = "Rot";
		ostringstream ss1, ss2, ss3;
		if (command.x > 0 || command.x < 0) {
			ss1 << command.x;
			command.transformName += "X" + ss1.str();
		}
		if (command.y > 0 || command.y < 0) {
			ss2 << command.y;
			command.transformName += "Y" + ss2.str();
		}
		if (command.z > 0 || command.z < 0) {
			ss3 << command.z;
			command.transformName += "Z" + ss3.str();
		}
		st.push(command);
		addTransformName(command.transformName);
	}

	// Y
	if(key==121) {
		execStack();
	}

	// U
	if(key==117) {
		initLineList();
	}

	// I
	if(key==105) {
		execTopStack();
	}

	// O
	if(key==111) {
		// param is the id of a transform, so we will need to get the field being removed
		removeFromStack(0);
	}

	glutPostRedisplay();
}

// the redraw function
void myDisplay(void)
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glLoadIdentity();		// clear modelview

	// put our scene in view
	// use the third value for zoom, 0 = no zoom, >0 = zoom
	gluLookAt(0.0, 0.0, 8.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// draw scene
	glBegin(GL_LINES);

		// house base
		// front face
		glVertex3d(lineList[0].x, lineList[0].y, lineList[0].z); glVertex3d(lineList[1].x, lineList[1].y, lineList[1].z);
		glVertex3d(lineList[1].x, lineList[1].y, lineList[1].z); glVertex3d(lineList[2].x, lineList[2].y, lineList[2].z);
		glVertex3d(lineList[2].x, lineList[2].y, lineList[2].z); glVertex3d(lineList[3].x, lineList[3].y, lineList[3].z);
		glVertex3d(lineList[3].x, lineList[3].y, lineList[3].z); glVertex3d(lineList[0].x, lineList[0].y, lineList[0].z);

		// back face
		glVertex3d(lineList[4].x, lineList[4].y, lineList[4].z); glVertex3d(lineList[5].x, lineList[5].y, lineList[5].z);
		glVertex3d(lineList[5].x, lineList[5].y, lineList[5].z); glVertex3d(lineList[6].x, lineList[6].y, lineList[6].z);
		glVertex3d(lineList[6].x, lineList[6].y, lineList[6].z); glVertex3d(lineList[7].x, lineList[7].y, lineList[7].z);
		glVertex3d(lineList[7].x, lineList[7].y, lineList[7].z); glVertex3d(lineList[4].x, lineList[4].y, lineList[4].z);

		// connect the sides
		glVertex3d(lineList[0].x, lineList[0].y, lineList[0].z); glVertex3d(lineList[4].x, lineList[4].y, lineList[4].z);
		glVertex3d(lineList[1].x, lineList[1].y, lineList[1].z); glVertex3d(lineList[5].x, lineList[5].y, lineList[5].z);
		glVertex3d(lineList[2].x, lineList[2].y, lineList[2].z); glVertex3d(lineList[6].x, lineList[6].y, lineList[6].z);
		glVertex3d(lineList[3].x, lineList[3].y, lineList[3].z); glVertex3d(lineList[7].x, lineList[7].y, lineList[7].z);

		// house top
		glVertex3d(lineList[0].x, lineList[0].y, lineList[0].z); glVertex3d(lineList[8].x, lineList[8].y, lineList[8].z);
		glVertex3d(lineList[1].x, lineList[1].y, lineList[1].z); glVertex3d(lineList[8].x, lineList[8].y, lineList[8].z);
		glVertex3d(lineList[4].x, lineList[4].y, lineList[4].z); glVertex3d(lineList[8].x, lineList[8].y, lineList[8].z);
		glVertex3d(lineList[5].x, lineList[5].y, lineList[5].z); glVertex3d(lineList[8].x, lineList[8].y, lineList[8].z);

		// left tree base
		// front face
		glVertex3d(lineList[9].x, lineList[9].y, lineList[9].z); glVertex3d(lineList[10].x, lineList[10].y, lineList[10].z);
		glVertex3d(lineList[10].x, lineList[10].y, lineList[10].z); glVertex3d(lineList[11].x, lineList[11].y, lineList[11].z);
		glVertex3d(lineList[11].x, lineList[11].y, lineList[11].z); glVertex3d(lineList[12].x, lineList[12].y, lineList[12].z);
		glVertex3d(lineList[12].x, lineList[12].y, lineList[12].z); glVertex3d(lineList[9].x, lineList[9].y, lineList[9].z);

		// back face
		glVertex3d(lineList[13].x, lineList[13].y, lineList[13].z); glVertex3d(lineList[14].x, lineList[14].y, lineList[14].z);
		glVertex3d(lineList[14].x, lineList[14].y, lineList[14].z); glVertex3d(lineList[15].x, lineList[15].y, lineList[15].z);
		glVertex3d(lineList[15].x, lineList[15].y, lineList[15].z); glVertex3d(lineList[16].x, lineList[16].y, lineList[16].z);
		glVertex3d(lineList[16].x, lineList[16].y, lineList[16].z); glVertex3d(lineList[13].x, lineList[13].y, lineList[13].z);

		// connect the sides
		glVertex3d(lineList[9].x, lineList[9].y, lineList[9].z); glVertex3d(lineList[13].x, lineList[13].y, lineList[13].z);
		glVertex3d(lineList[10].x, lineList[10].y, lineList[10].z); glVertex3d(lineList[14].x, lineList[14].y, lineList[14].z);
		glVertex3d(lineList[11].x, lineList[11].y, lineList[11].z); glVertex3d(lineList[15].x, lineList[15].y, lineList[15].z);
		glVertex3d(lineList[12].x, lineList[12].y, lineList[12].z); glVertex3d(lineList[16].x, lineList[16].y, lineList[16].z);

		// left tree top
		// front face
		glVertex3d(lineList[17].x, lineList[17].y, lineList[17].z); glVertex3d(lineList[18].x, lineList[18].y, lineList[18].z);
		glVertex3d(lineList[18].x, lineList[18].y, lineList[18].z); glVertex3d(lineList[19].x, lineList[19].y, lineList[19].z);
		glVertex3d(lineList[19].x, lineList[19].y, lineList[19].z); glVertex3d(lineList[20].x, lineList[20].y, lineList[20].z);
		glVertex3d(lineList[20].x, lineList[20].y, lineList[20].z); glVertex3d(lineList[17].x, lineList[17].y, lineList[17].z);

		// back face
		glVertex3d(lineList[21].x, lineList[21].y, lineList[21].z); glVertex3d(lineList[22].x, lineList[22].y, lineList[22].z);
		glVertex3d(lineList[22].x, lineList[22].y, lineList[22].z); glVertex3d(lineList[23].x, lineList[23].y, lineList[23].z);
		glVertex3d(lineList[23].x, lineList[23].y, lineList[23].z); glVertex3d(lineList[24].x, lineList[24].y, lineList[24].z);
		glVertex3d(lineList[24].x, lineList[24].y, lineList[24].z); glVertex3d(lineList[21].x, lineList[21].y, lineList[21].z);

		// connect the sides
		glVertex3d(lineList[17].x, lineList[17].y, lineList[17].z); glVertex3d(lineList[21].x, lineList[21].y, lineList[21].z);
		glVertex3d(lineList[18].x, lineList[18].y, lineList[18].z); glVertex3d(lineList[22].x, lineList[22].y, lineList[22].z);
		glVertex3d(lineList[19].x, lineList[19].y, lineList[19].z); glVertex3d(lineList[23].x, lineList[23].y, lineList[23].z);
		glVertex3d(lineList[20].x, lineList[20].y, lineList[20].z); glVertex3d(lineList[24].x, lineList[24].y, lineList[24].z);

		// right tree base
		// front face
		glVertex3d(lineList[25].x, lineList[25].y, lineList[25].z); glVertex3d(lineList[26].x, lineList[26].y, lineList[26].z);
		glVertex3d(lineList[26].x, lineList[26].y, lineList[26].z); glVertex3d(lineList[27].x, lineList[27].y, lineList[27].z);
		glVertex3d(lineList[27].x, lineList[27].y, lineList[27].z); glVertex3d(lineList[28].x, lineList[28].y, lineList[28].z);
		glVertex3d(lineList[28].x, lineList[28].y, lineList[28].z); glVertex3d(lineList[25].x, lineList[25].y, lineList[25].z);

		// back face
		glVertex3d(lineList[29].x, lineList[29].y, lineList[29].z); glVertex3d(lineList[30].x, lineList[30].y, lineList[30].z);
		glVertex3d(lineList[30].x, lineList[30].y, lineList[30].z); glVertex3d(lineList[31].x, lineList[31].y, lineList[31].z);
		glVertex3d(lineList[31].x, lineList[31].y, lineList[31].z); glVertex3d(lineList[32].x, lineList[32].y, lineList[32].z);
		glVertex3d(lineList[32].x, lineList[32].y, lineList[32].z); glVertex3d(lineList[29].x, lineList[29].y, lineList[29].z);

		// connect the sides
		glVertex3d(lineList[25].x, lineList[25].y, lineList[25].z); glVertex3d(lineList[29].x, lineList[29].y, lineList[29].z);
		glVertex3d(lineList[26].x, lineList[26].y, lineList[26].z); glVertex3d(lineList[30].x, lineList[30].y, lineList[30].z);
		glVertex3d(lineList[27].x, lineList[27].y, lineList[27].z); glVertex3d(lineList[31].x, lineList[31].y, lineList[31].z);
		glVertex3d(lineList[28].x, lineList[28].y, lineList[28].z); glVertex3d(lineList[32].x, lineList[32].y, lineList[32].z);

		// right tree top
		// front face
		glVertex3d(lineList[33].x, lineList[33].y, lineList[33].z); glVertex3d(lineList[34].x, lineList[34].y, lineList[34].z);
		glVertex3d(lineList[34].x, lineList[34].y, lineList[34].z); glVertex3d(lineList[35].x, lineList[35].y, lineList[35].z);
		glVertex3d(lineList[35].x, lineList[35].y, lineList[35].z); glVertex3d(lineList[36].x, lineList[36].y, lineList[36].z);
		glVertex3d(lineList[36].x, lineList[36].y, lineList[36].z); glVertex3d(lineList[33].x, lineList[33].y, lineList[33].z);

		// back face
		glVertex3d(lineList[37].x, lineList[37].y, lineList[37].z); glVertex3d(lineList[38].x, lineList[38].y, lineList[38].z);
		glVertex3d(lineList[38].x, lineList[38].y, lineList[38].z); glVertex3d(lineList[39].x, lineList[39].y, lineList[39].z);
		glVertex3d(lineList[39].x, lineList[39].y, lineList[39].z); glVertex3d(lineList[40].x, lineList[40].y, lineList[40].z);
		glVertex3d(lineList[40].x, lineList[40].y, lineList[40].z); glVertex3d(lineList[37].x, lineList[37].y, lineList[37].z);

		// connect the sides
		glVertex3d(lineList[33].x, lineList[33].y, lineList[33].z); glVertex3d(lineList[37].x, lineList[37].y, lineList[37].z);
		glVertex3d(lineList[34].x, lineList[34].y, lineList[34].z); glVertex3d(lineList[38].x, lineList[38].y, lineList[38].z);
		glVertex3d(lineList[35].x, lineList[35].y, lineList[35].z); glVertex3d(lineList[39].x, lineList[39].y, lineList[39].z);
		glVertex3d(lineList[36].x, lineList[36].y, lineList[36].z); glVertex3d(lineList[40].x, lineList[40].y, lineList[40].z);

	glEnd();
	// printString();
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