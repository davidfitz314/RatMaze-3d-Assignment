#include <cmath>
#include <cstring>
#include <ctype.h>
#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#include "glut.h"
#include "maze.h"
#include "graphics.h"
#include "Tga.h"

// Global Variables (Only what you need!)
//double screen_x = 700;
//double screen_y = 500;


// Textures
const int num_textures = 4;
static GLuint texName[num_textures];

// tga image loader code.

gliGenericImage *readTgaImage(char *filename)
{
  FILE *file;
  gliGenericImage *image;

  file = fopen(filename, "rb");
  if (file == NULL) {
    printf("Error: could not open \"%s\"\n", filename);
    return NULL;
  }
  image = gliReadTGA(file, filename);
  fclose(file);
  if (image == NULL) {
    printf("Error: could not decode file format of \"%s\"\n", filename);
    return NULL;
  }
  return image;
}


// Generic image loader code.
gliGenericImage *readImage(char *filename)
{
	size_t size = strlen(filename);
	if(toupper(filename[size-3]) == 'T' && toupper(filename[size-2]) == 'G' && toupper(filename[size-1]) == 'A')
	{
		gliGenericImage * result = readTgaImage(filename);
		if(!result)
		{
			cerr << "Error opening " << filename << endl;
			_getch();
			exit(1);
		}
		return result;
	}
	else
	{
		cerr << "Unknown Filetype!\n";
		_getch();
		exit(1);
	}
}

// This resets the edges of the texture image to a given "border color".
// You must call this for clamped images that do not take up the whole polygon.
// Otherwise, the texture edges will smear outward across the rest
// of the polygon.
void SetBorder(gliGenericImage * image)
{
	// set a border color.
	unsigned int border_r=255;
	unsigned int border_g=255;
	unsigned int border_b=255;
	int x,y;
	y=0;
	for(x=0; x<image->width; x++)
	{
		image->pixels[ 3*(y*image->width + x) + 0]=border_r;
		image->pixels[ 3*(y*image->width + x) + 1]=border_g;
		image->pixels[ 3*(y*image->width + x) + 2]=border_b;
	}
	y=1;
	for(x=0; x<image->width; x++)
	{
		image->pixels[ 3*(y*image->width + x) + 0]=border_r;
		image->pixels[ 3*(y*image->width + x) + 1]=border_g;
		image->pixels[ 3*(y*image->width + x) + 2]=border_b;
	}
	y=image->height-1;
	for(x=0; x<image->width; x++)
	{
		image->pixels[ 3*(y*image->width + x) + 0]=border_r;
		image->pixels[ 3*(y*image->width + x) + 1]=border_g;
		image->pixels[ 3*(y*image->width + x) + 2]=border_b;
	}
	y=image->height-2;
	for(x=0; x<image->width; x++)
	{
		image->pixels[ 3*(y*image->width + x) + 0]=border_r;
		image->pixels[ 3*(y*image->width + x) + 1]=border_g;
		image->pixels[ 3*(y*image->width + x) + 2]=border_b;
	}

	x=0;
	for(y=0; y<image->height; y++)
	{
		image->pixels[ 3*(y*image->width + x) + 0]=border_r;
		image->pixels[ 3*(y*image->width + x) + 1]=border_g;
		image->pixels[ 3*(y*image->width + x) + 2]=border_b;
	}
	x=1;
	for(y=0; y<image->height; y++)
	{
		image->pixels[ 3*(y*image->width + x) + 0]=border_r;
		image->pixels[ 3*(y*image->width + x) + 1]=border_g;
		image->pixels[ 3*(y*image->width + x) + 2]=border_b;
	}
	x=image->width-1;
	for(y=0; y<image->height; y++)
	{
		int index = 3*(y*image->width + x);
		image->pixels[ index + 0]=border_r;
		image->pixels[ index + 1]=border_g;
		image->pixels[ index + 2]=border_b;
	}
	x=image->width-2;
	for(y=0; y<image->height; y++)
	{
		int index = 3*(y*image->width + x);
		image->pixels[ index + 0]=border_r;
		image->pixels[ index + 1]=border_g;
		image->pixels[ index + 2]=border_b;
	}
}

bool PowerOf2(int h)
{
	if(h!= 2 && h!=4 && h!=8 && h!=16 && h!=32 && h!=64 && h!=128 && 
				h!=256 && h!=512 && h!=1024 && h!=2048 && h!=4096)
		return false;
	else
		return true;
}



