//////////////////////////////////////////////////////////////////////////
//            Line Integral Convolution for Flow Visualization          //
//                            Initial  Version                          //
//                              May 15, 1999                            //
//                                   by                                 //
//                            Zhanping Liu                              //
//                     (zhanping@erc.msstate.edu)                       //
//                               while with                             //
//                          Graphics  Laboratory                        //
//                           Peking  University                         //
//                              P. R.  China                            //
//----------------------------------------------------------------------//
//                             Later  Condensed                         //
//                             May 4,  2002                             //
//          VAIL (Visualization Analysis & Imaging Laboratory)          //
//                  ERC  (Engineering Research Center)                  //
//                     Mississippi State University                     //
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//This code was developed  based on  the original algorithm  proposed by//
//Brian Cabral  and  Leith (Casey) Leedom  in the paper  "Imaging Vector//
//Fields Using Line Integral Convolution", published  in  Proceedings of//
//ACM  SigGraph 93,  Aug 2-6,  Anaheim,  California,  pp. 263-270, 1993.//
//Permission to use, copy, modify, distribute and sell this code for any//
//purpose is hereby granted without fee,  provided that the above notice//
//appears in all copies  and  that both that notice and  this permission//
//appear in supporting documentation. The  developer  of this code makes//
//no  representations  about  the  suitability  of  this  code  for  any//
//purpose.  It is provided "as is"  without express or implied warranty.//
////////////////////////////////////////////////////////////////////////// 

#ifndef LIC_H
#define LIC_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream>
#include <fstream>

using namespace std;

class LIC
{
public:
    LIC(int n_xres, int n_yres, double* vfield);
    LIC(int n_xres, int n_yres);
    ~LIC();

    void SyntheszSaddle();
    void LoadColormap(char* cmap_file);
    void NormalizVectrs();
    void CalculateColor(double* vecMags);
    void GenBoxFiltrLUT(int LUTsiz);
    void MakeWhiteNoise();
    void FlowImagingLIC(unsigned char* pNoise, unsigned char* pImage, double krnlen);
    void WriteImage2PPM(char* f_name);
    double MinVal(double* array, int n);
    double MaxVal(double* array, int n);
    void HighPass();
    void HistEqual(double pwr);

private:
    int n_xres;
    int n_yres;

    double* pVectr;
    double* p_LUT0;
    double* p_LUT1;
    unsigned char* pNoise;
    unsigned char* pImage;
    double* cmap;
    double* vecMags;
    int* colorIndices;
    double licLength;
    int nIter;
	int curIter;

	int DISCRETE_FILTER_SIZE;
	double LINE_SQUARE_CLIP_MAX;
	double VECTOR_COMPONENT_MIN;

};

#endif



