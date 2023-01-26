#include "stdafx.h"
#include "ImageProcess.h"

bool comparePerc(Vec4f vec1, Vec4f vec2)
{
	double i = double(vec1[0]);
	double j = double(vec2[0]);
	return (i > j);
}

<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
bool compareContourAreas(vector<cv::Point> contour1, vector<cv::Point> contour2)
{
	double i = fabs(contourArea(Mat(contour1)));
	double j = fabs(contourArea(Mat(contour2)));
	return (i > j);
}

<<<<<<< HEAD
=======
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
float verifyCircle(Mat dt, Point2f center, float radius, vector<Point2f> & inlierSet)
{
	unsigned int counter = 0;
	unsigned int inlier = 0;
	float minInlierDist = 2.0f;
	float maxInlierDistMax = 50.0f;
	float maxInlierDist = radius/50.0f;
	if(maxInlierDist<minInlierDist) maxInlierDist = minInlierDist;
	if(maxInlierDist>maxInlierDistMax) maxInlierDist = maxInlierDistMax;

	// choose samples along the circle and count inlier percentage
	float t;
	for( t =0; t<2*3.14159265359f; t+= 0.05f)
	{
		counter++;
		float cX = radius*cos(t) + center.x;
		float cY = radius*sin(t) + center.y;

		if(cX < dt.cols)
			if(cX >= 0)
				if(cY < dt.rows)
					if(cY >= 0)
						if(dt.at<float>(cY,cX) < maxInlierDist)
						{
							inlier++;
							inlierSet.push_back(Point2f(cX,cY));
						}
	}

	return (float)inlier/float(counter);
}

inline void getCircle(Point2f& p1,Point2f& p2,Point2f& p3, Point2f& center, float& radius)
{
	float x1 = p1.x;
	float x2 = p2.x;
	float x3 = p3.x;

	float y1 = p1.y;
	float y2 = p2.y;
	float y3 = p3.y;

	// PLEASE CHECK FOR TYPOS IN THE FORMULA :)
	center.x = (x1*x1+y1*y1)*(y2-y3) + (x2*x2+y2*y2)*(y3-y1) + (x3*x3+y3*y3)*(y1-y2);
	center.x /= ( 2*(x1*(y2-y3) - y1*(x2-x3) + x2*y3 - x3*y2) );

	center.y = (x1*x1 + y1*y1)*(x3-x2) + (x2*x2+y2*y2)*(x1-x3) + (x3*x3 + y3*y3)*(x2-x1);
	center.y /= ( 2*(x1*(y2-y3) - y1*(x2-x3) + x2*y3 - x3*y2) );

	radius = sqrt((center.x-x1)*(center.x-x1) + (center.y-y1)*(center.y-y1));
}

vector<Point2f> getPointPositions(Mat binaryImage)
{
	vector<Point2f> pointPositions;

	for(unsigned int y=0; y<binaryImage.rows; ++y)
	{
		//unsigned char* rowPtr = binaryImage.ptr<unsigned char>(y);
		for(unsigned int x=0; x<binaryImage.cols; ++x)
		{
			//if(rowPtr[x] > 0) pointPositions.push_back(cv::Point2i(x,y));
			if(binaryImage.at<unsigned char>(y,x) > 0) pointPositions.push_back(cv::Point2f(x,y));
		}
	}

	return pointPositions;
}

BOOL ImageProcess::PrymidResize(Mat& mat, int rs)
{
	// Image가 클때 파리미드 알고리즘으로 축소함
	Mat _mat;
	_mat = mat.clone();

//	if(mat.rows >= 1000 || mat.cols >= 1000)
//	{
		// count 는 이미지 피라미드 다운을 몇번할지 선택 -> 사이즈 축소 목적
	for(int r = 0; r < rs; r++)
		pyrDown(_mat, _mat, Size( _mat.cols/2, _mat.rows/2 ));		
//	}

	mat.release();
	mat = _mat.clone();

	return TRUE;
}
BOOL ImageProcess::PrymidDenoise(Mat& mat, int dn)
{
	// 노이즈 제거를 위해 이미지의 Scale을 축소 확대 진행
	// 설정된 카운트만큼 진행한다
	Mat _mat;
	_mat = mat.clone();

	for(int i = 0 ; i < dn; i++)
	{
		pyrDown(_mat, _mat, Size( _mat.cols/2, _mat.rows/2 ));
		pyrUp(_mat, _mat, Size( _mat.cols*2, _mat.rows*2 ));
	}

	mat.release();
	mat = _mat.clone();

	return TRUE;
}

CString	ImageProcess::CSChannel(int channel)
{
	CString _sR;
	
	switch (channel)
	{
	case 1:		_sR.Format("GRAY");		break;
	case 3:		_sR.Format("COLOR");	break;
	default:	_sR.Format("Non");		break;	
	}

	return _sR;
}


BOOL ImageProcess::NormalizeImage(Mat src, Mat& rMat)
{
	// normalize Image는 gray 이미지만 대응하게 해놓음
	Mat nor_s;

	if(src.channels() == 3)
		cvtColor(src, nor_s, COLOR_BGR2GRAY);
	else
		nor_s = src.clone();

	// now map brightest pixel to 255 and smalles pixel val to 0. this is for easier finding of threshold
	double min, max;
	minMaxLoc(nor_s,&min,&max);

	float sub = min;
	float mult = 255.0f/(float)(max-sub);

	Mat normalized = nor_s - sub;
	normalized = mult * normalized;
	rMat = normalized.clone();
	return TRUE;
}

