#include "OV7670_control.h"
#include <stdint.h>

void bit_shift(uint16_t* bit_origin)
{
	uint16_t bit = *bit_origin;
	if(bit == 0 | bit == 1)
	{
		bit = 0x8000;
		*bit_origin =  bit;
		return;
	}
	bit = (bit >> 1);
	*bit_origin = bit;
}
void get_yellow_line(uint16_t src[IMG_ROWS*IMG_COLUMNS], uint16_t des[IMG_ROWS*IMG_COLUMNS/16])
{
	int h,s,v;
	uint16_t b,g,r;
	uint32_t n;
	uint8_t MAX_color; // Blue 0, Green 1, Red 2
	uint8_t min;
	uint16_t bit = 0;
	
	for (n = 0; n < ILI9341_PIXEL; n++) {
		
		    bit_shift(&bit);
		
				r = ( src[n] & 0xF800 ) >> 8;
				g = ( src[n] & 0x7E0 ) >> 3;
				b = ( src[n] & 0x1F) << 3;
				
				v = r;
				MAX_color = 2;
				if(g > v) { v = g; MAX_color = 1; }
				if(b > v) { v = b; MAX_color = 0; }
				
				min = r;
				if(g < min) min = g;
				if(b < min) min = b;
				
				if(v==0) s=0;
				else s=255*(float)(v-min)/v;
				switch(MAX_color)
				{
					 case 2 : h = 240 + (float)60 * (r - g) / (v - min); break;
					 case 1 : h = 120 + (float)60 * (b - r) / (v - min); break;
					 case 0 : h =       (float)60 * (g - b) / (v - min); break;
					 default : h = 0;
				}
				if(h<0) h = h+360;
				h = h/2;
				if( ( 25 < h && h < 95 ) && ( 65 < s && s < 255 ) && ( 10 < v && v < 255) )
						des[n/16] |= bit;
				else des[n/16] &= ~bit;
		 }
}
void get_origin_yellow_line(uint16_t src[IMG_ROWS*IMG_COLUMNS], uint16_t last_frame[IMG_ROWS*IMG_COLUMNS/16], uint16_t current_frame[IMG_ROWS*IMG_COLUMNS/16])
{
	int h,s,v;
	uint16_t b,g,r;
	uint32_t n, i;
	uint8_t MAX_color; // Blue 0, Green 1, Red 2
	uint8_t min;
	uint16_t bit = 0;
	for (n = 0; n < ILI9341_PIXEL; n++) {
	
		bit_shift(&bit);
		
		r = ( src[n] & 0xF800 ) >> 8;
		g = ( src[n] & 0x7E0 ) >> 3;
		b = ( src[n] & 0x1F) << 3;
				
		v = r;
		MAX_color = 2;
		if(g > v) { v = g; MAX_color = 1; }
		if(b > v) { v = b; MAX_color = 0; }
				
		min = r;
		if(g < min) min = g;
		if(b < min) min = b;
				
		if(v==0) s=0;
		else s=255*(float)(v-min)/v;
		switch(MAX_color)
		{
			 case 2 : h = 240 + (float)60 * (r - g) / (v - min); break;
			 case 1 : h = 120 + (float)60 * (b - r) / (v - min); break;
			 case 0 : h =       (float)60 * (g - b) / (v - min); break;
				default : h = 0;
		}
		if(h<0) h = h+360;
		h = h/2;
		
		if( ( 25 < h && h < 95 ) && ( 65 < s && s < 255 ) && ( 10 < v && v < 255) )
		{
			if(last_frame[n/16] & bit)
			  current_frame[n/16] |= bit;
		  else current_frame[n/16] &= ~bit;
		}
	}
	for(n = 0; n < ILI9341_PIXEL/16; n++)
	{
		//last_frame[n] = current_frame[n];
	}
}
