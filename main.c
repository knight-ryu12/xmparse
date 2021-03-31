#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "xm.h"

int main() {
	XM_Header *xm_h;
	FILE *fp;

	printf("header size: %ld\n", sizeof(XM_Header));

	fp = fopen("db_s1_r.xm","rb");
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
		printf("%X->",xm_h->pattern_order[i]);
	}
	printf("\n");
	


	free(xm_h);
	return 0;
}
