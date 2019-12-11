#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sss/sss.h"
#include "sss/randombytes.h"
#include <assert.h>

int main(int argc, char *argv[]) {
	// Recives in argv[1] number of shares to create and argv[2] the threshold

	int n_shares = atoi(argv[1]);
	int thershold = atoi(argv[2]);
	char pass[100];
	char fileName[20];
	FILE *fp;
	
	//rb - non text files
	fp = fopen("../pass.txt", "rb");
	//64 bits pass
	for(int i = 0, i < 64, i++)
		pass[w] = fgetc(fp);
	pass[64]='\0';
	fclose(fp);

	sss_Share shares[n_shares];
	
	// Split the secret into shares
	sss_create_shares(shares, pass, n_shares, thershold);

	for (int i = 0; i < n_shares; i++) 
	{
		snprintf(fileName, sizeof(fileName), "share%d.txt", i+1);

		fp = fopen(fileName, "w");

		for (int j = 0; j< sss_SHARE_LEN; j++)
			fputc(shares[i][j], fp);
		fputc('\0', fp);
		fclose(fp);
	}	
	return 0;
}