#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sss/sss.h"
#include "sss/randombytes.h"
//#include <assert.h>

int main(int argc, char *argv[])
{
	uint8_t data[sss_MLEN];
	int numberOfShares = atoi(argv[1]); // Receives the number of shares to create
	int thershold = atoi(argv[2]); // Receives the defined threshold
	char fileName[20];
	sss_Share shares[numberOfShares];
	
	//rb - non text files
	fp = fopen("./pass.txt", "rb");
	//64 bytess pass
	for(int i = 0; i < sss_MLEN; i++)
		data[i] = fgetc(fp);
	data[64] = '\0';
	fclose(fp);

	// Split the secret into shares
	sss_create_shares(shares, data, numberOfShares, thershold);

	for (int i = 0; i < numberOfShares; i++) {
		snprintf(fileName, sizeof(fileName), "share%d.txt", i+1);
		fp = fopen(fileName, "wb");
		for (int j = 0; j< sss_SHARE_LEN; j++)
			fputc(shares[i][j], fp);
		fputc('\0', fp);
		fclose(fp);
	}

	return 0;
}