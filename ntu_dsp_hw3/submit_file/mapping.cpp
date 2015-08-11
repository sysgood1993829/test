#include<iostream>
#include <cstring>
#include <fstream>
#include <vector>
using namespace std;

int main(int argc, char* argv[])
{
	 if(argc != 3) {
       cout<<"Error! Need two argument. Input file and output file's name."<<endl;
       return 0;
    }
//////////// read the input file /////////////
	char buffer[200];
    fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2],ios::out);
    string seq;
    vector< vector<string> > data;
	data.resize(37);
	vector<string> map;
	int index=0;
    while (fin >> seq)
    {
		
		string word = seq;
		fin >> seq;
		string initial;
		initial.push_back(seq[0]);
		initial.push_back(seq[1]);
		bool find=false;		
		for(int i=0;i<map.size();i++)
		{
			if(initial == map[i])
			{	
				data[i].push_back(word);
				find = true;
			}
		}
		if(!find)
		{
			map.push_back(initial);
			data[map.size()-1].push_back(word);
		}
		
		int pos = 2;
		for(int pos = 2;pos<seq.length();)
		{
			initial.clear();
			if(seq[pos] == '/')
			{
				initial.push_back(seq[pos+1]);
				initial.push_back(seq[pos+2]);
				bool find=false;		
				for(int i=0;i<map.size();i++)
				{
					if(initial == map[i])
					{	
						bool find2 = false;
						for(int k=0;k<data[i].size();k++)
						{
							if(data[i][k]==word)
								find2 = true;
						}
						if(!find2)
							data[i].push_back(word);
						find = true;
					}
				}
				if(!find)
				{
					map.push_back(initial);
					data[map.size()-1].push_back(word);
				}
				pos = pos +2;
			}
			else
				pos++;
		}
		/*if(index<10)
		{	
			cout<<word<<endl;
			cout<<seq<<endl;
		}
		index++;*/	
	}
	//debug 
	//for(int i=0;i<data[0].size();i++)
	//	cout<<data[0][i]<<endl;


	//Sort part
	/*for(int i=0;i<map.size();i++)
	{
		for(int j=0;j<map.size()-i;j++)
		{
			if(map[j]>map[j+1])
			{
				data[j].swap(data[j+1]);
				string temp = map[j];
				map[j] = map[j+1];
				map[j+1] = temp;
			}
		}
	}*/

	//////////// write the output file ///////////
	vector<string> sortmap;
	int min = 0;
	for(int i=0;i<map.size();i++)
		sortmap.push_back(map[i]);
	
	for(int i=0;i<sortmap.size();i++)
	{
		for(int j=0;j<sortmap.size()-i-1;j++)
		{
			if(sortmap[j]>sortmap[j+1])
			{
				string temp = sortmap[j];
				sortmap[j] = sortmap[j+1];
				sortmap[j+1] = temp;
			}
		}	
	}
	for(int i=0;i<sortmap.size();i++)
	{    
		int id = 0;
		for(int k=0;k<map.size();k++)
		{
			if(map[k]==sortmap[i])
				id = k;
		}//cout<<"gg"<<endl;
		fout<<sortmap[i]<<'\t';
		for(int j=0;j<data[id].size();j++)
		{
			if(j<data[id].size()-1)		
				fout<<data[id][j]<<" ";
			else
				fout<<data[id][j]<<endl;
		}
		for(int j=0;j<data[id].size();j++)
		{
			fout<<data[id][j]<<'\t'<<data[id][j]<<endl;
		}
		//cout<<i<<endl;
	}
    fin.close();
    fout.close();



return 0;
}
