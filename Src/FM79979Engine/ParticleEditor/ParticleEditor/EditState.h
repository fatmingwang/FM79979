#ifndef EDIT_STATE
#define EDIT_STATE

enum	eEditState
{
	eES_MOUSE = 0,
	eES_TRANSLATTION,
	eES_ROTATIONX,
	eES_ROTATIONY,
	eES_ROTATIONZ,
};

extern eEditState	g_eEditState;
extern Vector3	g_vEmiterPos;


#endif