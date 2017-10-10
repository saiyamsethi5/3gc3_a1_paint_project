#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "math.h"
#include <iostream>

#ifdef __APPLE__
#	include <OpenGL/gl.h>
#	include <OpenGL/glu.h>
#	include <GLUT/glut.h>
#else
#	include <stdlib.h>
#	include <gl/glut.h>
#	include <gl/gl.h>
#endif

//default screen width and height
const int width = 1000;
const int height = 700;

//status variables - to keep track of color and shape status selected
std::string status_shape = "point"; //defines current selected shape
float status_cc [4] = {0,1,0,0}; //defines current selectec color - red

//global variables used within mouse processing
bool draw_points = false;
bool draw_line = false;
bool draw_rectangle = false;
bool draw_circle = false;
bool draw_radial = false;
bool firstClick = true;
float xi, yi, xf, yf;
int distance_from_x_axis = 0;
int distance_from_y_axis = 0;

//function to draw points
void drawPoints(int x, int y){
	glColor4f(status_cc[0],status_cc[1],status_cc[2],status_cc[3]);
	glPointSize(5);
	glBegin(GL_POINTS);
		glVertex2f(x, y);
	glEnd();
}

//Belo Bresenhan's Functions Referenced from:
//https://stackoverflow.com/questions/35015683/opengl-bresenham-line
//Function to draw line if X distance is greater than Y
void bresenhamX(int x1, int y1, int x2, int y2, int dx, int dy){
    int d = 2 * dy - dx;
    int incrE = 2 * dy;
    int incrNE = 2 * (dy - dx);

    if (!(x1 < x2)){
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    drawPoints(x1, y1);
    while (x1 < x2){
        if (d < 0){d += incrE;}
        else{
            if (y1 < y2){++y1;}
            else{--y1;}
						d += incrNE;
        }
        ++x1;
        drawPoints(x1, y1);
    }
}

//Function to draw line if X distance is lesser than Y
void bresenhamY(int x1, int y1, int x2, int y2, int dx, int dy){
    int d = 2 * dx - dy;
    int incrE = 2 * dx;
    int incrNE = 2 * (dx - dy);

    if (!(y1 < y2)) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    drawPoints(x1, y1);
    while (y1 < y2) {
        if (d < 0){d += incrE;}
        else {
            if (x1 > x2){--x1;}
            else{++x1;}
            d += incrNE;
        }
        ++y1;
        drawPoints(x1, y1);
    }
}

//Function to draw line based on points selected
void drawLine (int x1, int y1, int x2, int y2){
	int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);

  if (dx >= dy){bresenhamX(x1, y1, x2, y2, dx, dy);}
  else{bresenhamY(x1, y1, x2, y2, dx, dy);}
}

//Function to draw rectangles using drawLine
void drawRectangle(int x1, int y1, int x2, int y2){
	drawLine (x1, y1, x2, y1);
	drawLine (x2, y1, x2, y2);
	drawLine (x2, y2, x1, y2);
	drawLine (x1, y2, x1, y1);
}

//Referenced from the followling linke:
//https://gamedev.stackexchange.com/questions/55615/how-to-draw-a-circle-with-webgl-using-gl-points
//Function used to draw cirlces
void drawCircle (int x1, int y1, int x2, int y2){
	float radius = sqrt(pow(abs(x2 - x1), 2) + pow(abs (y2 - y1), 2));

	int num_points = radius*100;
	float increment = ((2*M_PI)/num_points);

	for (float d = 0; d <= (2*M_PI)-increment; d += increment) {
		int x = (sin(d) * radius) + x1;
		int y = (cos(d) * radius) + y1;
		drawPoints(x,y);
	}
}

