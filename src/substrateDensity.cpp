//Calculate density(z) of sapphire

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>

using namespace std;

//Masses of atoms
const double masses[3] = {26.981540,15.994915,1.007825};

////////////////
// Prototypes //
////////////////

//Read data for one timestep
void readStep(ifstream &inFile,double *bounds,double *coords,int *elements,int numAtoms,int numSteps,int &timestep,string *arr);

//Calculate density(z) for one timestep
void calcDensity(int numAtoms,double *bounds,double *coords,int *elements,int nZ,double zWidth,double *zVals,double *density,double *center);

//Calculate the center of mass of the substrate for one timestep
void centerOfMass(int numAtoms,double *coords,int *elements,double *center);

//Write data for one timestep
void writeStep(ofstream &densFile,ofstream &centerFile,int nZ,double *density,double *center,int timestep);

//Count the number of water atoms in the first timestep
int countAtoms(ifstream &inFile);

//Count the number of timesteps
int countSteps(ifstream &inFile,int numAtoms);

//Split a string into a string array of words
string *strSplit(string str,string *arr);

//Get index and coordinates from string array of words
void strToData(double *&coords,int *&elements,string line,double bounds[6],string *arr);

//////////
// Main //
//////////

int main(int argc, char* argv[])
{
	//File I/O from command line arguments
	if(argc!=4)
		cout << "3 arguments required: substrate input, density output, center of mass output" << endl;

	else
	{
		
		//File streams
		ifstream inFile(argv[1]); //Substrate input
		ofstream densFile(argv[2]); //Density output
		ofstream centerFile(argv[3]); //Center of Mass output
		
		if(!inFile.good())
		{
			cout << "Error opening " << argv[1] << endl;
			return 0;
		}

		if(!densFile.good())
		{
			cout << "Error opening " << argv[2] << endl;
			return 0;
		}
		
		if(!centerFile.good())
		{
			cout << "Error opening " << argv[3] << endl;
			return 0;
		}
		
		cout << "Files opened" << endl;
		
		//Variables
		string line;
		string arr[8];
		int timestep;
		int numAtoms=countAtoms(inFile);
		int numSteps=countSteps(inFile,numAtoms); //501
		double center[3];

		//z values
		//Double check this before using non-integer values for zmin,zmax,zWidth
		double zmin=-15;
		double zmax=15;
		double zWidth = 0.5;
		int nZ = (int) floor((zmax-zmin)/zWidth);
		
		cout << "Variables declared" << endl;
		
		//Allocate arrays
		double *coords = new double[3*numAtoms];
		double bounds[6];
		int *elements = new int[numAtoms];
		double *zVals = new double[nZ];
		double *density = new double[nZ];
		
		cout << "Arrays allocated" << endl << endl;

		//Define z-values
		for(int i=0;i<nZ;i++)
			zVals[i]=zmin+i*zWidth;
		
		//Write legend to first line of file
		densFile << "#TIMESTEP ";
		for(int i=0;i<nZ;i++)
			densFile << "z=" << zVals[i]	<< " ";
		densFile << endl << endl;
		
		//Ensure that arrays are delete []d
		try
		{
			//Loop through timesteps
			for(int step=0;step<numSteps;step++)
			{
				if(step%50==0)
					cout << "Step " << step << "/" << numSteps << endl;
				
				//Read data
				readStep(inFile,bounds,coords,elements,numAtoms,numSteps,timestep,arr);

				//Calculate center of mass
				centerOfMass(numAtoms,coords,elements,center);

				//Density Calculations
				calcDensity(numAtoms,bounds,coords,elements,nZ,zWidth,zVals,density,center);

				//Write data
				writeStep(densFile,centerFile,nZ,density,center,timestep);
			}
		}
		
		catch(...)
		{
			cout << "Catch Delete" << endl;
			//Delete arrays
			delete [] coords;
			delete [] elements;
			delete [] zVals;
			delete [] density;
			throw;
		}
		
		//Delete arrays
		delete [] coords;
		delete [] elements;
		delete [] zVals;
		delete [] density;
		
		//Close output file stream
		densFile.close();
		centerFile.close();
	   
		cout << endl << "Calculations complete!" << endl;
		
		return 0;
	}
}

///////////////
// Functions //
///////////////


//Read data for one timestep
void readStep(ifstream &inFile,double *bounds,double *coords,int *elements,int numAtoms,int numSteps,int &timestep,string *arr)
{
	//Variables
	bool loopFlag;
	string line;
	static int step=0; //Count from zero
	static int lineNum=1;

	//Read Header
	
	//Skip first line
	getline(inFile,line);
	//Read timestep
	inFile >> timestep;
	inFile.ignore();
	//Skip next 3 lines
	for(int i=0;i<3;i++) inFile.ignore(256,'\n');
	//Read box bounds
	for(int i=0;i<3;i++)
	{
		inFile >> bounds[2*i] >> bounds[2*i+1];
		inFile.ignore();
	}
	//Ignore next line
	inFile.ignore(256,'\n');

	lineNum+=9;
	
	//Read Atoms
	for(int i=0;i<numAtoms;i++)
	{
		lineNum++;
		getline(inFile,line);

		//Get next oxygen data from current line
		strToData(coords,elements,line,bounds,arr);
		//cout << "e=" << *(elements-1) << endl;
		//cout << "c=(" << *(coords-3) << "," << *(coords-4) << "," << *(coords-1) << ")" << endl;

	}

	//Increment timestep counter
	step++;
	
}

