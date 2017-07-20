//Given a sorted lammps output file containing only water molecules, calculate the cosine of the angle between the dipole moment and the unit surface normal vector.

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>

using namespace std;

////////////////
// Prototypes //
////////////////

//Read data for one timestep
void readStep(ifstream &inFile,int *OIndices,double *OCoords,double *prevOCoords,int *HIndices,double *HCoords,int numMolecules,int &timestep);

//Calculate velocities for one timestep
void calculateStepVelocities(double *OCoords,double *prevOCoords,double *velocities,double *speeds,int numMolecules);

//Calculate dipoles for one timestep
void calculateStepDipoles(double *OCoords,double *HCoords,double *dipoles,int numMolecules);

//Write data for one timestep
void writeStep(ofstream &outFile,int *OIndices,double *OCoords,double *velocities,double *speeds,double *dipoles,int numMolecules,int timestep);

//Count the number of water atoms in the first timestep
int countAtoms(ifstream &inFile);

//Count the number of timesteps
int countSteps(ifstream &inFile);

//Split a string into a string array of words
string *strSplit(string str);

//Get index and coordinates from string array of words
void strToData(int *&index,double *&coords,string line,double bounds[6]);

//////////
// Main //
//////////

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
		
		cout << "Files opened" << endl;
		
		//Variables
		string line;
		int timestep;
		int numAtoms=countAtoms(inFile);
		int numMolecules=numAtoms/3;
		int numSteps=countSteps(inFile);
		
		cout << "Variables declared" << endl;
		
		//Allocate arrays
		int *OIndices = new int[numMolecules];
		double *OCoords = new double[3*numMolecules];
		double *prevOCoords = new double[3*numMolecules];
		int *HIndices = new int[2*numMolecules];
		double *HCoords = new double[2*3*numMolecules];
		double *velocities = new double[3*numMolecules];
		double *speeds = new double[numMolecules];
		double *dipoles = new double[3*numMolecules];
		
		cout << "Arrays allocated" << endl;
		
		//Write legend to first line of file
		outFile << "Oxygen# | Coord Vector | Speed | Velocity Vector | Unit Dipole Vector" << endl << endl;
		
		//Ensure that arrays are delete []d
		try
		{
			cout << "Timestep 1/" << numSteps << endl;
			
			//Loop through timesteps
			for(int step=1;step<=numSteps;step++)
			{
				if(step%50==0)
					cout << "Timestep " << step << "/" << numSteps << endl;
				
				//Read data
				readStep(inFile,OIndices,OCoords,prevOCoords,HIndices,HCoords,numMolecules,timestep);

				//Calculate velocities
				calculateStepVelocities(OCoords,prevOCoords,velocities,speeds,numMolecules);
				
				//Test
				//cout << "Current: (" << OCoords[0] << "," << OCoords[1] << "," << OCoords[2] << ") Previous: (" << prevOCoords[0] << "," << prevOCoords[1] << "," << prevOCoords[2] << ") Velocities: (" << velocities[0] << "," << velocities[1] << "," << velocities[2] <<  endl;

				//Calculate dipoles
				calculateStepDipoles(OCoords,HCoords,dipoles,numMolecules);
				
				//Write data
				writeStep(outFile,OIndices,OCoords,velocities,speeds,dipoles,numMolecules,timestep);
			}
		}
		
		catch(...)
		{
			cout << "Catch Delete" << endl;
			//Delete arrays
			delete [] OIndices;
			delete [] OCoords;
			delete [] prevOCoords;
			delete [] HIndices;
			delete [] HCoords;
			delete [] velocities;
			delete [] speeds;
			delete [] dipoles;
			throw;
		}
		
		//Delete arrays
		delete [] OIndices;
		delete [] OCoords;
		delete [] prevOCoords;
		delete [] HIndices;
		delete [] HCoords;
		delete [] velocities;
		delete [] speeds;
		delete [] dipoles;
		
		//Close output file stream
		outFile.close();
	   
		cout << "Calculations complete!" << endl;
		
		return 0;
	}
}

///////////////
// Functions //
///////////////


//Read data for one timestep
void readStep(ifstream &inFile,int *OIndices,double *OCoords,double *prevOCoords,int *HIndices,double *HCoords,int numMolecules,int &timestep)
{
	//Variables
	bool loopFlag;
	string line;
	static int step=0;
	static int lineNum=0;
	double bounds[6];
	
	//Increment timestep counter
	step++;
	
	//Read Header
	
	//Skip first line
	getline(inFile,line);
	//Save timestep #
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
	
	
	//Read Atoms
	for(int i=0;i<numMolecules;i++)
	{
		getline(inFile,line);
		lineNum++;
		
		//Don't save previous data for first timestep
		if(step>1)
		{
			//Save previous oxygen coords
			for(int i=0;i<3;i++)
				*prevOCoords++=*(OCoords+i);
		}
		
		//Get oxygen data from current line
		strToData(OIndices,OCoords,line,bounds);
		
		//Get hydrogen atom data from next two lines
		for(int j=0;j<2;j++)
		{
			//Read hydrogen data
			getline(inFile,line);
			lineNum++;
			strToData(HIndices,HCoords,line,bounds);
		}
	}
}


