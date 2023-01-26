#include "stdafx.h"
#include "ImageProcess.h"

bool comparePerc(Vec4f vec1, Vec4f vec2)
{
	double i = double(vec1[0]);
	double j = double(vec2[0]);
	return (i > j);
}

bool compareContourAreas(vector<cv::Point> contour1, vector<cv::Point> contour2)
{
	double i = fabs(contourArea(Mat(contour1)));
	double j = fabs(contourArea(Mat(contour2)));
	return (i > j);
}

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
	// Image�� Ŭ�� �ĸ��̵� �˰������� �����
	Mat _mat;
	_mat = mat.clone();

//	if(mat.rows >= 1000 || mat.cols >= 1000)
//	{
		// count �� �̹��� �Ƕ�̵� �ٿ��� ������� ���� -> ������ ��� ����
	for(int r = 0; r < rs; r++)
		pyrDown(_mat, _mat, Size( _mat.cols/2, _mat.rows/2 ));		
//	}

	mat.release();
	mat = _mat.clone();

	return TRUE;
}
BOOL ImageProcess::PrymidDenoise(Mat& mat, int dn)
{
	// ������ ���Ÿ� ���� �̹����� Scale�� ��� Ȯ�� ����
	// ������ ī��Ʈ��ŭ �����Ѵ�
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
	// normalize Image�� gray �̹����� �����ϰ� �س���
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
	// Image �������� �����̹��� �ջ��Ű�� ����
	Mat circle_image = src.clone();

	// �̹����� �������� ����	
	if(circle_image.empty())
		return FALSE;

	Mat canny_image;

	// �÷� �̹����� ��� ��� ��ȯ, ����̹����� ��� �״�� ����
	if(circle_image.channels() == 1)
		canny_image = circle_image.clone();
	else
		cvtColor(circle_image, canny_image, CV_BGR2GRAY);

	// ����þ� ��ó��
	GaussianBlur(canny_image, canny_image, Size(gK, gK), 0);
	// �־��� �Ӱ谪���� �̹��� ����ȭ ����
	threshold(canny_image, canny_image, nTH, 255, THRESH_BINARY);

	// ĳ�Ͽ����� Ȱ���� ���� ���� ����
	Canny(canny_image, canny_image, 125, 125*3,3);
	rMat = canny_image.clone();

	return TRUE;
}

__Circle ImageProcess::RansacCircleFit(Mat src, Rect roi, BOOL bBin, int gK, int nTH, BOOL bImgDebug, float minRadius, float InOutRatio)
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

	// ROI ����
	gray_resize = gray(Rect(roi.x, roi.y, roi.width, roi.height)).clone();  // ���� ����

	Mat mask;
	if(!bBin)
	{
		// image normalize
		NormalizeImage(gray_resize, normalized);		
	}
	else
		normalized = gray_resize.clone();

	if(bImgDebug)
		imshow(NORMALIZE_IMAGE, normalized);

	// ����þ� ��ó��
	GaussianBlur(normalized, normalized, Size(gK, gK), 0);
	// �־��� �Ӱ谪���� �̹��� ����ȭ ����
	threshold(normalized, normalized, nTH, 255, THRESH_BINARY);

	// ĳ�Ͽ����� Ȱ���� ���� ���� ����
	Canny(normalized, mask, nTH, 125*3,3);

	if(bImgDebug)
		imshow(BINARY_IMAGE, mask);

	vector<Point2f> edgePositions;
	edgePositions = getPointPositions(mask);

	// create distance transform to efficiently evaluate distance to nearest edge
	Mat dt;
	distanceTransform(255-mask, dt,CV_DIST_L1, 3);

	// Float �� �̹��� �����ֱ�
	//floatImageShow(dt);

	//TODO: maybe seed random variable for real random numbers.
	unsigned int nIterations = 0;

	cv::Point2f bestCircleCenter;
	float bestCircleRadius;
	float bestCirclePercentage = 0;
	//float minRadius = 50;   // TODO: ADJUST THIS PARAMETER TO YOUR NEEDS, otherwise smaller circles wont be detected or "small noise circles" will have a high percentage of completion

	float minCirclePercentage = 0.03f;  // at least 5% of a circle must be present? maybe more...

	int maxNrOfIterations = edgePositions.size();   // TODO: adjust this parameter or include some real ransac criteria with inlier/outlier percentages to decide when to stop
	
	__Circle bestCircles;
	InitCircleParam(bestCircles);

	vector<Vec4f> fvResult;
	Vec4f fvTemp;
	// fvResult[0]; Percent
	// fvResult[1]; radius
	// fvResult[2]; xCenter
	// fvResult[3]; yCenter

	double dbPercent = 0.99;		// Ȯ�������� �󸶳� ��Ȯ�� ����� ����ϴ°�?
	//double InOutRatio = 0.7;		// Inlier �� outlier �� ������ ���ΰ�? �������� ���������Ͱ� ���� ��
	int nSampling;					// ����Ȯ�� ���ġ
	// ex dbPercent = 0.99, InOutRatio = 0.7 �̶�� n sampling�� 48�� ������ 0.99 �ۼ�Ʈ�� ������ ����� ����
	// 48���̳� �������� �ð��� �ʹ����� �ɸ�
	// RANSAC�� ���� : Ȯ������ �����ͱ� ������ ��Ȯ���� �ø����� �ð��� �ʹ� �ɸ��� �ð��� ���̷��� ����Ȯ�� ����� ���� �� ����
	// ���� �ذ� ��� : ��ó�� �������� ����ȭ contour�� ���� �����͸� ����ȭ�ϸ� ���� ������?
	// ���� �ذ� ��� : ������ ��Ȯ���� ���ϴ°� �ƴ϶�� ��������� Ÿ���� �ʿ���
	// ���� �ذ� ��� : �ð��� �˳��ϴٸ� 0.99, 0.7�� �����Ͽ� ��Ȯ���� �÷��� ��

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

				fvTemp[0] = cPerc;
				fvTemp[1] = bestCircleRadius;
				fvTemp[2] = bestCircleCenter.x+roi.x;
				fvTemp[3] = bestCircleCenter.y+roi.y;
				fvResult.push_back(fvTemp);
			}
	}

	// draw if good circle was found
	if(bestCirclePercentage >= minCirclePercentage)
	{
		if(bestCircleRadius >= minRadius)
		{
			circle(color, cv::Point(bestCircleCenter.x + roi.x, bestCircleCenter.y + roi.y), bestCircleRadius, Scalar(255,255,0),1);
			circle(color, cv::Point(bestCircleCenter.x + roi.x, bestCircleCenter.y + roi.y), 2, Scalar(255,0,0),2);

			bestCircles.radius = bestCircleRadius;
			bestCircles.center.x = bestCircleCenter.x+roi.x;
			bestCircles.center.y = bestCircleCenter.y+roi.y;
			bestCircles.bFind = TRUE;
		}
	}

	// ��� Ȯ�ο� �̹��� ó��
