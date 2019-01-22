#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <zbar.h>
#include <iostream>
#include <iomanip>

#include"barcode_processing.h"


using namespace std;
using namespace cv;
using namespace zbar;

VideoCapture cap;

void CaptureWEB(Mat& img1, Mat &img2, Mat &img3)
{

	//Data Structure to store cam.
	//CvCapture* cap = cvCaptureFromCAM(1);   //old syntax, removed on an update of OpenCv
	
	cap.open(0);

	//Image variable to store frame
	//IplImage* frame;
	Mat frame;
	
	//Window to show livefeed
	//NamedWindow("LiveFeed", CV_WINDOW_AUTOSIZE);
	

	if (!cap.isOpened())
	{
		cout << "ERROR: Capture is null!\n";
	}


	while (1)
	{
		//Load the next frame
		//frame = cvQueryFrame(cap);
		cap >> frame;
		//If frame is not loaded break from the loop
		if (frame.empty())
			break;
		//Show the present frame
		imshow("LiveFeed", frame);
		//Escape Sequence
		char c = waitKey(33);
		//If the key pressed by user is Esc(ASCII is 27) then break out of the loop
		if (c == 27)
		{
			//IplImage *img = cvQueryFrame(cap);
			//Mat img1,img2,img3;
			cap >> img1;
			//imwrite("kappa.jpg", img);
			waitKey(1);

			//img = cvQueryFrame(cap);
			cap >> img2;
			//imwrite("kappa2.jpg", img);
			waitKey(1);
			cap >> img3;
			//img = cvQueryFrame(cap);
			//imwrite("kappa3.jpg", img);
			break;
		}
	}
	//CleanUp
	//cap.release();
	//cvDestroyAllWindows();

}

int counter_scan = 0;
char* scanned_barcode; 
int width, height;

/*
int in_borders(int x, int y)
{
	if (x < width-1 && x > 0 && y < height-1 && y > 0)
		return 1;
	return 0;
}
*/

int getBarcode(Mat img)
{

	// Create a zbar reader
	ImageScanner scanner;

	// Configure the reader
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);


	// Get the imageto scan
	Mat frame_grayscale;
	cvtColor(img, frame_grayscale, COLOR_RGB2GRAY);

	//frame_grayscale = imread("kappa.jpeg", IMREAD_GRAYSCALE);

	// Convert to grayscale
	//cvtColor(frame, frame_grayscale, CV_BGR2GRAY);

	// Obtain image data
	width = frame_grayscale.cols;
	height = frame_grayscale.rows;
	
	uchar *raw = (uchar *)(frame_grayscale.data);

	// Wrap image data
	Image image(width, height, "Y800", raw, width * height);

	// Scan the image for barcodes
	//int n = scanner.scan(image);
	scanner.scan(image);

	// Extract results
	int counter = 0, k = 0, j = 1;
