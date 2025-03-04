#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//функция для определения типа фигуры (stack overflow)
string getShapeName(vector<Point> contour) {
    vector<Point> approx;
    approxPolyDP(contour, approx, 0.02 * arcLength(contour, true), true);
    int edges = (int)approx.size();

    if (edges == 3) return "Triangle";
    else if (edges == 4) {
        Rect rect = boundingRect(approx);
        float aspectRatio = (float)rect.width / rect.height;
        return (aspectRatio >= 0.9 && aspectRatio <= 1.1) ? "Square" : "Rectangle";
    }
    else {
        return "Circle";
    }
}

int main() {
    //загрузка изображения
    Mat img = imread("../figure.png");
    if (img.empty()) {
        cout << "Ошибка загрузки изображения!" << endl;
        return -1;
    }

    //увеличиваем изображение, чтобы корректно воспринимался верхний круг у границы
    copyMakeBorder(img, img, 10, 10, 10, 10, BORDER_CONSTANT, Scalar(0, 0, 0));

    //перевод в HSV для корректной работы с синем цветом
    Mat hsv, mask;
    cvtColor(img, hsv, COLOR_BGR2HSV);

    //создаем маски для красного, синего и желтого цветов
    Mat maskRed1, maskRed2, maskBlue, maskYellow;

    inRange(hsv, Scalar(0, 100, 100), Scalar(10, 255, 255), maskRed1);  //красный v1
    inRange(hsv, Scalar(170, 100, 100), Scalar(180, 255, 255), maskRed2); //красный v2
    inRange(hsv, Scalar(100, 150, 100), Scalar(140, 255, 255), maskBlue); //синий
    inRange(hsv, Scalar(20, 100, 100), Scalar(30, 255, 255), maskYellow); //желтый

    //объединяем маски
    mask = maskRed1 | maskRed2 | maskBlue | maskYellow;

    //находим контуры
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    //заполняем внутренности фигур черным (оставляем только контуры)
    for (const auto& contour : contours) {
        if (contourArea(contour) > 500) { //фильтруем шум
            drawContours(img, vector<vector<Point>>{contour}, -1, Scalar(0, 0, 0), FILLED);
            drawContours(img, vector<vector<Point>>{contour}, -1, Scalar(0, 255, 0), 2);
        }
    }

    //подписываем фигуры (белым цветом, поверх контуров)
    for (const auto& contour : contours) {
        if (contourArea(contour) > 500) {
            string shapeName = getShapeName(contour);
            Moments M = moments(contour);
            if (M.m00 != 0) {
                int cx = (int)(M.m10 / M.m00);
                int cy = (int)(M.m01 / M.m00);
                putText(img, shapeName, Point(cx - 30, cy), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 255, 255), 2);
            }
        }
    }

    //обрезаем добавленный черный контур
    Rect roi(10, 10, img.cols - 20, img.rows - 20);
    img = img(roi);

    //отображение результата
    imshow("Контуры фигур", img);
    waitKey(0);
    return 0;
}
