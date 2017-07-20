//Use quicksort to put atoms in ascending order

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

//Count the number of water atoms in the system
int countAtoms(ifstream &inFile);

//Count the number of timesteps
int countSteps(ifstream &inFile);

//Read data for one timestep from file
void readData(ifstream &inFile,string *header,int *atomNumbers,string *wholeLines,int numAtoms);

//Quick sort!
void quickSort(int n[],string s[],int first,int last);

//Write sorted data to new file
void writeData(ofstream &outFile,string *header,string *wholeLines,int numAtoms);

//Swap elements i and j of parallel arrays
void swap(int n[],string s[],int i,int j);

//Get the first word from a string as an integer
int getFirstInt(string str);

int main(int argc, char* argv[])
{
	//File I/O from command line arguments
	if(argc!=3)
		cout << "2 arguments required: input & output files" << endl;

	else
	{
		//File streams
		ifstream inFile(argv[1]);
		ofstream outFile(argv[2]);
		
		if(!inFile.good())
		{	
			cout << "Error opening " << argv[1] << endl;
			return 0;
		}

		if(!outFile.good())
		{
			cout << "Error opening " << argv[2] << endl;
			return 0;
		}

		//Count atoms
		int numAtoms=countAtoms(inFile);
		
		//Count timesteps
		int numSteps=countSteps(inFile);
		
		cout << numAtoms << " atoms and " << numSteps << " timesteps" << endl;

		//Allocate arrays
		string *header = new string[9]; //Timestep,nAtoms,Box bounds
		int *atomNumbers = new int[numAtoms];
		string *wholeLines = new string[numAtoms];
		
		//For each timestep
		for(int step=0;step<numSteps;step++)
		{
			//Read data into arrays
			readData(inFile,header,atomNumbers,wholeLines,numAtoms);
			
			//Sort arrays
			quickSort(atomNumbers,wholeLines,0,numAtoms-1);
			
			//Write data to output file
			writeData(outFile,header,wholeLines,numAtoms);
		}

		return 0;
	}
}

//Count the number of water atoms in the system
int countAtoms(ifstream &inFile)
{
    //Count number of atoms
    bool countFlag=true;
    string line;
    int numAtoms=0;
    
    //Ignore the first 9 lines
	for(int i=0;i<9;i++)
		getline(inFile,line);
    
    while(countFlag)
    {
		getline(inFile,line);
		
		if(line.find("TIMESTEP")!=string::npos||inFile.eof())
			countFlag=false;
		else
			numAtoms++;
    }
	
    //Unset eof flag (if set) & return to beginning of file
    inFile.clear();
    inFile.seekg(0,ios::beg);
    
    return numAtoms;
}

//Count the number of timesteps
int countSteps(ifstream &inFile)
{
    string line;
    int numSteps=0;
    int lineNum=0;
    
    //Count number of timesteps
    while(getline(inFile,line))
	{
		if(line.find("TIMESTEP")!=string::npos)
			numSteps++;
	}
   
    //Unset eof flag (if set) & return to beginning of file
    inFile.clear();
    inFile.seekg(0,ios::beg);
    
    return numSteps;
}

//Read data for one timestep from file
void readData(ifstream &inFile,string *header,int *atomNumbers,string *wholeLines,int numAtoms)
{
	bool loopFlag=true;
    string line;
    
	//Header
	for(int i=0;i<9;i++)
	{
		getline(inFile,line);
		*header++=line;
	}
	
	//Atoms section
	for(int i=0;i<numAtoms;i++)
	{
		getline(inFile,line);
		*atomNumbers++=getFirstInt(line);
		*wholeLines++=line;
	}
}

//Write sorted data to new file
void writeData(ofstream &outFile,string *header,string *wholeLines,int numAtoms)
{
	//Header
	for(int i=0;i<9;i++)
		outFile << *header++ << endl;
	//Atoms section
	for(int i=0;i<numAtoms;i++)
		outFile << *wholeLines++ << endl;
}
	
    
//Swap elements i and j of parallel arrays
void swap(int n[],string s[],int i,int j)
{
	int nt=n[i];
	string st=s[i];
	
	n[i]=n[j];
	s[i]=s[j];
	
	n[j]=nt;
	s[j]=st;
}

//Quick sort!
void quickSort(int n[],string s[],int first,int last)
{
	if(last-first>0)
	{
		//Iteration counter
		static int count=0;
		
		count++;
		if(count%1000000==0)
			cout << "Quick sort pass #" << count << endl;

		//Pivot
		int pivot=n[(last+first)/2];
		
		//Indices
		int i=first;
		int j=last;
		
		while(i<=j)
		{
			while(n[i]<pivot)
				i++;
			while(n[j]>pivot)
				j--;
			if(i<=j)
			{
				swap(n,s,i,j);
				i++;
				j--;
			}
		}
		quickSort(n,s,first,j);
		quickSort(n,s,i,last);
	}
}
	
//Get the first word from a string as an integer
int getFirstInt(string str)
{
    string firstString;
    int firstInt;
    
    stringstream ss(str);
    ss >> firstString;
    firstInt=atoi(firstString.data());
    
    return firstInt;
}
