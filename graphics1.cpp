// OpenGL/GLUT starter kit for Windows 7 and Visual Studio 2010
// Created spring, 2011
//
// This is a starting point for OpenGl applications.
// Add code to the "display" function below, or otherwise
// modify this file to get your desired results.
//
// For the first assignment, add this file to an empty Windows Console project
//		and then compile and run it as is.
// NOTE: You should also have glut.h,
// glut32.dll, and glut32.lib in the directory of your project.
// OR, see GlutDirectories.txt for a better place to put them.
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <ctime>
using namespace std;
#include "glut.h"
#include "maze.h"
#include "graphics.h"
#include "rat.h"

// Global Variables (Only what you need!)
double screen_x = 700;
double screen_y = 500;
bool gMouseLeft = false;
bool gMouseMiddle = false;
bool gMouseRight = false;

enum viewtype{top_view, perspective_view, rat_view};
viewtype current_view = top_view;

int mstart;
int mendx;
int mendy;
int mM;
int mN;
bool top = true;
bool bottom = true;
bool left = true;
bool right = true;
Maze gMaze;
Rat mRat;
double gZ = 0.0;
double setAltitude = 1.0;
bool myA = false;
bool myUp = false;
// 
// Functions that draw basic primitives
//

double function(double x, double y)
{
	double z = .5*sin(x*.25) + .4*sin(y*.15-.43);
	z += sin(x*.45-.7) * cos(y*.315-.31)+.5;
	z += sin(x*.15-.97) * sin(y*.35-8.31);
	double amplitute = setAltitude;
	z *= amplitute;
	return z;
}

void DrawCircle(double x1, double y1, double radius)
{
	glBegin(GL_POLYGON);
	for(int i=0; i<32; i++)
	{
		double theta = (double)i/32.0 * 2.0 * 3.1415926;
		double x = x1 + radius * cos(theta);
		double y = y1 + radius * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();
}

void DrawRectangle(double x1, double y1, double x2, double y2)
{
	glBegin(GL_QUADS);
	glVertex2d(x1,y1);
	glVertex2d(x2,y1);
	glVertex2d(x2,y2);
	glVertex2d(x1,y2);
	glEnd();
}

void DrawLine(double x1, double y1, double x2, double y2)
{
	glBegin(GL_LINES);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glEnd();
}

void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
	glBegin(GL_TRIANGLES);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glVertex2d(x3,y3);
	glEnd();
}

// Outputs a string of text at the specified location.
void DrawText(double x, double y, char *string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	
	int len, i;
	glRasterPos2d(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) 
	{
		glutBitmapCharacter(font, string[i]);
	}

    glDisable(GL_BLEND);
}


//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslated(mRat.getX(), mRat.getY(), gZ);
	if(current_view == perspective_view)
	{
		glEnable(GL_DEPTH_TEST);
		glLoadIdentity();
		gluLookAt(-3,-3,7,  3,3,0,  0,0,1);
	}
	else if(current_view == top_view)
	{
		glDisable(GL_DEPTH_TEST);
		glLoadIdentity();
	}
	else // current_view == rat_view
	{
		glEnable(GL_DEPTH_TEST);
		glLoadIdentity();
		double z_level = gZ + .25;
		double x = mRat.getX();
		double y = mRat.getY();
		double dx = cos(mRat.getDirection() / 180*M_PI);
		double dy = sin(mRat.getDirection() / 180*M_PI);
		double at_x = x + dx;
		double at_y = y + dy;
		double at_z = z_level;
		gluLookAt(x,y,z_level,  at_x, at_y, at_z,  0,0,1);
	}

	if (myA){
		_sleep(500);
		gZ = 0.0;
		myA = false;
	}

	glClear(GL_COLOR_BUFFER_BIT);
	if (gMouseLeft){
		mRat.spinLeft();
	}

	if (gMouseRight){
		mRat.spinRight();
	}

	if (gMouseMiddle){
		mRat.move();
	}


	glColor3d(0,0,1);
	mRat.Draw();
	gMaze.Draw();
	
	glutSwapBuffers();
	glutPostRedisplay();
}



void SetPerspectiveView(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble) w/(GLdouble) h;
	gluPerspective( 
	/* field of view in degree */ 38.0,
	/* aspect ratio */ aspectRatio,
	/* Z near */ .1, /* Z far */ 30.0);
	glMatrixMode(GL_MODELVIEW);
}

