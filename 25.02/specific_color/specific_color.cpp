#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main() {
    // 1. Загрузка изображения
    Mat image = imread(".../lena.png");
    if (image.empty()) {
        cout << "Не удалось загрузить изображение!" << endl;
        return -1;
    }

    // 2. Преобразование в HSV
    Mat hsv;
    cvtColor(image, hsv, COLOR_BGR2HSV);

    // 3. Диапазон фиолетового цвета в HSV
    Scalar lower_purple(125, 50, 50);  // оттенок, насыщенность, яркость
    Scalar upper_purple(155, 255, 255);

    // 4. Создание маски
    Mat mask;
    inRange(hsv, lower_purple, upper_purple, mask);

    // 5. Применение маски
    Mat result;
    bitwise_and(image, image, result, mask);

    // 6. Объединение оригинала и результата на одном экране
    Mat combined;
    hconcat(image, result, combined); // горизонтальное объединение

    // 7. Уменьшение изображения под экран (Full HD)
    double scale = min(1.0, 1920.0 / combined.cols);
    Mat resized;
    resize(combined, resized, Size(), scale, scale);

    // 8. Отображение
    imshow("Оригинал | Фиолетовый ", resized);
    waitKey(0);
    destroyAllWindows();

    return 0;
}
