#pragma once

#include "../../../Common/NamedTypedObject.h"
#include "../../../Common/Template/TemplateClass.h"
#include "../../../Math/Vector4.h"
#include "../../../Math/Matrix44.h"
#include "../FMAnimationRule/FMAnimationRule.h"
#include "../../OpenGL/Glh.h"
namespace FATMING_CORE
{
	enum eParticleRotateMode
	{
		ePRM_NONE = 0,
		ePRM_SELF,		//rotate in local space
		ePRM_WORLD,			//rotate in world space
	};
	//make sure whole init class has already setup respond original data
	//ensure but wise,currently is under 32bit
	//I am lazy to check this now.
	struct sParticleData
	{
		Vector3	vPos;				//in the world, be it 2D or 3D.
		Vector3	vVelocity;			//to determine its movement along the timeline.
		Vector4	vColor;				//to determine the color of a particle.
		float	fLifespan;			//to determine how long a particle is visible, or dies out.
		Vector3	vSize;				//to determine how big the particle is.
		Vector3	vAngle;				//rotate Angle
		eParticleRotateMode	ePRM;	//while particle is rotation,determine what kind type it is.

		Vector3	vOriginalPos;		//while pos is set do not forget to set this too,so we could know the progress,or related data
		Vector3	vOrigonalVelocity;	//see above
		Vector4	vOriginalColor;		//see above
		float	fOriginaLifeSpan;	//see above
		Vector3	vOriginalSize;		//see above
		Vector3	vOriginaAngle;		//see above
		//the time has been past
		//fPastTime/fOriginaLifeSpan is equal liner progress.
		float	fPastTime;
		int		iCurrentAnimationTextureIndex;//for dynamic texture.
	};
	 //cTexture*		pSimpleGLTexture;//texture
//end PRT namespace
};