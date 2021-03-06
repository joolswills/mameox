/*************************************************************************

	Atari Night Driver hardware

*************************************************************************/

/*----------- defined in machine/nitedrvr.c -----------*/

extern unsigned char *nitedrvr_ram;

extern int nitedrvr_gear;
extern int nitedrvr_track;

READ8_HANDLER( nitedrvr_in0_r );
READ8_HANDLER( nitedrvr_in1_r );
READ8_HANDLER( nitedrvr_ram_r );
READ8_HANDLER( nitedrvr_steering_reset_r );
WRITE8_HANDLER( nitedrvr_steering_reset_w );
WRITE8_HANDLER( nitedrvr_out0_w );
WRITE8_HANDLER( nitedrvr_out1_w );
WRITE8_HANDLER( nitedrvr_ram_w );

void nitedrvr_crash_toggle(int dummy);

/*----------- defined in vidhrdw/nitedrvr.c -----------*/

extern unsigned char *nitedrvr_hvc;
WRITE8_HANDLER( nitedrvr_hvc_w );
VIDEO_UPDATE( nitedrvr );
