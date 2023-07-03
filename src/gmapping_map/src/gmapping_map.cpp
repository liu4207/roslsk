#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>
#include "ros/ros.h"
#include "nav_msgs/OccupancyGrid.h"

cv::Mat map;
bool mapInitialized = false;

void drawMap(const nav_msgs::OccupancyGrid::ConstPtr &ptr)
{
    std::cout << "resolution: " << ptr->info.resolution << std::endl;
    std::cout << "width: " << ptr->info.width << std::endl;
    std::cout << "height: " << ptr->info.height << std::endl;

    double scale = 1.0;
    int width = 1200;
    int height = 1200;
    cv::Point offset = {-1600, -1600};
    map = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);

    for (int i = 0; i < ptr->info.width * ptr->info.height; ++i)
    {
        int x = (i % ptr->info.width + offset.x) * scale;
        int y = (i / ptr->info.width + offset.y) * scale;

        if (ptr->data[i] == -1)
        {
            cv::rectangle(map, cv::Rect(x, y, scale, scale), cv::Scalar(220, 220, 220), -1);
        }
        else if (ptr->data[i] >= 80)
        {
            cv::rectangle(map, cv::Rect(x, y, scale, scale), cv::Scalar(50, 50, 50), -1);
        }
        else
        {
            cv::rectangle(map, cv::Rect(x, y, scale, scale), cv::Scalar(200, 240, 200), -1);
        }
    }

    mapInitialized = true;
}

void gmappingThread()
{
    system("rosrun gmapping slam_gmapping");
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "show_map");
    ros::NodeHandle nh;

    ros::Subscriber subscriber = nh.subscribe<nav_msgs::OccupancyGrid>("/map", 1000, drawMap);

    // 启动gmapping算法节点
    std::thread gmapping(gmappingThread);

    cv::namedWindow("Map", cv::WINDOW_NORMAL);

    while (ros::ok())
    {
        ros::spinOnce();

        // 检查地图是否已初始化
        if (mapInitialized)
        {
            if (!map.empty())
            {
                cv::imshow("Map", map);
                cv::waitKey(10);
            }
            else
            {
                std::cerr << "Failed to load map" << std::endl;
            }
        }
        else
        {
            std::cerr << "Failed to display map" << std::endl;
        }
    }

    // 关闭图像窗口
    cv::destroyAllWindows();

    return 0;
}
