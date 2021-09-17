#ifdef WIN32
#include "pch.h"
#else
#include "stdafx.h"
#endif
//#ifdef USE_OPEN_CV
#include "OpenCVTest_LineDetection.h"

cOpenCVTest_LineDetection::cOpenCVTest_LineDetection()
{
    m_pOpenGLFrame = new sMatWithFlag();
    m_pLineDetectFrame = new sMatWithFlag();
    m_pGrayFrame = new sMatWithFlag();
    m_pVideoImage = new FATMING_CORE::cBaseImage((const char*)"666");
    m_pGrayImage = new FATMING_CORE::cBaseImage((const char*)"999");
    m_pVideoImage->SetName(L"OpenCVToOpenGLTexture");
    m_pGrayImage->SetName(L"GrayImage");
    try
    {
        int length_threshold = 10;
        float distance_threshold = 1.41421356f;
        double canny_th1 = 100.0;
        double canny_th2 = 100.0;
        int canny_aperture_size = 3;
        bool do_merge = false;
        m_LineDetection = cv::ximgproc::createFastLineDetector(length_threshold,
            distance_threshold, canny_th1, canny_th2, canny_aperture_size,
            do_merge);
    }
    catch (cv::Exception& e)
    {
        cerr << e.msg << endl; // output exception message
    }
}

cOpenCVTest_LineDetection::~cOpenCVTest_LineDetection()
{
    this->CloseThreadAndWaitUntilFinish();
    m_LineDetectThread.CloseThreadAndWaitUntilFinish();
    SAFE_DELETE(m_pGrayFrame);
    SAFE_DELETE(m_pLineDetectFrame);
    SAFE_DELETE(m_pOpenGLFrame);
}