BOOL ImageProcess::RansacPreprocess(Mat src, Mat& rMat, int gK, int nTH)
{
	// Image 깊은복사 원본이미지 손상시키지 않음
	Mat circle_image = src.clone();

	// 이미지가 없을때는 리턴	
	if(circle_image.empty())
		return FALSE;

	Mat canny_image;

	// 컬러 이미지의 경우 흑백 전환, 흑백이미지의 경우 그대로 복사
	if(circle_image.channels() == 1)
		canny_image = circle_image.clone();
	else
		cvtColor(circle_image, canny_image, CV_BGR2GRAY);

	// 가우시안 블러처리
	GaussianBlur(canny_image, canny_image, Size(gK, gK), 0);
	// 주어진 임계값으로 이미지 이진화 진행
	threshold(canny_image, canny_image, nTH, 255, THRESH_BINARY);

	// 캐니엣지를 활용해 엣지 성분 검출
	Canny(canny_image, canny_image, 125, 125*3,3);
	rMat = canny_image.clone();

	return TRUE;
}

<<<<<<< HEAD
__Circle ImageProcess::RansacCircleFit(Mat src, Rect roi, BOOL bBin, int gK, int nTH, BOOL bImgDebug, float minRadius, float InOutRatio)
=======
<<<<<<< HEAD
<<<<<<< HEAD
__Circle ImageProcess::RansacCircleFit(Mat src, Rect roi, BOOL bBin, int gK, int nTH, BOOL bImgDebug, float minRadius, float InOutRatio)
=======
vector<__Circle> ImageProcess::RansacCircleFit(Mat src, Rect roi, BOOL bBin, int gK, int nTH, BOOL bImgDebug)
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
vector<__Circle> ImageProcess::RansacCircleFit(Mat src, Rect roi, BOOL bBin, int gK, int nTH, BOOL bImgDebug)
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
{
	Mat color;
	Mat gray, normalized, gray_resize;

	// convert to grayscale
	if(src.channels() == 3)
	{
		cvtColor(src, gray, CV_BGR2GRAY);
		color = src.clone();
	}
	else
	{
		gray = src.clone();
		color = Mat::zeros(src.rows, src.cols, CV_8UC3);
		cvtColor(gray, color, CV_GRAY2BGR);
	}

	// ROI 설정
	gray_resize = gray(Rect(roi.x, roi.y, roi.width, roi.height)).clone();  // 깊은 복사

	Mat mask;
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618

// 	if(!bBin)
// 	{
// 		// image normalize
// 		NormalizeImage(gray_resize, normalized);
// 		
// 		if(bImgDebug)
// 			imshow(NORMALIZE_IMAGE, normalized);
// 		//--------------------------------
// 
// 		// now compute threshold
// 		// TODO: this might ne a tricky task if noise differs...		
// 		//cv::threshold(input, mask, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
// 		//		threshold(normalized, mask, nTH, 255, CV_THRESH_BINARY);
// 
// 		// 가우시안 블러처리
// 		GaussianBlur(normalized, normalized, Size(gK, gK), 0);
// 		// 주어진 임계값으로 이미지 이진화 진행
// 		threshold(normalized, normalized, nTH, 255, THRESH_BINARY);
// 
// 		// 캐니엣지를 활용해 엣지 성분 검출
// 		Canny(normalized, mask, nTH, 125*3,3);
// 	}
// 	else
// 		mask = gray_resize.clone();	

<<<<<<< HEAD
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
	if(!bBin)
	{
		// image normalize
		NormalizeImage(gray_resize, normalized);		
	}
	else
		normalized = gray_resize.clone();

	if(bImgDebug)
		imshow(NORMALIZE_IMAGE, normalized);

	// 가우시안 블러처리
	GaussianBlur(normalized, normalized, Size(gK, gK), 0);
	// 주어진 임계값으로 이미지 이진화 진행
	threshold(normalized, normalized, nTH, 255, THRESH_BINARY);

	// 캐니엣지를 활용해 엣지 성분 검출
	Canny(normalized, mask, nTH, 125*3,3);

	if(bImgDebug)
		imshow(BINARY_IMAGE, mask);

	vector<Point2f> edgePositions;
	edgePositions = getPointPositions(mask);

	// create distance transform to efficiently evaluate distance to nearest edge
	Mat dt;
	distanceTransform(255-mask, dt,CV_DIST_L1, 3);

	// Float 형 이미지 보여주기
	//floatImageShow(dt);

	//TODO: maybe seed random variable for real random numbers.
	unsigned int nIterations = 0;

	cv::Point2f bestCircleCenter;
	float bestCircleRadius;
	float bestCirclePercentage = 0;
<<<<<<< HEAD
	//float minRadius = 50;   // TODO: ADJUST THIS PARAMETER TO YOUR NEEDS, otherwise smaller circles wont be detected or "small noise circles" will have a high percentage of completion
=======
<<<<<<< HEAD
<<<<<<< HEAD
	//float minRadius = 50;   // TODO: ADJUST THIS PARAMETER TO YOUR NEEDS, otherwise smaller circles wont be detected or "small noise circles" will have a high percentage of completion
=======
	float minRadius = 100;   // TODO: ADJUST THIS PARAMETER TO YOUR NEEDS, otherwise smaller circles wont be detected or "small noise circles" will have a high percentage of completion
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
	float minRadius = 100;   // TODO: ADJUST THIS PARAMETER TO YOUR NEEDS, otherwise smaller circles wont be detected or "small noise circles" will have a high percentage of completion
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c

	float minCirclePercentage = 0.03f;  // at least 5% of a circle must be present? maybe more...

	int maxNrOfIterations = edgePositions.size();   // TODO: adjust this parameter or include some real ransac criteria with inlier/outlier percentages to decide when to stop
<<<<<<< HEAD
	
	__Circle bestCircles;
	InitCircleParam(bestCircles);
=======
<<<<<<< HEAD
<<<<<<< HEAD
	
	__Circle bestCircles;
	InitCircleParam(bestCircles);
=======

	vector<__Circle> bestCirclesHough;
	__Circle c_result;
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======

	vector<__Circle> bestCirclesHough;
	__Circle c_result;
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c

	vector<Vec4f> fvResult;
	Vec4f fvTemp;
	// fvResult[0]; Percent
	// fvResult[1]; radius
	// fvResult[2]; xCenter
	// fvResult[3]; yCenter

	double dbPercent = 0.99;		// 확률적으로 얼마나 정확한 결과를 기대하는가?
<<<<<<< HEAD
	//double InOutRatio = 0.7;		// Inlier 와 outlier 의 비율이 얼마인가? 낮을수록 거짓데이터가 많은 것
=======
<<<<<<< HEAD
<<<<<<< HEAD
	//double InOutRatio = 0.7;		// Inlier 와 outlier 의 비율이 얼마인가? 낮을수록 거짓데이터가 많은 것
=======
	double InOutRatio = 0.3;		// Inlier 와 outlier 의 비율이 얼마인가? 낮을수록 거짓데이터가 많은 것
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
	double InOutRatio = 0.3;		// Inlier 와 outlier 의 비율이 얼마인가? 낮을수록 거짓데이터가 많은 것
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
	int nSampling;					// 실패확률 계산치
	// ex dbPercent = 0.99, InOutRatio = 0.7 이라면 n sampling은 48번 돌리면 0.99 퍼센트의 동일한 결과가 나옴
	// 48번이나 돌리려면 시간이 너무많이 걸림
	// RANSAC의 단점 : 확률적인 데이터기 때문에 정확도를 올리려면 시간이 너무 걸리고 시간을 줄이려면 부정확한 결과가 나올 수 있음
	// 단점 해결 방법 : 전처리 과정에서 이진화 contour에 대한 데이터를 간소화하면 되지 않을까?
	// 단점 해결 방법 : 굉장한 정확도를 요하는게 아니라면 어느정도의 타협이 필요함
	// 단점 해결 방법 : 시간이 넉넉하다면 0.99, 0.7로 셋팅하여 정확도를 늘려도 됨

	nSampling = log(1-dbPercent) / log(1-pow((1-InOutRatio), 2));

	int samecount = 0;
	for(unsigned int its=0; its< maxNrOfIterations * nSampling; ++its)
	{
		//RANSAC: randomly choose 3 point and create a circle:
		//TODO: choose randomly but more intelligent,
		//so that it is more likely to choose three points of a circle.
		//For example if there are many small circles, it is unlikely to randomly choose 3 points of the same circle.
		unsigned int idx1 = rand()%edgePositions.size();
		unsigned int idx2 = rand()%edgePositions.size();
		unsigned int idx3 = rand()%edgePositions.size();


		// we need 3 different samples:
		if(idx1 == idx2) idx2 = rand()%edgePositions.size();
		if(idx1 == idx3) idx3 = rand()%edgePositions.size();
		if(idx3 == idx2) idx2 = rand()%edgePositions.size();

		if(idx1 == idx2) continue;
		if(idx1 == idx3) continue;
		if(idx3 == idx2) continue;

		// create circle from 3 points:
		Point2f center; float radius;
		getCircle(edgePositions[idx1],edgePositions[idx2],edgePositions[idx3],center,radius);

		// inlier set unused at the moment but could be used to approximate a (more robust) circle from alle inlier
		vector<Point2f> inlierSet;

		//verify or falsify the circle by inlier counting:
		float cPerc = verifyCircle(dt,center,radius, inlierSet);

		// update best circle information if necessary
		if(cPerc >= bestCirclePercentage)
			if(radius >= minRadius)
			{
				bestCirclePercentage = cPerc;
				bestCircleRadius = radius;
				bestCircleCenter = center;

<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
				fvTemp[0] = cPerc;
				fvTemp[1] = bestCircleRadius;
				fvTemp[2] = bestCircleCenter.x+roi.x;
				fvTemp[3] = bestCircleCenter.y+roi.y;
				fvResult.push_back(fvTemp);
<<<<<<< HEAD
=======
=======
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
				c_result.radius = bestCircleRadius;
				c_result.xCenter = bestCircleCenter.x+roi.x;
				c_result.yCenter = bestCircleCenter.y+roi.y;

				fvTemp[0] = cPerc;
				fvTemp[1] = bestCircleRadius;
				fvTemp[2] = c_result.xCenter;
				fvTemp[3] = c_result.yCenter;
				fvResult.push_back(fvTemp);

<<<<<<< HEAD
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
			}
	}

	// draw if good circle was found
	if(bestCirclePercentage >= minCirclePercentage)
	{
		if(bestCircleRadius >= minRadius)
		{
			circle(color, cv::Point(bestCircleCenter.x + roi.x, bestCircleCenter.y + roi.y), bestCircleRadius, Scalar(255,255,0),1);
			circle(color, cv::Point(bestCircleCenter.x + roi.x, bestCircleCenter.y + roi.y), 2, Scalar(255,0,0),2);
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c

			bestCircles.radius = bestCircleRadius;
			bestCircles.center.x = bestCircleCenter.x+roi.x;
			bestCircles.center.y = bestCircleCenter.y+roi.y;
			bestCircles.bFind = TRUE;
		}
	}

	// 결과 확인용 이미지 처리
/*
<<<<<<< HEAD
=======
=======
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
			c_result.radius = bestCircleRadius;
			c_result.xCenter = bestCircleCenter.x+roi.x;
			c_result.yCenter = bestCircleCenter.y+roi.y;
			bestCirclesHough.push_back(c_result);
		}
	}

<<<<<<< HEAD
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
	sort(fvResult.begin(), fvResult.end(), comparePerc);

	for(int i = 0; i < fvResult.size(); i++)
	{
		double r,g,b;

		if(i == 0)
		{
			b = 255;	g = 0;					r = 0;
		}
		else
		{
			b = 0;		g = rand()%100;			r = rand()%100;
		}
<<<<<<< HEAD
		
=======
<<<<<<< HEAD
<<<<<<< HEAD
		
=======
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
		//r = rand()%255;
		

		// fvResult[0]; Percent
		// fvResult[1]; radius
		// fvResult[2]; xCenter
		// fvResult[3]; yCenter

<<<<<<< HEAD
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
		circle(color, cv::Point(fvResult[i][2], fvResult[i][3]), fvResult[i][1], Scalar(r,g,b),2);
		circle(color, cv::Point(fvResult[i][2], fvResult[i][3]), 2, Scalar(r,g,b),2);

		char buf1[100];
		sprintf_s(buf1, "Percent : %f", fvResult[i][0]);

		putText(color, buf1, cv::Point(20, (i+1)*30 ), CV_FONT_HERSHEY_SIMPLEX, 0.8, Scalar(r,g,b), 2, 1);
	}

<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
	namedWindow("output", WINDOW_NORMAL);
	imshow("output",color);	
 	waitKey(0);
*/

	return bestCircles;
}



