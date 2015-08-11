#include<stdio.h>
#include "Ngram.h"
#include<vector>
#include<fstream>
#include<cstring>
using namespace std;

double getBigramProb(const char *w1, const char *w2 , Ngram& lm,Vocab& voc)
{
    VocabIndex wid1 = voc.getIndex(w1);
    VocabIndex wid2 = voc.getIndex(w2);

    if(wid1 == Vocab_None)  //OOV
        wid1 = voc.getIndex(Vocab_Unknown);
    if(wid2 == Vocab_None)  //OOV
        wid2 = voc.getIndex(Vocab_Unknown);

    VocabIndex context[] = { wid1, Vocab_None };
    return lm.wordProb( wid2, context);
}
// Get P(W3 | W1, W2) -- trigram
double getTrigramProb(const char *w1, const char *w2, const char *w3, Ngram& lm,Vocab& voc) 
{
    VocabIndex wid1 = voc.getIndex(w1);
    VocabIndex wid2 = voc.getIndex(w2);
    VocabIndex wid3 = voc.getIndex(w3);

    if(wid1 == Vocab_None)  //OOV
        wid1 = voc.getIndex(Vocab_Unknown);
    if(wid2 == Vocab_None)  //OOV
        wid2 = voc.getIndex(Vocab_Unknown);
    if(wid3 == Vocab_None)  //OOV
        wid3 = voc.getIndex(Vocab_Unknown);

    VocabIndex context[] = { wid2, wid1, Vocab_None };
    return lm.wordProb( wid3, context );
}
int mapfind(const string str,vector<string> map)
{
	for(int i=0;i<map.size();i++)
	{
		if(str == map[i])
			return i;
	}
	return -1;
}