/*
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
		
		circle(color, cv::Point(fvResult[i][2], fvResult[i][3]), fvResult[i][1], Scalar(r,g,b),2);
		circle(color, cv::Point(fvResult[i][2], fvResult[i][3]), 2, Scalar(r,g,b),2);

		char buf1[100];
		sprintf_s(buf1, "Percent : %f", fvResult[i][0]);

		putText(color, buf1, cv::Point(20, (i+1)*30 ), CV_FONT_HERSHEY_SIMPLEX, 0.8, Scalar(r,g,b), 2, 1);
	}

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
	// �����Ϳ��� �ߺ����� �ʰ� N���� ������ ������ ä���Ѵ�.
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
	// PCA ������� ���� ���� �Ķ���͸� �����Ѵ�.

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

	//������ ������: sin(theta)*(x - sx) = cos(theta)*(y - sy);
	return 1;
}

double compute_distance(sLine &line, sPoint &x)
{
	// �� ��(x)�κ��� ����(line)�� ���� ������ ����(distance)�� ����Ѵ�.

	return fabs((x.x - line.sx)*line.my - (x.y - line.sy)*line.mx)/sqrt(line.mx*line.mx + line.my*line.my);
}

double model_verification (sPoint *inliers, int *no_inliers, sLine &estimated_model, sPoint *data, int no_data, double distance_threshold)
{
	*no_inliers = 0;

	double cost = 0.;

	for(int i=0; i<no_data; i++){
		// ������ ���� ������ ���̸� ����Ѵ�.
		double distance = compute_distance(estimated_model, data[i]);

		// ������ �𵨿��� ��ȿ�� �������� ���, ��ȿ�� ������ ���տ� ���Ѵ�.
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

	// ������ ���� ��ü ����	
	erode(binary, fg, cv::Mat(), cv::Point(-1, -1), 1);

	dilate(fg, fg, cv::Mat(), cv::Point(-1, -1), 1);
	erode(fg, fg, cv::Mat(), cv::Point(-1, -1), 1);
	dilate(fg, fg, cv::Mat(), cv::Point(-1, -1), 1);
	erode(fg, fg, cv::Mat(), cv::Point(-1, -1), 1);
	dilate(fg, fg, cv::Mat(), cv::Point(-1, -1), 1);

	Mat ct;
	ct = fg.clone();																		// ����͸� �Ҷ� ����� �̹����� ������ ������ �� ����

	vector<vector<cv::Point>> contours;														// ����ȭ�� ������ �ܰ��� ����
	findContours(ct, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	
	// �������� ũ�� ���ؼ� ū ������� Sorting
	sort(contours.begin(), contours.end(), compareContourAreas);						// �������� ����� ū������� Sorting
	
	Rect boundRect;
	boundRect = boundingRect(Mat(contours[0]));

	// �����¿� gap�� �ΰ� ��ǥ���� �̾ƺ���
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
	// �ʺ�� ���̸� �������� �̻����� ��ǥ�� �̾� ���� ����
	// ��Ʈ�� �������� �����¿� �̻��� ���� ����

	for (int i = 0; i < (int)v_cont.size(); i++)
	{
		Rect rbRect;
		int subWidth = 0;
		int subHeight = 0;

		rbRect = boundingRect(v_cont[i]);
		subWidth = rbRect.width*InOffset/100;						// �¿� ���� ���� 10���ξ��� ���� 
		subHeight = rbRect.height*InOffset/100;					// ���� ���� ���� 10���ξ��� ����

		for (int c = 0; c < (int)v_cont[i].size(); c++)
		{
			if(i == LEFT || i == RIGHT)
			{
				// ��,�������̴ϱ� ���� ������ 10���ξ��� ������
				if( (rbRect.y+subHeight < v_cont[i][c].y) && (rbRect.y+rbRect.height-subHeight > v_cont[i][c].y))
				{
					ptemp[0].x = v_cont[i][c].x;
					ptemp[0].y = v_cont[i][c].y;

					v_cont_loc[i].push_back(ptemp[0]);
				}
			}
			else 
			{
				// ��,�������̴ϱ� ���� �ʺ��� 10���ξ��� ������
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
	cv::Vec4i vIntersection[4]; // ����ǥ�� �Ѻ����� �������.. 
	cv::Point2d pCross[4];
	double angle[4];

	for (int location = 0; location < 4; location++)
	{
		// ����� Ȯ���ϱ� ���� Į�� �̹����� ������ ����
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

			// ���� �����Ϳ��� ���Ƿ� N���� ���� �����͸� ����.
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

			// �� �����͸� �������� �����ͷ� ���� �� �Ķ���͸� �����Ѵ�.
			compute_model_parameter (samples, no_samples, estimated_model);

			// 2. Verification

			// ���� �����Ͱ� ������ �𵨿� �� �´��� �˻��Ѵ�.
			double cost = model_verification (inliers, &no_inliers, estimated_model, data, no_data, distance_threshold);
			dist = Distance(cv::Point(samples[0].x, samples[0].y), cv::Point(samples[1].x, samples[1].y));

			// ���� ������ ���� �� �´´ٸ�, �� �𵨿� ���� ��ȿ�� �����ͷ� ���ο� ���� ���Ѵ�.
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

	// �������� ���� ���ϱ�
	// �»��, ���ϴ��� ��ǥ�� ����, ���ϴ��� ��ǥ�� ���� ������ Ȱ���� �������� ������
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
	// CW : �ð���� �̹��� ȸ�� (+90���� ���ư�)
	if(src.empty())
	{
		AfxMessageBox("�ε�� �̹����� �����ϴ�.");
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
	// ������ �� ���� IntersectionPoint �̰ɷ� ����ϱ�� ��
	float fIncrease1, fConstant1, fSameValue1;
	float fIncrease2, fConstant2, fSameValue2;
	// ���� 1.
	if(x1.x == x2.x)	// y ��� ������ ���
		fSameValue1 = x1.x;
	else
	{
		fIncrease1 = (float)(x2.y-x1.y)/(x2.x-x1.x);		// ���� ���ϱ�.
		fConstant1 = x1.y - fIncrease1*x1.x;				// ����� ���ϱ�.
	}

	// ���� 2.
	if(x3.x == x4.x)	// y ��� ������ ���
		fSameValue2 = x3.x;
	else
	{
		fIncrease2 = (float)(x4.y-x3.y)/(x4.x-x3.x);		// ���� ���ϱ�.
		fConstant2 = x3.y - fIncrease2*x3.x;				// ����� ���ϱ�.
	}

	// ����
	cv::Point pResult;
	if(x1.x == x2.x && x3.x == x4.x ) return cv::Point(-1,-1);	// ������ ��� ��������
	if(x1.x == x2.x)
		pResult = cv::Point(fSameValue1, fIncrease2*fSameValue1+fConstant2);		// ���� 1�� y �࿡ ������ ��� ����.
	else if(x3.x==x4.x)
		pResult = cv::Point(fSameValue2, fIncrease1*fSameValue2+fConstant1);		// ���� 2�� y �࿡ ������ ��� ����.
	else
	{
		pResult.x = -(fConstant1-fConstant2)/(fIncrease1-fIncrease2);
		pResult.y = fIncrease1*pResult.x +fConstant1;
	}
	return pResult;
}
// (p1, p2)�� ���� ������ (p3, p4)�� ���� ������ �������� ���ϴ� �Լ�
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
	//d = (atan2(dy, dx) * (180.0 / PI)); ������������� ������ �������� ���� �������� ������ ���� ����� �������� 360�� ��ǥ��� ��ȯ�ϱ� ����
	d = (atan2(dy, dx) * (180.0 / PI)-270);
	// ATAN2 �Լ��� ������� ���� �����δ� -pi ~ pi�����̸�, 60�������δ� -180�� ~ 180�� �̴�.
	// ���⿡�� �������� �����ϱ� ���Ͽ� 60������ ���� �������� ���� 360���� ���� �ָ� �Ʒ� �׸��� ���� 0 ~ 360���� �� ���� ���� �� �ִ�.
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
	// ���ʶ���, ������ ������ ã���� ���
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
	// ��Ÿ����� ����
	// pow(x,2) x�� 2��,  sqrt() ������
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
