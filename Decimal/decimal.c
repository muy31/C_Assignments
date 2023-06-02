#include <stdio.h>
#include "decimal.h"

int decimalAdd(size_t precision, char augend[], const char addend[]) {

	char preerror[precision];
	int carry = 0;
	int bad_digit = 0;

	for (size_t i = 0; i < precision; i++) {

		preerror[i] = augend[i];

		if (preerror[i] >= 0 && preerror[i] <= 9 && addend[i] >= 0 && addend[i] <= 9) {
			char nextDigit;

			if (carry == 0) {
				nextDigit = preerror[i] + addend[i];
			}
			else {
				nextDigit = preerror[i] + addend[i] + 1;
				carry = 0;
			}
			//Check if larger than 9, and set carry to true
			if (nextDigit > 9) {
				nextDigit -= 10;
				carry = 1;
			}
			augend[i] = nextDigit;
		}
		else {
			bad_digit = 1;
		}
	}

	if (bad_digit == 1) {
		for (size_t i = 0; i < precision; i++) {
			augend[i] = preerror[i];
		}
		return DECIMAL_BAD_DIGIT;
	}
	else if (carry == 1) {
		for (size_t i = 0; i < precision; i++) {
			augend[i] = preerror[i];
		}
		return DECIMAL_OVERFLOW;
	}
	else {
		return DECIMAL_OK;
	}
}

void decimalPrint(size_t precision, const char number[]) {
	int numberStarted = 0;

	for (size_t i = precision; i > 0; i--) {
		if (number[i - 1] != 0 || numberStarted == 1) {
			if (number[i - 1] >= 0 && number[i - 1] <= 9) {
				putchar(number[i - 1] + '0');
				numberStarted = 1;
			}
			else {
				printf(DECIMAL_BAD_OUTPUT);
				return;
			}
		}
	}

	//Print out zero if nothing was ever printed
	if (numberStarted == 0) {
		putchar('0');
	}
}


int decimalSubtract(size_t precision, char minuend[], const char subtrahend[]) {
	int regroup = 0;
	char preerror[precision];
	int bad_digit = 0;

	for (size_t index = 0; index < precision; index++) {

		preerror[index] = minuend[index];
		

		if (preerror[index] >= 0 && preerror[index] <= 9 && subtrahend[index] >= 0 && subtrahend[index] <= 9) {
			char newDigit;

			if (regroup) {
				newDigit = minuend[index] - subtrahend[index] - 1;
				regroup = 0;
			}
			else {
				newDigit = minuend[index] - subtrahend[index];
			}

			if (newDigit < 0) {
				newDigit += 10;
				regroup = 1;
			}

			minuend[index] = newDigit;

		}
		else {
			bad_digit = 1;
		}
		
		
	}

	if (bad_digit) {
		for (size_t i = 0; i < precision; i++) {
			minuend[i] = preerror[i];
		}
		return DECIMAL_BAD_DIGIT;
	} else	if (regroup) {
		for (size_t i = 0; i < precision; i++) {
			minuend[i] = preerror[i];
		}
		return DECIMAL_OVERFLOW;
	}
	else {
		return DECIMAL_OK;
	}
}

