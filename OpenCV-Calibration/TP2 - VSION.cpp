// TP2 - VSION.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
// OpenCV core module (matrices, etc.)
#include<opencv2\core\core.hpp>// OpenCV highgui: user interface, windows, etc.
#include<opencv2\highgui\highgui.hpp>// OpenCV image processing (converting to grayscale, etc.)
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\calib3d\calib3d.hpp>


using namespace std;
using namespace cv;

#define ESC_KEY 27
#define S_KEY 's'

char key;

int main(int argc, char** argv)
{
    /* Calibration de la caméra à partir d'une vidéo */
    // Key to store the keyboard input
    char key;
  
    // Name of the window to display the images
    string windowName = "OpenCV Calibration";
    
     Mat image;
     Mat gray_image;
     Mat imageUndistorted;

     Size board_sz = Size(9, 6);

     // Vector of corners
     vector<Point2f> corners;
     // Chess detection found
     bool found = false;

     // Object points
     vector<Point3f> vobject_points;
     vector<vector<Point3f>> object_points;

     // Image points
     vector<vector<Point2f>> image_points;

     // Camera Matrix
     Mat intrinsic = Mat(3, 3, CV_32FC1);

     // distCoeffs Matrix
     Mat distCoeffs;

     // Translation and Rotation Matrices
     vector<Mat> rotation;
     vector<Mat> translation;

     for (int i = 0; i < 6; i++)
     {
         for (int j = 0; j < 9; j++)
         {
             vobject_points.push_back(Point3f(i, j, 0));
         }
     }

     string nom = "./Pictures/Pic";
     string Img;

     // Save a capture
     bool save = false;

     int c = 0;
     int nbrPics;

     // The number of images used from the camera
     cout << "How many photos do you want to take?" << endl;
     cin >> nbrPics;

     // open the default camera
     VideoCapture cap(0); 

     // Creating a window to display the images
     namedWindow(windowName, WINDOW_AUTOSIZE);

     // Read the images to the last image
     while (cap.isOpened())
     {
         // get a new image from camera
         cap >> image; 
         // Finding chessboard corners
         found = findChessboardCorners(image, board_sz, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FILTER_QUADS);

            if (found)
             {
                 // Refinining  pixels
                 cvtColor(image, gray_image, COLOR_BGR2GRAY);
                 cornerSubPix(gray_image, corners, Size(11, 11), Size(-1, -1), TermCriteria(TermCriteria::EPS | TermCriteria::MAX_ITER, 30, 0.1));

                 // Displaying the detected corners on the chessboard
                 drawChessboardCorners(image, board_sz, corners, found);

                 // Adding points
                 if (save)
                 {
                     image_points.push_back(corners);
                     object_points.push_back(vobject_points);
                     if (c < 10)
                     {
                         Img = nom + "0" + to_string(c) + ".JPG";
                     }
                     else
                     {
                         Img = nom + to_string(c) + ".JPG";
                     }
                     save = false;
                 }
                 if (c >= nbrPics)
                     break;
             }
         

         imshow(windowName, image);

         key = waitKey(50);
         if (key == ESC_KEY)
         {
             break;
         }
         else if (key == S_KEY)
         {
             save = true;
             c++;
             cout << "Picture " << c << " saved" << endl;
         }
     }

     // Set up for a new camera calibration
     c = 0;

     // Destroying our window
     destroyWindow(windowName);

     // Performing camera calibration 
     calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rotation, translation);
     cout << "Camera matrix : " << endl << intrinsic << endl;
     cout << "Distorsion coefficients : " << endl << distCoeffs << endl;
    // undistort image
     undistort(image, imageUndistorted, intrinsic, distCoeffs);
     imshow("Undistorted image", imageUndistorted);
     waitKey(0);

     // exiting the program
     return EXIT_SUCCESS;

    /*Calibratipon d'une caméra pour les photos*/
    /* key = waitKey(10);
    Size board_sz = Size(9, 6);
    vector<Point2f> corners;
    string windowName = "OpenCV Calibration";
    namedWindow(windowName, WINDOW_AUTOSIZE);
    vector<vector<Point3f> > objpoints;
    vector<vector<Point2f>> image_points;
    vector<Point3f> objp;
    for (int i{ 0 }; i < 6; i++)
    {
        for (int j{ 0 }; j < 9; j++)
            objp.push_back(Point3f(j, i, 0));
    }
    Mat image;
    Mat gray;

    // Show Image inside a window with
    // the name provided

    for (int i = 1; i < 28; i++) {
        string nom = "calib_gopro/GOPR84";
        if (i < 10)
            nom += "0" + to_string(i) + ".JPG";
        else
            nom += to_string(i) + ".JPG";
        image = imread(nom);
        cvtColor(image, gray, COLOR_BGR2GRAY);
        bool found = findChessboardCorners(image, board_sz, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FILTER_QUADS);
        // Error Handling
        if (image.empty()) {
            cout << "Image File Not Found" << endl;

            // wait for any key press
            cin.get();
            return -1;
        }
        if (found) {
            TermCriteria criteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 30, 0.001);
            cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1), criteria);
            // Displaying the detected corner points on the checker board
            drawChessboardCorners(image, board_sz, corners, found);
            objpoints.push_back(objp);
            image_points.push_back(corners);
        }
        imshow(windowName, image);
        waitKey(0);

    }
    // Destroying the windows
    destroyWindow(windowName);

    Mat intrinsic= Mat(3, 3, CV_32FC1);
    Mat distCoeffs;
    vector<Mat> rvecs;
    vector<Mat> tvecs;
    calibrateCamera(objpoints, image_points, gray.size(), intrinsic, distCoeffs, rvecs, tvecs);
    cout << "cameraMatrix : " << intrinsic << endl;
    cout << "distCoeffs : " << distCoeffs << endl;

    return 0;
    */


    /* Détection d'un échiquier à partir d'une vidéo */
    /*
    Size board_sz = Size(9, 6);
    vector<Point2f> corners;
    Mat image;
    // The image converted into grayscale (see if we use it)
    Mat gray_image;
    VideoCapture cap(0); // open the default camera
    if (!cap.isOpened())  // check if we succeeded
        return -1;
    // Creating a window to display the images
    string windowName = "OpenCV Calibration";
    namedWindow(windowName, WINDOW_AUTOSIZE);
    bool gray = true;
    key = waitKey(30);
    while (key != ESC_KEY)
    {
        cap >> image; // get a new image from camera
        cvtColor(image, gray_image, COLOR_BGR2GRAY);
        //GaussianBlur(gray_image, gray_image, Size(7, 7), 1.5, 1.5);
        //Canny(gray_image, gray_image, 0, 30, 3);
        
        key = waitKey(30);
        bool found = findChessboardCorners(image, board_sz, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FILTER_QUADS);
        if (found) {
            TermCriteria criteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 30, 0.001);
            cornerSubPix(gray_image, corners, Size(11, 11), Size(-1, -1), criteria);
            // Displaying the detected corner points on the checker board
            drawChessboardCorners(image, board_sz, corners, found);
        }
        imshow(windowName, image);
    }
    // Destroying the windows
    destroyWindow(windowName);

    // Releasing the video capture
    cap.release();
    // exiting the program
    return EXIT_SUCCESS;
    */


    /* Détection d'un échiquier à partir des images  */
    /*
        key = waitKey(10);
        Size board_sz = Size(9, 6);
        vector<Point2f> corners;
        string windowName = "OpenCV Calibration";
        namedWindow(windowName, WINDOW_AUTOSIZE);
        Mat image;
        Mat gray;

        // Show Image inside a window with
        // the name provided

        for (int i = 1; i < 28; i++) {
            string nom = "calib_gopro/GOPR84";
            if (i < 10)
                nom += "0" + to_string(i) + ".JPG";
            else
                nom += to_string(i) + ".JPG";
            image = imread(nom);
            cvtColor(image, gray, COLOR_BGR2GRAY);
            bool found = findChessboardCorners(image, board_sz, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FILTER_QUADS);
            // Error Handling
            if (image.empty()) {
                cout << "Image File Not Found" << endl;

                // wait for any key press
                cin.get();
                return -1;
            }
            if (found) {
                TermCriteria criteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 30, 0.001);
                cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1), criteria);
                // Displaying the detected corner points on the checker board
                drawChessboardCorners(image, board_sz, corners, found);
            }
            imshow(windowName, image);
            waitKey(0);

        }
    
    return 0;
   */


   /* Affichage de fichiers (photos) */
    /*
    string windowName = "OpenCV Calibration";
    namedWindow(windowName, WINDOW_AUTOSIZE);
    Mat image;
 
    // Show Image inside a window with
    // the name provided
    
    for (int i = 1; i <28; i++) {
        string nom = "calib_gopro/GOPR84";
        if (i < 10)
            nom += "0" + to_string(i) + ".JPG";
        else
            nom +=to_string(i) + ".JPG";
        image = imread(nom);
        // Error Handling
        if (image.empty()) {
            cout << "Image File Not Found" << endl;

            // wait for any key press
            cin.get();
            return -1;
        }
        imshow(windowName, image);
        waitKey(0);
    }
    return 0;
    */


    /* Affichage de flux vidèo */
    /*
    // The current image// retrieved from the video capture// or read from a file
    Mat image;
    // The image converted into grayscale (see if we use it)
    Mat gray_image;
    VideoCapture cap(0); // open the default camera
    if (!cap.isOpened())  // check if we succeeded
        return -1;

    int num;
    while (!(cap.isOpened())) {
        cout << "Entrez un numéro" << endl;
        cin >> num;
        if (num == -1) {
            exit(0);
            break;
        }
    }

    // Creating a window to display the images
    string windowName = "OpenCV Calibration";
    namedWindow(windowName, WINDOW_AUTOSIZE);
    bool gray = true;
    while(key!=ESC_KEY)
    {
        cap >> image; // get a new image from camera
        if (gray == true) {
            cvtColor(image, gray_image, COLOR_BGR2GRAY);
            //GaussianBlur(gray_image, gray_image, Size(7, 7), 1.5, 1.5);
            //Canny(gray_image, gray_image, 0, 30, 3);
            imshow(windowName, gray_image);
        }
        else {
            imshow(windowName, image);
        }

        key = waitKey(30);
        if (key== 'g'){
            gray = !gray;
        }
    }
    
    // Showing the image in the window
    imshow(windowName,image);

    // Getting the new image from the camera
    cap.read(image);

    // Destroying the windows
    destroyWindow(windowName);
    
    // Releasing the video capture
    cap.release();


	// exiting the program
	return EXIT_SUCCESS;
    */
}