/*	unsigned int x, y, wdt, hgt;
	image.get_crop(x, y, wdt, hgt);
	cout << "x: " << x << endl;
	cout << "y: " << y << endl;
	cout << "wdt: " << wdt << endl;
	cout << "hgh: " << hgt << endl;
*/


	for (Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
	{
		//imshow("symbol", symbol);
		k = 1;
		time_t now;
		now = time(0);
		tm current;
		//current = localtime_s();
		localtime_s(&current, &now);


		// do something useful with results
		cout << "[" << current.tm_hour << ":" << current.tm_min << ":" << setw(2) << setfill('0') << current.tm_sec << "] " << counter << " "
			<< "decoded " << symbol->get_type_name()
			<< " symbol \"" << symbol->get_data() << '"' << endl;

		//cout << "Location: (" << symbol->get_location_x(0) << "," << symbol->get_location_y(0) << ")" << endl;
		//cout << "Size: " << symbol->get_location_size() << endl;

		// Draw location of the symbols found
		//if (symbol->get_location_size() == 4) {
			//scoate rectangle-ul daca nu iti place ce face
			/*for (int m = 0; m<4; ++m)
			{
				rectangle(frame_grayscale, Rect(symbol->get_location_x(m), symbol->get_location_y(m), 10, 10), Scalar(0, 255, 0));
			}*/
			/*line(img, Point(symbol->get_location_x(4), symbol->get_location_y(4)), Point(symbol->get_location_x(5), symbol->get_location_y(5)), Scalar(255, 100, 0), 2, 8, 0);
			line(img, Point(symbol->get_location_x(5), symbol->get_location_y(5)), Point(symbol->get_location_x(6), symbol->get_location_y(6)), Scalar(255, 100, 0), 2, 8, 0);
			line(img, Point(symbol->get_location_x(6), symbol->get_location_y(6)), Point(symbol->get_location_x(7), symbol->get_location_y(7)), Scalar(255, 100, 0), 2, 8, 0);
			line(img, Point(symbol->get_location_x(7), symbol->get_location_y(7)), Point(symbol->get_location_x(8), symbol->get_location_y(8)), Scalar(255, 100, 0), 2, 8, 0);

			/*Mat dstImage;
			double C=10;
			adaptiveThreshold(frame_grayscale, dstImage, 240, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 9, C);
			imshow("dstImage", dstImage);
			*/

			/*cout << "symbol->get_location_size()= " << symbol->get_location_size() << endl;
			cout << "(" << symbol->get_location_x(0) << "," << symbol->get_location_y(0) << ") - (" << symbol->get_location_x(1) << "," << symbol->get_location_y(1) << ")\n";
			cout << "(" << symbol->get_location_x(1) << "," << symbol->get_location_y(1) << ") - (" << symbol->get_location_x(2) << "," << symbol->get_location_y(2) << ")\n";
			cout << "(" << symbol->get_location_x(2) << "," << symbol->get_location_y(2) << ") - (" << symbol->get_location_x(3) << "," << symbol->get_location_y(3) << ")\n";
			cout << "(" << symbol->get_location_x(3) << "," << symbol->get_location_y(3) << ") - (" << symbol->get_location_x(0) << "," << symbol->get_location_y(0) << ")\n";

			cout << zbar_symbol_get_loc_x(*symbol, 0) << endl;
			cout << zbar_symbol_get_loc_y(*symbol, 0) << endl;

			cout << zbar_symbol_get_loc_x(*symbol, 1) << endl;
			cout << zbar_symbol_get_loc_y(*symbol, 1) << endl;

			cout << zbar_symbol_get_loc_x(*symbol, 2) << endl;
			cout << zbar_symbol_get_loc_y(*symbol, 2) << endl;

			cout << zbar_symbol_get_loc_x(*symbol, 3) << endl;
			cout << zbar_symbol_get_loc_y(*symbol, 3) << endl;

			cout << zbar_symbol_get_loc_x(*symbol, 8) << endl;
			cout << zbar_symbol_get_loc_y(*symbol, 8) << endl;
			*/


		/*Mat image_threshold;
		adaptiveThreshold(frame_grayscale, image_threshold, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 13, 10);
		imshow("grayscale", image_threshold);
		*/

		int min_x = INT_MAX, min_y = INT_MAX;
		int max_x = INT_MIN, max_y = INT_MIN;

		/*for (int i = 0; i < symbol->get_location_size(); i++)
		{
			//	cout << symbol->get_location_x(i) << " " << symbol->get_location_y(i) << endl;
			cout << (int)frame_grayscale.at<uchar>(symbol->get_location_y(i), symbol->get_location_x(i)) << " ";
		}
		*/
		//line(img, Point(symbol->get_location_x(0), symbol->get_location_y(0)), Point(symbol->get_location_x(1), symbol->get_location_y(1)), Scalar(100, 100, 150), 3, 8, 0);
		
		for (int i = 0; i < symbol->get_location_size(); i++)
		{
			//std::cout<< frame_grayscale.elemSize1();
			//if(symbol->get_location_x(i) < width && symbol->get_location_y(i) < height ) or next line
			//if(symbol->get_location_x(i) != 0 && symbol->get_location_x(i) != -1 && symbol->get_location_y(i) != 0 && symbol->get_location_y(i) != -1)
			//	frame_grayscale.at<int>(Point(symbol->get_location_x(i), symbol->get_location_y(i))) = 255;
			if (symbol->get_location_x(i) != 0 && symbol->get_location_x(i) != -1 && symbol->get_location_y(i) != 0 && symbol->get_location_y(i) != -1)
			{
				if (min_x > symbol->get_location_x(i))
					min_x = symbol->get_location_x(i);

				if (min_y > symbol->get_location_y(i))
					min_y = symbol->get_location_y(i);

				if (max_x < symbol->get_location_x(i))
					max_x = symbol->get_location_x(i);

				if (max_y < symbol->get_location_y(i))
					max_y = symbol->get_location_y(i);
			}
		}

		//imwrite("livecam_barcode.jpg", image_threshold);
		//Mat path = imread("livecam_barcode.jpg", cv::IMREAD_COLOR);

		line(img, Point(min_x, min_y), Point(max_x,min_y), Scalar(0, 0, 128), 2, 8, 0);
		line(img, Point(max_x, min_y), Point(max_x,max_y), Scalar(0, 0, 128), 2, 8, 0);
		line(img, Point(max_x, max_y), Point(min_x,max_y), Scalar(0, 0, 128), 2, 8, 0);
		line(img, Point(min_x, max_y), Point(min_x, min_y), Scalar(0, 0, 128), 2, 8, 0);
		imshow("LiveFeed", img);
		waitKey(1);
		
		//0 black
		//255 white
		/*int tl_x, tl_y, tr_x, tr_y, bl_x, bl_y, br_x, br_y;
		tl_x = min_x; tl_y = min_y; //top left
		tr_x = max_x; tr_y = min_y; //top right
		bl_x = min_x; bl_y = max_y; //bottom left
		br_x = max_x; br_y = max_y; //bottom right
		int black = 0, white = 255;

		Vec3b value;
		value[0] = 0;
		value[1] = 255;
		value[2] = 255;

		cout << endl;
		cout << "tl_x: " << tl_x << " tl_y:" << tl_y << " color: " << (int)image_threshold.at<uchar>(tl_x, tl_y) << endl;
		//top left corner
		while ((int)image_threshold.at<uchar>(tl_x, tl_y) == white && in_borders(tl_x, tl_y)) //while pixel is white, move a pixel to the right until it finds a black one
		{
			tl_x++; 
			path.at<Vec3b>(tl_x, tl_y) = value;
		}
		while (((int)image_threshold.at<uchar>(tl_x - 1, tl_y - 1) == black || (int)image_threshold.at<uchar>(tl_x, tl_y - 1) == black || (int)image_threshold.at<uchar>(tl_x + 1, tl_y - 1) == black) &&
			in_borders(tl_x - 1, tl_y - 1) && in_borders(tl_x, tl_y - 1) && in_borders(tl_x + 1, tl_y - 1))
			//while the 3 pixels above it are black, move on top until it finds a white one
		{
			if ((int)image_threshold.at<uchar>(tl_x, tl_y - 1) == black)
			{
				tl_y--; 
			}
			else if ((int)image_threshold.at<uchar>(tl_x - 1, tl_y - 1) == black)
			{
				tl_x--; tl_y--; 
			}
			else if ((int)image_threshold.at<uchar>(tl_x + 1, tl_y - 1) == black)
			{
				tl_x++; tl_y--; 
			}
			path.at<Vec3b>(tl_x, tl_y) = value;
		}
		cout << "tl_x: " << tl_x << " tl_y:" << tl_y << " color: " << (int)image_threshold.at<uchar>(tl_x, tl_y) << endl;
		
		cout << "tr_x: " << tr_x << " tr_y:" << tr_y << " color: " << (int)image_threshold.at<uchar>(tr_x, tr_y) << endl;
			//top right corner
			while ((int)image_threshold.at<uchar>(tr_x, tr_y) == white && in_borders(tr_x, tr_y)) //move a pixel to the left until it finds a black one
			{
				tr_x--;
			}
			while (((int)image_threshold.at<uchar>(tr_x - 1, tr_y - 1) == black || (int)image_threshold.at<uchar>(tr_x, tr_y - 1) == black || (int)image_threshold.at<uchar>(tr_x + 1, tr_y - 1) == black) &&
				in_borders(tr_x - 1, tr_y - 1) && in_borders(tr_x, tr_y - 1) && in_borders(tr_x + 1, tr_y - 1))
				//move a pixel on one on the above 3 pixels until all of them are white
			{
				if ((int)image_threshold.at<uchar>(tr_x, tr_y - 1) == black)
				{
					tr_y--; continue;
				}
				if ((int)image_threshold.at<uchar>(tr_x + 1, tr_y - 1) == black)
				{
					tr_x++; tr_y--; continue;
				}
				if ((int)image_threshold.at<uchar>(tr_x - 1, tr_y - 1) == black)
				{
					tr_x--; tr_y--; continue;
				}
			}
			cout << "tr_x: " << tr_x << " tr_y:" << tr_y << " color: " << (int)image_threshold.at<uchar>(tr_x, tr_y) << endl;

			
			cout << "bl_x: " << bl_x << " bl_y:" << bl_y << " color: " << (int)image_threshold.at<uchar>(bl_x, bl_y) << endl;
			//bottom left corner
			while ((int)image_threshold.at<uchar>(bl_x, bl_y) == white && in_borders(bl_x,bl_y)) //move a pixel to the left until it finds a black one
			{
				bl_x++;
			}
			while (( (int)image_threshold.at<uchar>(bl_x - 1, bl_y + 1) == black || (int)image_threshold.at<uchar>(bl_x, bl_y + 1) == black || (int)image_threshold.at<uchar>(bl_x + 1, bl_y + 1) == black) &&
				in_borders(bl_x-1,bl_y+1) && in_borders(bl_x,bl_y+1) && in_borders(bl_x+1,bl_y+1))
			//move a pixel on one on the above 3 pixels until all of them are white
			{
				if ((int)image_threshold.at<uchar>(bl_x, bl_y + 1) == black)
				{
					bl_y++; continue;
				}
				if ((int)image_threshold.at<uchar>(bl_x - 1, bl_y + 1) == black)
				{
					bl_x--; bl_y++; continue;
				}

				if ((int)image_threshold.at<uchar>(bl_x + 1, bl_y + 1) == black)
				{
					bl_x++; bl_y++; continue;
				}
			}
			cout << "bl_x: " << bl_x << " bl_y:" << bl_y << " color: " << (int)image_threshold.at<uchar>(bl_x, bl_y) << endl;

			cout << "br_x: " << br_x << " br_y:" << br_y << " color: " << (int)image_threshold.at<uchar>(br_x, br_y) << endl;
			//bottom right corner
			while ((int)image_threshold.at<uchar>(br_x, br_y) == white) //move a pixel to the left until it finds a black one
			{
				br_x--;
			}
			while ((int)image_threshold.at<uchar>(br_x - 1, br_y + 1) == black || (int)image_threshold.at<uchar>(br_x, br_y + 1) == black || (int)image_threshold.at<uchar>(br_x + 1, br_y + 1) == black)
			{
				if ((int)image_threshold.at<uchar>(br_x, br_y + 1) == black)
				{
					br_y++; continue;
				}
				if ((int)image_threshold.at<uchar>(br_x - 1, br_y + 1) == black)
				{
					br_x--; br_y++; continue;
				}

				if ((int)image_threshold.at<uchar>(br_x + 1, br_y + 1) == black)
				{
					br_x++; br_y++; continue;
				}
			}
			cout << "br_x: " << br_x << " br_y:" << br_y << "color: " << (int)image_threshold.at<uchar>(br_x, br_y) << endl;

			line(frame_grayscale, Point(tl_x, tl_y), Point(tr_x, tr_y), Scalar(255, 0, 0), 2, 8, 0);
			line(frame_grayscale, Point(tr_x, tr_y), Point(br_x, br_y), Scalar(255, 0, 0), 2, 8, 0);
			line(frame_grayscale, Point(br_x, br_y), Point(bl_x, bl_y), Scalar(255, 0, 0), 2, 8, 0);
			line(frame_grayscale, Point(bl_x, bl_y), Point(tl_x, tl_y), Scalar(255, 0, 0), 2, 8, 0);
			//}

			imshow("path image", path);
			// Get points
			/*for (Symbol::PointIterator point = symbol->point_begin(); point != symbol->point_end(); ++point) {
				//cout << point << endl;
				++point;
			}
			*/
		   counter++;

			// Show captured frame, now with overlays!
			//string title = "Frame " + to_string(counter_scan);
			//imshow(title, frame_grayscale);
			//imwrite("barcode_image.jpg", frame_grayscale);

			// clean up
			image.set_data(NULL, 0);

			//return stoll( symbol->get_data(),nullptr,10 ); //return a string converted to long long
			if (k && j)
			{
				scanned_barcode = new char[strlen(const_cast<char*>(symbol->get_data().c_str())) + 1];
				strcpy_s(scanned_barcode, strlen(const_cast<char*>(symbol->get_data().c_str())) + 1, const_cast<char*>(symbol->get_data().c_str()));
				j = 0;
			}
		}
	if (k)
	{
		cout << "Scanned barcode: " << scanned_barcode << endl;
		return 1;
	}

	// clean up
	image.set_data(NULL, 0);
	return -1;

}

