
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
using namespace std;

struct EncryptedChar{
	int m = 0; //decrypted
	int c = 0;	//encrypted
	char a = ' '; //m-> character representation
//	EncryptedChar(int decrypted,int encrypted,char display)
//	{
//		m = decrypted;
//		c = encrypted;
//		a = display;
//	}
//	EncyptedChar()
//	{ m = 0; c = 0; a = ' ';}
};


int decryptC(int);
int decryptC(int,int,int);

int decryptC(int encryptedChar)
{
	return decryptC(encryptedChar, 23, 77);	//d = 23, n = 77
}
int decryptC(int encryptedChar, int d, int n)
{
	int cd = pow(encryptedChar, d);
	int m = cd % n;
	return m;
}
std::vector<EncryptedChar>  readInput(std::string fileName)
{
	std::vector<EncryptedChar> listOfChars;
	std::stringstream inputStream;
	ifstream myFile;
	myFile.open(fileName);
	if (!myFile.fail())
	{
		inputStream << myFile.rdbuf();
		//std::cout << "The number of characters extracted is: " << myFile.gcount() << "\n";
		int count = 0;
		std::string allNums = inputStream.str();
		char * allNums_dup = strdup(allNums.c_str());
		std::string bufferStr = "";
		int bufferInt = 0;
		char curChar = ' ';
		char * bufferCharPtr;
		EncryptedChar ecStruct;



		//first loop iteration
	//	bufferCharPtr = strtok(allNums_dup,"\0");
	//	bufferInt = atoi(bufferCharPtr);
	//	ecStruct.c = bufferInt;
	//	listOfChars.push_back(ecStruct);
	//	std::cout << ": " << bufferInt << "\n";
	//	count ++;
	//	do
	//	{
	//		bufferCharPtr = strtok(NULL, "\0 ");
	//		bufferInt = atoi(bufferCharPtr);
	//		ecStruct.c = bufferInt;
	//		listOfChars.push_back(ecStruct);
	//		std::cout << count << ": " << bufferInt << "\n";
	//		count ++;
	//	}while(bufferCharPtr != NULL);		
	//	free(allNums_dup);

	//	std::cout << "The read file in string form is; " << allNums << "\n";
		//EncryptedChar ecStruct;
		while(std::getline(inputStream,bufferStr, ' '))
		{
		//	std::cout << "Str: " << bufferStr << " || ";
			bufferInt = std::stoi(bufferStr);
			ecStruct.c = bufferInt;
			listOfChars.push_back(ecStruct);
			std::cout << count << ": " << bufferInt << "\n";
			count ++;
		}
		//while (inputStream >> curChar)
		//{
		//	ecStruct.c = curChar; 
		//	listOfChars.push_back(ecStruct);
		//	std::cout << count << ": " << curChar << "\n";
		//	count ++;
		//}
		std::cout << "End reading file\n";
	}

	myFile.close();
	return listOfChars;
}
	

int main()
{
	std::string fileName = "example.txt";
	std::vector<EncryptedChar> listOfChars = readInput(fileName);
	return 0;
}
