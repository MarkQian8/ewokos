#include "MacWM.h"
#include <sys/kernel_tic.h>
#include <sys/klog.h>
#include <upng/upng.h>
#include <sconf/sconf.h>
#include <stdlib.h>

using namespace Ewok;

void MacWM::drawDesktop(graph_t* g) {
	int sz = 2; 
	int x = 0;
	int y = 0;

	uint32_t c1, c2, cm;
	c1 = desktopFGColor;
	c2 = desktopBGColor;

	for(int i=0; y<g->h; i++) {
		for(int j=0; x<g->w;j++) {
			graph_fill(g, x, y, sz, sz, c1);
			x += sz;

			cm = c1;
			c1 = c2;
			c2 = cm;
		}
		x = 0;
		y += sz;

		cm = c1;
		c1 = c2;
		c2 = cm;
	}
}

void MacWM::drawTitlePattern(graph_t* g, int x, int y, int w, int h, uint32_t fg) {
	int step = 3;
	y = y + step;
	int steps = h / step;

	for (int i = 1; i < steps-1; i++) {
		graph_line(g, x + 4, y, x + w - 8, y, fg);
		y += step;
	}
}

void MacWM::drawTitle(graph_t* g, xinfo_t* info, grect_t* r, bool top) {
	uint32_t fg, bg;
	getColor(&fg, &bg, top);
	gsize_t sz;
	font_text_size(info->title, &font, (uint32_t*)&sz.w, (uint32_t*)&sz.h);
	
	grect_t rect;
	getTitle(info, &rect);

	int pw = (r->w-sz.w)/2;
	int ph = (r->h-sz.h)/2;
	graph_fill(g, r->x, r->y, r->w, rect.h, bg);//title box
	if(top) {
		drawTitlePattern(g, r->x, r->y, pw, r->h, fg);
		drawTitlePattern(g, r->x+pw+sz.w, r->y, pw, r->h, fg);
	}
	graph_draw_text_font(g, r->x+pw, r->y+ph, info->title, &font, fg);//title
}


MacWM::~MacWM(void) {
	font_close(&font);
}

MacWM::MacWM(void) {
	desktopBGColor = 0xff555588;
	desktopFGColor = 0xff8888aa;
	bgColor = 0xff222222;
	fgColor = 0xff888888;
	bgTopColor = 0xffaaaaaa;
	fgTopColor = 0xff222222;
	titleH = 32;
}