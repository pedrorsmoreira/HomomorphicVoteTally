#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "sss/sss.h"
#include "sss/randombytes.h"

int main(int argc, char *argv[])
{
	// Receives the number of shares to create and the defined threshold
	uint8_t numberOfShares = atoi(argv[1]), thershold = atoi(argv[2]); 
	FILE* fp; char filename[20];

	unsigned char data[sss_MLEN+1];
	sss_Share shares[numberOfShares];

	fp = fopen("password.txt", "rb");
	for (int i = 0; i < sss_MLEN; ++i)
		data[i] = fgetc(fp);
	fclose(fp);
	data[sss_MLEN]='\0';

     // Split the secret into $numberOfShares shares (with a recombination theshold of $thershold)
    sss_create_shares(shares, data, numberOfShares, thershold);

    for (int i = 0; i < numberOfShares; i++) {
		snprintf(filename, sizeof(filename), "share%d.txt", i+1);
		fp = fopen(filename, "w");
		for (int j = 0; j < sss_SHARE_LEN; j++)
			fputc(shares[i][j], fp);
		fputc('\0', fp);
		fclose(fp);
	}

	return 0;
}
