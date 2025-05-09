#include <opencv2/opencv.hpp>

using namespace cv;

int main() {
    // ��������� �����������
    Mat image = imread("C:/Projects/First_opecv/Lena.png");

    // ���������, ������� �� ��������� �����������
    if (image.empty()) {
        std::cerr << "������: �� ������� ��������� �����������!" << std::endl;
        return -1;
    }

    // ������ ����� (��������� �����, �������� �����, ����, �������)
    line(image, Point(100, 215), Point(265, 70), Scalar(0, 0, 255), 2);

    // ������ ������������� (������� ����� ����, ������ ������ ����, ����, �������)
    rectangle(image, Point(150, 252), Point(250, 326), Scalar(0, 0, 255), 2);

    // ������ ���� (�����, ������, ����, �������)
    circle(image, Point(190, 200), 50, Scalar(0, 0, 255), 2);

    // ������ ������ (�����, ������� ����, ���� ��������, ��������� ����, �������� ����, ����, �������)
    ellipse(image, Point(140, 90), Size(100, 60), -35, 0, 360, Scalar(0, 0, 255), 2);

    // ���������� ����� ��������������
    std::vector<Point> polygon_points = {Point(100, 210), Point(265, 65),
                                         Point(250, 326), Point(150, 326)};
    polylines(image, polygon_points, true, Scalar(255, 0, 255), 2);

    // ��������� ����� (�����, ���������, �����, �������, ����, �������)
    putText(image, "WOW", Point(167, 300), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 255), 2);

    // ������� ���� � ������ ��� ������
    namedWindow("����������� � ��������������� ���������", WINDOW_NORMAL);
    resizeWindow("����������� � ��������������� ���������", 800, 700);  // ������������� ������ ����


    // ���������� ����������� � ����
    imshow("����������� � ��������������� ���������", image);

    // ������� ������� ������� ����� ��������� ����
    waitKey(0);

    return 0;
}
