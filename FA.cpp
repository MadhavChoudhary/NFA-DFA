#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <stdlib.h>
#include <queue>
#include <tgmath.h>

using namespace std;

#define pb push_back
#define loop(a,b) for(int i=a;i<=b;i++)
#define mp make_pair

class FA
{
public:
	int S,A;
	map<int, char> Alphabet;
	map<char, int> AlphaInt;
	list<int> **Transition;
	bool *isEnd;
	bool isDFA;
	int start;

	FA(int S, string Alpha, int A);
	void fillStateTable();
	void printStateTable();
	bool checkDeterministic();
	bool checkLanguage(string run);
	void removeNotReachableStates();
};

FA::FA(int S, string Alpha, int A)
{
	this->S = S;
	this->A = A;
	loop(0,A-1) Alphabet[i]=Alpha[i];
	loop(0,A-1) AlphaInt[Alpha[i]]=i;
	Transition = new list<int> *[S];
	loop(0,S-1) Transition[i] = new list<int>[A];
	isEnd = new bool[S];
	loop(0,S-1) isEnd[i]=false;
	isDFA=true;
	start=0;
}

void FA::fillStateTable()
{
	cout<<"Enter the State Table maintaining the same order\n";
	string line;
	for(int i=0;i<S;i++)
	{
		for(int j=0;j<A;j++)
		{
			cout<<"Enter for "<<i<<" upon "<<Alphabet[j]<<" : ";
			cin>>line;
			string delimiter = ",";

			if(line=="-"){
				isDFA=false;
				continue;
			}

			size_t pos = 0;
			while ((pos = line.find(delimiter)) != string::npos) {
			    Transition[i][j].pb(stoi(line.substr(0, pos)));
			    line.erase(0, pos + delimiter.length());
			    isDFA=false;
			}
			Transition[i][j].pb(stoi(line));
		}

		cout<<"End State? : ";
		cin>>line;
		if(line=="yes") isEnd[i]=true;
	}

}

void FA::removeNotReachableStates()
{
	queue< int > q;
	bool visited[S];
	loop(0,S-1) visited[i]=false;

	q.push(start);
		 
	while (!q.empty()) {
		
		int f= q.front();
	    q.pop();
	    visited[f]=true;

	    for(int j=0;j<A;j++)
	 		if(!visited[Transition[f][j].front()])
	 			q.push(Transition[f][j].front());
	 		
	}

	for(int i=0;i<S;i++)
		if(!visited[i])
			for(int j=0;j<A;j++)
				Transition[i][j].clear();
		
}

void FA::printStateTable()
{
	list<int>::iterator it;
	for(int i=0;i<S;i++)
	{
		for(int j=0;j<A;j++)
		{
			if(isDFA && Transition[i][j].size()==0)
				continue;

			else if(isEnd[i])
				cout<<"( "<<i<<" )"<<" --"<<Alphabet[j]<<"--> ";
			else if(start==i)
				cout<<"[ "<<i<<" ]"<<" --"<<Alphabet[j]<<"--> ";
			else
				cout<<"  "<<i<<"   --"<<Alphabet[j]<<"--> ";
			for(it=Transition[i][j].begin();it!=Transition[i][j].end();++it)
				cout<<*it<<" ";
			cout<<endl;
		}
	}
}

bool FA::checkLanguage(string run)
{
	//start at the start state
	if(isDFA)
	{
		int x=start,index=0;
		//check directly 
		while(index<run.length()){
			// printf("%d\n",x);
			x=Transition[x][AlphaInt[run[index++]]].front();
		}
		if(isEnd[x]) return true;
	}
	else
	{
		int index=0;
		list<int >::iterator it;
		//check by BFS Method
		queue< pair<int,int> > q;

	    q.push(mp(start,0));
		 
		while (!q.empty()) {
		 
		    pair<int,int> f = q.front();
		 	int index = f.second;
	 		int x = f.first;

	 		if(index>=run.length()) break;
	 		q.pop();

	 		int y = AlphaInt[run[index]];
	        for (it=Transition[x][y].begin();it!=Transition[x][y].end();++it) {
	        	q.push(mp(*it,index+1));
	        }
	    }

	    while(!q.empty()){
	    	if(isEnd[q.front().first])
	    		return true;
	    	q.pop();
	    }
	}

	return false;
}

FA *convertNFA(FA foo, string Alpha)
{
	int S = pow(2,foo.S)+1;
	int A = foo.A;
	int state[S];
	int new_states[S];

	state[0]=0;
	loop(1,S-1)
	{
		if(i & (i-1)) state[i]=0;
		else state[i]=1;
	}

	list<int>::iterator it;

	int next_state,x=0;

	FA *bar = new FA(S, Alpha, A);
	bar->start=1; //check this later


	//for existing states
	for(int i=0;i<foo.S;i++)
	{
		for(int j=0;j<A;j++)
		{
			next_state=0;
			for(it=foo.Transition[i][j].begin();it!=foo.Transition[i][j].end();++it)
					next_state+=pow(2,*it);

			bar->Transition[int(pow(2,i))][j].pb(next_state);

			

			if(state[next_state]==0) new_states[x++] = next_state;
			state[next_state] = 1;
		}
	}

	//for new states
	for(int i=0;i<x;i++)
	{
		for(int j=0;j<A;j++)
		{
			int n=0;
			for(int k=0;k<foo.S;k++)
			{
				if(new_states[i] & (1<<k))
				{
					int h=pow(2,k);
					if(n==0)
						n = bar->Transition[h][j].front();
					n = n | bar->Transition[h][j].front();
				}
			}
//printf("%d %d %d\n",new_states[i], j , n);
			if(state[n]==0)
			{
				new_states[x++] = n;
				state[n]=1;
			}
			
			bar->Transition[new_states[i]][j].pb(n);
	
		}
	}

	for(int i=0;i<S;i++)
	{
		if(state[i]==1)
		{
			for(int j=0;j<foo.S;j++)
			{	
				if(i & 1<<j)
				{
					if(foo.isEnd[j]){
						bar->isEnd[i]=true;
					}
				}
			}		
			
		}

	}

	bar->removeNotReachableStates();

	return bar;
}

int main()
{
	int S,A;
	
	string Alpha;
	string run;
	cout<<"Enter the no of States and size of Alphabet\n";
	cin>>S>>A;
	cout<<"Enter the Alphabet\n";
	cin>>Alpha;

	FA foo(S,Alpha,A);
	cout<<"\nThe Model has been created\n";

	foo.fillStateTable();
	cout<<"\n\nState Table:\n";
	foo.printStateTable();

	cout<<"\nEnter a string to test, \"exit\" to exit\n";
	while(1){
		cin>>run;
		if(run=="Exit" || run == "exit") break;
		if(foo.checkLanguage(run)) 
			cout<<"\'"<<run<<"\'    "<<" Belongs to language\n";
		else
			cout<<"\'"<<run<<"\'    "<<" Doesn't Belong\n";
	}
	if(foo.isDFA)
		printf("\nThis is a DFA\n");
	else{ 
		printf("\nThis is a NFA\n");
		FA *bar = convertNFA(foo,Alpha);
		cout<<"\nConverted to DFA:\n";
		bar->printStateTable();
	}

	return 0;
}
