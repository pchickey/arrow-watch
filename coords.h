/** \file
 * Coordinate systems used in the wireframe and camera framework.
 */
#ifndef _coords_h_
#define _coords_h_

#include <stdint.h>
#include <pulse_types.h>


typedef struct
{
	int8_t x[3];
} __attribute__((__packed__))
vertex_t;


typedef struct
{
	uint8_t v0;
	uint8_t v1;
} __attribute__((__packed__))
edge_t;


typedef struct
{
	uint8_t x;
	uint8_t y;
	//int8_t z; // depth, right now -1 == behind the camera, 1 == infront
} __attribute__((__packed__))
pixel_t;

#endif
