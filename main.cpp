#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void initialize(int ***array, int height, int width, int range);

int main(int argc, char** argv) 
{
	int HEIGHT = 256;
	int WIDTH = 256;
	int RANGE = 30;
	
	initialize(data, HEIGHT, WIDTH, RANGE);
	
	//print x,y,r\n to file .csv

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
