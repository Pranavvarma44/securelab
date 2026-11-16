
#include <stdio.h>
#include <string.h>

int main(void) {
    char buffer[16];

    // First experiment: short input
    const char input[] = "THIS_INPUT_IS_LONGER_THAN_SIXTEEN";

    printf("Copying %zu bytes into a 16-byte buffer...\n",
           strlen(input) + 1);

    memcpy(buffer, input, strlen(input) + 1);

    printf("Program reached the end of the copy.\n");

    return 0;
}
