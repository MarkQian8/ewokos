#include <graph/graph.h>
#include <sys/proc.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __cplusplus 
extern "C" { 
#endif

#define MIN(a, b) (((a) > (b))?(b):(a))

static inline void neon_alpha_8(uint32_t *b, uint32_t *f, uint32_t *d)
{
	__asm volatile(
		"vld4.8    {d20-d23},[%1]\n\t" // load foreground
		"vmov.u8	d28, 0xff\n\t"
		"vsub.u8	d28, d28, d23\n\t"
		"vmull.u8  q1,d20,d23\n\t"
		"vmull.u8  q3,d21,d23\n\t"
		"vmull.u8  q5,d22,d23\n\t"

		"vld4.8    {d24-d27},[%0]\n\t" // load background
		"vmov.u8	d29, 0xff\n\t"
		"vsub.u8	d29, d29, d27\n\t"
		"vmull.u8  q2,d24,d28\n\t"
		"vmull.u8  q4,d25,d28\n\t"
		"vmull.u8  q6,d26,d28\n\t" // apply alpha

		/*oa = oa + (255 - oa) * a / 255;*/
		"vmull.u8	q7, d29, d23\n\t"
		"vshr.u16  	q7,q7,#8\n\t"
		"vmov.u8	d29, 0x1\n\t"
		"vmull.u8	q8, d29, d27\n\t"
		"vadd.u16  	q7,q7,q8\n\t"

		"vadd.u16  q1,q1,q2\n\t"
		"vadd.u16  q3,q3,q4\n\t"
		"vadd.u16  q5,q5,q6\n\t"

		"vshr.u16  q1,q1,#8\n\t"
		"vshr.u16  q3,q3,#8\n\t"
		"vshr.u16  q5,q5,#8\n\t"

		"vmovn.u16 d20,q1\n\t"
		"vmovn.u16 d21,q3\n\t"
		"vmovn.u16 d22,q5\n\t"
		"vmovn.u16   d23,q7\n\t"

		"vst4.8   {d20-d23},[%2]\n\t"
		:
		: "r"(b), "r"(f), "r"(d)
		: "memory");
	return;
}

static inline void neon_8(uint32_t *s, uint32_t *d)
{
	__asm volatile(
		"vld4.8    {d20-d23},[%0]\n\t" // load foreground
		"vst4.8   {d20-d23},[%1]\n\t"
		:
		: "r"(s), "r"(d)
		: "memory");
	return;
}

static inline void neon_fill_load(uint32_t *s)
{
	__asm volatile(
		"vld4.8    {d20-d23},[%0]\n\t" // load foreground
		:
		: "r"(s)
		: "memory");
	return;
}


static inline void neon_fill_store(uint32_t *d)
{
	__asm volatile(
		"vst4.8   {d20-d23},[%0]\n\t"
		:
		: "r"(d)
		: "memory");
	return;
}

static inline void graph_pixel_argb_neon(graph_t *graph, int32_t x, int32_t y,
								  uint32_t *src, int size)
{
	uint32_t fg[8];
	uint32_t bg[8];
	uint32_t *dst = &graph->buffer[y * graph->w + x];

	if (size == 8)
	{
		neon_alpha_8(dst, src, dst);
	}
	else
	{
		memcpy(fg, src, 4 * size);
		memcpy(bg, dst, 4 * size);
		neon_alpha_8(bg, fg, bg);
		memcpy(dst, bg, 4 * size);
	}
}

static inline void graph_pixel_neon(graph_t *graph, int32_t x, int32_t y,
								  uint32_t *src, int size)
{
	uint32_t *dst = &graph->buffer[y * graph->w + x];

	if (size == 8)
	{
		neon_8(src, dst);
	}
	else
	{
		memcpy(dst, src, 4 * size);
	}
}

static int32_t insect(graph_t* src, grect_t* sr, graph_t* dst, grect_t* dr) {
	int32_t dx = sr->x < dr->x ? sr->x:dr->x;
	int32_t dy = sr->y < dr->y ? sr->y:dr->y;

	if(dx < 0) {
		sr->x -= dx;
		dr->x -= dx;
		sr->w += dx;
		dr->w += dx;
	}

	if(dy < 0) {
		sr->y -= dy;
		dr->y -= dy;
		sr->h += dy;
		dr->h += dy;
	}

	//insect src;
	if(!graph_insect(src, sr))
		return 0;
	if(!graph_insect(dst, dr))
		return 0;

	if(sr->w <= 0 || sr->h <= 0 ||
			dr->w <= 0 || dr->h <= 0)
		return 0;

	if(sr->w > dr->w)
		sr->w = dr->w;
	else
		dr->w = sr->w;
	
	if(sr->h > dr->h)
		sr->h = dr->h;
	else
		dr->h = sr->h;
	return 1;
}

void graph_fill_neon(graph_t* g, int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color) {
	uint32_t buf[8];

	if(g == NULL || w <= 0 || h <= 0)
		return;
	grect_t r = {x, y, w, h};
	if(!graph_insect(g, &r))
		return;

	register int32_t ex, ey;
	y = r.y;
	ex = r.x + r.w;
	ey = r.y + r.h;

	for(int i = 0; i < 8; i++)
		buf[i] = color;
	
	schd_core_lock();
	if(color_a(color) == 0xff) {
		neon_fill_load(buf);
		for(; y < ey; y++) {
			x = r.x;
			for(; x < ex; x+=8) {
				uint32_t *dst = &g->buffer[y * g->w + x];
				int pixels = ex -x;
				if(pixels >= 8)
					neon_fill_store(dst);
				else
					memcpy(dst, buf, pixels * 4);
			}
		}
	}
	else {
		for(; y < ey; y++) {
			x = r.x;
			for(; x < ex; x+=8) {
				graph_pixel_argb_neon(g, x, y, buf, MIN(ex-x, 8));
			}
		}
	}
	schd_core_unlock();
}

void graph_fill_3d_neon(graph_t* g, int x, int y, int w, int h, uint32_t color, bool rev) {
	uint32_t dark, bright;
	if(rev)
		graph_get_3d_color(color, &bright, &dark);
	else
		graph_get_3d_color(color, &dark, &bright);

	graph_fill_neon(g, x+1, y+1, w-2, h-2, color);
	graph_box_3d(g, x, y, w, h, bright, dark);
}


inline void graph_blt_neon(graph_t* src, int32_t sx, int32_t sy, int32_t sw, int32_t sh,
		graph_t* dst, int32_t dx, int32_t dy, int32_t dw, int32_t dh) {
	
	if(sw <= 0 || sh <= 0 || dw <= 0 || dh <= 0)
		return;

	grect_t sr = {sx, sy, sw, sh};
	grect_t dr = {dx, dy, dw, dh};
	graph_insect(dst, &dr);
	if(!insect(src, &sr, dst, &dr))
		return;

	register int32_t ex, ey;
	sy = sr.y;
	dy = dr.y;
	ex = sr.x + sr.w;
	ey = sr.y + sr.h;

	schd_core_lock();
	for(; sy < ey; sy++, dy++) {
		register int32_t sx = sr.x;
		register int32_t dx = dr.x;
		register int32_t offset = sy * src->w;
		for(; sx < ex; sx+=8, dx+=8) {
			graph_pixel_neon(dst, dx, dy, &src->buffer[offset + sx], MIN(ex-sx, 8));	
		}
	}
	schd_core_unlock();
}

inline void graph_blt_alpha_neon(graph_t* src, int32_t sx, int32_t sy, int32_t sw, int32_t sh,
		graph_t* dst, int32_t dx, int32_t dy, int32_t dw, int32_t dh, uint8_t alpha) {
	if(sw <= 0 || sh <= 0 || dw <= 0 || dh <= 0)
		return;

	grect_t sr = {sx, sy, sw, sh};
	grect_t dr = {dx, dy, dw, dh};
	graph_insect(dst, &dr);
	if(!insect(src, &sr, dst, &dr))
		return;
	register int32_t ex, ey;
	sy = sr.y;
	dy = dr.y;
	ex = sr.x + sr.w;
	ey = sr.y + sr.h;

	schd_core_lock();
	for(; sy < ey; sy++, dy++) {
		register int32_t sx = sr.x;
		register int32_t dx = dr.x;
		register int32_t offset = sy * src->w;
		for(; sx < ex; sx+=8, dx+=8) {
			graph_pixel_argb_neon(dst, dx, dy, &src->buffer[offset + sx], MIN(ex-sx, 8));	
		}
	}
	schd_core_unlock();
}


#ifdef __cplusplus
}
#endif