//Calculate density(z) for one timestep
void calcDensity(int numAtoms,double *bounds,double *coords,int *elements,int nZ,double zWidth,double *zVals,double *density,double *center)
{
	//x-y area is 171312.33624 AA^2 for spherical droplets
	double volume=(bounds[1]-bounds[0])*(bounds[3]-bounds[2])*zWidth;

	//unit conversion factor
	double convFact=10/6.022;

	int bin;

	//Start on z coordinate
	coords+=2;

	for(int i=0;i<numAtoms;i++)
	{
		//Subtract center of mass
		*coords-=center[2];

		//Identify which bin the atom falls in
		bin=(int)floor((*coords-zVals[0])/zWidth);

		//Add mass to bin
		//We are subtracting one from the element # because the file being read counts from 1, while C++ arrays count from 0.
		if(bin<nZ)
			density[bin]+=masses[*elements++-1];
		//If at the very end, add to last bin
		else
			density[bin-1]+=masses[*elements++-1];

		//Skip to next z coordinate
		coords+=3;
	}

	//Scale density from g to g/cc
	for(int i=0;i<nZ;i++)
		density[i]*=convFact/volume;
}

//Calculate the center of mass of the substrate for one timestep
void centerOfMass(int numAtoms,double *coords,int *elements,double *center)
{
	//Count total mass
	double totalMass=0;

	//Maximum x distance to consider for center of mass calculations
	double cutoff=50;

	//Reset center of mass to {0,0,0}
	for(int i=0;i<3;i++)
		center[i]=0;

	for(int j=0;j<numAtoms;j++)
	{
		//Check x position
		if(abs(*coords)<cutoff)
		{
			//Add x,y,z masses of each atom to center
			for(int i=0;i<3;i++)
				center[i]+=*coords++*masses[*elements-1];

			//Add mass of this atom to total mass
			totalMass+=masses[*elements++-1];

		}
		//Otherwise, skip to next molecule
		else
		{
			coords+=3;
			elements+=1;
		}
	}

	//Divide by total mass
	for(int i=0;i<3;i++)
		center[i]/=totalMass;
}

//Write data for one timestep
void writeStep(ofstream &densFile,ofstream &centerFile,int nZ,double *density,double *center,int timestep)
{
	//Write timestep
	densFile << timestep << " ";
	centerFile << timestep << " ";

	//Write densities
	for(int i=0;i<nZ;i++)
		densFile << *density++ << " ";

	//Write center of mass
	for(int i=0;i<3;i++)
		centerFile << *center++ << " ";
		
	//New line
	densFile << endl;
	centerFile << endl;
}

//Count the number of water atoms in the first timestep
int countAtoms(ifstream &inFile)
{
	cout << "Counting atoms..." << endl;
    //Count number of atoms
    bool countFlag=true;
    string line;
    int numAtoms=0;
    
    //Ignore the first 9 lines
	for(int i=0;i<9;i++) inFile.ignore(256,'\n');
    
    while(countFlag)
    {
	getline(inFile,line);
	
	//Count until reaching a line containing "TIMESTEP"
	if(line.find("TIMESTEP")!=string::npos||inFile.eof())
	    countFlag=false;
	else
	    numAtoms++;

    }
	
    //Unset eof flag (if set) & return to beginning of file
    inFile.clear();
    inFile.seekg(0,ios::beg);
    
	cout << "Counted " << numAtoms << " atoms." << endl;
    return numAtoms;
}

//Count the number of timesteps
int countSteps(ifstream &inFile,int numAtoms)
{   
	cout << "Counting timesteps..." << endl;
    string line;
    int lineNum=0;
	int numSteps;
    
    //Count number of timesteps
    while(getline(inFile,line))
    {
		lineNum++;
		if(lineNum%100000000==0)
			cout << "line " << lineNum << endl;
    }
    
    //Unset eof flag (if set) & return to beginning of file
    inFile.clear();
    inFile.seekg(0,ios::beg);

    numSteps=lineNum/(numAtoms+9);
	cout << "Counted " << numSteps << " steps." << endl;
    return numSteps;
}

/*
//Count the number of timesteps
int countSteps(ifstream &inFile)
{   
	cout << "Counting timesteps..." << endl;
    string line;
    int numSteps=0;
    int lineNum=0;
    
    //Count number of timesteps
    while(getline(inFile,line))
    {
		if(line.find("TIMESTEP")!=string::npos)
			numSteps++;
		if(numSteps%100==0)
			cout << numSteps << endl;
    }
    
    //Unset eof flag (if set) & return to beginning of file
    inFile.clear();
    inFile.seekg(0,ios::beg);
    
	cout << "Counted " << numSteps << " steps." << endl;
    return numSteps;
}
*/

//Split a string into a string array of words
string *strSplit(string str,string *arr)
{
    int len=str.length();
    stringstream ss(str);
    const int numWords=8;
    
	/*
	//Count number of words
    for(int ch=0;ch<len;ch++)
	if(isspace(str[ch]))
	    numWords++;
	*/
    
    //Allocate array
    //string arr[numWords];
    
    //Read string into array
    for(int i=0;i<len;i++)
		ss >> *(arr+i);
    
    return arr;
}

//Get index and coordinates from string array of words 
void strToData(double *&coords,int *&elements,string line,double bounds[6],string *arr)
{
    string str;
	string img;
    
    //Split string into array of words
    string *strArr=strSplit(line,arr);
	
    //Get element
    str=*++strArr;
    *elements++=atoi(str.data());

    //Get coordinates from the third, fourth, and fifth elements in the array
    //string -> cstring -> double
    for(int i=0;i<3;i++)
    {
		str=*++strArr;
		img=*(strArr+3); //image: ix,iy,iz
		//Unscale and unwrap data
		*coords++=bounds[2*i]+(bounds[2*i+1]-bounds[2*i])*(atof(str.data())+atof(img.data())); 
    }
}
