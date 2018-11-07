#include <bits/stdc++.h>
using namespace std;

struct Implicant{
	int* array;
	int size;
	Implicant(int _size){
		size = _size;
		array = new int[_size];
	}
	Implicant(int _size, Implicant i1, Implicant i2){
		size = _size;
		array = new int[_size];
		int i=0,j=0,k=0;
		while(i<i1.size && j<i2.size){
			if(i1.array[i]<i2.array[j])
				this->array[k++] = i1.array[i++];
			else
				this->array[k++] = i2.array[j++];
		}
		while(i<i1.size)
			this->array[k++] = i1.array[i++];
		while(j<i2.size)
			this->array[k++] = i2.array[j++];
	}
	bool operator == (Implicant imp)
	{
		if(this->size != imp.size)
			return 0;
		else{
			for(int i=0; i<this->size; i++){
				if(this->array[i] != imp.array[i])
					return 0;
			}
			return 1;
		}
	}
	void print()
	{
		cout<<" ( ";
		for(int i=0; i< this->size; i++)	
			cout<<this->array[i]<<" ";
		cout<<") ";
	}
};

struct Bucket{
	list<Implicant> implicants;
};

struct BucketStore{
	stack<Bucket*> store;
}bucketStore;

int largestMinTerm(list<int> &minTerms)
{
	minTerms.sort();
	return minTerms.back();
}
	
int numOf1(int n)
{
	int c=0;
	while(n>0){
		if(n%2)
			c++;
		n/=2;
	}
	return c;
}

bool is2n(int n)
{
	int x=1;
	while(x<=n)
	{
		if(x==n)
			return 1;
		x*=2;
	}
	return 0;
}

bool isPair(Implicant i1, Implicant i2)
{
	int diff;
	diff = i2.array[0] - i1.array[0];
	if(diff <=0 || !is2n(diff))
		return 0;
	else{
		for(int i=0; i< i2.size; i++){
			if( i2.array[i] - i1.array[i] != diff)
				return 0;
		}
	}
	return 1;
}

bool isImpPresent(list <Implicant> listImp, Implicant imp)
{
	for(auto it = listImp.begin(); it != listImp.end(); ++it){
		if(*it == imp)
			return 1;
	}
	return 0;
}

bool isMinTermPresent(int m, Implicant imp)
{
	for(int i=0; i<imp.size; i++){
		if(m == imp.array[i])
			return 1;
	}
	return 0;
}

int findImplicant(Bucket pBucket[], Bucket nBucket[], int sizeOfNBucket)
{
	if(sizeOfNBucket<1)
		return 0;
	Implicant *implicant;
	for(int i=0; i<sizeOfNBucket; i++){
		for(auto it1 = pBucket[i].implicants.begin();it1 != pBucket[i].implicants.end(); ++it1){
			for(auto it2=pBucket[i+1].implicants.begin();it2!=pBucket[i+1].implicants.end();++it2){
				if(isPair(*it1,*it2)){
					implicant = new Implicant((it1->size)+(it2->size),*it1,*it2);
					if(!(isImpPresent(nBucket[i].implicants, *implicant)))
						nBucket[i].implicants.push_back(*implicant);
					free(implicant);
				}
			}
		}
	}
	bucketStore.store.push(nBucket);
	Bucket *mBucket;
	mBucket = new Bucket[sizeOfNBucket-1];
	return findImplicant(nBucket, mBucket, sizeOfNBucket-1);
}
					
bool isSuperset(Implicant newImp, Implicant oldImp)
{
	int i,j,flag;
	for(i=0;i<newImp.size;i++){
		flag=0;
		for(j=0;j<oldImp.size;j++){
			if(newImp.array[i] == oldImp.array[j]){
				flag = 1;
				break;
			}
		}
		if(!flag)
			return 0;
	}
	return 1;
}

void getPrimeImplicants(list <Implicant> &primeImp)
{
	int n=1,j,flag;
	Bucket *bktPtr;
	while(!bucketStore.store.empty())
	{
		bktPtr = bucketStore.store.top();
		for(int j=0;j<n;j++){	
			for(auto it=bktPtr[j].implicants.begin(); it!=bktPtr[j].implicants.end(); ++it){
				flag = 1;
					for(auto it2 = primeImp.begin();it2 != primeImp.end(); ++it2){
						if(isSuperset(*it,*it2)){
							flag = 0;
							break;
						}
					}
				if(flag)
					primeImp.push_back(*it);
			}
		}
		n++;
		bucketStore.store.pop();
	}	
}