int get_success(Mat img)
{
	int success = getBarcode(img);
	return success;
}

int main()
{

	/*int k = 0;
	do
	{
		cout << "Apasa ESC cand esti gata" << endl;
		CaptureWEB();
		k = getBarcode();
	} while (k == 0);
	*/

	Mat img1, img2, img3;

	CaptureWEB(img1,img2,img3); //captures first 3 frames

	auto start = std::chrono::steady_clock::now();
	auto now = start;

	int width=img1.cols, height=img1.rows;
	cout << "width: " << width << "\nheight: " << height << endl;

	int success = -1;
	do
	{
		counter_scan++;
		if ((success = get_success(img1)) == -1)
		{
			counter_scan++;
			if ((success = get_success(img2)) == -1)
			{
				counter_scan++;
				if ((success = get_success(img3)) == -1)
				{
					cap >> img1; waitKey(1);
					imshow("LiveFeed", img1);
					cap >> img2; waitKey(1);
					imshow("LiveFeed", img2);
					cap >> img3; waitKey(1);
					imshow("LiveFeed", img3);
				}
			}
		}
		now = std::chrono::steady_clock::now();
	} while (now < start + std::chrono::seconds{ 10 } && success == -1 );


	cap.release();

	if (success != -1)
	{
		cout << "Success! " << endl;
		if (process_barcode(scanned_barcode) != 1)
		{
			system("PAUSE");
			exit(1);
		}

		search_barcode_in_file_new(scanned_barcode);
	}
	else 
	{
		cout << "Barcode not found, time limit exceeded" << endl;
	}

	waitKey(0);
	//system("PAUSE");

	return 0;

}