//menu processing function - GLUT manu processing callback function
void menuProcess (int value){
	float r_val = ((float) rand() / (RAND_MAX));
	float g_val = ((float) rand() / (RAND_MAX));
	float b_val = ((float) rand() / (RAND_MAX));

	if (value == 0) {exit(0);}

	if (value == 1){status_cc[0]=1; status_cc[1]=0; status_cc[2]=0; status_cc[3]=0;}
	if (value == 2){status_cc[0]=0; status_cc[1]=1; status_cc[2]=0; status_cc[3]=0;}
	if (value == 3){status_cc[0]=0; status_cc[1]=0; status_cc[2]=1; status_cc[3]=0;}
	if (value == 4){status_cc[0]=0.5; status_cc[1]=0; status_cc[2]=0.5; status_cc[3]=0;}
	if (value == 5){status_cc[0]=1; status_cc[1]=1; status_cc[2]=0; status_cc[3]=0;}
	if (value == 11){status_cc[0]=r_val; status_cc[1]=g_val; status_cc[2]=b_val; status_cc[3]=0;}

	if (value == 6){status_shape="point"; draw_points=true; draw_radial=false;}
	if (value == 7){status_shape="line"; draw_line=true; draw_radial=false;}
	if (value == 8){status_shape="rectangle"; draw_rectangle=true; draw_radial=false;}
	if (value == 9){status_shape="circle"; draw_circle=true; draw_radial=false;}
	if (value == 12){status_shape="radial"; drawPoints(width/2, height/2); draw_radial=true;}

	if (value == 10) {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); glClearColor(0,0,0,1); glutSwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); glClearColor(0,0,0,1); glutSwapBuffers();}
}

