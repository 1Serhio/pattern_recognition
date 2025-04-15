#include <opencv2/opencv.hpp>
#include <iostream>

bool isSquare(const std::vector<cv::Point>& contour) {
    const double epsilon = 0.02 * cv::arcLength(contour, true);
    std::vector<cv::Point> approx;
    cv::approxPolyDP(contour, approx, epsilon, true);

    if (approx.size() == 4 && cv::isContourConvex(approx)) {
        double side1 = cv::norm(approx[0] - approx[1]);
        double side2 = cv::norm(approx[1] - approx[2]);
        double aspectRatio = side1 / side2;
        return aspectRatio > 0.8 && aspectRatio < 1.2;
    }
    return false;
}

int main() {
    cv::VideoCapture cap("D:/Рабочий стол/Распознавание образов/video.mp4");
    if (!cap.isOpened()) {
        std::cerr << "Ошибка: не удалось открыть видео.\n";
        return -1;
    }

    int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(cv::CAP_PROP_FPS);

    // Настройка записи видео
    cv::VideoWriter writer("result.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps,
        cv::Size(frame_width, frame_height));

    cv::Mat frame, gray, blurred, thresh;
    std::vector<std::vector<cv::Point>> contours;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);
        cv::adaptiveThreshold(blurred, thresh, 255, cv::ADAPTIVE_THRESH_MEAN_C,
            cv::THRESH_BINARY, 11, -2);

        contours.clear();
        cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (const auto& contour : contours) {
            double area = cv::contourArea(contour);
            if (area > 300 && isSquare(contour)) {
                cv::Rect boundingBox = cv::boundingRect(contour);

                // Отсечь объекты слишком близко к краям
                if (boundingBox.y < 5 || boundingBox.y + boundingBox.height > frame.rows - 5)
                    continue;

                // Вырезаем ROI и фильтруем по средней яркости
                cv::Mat roi = gray(boundingBox);
                cv::Mat roiMask;
                cv::inRange(roi, 30, 255, roiMask); // маска — игнорируем почти чёрные пиксели
                cv::Scalar meanVal = cv::mean(roi, roiMask);

                if (meanVal[0] < 100) continue; // слишком тёмный — не наш объект

                // Отрисовка
                cv::Point center(
                    boundingBox.x + boundingBox.width / 2,
                    boundingBox.y + boundingBox.height / 2
                );
                cv::drawContours(frame, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(0, 255, 0), 2);
                cv::circle(frame, center, 5, cv::Scalar(0, 0, 255), -1);
            }
        }


        cv::imshow("Tracking", frame);
        writer.write(frame); // запись кадра

        if (cv::waitKey(30) == 27) break; // ESC
    }

    cap.release();
    writer.release();
    cv::destroyAllWindows();
    std::cout << "Видео сохранено в файл result.avi\n";
    return 0;
}
