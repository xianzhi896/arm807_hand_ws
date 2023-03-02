/*
 * @Author: xbw-ubuntu 15116921911@example.com
 * @Date: 2022-09-14 10:56:12
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-02-08 19:20:00
 * @FilePath: /catkin_ws/src/arm807/real_arm/src/arm_main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "real_arm_hand/RealRobot.h"

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "real_arm_node");
    RealRobot robot("arm_position_controller/follow_joint_trajectory");
    ros::AsyncSpinner spinner(1);
    spinner.start();

    ros::Time time_current = ros::Time::now();
    ros::Time  time_prev = time_current;
    double elapsed = 0;
    while (ros::ok())
    {
        time_current = ros::Time::now();
        elapsed = (time_current - time_prev).toSec();
        if(elapsed >= 0.1)
        {
            time_prev = time_current;
            robot.jointStateUpdate();
        }
        usleep(5000);
    }
    cout << "\033[1m\033[32m机械臂程序正在关闭...\033[0m" << endl;
    return 0;
}