/*************************************************************************

	Venture Line Super Rider driver

**************************************************************************/

/*----------- defined in vidhrdw/suprridr.c -----------*/

extern data8_t *suprridr_fgram;
extern data8_t *suprridr_bgram;

VIDEO_START( suprridr );
PALETTE_INIT( suprridr );

WRITE8_HANDLER( suprridr_flipx_w );
WRITE8_HANDLER( suprridr_flipy_w );
WRITE8_HANDLER( suprridr_fgdisable_w );
WRITE8_HANDLER( suprridr_fgscrolly_w );
WRITE8_HANDLER( suprridr_bgscrolly_w );

WRITE8_HANDLER( suprridr_fgram_w );
WRITE8_HANDLER( suprridr_bgram_w );

VIDEO_UPDATE( suprridr );
