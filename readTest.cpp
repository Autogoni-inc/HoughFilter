#include <iostream>
#include <fstream>

using namespace std;

void readImage(char *, int, int);

#define WIDTH 256
#define HEIGHT 256

float *data;
float array[WIDTH][HEIGHT];

int main()
{

	cout << "Hello World!";

	data = new float[WIDTH*HEIGHT];	
	readImage("lab5_spatial_image.bin", WIDTH, HEIGHT);

	int count = 0;
	for(int i = 0; i < HEIGHT; i++)
	{
		for(int j = 0; j < WIDTH; j++)
		{
			array[i][j] = data[count];
			count++;
			cout << array[i][j] << "\r\n";
		}			
	}

	
}	


void readImage(char * filename, int numRows, int numCols)
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