void cOpenCVTest_LineDetection::CameraReadThread(float e_fElpaseTime)
{
    m_CameraFPS.Update();
    if (!m_pVideoCapture)
    {
        //Sleep(100);
        return;
    }
    if (!m_pVideoCapture->isOpened())
    {
        if (m_strCameraURL.length())
        {
            m_pVideoCapture->open(m_strCameraURL.c_str());
            Sleep(1000);
        }
        else
        {
            auto l_b = m_pVideoCapture->open(0);
            Sleep(100);
        }
        return;
    }
    Sleep(1);
    cv::Mat l_Frame;
    *m_pVideoCapture >> l_Frame;
    {
        MUTEX_PLACE_HOLDER(m_OpenGLFrameMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");
        if (m_pOpenGLFrame->bNewData == false)
        {
            m_pOpenGLFrame->Frame = l_Frame.clone();
            m_pOpenGLFrame->bNewData = true;
        }
    }
    {
        MUTEX_PLACE_HOLDER(m_LineFrameMutex, "m_LineFrameMutex");
        if (m_pLineDetectFrame->bNewData == false)
        {
            m_pLineDetectFrame->Frame = l_Frame.clone();
            m_pLineDetectFrame->bNewData = true;
        }
    }
}

void cOpenCVTest_LineDetection::RecognizerThread(float e_fElpaseTime)
{
    if (m_pLineDetectFrame->bNewData)
    {
        m_RecognizerLineFPS.Update();
        cv::Mat l_BGR2GRAY,l_HSV,l_Mask;
        {
            MUTEX_PLACE_HOLDER(m_LineFrameMutex, "m_LineFrameMutex");
            cv::cvtColor(m_pLineDetectFrame->Frame, l_BGR2GRAY, cv::ColorConversionCodes::COLOR_BGR2GRAY);
            int l_iOffset = 10;
            cv::Scalar lower_val = (147 - l_iOffset, 112 - l_iOffset, 219 - l_iOffset);
            cv::Scalar upper_val = (147 + l_iOffset, 112 + l_iOffset, 219 + l_iOffset);
            //cv::Scalar lower_val = (10,254,254);
            //cv::Scalar upper_val = (255,255,255);
            //cv::inRange(m_pLineDetectFrame->Frame, lower_val, upper_val, l_BGR2GRAY);
            //cv::cvtColor(l_Mask, l_HSV, cv::ColorConversionCodes::COLOR_GRAY2BGR);
            //OpenCV are in BGR order
            //cv::cvtColor(l_HSV, l_BGR2GRAY, cv::ColorConversionCodes::COLOR_BGR2GRAY);
            //https://stackoverflow.com/questions/54051094/color-blocks-detection-and-label-in-opencv
            ////add blur because of pixel artefacts
            //cv::GaussianBlur(m_pLineDetectFrame->Frame,l_BGR2GRAY, cv::Size(5, 5), 5);
            ////convert to HSV
            //cv::cvtColor(l_BGR2GRAY, l_HSV, cv::ColorConversionCodes::COLOR_BGR2HSV);
            //cv::Scalar lower_val = (40, 100, 100);
            //cv::Scalar upper_val = (60, 255, 200);
            ////Threshold the HSV image to get only green colors
            //cv::inRange(l_HSV, lower_val, upper_val, l_Mask);
            ////apply mask to original image
            //cv::bitwise_and(l_BGR2GRAY, l_BGR2GRAY, l_BGR2GRAY,l_Mask);
            //https://stackoverflow.com/questions/38082004/opencv-inrange-function
            //only compare green
            //inRange(src, Scalar(0, 50, 0), Scalar(255, 100, 255), threshold);
            // //purple
            //147, 112, 219
            //int l_iOffset = 10;
            //cv::Scalar lower_val = (147- l_iOffset, 112- l_iOffset, 219- l_iOffset);
            //cv::Scalar upper_val = (147+ l_iOffset, 112+ l_iOffset, 219+ l_iOffset);
            //cv::Scalar lower_val = (10,254,254);
            //cv::Scalar upper_val = (255,255,255);
            //auto l_iChannel = m_pLineDetectFrame->Frame.channels();
            //cv::inRange(m_pLineDetectFrame->Frame, lower_val, upper_val, l_BGR2GRAY);
            //auto l_iChannel2 = l_Mask.channels();
            /*try
            {
                cv::cvtColor(l_Mask, l_BGR2GRAY, cv::ColorConversionCodes::COLOR_BGR2GRAY);
            }
            catch (cv::Exception e)
            {
                auto l_str = e.what();
            }*/
            
            //l_BGR2GRAY = m_pLineDetectFrame->Frame.clone();
        }
        vector<cv::Vec4f> lines_std;
        try
        {
            m_LineDetection->detect(l_BGR2GRAY, lines_std);
        }
        catch (cv::Exception e)
        {
            auto l_str = e.what();
        }
        vector<Vector2> l_AllInesVector;
        for (auto l_Line : lines_std)
        {
            l_AllInesVector.push_back(Vector2(l_Line[0], l_Line[1]));
            l_AllInesVector.push_back(Vector2(l_Line[2], l_Line[3]));
        }
        {
            MUTEX_PLACE_HOLDER(m_LineVectorMutex, "m_LineVectorMutex");
            m_DetectLineVector = l_AllInesVector;
            if (m_pGrayFrame->bNewData == false)
            {
                //cv::Scalar lower_val = (10,10,10);
                //cv::Scalar upper_val = (255,255,255);
                //auto l_iCan = m_pGrayFrame->Frame.channels();
                m_pGrayFrame->Frame = l_BGR2GRAY;// m_pLineDetectFrame->Frame.clone();
                m_pGrayFrame->bNewData = true;
            }
        }
        m_pLineDetectFrame->bNewData = false;
    }
}

void cOpenCVTest_LineDetection::OpenCamera(const char* e_strCameraURL)
{
    m_LineDetectThread;
    if (e_strCameraURL)
    {
        m_strCameraURL = e_strCameraURL;
    }
    SAFE_DELETE(m_pVideoCapture);
    m_pVideoCapture = new cv::VideoCapture("rkcamsrc io-mode=4 isp-mode=2A tuning-xml-path=/etc/cam_iq/IMX219.xml ! video/x-raw,format=NV12,width=640,height=360 ! videoconvert ! appsink");
    f_ThreadWorkingFunction l_f_ThreadWorkingFunction = std::bind(&cOpenCVTest_LineDetection::CameraReadThread, this, std::placeholders::_1);
    this->ThreadDetach(l_f_ThreadWorkingFunction, "cGameNetwork::CreateAsServer");
    f_ThreadWorkingFunction l_f_ThreadWorkingFunction2 = std::bind(&cOpenCVTest_LineDetection::RecognizerThread, this, std::placeholders::_1);
    m_LineDetectThread.ThreadDetach(l_f_ThreadWorkingFunction2, "cGameNetwork::CreateAsServer");
}

void cOpenCVTest_LineDetection::Update(float e_fElpaseTime)
{
    if (m_pOpenGLFrame)
    {
#ifdef WIN32
        GLenum inputColourFormat = GL_BGR;
#else
        GLenum inputColourFormat = GL_RGB;
#endif
        {
            MUTEX_PLACE_HOLDER(m_OpenGLFrameMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");
            if (m_pOpenGLFrame->bNewData)
            {
                //
                m_pVideoImage->SetupTexture(3, m_pOpenGLFrame->Frame.cols, m_pOpenGLFrame->Frame.rows, inputColourFormat, GL_UNSIGNED_BYTE, false, (GLvoid*)m_pOpenGLFrame->Frame.data, false);
                m_pOpenGLFrame->bNewData = false;
            }
        }
    }
}

void cOpenCVTest_LineDetection::Render()
{
    if (m_pVideoImage)
    {
        Vector2 l_vGrayPos;
        float l_fScale = 1.f;
        auto l_pTex = m_pVideoImage->GetTexture();
        if (l_pTex)
        {
            if (l_pTex->GetWidth() <= 640)
            {
                //l_fScale = 2.f;
                m_pVideoImage->SetWidth((int)(l_pTex->GetWidth() * l_fScale));
                m_pVideoImage->SetHeight((int)(l_pTex->GetHeight() * l_fScale));
            }
        }
        if (m_pGrayImage)
        {
            l_vGrayPos = Vector2(m_pVideoImage->GetWidth() + 10, 0);
            m_pGrayImage->SetPos(l_vGrayPos);
        }
        m_pVideoImage->Render();
        std::wstring l_str = L"CameraFPS:";
        l_str += ValueToStringW(m_CameraFPS.GetFPS());
        {
            l_str += L"\n";
            l_str += L"\n";
            l_str += L"Recognizer:";
            l_str += ValueToStringW(m_RecognizerLineFPS.GetFPS());
            l_str += L"\n";
        }
        cGameApp::RenderFont(700, 0, l_str.c_str());
        std::vector<Vector2> l_DetectLineVector;
        {
            MUTEX_PLACE_HOLDER(m_LineVectorMutex, "m_LineVectorMutex");
            l_DetectLineVector = m_DetectLineVector;
            if (m_pGrayImage && m_pGrayFrame->bNewData == true)
            {
                int l_iChannel = m_pGrayFrame->Frame.channels();
                bool l_bRenderGray = true;
                if (l_bRenderGray)
                {
                    auto inputColourFormat = GetPixelFormatByChannel(l_iChannel);
                    m_pGrayImage->SetupTexture(l_iChannel, m_pGrayFrame->Frame.cols, m_pGrayFrame->Frame.rows, inputColourFormat, GL_UNSIGNED_BYTE, false, (GLvoid*)m_pGrayFrame->Frame.data, false);
                }
                m_pGrayFrame->bNewData = false;
            }
        }
        if (m_pGrayFrame)
        {
            m_pGrayImage->Render();
        }
        auto l_uiSize = l_DetectLineVector.size()/2;
        for (size_t i=0;i< l_uiSize;++i)
        {
            std::vector<Vector2> l_Line = { l_DetectLineVector[i * 2]* l_fScale ,l_DetectLineVector[i * 2 + 1]* l_fScale };
            GLRender::RenderLine(&l_Line,Vector4::Red);
        }
    }
}
