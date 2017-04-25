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
void voting2(int ***array, int height, int width, int range, int image[12][13]);
void voting(int ***array, int height, int width, int range, int **image);
void threshold(int ***array, int height, int width, int range, int th);
void th_range(int start, int end, int ***array, int height, int width, int range);
void readImage(char* filename, int height, int width, int **image);

int main(int argc, char** argv) 
{
	//error checking command line args
	if (argc != 4)
	{
		printf("Error: please enter 3 command line arguments %i\n", argc);
	}
	int HEIGHT = 12;// atoi(argv[2]);
	int WIDTH = 13;// atoi(argv[3]);
	int RANGE = 3;

	int image[12][13] = {   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	    		   			{0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
	               			{0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
	               			{0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
				   			{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0},
				   			{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
							{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0},
	    		   			{0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
	               			{0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
	               			{0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
				   			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				   			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

	printf("Hello world\n");

	char filename[] = "test.txt"; //argv[4];

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

	int **image2;
	image2 = new int*[HEIGHT];  // layer 1
	for (int i = 0; i < HEIGHT; ++i) 
	{  // layer 2
		image2[i] = new int[WIDTH];
	}

	initialize(data, HEIGHT, WIDTH, RANGE);

	readImage(filename, HEIGHT, WIDTH, image2);

	voting2(data, HEIGHT, WIDTH, RANGE, image);  

	printM(data, HEIGHT, WIDTH, RANGE);
	printf("DONE PRINTING\n");
	th_range(4,9,data, HEIGHT, WIDTH, RANGE);


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

void voting2(int ***array, int height, int width, int range, int image[12][13])
{
	int a, b;
	int maxDegree = 360;
	int MINRAD = 1;
	float PI = 3.1415926535897;
	// lookup table for duplicate votes
	int ***lookup;
	lookup = new int**[height];  // layer 1
	for (int i = 0; i < height; ++i) 
	{  // layer 2
		lookup[i] = new int*[width];
		for (int j = 0; j < width; ++j)  // layer 3
		{
			lookup[i][j] = new int[range];
		}
	}
	
	//printf("Done initializing params");

	for (int y = 0; y < height; y++) //go through every row
	{
		//printf("Int loop1");
		for (int x = 0; x < width; x++) //go through every column
		{
			//printf("Int loop2");
			if (image[x][y] == 1) // if it is a possible circle edge
			{
				//printf("found 1 pixel");
				for (int radius = 0; radius < range; radius++) //go through every radius
				{
					//printf("in loop3");
					initialize(lookup, height, width, range);
					for (int d = 0; d < maxDegree; d++)
					{	
						//printf("in loop 4");
						a = x - (radius + MINRAD) * cos(d * PI / 180);
						b = y - (radius + MINRAD) * sin(d *PI / 180);
						//printf("\nsin and COS works!\n");
						
						if(a < height && a > 0 && b < width && b > 0) // point noot out of bounds
						{
							if(lookup[a][b][radius] != 0) { /*printf("\nduplicate vote\n")*/;continue; } // do not vote twice per rafius
							//printf("\nvote start\n");
							array[a][b][radius]++;
							lookup[a][b][radius]++;
							//printf("\nvote end\n");
						}
						//printf("\ngot to end of loop 4\n");
					}
				}
			}
		}
	}
	// De-Allocate memory to prevent memory leak
	for(int i = 0; i < height; ++i) 
	{
		for(int j = 0; j < width; ++j)
		{
			delete [] lookup[i][j];
		}
		delete [] lookup[i];
	}
	delete [] lookup;
}

void threshold(int ***array, int height, int width, int range, int th)
{
	ofstream myfile;
	char str[80];
	char buffer[200];
	sprintf(buffer, "%d", th);	
  	strcpy (str,"th_");
  	strcat (str, buffer);
  	strcat (str,".csv");
    myfile.open (str);
    int actual_radius = 0;
    
	for(int i = 0; i < height; i++) 
	{
		for(int j = 0; j < width; j++) 
		{
			for(int k = 0; k < range; k++) 
			{
				// TO DO : GET RID OF LAST LINE
				if(array[i][j][k] >= th)
				{
					actual_radius = k + 1; // because starts at 0
					myfile << i << "," << j << "," << actual_radius << "\n" << std::flush;
				}
			}
		}
	}
	myfile.close();
}

void th_range(int start, int end, int ***array, int height, int width, int range)
{
	for(int i = start; i <= end; i++) 
	{
		threshold(array, height, width, range, i);
	}
	myfile.close();
}

void readImage(char* filename, int height, int width, int **image)
{
	ifstream myfile;
	cout << "Reading image" << endl;
	myfile.open(filename);
	cout << "read file done";
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			myfile >> image[i][j];
			if(image[i][j] != 0) { image[i][j] = 1; }
			cout << image[i][j] << " - read: " << i << "," << j << "\n";
		}
	}
	myfile.close();
	cout << "done";
}













void voting(int ***array, int width, int height, int range, int **image)
{
	int a, b;
	int maxDegree = 360;
	int MINRAD = 1;
	float PI = 3.1415926535897;
	// lookup table for duplicate votes
	int ***lookup;
	lookup = new int**[height];  // layer 1
	for (int i = 0; i < height; ++i) 
	{  // layer 2
		lookup[i] = new int*[width];
		for (int j = 0; j < width; ++j)  // layer 3
		{
			lookup[i][j] = new int[range];
		}
	}
	//cout << "voting";
	
	//printf("Done initializing params");

	for (int y = 0; y < height; y++) //go through every row
	{
		printf("Int loop1");
		for (int x = 0; x < width; x++) //go through every column
		{
			//printf("Int loop2");
			if (image[x][y] == 1) // if it is a possible circle edge
			{
				//printf("found 1 pixel");
				for (int radius = 0; radius < range; radius++) //go through every radius
				{
					//printf("in loop3");
					initialize(lookup, height, width, range);
					for (int d = 0; d < maxDegree; d++)
					{	
						//printf("in loop 4");
						a = x - (radius + MINRAD) * cos(d * PI / 180);
						b = y - (radius + MINRAD) * sin(d *PI / 180);
						//printf("\nsin and COS works!\n");
						//cout << "\nvoting inner loop\n";
						if(a < height && a > 0 && b < width && b > 0) // point noot out of bounds
						{
							if(lookup[a][b][radius] != 0) { /*printf("\nduplicate vote\n")*/;continue; } // do not vote twice per rafius
							//printf("\nvote start\n");
							array[a][b][radius]++;
							//cout << "\nVOTED\n";
							lookup[a][b][radius]++;
							//printf("\nvote end\n");
						}
						//printf("\n--------------------------------------------------got to end of loop 4\n");
					}
				}
			}
		}
	}
	// De-Allocate memory to prevent memory leak
	for(int i = 0; i < height; ++i) 
	{
		for(int j = 0; j < width; ++j)
		{
			delete [] lookup[i][j];
		}
		delete [] lookup[i];
	}
	delete [] lookup;
}
