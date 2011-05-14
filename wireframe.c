/** \file
 * Wireframe drawing routine.
 */
#include "wireframe.h"
#include "draw.h"
#include <string.h>

void
wireframe_draw(
	const camera_t * camera,
	const int num_vertices,
	const vertex_t * vertices,
	const int num_edges,
	const edge_t * edges,
	pixel_t * pixels,
	pixel_t * temp
)
{
	static color24_t c = { 100, 100, 100 };
//	c.red += rand() % 8 - 3;
//	c.green += rand() % 8 - 3;
//	c.blue += rand() % 8 - 3;

	for (int i = 0 ; i < num_vertices ; i++)
		camera_project(camera, &vertices[i], &temp[i]);
	

	// Draw the edges by first drawing in black, then
	// in the color that the edge specifies
	for (int i = 0 ; i < num_edges ; i++)
	{
		const edge_t * const e = &edges[i];
		const pixel_t * old_p0 = &pixels[e->v0];
		const pixel_t * old_p1 = &pixels[e->v1];

		// Check to see if the pixel was behind us
		if (old_p0->x != -1 && old_p1->x != -1)
			draw_line(
				COLOR_BLACK24,
				old_p0->x, old_p0->y,
				old_p1->x, old_p1->y
			);

		const pixel_t * new_p0 = &temp[e->v0];
		const pixel_t * new_p1 = &temp[e->v1];

		if (new_p0->x != -1 && new_p1->x != -1)
			draw_line(
				c,
				new_p0->x, new_p0->y,
				new_p1->x, new_p1->y
			);
	}

	// Now copy the new locations to the permanent place
	memcpy(pixels, temp, num_vertices * sizeof(*pixels));
}