__RANSAC ImageProcess::RansacProcess(int mode, Mat src, Rect roi, BOOL bBin, int gK, int nTH, BOOL bImgDebug, float minRadius, float InOutRatio, int InOffset)
{
	__Rectangle ran_rect;
	__Circle ran_circ;
	__RANSAC bestRansac;

	InitParam(bestRansac);

	LARGE_INTEGER timer, start, end;

	QueryPerformanceFrequency(&timer);
	QueryPerformanceCounter(&start);

	if(mode == RANSAC_LINE)
		ran_rect = RansacLineFit(src, roi, bBin, gK, nTH, bImgDebug, minRadius, InOutRatio, InOffset);
	else
		ran_circ = RansacCircleFit(src, roi, bBin, gK, nTH, bImgDebug, minRadius, InOutRatio);

	QueryPerformanceCounter(&end);

	if(mode == RANSAC_LINE)
	{
		if(ran_rect.bFind == TRUE)
		{
			bestRansac.ran_rect = ran_rect;
			bestRansac.ran_rect.tact = (end.QuadPart - start.QuadPart)/(float)(timer.QuadPart)*1000;
		}
	}
	else
	{
		if(ran_circ.bFind == TRUE)
		{
			bestRansac.ran_circ = ran_circ;
			bestRansac.ran_circ.tact = (end.QuadPart - start.QuadPart)/(float)(timer.QuadPart)*1000;
		}
	}



	return bestRansac;
}


