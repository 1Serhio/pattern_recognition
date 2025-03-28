#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Загружаем изображение
    cv::Mat image = cv::imread("C:/Users/4mo/source/repos/FirstOpenCV/FirstOpenCV/lena.png");

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
    cv::Mat topLeft = image(cv::Rect(0, 0, halfWidth, halfHeight));
    cv::Mat topRight = image(cv::Rect(halfWidth, 0, halfWidth, halfHeight));
    cv::Mat bottomLeft = image(cv::Rect(0, halfHeight, halfWidth, halfHeight));
    cv::Mat bottomRight = image(cv::Rect(halfWidth, halfHeight, halfWidth, halfHeight));

    // Применяем разные обработки к каждой части
    cv::Mat processedTopLeft, processedTopRight, processedBottomLeft, processedBottomRight;

    // 1. Инвертируем цвета в верхнем левом углу
    cv::bitwise_not(topLeft, processedTopLeft);

    // 2. Переводим в оттенки серого в верхнем правом углу
    cv::cvtColor(topRight, processedTopRight, cv::COLOR_BGR2GRAY);
    cv::cvtColor(processedTopRight, processedTopRight, cv::COLOR_GRAY2BGR); // Преобразуем обратно в 3 канала

    // 3. Увеличиваем контрастность в нижнем левом углу
    processedBottomLeft = bottomLeft * 1.5;

    // 4. Применяем размытие в нижнем правом углу
    cv::GaussianBlur(bottomRight, processedBottomRight, cv::Size(15, 15), 5);

    // Создаём пустое изображение для результата
    cv::Mat resultImage = cv::Mat::zeros(height, width, image.type());

    // Вставляем обработанные части в соответствующие области
    processedTopLeft.copyTo(resultImage(cv::Rect(0, 0, halfWidth, halfHeight)));
    processedTopRight.copyTo(resultImage(cv::Rect(halfWidth, 0, halfWidth, halfHeight)));
    processedBottomLeft.copyTo(resultImage(cv::Rect(0, halfHeight, halfWidth, halfHeight)));
    processedBottomRight.copyTo(resultImage(cv::Rect(halfWidth, halfHeight, halfWidth, halfHeight)));

    // Масштабируем изображения для экрана 1920x1080
    cv::Mat resizedOriginal, resizedProcessed;

    int targetWidth = 900;  // Половина экрана по ширине (~900 пикс.)
    int targetHeight = (image.rows * targetWidth) / image.cols;  // Сохраняем пропорции

    cv::resize(image, resizedOriginal, cv::Size(targetWidth, targetHeight));
    cv::resize(resultImage, resizedProcessed, cv::Size(targetWidth, targetHeight));

    // Объединяем два изображения в одно (размещаем их рядом)
    cv::Mat displayImage;
    cv::hconcat(resizedOriginal, resizedProcessed, displayImage);

    // Отображаем результат
    cv::imshow("Оригинал и обработанное изображение", displayImage);

    // Ожидаем нажатия клавиши перед закрытием окон
    cv::waitKey(0);

    return 0;
}