/* arrowwatch.c.
 * Based on 'teatime' code by Trammell Hudson
 * Modifications by Pat Hickey, May 2011
 */
#include <pulse_os.h>
#include <pulse_types.h>
#include <app_resources.h>
#include "sin_table.h"
#include "draw.h"
#include "mathutil.h"
#include "camera.h"
#include "wireframe.h"
#include "umpl_interface.h"

static uint32_t not_charging;

static uint8_t last_min;
static uint8_t last_sec;
static uint8_t shutdown_scheduled;

static camera_t camera;

// Bring in a model to render
#include "arrow.h"

#define ARRAY_COUNT(x) (sizeof(x) / sizeof(*x))
static pixel_t pixels[ARRAY_COUNT(vertices)];

static int8_t default_m[9];
static int8_t m1[9];
static int8_t m2[9];
static int8_t m3[9];

static int8_t *rmats[4];
static int8_t selected_mat;


void
main_app_loop(void)
{
    uint8_t umpl_got;
	const uint64_t millis = pulse_get_millis();

        // If the not-charging state has not been toggled in the last
        // 30 seconds, schedule a power down soon.
        if (not_charging && millis - not_charging > 60000)
	{
		if (!shutdown_scheduled)
			pulse_update_power_down_timer(1);
		shutdown_scheduled = 1;
		return;
	}

    /* The umpl_input_handler is nonblocking.
     * It will read a packet off the debug serial
     * port, and copy the rotation matrix payload 
     * to the provided buffer.
     */

    umpl_got = umpl_input_handler(default_m);

    if (umpl_got)
        dbg_putc((char)0); // (for debugging)

	camera_setup_rmat(&camera, 4000, rmats[selected_mat]);


#if 1
	pixel_t temp_pixels[ARRAY_COUNT(pixels)];
	wireframe_draw(
		&camera,
		ARRAY_COUNT(vertices),
		vertices,
		ARRAY_COUNT(edges),
		edges,
		pixels,
		temp_pixels
	);
#else
	for (int i = 0 ; i < ARRAY_COUNT(vertices) ; i++)
	{
		const vertex_t * const v = &vertices[i];
		pixel_t * const p = &pixels[i];
		draw_pixel(COLOR_BLACK24, p->x, p->y);
		camera_project(&camera, v, p);
		draw_pixel(COLOR_GREEN, p->x, p->y);
	}
#endif

	// Update the clock once per minute
	struct pulse_time_tm now;
	pulse_get_time_date(&now);
	if (last_sec == now.tm_sec)
		return;
	last_sec = now.tm_sec;
	if (last_min != now.tm_min)
		pulse_blank_canvas();
	last_min = now.tm_min;

	char buf[16];
	sprintf(buf, "%02d:%02d",
		now.tm_hour,
		now.tm_min
	);

	draw_monostring(8, VSCREEN_HEIGHT - FONT_HEIGHT - 1, COLOR_GREEN, buf);
}


static void
wakeup(void *arg)
{
	pulse_oled_set_brightness(100);
	pulse_blank_canvas();
	not_charging = pulse_get_millis();
	shutdown_scheduled = 0;
	//srand(pulse_get_millis());
}


void
main_app_init(void)
{
	// Register a callback for the woke_from_button event
	pulse_register_callback(
		ACTION_WOKE_FROM_BUTTON,
		&wakeup
	);

	wakeup(0);

    // identity matrix to start
    // we'll overwrite this matrix from the
    // umpl input.
    default_m[0] = 128;
    default_m[1] = 0;
    default_m[2] = 0;
    default_m[3] = 0;
    default_m[4] = 128;
    default_m[5] = 0;
    default_m[6] = 0;
    default_m[7] = 0;
    default_m[8] = 128;

    // generated by hand. m1,2,3 won't change.
    m1[0] = -118;
    m1[1] = 9;
    m1[2] = 48;
    m1[3] = -48;
    m1[4] = -115;
    m1[5] = -23;
    m1[6] = 0;
    m1[7] = -25;
    m1[8] = 125;

    m2[0] = 88;
    m2[1] = -17;
    m2[2] = 90;
    m2[3] = -88;
    m2[4] = -90;
    m2[5] = 17;
    m2[6] = 25;
    m2[7] = 0;
    m2[8] = 125;

    m3[0] = 110;
    m3[1] = 31;
    m3[2] = 55;
    m3[3] = -63;
    m3[4] =  96;
    m3[5] = 55;
    m3[6] = 0;
    m3[7] = -63;
    m3[8] = 110;

    // You can cycle through a few differnt arrows.
    // (This is just to test the display code 
    // without a sensor attached)
    rmats[0] = default_m;
    rmats[1] = m1;
    rmats[2] = m2;
    rmats[3] = m3;
    selected_mat = 0;
}


void
main_app_handle_button_down(void)
{
	last_min = last_sec = -1;

    // cycle through rotation matricies.
    if (++selected_mat == 4)
        selected_mat = 0;
}


void
main_app_handle_button_up(void)
{
}


void
main_app_handle_hardware_update(
	enum PulseHardwareEvent event
)
{
	switch (event)
	{
	case BATTERY_CHARGING:
		// Erase the last non-charging time to avoid flakey power
		not_charging = 0;
		break;

	case BATTERY_NOT_CHARGING:
		// Record the time that the state was toggled.
		not_charging = pulse_get_millis();
		break;

	default:
		break;
	}
}


void
main_app_handle_doz(void)
{
	// Fade out
	for (int i = 100; i >= 0; i -= 6)
	{
		pulse_oled_set_brightness(i);
		pulse_mdelay(60);
	}
}
