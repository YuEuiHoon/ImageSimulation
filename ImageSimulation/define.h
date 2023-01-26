
#pragma warning(disable:4005)
#pragma warning(disable:4073)
#pragma warning(disable:4075)
#pragma warning(disable:4099)
#pragma warning(disable:4204)
#pragma warning(disable:4244)
#pragma warning(disable:4482)
#pragma warning(disable:4553)
#pragma warning(disable:4786)
#pragma warning(disable:4800)
#pragma warning(disable:4819)
#pragma warning(disable:4996)

// opencv 3.0 library
#pragma comment(lib, "opencv_core300d.lib")
#pragma comment(lib, "opencv_highgui300d.lib")
#pragma comment(lib, "opencv_imgproc300d.lib")
#pragma comment(lib, "opencv_imgcodecs300d.lib")

// evision 6.7 library
#pragma comment(lib, "EasyMs60d.lib")
#pragma comment(lib, "EObjMs60d.lib")
#pragma comment(lib, "EInsMs60d.lib")
#pragma comment(lib, "EGgeMs60d.lib")
#pragma comment(lib, "EImgMs60d.lib")
#pragma comment(lib, "EFndMs60d.lib")
#pragma comment(lib, "EMchMs60d.lib")
#pragma comment(lib, "EMxcMs60d.lib")

#pragma comment(lib, "Multicam.lib")

using namespace std;

// opencv 3.0
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "CvvImage.h"

using namespace cv;

// eVision 6.7
#include "eVision_6_7/easy.h"
#include "eVision_6_7/EImage.h"
#include "eVision_6_7/EFind.h"
#include "eVision_6_7/EObject.h"
#include "eVision_6_7/EMatch.h"
#include "eVision_6_7/EGauge.h"
#include "eVision_6_7/EMatrixCode.h"
#include "eVision_6_7/Easy_License.h"
///////////////////////////////////////////////

// eVision 2.17
// #include "evision_2_17/Open_eVision_2_17.h"
// using namespace Euresys::Open_eVision_2_17;


// 기본 문자 경로 define
#define RESULT_WINDOW		"Result Image"
#define INPUT_WINDOW		"Input Image"
#define BINARY_IMAGE		"Binary Image"
#define NORMALIZE_IMAGE		"Normalize Image"
#define PARAMETER_PATH		"system.ini"
#define LEARN_IMAGE_PATH	"LearnImage"

// 기본 숫자 define
#define TIMER_REFRESH		1111

#define LEFT				0
#define TOP					1
#define RIGHT				2
#define BOTTOM				3
#define SIDE				4

#define RANSAC_LINE			0
#define RANSAC_CIRCLE		1

// RANSAC Circle Fitting
#define CIRCLE_BIN			0		// 그냥 이진화
#define CIRCLE_NORMAL		1		// 노말라이즈 한 후 이진화

#define RECTANGLE_EVIEION	0		// 패턴매칭 이용
#define RECTANGLE_OPENCV	1		// RANSAC Line Fitting 활용


enum {
	// 빨간색
	RED					=	0x000000ff,
	LRED				=	0x008080ff,
	DRED				=	0x000000d0,

	// 핑크색
	PINK				=	0x00aaaaff,
	LPINK				=	0x00ccccff,
	DPINK				=	0x006666aa,

	// 주황색
	ORANGE				=	0x004060ff,
	LORANGE				=	0x009090ff,
	DORANGE				=	0x002040aa,

	// 노란색
	YELLOW				=	0x0000ffff,
	LYELLOW				=	0x0080ffff,
	DYELLOW				=	0x0000c0c0,

	// 초록색
	GREEN				=	0x0000ff00,
	LGREEN				=	0x0080ff80,
	DGREEN				=	0x00008000,

	// 파란색
	BLUE				=	0x00ff0000,
	LBLUE				=	0x00ffa0a0,
	DBLUE				=	0x00b00000,

	// 하늘색
	CYAN				=	0x00ffff80,
	LCYAN				=	0x00ffffc0,
	DCYAN				=	0x00ffff00,

	// 남색
	INDIGO				=	0x00800000,
	LINDIGO				=	0x00c00000,	
	DINDIGO				=	0x00600000,

	// 보라색
	PURPLE				=	0x00ff00ff,
	LPURPLE				=	0x00ffA0ff,
	DPURPLE				=	0x00b000b0,

	// 흑백
	WHITE				=	0x00ffffff,
	BLACK				=	0x00000000,
	BACK				=	0x00f0f0f0,

	// 회색
	GRAY				=	0x00909090,
	WGRAY				=	0x00c0c0c0,
	LGRAY				=	0x00b0b0b0,
	AGRAY				=	0x00a0a0a0,
	DGRAY				=	0x00707070,
};

struct __Circle{
	bool bFind;
	double radius;
	cv::Point center;
	float tact;
};

struct __Rectangle{
	bool bFind;
	double angle;
	cv::Point vertex[4];
	cv::Point center;
	float tact;
};

struct sPoint {
	double x, y;
};

struct sLine {
	double mx, my;
	double sx, sy;
};

struct __RANSAC{
	__Circle	ran_circ;
	__Rectangle	ran_rect;
};

struct __MATCH{
	bool bFind; 
	EMatchPosition mMCpos;
	float tact;
};

struct __MatchResult{
	__RANSAC mRS;
	__MATCH  mMC;
	double dist;	
};