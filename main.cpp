#include <bitset>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

void showColorFileImage(const std::vector<char>& file, const int image_width) {
    cv::Mat file_matrix((file.size() / 3) / image_width, image_width, CV_8UC3);

    auto mit = file_matrix.begin<cv::Vec3b>();
    for (auto it = file.cbegin(); it != file.cend(); ++mit) {
        (*mit)[0] = static_cast<uchar>(*it++);
        (*mit)[1] = static_cast<uchar>(*it++);
        (*mit)[2] = static_cast<uchar>(*it++);
    }

    cv::imshow("File Image", file_matrix);
    cv::waitKey(0);
}

void showBlackWhiteFileImage(const std::vector<char>& file,
                             const int image_width) {
    cv::Mat file_matrix((file.size() * 8) / image_width, image_width, CV_8UC1);

    auto mit = file_matrix.begin<uchar>();
    for (auto it = file.cbegin(); it != file.cend(); ++it) {
        std::bitset<8> bits(*it);
        for (int i = 0; i < 8; ++i) {
            *mit++ =
                (bits[i] ? static_cast<uchar>(0) : static_cast<uchar>(255));
        }
    }

    cv::imshow("File Image", file_matrix);
    cv::waitKey(0);
}

std::vector<char> readFile(const std::string& file_path) {
    std::vector<char> file;

    try {
        std::ifstream fstream;
        fstream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fstream.open(file_path, std::ios::binary | std::ios::ate);

        std::ifstream::pos_type pos = fstream.tellg();
        fstream.seekg(0, std::ios::beg);
        fstream.read(&file[0], pos);

        return file;
    } catch (...) {
        std::cerr << "Exception while reading file!" << std::endl;
    }

    return file;
}

int main(int argc, char* argv[]) {
    const std::string file_path = argv[1];
    if (argc <= 0 || file_path.empty()) {
        std::cerr << "No file path param!" << std::endl;
        return EXIT_FAILURE;
    }

    const std::vector<char> file = readFile(file_path);
    if (file.empty()) {
        std::cerr << "Failed to read a file!" << std::endl;
        return EXIT_FAILURE;
    }

    constexpr int image_width = 128;
    showBlackWhiteFileImage(file, image_width);
    showColorFileImage(file, image_width);

    return EXIT_SUCCESS;
}
