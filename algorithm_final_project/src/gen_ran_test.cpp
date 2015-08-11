#include<iostream>
#include<fstream>
#include<stdlib.h> 
#include<vector>
using namespace std;
int main(int argc, char **argv)
{
	
	srand(time(NULL));
	if(argc == 5)
	{
		int X_max=atoi(argv[2]);
		int Y_max=atoi(argv[3]);
		int chip_num = atoi(argv[4]);
		vector<vector< int > > cor;
		//cor.resize(chip_num);
		fstream fout;
		fout.open(argv[1],ios::out);
	    fout <<"ALPHA="<<rand()%100+20<<endl;
		fout <<"BETA="<<rand()%100+30<<endl;
		fout <<"OMEGA="<<rand()%1000+100<<endl;
		for(int i=0;i<chip_num;i++)
		{
			//fout<<j*120<<","<<i*130<<","<<j*120+100<<","<<i*130+100<<endl;
			vector<int> now;
			bool find = false;
			now.push_back(rand()%X_max);
			now.push_back(rand()%Y_max);
			//cout<<now[0]<<" "<<now[1]<<endl;
			now.push_back(rand()%100+1+now[0]);
			now.push_back(rand()%100+1+now[1]);
			for(int j=0;j<cor.size();j++)
			{
				if((cor[j][0]<=now[0] && now[0]<=cor[j][2] && cor[j][1]<=now[1] && now[1]<=cor[j][3])||
					(cor[j][0]<=now[2] && now[2]<=cor[j][2] && cor[j][1]<=now[3] && now[3]<=cor[j][3]) )
					find = true;
			}
			if(!find)
			{
				cor.push_back(now);
				fout<<now[0]<<","<<now[1]<<","<<now[2]<<","<<now[3]<<endl;
			}
		}
		fout.close();
	}
	else
	{
		cout<<"please enter a file name,Xcor_max,Ycor_max,chip_num(iteration num)"<<endl;
	}
}
