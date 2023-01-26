#define PI								3.1415926535897932384


class ImageProcess{
public:
	CString				CSChannel(int channel);
	void				InitParam(__RANSAC &rs);
	void				InitCircleParam(__Circle &rsc);
	void				InitRectangleParam(__Rectangle &rsr);
	BOOL				PrymidResize(Mat& mat, int rs = 0);			// 원본 변경
	BOOL				PrymidDenoise(Mat& mat, int dn = 0);		// 원본 변경
	BOOL				NormalizeImage(Mat src, Mat& rMat);
	BOOL				RansacPreprocess(Mat src, Mat& rMat, int gK, int nTH);
	__RANSAC			RansacProcess(int mode, Mat src, Rect roi, BOOL bBin = TRUE, int gK = 15, int nTH = 30, BOOL bImgDebug = FALSE, float minRadius = 50.0f, float InOutRatio = 0.7f, int InOffset = 15); // bBin 변수 : 이미 이진화되어 있는 이미지는 normalize 할필요 없어서 추가
	__Circle			RansacCircleFit(Mat src, Rect roi, BOOL bBin = TRUE, int gK = 15, int nTH = 30, BOOL bImgDebug = FALSE, float minRadius = 50.0f, float InOutRatio = 0.7f); // bBin 변수 : 이미 이진화되어 있는 이미지는 normalize 할필요 없어서 추가		
	__Rectangle			RansacLineFit(Mat src, Rect roi, BOOL bBin = TRUE, int gK = 15, int nTH = 30, BOOL bImgDebug = FALSE, float boundary = 50.0f, float InOutRatio = 0.7f, int InOffset = 15); // bBin 변수 : 이미 이진화되어 있는 이미지는 normalize 할필요 없어서 추가
	BOOL				RotateMat(Mat src, Mat& rMat, double degree);
	cv::Point			GetIntersectPoint2D(cv::Point x1, cv::Point x2, cv::Point x3, cv::Point x4);
	cv::Point2d			IntersectionPoint(const cv::Point2d* p1, const cv::Point2d* p2, const cv::Point2d* p3, const cv::Point2d* p4);
	double				GetAngle(cv::Point s_pos, cv::Point e_pos);
	void				edgeLines(Mat img, Mat &line_result, vector<Vec4i> lines);
	double				Distance(cv::Point p1, cv::Point p2);
};