Cell::Cell()
{
	mLeft = true;
	mBottom = true;
	mRight = true;
	mTop = true;
	mVisited = false;
}

bool Cell::getTop()
{
	return this->mTop; 
}

bool Cell::getBottom()
{
	return this->mBottom; 
}

bool Cell::getLeft()
{
	return this->mLeft; 
}

bool Cell::getRight()
{
	return this->mRight; 
}

void Cell::SetTop(bool state)
{
	this->mTop = state;
}

void Cell::SetBottom(bool state)
{
	this->mBottom = state;
}

void Cell::SetRight(bool state)
{
	this->mRight = state;
}

void Cell::SetLeft(bool state)
{
	this->mLeft = state;
}

void Cell::Draw(int i, int j)
{
	
	if (mBottom)
	{
		if (j==0){
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texName[0]);
			glColor3ub(0, 0, 0);
			glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex3d(i,j,0);
			glTexCoord2f(1,0); glVertex3d(i+1,j,0);
			glTexCoord2f(1,1); glVertex3d(i+1,j,1);
			glTexCoord2f(0,1); glVertex3d(i,j,1);
			glEnd();
			DrawLine(i,j, i+1,j);
			glDisable(GL_TEXTURE_2D);
		}
	}
	if (mTop)
	{
		if (i%2 == 0){

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texName[2]);
			glColor3ub(55, 55, 55);
			glBegin(GL_QUADS);
			glTexCoord2f(-.5,-.5); glVertex3d(i,j+1,0);
			glTexCoord2f(1.5,-.5); glVertex3d(i+1,j+1,0);
			glTexCoord2f(1.5,1.5); glVertex3d(i+1,j+1,1);
			glTexCoord2f(-.5,1.5); glVertex3d(i,j+1,1);
			glEnd();
			DrawLine(i,j+1, i+1,j+1);
			glDisable(GL_TEXTURE_2D);

		} else if (i%3 == 0){
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texName[3]);
			glColor3ub(0, 0, 0);
			glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex3d(i,j+1,0);
			glTexCoord2f(8,0); glVertex3d(i+1,j+1,0);
			glTexCoord2f(8,4); glVertex3d(i+1,j+1,1);
			glTexCoord2f(0,4); glVertex3d(i,j+1,1);
			glEnd();
			DrawLine(i,j+1, i+1,j+1);
			glDisable(GL_TEXTURE_2D);
		} else {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texName[0]);
			glColor3ub(0, 0, 0);
			glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex3d(i,j+1,0);
			glTexCoord2f(2,0); glVertex3d(i+1,j+1,0);
			glTexCoord2f(2,2); glVertex3d(i+1,j+1,1);
			glTexCoord2f(0,2); glVertex3d(i,j+1,1);
			glEnd();
			DrawLine(i,j+1, i+1,j+1);
			glDisable(GL_TEXTURE_2D);

			
		}
	}
	if (mLeft)
	{
		if (i==0){
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texName[0]);
			glColor3ub(0, 0, 0);
			glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex3d(i,j,0);
			glTexCoord2f(2,0); glVertex3d(i,j+1,0);
			glTexCoord2f(2,2); glVertex3d(i,j+1,1);
			glTexCoord2f(0,2); glVertex3d(i,j,1);
			glEnd();
			DrawLine(i,j, i,j+1);
			glDisable(GL_TEXTURE_2D);
		}
	}
	if (mRight)
	{
		if (j%2==0){
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texName[0]);
			glColor3ub(0, 0, 0);
			glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex3d(i+1,j,0);
			glTexCoord2f(1,0); glVertex3d(i+1,j+1,0);
			glTexCoord2f(1,1); glVertex3d(i+1,j+1,1);
			glTexCoord2f(0,1); glVertex3d(i+1,j,1);
			glEnd();
			DrawLine(i+1,j, i+1,j+1);
			glDisable(GL_TEXTURE_2D);

		} else {
			
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texName[3]);
			glColor3ub(0, 0, 0);
			glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex3d(i+1,j,0);
			glTexCoord2f(1,0); glVertex3d(i+1,j+1,0);
			glTexCoord2f(1,1); glVertex3d(i+1,j+1,1);
			glTexCoord2f(0,1); glVertex3d(i+1,j,1);
			glEnd();
			DrawLine(i+1,j, i+1,j+1);
			glDisable(GL_TEXTURE_2D);
		}
	}
	
}

void Cell::SetVisited(bool state)
{
	this->mVisited = state;
}