//Calculate velocities for one timestep
void calculateStepVelocities(double *OCoords,double *prevOCoords,double *velocities,double *speeds,int numMolecules)
{
	//Variables
	static int step=0;
	static double sumSq;
	
	//Increment step counter
	step++;
	
	for(int mol=0;mol<numMolecules;mol++)
	{
		//Reset sumSq
		sumSq=0;
		
		for(int i=0;i<3;i++)
		{
			//Skip the first timestep
			if(step>1)
				*velocities=(*OCoords++ - *prevOCoords++)/2e3; //dt = 2000fs => units = AA/fs
			else
				*velocities=0;
			
			//Add to sumSq and move velocity pointer
			sumSq+=(*velocities * *velocities++);
		}
		
		//Calculate speed from sumSq
		*speeds++=sqrt(sumSq);
	}
}


//Calculate dipoles for one timestep
void calculateStepDipoles(double *OCoords,double *HCoords,double *dipoles,int numMolecules)
{
	//Coordinates of centroid between hydrogen atoms
	double avg[3];
	
	//non-normalized dipole
	double nonNormalized[3];
	
	//For magnitude
	double magnitude;
	double sumSq;
	
	//Loop through molecule
	for(int mol=0;mol<numMolecules;mol++)
	{
		//Reset sumSq
		sumSq=0;
		
		//Loop through coordinates in a molecule
		for(int i=0;i<3;i++)
		{
			//Calculate centroid
			avg[i]=(*(HCoords+i)+(*(HCoords+i+3)))/2;
			//Calculate dipole
			nonNormalized[i]=avg[i]-*OCoords++;
			
			//Add to sumSq
			sumSq+=nonNormalized[i]*nonNormalized[i];
		}
		
		//Calculate magnitude of dipole
		magnitude=sqrt(sumSq);
		
		//Save normalized dipole
		for(int i=0;i<3;i++)
			*dipoles++=nonNormalized[i]/magnitude;
		
		//Move HCoords pointer to next molecule (2 atoms, 3 coordinates each)
		HCoords+=2*3;
	}
}

//Write data for one timestep
void writeStep(ofstream &outFile,int *OIndices,double *OCoords,double *velocities,double *speeds,double *dipoles,int numMolecules,int timestep)
{
	//Mark timestep
	outFile << "TIMESTEP " << timestep << endl;

	//Loop through molecules
	for(int molecule=0;molecule<numMolecules;molecule++)
	{
		//Oxygen #
	    outFile << *OIndices++ << " ";
		//Coord Vector
	    for(int i=0;i<3;i++)
			outFile << *OCoords++ << " ";
		//Speed
		outFile << *speeds++ << " ";
		//Velocity Vector
		for(int i=0;i<3;i++)
			outFile << *velocities++ << " ";
		//Unit Dipole Vector
	    for(int i=0;i<3;i++)
			outFile << *dipoles++ << " ";
	    outFile << endl;
	}
	
	outFile << endl;
}

//Count the number of water atoms in the first timestep
int countAtoms(ifstream &inFile)
{
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

//Split a string into a string array of words
string *strSplit(string str)
{
    int len=str.length();
    stringstream ss(str);
    int numWords=1;
    
    //Count number of words
    for(int ch=0;ch<len;ch++)
	if(isspace(str[ch]))
	    numWords++;
    
    //Allocate array
    static string *arr = new string[numWords];
    
    //Read string into array
    for(int i=0;i<len;i++)
		ss >> *(arr+i);
    
    return arr;
}

//Get index and coordinates from string array of words 
void strToData(int *&index,double *&coords,string line,double bounds[6])
{
    string str;
	string img;
    
    //Split string into array of words
    string *strArr=strSplit(line);
	
    //Get index
    str=*strArr++;
    *index++=atoi(str.data());
    
    //Get coordinates from the third, fourth, and fifth elements in the array
    //string -> cstring -> double
    for(int i=0;i<3;i++)
    {
		str=*++strArr;
		img=*(strArr+3); //image: ix,iy,iz
		*coords++=bounds[2*i]+(bounds[2*i+1]-bounds[2*i])*(atof(str.data())+atof(img.data())); //Unscale and unwrap data
    }
}