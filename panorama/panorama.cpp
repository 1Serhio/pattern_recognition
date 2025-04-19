#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/stitching.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    string path1 = "D:/Рабочий стол/Распознавание образов/25.03/1.png";
    string path2 = "D:/Рабочий стол/Распознавание образов/25.03/2.png";

    Mat img1 = imread(path1);
    Mat img2 = imread(path2);
    if (img1.empty() || img2.empty()) {
        cout << "Не удалось загрузить изображения!" << endl;
        return -1;
    }

    // Обнаружение ключевых точек и дескрипторов
    Ptr<Feature2D> detector = SIFT::create();
    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    detector->detectAndCompute(img1, noArray(), keypoints1, descriptors1);
    detector->detectAndCompute(img2, noArray(), keypoints2, descriptors2);

    // Сопоставление дескрипторов с помощью BFMatcher и Ratio Test
    BFMatcher matcher(NORM_L2);
    vector<vector<DMatch>> knnMatches;
    matcher.knnMatch(descriptors1, descriptors2, knnMatches, 2);

    const float ratio_thresh = 0.75f;
    vector<DMatch> goodMatches;
    for (size_t i = 0; i < knnMatches.size(); i++) {
        if (knnMatches[i][0].distance < ratio_thresh * knnMatches[i][1].distance) {
            goodMatches.push_back(knnMatches[i][0]);
        }
    }

    // Извлечение точек и применение RANSAC
    vector<Point2f> points1, points2;
    for (const auto& match : goodMatches) {
        points1.push_back(keypoints1[match.queryIdx].pt);
        points2.push_back(keypoints2[match.trainIdx].pt);
    }

    vector<uchar> inliersMask;
    Mat H = findHomography(points2, points1, RANSAC, 4, inliersMask);

    // Визуализация совпавших inlier-точек
    int rows = max(img1.rows, img2.rows);
    int cols = img1.cols + img2.cols;
    Mat matchCanvas(rows, cols, img1.type(), Scalar::all(0));
    img1.copyTo(matchCanvas(Rect(0, 0, img1.cols, img1.rows)));
    img2.copyTo(matchCanvas(Rect(img1.cols, 0, img2.cols, img2.rows)));

    int shown = 0;
    int maxShown = 250; // Максимум 250 совпадений для наглядности

    for (size_t i = 0; i < goodMatches.size(); i++) {
        if (inliersMask[i]) {
            Point2f pt1 = keypoints1[goodMatches[i].queryIdx].pt;
            Point2f pt2 = keypoints2[goodMatches[i].trainIdx].pt + Point2f((float)img1.cols, 0);
            line(matchCanvas, pt1, pt2, Scalar(0, 255, 0), 1);
            if (++shown >= maxShown) break;
        }
    }

    imshow("Inlier Matches (RANSAC)", matchCanvas);
    imwrite("inlier_matches.jpg", matchCanvas);
    waitKey(0);

    // Построение панорамы
    Mat result;
    warpPerspective(img2, result, H, Size(img1.cols + img2.cols, img1.rows));
    img1.copyTo(result(Rect(0, 0, img1.cols, img1.rows)));

    imshow("Panorama Result", result);
    imwrite("panorama.jpg", result);
    waitKey(0);

    return 0;
}
