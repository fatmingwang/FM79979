namespace FATMING_CORE
{
	//1.1 fix point to leftup not center.
	//1.2 image size change to image scale,
	//1.3f blending parse changed
	//so while image is totaly change it still could be scale change(common for use).
	const static float g_fMPDIVersion = 1.3f;
#ifdef WIN32
//for MPDI old version 
#ifdef MPDI_VERSION_1_0
	void	ExportPointDataList(ATG::XMLWriter*e_pXMLWriter,cCueToStartCurveWithTime*e_pCueToStartCurvesWithTime);
#else
	char*	TexBehaviorDataWithImageIndexDataToString(sTexBehaviorDataWithImageIndexData e_sTexBehaviorDataWithImageIndexData,sPuzzleData**e_ppPuzzleData,int e_iNumData);
	void	ExportPointDataList(ATG::XMLWriter*e_pXMLWriter,cCueToStartCurveWithTime*e_pCueToStartCurvesWithTime);
#endif
	void	ExportMPDI(ATG::XMLWriter*e_pXMLWriter,cMPDIList*e_pMPDIList);
//end WIN32
#endif	
}