//============================================================================
// Name        : littleTri.cpp
// Author      : eric_hsiao
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cmath>
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
	cout<< "Do you want to compute its area?? enter(y/n)"<<endl;
	char want;
	double answer;
	double s = ((a+b)+c)/2;
	answer = sqrt(s*(s-a)*(s-b)*(s-c));
	cin>>want;
	if(want == 'y')
		cout<<"Its area is:"<<answer<<endl;
	else if(want == 'n')
		cout<<"Bye~~~"<<endl;
	else
		cout<<"y or n only!!!!"<<endl;
	return 0;
}
