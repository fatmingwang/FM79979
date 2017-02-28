#include <vector>
#include <complex>
//lazy to do this now.
class cChartAssist
{
	//	std::vector<Vector2>				m_XAxis;
	//	std::vector<Vector2>				m_YAxis;
	//	std::vector<std::vector<Vector2>*>	m_DataVector;
	//	float					m_fScale;
	//public:
	//	cChartAssist();
	//	~cChartAssist();
	//	void	SetData(int e_iXAxisValue,int e_iYAxisValue,std::vector<Vector2>*e_pData);
	//	void	Render();
//	complex ll;
};

//#include < cv.h >
//#include < highgui.h >
//#include < al.h >
//#include < alc.h >
//#include < iostream >
//#include < stdio.h >
//#include < windows.h >
//#include < conio.h >
//#include < fftw3.h >
//#include < math.h >
//
//#define BUFFERSIZE 8820
//#define Nr 4410 // # real number
//#define Nc floor( (double)Nr/2.0 )+1 // # fourier number
//
//using namespace std;
//
//const int SRATE = 44100; // sampling rate. means 44100 samples per second (CD quality)
//const int SSIZE = 4410; // the size of the ring buffer, Sampling-Rate * Seconds * Resolution * Trackse
//
//ALshort buffer[BUFFERSIZE]; // ALbyte: signed 8-bit 2's-complement integer
//ALint sample; // ALint: signed 32-bit 2's-complement integer
//
//int main(int argc, char *argv[])
//{
//	int i,j,k,l;
//
//	double *FT_in;
//	fftw_complex *FT_out; //直角坐標
//	double *FT_Amp; //極座標大小
//	fftw_plan FT_plan;
//
//	FT_in = (double*) fftw_malloc(sizeof(double) * Nr);
//	FT_out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * Nc);
//	FT_Amp = (double*) fftw_malloc(sizeof(double) * Nc);
//	FT_plan = fftw_plan_dft_r2c_1d(Nr, FT_in, FT_out, FFTW_ESTIMATE);
//
//
//	IplImage *image1;
//	image1 = cvCreateImage(cvSize(1103,500),IPL_DEPTH_8U,3); // FFT Transform
//
//	alGetError(); // This function returns the current error state and then clears the error state
//
//	ALCdevice *device = alcCaptureOpenDevice(NULL, SRATE, AL_FORMAT_MONO16, SSIZE);
//
//	if (alGetError() != AL_NO_ERROR) {
//		cout << "Error!" << endl;
//		return 0;
//	}
//	alcCaptureStart(device);
//
//	const ALchar *actualDeviceName;
//	actualDeviceName = alcGetString(device, ALC_DEVICE_SPECIFIER);
//
//	cout << "actualDeviceName: " << actualDeviceName << endl;
//	cout << "device adress: " << device << endl;
//	system("PAUSE");
//
//	while(1)
//	{
//		Sleep(1);
//		alcGetIntegerv(device, ALC_CAPTURE_SAMPLES, 1, &sample);
//
//		if(sample >= 4410)
//		{
//			alcCaptureSamples(device, (ALCvoid *)buffer, sample);
//
//			for(i = 0; i < 4410; i++) FT_in[i] = buffer[i];
//			fftw_execute(FT_plan);
//			for(i = 0; i < Nc; i++)
//			{
//				FT_Amp[i] = sqrt( pow(FT_out[i][0],2) + pow(FT_out[i][1],2) ); // amplitude of polar coordinate
//			}
//
//			cvSetZero(image1);
//			for(int j = 0; j < image1->widthStep; j+=3)
//			{
//				k = j/3;
//				l = (float) log10(FT_Amp[k]/10.0+1)*80.0;
//
//				for(int i = image1->height -1 - l; i < image1->height; i++)
//				{
//
//					image1->imageData[i*image1->widthStep + j ] = 255;
//					image1->imageData[i*image1->widthStep + j +1] = 255;
//					image1->imageData[i*image1->widthStep + j +2] = 255;
//				}
//			}
//
//			cvNamedWindow("Freq",1);
//			cvShowImage("Freq",image1);
//
//			if(cvWaitKey (1) == 'q') break;
//		}
//
//	}
//
//	alcCaptureStop(device);
//	alcCaptureCloseDevice(device);
//
//	cvDestroyWindow("Freq");
//	cvReleaseImage(&image1);
//
//	return 0;
//}