bool Cell::GetVisited()
{
	return this->mVisited;
}

Maze::Maze()
{
}

void Maze::repeatImages()
{
	gliGenericImage *image[num_textures];
	int n=0;
	image[n++] = readImage("bricks.tga");
	image[n++] = readImage("carpet.tga");
	image[n++] = readImage("jailcell.tga");
	image[n++] = readImage("jailcell2.tga");
	if(n!=num_textures)
	{
		printf("Error: Wrong number of textures\n");
		_getch();
		exit(1);;
	}

	glGenTextures(num_textures, texName);

	for(int i=0; i<num_textures; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texName[i]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		int repeats = false;
		if (i == 0 || i ==3){
			repeats = true;
		} else {
			repeats = false;
		}
		int needs_border = false; // Needed if clamping and not filling the whole polygon.
		if (i == 2){
			needs_border = false;
		} else {
			needs_border = false;
		}
		if(repeats)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
		if(needs_border)
		{
			// set a border.
			SetBorder(image[i]);
		}

		bool mipmaps = false;
		if(!PowerOf2(image[i]->height) || !PowerOf2(image[i]->width))
		{
			// WARNING: Images that do not have width and height as 
			// powers of 2 MUST use mipmaps.
			mipmaps = true; 
		}

		if (mipmaps) 
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, image[i]->components,
					image[i]->width, image[i]->height,
					image[i]->format, GL_UNSIGNED_BYTE, image[i]->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					//GL_LINEAR_MIPMAP_LINEAR);
					GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
					//GL_LINEAR);
					GL_NEAREST);
		} 
		else 
		{
			glTexImage2D(GL_TEXTURE_2D, 0, image[i]->components,
					image[i]->width, image[i]->height, 0,
					image[i]->format, GL_UNSIGNED_BYTE, image[i]->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}

}

void Maze::Init()
{
	
	// Find a start cell:
	RemoveWallsR(0, 0);
	mStartI = rand()%M;
	mEndI = rand()%M;
	this->mCells[mStartI][0].SetBottom(false);
	this->mCells[mEndI][N-1].SetTop(false);
	//RemoveWallsR(mStartI,0);
	//RemoveWallsT(mEndI,3);
	
	
}



bool Maze::isItWall(double x_in, double y_in, double radius_in)
{
	int tempX = int(x_in);
	int tempY = int(y_in);
	//wall up, down, right, left
	if (mCells[tempX][tempY].getTop() && (y_in + radius_in) >= (tempY+1))
	{
		
		mCells[tempX][tempY].SetTop(false);
		return false;
	}
	if (mCells[tempX][tempY].getBottom() && (y_in - radius_in) <= (tempY))
	{
		
		mCells[tempX][tempY].SetBottom(false);
		return false;
	}
	if (mCells[tempX][tempY].getRight() && (x_in + radius_in) >= (tempX+1))
	{
		
		mCells[tempX][tempY].SetRight(false);
		return false;
	}
	if (mCells[tempX][tempY].getLeft() && (x_in - radius_in) <= (tempX))
	{
		
		mCells[tempX][tempY].SetLeft(false);
		return false;
	}
	//corners upl, upr, dl, dr
	//distance x_in - tempx
	//distance y_in - tempy
	//bottom right
	//if (r> offsetx && r>1-offsettx)return true
	double offsetX = x_in - tempX;
	double offsetY = y_in - tempY;
	if (radius_in > offsetY && radius_in > (1-offsetX)){
		return true;
	}
	if (radius_in > offsetY && radius_in > offsetX){
		return true;
	}
	if (radius_in > (1-offsetX) && radius_in > (1-offsetY)){
		return true;
	}
	if (radius_in > offsetX && radius_in > (1-offsetY)){
		return true;
	}
	return false;
}

bool Maze::isValid(int i_in, int j_in)
{
	//0 up
	//1 down
	//2 left
	//3 right
	if (i_in < 0 || j_in < 0){
		return false;
	}
	if (i_in >= M || j_in >= N){
		return false;
	}
	if (mCells[i_in][j_in].GetVisited()){
		return false;
	}
	return true;
}

