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

// Refer to Enhanced LIC:
// Enhanced Line Integral Convolution with Flow Feature Detection (1997)
// by Arthur Okada , David Lane
// In SPIE Vol. 3017 Visual Data Exploration and Analysis IV
// http://citeseerx.ist.psu.edu/viewdoc/download;jsessionid=62D238FE3BCA070245D7748174523482?doi=10.1.1.35.6799&rep=rep1&type=pdf

#include "LIC.h"

LIC::LIC(int _n_xres, int _n_yres, double* _vfield)
{
	n_xres = _n_xres;
	n_yres = _n_yres;
	pVectr = _vfield;
    p_LUT0 = (double*) malloc( sizeof(double) * DISCRETE_FILTER_SIZE);
    p_LUT1 = (double*) malloc( sizeof(double) * DISCRETE_FILTER_SIZE);
    pNoise = (unsigned char* ) malloc( sizeof(unsigned char) * n_xres * n_yres     );
    pImage = (unsigned char* ) malloc( sizeof(unsigned char) * n_xres * n_yres     );
    cmap = (double* ) malloc( sizeof(double) * 256 * 3 );
    vecMags = (double* ) malloc( sizeof(double) * n_xres * n_yres );
    colorIndices = (int*) malloc( sizeof(int) * n_xres * n_yres);
    licLength = 40;
    nIter = 2;
    curIter = 0;

	DISCRETE_FILTER_SIZE = 2048;
    LINE_SQUARE_CLIP_MAX = 100000.0;
    VECTOR_COMPONENT_MIN = 0.050000 ;

    LoadColormap("phase_cmap.txt");
    NormalizVectrs();
    CalculateColor(vecMags);
    MakeWhiteNoise();
    GenBoxFiltrLUT(DISCRETE_FILTER_SIZE);
    FlowImagingLIC(pNoise, pImage, licLength);
    // Repeat LIC as many times as desired
    for(int iter=1; iter<nIter; iter++)
        FlowImagingLIC(pImage, pImage, licLength);
    HighPass();
    HistEqual(.85);
    WriteImage2PPM("LIC.ppm");
}

LIC::LIC(int _n_xres, int _n_yres)
{
	DISCRETE_FILTER_SIZE = 2048;
    LINE_SQUARE_CLIP_MAX = 100000.0;
    VECTOR_COMPONENT_MIN = 0.050000 ;

	n_xres = _n_xres;
	n_yres = _n_yres;
    pVectr = (double*) malloc( sizeof(double) * n_xres * n_yres * 2 );
    p_LUT0 = (double*) malloc( sizeof(double) * DISCRETE_FILTER_SIZE);
    p_LUT1 = (double*) malloc( sizeof(double) * DISCRETE_FILTER_SIZE);
    pNoise = (unsigned char* ) malloc( sizeof(unsigned char) * n_xres * n_yres     );
    pImage = (unsigned char* ) malloc( sizeof(unsigned char) * n_xres * n_yres     );
    cmap = (double* ) malloc( sizeof(double) * 256 * 3 );
    vecMags = (double* ) malloc( sizeof(double) * n_xres * n_yres );
    colorIndices = (int*) malloc( sizeof(int) * n_xres * n_yres);
    licLength = 40;
    nIter = 2;
    curIter = 0;

    SyntheszSaddle();
    LoadColormap("phase_cmap.txt");
    NormalizVectrs();
    CalculateColor(vecMags);
    MakeWhiteNoise();
    GenBoxFiltrLUT(DISCRETE_FILTER_SIZE);
    FlowImagingLIC(pNoise, pImage, licLength);
    // Repeat LIC as many times as desired
    for(int iter=1; iter<nIter; iter++)
        FlowImagingLIC(pImage, pImage, licLength);
    HighPass();
    HistEqual(.85);
    WriteImage2PPM("LIC.ppm");
}

LIC::~LIC()
{
    free(pVectr);    pVectr = NULL;
    free(p_LUT0);    p_LUT0 = NULL;
    free(p_LUT1);    p_LUT1 = NULL;
    free(pNoise);    pNoise = NULL;
    free(pImage);    pImage = NULL;
    free(cmap);    cmap = NULL;
    free(vecMags); vecMags = NULL;
    free(colorIndices); colorIndices = NULL;
}

///        synthesize a saddle-shaped vector field     ///
void LIC::SyntheszSaddle()
{
        for(int  j = 0;  j < n_yres;  j ++)
        for(int  i = 0;  i < n_xres;  i ++)
        {
            int     index = (  (n_yres - 1 - j) * n_xres + i  )  <<  1;
            pVectr[index    ] = - ( j / (n_yres - 1.0f) - 0.5f );
            pVectr[index + 1] =     i / (n_xres - 1.0f) - 0.5f;
        }
}


