#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hypercomplex.h"

// Two-dimensional hypercomplex numbers.
//
// Each number is of the form a + b*u, where
// u*u == a for some real number a.
//
// Examples:
//
// If u*u == -1, we get the complex numbers.
// If u*u == 0, we get the dual numbers.
// If u*u == 1, we get the split numbers.
//
// Since we can't compute with reals, we will represent
// coordinates as doubles.

// Construct a hypercomplex number a+b*u from the given coordinates.
// Argument u2 gives the value of u*u.
// The effect of operations on numbers with different values of u2
// is undefined.
struct h hPack(double a, double b, double u2) {
	struct h h1;
	h1.a = a;
	h1.b = b;
	h1.u2 = u2;

	return h1;
}

// Return the coordinates of a hypercomplex number
// through the given pointers.
void hUnpack(struct h z, double* a, double* b) {
	*a = z.a;
	*b = z.b;
}

// Return z1 + z2.
struct h hAdd(struct h z1, struct h z2) {
	
	double a1;
	double a2;
	double b1;
	double b2;

	hUnpack(z1, &a1, &b1);
	hUnpack(z2, &a2, &b2);

	return hPack(a1 + a2, b1 + b2, z1.u2);
}

// Return z1 - z2.
struct h hSubtract(struct h z1, struct h z2) {
	double a1;
	double a2;
	double b1;
	double b2;

	hUnpack(z1, &a1, &b1);
	hUnpack(z2, &a2, &b2);
	
	return hPack(a1 - a2, b1 - b2, z1.u2);
}

// Return z1 * z2.
struct h hMultiply(struct h z1, struct h z2) {
	double a1;
	double a2;
	double b1;
	double b2;

	hUnpack(z1, &a1, &b1);
	hUnpack(z2, &a2, &b2);

	return hPack(a1 * a2 +	b1 * b2 * z1.u2, b1 * a2 + a1 * b2, z1.u2);
}

// Given z == a + b*u, return its conjugate a - b*u.
struct h hConjugate(struct h z) {
	double a1;
	double b1;

	hUnpack(z, &a1, &b1);
	return hPack(a1, 0 - b1, z.u2);
}

// Return q such that q*z2 == z1.
// If z2 is a zero divisor, behavior is undefined.
struct h hDivide(struct h z1, struct h z2) {
	
	struct h numerator = hMultiply(hConjugate(z2), z1);
	struct h denom = hMultiply(hConjugate(z2), z2);

	double aN;
	double aD;
	double bN;
	double bD;

	hUnpack(numerator, &aN, &bN);
	hUnpack(denom, &aD, &bD);

	struct h fin = hPack(aN/aD, bN/aD, z1.u2);
	return fin;

}