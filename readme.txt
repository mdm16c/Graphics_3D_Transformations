compile with: g++ -o main -Wall main.cpp -mwindows glut32.lib -lopengl32 -lglu32
run with: main

Use key presses to perform actions
Q - adds a transformation to the stack with the values .25, .5, and .75 for x, y, and z respectively
W - adds a scale to the stack with the values 1.2, 1.3, and 1.4 for x, y, and z respectively
E - adds a rotation to the stack with the values 5, 10, and 2 for x, y, and z respectively
Y - execute everything currently in the stack
U - reset the stack and screen
I - execute the transformation at the top of the stack
O - remove the transformation at the 2nd to bottom of the stack (if it exists)