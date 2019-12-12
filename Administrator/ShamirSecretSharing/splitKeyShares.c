#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sss/sss.h"
#include "sss/randombytes.h"
#include <assert.h>

int main(int argc, char *argv[])
{
	// Receives the number of shares to create and the defined threshold
	int numberOfShares = atoi(argv[1]), thershold = atoi(argv[2]); 
	FILE* fp; char filename[20];

	uint8_t data[sss_MLEN] = {0};
	sss_Share shares[numberOfShares];

	int i = 0;

    fp = fopen("pass.txt", "rb");
    do {
    	char c = fgetc(fp);
    	if (feof(fp) || c == '\n') break;
    	data[i++] = (uint8_t) c;
    } while(1);
    fclose(fp);

    printf("\n");
    for (i = 0; i < sss_MLEN; ++i)
    	printf("data[%d] %d\n", i, data[i]);
    printf("\n");

    // Split the secret into $numberOfShares shares (with a recombination theshold of $thershold)
    sss_create_shares(shares, data, numberOfShares, thershold);

    for (i = 0; i < numberOfShares; i++) {
		snprintf(filename, sizeof(filename), "share%d.txt", i+1);
		fp = fopen(filename, "wb");
		for (int j = 0; j < sss_SHARE_LEN; j++)
			fputc((uint8_t)shares[i][j], fp);
		fclose(fp);
	}
	
	return 0;
}