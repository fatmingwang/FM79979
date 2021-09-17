#ifdef WIN32
#include "pch.h"
#else
#include "stdafx.h"
#endif
//#ifdef USE_OPEN_CV
#include "opencv2/opencv.hpp"
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "OpenCVTest_OCR.h"
#include <opencv2/dnn.hpp>

#include <locale>
#include <codecvt>

using namespace cv;
using namespace cv::dnn;

tesseract::TessBaseAPI* g_TesseractOCR = nullptr;

Net g_Detector;
Net g_Recognizer;

void decodeBoundingBoxes(const Mat& scores, const Mat& geometry, float scoreThresh,
	std::vector<RotatedRect>& detections, std::vector<float>& confidences);
void fourPointsTransform(const Mat& frame, Point2f vertices[4], Mat& result);
void decodeText(const Mat& scores, std::string& text);

cOpenCVTest_OCR::cOpenCVTest_OCR()
{
    m_pFreetypeGlyphRender = new cFreetypeGlyphRender("Language/msjhl.ttc", 32);
    auto l_vColor = Vector4::Yellow * 2;
    l_vColor.w = 1.f;
    m_pFreetypeGlyphRender->SetColor(l_vColor);
    //g_TesseractOCR = nullptr;
    g_TesseractOCR = new tesseract::TessBaseAPI();
    if (g_TesseractOCR)
    {
        //trained data.
        //https://github.com/tesseract-ocr/tessdata
        //bool l_bInit = g_TesseractOCR->Init("opencv", "opencv/eng.traineddata", tesseract::OEM_LSTM_ONLY);
        //bool l_bInit = g_TesseractOCR->Init("opencv/tesseract_trained_data","eng");
        //bool l_bInit = g_TesseractOCR->Init("opencv/tesseract_trained_data", "eng", tesseract::OEM_LSTM_ONLY);
        //bool l_bInit = g_TesseractOCR->Init("opencv/tesseract_trained_data", "eng");
        
        bool l_bInit = g_TesseractOCR->Init("opencv/tesseract_trained_data", "chi_tra+eng");
        //bool l_bInit = g_TesseractOCR->Init("opencv/tesseract_trained_data", "chi_sim+eng");
        //bool l_bInit = g_TesseractOCR->Init("opencv/tesseract_trained_data", "chi_tra_vert");
        
        //g_TesseractOCR->SetPageSegMode(tesseract::PSM_AUTO_ONLY);
        FMLog::Log(!l_bInit ? "Tess Init ok!" : "Tess Init Failed!", false);
    }
    else
    {
        String modelDecoder = "opencv/frozen_east_text_detection.pb";
        String modelRecognition = "opencv/crnn.onnx";
        CV_Assert(!modelDecoder.empty());
        // Load networks.
        g_Detector = readNet(modelDecoder);
        g_Detector.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        g_Detector.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        if (!modelRecognition.empty())
        {
            g_Recognizer = readNet(modelRecognition);
        }
    }
	m_pVideoCapture = nullptr;
	m_pVideoImage = nullptr;
    m_pOpenGLFrame = new sMatWithFlag();
    m_pRecognizerFrame = new sMatWithFlag();
    m_pConvertedBlobFrame = nullptr;// new sMatWithFlag();
    m_pVideoImage = new FATMING_CORE::cBaseImage((const char*)"");
    m_pVideoImage->SetName(L"OpenCVToOpenGLTexture");
    m_pConvertedBlobImage = nullptr;// new FATMING_CORE::cBaseImage((const char*)"");
    //m_pConvertedBlobImage->SetName(L"m_pConvertedBlobImage");
}

cOpenCVTest_OCR::~cOpenCVTest_OCR()
{
    this->CloseThreadAndWaitUntilFinish();
    m_RecognizerThread.CloseThreadAndWaitUntilFinish();
    SAFE_DELETE(m_pConvertedBlobFrame);
    SAFE_DELETE(m_pOpenGLFrame);
    SAFE_DELETE(m_pRecognizerFrame);
    SAFE_DELETE(g_TesseractOCR);
    SAFE_DELETE(m_pVideoCapture);
}

