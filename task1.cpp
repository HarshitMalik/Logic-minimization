#include <bits/stdc++.h>
using namespace std;

class InputData
{
	public:
		int numOfInpVar, numOfOutVar;
		vector<char> inpVar;
		vector<char> outVar;
		vector<int> expType;
		vector<string> exp;
		InputData()
		{
			numOfInpVar =0;
			numOfOutVar =0;
		}
		void print()
		{
			cout<<"Input variable : ";
			for(int i=0; i<numOfInpVar; i++)
				cout<<inpVar[i]<<" ";
			cout<<endl;
			cout<<"Output variable : ";
			for(int i=0; i<numOfOutVar; i++)
				cout<<outVar[i]<<" ";
			cout<<endl;
			cout<<"Expressions : \n";
			for(int i=0; i<numOfOutVar; i++)
				cout<<exp[i]<<endl;
		}
}inputData;

int bin2dec(int a[],int n)
{
	int num=0,k=0,i;
	for(i=n-1;i>=0;i--)
		num+=(a[i]*pow(2,k++));
	return num;
}

void change(set<int> &minTerms, int a[], int n);

void addMinTerm(set<int> &minTerms, int a[], int n)
{
	int i,f=0;
	for(int i=0; i<n;i++)
	{
		if(a[i]==-1)
		{
			f=1;
			change(minTerms,a,n);
			break;
		}
	}
	if(!f)
		minTerms.insert(bin2dec(a,n));
}

void change(set<int> &minTerms, int a[], int n)
{
	int i,f=0;
	for(int i=0; i<n;i++)
	{
		if(a[i]==-1)
		{
			a[i]=0;
			addMinTerm(minTerms,a,n);
			a[i]=1;
			addMinTerm(minTerms,a,n);
			a[i]=-1;
		}
	}
}
	
		

void minTerm(string exp,int type,ofstream& fout)
{
	set<int> minTerms;
	int i,j,arr[inputData.numOfInpVar];
	if(!type)
	{
		for(i=2;exp[i]!='\0';i++)
		{
			if(exp[i]>='a' && exp[i]<='z')
			{
				for(j=0;j<inputData.numOfInpVar;j++)
						arr[j]=-1;
				while(exp[i]>='a' && exp[i]<='z')
				{
					for(j=0;j<inputData.numOfInpVar;j++)
					{
						if(exp[i]==inputData.inpVar[j])
						{
							if(exp[i+1]=='\'')
							{
								arr[j]=0;
								i++;
							}
							else
								arr[j]=1;
							break;
						}
					}
					i++;
				}
				addMinTerm(minTerms,arr,inputData.numOfInpVar);
			}
		}
		fout<<exp[1]<<" = m( ";
		for(auto it = minTerms.begin(); it != minTerms.end(); it++)
			fout<<*it<<" ";	
		fout<<")"<<endl;
	}
	else
	{
		for(i=2;exp[i]!='\0';i++)
		{
			if(exp[i]=='(')
			{
				for(j=0;j<inputData.numOfInpVar;j++)
						arr[j]=-1;
				while(exp[i]!=')')
				{
					for(j=0;j<inputData.numOfInpVar;j++)
					{
						if(exp[i]==inputData.inpVar[j])
						{
							if(exp[i+1]=='\'')
							{
								arr[j]=1;
								i++;
							}
							else
								arr[j]=0;
							break;
						}
					}
					i++;
				}
				addMinTerm(minTerms,arr,inputData.numOfInpVar);
			}
		}
		set<int> temp;
		for(j=0;j<(int)(pow(2,inputData.numOfInpVar));j++)
			temp.insert(j);
		for(auto it = minTerms.begin(); it != minTerms.end(); it++)
			temp.erase(*it);
		fout<<exp[1]<<" = m( ";
		for(auto it = temp.begin(); it != temp.end(); it++)
			fout<<*it<<" ";	
		fout<<")"<<endl;
	}	
}



int main()
{
	string line; 
	int i;
	ifstream fin("input.txt");
	ofstream fout("outputTask1.txt");
	if(!fin.is_open())
		cout<<"Error in input file reading !\n";
	else
	{
		getline(fin,line);
		for(i=6; line[i]!='\0'; i++)
		{
			if(line[i]>='a' && line[i]<='z')
				inputData.inpVar.push_back(line[i]);
		}
		getline(fin,line);
		for(i=7; line[i]!='\0'; i++)
		{
			if(line[i]>='a' && line[i]<='z')
				inputData.outVar.push_back(line[i]);
		}
		fout<<".input ";
		for(auto it = inputData.inpVar.begin();it != inputData.inpVar.end();it++)
			fout<<*it<<" ";
		fout<<endl;
		reverse(inputData.inpVar.begin(),inputData.inpVar.end());
		inputData.numOfInpVar = inputData.inpVar.size();
		inputData.numOfOutVar = inputData.outVar.size();
		for(i=0;i<inputData.numOfOutVar;i++)
		{
			getline(fin,line);
			inputData.exp.push_back(line);
			for(int j=0;line[j]!='\0';j++)
			{
				if(line[j] == '(')
				{
					inputData.expType.push_back(1);
					break;
				}
				else if(line[j] == '+')
				{
					inputData.expType.push_back(0);
					break;
				}
			}
		}
		cout<<"File reading succesful.\nFile Data :\n";
		inputData.print();
		for(i=0;i<inputData.numOfOutVar;i++)
			minTerm(inputData.exp[i],inputData.expType[i],fout);
	}
	cout<<"Min term File created succesfully.\n";
	fin.close();
	fout.close();
return 0;
}



