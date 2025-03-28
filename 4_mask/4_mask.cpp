#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;  // Теперь можно не писать cv:: перед функциями

int main() {
    // Загружаем изображение
    Mat image = imread(".../lena.png");

    // Проверяем, успешно ли загружено изображение
    if (image.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображение!" << std::endl;
        return -1;
    }

    // Получаем размеры изображения
    int height = image.rows;
    int width = image.cols;
    int halfHeight = height / 2;
    int halfWidth = width / 2;

    // Разделяем изображение на 4 части
    Mat topLeft = image(Rect(0, 0, halfWidth, halfHeight));
    Mat topRight = image(Rect(halfWidth, 0, halfWidth, halfHeight));
    Mat bottomLeft = image(Rect(0, halfHeight, halfWidth, halfHeight));
    Mat bottomRight = image(Rect(halfWidth, halfHeight, halfWidth, halfHeight));

    // Применяем разные обработки к каждой части
    Mat processedTopLeft, processedTopRight, processedBottomLeft, processedBottomRight;

    // 1. Инвертируем цвета в верхнем левом углу
    bitwise_not(topLeft, processedTopLeft);

    // 2. Переводим в оттенки серого в верхнем правом углу
    cvtColor(topRight, processedTopRight, COLOR_BGR2GRAY);
    cvtColor(processedTopRight, processedTopRight, COLOR_GRAY2BGR); // Преобразуем обратно в 3 канала

    // 3. Увеличиваем контрастность в нижнем левом углу
    processedBottomLeft = bottomLeft * 1.5;

    // 4. Применяем размытие в нижнем правом углу
    GaussianBlur(bottomRight, processedBottomRight, Size(15, 15), 5);

    // Создаём пустое изображение для результата
    Mat resultImage = Mat::zeros(height, width, image.type());

    // Вставляем обработанные части в соответствующие области
    processedTopLeft.copyTo(resultImage(Rect(0, 0, halfWidth, halfHeight)));
    processedTopRight.copyTo(resultImage(Rect(halfWidth, 0, halfWidth, halfHeight)));
    processedBottomLeft.copyTo(resultImage(Rect(0, halfHeight, halfWidth, halfHeight)));
    processedBottomRight.copyTo(resultImage(Rect(halfWidth, halfHeight, halfWidth, halfHeight)));

    // Масштабируем изображения для экрана 1920x1080
    Mat resizedOriginal, resizedProcessed;
    
    int targetWidth = 900;  // Половина экрана по ширине (~900 пикс.)
    int targetHeight = (image.rows * targetWidth) / image.cols;  // Сохраняем пропорции

    resize(image, resizedOriginal, Size(targetWidth, targetHeight));
    resize(resultImage, resizedProcessed, Size(targetWidth, targetHeight));

    // Объединяем два изображения в одно (размещаем их рядом)
    Mat displayImage;
    hconcat(resizedOriginal, resizedProcessed, displayImage);

    // Отображаем результат
    imshow("Оригинал и обработанное изображение", displayImage);

    // Ожидаем нажатия клавиши перед закрытием окон
    waitKey(0);

    return 0;
}