//createMenu function - GLUT menu creation function
void createMenu(void){
		//menu id's for right-click options
		int colors_menu_id, shapes_menu_id;
		int colors_submenu_id, shapes_submenu_id;

		colors_submenu_id = glutCreateMenu(menuProcess);
		glutAddMenuEntry("Red", 1);
		glutAddMenuEntry("Green", 2);
		glutAddMenuEntry("Blue", 3);
		glutAddMenuEntry("Purple", 4);
		glutAddMenuEntry("Yellow", 5);
		glutAddMenuEntry("Random", 11);
		colors_menu_id = glutCreateMenu(menuProcess);

		shapes_submenu_id = glutCreateMenu(menuProcess);
		glutAddMenuEntry("Point", 6);
		glutAddMenuEntry("Line", 7);
		glutAddMenuEntry("Rectangle", 8);
		glutAddMenuEntry("Circle", 9);
		glutAddMenuEntry("Radial", 12);
		shapes_menu_id = glutCreateMenu(menuProcess);

		glutAddSubMenu("Colors", colors_submenu_id);
		glutAddSubMenu("Shapes", shapes_submenu_id);

		glutAddMenuEntry("Clear", 10);
		glutAddMenuEntry("Quit", 0);

		glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//mouseMove function to calculate mouse moves
void mouseMove(int x, int y) {
		y = height - y;
		if (draw_radial){
			distance_from_x_axis = abs(x-(width/2));
			distance_from_y_axis = abs(y-(height/2));

			drawPoints(x,y);

			drawPoints((width/2)-distance_from_x_axis,y);
			drawPoints((width/2)+distance_from_x_axis,y);
			drawPoints(x,(height/2)+(distance_from_y_axis));
			drawPoints(x,(height/2)-(distance_from_y_axis));
			drawPoints((width/2)-distance_from_x_axis, (height/2)-(distance_from_y_axis));
			drawPoints((width/2)+distance_from_x_axis, (height/2)+(distance_from_y_axis));
			drawPoints((width/2)-distance_from_x_axis, (height/2)+(distance_from_y_axis));
			drawPoints((width/2)+distance_from_x_axis, (height/2)-(distance_from_y_axis));

			drawPoints((width/2)+(distance_from_y_axis), (height/2)+(distance_from_x_axis));
			drawPoints((width/2)-(distance_from_y_axis), (height/2)+(distance_from_x_axis));
			drawPoints((width/2)-(distance_from_y_axis), (height/2)-(distance_from_x_axis));
			drawPoints((width/2)+(distance_from_y_axis), (height/2)-(distance_from_x_axis));

			glPushMatrix();
			glTranslatef(395,-250,0.0); // 3. Translate to the object's position.
			glRotatef(45.0,0.0,0.0,1.0); // 2. Rotate the object.

			drawPoints((width/2)-distance_from_x_axis,y);
			drawPoints((width/2)+distance_from_x_axis,y);
			drawPoints(x,(height/2)+(distance_from_y_axis));
			drawPoints(x,(height/2)-(distance_from_y_axis));
			drawPoints((width/2)-distance_from_x_axis, (height/2)-(distance_from_y_axis));
			drawPoints((width/2)+distance_from_x_axis, (height/2)+(distance_from_y_axis));
			drawPoints((width/2)-distance_from_x_axis, (height/2)+(distance_from_y_axis));
			drawPoints((width/2)+distance_from_x_axis, (height/2)-(distance_from_y_axis));

			drawPoints((width/2)+(distance_from_y_axis), (height/2)+(distance_from_x_axis));
			drawPoints((width/2)-(distance_from_y_axis), (height/2)+(distance_from_x_axis));
			drawPoints((width/2)-(distance_from_y_axis), (height/2)-(distance_from_x_axis));
			drawPoints((width/2)+(distance_from_y_axis), (height/2)-(distance_from_x_axis));

			glPopMatrix();
		}
    glutPostRedisplay();
}

//Mouse  processingfunction - GLUT mouse callback function
void mouseProcessing(int btn, int state, int x, int y){
	y = height-y;	//readjusts GLUT height to OpenGL height

	//checks is point selected and mouse clicked - recalls display function
	if (status_shape == "point"){
		if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
			if (firstClick){
				xi = x;
				yi = y;
				firstClick = true;
				draw_points = true;
				draw_line = false;
				draw_rectangle = false;
				draw_circle = false;
				draw_radial = false;
			}
			glutPostRedisplay();
		}
	}

	//checks is line selected and mouse clicked - recalls display function
	if (status_shape == "line"){
		if( btn == GLUT_LEFT_BUTTON && state == GLUT_UP ){
        if (firstClick){
						glutSwapBuffers();
            xi = x;
            yi = y;
            firstClick = false;
						draw_points = false;
						draw_line = false;
						draw_rectangle = false;
						draw_circle = false;
						draw_radial = false;
        }
        else{
						glutSwapBuffers();
            xf = x;
            yf = y;
            firstClick = true;
						draw_points = true;
						draw_line = true;
						draw_rectangle = false;
						draw_circle = false;
						draw_radial = false;
        }
				glutPostRedisplay();
    }
	}

	//checks is rectangle selected and mouse clicked - recalls display function
	if (status_shape == "rectangle"){
		if( btn == GLUT_LEFT_BUTTON && state == GLUT_UP ){
				if (firstClick){
						glutSwapBuffers();
						xi = x;
						yi = y;
						firstClick = false;
						draw_points = false;
						draw_line = false;
						draw_rectangle = false;
						draw_circle = false;
						draw_radial = false;
				}
				else{
						glutSwapBuffers();
						xf = x;
						yf = y;
						firstClick = true;
						draw_points = false;
						draw_line = false;
						draw_rectangle = true;
						draw_circle = false;
						draw_radial = false;
				}
				glutPostRedisplay();
		}
	}

	//checks is circle selected and mouse clicked - recalls display function
	if (status_shape == "circle"){
		if( btn == GLUT_LEFT_BUTTON && state == GLUT_UP ){
				if (firstClick){
						glutSwapBuffers();
						xi = x;
						yi = y;
						firstClick = false;
						draw_points = false;
						draw_line = false;
						draw_rectangle = false;
						draw_circle = false;
						draw_radial = false;
				}
				else{
						glutSwapBuffers();
						xf = x;
						yf = y;
						firstClick = true;
						draw_points = false;
						draw_line = false;
						draw_rectangle = false;
						draw_circle = true;
						draw_radial = false;
				}
				glutPostRedisplay();
		}
	}
	if (status_shape == "radial"){
		glutSwapBuffers();
		xi = x;
		yi = y;
		firstClick = false;
		draw_points = false;
		draw_line = false;
		draw_rectangle = false;
		draw_circle = false;
		draw_radial = true;
		glutPostRedisplay();
	}
}

