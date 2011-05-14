/** \file
 * Line and pixel drawing routines.
 * Using a virtual screen buffer that is twice the resolution
 * along each axis so that anti-aliasing can be done later.
 */
#ifndef _draw_h_

#include <pulse_os.h>
#include <pulse_types.h>
#include "app_resources.h"

#define VSCREEN_SHIFT	1
#define VSCREEN_WIDTH	(((uint32_t) SCREEN_WIDTH) << VSCREEN_SHIFT)
#define VSCREEN_HEIGHT	(((uint32_t) SCREEN_HEIGHT) << VSCREEN_SHIFT)

/** Predefined colors that we use */
#define COLOR_GRAY		((color24_t) { 0x18, 0x18, 0x18, 0 } )
#define COLOR_RED		((color24_t) { 0xFF, 0x00, 0x00, 0 } )
#define COLOR_GREEN		((color24_t) { 0x00, 0xFF, 0x00, 0 } )
#define COLOR_DARKGREEN		((color24_t) { 0x00, 0x10, 0x00, 0 } )
#define COLOR_YELLOW		((color24_t) { 0x10, 0x10, 0x00, 0 } )
#define COLOR_MIDNIGHTBLUE	((color24_t) { 0x19, 0x19, 0x70, 0 } )
#define COLOR_BLUE		((color24_t) { 0x80, 0x90, 0x00, 0 } )
#define COLOR_WHITE		((color24_t) { 0xFF, 0xFF, 0xFF, 0 } )
#define COLOR_BLACK		((color24_t) { 0x00, 0x00, 0x00, 0 } )

/** For the Clocktopia font */
#define FONT_WIDTH (8 << VSCREEN_SHIFT)
#define FONT_HEIGHT (32 << VSCREEN_SHIFT)
#define FONT FONT_CLOCKOPIA_32



/* Draw a pixel in vscreen coordinates */
void
draw_pixel(
	color24_t c,
	uint32_t x,
	uint32_t y
);

/* Draw a non-antialised line using Bresenham's_line_algorithm
 * in vscreen coordinates */
void
draw_line(
	color24_t c,
	uint32_t x0,
	uint32_t y0,
	uint32_t x1,
	uint32_t y1
);


void
draw_monostring(
	uint32_t x,
	uint32_t y,
	color24_t color,
	const char * s
);

#endif
