#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void printM(int ***data, int h, int w, int r);
void initialize(int ***array, int height, int width, int range);
void voting(int ***array, int height, int width, int range, int **image);
void threshold(int ***array, int height, int width, int range, int th);

int main(int argc, char** argv) 
{
	//error checking command line args
	if (argc != 4)
	{
		printf("Error: please enter 3 command line arguments %i\n", argc);
	}
	int HEIGHT = 2;// atoi(argv[2]);
	int WIDTH = 2;// atoi(argv[3]);
	int RANGE = 3;
	int image[4][4] = {{0, 0, 0, 0},
		    {0, 0, 1, 0},
		    {0, 1, 0, 1},
		    {0, 0, 1, 0}};

	printf("Hello world\n");

	int ***data;

	// Allocate memory
	data = new int**[HEIGHT];  // layer 1
	for (int i = 0; i < HEIGHT; ++i) 
	{  // layer 2
		data[i] = new int*[WIDTH];
		for (int j = 0; j < WIDTH; ++j)  // layer 3
		{
			data[i][j] = new int[RANGE];
		}
	}
		
	initialize(data, HEIGHT, WIDTH, RANGE);

	printM(data, HEIGHT, WIDTH, RANGE);
	printf("DONE PRINTING\n");
	threshold(data, HEIGHT, WIDTH, RANGE, 0);


	// De-Allocate memory to prevent memory leak
	for (int i = 0; i < HEIGHT; ++i) 
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			delete [] data[i][j];
		}
		delete [] data[i];
	}
	delete [] data;
	printf("DEALLOCATED\n");

	return 0;
}


void initialize(int ***array, int height, int width, int range)
{
	cout << "beginning of init" << endl;

	for (int i = 0; i < height; i++) //go through every row
	{
		for (int j = 0; j < width; j++) //go through every column
		{
			for (int k = 0; k < range; k++) //go through every radius
			{
				array[i][j][k] = 0;
			}
		}
	}

	cout << "end of init" << endl;
}

void printM(int ***data, int h, int w, int r) 
{
	for(int i = 0; i < h; i++) 
	{
		for(int j = 0; j < w; j++) 
		{
			for(int k = 0; k < r; k++) 
			{
				printf("[%d][%d][%d] = %d | ", i, j, k, data[i][j][k]);
			}
			printf("\n");
		}
		printf("\n");
	}
}

void voting(int ***array, int height, int width, int range, int **image)
{
	int a, b;
	int maxDegree = 360;
	float PI = 3.1415926535897;

	for (int y = 0; y < height; y++) //go through every row
	{
		for (int x = 0; x < width; x++) //go through every column
		{
			if (image[x][y] == 1)
			{
				for (int radius = 0; radius < range; radius++) //go through every radius
				{
					// NOTE RADIUS MUST BE OFFSET IN CALCULATIONS
					// NOTE INDEX CANT BE OUT OF BOUNDS
					// NOTE RADIUS CANT BE 0
					// NOTE CANT VOTE TWICE FOR SAME PIXEL
					for (int d = 0; d < maxDegree; d++)
					{
						a = x - radius * cos(d * PI / 180);
						b = y - radius * sin(d *PI / 180);
						array[a][b][radius]++;
					}
				}
			}
		}
	}
}

void threshold(int ***array, int height, int width, int range, int th)
{
	ofstream myfile;
    myfile.open ("circles.csv");
    
	for(int i = 0; i < height; i++) 
	{
		for(int j = 0; j < width; j++) 
		{
			for(int k = 0; k < range; k++) 
			{
				// TO DO : GET RID OF LAST LINE
				if(array[i][j][k] >= th)
				{
					myfile << i << "," << j << "," << k << "\n";
				}
			}
		}
	}
	myfile.close();
}
