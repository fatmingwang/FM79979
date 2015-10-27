#ifndef _MAGIC_TOWER_MOVE_OBJECT_H_
#define _MAGIC_TOWER_MOVE_OBJECT_H_

	//<MovingObject Range="0,0,100,100" Speed="" Gesture="" DelayToGo="" DistanceRandPos="0,1">
	//	<MovingObjectImage MPDIList="" MPDI="" />
	//</MovingObject>
//change it to class and inhrient NamedTpyedObject

	struct	sMovingObject
	{
		sMovingObject(TiXmlElement*e_pTiXmlElement);
		~sMovingObject();
		Vector2			vMovingOrientation;
		//detect how it moving
		eGestureEnum	MovingOrientation;
		cMPDI*			pMovingObjectImage;
		Vector4			vRestrict;
		struct	sData
		{
			float	fSpeed;
			float	fDelayTimeToGo;
			UT::sMinMaxData<float>	DistanceRandPos;//0~1
			float	fCloseToMinusSpeed;
		};
		sData			Data;
		//0~1
		void			Init(sData e_Data);
		void			Update(float e_fElpaseTime);
		void			Render();
	};
//<TwoMovingObjectController FrameMPDIList="" FrameMPDI="">
//	<MovingObject Range="0,0,100,100" Speed="" Gesture="" DelayToGo="">
//		<MovingObjectImage MPDIList="" MPDI="" />
//	</MovingObject>
//	<MovingObject Range="0,0,100,100" Speed="" Gesture="" DelayToGo="">
//		<MovingObjectImage MPDIList="" MPDI="" />
//	</MovingObject>
//	<LVData LV="1" Speed="30" DelayToGo="" CloseToMinusSpeed="10" />
//</TwoMovingObjectController>
	struct	sTwoMovingObjectController
	{
		struct	sLVData
		{
			int						iCollectEnergy;
			sMovingObject::sData	Data[2];
		};
		sLVData				GetLVData(TiXmlElement*e_pElement);
		std::vector<sLVData>LVDataList;
		cBasicSound*		m_pHorverSound;
		bool				bTouchInVaildArea;
		cClickMouseBehavior	KeyToCollide;
		cMPDI*				pFrameImage;
		bool				bIsDone;
		//how cloase they are
		float				fTwoObjectCloseValue;
		sMovingObject*		pMovingObject[2];
		sTwoMovingObjectController(TiXmlElement*e_pTiXmlElement);
		~sTwoMovingObjectController();
		void				Init(int e_iLVIndex);
		void				Update(float e_fElpaseTime);
		void				Render();
		bool				IsCollide(int e_iPosX,int e_iPosY);
		bool				IsCollide();
		void				MouseDown(int e_iPosX,int e_iPosY);
		void				MouseMove(int e_iPosX,int e_iPosY);
		void				MouseUp(int e_iPosX,int e_iPosY);
		void				MouseLeave(int e_iPosX,int e_iPosY);
		void				DebugRender();
	};

#endif