void cOpenCVTest_OCR::CameraReadThread(float e_fElpaseTime)
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
        MUTEX_PLACE_HOLDER(m_RecognizerFrameCopyMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");
        if (m_pRecognizerFrame->bNewData == false)
        {
            m_pRecognizerFrame->Frame = l_Frame.clone();
            m_pRecognizerFrame->bNewData = true;
        }
    }    
}

void	cOpenCVTest_OCR::RecognizerThread(float e_fElpaseTime)
{
    Mat l_Frame;
    {
        if (m_pRecognizerFrame->bNewData)
        {
            MUTEX_PLACE_HOLDER(m_RecognizerFrameCopyMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");
            //640,480
            //for DNN must be multiple of 32
            //cv::resize(*m_pRecognizerFrame->pFrame, frame, cv::Size(320, 256));
            //cv::resize(*m_pRecognizerFrame->pFrame, frame, cv::Size(256, 192));
            l_Frame = m_pRecognizerFrame->Frame.clone();

        }
        else
        {
            Sleep(1);
            static int lll = 0;
            //FMLog::Log(UT::ComposeMsgByFormat("Count:%d", lll).c_str(), false);
            ++lll;
            return;
        }
    }
    m_RecognizerWordFPS.Update();
    if (g_TesseractOCR)
    {
        TesseractParseTextIterator(l_Frame);
        //TesseractParseTextImageComponet(l_Frame);
    }
    else
    {
        OpencvDNNParseText(l_Frame);
    }
    m_pRecognizerFrame->bNewData = false;
}

void	cOpenCVTest_OCR::OpencvDNNParseText(cv::Mat& e_Frame)
{
    Mat blob;
    sTimeAndFPS l_sTimeAndFPS;
    l_sTimeAndFPS.Update();
    float confThreshold = 0.5f;
    float nmsThreshold = 0.4f;
    //must be multiply 32
    int inpWidth = 640;
    int inpHeight = 480;
    //int inpWidth = e_Frame.cols;
    //int inpHeight = e_Frame.rows;
    bool openSuccess = true;
    CV_Assert(openSuccess);
    std::vector<Mat> outs;
    std::vector<String> outNames(2);
    outNames[0] = "feature_fusion/Conv_7/Sigmoid";
    outNames[1] = "feature_fusion/concat_3";
    blobFromImage(e_Frame, blob, 1.0, Size(inpWidth, inpHeight), Scalar(123.68, 116.78, 103.94), true, false);
    l_sTimeAndFPS.Update();
    auto l_f1 = l_sTimeAndFPS.fElpaseTime;
    g_Detector.setInput(blob);
    g_Detector.forward(outs, outNames);
    l_sTimeAndFPS.Update();
    auto l_f2 = l_sTimeAndFPS.fElpaseTime;
    Mat scores = outs[0];
    Mat geometry = outs[1];
    // Decode predicted bounding boxes.
    std::vector<RotatedRect> boxes;
    std::vector<float> confidences;
    {
        //MUTEX_PLACE_HOLDER(m_ConvertedBlobFrameMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");
        //if (m_pConvertedBlobFrame)
        //{
        //    scores.copyTo(*m_pConvertedBlobFrame->pFrame);
        //    m_pConvertedBlobFrame->bNewData = true;
        //}
    }
    decodeBoundingBoxes(scores, geometry, confThreshold, boxes, confidences);
    l_sTimeAndFPS.Update();
    auto l_f3 = l_sTimeAndFPS.fElpaseTime;
    // Apply non-maximum suppression procedure.
    std::vector<int> indices;
    NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
    l_sTimeAndFPS.Update();
    auto l_f4 = l_sTimeAndFPS.fElpaseTime;
    Point2f ratio((float)e_Frame.cols / inpWidth, (float)e_Frame.rows / inpHeight);
    // Render text.
    std::string l_strWholdWorld;
    std::vector < sPosAndText>	l_PosAndTextVector;
    for (size_t i = 0; i < indices.size(); ++i)
    {
        sPosAndText l_PosAndText;
        RotatedRect& box = boxes[indices[i]];
        Point2f vertices[4];
        box.points(vertices);
        for (int j = 0; j < 4; ++j)
        {
            vertices[j].x *= ratio.x;
            vertices[j].y *= ratio.y;
            l_PosAndText.PointsVector.push_back(Vector2(vertices[j].x, vertices[j].y));
        }
        l_PosAndText.PointsVector.push_back(Vector2(vertices[0].x, vertices[0].y));
        {
            Mat cropped;
            fourPointsTransform(e_Frame, vertices, cropped);
            cvtColor(cropped, cropped, cv::COLOR_BGR2GRAY);
            Mat blobCrop = blobFromImage(cropped, 1.0 / 127.5, Size(), Scalar::all(127.5));
            g_Recognizer.setInput(blobCrop);
            //tickMeter.start();
            Mat result = g_Recognizer.forward();
            //tickMeter.stop();
            std::string wordRecognized = "";
            decodeText(result, wordRecognized);
            l_PosAndText.strText = utf8_to_utf16(wordRecognized);
            l_strWholdWorld += wordRecognized;
            l_strWholdWorld += "\n";
        }
        l_PosAndTextVector.push_back(l_PosAndText);
    }
    {
        MUTEX_PLACE_HOLDER(m_RecognizerSetWordMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");
        l_sTimeAndFPS.Update();
        m_strRecognizerWord = "ReadWordElpaseTime:";
        m_strRecognizerWord += ValueToString(l_sTimeAndFPS.fElpaseTime);
        //m_strRecognizerWord += "\n";
        //m_strRecognizerWord += l_strWholdWorld;
        m_WordAndPosVector = l_PosAndTextVector;
    }
}

void	cOpenCVTest_OCR::TesseractParseTextIterator(cv::Mat&e_Frame)
{
    //Mat im = cv::imread("BluffingGirl/Image/Alert_Banner.png", IMREAD_COLOR);
    //g_TesseractOCR->SetImage(im.data, im.cols, im.rows, 3, im.step);
    sTimeAndFPS l_sTimeAndFPS;
    l_sTimeAndFPS.Update();
    bool l_bDoDenoise = false;
    if (l_bDoDenoise)
    {
        cv::Mat l_Mat;
        cv::fastNlMeansDenoisingColored(e_Frame, l_Mat);
        g_TesseractOCR->SetImage(l_Mat.data, l_Mat.cols, l_Mat.rows, 3, (int)l_Mat.step);
    }
    else
    {
        g_TesseractOCR->SetImage(e_Frame.data, e_Frame.cols, e_Frame.rows, 3, (int)e_Frame.step);
    }
    //https://tesseract-ocr.github.io/tessdoc/APIExample.html
    g_TesseractOCR->Recognize(0);
    tesseract::ResultIterator* ri = g_TesseractOCR->GetIterator();
    //tesseract::PageIteratorLevel level = tesseract::RIL_TEXTLINE;
    tesseract::PageIteratorLevel level = tesseract::RIL_WORD;
    if (ri != 0)
    {
        std::string l_strWholdWorld;
        std::vector < sPosAndText>	l_PosAndTextVector;
        do
        {
            sPosAndText l_sPosAndText;
            const char* word = ri->GetUTF8Text(level);
            if (word)
            {
                //l_strWholdWorld += word;
                //l_strWholdWorld += "\n";
                float conf = ri->Confidence(level);
                l_sPosAndText.fConfidence = conf;
                {
                    int x1, y1, x2, y2;
                    ri->BoundingBox(level, &x1, &y1, &x2, &y2);
                    l_sPosAndText.PointsVector.push_back(Vector2(x1, y1));
                    l_sPosAndText.PointsVector.push_back(Vector2(x2, y1));
                    l_sPosAndText.PointsVector.push_back(Vector2(x2, y2));
                    l_sPosAndText.PointsVector.push_back(Vector2(x1, y2));
                    l_sPosAndText.PointsVector.push_back(Vector2(x1, y1));               
                    l_sPosAndText.strText = utf8_to_utf16(word);
                    l_sPosAndText.strText += L":";
                    l_sPosAndText.strText += ValueToStringW((int)conf);
                    l_PosAndTextVector.push_back(l_sPosAndText);
                    //printf("word: '%s';  \tconf: %.2f; BoundingBox: %d,%d,%d,%d;\n", word, conf, x1, y1, x2, y2);
                    delete[] word;
                }
            }
        } while (ri->Next(level));
        //
        l_sTimeAndFPS.Update();
        {
            MUTEX_PLACE_HOLDER(m_RecognizerSetWordMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");
            m_strRecognizerWord = "ReadWordElpaseTime:";
            m_strRecognizerWord += ValueToString(l_sTimeAndFPS.fElpaseTime);
            //m_strRecognizerWord += "\n";
            //m_strRecognizerWord += l_strWholdWorld;
            m_WordAndPosVector = l_PosAndTextVector;
        }
    }

    //auto l_str = g_TesseractOCR->GetUTF8Text();
    //if (l_str && strlen(l_str))
    //{
    //    //FMLog::Log(UT::ComposeMsgByFormat("OCR output:\n%s", l_str).c_str(),false);
    //    auto l_Result = CharToWchar(l_str);
    //    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    //    //std::string narrow = converter.to_bytes(wide_utf16_source_string);
    //    std::wstring wide = converter.from_bytes(l_str);
    //    if (l_Result.length() > 3)
    //    {
    //        int a = 0;
    //    }
    //    auto outText = string(l_str);
    //    MUTEX_PLACE_HOLDER(m_RecognizerSetWordMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");
    //    l_sTimeAndFPS.Update();
    //    m_strRecognizerWord = outText;
    //    delete l_str;
    //}
}

void	cOpenCVTest_OCR::TesseractParseTextImageComponet(cv::Mat& e_Frame)
{
    sTimeAndFPS l_sTimeAndFPS;
    l_sTimeAndFPS.Update();
    g_TesseractOCR->SetImage(e_Frame.data, e_Frame.cols, e_Frame.rows, 3, (int)e_Frame.step);
    //Boxa* boxes = g_TesseractOCR->GetComponentImages(tesseract::RIL_TEXTLINE, true, NULL, NULL);
    Boxa* boxes = g_TesseractOCR->GetComponentImages(tesseract::RIL_WORD, true, NULL, NULL);
    if (boxes)
    {
        std::string l_strWholdWorld;
        std::vector < sPosAndText>	l_PosAndTextVector;
        for (int i = 0; i < boxes->n; i++)
        {
            BOX* box = boxaGetBox(boxes, i, L_CLONE);
            sPosAndText l_PosAndText;
            Vector2 l_vVer[4] =
            {
                {box->x,box->y},
                {box->x + box->w,box->y},
                {box->x + box->w,box->y + box->h},
                {box->x,box->y + box->h}
            };
            for (int i = 0; i < 4; ++i)
            {
                l_PosAndText.PointsVector.push_back(l_vVer[i]);
            }
            l_PosAndText.PointsVector.push_back(l_vVer[0]);
            g_TesseractOCR->SetRectangle(box->x, box->y, box->w, box->h);
            char* ocrResult = g_TesseractOCR->GetUTF8Text();
            if (ocrResult)
            {
                int conf = g_TesseractOCR->MeanTextConf();
                l_PosAndText.fConfidence = (float)conf;
                l_strWholdWorld += ocrResult;
                l_PosAndTextVector.push_back(l_PosAndText);
                //l_sPosAndText.fConfidence = ;
            }
            boxDestroy(&box);
        }
        //
        l_sTimeAndFPS.Update();
        {
            MUTEX_PLACE_HOLDER(m_RecognizerSetWordMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");
            m_strRecognizerWord = "ReadWordElpaseTime:";
            m_strRecognizerWord += ValueToString(l_sTimeAndFPS.fElpaseTime);
            //m_strRecognizerWord += "\n";
            //m_strRecognizerWord += l_strWholdWorld;
            m_WordAndPosVector = l_PosAndTextVector;
        }
    }
}
//https://www.itread01.com/content/1543374489.html
void cOpenCVTest_OCR::OpenCamera(const char* e_strCameraURL)
{
	if (e_strCameraURL)
	{
		m_strCameraURL = e_strCameraURL;
	}
	SAFE_DELETE(m_pVideoCapture);
	m_pVideoCapture = new cv::VideoCapture("rkcamsrc io-mode=4 isp-mode=2A tuning-xml-path=/etc/cam_iq/IMX219.xml ! video/x-raw,format=NV12,width=640,height=360 ! videoconvert ! appsink");
    f_ThreadWorkingFunction l_f_ThreadWorkingFunction = std::bind(&cOpenCVTest_OCR::CameraReadThread, this, std::placeholders::_1);
    this->ThreadDetach(l_f_ThreadWorkingFunction, "cGameNetwork::CreateAsServer");
    f_ThreadWorkingFunction l_f_ThreadWorkingFunction2 = std::bind(&cOpenCVTest_OCR::RecognizerThread, this, std::placeholders::_1);
    m_RecognizerThread.ThreadDetach(l_f_ThreadWorkingFunction2, "cGameNetwork::CreateAsServer");
	//m_pVideoCapture = new cv::VideoCapture();
	//m_pVideoCapture->open(e_strCameraURL);
}
//opencv-4.4.0-vc14_vc15\opencv\build\testdata\cv\face\face_landmark_model.dat
void cOpenCVTest_OCR::Update(float e_fElpaseTime)
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
                m_pVideoImage->SetupTexture(3, m_pOpenGLFrame->Frame.cols, m_pOpenGLFrame->Frame.rows, inputColourFormat, GL_UNSIGNED_BYTE, false, (GLvoid*)m_pOpenGLFrame->Frame.data, false);
                m_pOpenGLFrame->bNewData = false;
            }
        }
        {
            MUTEX_PLACE_HOLDER(m_ConvertedBlobFrameMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");
            if (m_pConvertedBlobFrame && m_pConvertedBlobFrame->bNewData)
            {
                if (m_pConvertedBlobFrame->Frame.cols > 0 && m_pConvertedBlobImage)
                {
                    m_pConvertedBlobImage->SetupTexture(3, m_pConvertedBlobFrame->Frame.cols, m_pConvertedBlobFrame->Frame.rows, inputColourFormat, GL_UNSIGNED_BYTE, false, (GLvoid*)m_pConvertedBlobFrame->Frame.data, false);
                }
                m_pConvertedBlobFrame->bNewData = false;
            }
        }
    }
}

void cOpenCVTest_OCR::Render()
{
    if (m_pVideoImage)
    {
        std::vector < sPosAndText>	l_WordAndPosVector;
        float l_fScale = 1.f;
        auto l_pTex = m_pVideoImage->GetTexture();
        if (l_pTex)
        {
            if (l_pTex->GetWidth() <= 640)
            {
                l_fScale = 2.f;
                m_pVideoImage->SetWidth((int)(l_pTex->GetWidth() * l_fScale));
                m_pVideoImage->SetHeight((int)(l_pTex->GetHeight() * l_fScale));
            }
        }
        m_pVideoImage->Render();
        //if (m_pConvertedBlobImage)
        //{
        //    m_pConvertedBlobImage->SetPos(Vector2(m_pVideoImage->GetWidth()+10,0));
        //    m_pConvertedBlobImage->Render();
        //}
        std::wstring l_str = L"CameraFPS:";
        l_str += ValueToStringW(m_CameraFPS.GetFPS());
        {
            l_str += L"\n";
            MUTEX_PLACE_HOLDER(m_RecognizerSetWordMutex, "cOpenCVTest_FaceLandmark::CameraReadThread");
            l_str += ValueToStringW(m_strRecognizerWord);
            l_str += L"\n";
            l_str += L"RecognizerWord:";
            l_str += ValueToStringW(m_RecognizerWordFPS.GetFPS());
            l_str += L"\n";
            l_WordAndPosVector = m_WordAndPosVector;
        }
        cGameApp::RenderFont(700, 0, l_str.c_str());
        for (size_t i = 0; i < l_WordAndPosVector.size(); ++i)
        {
            auto l_WordAndPos = l_WordAndPosVector[i];
            auto l_fConf = l_WordAndPos.fConfidence;
            //if (l_fConf > 1.1f && l_fConf < 30.f)
            //{
            //        continue;
            //}
            //else
            //if (l_fConf < 0.3f )
            //{
            //    continue;
            //}
            GLRender::RenderLine(&l_WordAndPos.PointsVector, Vector4::Green,cMatrix44::ScaleMatrix(Vector3(l_fScale, l_fScale, l_fScale)));
            auto l_Pos = l_WordAndPos.PointsVector[1];
            l_Pos *= l_fScale;
            auto l_v1 = l_WordAndPos.PointsVector[0];
            auto l_v2 = l_WordAndPos.PointsVector[2];
            auto l_vData = (l_v2 - l_v1)/2 + l_v1;
            l_vData.x -= 20.f;
            if (m_pFreetypeGlyphRender)
            {
                m_pFreetypeGlyphRender->SetText(l_WordAndPos.strText.c_str());
                m_pFreetypeGlyphRender->SetPos(l_vData);
                m_pFreetypeGlyphRender->Render();
            }
            //cGameApp::RenderFont(l_vData,ValueToStringW(l_WordAndPos.strText).c_str());
        }
        
    }
}


void decodeBoundingBoxes(const Mat& scores, const Mat& geometry, float scoreThresh,
    std::vector<RotatedRect>& detections, std::vector<float>& confidences)
{
    detections.clear();
    CV_Assert(scores.dims == 4); CV_Assert(geometry.dims == 4); CV_Assert(scores.size[0] == 1);
    CV_Assert(geometry.size[0] == 1); CV_Assert(scores.size[1] == 1); CV_Assert(geometry.size[1] == 5);
    CV_Assert(scores.size[2] == geometry.size[2]); CV_Assert(scores.size[3] == geometry.size[3]);
    const int height = scores.size[2];
    const int width = scores.size[3];
    for (int y = 0; y < height; ++y)
    {
        const float* scoresData = scores.ptr<float>(0, 0, y);
        const float* x0_data = geometry.ptr<float>(0, 0, y);
        const float* x1_data = geometry.ptr<float>(0, 1, y);
        const float* x2_data = geometry.ptr<float>(0, 2, y);
        const float* x3_data = geometry.ptr<float>(0, 3, y);
        const float* anglesData = geometry.ptr<float>(0, 4, y);
        for (int x = 0; x < width; ++x)
        {
            float score = scoresData[x];
            if (score < scoreThresh)
                continue;
            // Decode a prediction.
            // Multiple by 4 because feature maps are 4 time less than input image.
            float offsetX = x * 4.0f, offsetY = y * 4.0f;
            float angle = anglesData[x];
            float cosA = std::cos(angle);
            float sinA = std::sin(angle);
            float h = x0_data[x] + x2_data[x];
            float w = x1_data[x] + x3_data[x];
            Point2f offset(offsetX + cosA * x1_data[x] + sinA * x2_data[x],
                offsetY - sinA * x1_data[x] + cosA * x2_data[x]);
            Point2f p1 = Point2f(-sinA * h, -cosA * h) + offset;
            Point2f p3 = Point2f(-cosA * w, sinA * w) + offset;
            RotatedRect r(0.5f * (p1 + p3), Size2f(w, h), -angle * 180.0f / (float)CV_PI);
            detections.push_back(r);
            confidences.push_back(score);
        }
    }
}
void fourPointsTransform(const Mat& frame, Point2f vertices[4], Mat& result)
{
    const Size outputSize = Size(100, 32);
    Point2f targetVertices[4] = { Point(0, outputSize.height - 1),
                                  Point(0, 0), Point(outputSize.width - 1, 0),
                                  Point(outputSize.width - 1, outputSize.height - 1),
    };
    Mat rotationMatrix = getPerspectiveTransform(vertices, targetVertices);
    warpPerspective(frame, result, rotationMatrix, outputSize);
}
void decodeText(const Mat& scores, std::string& text)
{
    static const std::string alphabet = "0123456789abcdefghijklmnopqrstuvwxyz";
    Mat scoresMat = scores.reshape(1, scores.size[0]);
    std::vector<char> elements;
    elements.reserve(scores.size[0]);
    for (int rowIndex = 0; rowIndex < scoresMat.rows; ++rowIndex)
    {
        Point p;
        minMaxLoc(scoresMat.row(rowIndex), 0, 0, 0, &p);
        if (p.x > 0 && static_cast<size_t>(p.x) <= alphabet.size())
        {
            elements.push_back(alphabet[p.x - 1]);
        }
        else
        {
            elements.push_back('-');
        }
    }
    if (elements.size() > 0 && elements[0] != '-')
        text += elements[0];
    for (size_t elementIndex = 1; elementIndex < elements.size(); ++elementIndex)
    {
        if (elementIndex > 0 && elements[elementIndex] != '-' &&
            elements[elementIndex - 1] != elements[elementIndex])
        {
            text += elements[elementIndex];
        }
    }
}