int main(int argc, char* argv[])
{
// ./disambig �ext $file �ap $map �m $LM �rder $order
	if(argc!=9)
	{
		cout<<"You need the arguments ./disambig �ext $file �ap $map �m $LM �rder $order"<<endl;
		return 0;
	}

	// loading language model
	Vocab voc;
	Ngram lm(voc,2);
	{
		//const char lm_filename[] = argv[2];
		File lmFile(argv[6],"r");
		lm.read(lmFile);
		lmFile.close();
	}

	// construct map
	vector<string> map;
	fstream fin(argv[4]);
	string initial;
	fin >> initial;
	map.push_back(initial);
	vector< vector<string> > data;
	data.resize(37);
	//map.resize(37);
	int a[37] = {0xA374,0xA375,0xA376,0xA377,0xA378,0xA379,0xA37A,0xA37B,0xA37C,0xA37D,
				 0xA37E,0xA3A1,0xA3A2,0xA3A3,0xA3A4,0xA3A5,0xA3A6,0xA3A7,0xA3A8,0xA3A9,
				 0xA3AA,0xA3B8,0xA3B9,0xA3BA,0xA3AB,0xA3AC,0xA3AD,0xA3AE,0xA3AF,0xA3B0,
				 0xA3B1,0xA3B2,0xA3B3,0xA3B4,0xA3B5,0xA3B6,0xA3B7};
	for(int i=1;i<37;i++)
	{
		string temp;
		temp.push_back(char(a[i]/256));
		temp.push_back(char(a[i]%256));
		map.push_back(temp);
	}
	//for(int i=0;i<37;i++)
	//	cout<<map[i]<<endl;

	//construct data mapping
	int index = 0;
	bool renew = false;
	bool find = false;
	while(fin >> initial)
	{
		for(int i =0;i<map.size();i++)
		{
			if(initial == map[i])
			{
				index = i;
				renew = true;
				//cout<<"gg"<<endl;
				//cout<<initial<<endl;
				break;
			}
		}
		if(renew)
		{
			renew = false;
			continue;
		}
		else
		{
			for(int i =0;i<data[index].size();i++)
			{
				if(initial == data[index][i])
				{
					find = true;
					break;
				}
			}
			if(!find)
			{
				data[index].push_back(initial);
			}
			else
				find = false;
		}
	}
	// erase the word which is not in the corpse can enhance speed
	
	for(int i=0;i<data.size();i++)
	{
		for(int j=0;j<data[i].size();j++)
		{	
			VocabIndex wid = voc.getIndex(data[i][j].c_str());
    		if(wid == Vocab_None) {
        		data[i].erase(data[i].begin()+j);
				j--;
    		}
		}
	}
	



//        input part disambig !!!!
	
	fstream fin2(argv[2]);
	string mystr;
	string s = argv[8];
	if(s=="2")//bigram part
	{
		while(getline(fin2,mystr))
		{
			//define variable here
			vector<string> dis;				//parse the word
			vector< vector<double> > prob;	//calculate the max probability.
			vector< vector<string> > vmap;	//record the word map
			vector< vector<int> > trace;	//record the previous member with max prob.
			int str_length = 0;				//the string length
			int space_num = 0;				//the space number
			for(int k = 0;k<mystr.length();k++)
			{
				if(mystr[k]==' ')
					space_num++;
			}
			str_length = (mystr.length()-space_num)/2;
			dis.resize(str_length);
			prob.resize(str_length);
			vmap.resize(str_length);
			trace.resize(str_length);
			// the following is the main parse part
			int n_index = 0;
			int even = 0;
			for(int k=0;k<mystr.length();k++)
			{
				if(mystr[k]!=' ')
				{
					if(even == 0)
					{
						dis[n_index].push_back(mystr[k]);
						even = 1;
					}
					else
					{
						dis[n_index].push_back(mystr[k]);
						even = 0;
						n_index++;
					}
				}
				else
					continue;
			}
			//for(int i=0;i<dis.size();i++)
			//	cout<<dis[i]<<endl;

			//determine the max prob. and check the trace
			int bpmidx = 0;
			for(int i=0;i<dis.size();i++)
			{	
				bpmidx = mapfind(dis[i],map);
				if(bpmidx == -1)
				{
					if(i == 0)
					{		
						vmap[i].push_back(dis[i]);
						trace[i].push_back(0);
						prob[i].push_back(0);
					}
					else
					{
						if(vmap[i-1].size()>1)
						{
							double maxprob =-99999;
							int where = 0;
							for(int kk=0;kk<prob[i-1].size();kk++)
							{
								if(prob[i-1][kk]+getBigramProb(vmap[i-1][kk].c_str(),dis[i].c_str(),lm,voc)>maxprob)
								{
									maxprob = prob[i-1][kk]+getBigramProb(vmap[i-1][kk].c_str(),dis[i].c_str(),lm,voc);
									where = kk;
								}	

							}
							vmap[i].push_back(dis[i]);
							trace[i].push_back(where);
							prob[i].push_back(maxprob);

						}
						else
						{
							trace[i].push_back(0);
							prob[i].push_back(prob[i-1][0]);
							vmap[i].push_back(dis[i]);
						}
					}
				}
				else
				{
					if(i == 0)
					{
						for(int j=0;j<data[bpmidx].size();j++)
						{
							VocabIndex wid1 = voc.getIndex(data[bpmidx][j].c_str());
							if(wid1 == Vocab_None)  //OOV
								wid1 = voc.getIndex(Vocab_Unknown);

							VocabIndex context[] = {  Vocab_None };
							vmap[i].push_back(data[bpmidx][j]);
							trace[i].push_back(0);
							prob[i].push_back(lm.wordProb( wid1,context));
						}
					}
					else
					{
						if(vmap[i-1].size()>1)
						{
							for(int jj=0;jj<data[bpmidx].size();jj++)
							{
								double maxprob =-99999;
								int where = 0;
								for(int kk=0;kk<prob[i-1].size();kk++)
								{
									if(prob[i-1][kk]+getBigramProb(vmap[i-1][kk].c_str(),data[bpmidx][jj].c_str(),lm,voc)>maxprob)
									{
										maxprob = prob[i-1][kk]+getBigramProb(vmap[i-1][kk].c_str(),data[bpmidx][jj].c_str(),lm,voc);
										where = kk;
									}	

								}
								vmap[i].push_back(data[bpmidx][jj]);
								trace[i].push_back(where);
								prob[i].push_back(maxprob);
							}

						}
						else
						{
							for(int ii=0;ii<data[bpmidx].size();ii++)
							{
								trace[i].push_back(0);
								prob[i].push_back(getBigramProb(vmap[i-1][0].c_str(),data[bpmidx][ii].c_str(),lm,voc));
								vmap[i].push_back(data[bpmidx][ii]);
							}
						}
					}
				}
			}
			// find the true trace here
			int last = 0;
			double maxpp=-99999;
			if(trace[str_length-1].size()>1)
			{
				for(int j=0;j<prob[str_length-1].size();j++)
				{
					if(prob[str_length-1][j]>maxpp)
					{	
						maxpp = prob[str_length-1][j];
						last = j;
					}
				}
			}
			vector<string> rightstr;
			rightstr.resize(str_length);
			for(int i = dis.size()-1;i>-1;i--)
			{
				rightstr[i] = vmap[i][last];
				last = trace[i][last];
			}
			//output the file or display on the screen
			cout<<"<s> ";
			for(int i=0;i<rightstr.size();i++)
				cout<<rightstr[i]<<" ";
			cout<<"</s>"<<endl;
		}
	}
	else if(s == "3")//trigram part
	{
		while(getline(fin2,mystr))
		{
			//define variable here
			vector<string> dis;//parse the word
			vector< vector<double> > prob;//calculate the max probability.
			vector< vector<string> > vmap;//record the word map
			vector< vector<int> > trace;//record the previous member with max prob.
			int str_length = 0;//the string length
			int space_num = 0;//the space number
			for(int k = 0;k<mystr.length();k++)
			{
				if(mystr[k]==' ')
					space_num++;
			}
			str_length = (mystr.length()-space_num)/2;
			dis.resize(str_length);
			prob.resize(str_length);
			vmap.resize(str_length);
			trace.resize(str_length);
			// the following is the main parse part
			int n_index = 0;
			int even = 0;
			for(int k=0;k<mystr.length();k++)
			{
				if(mystr[k]!=' ')
				{
					if(even == 0)
					{
						dis[n_index].push_back(mystr[k]);
						even = 1;
					}
					else
					{
						dis[n_index].push_back(mystr[k]);
						even = 0;
						n_index++;
					}
				}
				else
					continue;
			}
			
			//determine the max prob. and check the trace
			int bpmidx = 0;
			for(int i=0;i<dis.size();i++)
			{	
				bpmidx = mapfind(dis[i],map);
				if(bpmidx == -1)
				{
					if(i == 0)
					{		
						vmap[i].push_back(dis[i]);
						trace[i].push_back(0);
						prob[i].push_back(0);
					}
					else if(i==1)
					{
						if(vmap[i-1].size()>1)
						{
							double maxprob =-99999;
							int where = 0;
							for(int kk=0;kk<prob[i-1].size();kk++)
							{
								if(prob[i-1][kk]+getBigramProb(vmap[i-1][kk].c_str(),dis[i].c_str(),lm,voc)>maxprob)
								{
									maxprob = prob[i-1][kk]+getBigramProb(vmap[i-1][kk].c_str(),dis[i].c_str(),lm,voc);
									where = kk;
								}	
							}
							vmap[i].push_back(dis[i]);
							trace[i].push_back(where);
							prob[i].push_back(maxprob);

						}
						else
						{
							trace[i].push_back(0);
							prob[i].push_back(prob[i-1][0]);
							vmap[i].push_back(dis[i]);
						}
					}
					else
					{
						if(vmap[i-1].size()*vmap[i-2].size()>1)
						{
							double maxprob =-99999;
							int where = 0;
							for(int kk=0;kk<prob[i-1].size();kk++)
							{
								for(int jj=0;jj<prob[i-2].size();jj++)
								{
									if(prob[i-2][jj]+
										getBigramProb(vmap[i-2][jj].c_str(),vmap[i-1][kk].c_str(),lm,voc)+
										getTrigramProb(vmap[i-2][jj].c_str(),vmap[i-1][kk].c_str(),dis[i].c_str(),lm,voc)
										>maxprob)
									{
										maxprob = prob[i-2][jj]+
										getBigramProb(vmap[i-2][jj].c_str(),vmap[i-1][kk].c_str(),lm,voc)+
										getTrigramProb(vmap[i-2][jj].c_str(),vmap[i-1][kk].c_str(),dis[i].c_str(),lm,voc);
										where = kk;
									}
								}	
							}
							vmap[i].push_back(dis[i]);
							trace[i].push_back(where);
							prob[i].push_back(maxprob);

						}
						else
						{
							trace[i].push_back(0);
							prob[i].push_back(prob[i-1][0]);
							vmap[i].push_back(dis[i]);
						}

					}
				}
				else
				{
					if(i == 0)
					{
						for(int j=0;j<data[bpmidx].size();j++)
						{
							VocabIndex wid1 = voc.getIndex(data[bpmidx][j].c_str());
							if(wid1 == Vocab_None)  //OOV
								wid1 = voc.getIndex(Vocab_Unknown);

							VocabIndex context[] = {  Vocab_None };
							vmap[i].push_back(data[bpmidx][j]);
							trace[i].push_back(0);
							prob[i].push_back(lm.wordProb( wid1,context));
						}
					}
					else if(i==1)
					{
						if(vmap[i-1].size()>1)
						{
							for(int jj=0;jj<data[bpmidx].size();jj++)
							{
								double maxprob =-99999;
								int where = 0;
								for(int kk=0;kk<prob[i-1].size();kk++)
								{
									if(prob[i-1][kk]+getBigramProb(vmap[i-1][kk].c_str(),data[bpmidx][jj].c_str(),lm,voc)>maxprob)
									{
										maxprob = prob[i-1][kk]+getBigramProb(vmap[i-1][kk].c_str(),data[bpmidx][jj].c_str(),lm,voc);
										where = kk;
									}	

								}
								vmap[i].push_back(data[bpmidx][jj]);
								trace[i].push_back(where);
								prob[i].push_back(maxprob);
							}

						}
						else
						{
							for(int ii=0;ii<data[bpmidx].size();ii++)
							{
								trace[i].push_back(0);
								prob[i].push_back(getBigramProb(vmap[i-1][0].c_str(),data[bpmidx][ii].c_str(),lm,voc));
								vmap[i].push_back(data[bpmidx][ii]);
							}
						}
					}
					else
					{
						if(vmap[i-1].size()*vmap[i-2].size()>1)
						{
							for(int hh=0;hh<data[bpmidx].size();hh++)
							{
								double maxprob =-99999;
								int where = 0;
								for(int kk=0;kk<prob[i-1].size();kk++)
								{
									for(int jj=0;jj<prob[i-2].size();jj++)
									{
										if(prob[i-2][jj]+
											getBigramProb(vmap[i-2][jj].c_str(),vmap[i-1][kk].c_str(),lm,voc)+
											getTrigramProb(vmap[i-2][jj].c_str(),vmap[i-1][kk].c_str(),data[bpmidx][hh].c_str(),lm,voc)
											>maxprob)
										{
											maxprob = prob[i-2][jj]+
											getBigramProb(vmap[i-2][jj].c_str(),vmap[i-1][kk].c_str(),lm,voc)+
											getTrigramProb(vmap[i-2][jj].c_str(),vmap[i-1][kk].c_str(),data[bpmidx][hh].c_str(),lm,voc);
											where = kk;
										}
									}	
								}
								vmap[i].push_back(data[bpmidx][hh]);
								trace[i].push_back(where);
								prob[i].push_back(maxprob);
							}
						}
						else
						{
							for(int ii=0;ii<data[bpmidx].size();ii++)
							{
								trace[i].push_back(0);
								prob[i].push_back(getTrigramProb(vmap[i-2][0].c_str(),vmap[i-1][0].c_str(),data[bpmidx][ii].c_str(),lm,voc));
								vmap[i].push_back(data[bpmidx][ii]);
							}
						}

					}
				}
			}


			// find the true trace here
			int last = 0;
			double maxpp=-99999;
			if(trace[str_length-1].size()>1)
			{
				for(int j=0;j<prob[str_length-1].size();j++)
				{
					if(prob[str_length-1][j]>maxpp)
					{	
						maxpp = prob[str_length-1][j];
						last = j;
					}
				}
			}
			vector<string> rightstr;
			rightstr.resize(str_length);
			for(int i = dis.size()-1;i>-1;i--)
			{
				rightstr[i] = vmap[i][last];
				last = trace[i][last];
			}
			//output the file or display on the screen
			cout<<"<s> ";
			for(int i=0;i<rightstr.size();i++)
				cout<<rightstr[i]<<" ";
			cout<<"</s>"<<endl;
		}
	}
	else
		cout<<"the order should be 2 or 3"<<endl;
	
}






