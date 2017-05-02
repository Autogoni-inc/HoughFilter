#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Ben Hammel, Emmali Hanson, Kevin Wright, Gleb Sklyr

/* 
	last modified: 04/25/2017 (MM/DD/YYYY) at 22:52 CST
    description: an implementation of the Hough ("huff") transform to 
    find circles in a binary image that was already processed to
    contain edges.  the program writes the coordinates and radius
    of the detected circle into a csv file based on a specified 
    threshold.  for each threshold applied, a new file is created.
*/

using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void printM(int ***data, int h, int w, int r);
void initialize(int ***array, int height, int width, int range);
void voting2(int ***array, int height, int width, int range, int image[12][13]);
void voting(int ***array, int height, int width, int minRad, int range, int **image);
void threshold(int ***array, int height, int width, int minRad, int range, int th);
void th_range(int start, int end, int ***array, int height, int width, int minRad, int range);
void readImage(char* filename, int height, int width, int **image);
void readImage2(int *, char *, int, int);
void bestCircle(int ***array, int height, int width, int minRad, int range);


int main(int argc, char** argv) 
{
	//error checking command line args
	if (argc != 6)
	{
		printf("Error: please enter 5 command line arguments %i\n", argc);
		exit(EXIT_FAILURE);
	}
//	int HEIGHT = 12;// atoi(argv[2]);
//	int WIDTH = 13;// atoi(argv[3]);
//	int RANGE = 2;
//	int minRad = 30;

//	int H = 90;
//	int W = 100;
	
	char* filename = argv[1];
	int H = atoi(argv[2]);
	int W = atoi(argv[3]);
	int minRad = atoi(argv[4]);
	int maxRad = atoi(argv[5]);
	int RANGE = maxRad - minRad;

	// Test image
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

//	char filename[] = "test.txt"; //argv[4];
	
	// voting array
	int ***data;
	// Allocate memory
	data = new int**[H];  // layer 1
	for (int i = 0; i < H; ++i) 
	{  // layer 2
		data[i] = new int*[W];
		for (int j = 0; j < W; ++j)  // layer 3
		{
			data[i][j] = new int[RANGE];
		}
	}

	// MUST ADD MEMORY DEALLOCATION BEFORE UNCOMMENTING THIS

	int *binData = new int[W*H];
	readImage2(binData, filename, W, H);

	int **binImage;
	binImage = new int*[H];  // layer 
	for (int i = 0; i < H; ++i) 
	{  // layer 2
		binImage[i] = new int[W];
	}

	int count = 0;
	for(int i = 0; i < H; i++)
	{
		for(int j = 0; j < W; j++)
		{
			if(binData[count] != 0)
			{
				binData[count] = 1;
			}
			binImage[i][j] = binData[count];
			count++;
		}
	}

	// Hough
	initialize(data, H, W, RANGE); // zero voting array
	// readImage(filename, HEIGHT, WIDTH, image2); // read image from file

	//voting2(data, HEIGHT, WIDTH, RANGE, image); // vote
         voting(data, H, W, minRad, RANGE, binImage);

	// printM(data, HEIGHT, WIDTH, RANGE); // optional debug printout
//	th_range(1, 9, data, HEIGHT, WIDTH, RANGE); // filter out the high scoring circles into files
	//th_range(1, 1000, data, H, W, minRad, RANGE);
	bestCircle(data, H, W, minRad, RANGE);

	// De-Allocate memory to prevent memory leak
	for (int i = 0; i < H; ++i) 
	{
		for (int j = 0; j < W; ++j)
		{
			delete [] data[i][j];
		}
		delete [] data[i];
	}
	delete [] data;


	for(int i = 0; i < H; ++i)
	{
		delete [] binImage[i];
	}
	delete [] binImage;

	return 0;
}

// zero out a 3D array
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

// print 3D array to console for visualization
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

// Hough voting implementation
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

	for (int y = 0; y < height; y++) //go through every row
	{
		for (int x = 0; x < width; x++) //go through every column
		{
			if (image[x][y] == 1) // if it is a possible circle edge
			{
				for (int radius = 0; radius < range; radius++) //go through every radius
				{
					initialize(lookup, height, width, range); // each time zero out the lookup table
					for (int d = 0; d < maxDegree; d++)
					{	
						// possible circ/thle center coordinates
						a = x - (radius + MINRAD) * cos(d * PI / 180);
						b = y - (radius + MINRAD) * sin(d *PI / 180);

						// do not vote if the circle is out of bounds
						if((a + (radius+1)) >= width || (a - (radius+1)) < 0 || (b + (radius+1)) >= height || (b - (radius+1)) < 0) { continue; }
						if(a < height && a > 0 && b < width && b > 0) // point noot out of bounds
						{
							if(lookup[a][b][radius] != 0) { continue; } // do not vote twice per rafius
							array[a][b][radius]++; // vote!
							lookup[a][b][radius]++; // mark as already voted for this radius
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

void bestCircle(int ***array, int height, int width, int minRad, int range)
{
	ofstream myfile;
	char str[80];
	strcpy(str, "out.csv");
	myfile.open(str);
	int actual_radius = 0;
	int bestH = 0;
	int bestW = 0;
	int bestRange = 0;
	int bestScore = 0;

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			for(int k = 0; k < range; k++)
			{
				if(array[i][j][k] >= bestScore)
				{
					bestH = i;
					bestW = j;
					bestRange = k;
					bestScore = array[i][j][k];
				}				
			}	
		}
	}
	
	actual_radius = bestRange + minRad;
	myfile << bestH << "," << bestW << "," << actual_radius << std::flush;

	myfile.close();

}

// use a threshold to print legible circles from voting array to a csv file
void threshold(int ***array, int height, int width, int minRad, int range, int th)
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
					actual_radius = k + minRad; // because starts at 0
					myfile << i << "," << j << "," << actual_radius << "\n" << std::flush;
				}
			}
		}
	}
	myfile.close();
}

// use the threshold() function for a range of values
void th_range(int start, int end, int ***array, int height, int width, int minRad, int range)
{
	for(int i = start; i <= end; i++) 
	{
		threshold(array, height, width, minRad, range, i);
	}
}

// read an image into an array
void readImage(char* filename, int height, int width, int **image)
{
	ifstream myfile;
	myfile.open(filename);
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
}

void readImage2(int *data, char * filename, int numRows, int numCols)
{
	ifstream infile;
	infile.open(filename, ios::in|ios::binary);
	if(infile.is_open())
	{	
		infile.read((char *)data, numRows*numCols*4);
		infile.close();
	}
	else
	{
		cout << "Unable to open" << filename << " for reading\r\n";
	}

}









/* CONSTRUCTION ZONE KEEP OUT */


// copy of voting method
void voting(int ***array, int width, int height, int minRad, int range, int **image)
{
	int a, b;
	int maxDegree = 360;
	int MINRAD = minRad;
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
						if((a + (radius+1)) >= width || (a - (radius+1)) < 0 || (b + (radius + 1)) >= height || (b - (radius+1)) < 0) { continue; } 
						if(a < height && a > 0 && b < width && b > 0) // point noot out of bounds
						{
						//	cout << "a " << a << " b " << b << " radius " << radius << "\r\n";
							if(lookup[a][b][radius] != 0) { /*printf("\nduplicate vote\n")*/;continue; } // do not vote twice per rafius
						//	cout << "voted\r\n";
							//printf("\nvote start\n");
							array[a][b][radius]++;
							//cout << "\nVOTED\n";
							lookup[a][b][radius]++;
							//printf("\nvote end\n")
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
