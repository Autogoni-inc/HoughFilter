#include <iostream>
#include <fstream>

using namespace std;

void readImage(char *, int, int);

float *data;
float array[256][256];

int main()
{

	cout << "Hello World!";

	data = new float[256*256];	
	readImage("lab5_spatial_image.bin", 256, 256);

	int count = 0;
	for(int i = 0; i < 256; i++)
	{
		for(int j = 0; j < 256; j++)
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
