/* 1-D inverse interpolation.

Takes: < irregular.rsf head=header.rsf > regular.rsf
*/

#include <float.h>
#include <math.h>

#include <rsf.h>

#include "int1.h"
#include "interp.h"
#include "tcai1.h"
#include "triangle1.h"
#include "causint.h"
#include "cgstep.h"
#include "bigsolver.h"
#include "pef.h"
#include "bound.h"
#include "polydiv.h"

int main (int argc, char* argv[])
{
    bool prec, pef;
    int id, nd, nt, it, nx, interp, filt, niter, three=3, i;
    float *mm, *dd, *offset, *aa, x0, dx, xmin, xmax, f, eps;
    filter bb;
    sf_file in, out, head;

    sf_init (argc,argv);
    in = sf_input("in");
    out = sf_output("out");

    if (!sf_histint(in,"n1",&nd)) nd=1;
    if (!sf_histint(in,"n2",&nt)) nt=1;
    if (SF_FLOAT != sf_gettype(in)) sf_error("Need float input");

    /* create coordinates */
    offset = sf_floatalloc(nd);

    head = sf_input("head");
    if (SF_FLOAT != sf_gettype(head)) sf_error("Need float head");
    sf_read (offset,sizeof(float),nd,head);
    sf_fileclose (head);

    xmin = +FLT_MAX;
    xmax = -FLT_MAX;
    for (id=0; id<nd; id++) {	
	f = offset[id]; 
	if (f < xmin) xmin=f;
	if (f > xmax) xmax=f;
    }
 
    /* create model */
    if (!sf_getint ("nx",&nx)) sf_error("Need nx=");
    /* number of bins */

    sf_putint(out,"n1",nx);

    /* let user overwrite */
    sf_getfloat ("xmin",&xmin);
    /* grid size */
    sf_getfloat ("xmax",&xmax);
    if (xmax <= xmin) sf_error ("xmax=%f <= xmin=%f",xmax,xmin);

    if (!sf_getfloat("x0",&x0)) x0=xmin; 
    /* grid origin */
    sf_putfloat (out,"o1",x0);

    if (!sf_getfloat("dx",&dx)) {
	/* grid sampling */
	if (1 >= nx) sf_error("Need dx=");
	dx = (xmax-xmin)/(nx-1);
    }
    sf_putfloat (out,"d1",dx);
    
    /* initialize interpolation */
    if (!sf_getint("interp",&interp)) interp=2;
    /* [1,2] forward interpolation method, 1: nearest neighbor, 2: linear */

    switch (interp) {
	case 1:
	    int1_init (offset, x0,dx,nx, bin_int, 1, nd);
	    sf_warning("Using nearest-neighbor interpolation");
	    break;
	case 2:
	    int1_init (offset, x0,dx,nx, lin_int, 2, nd);
	    sf_warning("Using linear interpolation");
	    break;
	default:
	    sf_error("Unsupported interp=%d",interp);
	    break;
    }

    if (!sf_getint("filter",&filt)) filt=1;
    /* filter type */
    if (!sf_getbool("prec",&prec)) prec=true;
    /* if y, use preconditioning */

    if (!prec) {
	filt++;
	sf_warning("Using differential regularization");	    
	aa = sf_floatalloc(filt);
	tcai1_init(filt,aa);
	if (filt==2) {
	    aa[0] = 1.;
	    aa[1] = -1.;
	} else {
	    aa[0] = 1.;
	    aa[1] = -2.;
	    aa[2] = 1.;
	}
    } else if (filt > 1) {
	triangle1_init (filt, nx);
    }

    mm = sf_floatalloc(nx);
    dd = sf_floatalloc(nd);

    if (!sf_getint("niter",&niter)) niter=nx;
    /* number of conjugate-gradient iterations */
    if (!sf_getfloat("eps",&eps)) eps=0.2;
    /* regularization parameter */
    if (!sf_getbool("pef",&pef)) pef=false;
    /* if y, use PEF for regularization */
 
    for (it=0; it < nt; it++) { /* loop over time slices */
	sf_read (dd,sizeof(float),nd,in);
	if (prec) {
	    if (filt==1) {
		solver_prec(int1_lop, cgstep, causint_lop, nx, nx, nd,
			    mm, dd, niter, eps, "end");
	    } else {
		solver_prec(int1_lop, cgstep, triangle1_lop, nx, nx, nd,
			    mm, dd, niter, eps, "verb", false, "end");
	    }
	} else {
	    solver_reg(int1_lop, cgstep, tcai1_lop, nx+filt, nx, nd, 
		       mm, dd, niter, eps, "end");
	}
	cgstep_close();

	if (pef) {
	    bb = allocatehelix (2);
	    aa = sf_floatalloc (3);
	    aa[0] = 1.;
	    bb->lag[0] = 1;
	    bb->lag[1] = 2;
	    bound(1, &nx, &nx, &three, bb); 
	    for (i=0; i < 3; i++) {
		find_pef (nx, mm, bb, 3);
		aa[1] = bb->flt[0];
		aa[2] = bb->flt[1];
		if (prec) {
		    polydiv_init (nx, bb);
		    solver_prec(int1_lop, cgstep, polydiv_lop, nx, nx, nd,
				mm, dd, niter, eps, "end");
		} else {
		    tcai1_init (3, aa);
		    solver_reg(int1_lop, cgstep, tcai1_lop, nx+filt, nx, nd, 
			       mm, dd, niter, eps, "end");
		}
		cgstep_close();
	    }
	}
	
    	sf_write (mm,sizeof(float),nx,out);
    }

    exit(0);
}

/* 	$Id: Minvbin1.c,v 1.6 2003/10/01 14:38:31 fomels Exp $	 */

