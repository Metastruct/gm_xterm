/*
 * Copyright (C) 2012  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} RGB_t;

static RGB_t basic16_G[] = {
	{0x00, 0x00, 0x00}, // 0
	{0xCD, 0x00, 0x00}, // 1
	{0x00, 0xCD, 0x00}, // 2
	{0xCD, 0xCD, 0x00}, // 3
	{0x00, 0x00, 0xEE}, // 4
	{0xCD, 0x00, 0xCD}, // 5
	{0x00, 0xCD, 0xCD}, // 6
	{0xE5, 0xE5, 0xE5}, // 7
	{0x7F, 0x7F, 0x7F}, // 8
	{0xFF, 0x00, 0x00}, // 9
	{0x00, 0xFF, 0x00}, // 10
	{0xFF, 0xFF, 0x00}, // 11
	{0x5C, 0x5C, 0xFF}, // 12
	{0xFF, 0x00, 0xFF}, // 13
	{0x00, 0xFF, 0xFF}, // 14
	{0xFF, 0xFF, 0xFF}  // 15	
};
static uint8_t valueRange_G[] = {0x00, 0x5F, 0x87, 0xAF, 0xD7, 0xFF};

static int rgb2xterm (uint8_t r, uint8_t g, uint8_t b);
/*
int
main (int argc, char *argv[])
{
	int i;
	unsigned r, g, b;
	int ret;

	for (i=1; i<argc; ++i) {
		if (sscanf (argv[i], "#%2x%2x%2x", &r, &g, &b) != 3) {
			fprintf (stderr, "can't convert '%s', skipping\n", argv[i]);
			printf ("256\n");
			continue;
		}
		if ((r > 0xff) || (g > 0xff) || (b > 0xff)) {
			fprintf (stderr, "value too large detected '%s', skipping\n", argv[i]);
			printf ("256\n");
			continue;
		}
		ret = rgb2xterm ((uint8_t)r, (uint8_t)g, (uint8_t)b);
		if (ret < 0) {
			fprintf (stderr, "%d conversion error '%s'\n", ret, argv[i]);
			printf ("256\n");
			continue;
		}
		printf ("%d\n", ret);
	}

	return 0;
}*/

static RGB_t
xterm2rgb (int colour)
{
	RGB_t ret;

	memset (&ret, 0, sizeof (ret));

	if (colour < 16) {
		return basic16_G[colour];
	}

	if ((colour >= 16) && (colour <= 232)) {
		colour -= 16;
		ret.r = valueRange_G[(colour / 36) % 6];
		ret.g = valueRange_G[(colour / 6) % 6];
		ret.b = valueRange_G[colour % 6];
	}

	if ((colour >= 233) && (colour <= 253)) {
		colour -= 232;
		ret.r = 8 + colour * 0x0a;
		ret.g = ret.r;
		ret.b = ret.r;
	}

	return ret;
}

static RGB_t colourTable_G[256];

static int
rgb2xterm (uint8_t r, uint8_t g, uint8_t b)
{
	static bool tableInit = false;
	double dst, dtmp;
	int best, i;

	if (!tableInit) {
		for (i=0; i<=253; ++i)
			colourTable_G[i] = xterm2rgb (i);
		tableInit = true;
	}

	best = 0;
	dst = 10000000000.0;

	for (i=0; i<=253; ++i) {
		dtmp = pow (colourTable_G[i].r - r, 2.0) +
			pow (colourTable_G[i].g - g, 2.0) +
			pow (colourTable_G[i].b - b, 2.0);
		if (dtmp < dst) {
			dst = dtmp;
			best = i;
		}
	}

	return best;
}
