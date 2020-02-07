
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


int computeInverseMod(int);
int computeInverseMod(int,int,int);

int computeInverseMod(int encryptedChar)
{
	return computeInverseMod(encryptedChar, 23, 77);	//d = 23, n = 77
}
int computeInverseMod(int encryptedChar, int d, int n)
{
	int cd = pow(encryptedChar, d);
	int m = cd % n;
	return m;
}
string readInput(std::string fileName)
{
	std::stringstream inputStream;
	fstream myFile;
	myFile.open(fileName);
	if (!myFile.fail())
	{
		myFile.read();
		std::cout << "The number of characters extracted is: " << myFile.gcount() << "\n";
		int count = 0;
		char curChar = ' ';
		while (curChar << myFile)
		{
			std::cout << count << ": " << curChar << "\n";
		}
		std::cout << "End reading file\n";
	}
}
	

int main()
{
	std::string fileName = "example.txt";
	readInput(fileName);
	return 0;
}