void Maze::RemoveWallsR(int i, int j)
{
	this->mCells[i][j].SetVisited(true);
	vector<int> options;
	if (isValid(i,j+1)){
		options.push_back(0);
	}
	if (isValid(i,j-1)){
		options.push_back(1);
	}
	if (isValid(i-1,j)){
		options.push_back(2);
	}
	if (isValid(i+1,j)){
		options.push_back(3);
	}

	//shuffle the options
	random_shuffle(options.begin(), options.end());
	

	for (unsigned int m=0;m<options.size();m++){
		
		if (options[m] == 0){
			//up is next
			if (isValid(i,j+1)){
				mCells[i][j].SetTop(false);
				mCells[i][j+1].SetBottom(false);
				RemoveWallsR(i,j+1);
			}
		} else if (options[m] == 1){
			//down
			if (isValid(i,j-1)){
				mCells[i][j-1].SetTop(false);
				mCells[i][j].SetBottom(false);
				RemoveWallsR(i,j-1);
			}
		} else if (options[m] == 2){
			//left
			if (isValid(i-1,j)){
				mCells[i][j].SetLeft(false);
				mCells[i-1][j].SetRight(false);
				RemoveWallsR(i-1,j);
			}
		} else if (options[m] == 3){
			//right
			if (isValid(i+1,j)){
				mCells[i+1][j].SetLeft(false);
				mCells[i][j].SetRight(false);
				RemoveWallsR(i+1,j);
			}
		}
	}	
	
}

int Maze::getStart()
{
	return this->mStartI;
}

int Maze::getEndX()
{
	return this->mEndI;
}

int Maze::getEndY()
{
	return N-1;
}

int Maze::getM()
{
	return M;
}
int Maze::getN()
{
	return N;
}

void Maze::removeTopWall(double x_in, double y_in, bool bool_in)
{
	int tempx = int(x_in);
	int tempy = int(y_in);
	this->mCells[tempx][tempy].SetTop(bool_in);
	this->mCells[tempx][tempy+1].SetBottom(bool_in);
}

void Maze::removeBottomWall(double x_in, double y_in, bool bool_in)
{
	int tempx = int(x_in);
	int tempy = int(y_in);
	this->mCells[tempx][tempy].SetBottom(bool_in);
	this->mCells[tempx][tempy-1].SetTop(bool_in);
}

void Maze::removeRightWall(double x_in, double y_in, bool bool_in)
{
	int tempx = int(x_in);
	int tempy = int(y_in);
	this->mCells[tempx][tempy].SetRight(bool_in);
	this->mCells[tempx+1][tempy].SetLeft(bool_in);
}

void Maze::removeLeftWall(double x_in, double y_in, bool bool_in)
{
	int tempx = int(x_in);
	int tempy = int(y_in);
	this->mCells[tempx][tempy].SetLeft(bool_in);
	this->mCells[tempx-1][tempy].SetRight(bool_in);
}

void Maze::Draw()
{
	/*glColor3ub(178.5, 178.5, 178.5);
	glBegin(GL_QUADS);
	glVertex3d(0,0,1);
	glVertex3d(M,0,1);
	glVertex3d(M,N,1);
	glVertex3d(0,N,1);
	glEnd();*/
	

	for(int i=0; i<M; i++)
		for(int j=0; j<N; j++){
			if (i == this->getStart() && j==0){
				mCells[i][j].Draw(i,j);
				glColor3ub(51, 51, 178.5);
				glBegin(GL_QUADS);
				glVertex3d(i,j,0);
				glVertex3d(i+1,j,0);
				glVertex3d(i+1,j+1,0);
				glVertex3d(i,j+1,0);
				glEnd();
			}

			if (i == this->getEndX() && j==this->getEndY()){
				mCells[i][j].Draw(i,j);
				glColor3ub(51, 178.5, 51);
				glBegin(GL_QUADS);
				glVertex3d(i,j,0);
				glVertex3d(i+1,j,0);
				glVertex3d(i+1,j+1,0);
				glVertex3d(i,j+1,0);
				glEnd();
			} else {
				mCells[i][j].Draw(i,j);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, texName[1]);

				glBegin(GL_QUADS);
				glTexCoord2f(0,0); glVertex3d(i,j,0);
				glTexCoord2f(1,0); glVertex3d(i+1,j,0);
				glTexCoord2f(1,1); glVertex3d(i+1,j+1,0);
				glTexCoord2f(0,1); glVertex3d(i,j+1,0);
				glEnd();

				glDisable(GL_TEXTURE_2D);
			}
		}
	//if(current_view == top_view) 
	//{
	// draw walls as GL_LINES
	//}
	//else
	//{
	// draw walls as GL_QUADS
	// figure out a way to draw each wall in a different color. (you don't have to save the color of the wall)
	// figure out a way to prevent two co-planar wall from 'bleeding' on top of each other when drawing.
	//}
}