bool find_in_samples (sPoint *samples, int no_samples, sPoint *data)
{
	for (int i=0; i<no_samples; ++i) {
		if (samples[i].x == data->x && samples[i].y == data->y) {
			return true;
		}
	}
	return false;
}

void get_samples (sPoint *samples, int no_samples, sPoint *data, int no_data)
{
	// 데이터에서 중복되지 않게 N개의 무작위 셈플을 채취한다.
	for (int i=0; i<no_samples; ) {
		int j = rand()%no_data;

		if (!find_in_samples(samples, i, &data[j])) {
			samples[i] = data[j];
			++i;
		}
	};
}

int compute_model_parameter(sPoint samples[], int no_samples, sLine &model)
{
	// PCA 방식으로 직선 모델의 파라메터를 예측한다.

	double sx  = 0, sy  = 0;
	double sxx = 0, syy = 0;
	double sxy = 0, sw  = 0;

	for(int i = 0; i<no_samples;++i)
	{
		double &x = samples[i].x;
		double &y = samples[i].y;

		sx  += x;	
		sy  += y;
		sxx += x*x; 
		sxy += x*y;
		syy += y*y;
		sw  += 1;
	}

	//variance;
	double vxx = (sxx - sx*sx/sw)/sw;
	double vxy = (sxy - sx*sy/sw)/sw;
	double vyy = (syy - sy*sy/sw)/sw;

	//principal axis
	double theta = atan2(2*vxy, vxx - vyy)/2;

	model.mx = cos(theta);
	model.my = sin(theta);

	//center of mass(xc, yc)
	model.sx = sx/sw;
	model.sy = sy/sw;

	//직선의 방정식: sin(theta)*(x - sx) = cos(theta)*(y - sy);
	return 1;
}

