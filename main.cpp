#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include "pm.h"

// PatchMatch.exe F:\1_Code\2_CPP\PatchMatchStereoSAD\Speckle_Data\data75F\im0.png F:\1_Code\2_CPP\PatchMatchStereoSAD\Speckle_Data\data75F\im1.png F:\1_Code\2_CPP\PatchMatchStereoSAD\Speckle_Data\data75F\PM3SAD11MF7 11 500 3
// PatchMatch.exe F:\1_Code\2_CPP\PatchMatchStereoSAD\Speckle_Data\data150F\im0.png F:\1_Code\2_CPP\PatchMatchStereoSAD\Speckle_Data\data150F\im1.png F:\1_Code\2_CPP\PatchMatchStereoSAD\Speckle_Data\data150F\PM3SAD11MF7 11 670 3
// PatchMatch.exe F:\1_Code\2_CPP\PatchMatchStereoSAD\MiddEval3_Data\trainingQ\Adirondack\im0.png F:\1_Code\2_CPP\PatchMatchStereoSAD\MiddEval3_Data\trainingQ\Adirondack\im1.png F:\1_Code\2_CPP\PatchMatchStereoSAD\MiddEval3_Data\trainingQ\Adirondack\PM3SAD11MF7 11 75 3

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

    std::string out_dir = argv[3];

    const float alpha = 0.9f;
    const float gamma = 10.0f;
    const float tau_c = 10.0f;
    const float tau_g = 2.0f;

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
    pm::PatchMatch patch_match(alpha, gamma, tau_c, tau_g, window_size, max_disparity, plane_penalty);
    patch_match.set(img1, img2);
    patch_match.process(iterations);
    patch_match.postProcess();

    cv::Mat1f disp1 = patch_match.getLeftDisparityMap();
    cv::Mat1f disp2 = patch_match.getRightDisparityMap();

    std::vector<int> compression_params;  //无损压缩参数
    compression_params.push_back(cv::IMWRITE_TIFF_COMPRESSION);
    compression_params.push_back(1);
    imwrite(out_dir + "/left_disparity.tiff", disp1, compression_params);
    imwrite(out_dir + "/right_disparity.tiff", disp2, compression_params);

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