///        normalize the vector field     ///
void LIC::NormalizVectrs()
{
        for(int     j = 0;  j < n_yres;  j ++)
        for(int     i = 0;  i < n_xres;  i ++)
        {
            int        index = (j * n_xres + i) << 1;
            double    vcMag = double(  sqrt( double(pVectr[index] * pVectr[index] + pVectr[index + 1] * pVectr[index + 1]) )  );
            // Save vector magnitude to array
            vecMags[index >> 1] = vcMag;

            double    scale = (vcMag == 0.0f) ? 0.0f : 1.0f / vcMag;
            pVectr[index    ] *= scale;
            pVectr[index + 1] *= scale;
        }
}


///        make white noise as the LIC input texture     ///
void LIC::MakeWhiteNoise()
{
        for(int  j = 0;   j < n_yres;  j ++)
        for(int  i = 0;   i < n_xres;  i ++)
        {
            int  r = rand();
            r = (  (r & 0xff) + ( (r & 0xff00) >> 8 )  ) & 0xff;
            pNoise[j * n_xres + i] = (unsigned char) r;
        }
}


///        generate box filter LUTs     ///
void LIC::GenBoxFiltrLUT(int LUTsiz)
{
           for(int  i = 0;  i < LUTsiz;  i ++)  p_LUT0[i] = p_LUT1[i] = i;
}


///        write the LIC image to a PPM file     ///
void LIC::WriteImage2PPM(char* f_name)
{
    cout << "Write to file: '" << f_name << "'" << endl;
    double dblval;
    unsigned char color[3];
    int index;
    FILE*    o_file;
    if(   ( o_file = fopen(f_name, "w") )  ==  NULL   )
    {
        printf("Can't open output file\n");
        return;
    }

    fprintf(o_file, "P6\n%d %d\n255\n", n_xres, n_yres);

    for(int  j = 0;  j < n_yres;  j ++)
    for(int  i = 0;  i < n_xres;  i ++)
    {
        index = j*n_xres + i;
        dblval = pImage[index];
        for(int k=0; k<3; k++)
            color[k] = (unsigned char) floor(dblval * cmap[3*colorIndices[index] + k]);

        fprintf(o_file, "%c%c%c", color[0], color[1], color[2]);
    }

    fclose (o_file);    o_file = NULL;
}


