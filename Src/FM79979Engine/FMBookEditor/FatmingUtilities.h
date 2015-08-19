#pragma once

#include "FMBookEditorUIImplement/MPDI.h"
#include "FMBookEditorUIImplement/EventInstanceEditor.h"
#include "FMBookEditorUIImplement/Particle.h"
#include "FMBookEditorUIImplement/PI.h"
#include "FMBookEditorUIImplement/Sound.h"
#include "FMBookEditorUIImplement/XMLText.h"
#include "FMBookEditorUIImplement/Image.h"

namespace FMBookEditor
{
	public ref class cFatmingUtilitiesData
	{
		cFMBookApp*m_pFMBookApp;
	public:
		cFatmingUtilitiesData(FMBookEditorUI::FMBookUI^e_pUserControl1,cFMBookApp*e_pFMBookApp)
		{
			m_pFMBookApp = e_pFMBookApp;
			m_pFMBookUI = e_pUserControl1;
			m_pMPDI = gcnew FMBookEditor::MPDI(m_pFMBookUI->m_FatmingUtilities->m_MPDI);
			m_pMPDI->SetAnimationParser(m_pFMBookApp->m_spAnimationParser);
			m_pParticle = gcnew FMBookEditor::Particle(m_pFMBookUI->m_FatmingUtilities->m_Particle);
			m_pPI = gcnew FMBookEditor::PI(m_pFMBookUI->m_FatmingUtilities->m_PI);
			m_pPI->SetImageParser(m_pFMBookApp->m_spImageParser);
			m_pSound = gcnew FMBookEditor::Sound(m_pFMBookUI->m_FatmingUtilities->m_Sound);
			m_pSound->SetSoundParser(m_pFMBookApp->m_spSoundParser);
			m_pXMLText = gcnew FMBookEditor::XMLText(m_pFMBookUI->m_FatmingUtilities->m_XMLText);
			m_pImage = gcnew FMBookEditor::Image(m_pFMBookUI->m_FatmingUtilities->m_Image);
			m_pImage->SetImageParser(m_pFMBookApp->m_spImageParser);
		}
		~cFatmingUtilitiesData()
		{
			delete m_pMPDI;
			delete m_pParticle;
			delete m_pPI;
			delete m_pSound;
			delete m_pXMLText;
			delete m_pImage;
		}
		FMBookEditor::MPDI^			m_pMPDI;
		FMBookEditor::Particle^		m_pParticle;
		FMBookEditor::PI^			m_pPI;
		FMBookEditor::Sound^		m_pSound;
		FMBookEditor::XMLText^		m_pXMLText;
		FMBookEditor::Image^		m_pImage;
		FMBookEditorUI::FMBookUI^	m_pFMBookUI;
		public:void	Refresh();
	};
}