void SetTopView(int w, int h)
{
	// go into 2D mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double world_margin_x = 0.5;
	double world_margin_y = 0.5;
	gluOrtho2D(-world_margin_x, M+world_margin_x, 
		-world_margin_y, N+world_margin_y);
	glMatrixMode(GL_MODELVIEW);
}


// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y)
{
	switch (c) 
	{
		case 27: // escape character means to quit the program
			exit(0);
			break;
		case '1':
			
			mRat.Reset(mstart);
			// do something when 'b' character is hit.
			break;
		case '2':
			
			mRat.jEnd(mendx, mendy);
			// do something when 'b' character is hit.
			break;
		case '3':
			
			mRat.randomJump(mM, mN);
			// do something when 'b' character is hit.
			break;
		case 't':
			if (top){
				gMaze.removeTopWall(mRat.getX(), mRat.getY(), false);
				mRat.setMaze(&gMaze);
				top = false;
			} else {
				gMaze.removeTopWall(mRat.getX(), mRat.getY(), true);
				mRat.setMaze(&gMaze);
				top = true;
			}
			break;
		case 'b':
			if (top){
				gMaze.removeBottomWall(mRat.getX(), mRat.getY(), false);
				mRat.setMaze(&gMaze);
				top = false;
			} else {
				gMaze.removeBottomWall(mRat.getX(), mRat.getY(), true);
				mRat.setMaze(&gMaze);
				top = true;
			}
			break;
		case 'r':
			if (top){
				gMaze.removeRightWall(mRat.getX(), mRat.getY(), false);
				mRat.setMaze(&gMaze);
				top = false;
			} else {
				gMaze.removeRightWall(mRat.getX(), mRat.getY(), true);
				mRat.setMaze(&gMaze);
				top = true;
			}
			break;
		case 'l':
			if (top){
				gMaze.removeLeftWall(mRat.getX(), mRat.getY(), false);
				mRat.setMaze(&gMaze);
				top = false;
			} else {
				gMaze.removeLeftWall(mRat.getX(), mRat.getY(), true);
				mRat.setMaze(&gMaze);
				top = true;
			}
			break;
		case 'm':
			current_view = rat_view;
			SetPerspectiveView(screen_x, screen_y);
			break;
		case 'p':
			current_view = perspective_view;
			SetPerspectiveView(screen_x, screen_y);
			break;
		case 'u':
			current_view = top_view;
			SetTopView(screen_x, screen_y);
			break;
		case 'a':
			gZ = .25;
			myA = true;
			
			break;
		case 's':
			if (gZ >= 0){
				gZ -= .01;
			}
			break;
		case 'd':
			gZ = 0.0;
			break;
		
		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	// Reset our global variables to the new width and height.
	//screen_x = w;
	//screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	//glViewport(0, 0, w, h);

	// Set the projection mode to 2D orthographic, and set the world coordinates:
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(-.5, M+.5, -.5, N+.5);
	//glMatrixMode(GL_MODELVIEW);

	// reshape:
//void reshape(int w, int h)
//{
	screen_x = w;
	screen_y = h;
	glViewport(0, 0, w, h);

	if(current_view == top_view)
	{
		SetTopView(w,h);
	}
	else if(current_view == perspective_view)
	{
		SetPerspectiveView(w,h);
	}
	else // current_view == rat_view
	{
		SetPerspectiveView(w,h);
	}
//}

}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		gMouseLeft = true;
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
		gMouseLeft = false;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
		gMouseMiddle = true;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
		gMouseMiddle = false;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
	{
		gMouseRight = true;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_UP) 
	{
		gMouseRight = false;
	}
	glutPostRedisplay();
}

// Your initialization code goes here.
void InitializeMyStuff()
{
	gMaze.repeatImages();
	gMaze.Init();
	mRat.setMaze(&gMaze);
	mstart = gMaze.getStart();
	mendx = gMaze.getEndX();
	mendy = gMaze.getEndY();
	mM = gMaze.getM();
	mN = gMaze.getN();
	mRat.Init(double(mstart));
	current_view = rat_view;
	
	
}


int main(int argc, char **argv)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	srand(time(0));
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("This appears in the title bar");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glColor3d(0,0,0); // forground color
	glClearColor(1, 1, 1, 0); // background color
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}
