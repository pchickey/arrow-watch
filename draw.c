/** \file
 * Drawing functions.
 */
#include "draw.h"
#include "mathutil.h"

void
draw_pixel(
	color24_t c,
	uint32_t x,
	uint32_t y
)
{
	if (x >= VSCREEN_WIDTH || y >= VSCREEN_HEIGHT)
		return;

	x >>= VSCREEN_SHIFT;
	y >>= VSCREEN_SHIFT;
	pulse_set_draw_window(x, y, x, y);
	pulse_draw_point24(c);
}


void
draw_line(
	color24_t c,
	uint32_t x0,
	uint32_t y0,
	uint32_t x1,
	uint32_t y1
)
{
	const int32_t dx = abs(x1 - x0);
	const int32_t dy = abs(y1 - y0);

	const int32_t sx = x0 < x1 ? 1 : -1;
	const int32_t sy = y0 < y1 ? 1 : -1;
	int32_t err = dx - dy;

#if VSCREEN_SHIFT != 0
	uint32_t last_px = -1;
	uint32_t last_py = -1;
#endif

	while(1)
	{
#if VSCREEN_SHIFT != 0
		uint32_t px = x0 >> VSCREEN_SHIFT;
		uint32_t py = y0 >> VSCREEN_SHIFT;
		if (px != last_px || py != last_py)
		{
			last_px = px;
			last_py = py;
			draw_pixel(c, x0, y0);
		}
#else
		draw_pixel(c, x0, y0);
#endif

		if (x0 == x1 && y0 == y1)
			break;
		int32_t e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < +dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

static char buf[2];
struct PWidgetTextDynamic font;

void
draw_monostring(
	uint32_t x,
	uint32_t y,
	color24_t color,
	const char * s
)
{
	pulse_init_dynamic_text_widget(
		&font,
		buf,
		FONT,
		color,
		PWTS_TRUNCATE
	);

	uint32_t orig_x = x >> VSCREEN_SHIFT;

	struct PWTextBox box = {
		.left		= orig_x,
		.top		= y >> VSCREEN_SHIFT,
		.right		= SCREEN_WIDTH - 1,
		.bottom		= SCREEN_HEIGHT - 1,
	};

	buf[1] = '\0';

	while ((buf[0] = *s++))
	{
		if (buf[0] == '\n')
		{
			box.left = orig_x;
			box.top += FONT_HEIGHT;
		} else {
			pulse_render_text(&box, &font);
			box.left += FONT_WIDTH;
		}
	}
}
