#ifndef _sf_alloc_h
#define _sf_alloc_h

#include <stdlib.h>

#include "c99.h"

    /*@out@*/ void *sf_alloc (size_t n    /* number of elements */, 
			      size_t size /* size of one element */);
/*< output-checking allocation >*/

void *sf_realloc (void* ptr   /* previous data */, 
		  size_t n    /* number of elements */, 
		  size_t size /* size of one element */);
/*< output-checing reallocation >*/

/*@out@*/ char *sf_charalloc (size_t n /* number of elements */);
/*< char allocation >*/

/*@out@*/ unsigned char *sf_ucharalloc (size_t n /* number of elements */);
/*< unsigned char allocation >*/

/*@out@*/ int *sf_intalloc (size_t n /* number of elements */);
/*< int allocation >*/

/*@out@*/ float *sf_floatalloc (size_t n /* number of elements */);
/*< float allocation >*/

#ifndef __cplusplus

/*@out@*/ float complex *sf_complexalloc (size_t n /* number of elements */) ;
/*< complex allocation >*/

/*@out@*/ float complex **sf_complexalloc2 (size_t n1 /* fast dimension */, 
					    size_t n2 /* slow dimension */);
/*< complex 2-D allocation, out[0] points to a contiguous array >*/

#endif

/*@out@*/ bool *sf_boolalloc (size_t n /* number of elements */);
/*< bool allocation >*/

/*@out@*/ bool **sf_boolalloc2 (size_t n1 /* fast dimension */, 
				size_t n2 /* slow dimension */);
/*< bool 2-D allocation, out[0] points to a contiguous array >*/

/*@out@*/ float **sf_floatalloc2 (size_t n1 /* fast dimension */, 
				  size_t n2 /* slow dimension */);
/*< float 2-D allocation, out[0] points to a contiguous array >*/

/*@out@*/ float ***sf_floatalloc3 (size_t n1 /* fast dimension */, 
				   size_t n2 /* slower dimension */, 
				   size_t n3 /* slowest dimension */);
/*< float 3-D allocation, out[0][0] points to a contiguous array >*/

/*@out@*/ int **sf_intalloc2 (size_t n1 /* fast dimension */, 
			      size_t n2 /* slow dimension */);
/*< float 2-D allocation, out[0] points to a contiguous array >*/

/*@out@*/ char **sf_charalloc2 (size_t n1 /* fast dimension */, 
				size_t n2 /* slow dimension */) ;
/*< char 2-D allocation, out[0] points to a contiguous array >*/

/*@out@*/ unsigned char **sf_ucharalloc2 (size_t n1 /* fast dimension */, 
					  size_t n2 /* slow dimension */);
/*< unsigned char 2-D allocation, out[0] points to a contiguous array >*/

#endif
