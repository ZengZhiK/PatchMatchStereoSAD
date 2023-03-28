#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <ctime>
#include "pm.h"

// PatchMatch.exe F:\1_Code\2_CPP\PatchMatchStereoSAD\imgs\Speckle\BL75\qinnv\im0.png F:\1_Code\2_CPP\PatchMatchStereoSAD\imgs\Speckle\BL75\qinnv\im1.png F:\1_Code\2_CPP\PatchMatchStereoSAD\imgs\Speckle\BL75\qinnv\PM3SAD11MF7 11 500 3
// PatchMatch.exe F:\1_Code\2_CPP\PatchMatchStereoSAD\imgs\Speckle\BL150\qinnv\im0.png F:\1_Code\2_CPP\PatchMatchStereoSAD\imgs\Speckle\BL150\qinnv\im1.png F:\1_Code\2_CPP\PatchMatchStereoSAD\imgs\Speckle\BL150\qinnv\PM3SAD11MF7 11 670 3
// PatchMatch.exe F:\1_Code\2_CPP\PatchMatchStereoSAD\imgs\Speckle\BL150\gdqn\im0.png F:\1_Code\2_CPP\PatchMatchStereoSAD\imgs\Speckle\BL150\gdqn\im1.png F:\1_Code\2_CPP\PatchMatchStereoSAD\imgs\Speckle\BL150\gdqn\PM3SAD11MF7 11 670 3
// PatchMatch.exe F:\1_Code\2_CPP\PatchMatchStereoSAD\imgs\MiddEval3\im0.png F:\1_Code\2_CPP\PatchMatchStereoSAD\imgs\MiddEval3\im1.png F:\1_Code\2_CPP\PatchMatchStereoSAD\imgs\MiddEval3\PM3SAD11MF7 11 75 3

bool check_image(const cv::Mat &image, std::string name = "Image") {
    if (!image.data) {
        std::cerr << name << " data not loaded.\n";
        return false;
    }
    return true;
}


bool check_dimensions(const cv::Mat &img1, const cv::Mat &img2) {
    if (img1.cols != img2.cols or img1.rows != img2.rows) {
        std::cerr << "Images' dimensions do not corresponds.";
        return false;
    }
    return true;
}


int main(int argc, char **argv) {
    if (argc != 7) {
        std::cerr << "parameter number error." << std::endl;
        return 1;
    }

    std::cerr << "left img: " << argv[1] << std::endl;
    std::cerr << "right img: " << argv[2] << std::endl;

    std::string out_dir = argv[3];
    std::cerr << "result dir: " << out_dir << std::endl;

//    const float alpha = 0.9f;
//    const float gamma = 10.0f;
//    const float tau_c = 10.0f;
//    const float tau_g = 2.0f;

    int window_size = atoi(argv[4]);
    int max_disparity = atoi(argv[5]);
    int plane_penalty = 120;

    int iterations = atoi(argv[6]);

    std::cerr << "window_size: " << window_size
              << ", max_disparity: " << max_disparity
              << ", iterations: " << iterations << std::endl;

    // Reading images
    cv::Mat img1 = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    cv::Mat img2 = cv::imread(argv[2], cv::IMREAD_GRAYSCALE);
    cv::cvtColor(img1, img1, cv::COLOR_GRAY2BGR);
    cv::cvtColor(img2, img2, cv::COLOR_GRAY2BGR);

    // Image loading check
    if (!check_image(img1, "Image 1") or !check_image(img2, "Image 2"))
        return 1;

    // Image sizes check
    if (!check_dimensions(img1, img2))
        return 1;

    // processing images
    clock_t startTime = clock();
    pm::PatchMatch patch_match(window_size, max_disparity, plane_penalty);
    patch_match.set(img1, img2);
    patch_match.process(iterations);
    patch_match.postProcess();
    clock_t endTime = clock();
    std::cerr << "The run time is: " << (double) (endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;

    cv::Mat1f disp1 = patch_match.getLeftDisparityMap();
    cv::Mat1f disp2 = patch_match.getRightDisparityMap();

//    std::vector<int> compression_params;  //无损压缩参数
//    compression_params.push_back(cv::IMWRITE_TIFF_COMPRESSION);
//    compression_params.push_back(1);
//    cv::imwrite(out_dir + "/left_disparity.tiff", disp1, compression_params);
//    cv::imwrite(out_dir + "/right_disparity.tiff", disp2, compression_params);

    std::string command;
    command = "mkdir " + out_dir;
    system(command.c_str());
    cv::imwrite(out_dir + "/left_disparity.pfm", disp1);
    cv::imwrite(out_dir + "/right_disparity.pfm", disp2);

    cv::normalize(disp1, disp1, 0, 255, cv::NORM_MINMAX);
    cv::normalize(disp2, disp2, 0, 255, cv::NORM_MINMAX);

    try {
        cv::imwrite(out_dir + "/left_disparity.png", disp1);
        cv::imwrite(out_dir + "/right_disparity.png", disp2);
    }
    catch (std::exception &e) {
        std::cerr << "Disparity save error.\n" << e.what();
        return 1;
    }

    return 0;
}
