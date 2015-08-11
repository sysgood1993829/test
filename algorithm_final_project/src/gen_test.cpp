#include<iostream>
#include<fstream>
#include <stdlib.h> 
using namespace std;
int main(int argc, char **argv)
{
	if(argc == 4)
	{
		int i_max=atoi(argv[2]);
		int j_max=atoi(argv[3]);
		fstream fout;
		fout.open(argv[1],ios::out);
	    fout <<"ALPHA=20"<<endl;
		fout <<"BETA=30"<<endl;
		fout <<"OMEGA=1000"<<endl;
		for(int i=0;i<i_max;i++)
		{
			for(int j=0;j<j_max;j++)
			{
				fout<<j*120<<","<<i*130<<","<<j*120+100<<","<<i*130+100<<endl;
			}
		}
		fout.close();
	}
	else
	{
		cout<<"please enter a file name,y_max,x_max"<<endl;
	}
}
