#pragma once
#include <stdint.h>

const char* note[] = {
	"C ", // 0
	"C+", // 1
	"D ", // 2
	"D+", // 3
	"E ", // 4
	"F ", // 5
	"F+", // 6
	"G ", // 7
	"G+", // 8
	"A ", // 9
	"A+", // 10
	"B "  // 11
};




typedef struct {
	uint8_t note; // 0-71 
	uint8_t instrument;
	uint8_t volume_column;
	uint8_t effect_type;
	uint8_t effect_parameter;
} __attribute((packed)) row_data_t;

typedef struct {
	uint32_t header_length; // 9
	uint8_t packing_type; // Always 0
	uint16_t num_rows; //1..256 (0 = 1)
	uint16_t packed_patterndata_size;
	row_data_t *pattern_data;
} __attribute__((packed)) XM_Pattern;

typedef struct {
	char ID[17]; // 'Extended Module: '
	char name[20]; // Module Name 0-padded
	uint8_t padding; // 1a
	char tracker_name[20]; // Tracker Name, like "FastTracker II". 0-padded
	uint16_t version; // 01 03 = 1.3

	uint32_t header_size; // the byte below. it should be more than 21 byte.
	uint16_t song_length; // in Pattern Order. 0 = 1pattern.
	uint16_t restart_pos; // in Position. 1 = 0th pattern
	uint16_t num_channels; // 2 4 6 8..
	uint16_t num_patterns; // max 256
	uint16_t num_instruments; // 128
	uint16_t flags; // 0 = amiga 1 = linear
	uint16_t default_tempo;
	uint16_t default_bpm;
	uint8_t pattern_order[256];
} __attribute__((packed)) XM_Header;
