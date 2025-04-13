#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Функция для изменения размера изображения
Mat resizeToFit(const Mat& src, Size targetSize) {
    Mat resized;
    resize(src, resized, targetSize);
    return resized;
}

// Функция для создания коллажа изображений 2x3
Mat createGridDisplay(const vector<Mat>& images, Size singleSize) {
    int rows = 2, cols = 3;
    Mat grid(singleSize.height * rows, singleSize.width * cols, images[0].type());

    for (int i = 0; i < images.size(); ++i) {
        Mat resized;
        resize(images[i], resized, singleSize);
        int x = (i % cols) * singleSize.width;
        int y = (i / cols) * singleSize.height;
        resized.copyTo(grid(Rect(x, y, singleSize.width, singleSize.height)));
    }
    return grid;
}

int main() {
    string imagePath = ".../lena.png";
    Mat original = imread(imagePath);

    if (original.empty()) {
        cout << "Не удалось загрузить изображение!" << endl;
        return -1;
    }

    // === ЭТАП 1 ===
    Mat hsv, lab, yuv, xyz, gray;
    cvtColor(original, hsv, COLOR_BGR2HSV);
    cvtColor(original, lab, COLOR_BGR2Lab);
    cvtColor(original, yuv, COLOR_BGR2YUV);
    cvtColor(original, xyz, COLOR_BGR2XYZ);
    cvtColor(original, gray, COLOR_BGR2GRAY);

    // Переводим одноканальные изображения в 3 канала для отображения
    Mat gray3ch;
    cvtColor(gray, gray3ch, COLOR_GRAY2BGR);

    vector<Mat> allImages = { original, hsv, lab, yuv, xyz, gray3ch };
    Mat combined = createGridDisplay(allImages, Size(320, 240)); // 3 колонки по 320 = 960 ширины

    imshow("Различные цветовые пространства", combined);
    waitKey(0);
    destroyAllWindows();

    return 0;
}