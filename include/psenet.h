// Created by luozhiwang (luozw1994@outlook.com)
// Date: 2020/7/1

#ifndef TENSORRT_PSENET_H
#define TENSORRT_PSENET_H

#include <thread>
#include <mutex>
#include <map>

#include "tensorrt.h"
#include "utils.h"

class Psenet : private TensorRT{
private:
    common::DetectParams mDetectParams;
    std::mutex mMutex;
private:

    //! If the image is padded, bboxes need to be restored.
    //! \param ih Input image height
    //! \param iw Input image width
    //! \param oh Output image height
    //! \param ow Output image width
    //! \param mask cv::Mat
    cv::Mat transformBbx(const int &ih, const int &iw, const int &oh, const int &ow, cv::Mat &mask, std::vector<cv::RotatedRect> &RBox, bool is_padding=true);

    std::vector<cv::RotatedRect> point2RBox(std::map<int, std::vector<cv::Point>>);
public:

    //! Initializing
    //! \param inputParams
    //! \param trtParams
    //! \param yoloParams
    Psenet(common::InputParams inputParams, common::TrtParams trtParams, common::DetectParams yoloParams);

    //! Read images into buffer
    //! \param images
    //! \return Float32 file data
    std::vector<float> preProcess(const std::vector<cv::Mat> &images) const;

    //! Post Process for Psenet
    //! \param bufferManager It contains inference result
    //! \param postThres
    //! \param nms
    //! \return cv::Mat CV8U point_value = 0, 1, 2... cid (0 for background), contours
    std::tuple<cv::Mat, std::map<int, std::vector<cv::Point>>> postProcess(common::BufferManager &bufferManager, float postThres=-1);

    //! Init Inference Session
    //! \param initOrder 0==========> init from SerializedPath. If failed, init from onnxPath.
    //!                             1 ==========> init from onnxPath and save the session into SerializedPath if it doesnt exist.
    //!                             2 ==========> init from onnxPath and force to save the session into SerializedPath.
    //! \return true if no errors happened.
    bool initSession(int initOrder) override;

    //!
    //! \param image
    //! \param posThres Post process threshold.
    //! \return Segmentation Mask CV8U, RotateRect
    std::tuple<cv::Mat, std::vector<cv::RotatedRect>> predOneImage(const cv::Mat &image, float postThres=-1);

};


#endif //TENSORRT_PSENET_H
