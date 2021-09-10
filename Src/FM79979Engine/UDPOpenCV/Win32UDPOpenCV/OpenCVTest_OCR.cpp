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

using namespace cv;
using namespace cv::dnn;
void decodeBoundingBoxes(const Mat& scores, const Mat& geometry, float scoreThresh,
	std::vector<RotatedRect>& detections, std::vector<float>& confidences);
void fourPointsTransform(const Mat& frame, Point2f vertices[4], Mat& result);
void decodeText(const Mat& scores, std::string& text);

cOpenCVTest_OCR::cOpenCVTest_OCR()
{
	m_pVideoCapture = nullptr;
	m_pVideoImage = nullptr;
	m_pFrame = new sMatWithFlag();
	m_pCascadeClassifier = new cv::CascadeClassifier();
	if (!m_pCascadeClassifier->load("opencv/haarcascade_frontalface_alt2.xml"))
	{
		int a = 0;
	}
}

cOpenCVTest_OCR::~cOpenCVTest_OCR()
{
	SAFE_DELETE(m_pVideoCapture);
}

//
//const char* keys =
//"{ help  h     | | Print help message. }"
//"{ input i     | | Path to input image or video file. Skip this argument to capture frames from a camera.}"
//"{ model m     | | Path to a binary .pb file contains trained detector network.}"
//"{ ocr         | | Path to a binary .pb or .onnx file contains trained recognition network.}"
//"{ width       | 320 | Preprocess input image by resizing to a specific width. It should be multiple by 32. }"
//"{ height      | 320 | Preprocess input image by resizing to a specific height. It should be multiple by 32. }"
//"{ thr         | 0.5 | Confidence threshold. }"
//"{ nms         | 0.4 | Non-maximum suppression threshold. }";
//
int qoo(int argc, char** argv)
{
    // Parse command line arguments.
    CommandLineParser parser(argc, argv, "");
    parser.about("Use this script to run TensorFlow implementation (https://github.com/argman/EAST) of "
        "EAST: An Efficient and Accurate Scene Text Detector (https://arxiv.org/abs/1704.03155v2)");
    if (argc == 1 || parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
    float confThreshold = 0.5f;
    float nmsThreshold = 0.4f;
    int inpWidth = 320;
    int inpHeight = 320;
    String modelDecoder = "opencv/frozen_east_text_detection.pb";
    String modelRecognition = "opencv/frozen_east_text_detection.pb";

    CV_Assert(!modelDecoder.empty());
    // Load networks.
    Net detector = readNet(modelDecoder);
    Net recognizer;
    if (!modelRecognition.empty())
        recognizer = readNet(modelRecognition);
    // Open a video file or an image file or a camera stream.
    VideoCapture cap;
    bool openSuccess = parser.has("input") ? cap.open(parser.get<String>("input")) : cap.open(0);
    CV_Assert(openSuccess);
    static const std::string kWinName = "EAST: An Efficient and Accurate Scene Text Detector";
    namedWindow(kWinName, WINDOW_NORMAL);
    std::vector<Mat> outs;
    std::vector<String> outNames(2);
    outNames[0] = "feature_fusion/Conv_7/Sigmoid";
    outNames[1] = "feature_fusion/concat_3";
    Mat frame, blob;
    TickMeter tickMeter;
    while (waitKey(1) < 0)
    {
        cap >> frame;
        if (frame.empty())
        {
            waitKey();
            break;
        }
        blobFromImage(frame, blob, 1.0, Size(inpWidth, inpHeight), Scalar(123.68, 116.78, 103.94), true, false);
        detector.setInput(blob);
        tickMeter.start();
        detector.forward(outs, outNames);
        tickMeter.stop();
        Mat scores = outs[0];
        Mat geometry = outs[1];
        // Decode predicted bounding boxes.
        std::vector<RotatedRect> boxes;
        std::vector<float> confidences;
        decodeBoundingBoxes(scores, geometry, confThreshold, boxes, confidences);
        // Apply non-maximum suppression procedure.
        std::vector<int> indices;
        NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
        Point2f ratio((float)frame.cols / inpWidth, (float)frame.rows / inpHeight);
        // Render text.
        for (size_t i = 0; i < indices.size(); ++i)
        {
            RotatedRect& box = boxes[indices[i]];
            Point2f vertices[4];
            box.points(vertices);
            for (int j = 0; j < 4; ++j)
            {
                vertices[j].x *= ratio.x;
                vertices[j].y *= ratio.y;
            }
            if (!modelRecognition.empty())
            {
                Mat cropped;
                fourPointsTransform(frame, vertices, cropped);
                cvtColor(cropped, cropped, cv::COLOR_BGR2GRAY);
                Mat blobCrop = blobFromImage(cropped, 1.0 / 127.5, Size(), Scalar::all(127.5));
                recognizer.setInput(blobCrop);
                tickMeter.start();
                Mat result = recognizer.forward();
                tickMeter.stop();
                std::string wordRecognized = "";
                decodeText(result, wordRecognized);
                putText(frame, wordRecognized, vertices[1], FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 255));
            }
            for (int j = 0; j < 4; ++j)
                line(frame, vertices[j], vertices[(j + 1) % 4], Scalar(0, 255, 0), 1);
        }
        // Put efficiency information.
        std::string label = format("Inference time: %.2f ms", tickMeter.getTimeMilli());
        putText(frame, label, Point(0, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0));
        imshow(kWinName, frame);
        tickMeter.reset();
    }
    return 0;
}


void cOpenCVTest_OCR::CameraReadThread(float e_fElpaseTime)
{
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
	//m_pVideoCapture = new cv::VideoCapture();
	//m_pVideoCapture->open(e_strCameraURL);
}
//opencv-4.4.0-vc14_vc15\opencv\build\testdata\cv\face\face_landmark_model.dat
void cOpenCVTest_OCR::Update(float e_fElpaseTime)
{
	CameraReadThread(e_fElpaseTime);
	if (m_pFrame)
	{
		//MUTEX_PLACE_HOLDER(m_FrameMutex, "cOpenCVTest_OCR::CameraReadThread");
#ifdef WIN32
		GLenum inputColourFormat = GL_BGR;
#else
		GLenum inputColourFormat = GL_RGB;
#endif
		if (!m_pVideoImage)
		{
			//m_pVideoImage = new cBaseImage(L"OpenCVToOpenGLTexture");
			m_pVideoImage = new FATMING_CORE::cBaseImage((const char*)"");
			m_pVideoImage->SetName(L"OpenCVToOpenGLTexture");
		}
		{
			MUTEX_PLACE_HOLDER(m_FrameMutex, "cOpenCVTest_OCR::CameraReadThread");
			if (m_pFrame->bNewData)
			{
				m_pVideoImage->SetupTexture(3, m_pFrame->pFrame->cols, m_pFrame->pFrame->rows, inputColourFormat, GL_UNSIGNED_BYTE, false, (GLvoid*)m_pFrame->pFrame->data, false);
				m_pFrame->bNewData = false;
			}
		}
	}
}

void cOpenCVTest_OCR::Render()
{
	if (m_pVideoImage)
	{
		m_pVideoImage->Render();

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