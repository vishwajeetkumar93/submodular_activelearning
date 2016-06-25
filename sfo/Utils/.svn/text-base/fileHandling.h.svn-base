/*
 *	Utility code for file handling of various functions
	Implemented originally by Hui Lin, modified by Yuzong Liu and Kai Wei. Made more general by Rishabh Iyer.
	Melodi Lab, University of Washington, Seattle
 *
 */
#ifndef __FEATUREREP__
#define __FEATUREREP__
struct Feature{ // Specific feature f
     long int index; // index f
     int num_uniq_wrds; // number of non-zero enteries
     int* featureIndex; // Indices of m_f which are non-zero (generally sparse)
     float* featureVec; // score of the features present (i.e values of m_f)
     int tot_num_wrds;
};
#endif

long GetFileSize(const char * filename )
{
    struct stat statebuf;

    if ( stat( filename, &statebuf ) == -1 )
        return -1L;
    else
        return statebuf.st_size;
}




