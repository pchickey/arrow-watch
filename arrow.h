
#ifndef __ARROW_H__
#define __ARROW_H__

#ifndef EDGE_COLOR
#define EDGE_COLOR { 0, 0xFF, 0, 0 }
#endif

#include "coords.h"

/* Simple 3D Arrow. 
 *  Two copies of the same XY coordinates, at 20 and -20 in Z plane.
 *  XY plane verticies:
 *           2
 *  0       (*)
 * (*)      (*)1     
 *                 (*)3
 * (*)      (*)5    
 *  6       (*)
 *           4 
 *  (in xy, pts 0 through 6 same as pts 7 through 13)
 */

static const vertex_t vertices[] = {
{ {    20,    -60,      20 } }, // 0
{ {    20,     20,      20 } }, // 1
{ {    35,     20,      20 } }, // 2
{ {     0,     60,      20 } }, // 3
{ {   -35,     20,      20 } }, // 4
{ {   -20,     20,      20 } }, // 5
{ {   -20,    -60,      20 } }, // 6
{ {    20,    -60,     -20 } }, // 7
{ {    20,     20,     -20 } }, // 8
{ {    35,     20,     -20 } }, // 9
{ {     0,     60,     -20 } }, // 10
{ {   -35,     20,     -20 } }, // 11
{ {   -20,     20,     -20 } }, // 12
{ {   -20,    -60,     -20 } }  // 13
};
static const edge_t edges[] = {
{   0,   1 }, /* perimeter */
{   1,   2 },
{   2,   3 },
{   3,   4 },
{   4,   5 },
{   5,   6 },
{   6,   0 },
{   7,   8 }, /* perimeter */
{   8,   9 }, 
{   9,   10 }, 
{   10,  11 }, 
{   11,   12 },
{   12,   13 },
{   13,   7 }, 
{   0,   7 }, /* verticals */
{   1,   8 }, 
{   2,   9 }, 
{   3,   10 }, 
{   4,   11 }, 
{   5,   12 }, 
{   6,   13}
};

#endif // __ARROW_H__

