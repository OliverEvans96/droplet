// File Name: test_lic.cpp
// Description: Test LIC
// Created: Wed Feb 15, 2017 | 06:47pm EST
// Last Modified: Wed Feb 22, 2017 | 05:58pm EST

//////////////////////////////////////////////////////////////////////////
//                           GNU GPL LICENSE                            //
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Copyright Oliver Evans 2017 <oliverevans96@gmail.com>                //
//                                                                      //
// This program is free software: you can redistribute it and/or modify //
// it under the terms of the GNU General Public License as published by //
// the Free Software Foundation, either version 3 of the License, or    //
// (at your option) any later version.                                  //
//                                                                      //
// This program is distributed in the hope that it will be useful,      //
// but WITHOUT ANY WARRANTY; without even the implied warranty of       //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         //
// GNU General Public License for more details.                         //
//                                                                      //
// You should have received a copy of the GNU General Public License    //
// along with this program. If not, see <http://www.gnu.org/licenses/>. //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "FieldViz.h"
#include <iostream>
#include <fstream>

int main()
{
    cout << "Start" << endl;

	// Data arrays
	int nAtoms = 8803;

	double *xx = new double[nAtoms];
	double *yy = new double[nAtoms];
	double *zz = new double[nAtoms];
	double *vx = new double[nAtoms];
	double *vy = new double[nAtoms];
	double *vz = new double[nAtoms];

	int n_xPix = 800;
	int n_yPix = 600;

	double xmin = 20;
	double xmax = 100;
	double ymin = -10;
	double ymax = 100;

	double eps = 5;
	int nIter = 2;
	double expnt = .5;

	// Read data from file
	ifstream dataStream("40Aatom3.txt");

	for(int ii=0; ii<nAtoms; ii++)
	{
		dataStream >> xx[ii] >> yy[ii] >> zz[ii]
				   >> vx[ii] >> vy[ii] >> vz[ii];
	}
	
    FieldViz myViz(n_xPix, n_yPix, xmin, xmax, ymin, ymax);
    //myViz.SetColormap("phase_cmap.txt");
    //myViz.SetLICParams(nIter,expnt);
    //myViz.SetNumPixels(n_xPix, n_yPix);
    //myViz.Init();
	//myViz.SyntheszSaddle();
    myViz.SetCylDataFromCart(xx, yy, zz, vx, vy, vz, nAtoms);
  	myViz.SetBounds(xmin, xmax, ymin, ymax);
  	myViz.GenFieldFromData(eps);
	myViz.WriteField("xfield.txt", "yfield.txt","ij");
	myViz.WriteField4Col("xyfield.txt");
    myViz.PreProcess();
    myViz.PerformLIC();
    myViz.PostProcess();
    myViz.WriteImage2PPM("fv.ppm");

	// Deallocate arrays
	delete [] xx, yy, zz, vx, vy, vz;

    cout << "Finish" << endl;
}
