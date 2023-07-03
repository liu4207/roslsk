#include "show_color_camera.h"
void imageCb(const sensor_msgs::ImageConstPtr &msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception &e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }
    cv::imshow("color_camera", cv_ptr->image);
    cv::waitKey(3);
}
int main(int argc, char **argv)
{
    ros::init(argc, argv, "show_color_camera");
    ros::NodeHandle n;
    image_transport::ImageTransport it(n);
    image_transport::Subscriber image_sub_ = it.subscribe("/camera/color/image_raw", 1000, imageCb);
    ros::spin();

    return 0;
}