#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // Загрузка изображения
    string imagePath = ".../lena.png";
    Mat original = imread(imagePath);

    if (original.empty()) {
        cout << "Не удалось загрузить изображение!" << endl;
        return -1;
    }

    // Создание копии оригинала
    Mat result = original.clone();

    // Преобразование в оттенки серого
    Mat gray;
    cvtColor(result, gray, COLOR_BGR2GRAY);

    // Обнаружение линий методом Хафа
    Mat edges;
    Canny(gray, edges, 50, 150); // Получаем границы

    vector<Vec2f> lines;
    HoughLines(edges, lines, 1, CV_PI / 180, 360);

    // Отрисовка линий на result
    for (size_t i = 0; i < lines.size(); i++) {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        line(result, pt1, pt2, Scalar(0, 0, 255), 2); // красные линии
    }

    // Сглаживание (Гауссово размытие)
    Mat blurred;
    GaussianBlur(gray, blurred, Size(9, 9), 2);

    // Обнаружение окружностей методом HoughCircles
    vector<Vec3f> circles;
    HoughCircles(blurred, circles, HOUGH_GRADIENT, 1,
        blurred.rows / 4,  // minDist
        100, 60,           // param1, param2
        10, 0);            // minRadius, maxRadius

    // Отрисовка окружностей
    for (size_t i = 0; i < circles.size(); i++) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(result, center, radius, Scalar(226, 43, 138), 2); 
        circle(result, center, 2, Scalar(255, 0, 0), 3);       
    }

    // Показ финального изображения
    // Уменьшение изображения до размера экрана (Full HD)
    int screenWidth = 1920;
    int screenHeight = 1080;

    int w = result.cols;
    int h = result.rows;

    double scaleW = (double)screenWidth / w;
    double scaleH = (double)screenHeight / h;
    double scale = std::min(1.0, std::min(scaleW, scaleH)); // не увеличиваем

    Mat resizedResult;
    if (scale < 1.0)
        resize(result, resizedResult, Size(), scale, scale);
    else
        resizedResult = result;

    imshow("Изображение с линиями и окружностями", resizedResult);
    waitKey(0);
    destroyAllWindows();


    return 0;
}
