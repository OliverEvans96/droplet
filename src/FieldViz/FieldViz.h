// File Name: FieldViz.cpp
// Description: Vector Field Visualization (LIC)
// Created: Thu Feb 16, 2017 | 08:57pm EST
// Last Modified:

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

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cstring>

using namespace std;

#ifndef FIELDVIZ_H
#define FIELDVIZ_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream>
#include <fstream>

using namespace std;

class FieldViz
{
public:
    FieldViz();
    FieldViz(int n_xPix, int n_yPix);
    FieldViz(int n_xPix, int n_yPix, double xmin, double xmax, double ymin, double ymax);
    ~FieldViz();

    void SetColormap(char* _cmapFile);
    void SetNumPixels(int _n_xPix, int _n_yPix);
    void Init();
    void PreProcess();
    void SetLICParams(int _nIter, double _histEqExp);
    void SetCylDataFromCart(double* _xx, double* _yy, double* zz,
                            double* _vx, double* _vy, double* vz,
                            int nAtoms);
    void PerformLIC();
    void PostProcess();
    void SetData(double* _xx, double* _yy, double* _vx, double* _vy, int _nAtoms);
	void ResetField();
	void SetNAtoms(int _nAtoms);
    void SetBounds(double _xmin, double _xmax, double _ymin, double _ymax);
    double MinVal(double* array, int n);
    double MaxVal(double* array, int n);
	void WriteField(const char* xFile, const char* yFile, const char* format);
	void WriteField4Col(char* outFile);
	void AddFieldFromData(double eps);
	void DivideFieldByFrames();
    void GenFieldFromData(double eps);
    void HighPass();
    void HistEqual(double pwr);
	double SmoothStep(double tt, double aa);
	void NormField();

	//void ConvertXYtoIJ(double** A_prime, double** A, int nx_prime, int ny_prime)
	//void Convert2DTo1D(double** array2d, double* array1d, int mm, int nn)
    void SyntheszSaddle();
    void LoadColormap(char* cmap_file);
    void NormalizVectrs();
    void CalculateColor(double* vecMags);
    void GenBoxFiltrLUT(int LUTsiz);
    void MakeWhiteNoise(unsigned char* pNoise);
    void FlowImagingLIC(unsigned char* pNoise, unsigned char* pImage, double krnlen);
    void WriteImage2PPM(const char* f_name);

private:
    int n_xPix, n_yPix;
    double *xx, *yy, *vx, *vy;
	double **xField, **yField;
	double **xFieldTmp, **yFieldTmp;
    int nAtoms;
    double xMin, xMax, yMin, yMax;
    double xRange, yRange;
    double xPixRes, yPixRes;
    int n_xBlocks, n_yBlocks;
    vector<int>** indVecs;

    double* pVectr;
    double* p_LUT0;
    double* p_LUT1;
    unsigned char* pNoise;
    unsigned char* pImage;
    double* cmap;
    double* vecMags;
    double* vecAngles;
    int* colorIndices;
    double licLength;
    int nIter;
    double histEqExp;
    char* cmapFile;
	int nFrames;

    int DISCRETE_FILTER_SIZE;
    double LINE_SQUARE_CLIP_MAX;
    double VECTOR_COMPONENT_MIN;

};

#endif



 
