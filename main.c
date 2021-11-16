#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "xm.h"

int main(int argc,char *argv[]) {
	XM_Header *xm_h;
	FILE *fp;

	printf("header size: %ld\n", sizeof(XM_Header));

	if(argc < 2) {
		printf("%s: filename\n",argv[0]);
		return 0;
	}

	fp = fopen(argv[1],"rb");
	if(fp == NULL) {
		printf("fp null\n");
		return -1;
	}

	xm_h = malloc(sizeof(XM_Header));
	if(xm_h == NULL) return -2;
	fread(xm_h, 1, sizeof(XM_Header), fp);
	
	printf("Name: %.20s\n", xm_h->name);
	printf("Tracker Name: %.20s\n", xm_h->tracker_name);
	
	printf("Pattern ordering:\n");
	for(int i=0; i<xm_h->song_length; i++) {
		printf("%d",xm_h->pattern_order[i]);
		if(i < xm_h->song_length) printf("->");
	}
	printf("\n");
	
	// Get Patterns
	// Try to read n patterns from data
	// Preallocate spaces
	XM_Pattern_Data patterns[xm_h->num_patterns]; // Pre-alloc num_patters
	for(int i=0; i<xm_h->num_patterns; i++) {
		fread(&patterns[i].pattern_header,1,sizeof(XM_Pattern_Header),fp);
		printf("Pattern %d: Size %04X Row %X\n",i,patterns[i].pattern_header.packed_patterndata_size, patterns[i].pattern_header.num_rows);
		XM_Pattern_Header *header = &patterns[i].pattern_header;
		if(header->packed_patterndata_size == 0) continue;
		// Alloate row_data
		patterns[i].pattern_data = malloc(header->num_rows * xm_h->num_channels * sizeof(row_data_t));
		for(int row=0; row<header->num_rows; row++) {
			//patterns[i].pattern_data = (sizeof(pattern_t) * header->num_rows);
			for(int chnl=0; chnl<xm_h->num_channels; chnl++) {
				//patterns[i].pattern_data[row][chnl] = malloc(sizeof(row_data_t))
				row_data_t *cur_row_ch = &patterns[i].pattern_data[row + chnl];
				
				uint8_t n;
				fread(&n,1,1,fp);
				printf("Row %d chnl %d, packet %02X: ",row,chnl,n);
				if(n & 0x80) {
					if(n == 0x80) {
						printf("None");
					} else {
						// packed; parse 
						if(n & 0x01) {
							printf("Note:");
							fread(&cur_row_ch->note,1,1,fp);
							printf("%d -> %s%d, ",cur_row_ch->note,note[cur_row_ch->note%12],cur_row_ch->note/12);
							// Next byte is a note.
							// Parse Note
							//fseek(fp,1,SEEK_CUR);
						}
						if(n & 0x02) {
							printf("Instr:");
							fread(&cur_row_ch->instrument,1,1,fp);
							printf("%d, ",cur_row_ch->instrument);
							//fseek(fp,1,SEEK_CUR);
						}
						if(n & 0x04) {
							printf("Volume ");
							fread(&cur_row_ch->volume_column,1,1,fp);
							// Volume command have some special meaning!
							
						}
						if(n & 0x08) {
							printf("Effect Type,");
							//fseek(fp,1,SEEK_CUR);
							fread(&cur_row_ch->effect_type,1,1,fp);
						}
						if(n & 0x10) {
							printf("Effect Value,");
							//fseek(fp,1,SEEK_CUR);
							fread(&cur_row_ch->effect_parameter,1,1,fp);
						}
					}
				} else {
					cur_row_ch->note = n;
					fread(&cur_row_ch->instrument,1,1,fp);
					fread(&cur_row_ch->volume_column,1,1,fp);
					fread(&cur_row_ch->effect_type,1,1,fp);
					fread(&cur_row_ch->effect_parameter,1,1,fp);
					printf("MOD, Note %d -> %s%d, Volume, Et, Ep",n,note[n%12],n/12);

					//fseek(fp,4,SEEK_CUR);
					// it's MOD format. read 4 more byte.
				}
			printf("\n");
			}
			//printf("\n");
		}
		//fseek(fp,patterns[i].pattern_header.packed_patterndata_size,SEEK_CUR);
		// Read pattern header length
	}
	



	free(xm_h);
	return 0;
}