///        flow imaging (visualization) through Line Integral Convolution     ///
void LIC::FlowImagingLIC(unsigned char*  pNoise, unsigned char*  pImage,double krnlen )
{
        cout << "LIC iter " << ++curIter << endl;
        int        vec_id;                        ///ID in the VECtor buffer (for the input flow field)
        int        advDir;                        ///ADVection DIRection (0: positive;  1: negative)
        int        advcts;                        ///number of ADVeCTion stepS per direction (a step counter)
        int        ADVCTS = int(krnlen * 3);    ///MAXIMUM number of advection steps per direction to break dead loops

        double    vctr_x;                        ///x-component  of the VeCToR at the forefront point
        double    vctr_y;                        ///y-component  of the VeCToR at the forefront point
        double    clp0_x;                        ///x-coordinate of CLiP point 0 (current)
        double    clp0_y;                        ///y-coordinate of CLiP point 0    (current)
        double    clp1_x;                        ///x-coordinate of CLiP point 1 (next   )
        double    clp1_y;                        ///y-coordinate of CLiP point 1 (next   )
        double    samp_x;                        ///x-coordinate of the SAMPle in the current pixel
        double    samp_y;                        ///y-coordinate of the SAMPle in the current pixel
        double    tmpLen;                        ///TeMPorary LENgth of a trial clipped-segment
        double    segLen;                        ///SEGment   LENgth
        double    curLen;                        ///CURrent   LENgth of the streamline
        double    prvLen;                        ///PReVious  LENgth of the streamline
        double    W_ACUM;                        ///ACcuMulated Weight from the seed to the current streamline forefront
        double    texVal;                        ///TEXture VALue
        double    smpWgt;                        ///WeiGhT of the current SaMPle
        double    t_acum[2];                    ///two ACcUMulated composite Textures for the two directions, perspectively
        double    w_acum[2];                    ///two ACcUMulated Weighting values   for the two directions, perspectively
        double*    wgtLUT = NULL;                ///WeiGhT Look Up Table pointing to the target filter LUT
          double    len2ID = (DISCRETE_FILTER_SIZE - 1) / krnlen;    ///map a curve LENgth TO an ID in the LUT

        ///for each pixel in the 2D output LIC image///
        for(int  j = 0;     j < n_yres;  j ++)
        for(int  i = 0;     i < n_xres;  i ++)
        {
            ///init the composite texture accumulators and the weight accumulators///
            t_acum[0] = t_acum[1] = w_acum[0] = w_acum[1] = 0.0f;

            ///for either advection direction///
            for(advDir = 0;  advDir < 2;  advDir ++)
            {
                ///init the step counter, curve-length measurer, and streamline seed///
                advcts = 0;
                curLen = 0.0f;
                clp0_x = i + 0.5f;
                clp0_y = j + 0.5f;

                ///access the target filter LUT///
                wgtLUT = (advDir == 0) ? p_LUT0 : p_LUT1;

                ///until the streamline is advected long enough or a tightly  spiralling center / focus is encountered///
                while( curLen < krnlen && advcts < ADVCTS )
                 {
                    ///access the vector at the sample///
                    vec_id = ( int(clp0_y) * n_xres + int(clp0_x) ) << 1;
                    vctr_x = pVectr[vec_id    ];
                    vctr_y = pVectr[vec_id + 1];

                    ///in case of a critical point///
                    if( vctr_x == 0.0f && vctr_y == 0.0f )
                    {
                        t_acum[advDir] = (advcts == 0) ? 0.0f : t_acum[advDir];           ///this line is indeed unnecessary
                        w_acum[advDir] = (advcts == 0) ? 1.0f : w_acum[advDir];
                        break;
                    }

                    ///negate the vector for the backward-advection case///
                    vctr_x = (advDir == 0) ? vctr_x : -vctr_x;
                    vctr_y = (advDir == 0) ? vctr_y : -vctr_y;

                    ///clip the segment against the pixel boundaries --- find the shorter from the two clipped segments///
                    ///replace  all  if-statements  whenever  possible  as  they  might  affect the computational speed///
                    segLen = LINE_SQUARE_CLIP_MAX;
                    segLen = (vctr_x < -VECTOR_COMPONENT_MIN) ? ( int(     clp0_x         ) - clp0_x ) / vctr_x : segLen;
                    segLen = (vctr_x >  VECTOR_COMPONENT_MIN) ? ( int( int(clp0_x) + 1.5f ) - clp0_x ) / vctr_x : segLen;
                    segLen = (vctr_y < -VECTOR_COMPONENT_MIN) ?
                             (      (    (  tmpLen = ( int(     clp0_y)          - clp0_y ) / vctr_y  )  <  segLen    ) ? tmpLen : segLen      )
                            : segLen;
                    segLen = (vctr_y >  VECTOR_COMPONENT_MIN) ?
                             (      (    (  tmpLen = ( int( int(clp0_y) + 1.5f ) - clp0_y ) / vctr_y  )  <  segLen    ) ? tmpLen : segLen      )
                            : segLen;

                    ///update the curve-length measurers///
                    prvLen = curLen;
                    curLen+= segLen;
                    segLen+= 0.0004f;

                    ///check if the filter has reached either end///
                    segLen = (curLen > krnlen) ? ( (curLen = krnlen) - prvLen ) : segLen;

                    ///obtain the next clip point///
                    clp1_x = clp0_x + vctr_x * segLen;
                    clp1_y = clp0_y + vctr_y * segLen;

                    ///obtain the middle point of the segment as the texture-contributing sample///
                    samp_x = (clp0_x + clp1_x) * 0.5f;
                    samp_y = (clp0_y + clp1_y) * 0.5f;

                    ///obtain the texture value of the sample///
                    texVal = pNoise[ int(samp_y) * n_xres + int(samp_x) ];

                    ///update the accumulated weight and the accumulated composite texture (texture x weight)///
                    W_ACUM = wgtLUT[ int(curLen * len2ID) ];
                    smpWgt = W_ACUM - w_acum[advDir];
                    w_acum[advDir]  = W_ACUM;
                    t_acum[advDir] += texVal * smpWgt;

                    ///update the step counter and the "current" clip point///
                    advcts ++;
                    clp0_x = clp1_x;
                    clp0_y = clp1_y;

                    ///check if the streamline has gone beyond the flow field///
                    if( clp0_x < 0.0f || clp0_x >= n_xres || clp0_y < 0.0f || clp0_y >= n_yres)  break;
                }
             }

            ///normalize the accumulated composite texture///
             texVal = (t_acum[0] + t_acum[1]) / (w_acum[0] + w_acum[1]);

            ///clamp the texture value against the displayable intensity range [0, 255]
            texVal = (texVal <   0.0f) ?   0.0f : texVal;
            texVal = (texVal > 255.0f) ? 255.0f : texVal;
            pImage[j * n_xres + i] = (unsigned char) texVal;
        }
}