double compute_distance(sLine &line, sPoint &x)
{
	// 한 점(x)로부터 직선(line)에 내린 수선의 길이(distance)를 계산한다.

	return fabs((x.x - line.sx)*line.my - (x.y - line.sy)*line.mx)/sqrt(line.mx*line.mx + line.my*line.my);
}

double model_verification (sPoint *inliers, int *no_inliers, sLine &estimated_model, sPoint *data, int no_data, double distance_threshold)
{
	*no_inliers = 0;

	double cost = 0.;

	for(int i=0; i<no_data; i++){
		// 직선에 내린 수선의 길이를 계산한다.
		double distance = compute_distance(estimated_model, data[i]);

		// 예측된 모델에서 유효한 데이터인 경우, 유효한 데이터 집합에 더한다.
		if (distance < distance_threshold) {
			cost += 1.;

			inliers[*no_inliers] = data[i];
			++(*no_inliers);
		}
	}

	return cost;
}

__Rectangle ImageProcess::RansacLineFit(Mat src, Rect roi, BOOL bBin, int gK, int nTH, BOOL bImgDebug, float boundary, float InOutRatio, int InOffset)
{	
	__Rectangle bestLine;
	InitRectangleParam(bestLine);

	Mat rans, binary, fg, cont;
	rans = src.clone();

	if(rans.empty())
		return bestLine;
	Mat rans_roi(rans, Rect(roi.x, roi.y, roi.width, roi.height));

	cont = Mat::zeros(rans_roi.size(), CV_8UC3);
	cvtColor(rans_roi, cont, CV_GRAY2BGR);

	if(bImgDebug)
	{
		namedWindow("ROI", WINDOW_NORMAL);
		namedWindow("Binary Image", WINDOW_NORMAL);
		namedWindow("Foreground Image", WINDOW_NORMAL);
		namedWindow("Ransac-Line", WINDOW_NORMAL);		
	}	

	threshold(rans_roi, binary, nTH, 255, THRESH_BINARY_INV);

	// 잡음과 작은 객체 제거	
	erode(binary, fg, cv::Mat(), cv::Point(-1, -1), 1);

	dilate(fg, fg, cv::Mat(), cv::Point(-1, -1), 1);
	erode(fg, fg, cv::Mat(), cv::Point(-1, -1), 1);
	dilate(fg, fg, cv::Mat(), cv::Point(-1, -1), 1);
	erode(fg, fg, cv::Mat(), cv::Point(-1, -1), 1);
	dilate(fg, fg, cv::Mat(), cv::Point(-1, -1), 1);

	Mat ct;
	ct = fg.clone();																		// 모니터링 할때 디버깅 이미지가 깨져서 복사한 후 진행

	vector<vector<cv::Point>> contours;														// 이진화된 영상의 외곽선 추출
	findContours(ct, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	
	// 컨투어의 크기 비교해서 큰 순서대로 Sorting
	sort(contours.begin(), contours.end(), compareContourAreas);						// 컨투어의 사이즈를 큰순서대로 Sorting
	
	Rect boundRect;
	boundRect = boundingRect(Mat(contours[0]));

	// 상하좌우 gap을 두고 좌표들을 뽑아보자
	vector<vector<cv::Point>> v_cont(4);
	vector<vector<cv::Point>> v_cont_loc(4);
	vector<cv::Point> ptemp(1);
	int location = 0;

	for (int i = 0; i < (int)contours[0].size(); i++)
	{
		ptemp[0].x = contours[0][i].x;
		ptemp[0].y = contours[0][i].y;

		if(abs(boundRect.x - ptemp[0].x) < boundary)
			location = LEFT;
		else if(abs((boundRect.x+boundRect.width) - ptemp[0].x) < boundary)
			location = RIGHT;
		else if(abs(boundRect.y - ptemp[0].y) < boundary)
			location = TOP;
		else if(abs((boundRect.y+boundRect.height) - ptemp[0].y) < boundary)
			location = BOTTOM;
		else
			location = SIDE;

		if(location != SIDE)
			v_cont[location].push_back(ptemp[0]);
	}

	//////////////////////////////////////////////////////
	// 너비와 높이를 기준으로 이상적인 좌표만 뽑아 내게 하자
	// 렉트를 기준으로 상하좌우 이상한 값을 제거

	for (int i = 0; i < (int)v_cont.size(); i++)
	{
		Rect rbRect;
		int subWidth = 0;
		int subHeight = 0;

		rbRect = boundingRect(v_cont[i]);
		subWidth = rbRect.width*InOffset/100;						// 좌우 양쪽 끝의 10프로씩은 버림 
		subHeight = rbRect.height*InOffset/100;					// 상하 양쪽 끝의 10프로씩은 버림

		for (int c = 0; c < (int)v_cont[i].size(); c++)
		{
			if(i == LEFT || i == RIGHT)
			{
				// 좌,우측변이니까 상하 높이의 10프로씩은 버리자
				if( (rbRect.y+subHeight < v_cont[i][c].y) && (rbRect.y+rbRect.height-subHeight > v_cont[i][c].y))
				{
					ptemp[0].x = v_cont[i][c].x;
					ptemp[0].y = v_cont[i][c].y;

					v_cont_loc[i].push_back(ptemp[0]);
				}
			}
			else 
			{
				// 상,하측변이니까 상하 너비의 10프로씩은 버리자
				if( (rbRect.x+subWidth < v_cont[i][c].x) && (rbRect.x+rbRect.width-subWidth > v_cont[i][c].x))
				{
					ptemp[0].x = v_cont[i][c].x;
					ptemp[0].y = v_cont[i][c].y;
					v_cont_loc[i].push_back(ptemp[0]);
				}
			}
		}
	}

	sLine line[4];
	cv::Vec4i vIntersection[4]; // 두좌표를 한변수에 담기위해.. 
	cv::Point2d pCross[4];
	double angle[4];

	for (int location = 0; location < 4; location++)
	{
		// 결과를 확인하기 위한 칼라 이미지에 컨투어 복사
// 		r = 255;	g = rand()%255;	b = rand()%255;
// 		drawContours(cont, v_cont_loc, location, Scalar(b,g,r), 1);

		double distance_threshold = 20;
		int no_data = v_cont_loc[location].size();
		sPoint *data = new sPoint[no_data];

		for (int i=0; i<no_data; ++i) 
		{
			data[i].x = v_cont_loc[location][i].x;
			data[i].y = v_cont_loc[location][i].y;
		}

		const int no_samples = 2;

		if (no_data < no_samples) {
			return bestLine;
		}

		sPoint *samples = new sPoint[no_samples];

		int no_inliers = 0;
		sPoint *inliers = new sPoint[no_data];

		sLine estimated_model;
		double max_cost = 0.;

		int max_iteration = (int)(1 + log(1. - 0.99)/log(1. - pow(InOutRatio, 2)));
		
		double dist=0., max_dist=0.;
		int best1_x, best1_y;
		int best2_x, best2_y;
		for (int i = 0; i<max_iteration; i++) 
		{
			// 1. hypothesis

			// 원본 데이터에서 임의로 N개의 셈플 데이터를 고른다.
			//get_samples (samples, no_samples, data, no_data);

			unsigned int idx1 = rand()%no_data;
			unsigned int idx2 = rand()%no_data;

			// we need 3 different samples:
			if(idx1 == idx2) idx2 = rand()%no_data;

			if(idx1 == idx2) continue;

			samples[0].x = v_cont_loc[location][idx1].x;
			samples[0].y = v_cont_loc[location][idx1].y;
			samples[1].x = v_cont_loc[location][idx2].x;
			samples[1].y = v_cont_loc[location][idx2].y;

			// 이 데이터를 정상적인 데이터로 보고 모델 파라메터를 예측한다.
			compute_model_parameter (samples, no_samples, estimated_model);

			// 2. Verification

			// 원본 데이터가 예측된 모델에 잘 맞는지 검사한다.
			double cost = model_verification (inliers, &no_inliers, estimated_model, data, no_data, distance_threshold);
			dist = Distance(cv::Point(samples[0].x, samples[0].y), cv::Point(samples[1].x, samples[1].y));

			// 만일 예측된 모델이 잘 맞는다면, 이 모델에 대한 유효한 데이터로 새로운 모델을 구한다.
			if (max_cost <= cost && max_dist <= dist) {
				max_cost = cost;
				max_dist = dist;

				compute_model_parameter (inliers, no_inliers, line[location]);
				best1_x = samples[0].x;
				best1_y = samples[0].y;
				best2_x = samples[1].x;
				best2_y = samples[1].y;
			}
		}		
		delete [] samples;
		delete [] inliers;
		delete [] data;

		vIntersection[location][0] = (int)(line[location].sx-100*line[location].mx);
		vIntersection[location][1] = (int)(line[location].sy-100*line[location].my);
		vIntersection[location][2] = (int)(line[location].sx+100*line[location].mx);
		vIntersection[location][3] = (int)(line[location].sy+100*line[location].my);

		//		if(100. < max_cost)
		//			cv::line(cont, cv::Point(vIntersection[location][0], vIntersection[location][1]), cv::Point(vIntersection[location][2], vIntersection[location][3]), Scalar(0,0,255), 1);
		//cv::line(cont, cv::Point(vIntersection[location][0], vIntersection[location][1]), cv::Point(vIntersection[location][2], vIntersection[location][3]), Scalar(0,0,255), 1);
	}	
	
	char buf1[100];
	for(int i = 0; i < 4; i++)
	{
		if( i != 3)
		{
			pCross[i] = IntersectionPoint(&cv::Point2d(vIntersection[i][0], vIntersection[i][1]), &cv::Point2d(vIntersection[i][2], vIntersection[i][3])
				, &cv::Point2d(vIntersection[i+1][0], vIntersection[i+1][1]), &cv::Point2d(vIntersection[i+1][2], vIntersection[i+1][3]));
		}
		else
		{
			pCross[i] = IntersectionPoint(&cv::Point2d(vIntersection[i][0], vIntersection[i][1]), &cv::Point2d(vIntersection[i][2], vIntersection[i][3])
				, &cv::Point2d(vIntersection[0][0], vIntersection[0][1]), &cv::Point2d(vIntersection[0][2], vIntersection[0][3]));			

			cv::line(cont, pCross[i], pCross[0], Scalar(0,0,255), 1);
			angle[i] = GetAngle(pCross[i], pCross[0]);

			sprintf_s(buf1, "angle : %.3f", angle[i]);
			putText(cont, buf1, cv::Point(pCross[i].x+10, pCross[i].y+10), CV_FONT_HERSHEY_SIMPLEX, 0.8, Scalar(150, 150, 150), 2, 1);
		}

		circle(cont, pCross[i],2,Scalar(255,255,0), 3);

		if(i > 0)
		{
			cv::line(cont, pCross[i-1], pCross[i], Scalar(0,0,255), 1);	
			angle[i-1] = GetAngle(pCross[i-1], pCross[i]);

			sprintf_s(buf1, "angle : %.3f", angle[i-1]);
			putText(cont, buf1, cv::Point(pCross[i-1].x+10, pCross[i-1].y+10), CV_FONT_HERSHEY_SIMPLEX, 0.8, Scalar(150, 150, 150), 2, 1);
		}

		sprintf_s(buf1, "%d", i);
		putText(cont, buf1, cv::Point(pCross[i].x-10, pCross[i].y-10), CV_FONT_HERSHEY_SIMPLEX, 0.8, Scalar(150, 150, 150), 2, 1);
	}

	// 두직선의 교점 구하기
	// 좌상단, 우하단의 좌표와 우상단, 좌하단의 좌표를 가진 직선을 활용해 교차점을 구하자
	cv::Point center = cv::Point(-1,-1);
	
	center = IntersectionPoint(&pCross[0], &pCross[2],&pCross[1], &pCross[3]);

	cv::line(cont, pCross[0], pCross[2], Scalar(0,255,255), 1);
	cv::line(cont, pCross[1], pCross[3], Scalar(0,255,255), 1);

	circle(cont, center,2,Scalar(0,255,0), 3);

	if(bImgDebug)
	{
		imshow("ROI", rans_roi);
		imshow("Binary Image", binary);
		imshow("Foreground Image", fg);
		imshow("Ransac-Line", cont);
	}

	if(center.x != -1 && center.y != -1)
	{
		bestLine.bFind = TRUE;
		bestLine.angle = angle[3];
		bestLine.center.x = center.x+roi.x;
		bestLine.center.y = center.y+roi.y;

		for(int v = 0; v < 4; v++)
		{
			bestLine.vertex[v].x = pCross[v].x+roi.x;
			bestLine.vertex[v].y = pCross[v].y+roi.y;
		}		
	}

	return bestLine;
}




BOOL ImageProcess::RotateMat(Mat src, Mat& rMat, double degree)
{
	// CW : 시계방향 이미지 회전 (+90도씩 돌아감)
	if(src.empty())
	{
		AfxMessageBox("로드된 이미지가 없습니다.");
		return FALSE;
	}
	Mat t;		t=src.clone();

	int Height = t.rows / 2;//getting middle point of rows//
	int Width = t.cols / 2;//getting middle point of height//
	Mat for_Rotation = getRotationMatrix2D(cv::Point(Width, Height), (degree*-1), 1);//affine transformation matrix for 2D rotation//
	Mat for_Rotated;//declaring a matrix for rotated image
	warpAffine(t, for_Rotated, for_Rotation, t.size());//applying affine transformation//
	rMat = for_Rotated.clone();
	return TRUE;
}


cv::Point ImageProcess::GetIntersectPoint2D(cv::Point x1, cv::Point x2, cv::Point x3, cv::Point x4)
{
	// 오차가 좀 있음 IntersectionPoint 이걸로 사용하기로 함
	float fIncrease1, fConstant1, fSameValue1;
	float fIncrease2, fConstant2, fSameValue2;
	// 직선 1.
	if(x1.x == x2.x)	// y 축과 평행한 경우
		fSameValue1 = x1.x;
	else
	{
		fIncrease1 = (float)(x2.y-x1.y)/(x2.x-x1.x);		// 기울기 구하기.
		fConstant1 = x1.y - fIncrease1*x1.x;				// 상수값 구하기.
	}

	// 직선 2.
	if(x3.x == x4.x)	// y 축과 평행한 경우
		fSameValue2 = x3.x;
	else
	{
		fIncrease2 = (float)(x4.y-x3.y)/(x4.x-x3.x);		// 기울기 구하기.
		fConstant2 = x3.y - fIncrease2*x3.x;				// 상수값 구하기.
	}

	// 교점
	cv::Point pResult;
	if(x1.x == x2.x && x3.x == x4.x ) return cv::Point(-1,-1);	// 평행할 경우 교점없음
	if(x1.x == x2.x)
		pResult = cv::Point(fSameValue1, fIncrease2*fSameValue1+fConstant2);		// 직선 1이 y 축에 평행할 경우 교점.
	else if(x3.x==x4.x)
		pResult = cv::Point(fSameValue2, fIncrease1*fSameValue2+fConstant1);		// 직선 2가 y 축에 평행할 경우 교점.
	else
	{
		pResult.x = -(fConstant1-fConstant2)/(fIncrease1-fIncrease2);
		pResult.y = fIncrease1*pResult.x +fConstant1;
	}
	return pResult;
}
// (p1, p2)를 이은 직선과 (p3, p4)를 이은 직선의 교차점을 구하는 함수
// Function to get intersection point with line connecting points (p1, p2) and another line (p3, p4).
cv::Point2d ImageProcess::IntersectionPoint(const cv::Point2d* p1, const cv::Point2d* p2, const cv::Point2d* p3, const cv::Point2d* p4)
{
	cv::Point2d ret;
	if((p1->x - p2->x)*(p3->y - p4->y) != (p1->y - p2->y)*(p3->x - p4->x))
		ret.x = ((p1->x*p2->y - p1->y*p2->x)*(p3->x - p4->x) - (p1->x - p2->x)*(p3->x*p4->y - p3->y*p4->x))/( (p1->x - p2->x)*(p3->y - p4->y) - (p1->y - p2->y)*(p3->x - p4->x) );
	else
		return cv::Point2d(-1,-1);
	if((p1->x - p2->x)*(p3->y - p4->y) != (p1->y - p2->y)*(p3->x - p4->x))
		ret.y = ((p1->x*p2->y - p1->y*p2->x)*(p3->y - p4->y) - (p1->y - p2->y)*(p3->x*p4->y - p3->y*p4->x)) / ( (p1->x - p2->x)*(p3->y - p4->y) - (p1->y - p2->y)*(p3->x - p4->x) );
	else
		return cv::Point2d(-1,-1);

	return ret;
}

double	ImageProcess::GetAngle(cv::Point s_pos, cv::Point e_pos)
{
	double dy = e_pos.y-s_pos.y;
	double dx = e_pos.x-s_pos.x;
	double d, degree;
	//d = (atan2(dy, dx) * (180.0 / PI)); 각도계산이지만 각도의 기준점을 모터 기준으로 돌리기 위해 상단을 기준으로 360도 좌표계로 변환하기 위함
	d = (atan2(dy, dx) * (180.0 / PI)-270);
	// ATAN2 함수의 결과값은 라디안 값으로는 -pi ~ pi까지이며, 60진법으로는 -180도 ~ 180도 이다.
	// 여기에서 음수값을 제거하기 위하여 60진법의 경우는 음수값에 한해 360도를 더해 주면 아래 그림과 같이 0 ~ 360도로 그 값을 구할 수 있다.
	if(d < 0)
		degree = d + 360;
	else
		degree = d;
	return degree;
}


void ImageProcess::edgeLines(Mat img, Mat &line_result, vector<Vec4i> lines)
{
	// DETECTING EDGE LINES
	// SET line_row_theshold, line_high_threshold
	// GOT A LINE VALUE WITHIN THE RANGE OF SLOPE
	// 왼쪽라인, 오른쪽 라인을 찾을때 사용
	int width = img.rows;
	for(size_t i = 0; i < lines.size(); i++)
	{
		Vec4i _lines = lines[i];
		int x1 = _lines[0];
		int y1 = _lines[1];
		int x2 = _lines[2];
		int y2 = _lines[3];
		float slope =0.;
		if(x2 == x1)
		{
			slope = 999.;
		}
		else
		{
			slope = fabsf((y1 - y2) / float(x1 - x2));
		}
		float line_row_theshold = tan(15*CV_PI/180);
		float line_high_threshold = tan(89.9*CV_PI/180);
		if (line_row_theshold < slope && slope < line_high_threshold)
		{
			if(x1 < width / 2)
			{
				// left side edge
				line(line_result, cv::Point(x1, y1), cv::Point(x2, y2), Scalar(255,0,0), 1, LINE_AA);
			}
			else
			{
				// right side edge
				line(line_result, cv::Point(x1, y1), cv::Point(x2, y2), Scalar(0,255,0), 1, LINE_AA);
			}
		}
		else
			line(line_result, cv::Point(x1, y1), cv::Point(x2, y2), Scalar(0,0,255), 1, LINE_AA);
	}
}

double ImageProcess::Distance(cv::Point p1, cv::Point p2)
{
	double distance;
	// 피타고라스의 정리
	// pow(x,2) x의 2승,  sqrt() 제곱근
	distance = sqrt(pow(abs((double)p1.x - (double)p2.x), 2) + pow(abs((double)p1.y - (double)p2.y), 2));
	return distance;
}


void ImageProcess::InitParam(__RANSAC &rs)
{
	InitCircleParam(rs.ran_circ);
	InitRectangleParam(rs.ran_rect);
}

void ImageProcess::InitCircleParam(__Circle &rsc)
{
	rsc.bFind = FALSE;
	rsc.radius = 0.;
	rsc.center = cv::Point(0,0);
	rsc.tact = 0.;
}
void ImageProcess::InitRectangleParam(__Rectangle &rsr)
{
	rsr.bFind = FALSE;
	rsr.angle = 0.;
	rsr.vertex[0] = cv::Point(0,0);
	rsr.vertex[1] = cv::Point(0,0);
	rsr.vertex[2] = cv::Point(0,0);
	rsr.vertex[3] = cv::Point(0,0);
	rsr.center = cv::Point(0,0);
	rsr.tact = 0.;
}
<<<<<<< HEAD
=======
=======
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
//	namedWindow("output", WINDOW_NORMAL);
//	imshow("output",color);

	
 	//waitKey(0);

	return bestCirclesHough;
<<<<<<< HEAD
}
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
}
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
