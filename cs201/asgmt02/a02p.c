// starter file for Assignment 2p

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// function for checking that your assembly code is computing the correct result
double quadraticRootC(double a, double b, double c)
{
	return (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
}

double quadraticRoot(double a, double b, double c)
{
    double root = 0;
	// write assembly code below to calculate the quadratic root
	asm
        (
		"fldl   %[a]              \n"  /* a                      */
                "fadd   %%ST(0)           \n"  /* 2a                     */
                "fldl   %[a]              \n"  /* a , 2a                 */
                "fldl   %[c]              \n"  /* c, a, 2a               */
                "fmulp  %%ST(1)           \n"  /* ac, 2a                 */
                "fadd   %%ST(0)           \n"  /* 2ac 3a                 */
                "fadd   %%ST(0)           \n"  /* 4ac, 2a                */
                "fchs                     \n"  /* -4ac, 2a               */
                "fldl   %[b]              \n"  /* b, -4ac, 2a            */
                "fldl   %[b]              \n"  /* b,b -4ac, 2a           */
                "fmulp  %%ST(1)           \n"  /* b*b, -4ac, 2a          */
                "faddp  %%ST(1)           \n"  /* b^2 - 4ac, 2a          */
                "fsqrt                    \n"  /* sqrt(b^2-4ac)          */
                "fldl   %[b]              \n"  /* b                      */
                "fchs                     \n"  /* change sign-b          */
                "faddp  %%ST(1)           \n"  /* -b + sqrt(b*b-4ac)     */
                "fdivp  %%ST(1)           \n"  /* -b + sqrt(b^2-4ac/2    */
                :"=t"(root)                    /* out,I  used '=t' to return top of floating */
                                               /* point stack which is where the result is   */
                                               /* Source: gcc online machine constraint docs */ 
                :[a]"m"(a),[b]"m"(b),[c]"m"(c) /* in                     */
                :"eax"                         /* clobber                */
	);

	return root;
}

// do not change this function, except to print your own name
int main(int argc, char **argv)
{
	double	a, b, c;
	double	root, rootC;

	printf("CS201 - Assignment 2p - Jonathon Sonesen\n");	// print your own name instead
	if (argc != 4) {
		printf("need 3 arguments: a, b, c\n");
		return -1;
		}
	a = atof(argv[1]);
	b = atof(argv[2]);
	c = atof(argv[3]);
	root = quadraticRoot(a, b, c);
	rootC = quadraticRootC(a, b, c);

	printf("quadraticRoot(%.3f, %.3f, %.3f) = %.3f, %.3f\n", a, b, c, root, rootC);
   
        
	return 0;
}
