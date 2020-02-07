
#include <cmath>
#include <iostream>
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

int main()
{
	return 0;
}