// Load colormap from file
void LIC::LoadColormap(char* cmap_file)
{
    ifstream cmap_stream(cmap_file);
    int index;

    if(!cmap_stream.good())
        cout << "Failed to load cmap @ '" << cmap_file << "'" << endl;

    for(int i=0; i<256; i++)
    {
        index = 3*i;
        cmap_stream >> cmap[index] >> cmap[index+1] >> cmap[index+2];
    }

    cmap_stream.close();
}

// Calculate color index from magnitude
void LIC::CalculateColor(double* vecMags)
{
    int index;
    double mag;
    double min = MinVal(vecMags, n_xres*n_yres);
    double max = MaxVal(vecMags, n_xres*n_yres);
    double range = max - min;

    for(int i=0; i< n_xres; i++)
    {
        for(int j=0; j<n_yres; j++)
        {
            index = j*n_xres + i;
            mag = vecMags[index];
            if(mag > min && mag < max)
                colorIndices[index] = (int) floor(256*(mag - min)/range);
            else if (mag <= min)
                colorIndices[index] = 0;
            else
                colorIndices[index] = 255;
        }
    }
}

// Find minimum value from array
double LIC::MinVal(double* array, int n)
{
    double min = array[0];

    for(int i=0; i<n; i++)
        if(array[i] < min)
            min = array[i];

    return min;
}

// Find minimum value from array
double LIC::MaxVal(double* array, int n)
{
    double max = array[0];

    for(int i=0; i<n; i++)
        if(array[i] > max)
            max = array[i];

    return max;
}

// 3x3 High pass filter
void LIC::HighPass()
{
    // Sharpening convolution kernel
    double K[9] = {    0,   -1./5,    0,
                   -1./5,       5,-1./5,
                       0,   -1./5,    0};

    double* newimg = (double*) malloc(sizeof(double) * n_xres * n_yres);
    int outer_ind, inner_ind, ind1, ind2;
    double tmpdbl;

    // Loop through image pixels
    for(int i=0; i<n_xres; i++)
    {
        for(int j=0; j<n_yres; j++)
        {
            outer_ind = j*n_xres+i;
            newimg[outer_ind] = 0;
            // Loop through 3x3 convolution kernel
            // & adjacent pixels in image
			for(int m=0; m<3; m++)
			{
				for(int n=0; n<3; n++)
				{
					// Extend boundary
					ind1 = i-1+m;
					ind2 = j-1+n;
					if(ind1 < 0)
						ind1 = 0;
					else if (ind1 > n_xres-1)
						ind1 = n_xres-1;
					if(ind2 < 0)
						ind2 = 0;
					else if (ind2 > n_yres-1)
						ind2 = n_yres-1;
					inner_ind = ind2*n_xres + ind1;
					tmpdbl = (double) pImage[inner_ind];
					newimg[outer_ind] += tmpdbl * K[3*n+m];
				}
			}
        }
    }

    // Rescale to [0,256]
    double min = MinVal(newimg, n_xres*n_yres);
    double max = MaxVal(newimg, n_xres*n_yres);
    double range = max-min;
    for(int i=0; i<n_xres*n_yres; i++)
        newimg[i] = 256 * (newimg[i]-min)/range;

    // Update image
    for(int i=0; i<n_xres*n_yres; i++)
        pImage[i] = (unsigned char) floor(newimg[i]);

    free(newimg);
}

// Approximate histogram equalization filter by raising to power
void LIC::HistEqual(double pwr)
{
    cout << "Histogram equalization: pwr=" << pwr << endl;
    int ind;
    double tmp;

    for(int i=0; i<n_xres; i++)
    {
        for(int j=0; j<n_yres; j++)
        {
            ind = n_xres*j + i;
            tmp = (double) pImage[ind];
            // Transform to [-.5,.5]
            tmp = tmp/256.-.5;
            // Raise to power to push closer to -.5 or .5
            if(tmp >= 0)
                tmp = pow(tmp,pwr);
            else
                tmp = -pow(fabs(tmp),pwr);
            // Convert back to [0,256]
            tmp = 256 * (tmp+.5);
            pImage[ind] = (unsigned char) floor(tmp);
        }
    }
}

