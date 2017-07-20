//Calculate position of z-center of mass as a function of x for sapphire

#include <iostream>
#include <iomanip>
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

//Read Header for a timestep	
void readHeader(ifstream &inFile,double *bounds,int &timestep,string line);

//Read data for one timestep
void readStep(ifstream &inFile,double *bounds,double *coords,int *elements,int numAtoms,int numSteps,int &timestep,string *arr);

// Calculate total mass of substrate
double calculateTotalMass(int numAtoms,int *elements);

//Calculate the center of mass of the substrate for one timestep
void centerOfMass(int numAtoms,double *coords,int *elements,double *zbar,int nx,double *bounds,double binLength,double totalMass);

//Write data for one timestep
void writeStep(ofstream &centerFile,double *zbar,int timestep,int nx);

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
	if(argc!=3)
		cout << "2 arguments required: substrate input, substrate position output" << endl;

	else
	{

		cout << "### Substrate Position ###" << endl;
		
		//File streams
		ifstream inFile(argv[1]); //Substrate input
		ofstream centerFile(argv[2]); //Center of Mass output
		
		if(!inFile.good())
		{
			cout << "Error opening " << argv[1] << endl;
			return 0;
		}

		if(!centerFile.good())
		{
			cout << "Error opening " << argv[2] << endl;
			return 0;
		}
		
		cout << "Files opened" << endl;
		
		//Variables
		string line;
		string arr[8];
		int timestep;
		int numAtoms=countAtoms(inFile);
		int numSteps=countSteps(inFile,numAtoms); //501

		// Total mass of substrate
		double totalMass;

		// Bin length in both x & y directions (Angstroms)
		double binLength = 15;

		//Allocate arrays
		double *coords = new double[3*numAtoms];
		double bounds[6];
		int *elements = new int[numAtoms];

		// Read header to determine box bounds, then reset file cursor
		readHeader(inFile,bounds,timestep,line);
		inFile.seekg(0);

		// Number of bins in each dimension
		int nx = (int) ceil((bounds[1]-bounds[0])/binLength);

		cout << "Bounds: x [" << bounds[0] << "," << bounds[1] << "], y [" << bounds[2] << "," << bounds[3] << "]" << endl;
		cout << "binLength = " << binLength << endl;
		cout << "# of bins: nx = " << nx << endl;

		// Center of mass array over x,y plane
		// Contains z-values of COM in corresponding bin
		double* zbar = new double[nx];
		
		cout << "Arrays allocated" << endl << endl;
		
		//Write legend to first line of file
		centerFile << "# z center of mass: x along rows y, along cols " << endl;
		centerFile << "# binLength = " << binLength << endl;
		centerFile << "# xbounds: " << bounds[0] << " " << bounds[1] << endl;
		centerFile << "# zbounds: " << bounds[4] << " " << bounds[5] << endl;
		centerFile << endl;
		
		//Ensure that arrays are delete []d
		try
		{
			//Loop through timesteps
			for(int step=0;step<numSteps;step++)
			{
				if(step%50==0)
					cout << "Step " << step + 1 << "/" << numSteps << endl;

				//Read data
				readStep(inFile,bounds,coords,elements,numAtoms,numSteps,timestep,arr);

				// Calculate total mass of substrate on first timestep
				if(step==0)
				{
					totalMass = calculateTotalMass(numAtoms,elements);
					cout << "Total Mass = " << totalMass << endl;
				}

				//Calculate center of mass
				centerOfMass(numAtoms,coords,elements,zbar,nx,bounds,binLength,totalMass);

				// Write data
				writeStep(centerFile,zbar,timestep,nx);

			}
		}
		
		catch(...)
		{
			cout << "Catch Delete" << endl;
			//Delete arrays
			delete [] coords;
			delete [] elements;
			throw;
		}
		
		//Delete arrays
		delete [] coords;
		delete [] elements;

		delete [] zbar;
		
		//Close output file stream
		centerFile.close();
	   
		cout << endl << "Calculations complete!" << endl;

		cout << "###" << endl;
		
		return 0;
	}
}

///////////////
// Functions //
///////////////

//Read Header for a timestep	
void readHeader(ifstream&inFile,double *bounds,int &timestep,string line)
{
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
}

//Read data for one timestep
void readStep(ifstream &inFile,double *bounds,double *coords,int *elements,int numAtoms,int numSteps,int &timestep,string *arr)
{
	//Variables
	bool loopFlag;
	string line;
	static int step=0; //Count from zero
	static int lineNum=1;

	// Read header
	readHeader(inFile,bounds,timestep,line);

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

// Calculate total mass of substrate
double calculateTotalMass(int numAtoms,int *elements)
{
	double mass = 0;

	for(int k=0;k<numAtoms;k++)
		mass += masses[*elements++ - 1];
	
	return mass;
}

//Calculate the center of mass of the substrate for one timestep
void centerOfMass(int numAtoms,double *coords,int *elements,double *zbar,int nx,double *bounds,double binLength,double totalMass)
{
	// Find x and y extrema
	double xmin = bounds[0];
	double ymin = bounds[2];
	double zmin = bounds[4];

	double xmax = bounds[1];
	double ymax = bounds[3];
	double zmax = bounds[5];

	// Box length in each direction
	double xLength = xmax - xmin;
	double yLength = ymax - ymin;
	double zLength = zmax - zmin;

	// Coordinates
	double x,y,z;
	// Spatial counters
	int i;

	//Reset center of mass to {0,0,0}
	for(int i=0;i<nx;i++)
		zbar[i]=0;

	// Loop through all substrate atoms
	for(int k=0;k<numAtoms;k++)
	{
		// Extract coordinates
		x = *coords++;
		y = *coords++;
		z = *coords++;

		// Determine bin
		i = (int) floor((x - xmin)/binLength);
		if(i<0)
		{
			i = 0;
			cout << "x:" << x << "/" << xmin << " i: " << i << "/" << nx << endl;
		}
		else if(i>=nx)
		{
			i = nx-1;
			cout << "i>nx" << endl;
		}

		// WE'RE GETTING NEGATIVE VALUES FOR I AND J HERE
		// Add to center of mass of appropriate bin
		zbar[i] += (z * masses[*elements++ - 1]) / totalMass;
	}
}

//Write data for one timestep
void writeStep(ofstream &centerFile,double *zbar,int timestep,int nx)
{
	//Write timestep
	centerFile << setw(8) << timestep << " ";

	//Write center of mass
	for(int i=0;i<nx;i++)
		centerFile << setprecision(5) << setw(8) << zbar[i] << " ";
		
	//New line
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
		//*coords++=bounds[2*i]+(bounds[2*i+1]-bounds[2*i])*(atof(str.data())+atof(img.data())); 
		// IGNORE IMAGE NUMBER
		*coords++=bounds[2*i]+(bounds[2*i+1]-bounds[2*i])*(atof(str.data())); 
    }
}
