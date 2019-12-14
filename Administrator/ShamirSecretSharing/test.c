#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sss/sss.h"
#include "sss/randombytes.h"
#include <assert.h>

#define num_shares 5
#define shares_threshold 4

/*
int main(int argc, char *argv[])
{
	uint8_t data[sss_MLEN] = {0}, restored[sss_MLEN] = {0};
	FILE *fp;
	char filename[20];
	sss_Share shares2[num_shares];
	sss_Share x;
	int j = 0;

	for (int i = 0; i < num_shares; i++) {
		snprintf(filename, sizeof(filename), "share%d.txt", i+1);
		fp = fopen(filename, "rb");
		j = 0;
		do {
			char c = fgetc(fp);
			*x = c;
			if (feof(fp) || c == '\n') break;
			shares2[i][j++] = *x;
		} while(1);
	}

	// PRINTS
	j=0;
	fp = fopen("password.txt", "rb");
    do {
    	char c = fgetc(fp); 
    	if (feof(fp) || c == '\n') break;
    	data[j++] = (uint8_t) c;
    } while(1);
    fclose(fp);

	printf("\n");
    for (j = 0; j < sss_MLEN; ++j)
    	printf("data[%d] %d\n", j, data[j]);
    printf("\n");
    // PRINTS


	int tmp;
	// Combine some of the shares to restore the original secret
	tmp = sss_combine_shares(restored, shares2, shares_threshold);

	// PRINTS
	printf("\n");
    for (j = 0; j < sss_MLEN; ++j)
    	printf("restored[%d] %d\n", j, restored[j]);
    printf("\n");
    // PRINTS

	assert(tmp == 0);
	assert(memcmp(restored, data, sss_MLEN) == 0);
	
	printf("sss_MLEN %ld\n", sss_MLEN);

	// Escreve num ficheiro a pass dps de juntar as shares
	fp = fopen("recovered_password.txt", "wb");
	for (j = 0;j< sss_MLEN;j++) {
		fputc(restored[j], fp);
	}
	fputc('\0', fp);
	fclose(fp);

}*/

int main(int argc, char *argv[])
{
	unsigned char data[130], restored[130], c;
	FILE *fp;
	char filename[20];
	sss_Share shares2[num_shares];
	int j = 0;

	for (int i = 0; i < num_shares; i++) {
		snprintf(filename, sizeof(filename), "share%d.txt", i+1);
		fp = fopen(filename, "rb");
		j = 0;

		while (j < sss_SHARE_LEN) {
			c = fgetc(fp);
			shares2[i][j++] = c;
		}
		fclose(fp);
	}

	int tmp;
	// Combine some of the shares to restore the original secret
	tmp = sss_combine_shares(restored, shares2, shares_threshold);

	// PRINTS
	int i;
	fp = fopen("password.txt", "rb");	
    do {
    	c = fgetc(fp);
    	if ((c = fgetc(fp)) == EOF) break;
    	data[i++] = fgetc(fp);
    } while(i < sss_MLEN);
    fclose(fp);

    data[i]='\0';

    printf("\n");
    for (j = 0; j < sss_MLEN; ++j)
    	printf("data[%d] %d\n", j, data[j]);
    printf("\n\n");
    for (j = 0; j < sss_MLEN; ++j)
    	printf("restored[%d] %d\n", j, restored[j]);
    printf("\n");
    // PRINTS

	assert(tmp == 0);
	//assert(memcmp(restored, data, sss_MLEN) == 0);

	// Escreve num ficheiro a pass dps de juntar as shares
	fp = fopen("recovered_password.txt", "wb");
	for (j = 0; j < sss_MLEN; j++) {
		fputc(restored[j], fp);
	}
	fputc('\0', fp);
	fclose(fp);
}