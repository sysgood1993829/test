//============================================================================
// Name        : littleTri.cpp
// Author      : eric_hsiao
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	cout<< "please input 3 integers:"<<endl;
	int a,b,c;
	cin>>a>>b>>c;
	if(a+b>c && a+c>b && b+c>a )
		cout<<"It is a legistimate triangle.^___^"<<endl;
	else
		cout<<"It is not a triangle.Q___Q"<<endl;
	return 0;
}