//keyboard processing function - GLUT keyboard callback function
void keyboardProcessing(unsigned char key, int xIn, int yIn){
	switch (key){
		case 'q':
		case 'Q':
		case 27:	//27 is the esc key
			exit(0);
			break;
		case 'c':
		case 'C':
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); glClearColor(0,0,0,1); glutSwapBuffers();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); glClearColor(0,0,0,1); glutSwapBuffers();
	}

	if (key == 'w' || key == 'W'){
		status_shape="point";
		draw_points = true;
		draw_radial = false;
	}
	if (key == 'e' || key == 'E'){
		status_shape="line";
		draw_line= true;
		draw_radial = false;
	}
	if (key == 'r' || key == 'R'){
		status_shape="rectangle";
		draw_rectangle = true;
		draw_radial = false;
	}
	if (key == 't' || key == 'T'){
		status_shape="circle";
		draw_circle = true;
		draw_radial = false;
	}
	if (key == 'y' || key == 'Y'){
		status_shape="radial";
		drawPoints(width/2, height/2);
		draw_radial = true;
	}

	float r_val = ((float) rand() / (RAND_MAX));
	float g_val = ((float) rand() / (RAND_MAX));
	float b_val = ((float) rand() / (RAND_MAX));

	if (key == 49){status_cc[0]=1; status_cc[1]=0; status_cc[2]=0; status_cc[3]=0;} //red
	if (key == 50){status_cc[0]=0; status_cc[1]=1; status_cc[2]=0; status_cc[3]=0;} //green
	if (key == 51){status_cc[0]=0; status_cc[1]=0; status_cc[2]=1; status_cc[3]=0;} //blue
	if (key == 52){status_cc[0]=0.5; status_cc[1]=0; status_cc[2]=0.5; status_cc[3]=0;} //purple
	if (key == 53){status_cc[0]=1; status_cc[1]=1; status_cc[2]=0; status_cc[3]=0;} //yellow
	if (key == 54){status_cc[0]=r_val; status_cc[1]=g_val; status_cc[2]=b_val; status_cc[3]=0;} //random
}

//init function to initialize main window
void init(void){
	glClear(GL_COLOR_BUFFER_BIT); //clears the front buffer
	glClearColor(0,0,0,1); //sets the color of the front buffer
	glutSwapBuffers(); //switches to the front buffer

	glViewport( 0,0, width, height );
	glMatrixMode( GL_PROJECTION );
	glOrtho( 0.0, width, 0.0, height, 1.0, -1.0 ); //defines canvas mesurements

	/* initialize viewing values */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//display function - GLUT display callback function
void display(void){
	if (draw_points){
		glutSwapBuffers();
		drawPoints(xi, yi);
	}
	if (draw_line){
		glutSwapBuffers();
		drawLine(xi, yi, xf, yf);
  }
	if (draw_rectangle){
		drawRectangle(xi, yi, xf, yf);
	}
	if (draw_circle){
		drawCircle(xi, yi, xf, yf);
	}
	if (draw_radial){
		//Because so intensive, had to move the MotionFunc from the main to
		//the display to avoid calls each loop.
		glutSwapBuffers();
		glutMotionFunc(mouseMove);
	}
	glutSwapBuffers();
}

/* main function - program entry point */
int main(int argc, char** argv){
	//mandatory keyboard and menu commands
	printf("%s\n", "Welcome to Paint - Saiyam Sethi - sethis3 - 001318666");
	printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n", "Mouse Commands:", "-Draw: Point: Left Click to Draw", "-Draw: Line: Left Click once to indicate starting point. Left Click again to indicate ending point",
	"-Draw: Rectangle: Left Click to specify top-left corner. Left-Click again to specify bottom right corner", "-Draw: Circle: Left-Click to specify center point. Left-Click again to specify outer point",
	"-Draw: Radial: Left-Click and hold and move mouse to trigger continuous radial drawling.","-Access Menus: Right Click on the Screen for Shape and Color options");

	printf("%s\n%s\n%s\n%s\n\n", "Keyboard Commands:", "-Change Colors: 1-Red, 2-Green, 3-Blue, 4-Purple, 5-Yellow, 6-Random",
	"-Select Shape: w/W-Point, e/E-Line, r/R-Rectangle, t/T-Circle, y/Y-Radial",
	"-Options: c/C-Clear Screen, q/Q-Quit Program");

	printf("%s\n", "PLEASE READ THE README.TXT PROVIDED BEFORE RUNNING FOR FURTHER LIMITATIONS");

	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

  glutInitWindowSize (width, height); //sets window size
	glutCreateWindow("paint");	//creates the window

	createMenu(); //menu processing

	glutDisplayFunc(display);	//registers "display" as the display callback function
  glutKeyboardFunc(keyboardProcessing);
	glutMouseFunc(mouseProcessing);

	init();
  glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}