void getEssPrimeImplicants(list<Implicant> &essPrimeImp,list<Implicant> primeImp,list<int> minTerms)
{
		
	int i,n= minTerms.back()+1,numOfMinTerm=0,flag;
	int minTermArr[n][2]={0};
	for(auto it = minTerms.begin();it != minTerms.end(); ++it){
			minTermArr[*it][0]++;
			numOfMinTerm++;
	}
	for(i=0;i<n;i++){
		if(minTermArr[i][0]){
			for( auto it = primeImp.begin(); it!=primeImp.end(); it++){
				if(isMinTermPresent(i,*it)){
					minTermArr[i][1]++;
				}
			}
		}
	}
	while(numOfMinTerm){
		auto it = primeImp.begin();
		while(it!=primeImp.end()){
			flag=1;
			for(i=0;i<n;i++){
					if(minTermArr[i][0] && isMinTermPresent(i,*it)){
						for(int j=0;j<it->size;j++)
							minTermArr[it->array[j]][0]=0;
						essPrimeImp.push_back(*it);
						auto deleteItr = it;
						it++;
						flag =0;
						primeImp.erase(deleteItr);
						break;
					}
				}
			if(flag)
				it++;
		}
		it = primeImp.begin();
		while(it!=primeImp.end()){
			flag = 1;
			for(i=0;i<n;i++){
				if(minTermArr[i][0] && isMinTermPresent(i,*it)){
					flag = 0;
					break;
				}
			}
			if(flag){
				auto deleteItr = it;
				it++;				
				primeImp.erase(deleteItr);
			}
			else
				it++;	
		}	
		numOfMinTerm = 0;
		for(i=0;i<n;i++){
			if(minTermArr[i][0] == 1)
				numOfMinTerm++;
		}
	}
}

void getExp(list<Implicant> essPrimeImp,int n,vector<char>inpVar,ofstream& fout)
{
	int i,j,b,flag;
	for( auto it = essPrimeImp.begin(); it!=essPrimeImp.end(); it++){
		int a[n-1];
		for(i=0;i<n-1;i++)
			a[i]=-1;
		for(j=0;j< n-1; j++){
			b=((0x1<<j)&it->array[0]);	
			flag=1;
			for(i=1;i < it->size; i++){
				if(((0x1<<j)& it->array[i])!=b){
					flag=0;
					break;
				}
			}
			if(flag)
			{
				if(b)
					a[j]=1;
				else
					a[j]=0;
			}
		}
		if(a[0]!=-1)
			{
				if(a[0])
					fout<<inpVar[0];
				else
					fout<<inpVar[0]<<"'";
				fout<<" + ";			
			}
		for(i=1;i<n-1;i++)
		{			
			if(a[i]!=-1)
			{
				if(a[i])
					fout<<inpVar[i];
				else
					fout<<inpVar[i]<<"'";
			}
		}
	}
	fout<<"\n";
}

void fun(list<int> minTerms,vector<char> inpVar,ofstream& fout)
{
	int size_of_pBucket,i;
	Implicant *implicant;
	size_of_pBucket = ceil(log2(largestMinTerm(minTerms)))+1;
	Bucket *pBucket;
	pBucket = new Bucket[size_of_pBucket];
	for(auto it = minTerms.begin();it != minTerms.end(); ++it){
		implicant = new Implicant(1);
		implicant->array[0] = *it;
		pBucket[numOf1(*it)].implicants.push_back(*implicant);
		free(implicant);
	}
	
	bucketStore.store.push(pBucket);
	Bucket *nBucket;
	nBucket = new Bucket[size_of_pBucket-1];
	findImplicant(pBucket,nBucket, size_of_pBucket-1);
	Bucket prime_implicants;
	Bucket essPrimeImp;
	getPrimeImplicants(prime_implicants.implicants);
	getEssPrimeImplicants(essPrimeImp.implicants,prime_implicants.implicants,minTerms);
	getExp(essPrimeImp.implicants,size_of_pBucket,inpVar,fout);
}

int main()
{
	list <int> minTerms;
	int size_of_pBucket,i;
	Implicant *implicant;
	string line;
	vector<char> inputVar;

	ifstream fin("outputTask1.txt");
	ofstream fout("outputTask2.txt");
	if(!fin.is_open() || !fout.is_open())
		cout<<"Error in file reading or writing\n";
	else
	{
		getline(fin,line);
		for(i=6; line[i]!='\0'; i++)
		{
			if(line[i]>='a' && line[i]<='z')
				inputVar.push_back(line[i]);
		}
		cout<<" Input variables : ";
		for(auto it = inputVar.begin();it != inputVar.end(); ++it)
				cout<<*it<<" ";
		cout<<endl;
		getline(fin,line);		
		while(fin)
		{			
			for(i=1; line[i]!='\0'; i++)
			{
				if(line[i]>='0' && line[i]<='9')
				{
					int n=0;
					while(line[i]>='0' && line[i]<='9')
					{
						n=n*10+(line[i]-'0');
						i++;
					}
					minTerms.push_back(n);
				}
			}
			fout<<line[0]<<" = ";
			fun(minTerms,inputVar,fout);
			minTerms.clear();
			getline(fin,line);
		}
	}
	cout<<"\n Output file created succesfully\n";
	fin.close();
	fout.close();	
	return 0;
}
