#include <stdio.h>
#include <ctype.h>

int main() {

	int c1 = getchar();
	int c2;

	while (c1 != EOF) {

		c2 = getchar();

		if (c2 == EOF) {
			printf("%c", c1);
			break;
		} else if (islower(c1) && islower(c2) && ((c1 % 7) == (c2 % 7))) {
			printf("%c%c", c2, c1);
			c1 = getchar();
		} else {
			printf("%c", c1);
			c1 = c2;
		}
	}
}
