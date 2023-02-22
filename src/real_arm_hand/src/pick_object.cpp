/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-02-16 21:38:24
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-02-17 14:59:14
 * @FilePath: /arm807_hand_ws/src/real_arm_hand/src/pick_object.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <cstdio>   //Input and Output operations

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit/collision_detection/collision_matrix.h>

#include <moveit_msgs/CollisionObject.h>
#include <moveit_msgs/GraspPlanning.h>

class PickObject {
public:
    moveit::planning_interface::MoveGroupInterface *arm;
    moveit::planning_interface::MoveGroupInterface *gripper;
    moveit::planning_interface::PlanningSceneInterface psi;
    std::string move_group_arm;
    std::string move_group_gripper;

    PickObject(){
        ros::NodeHandle pnh("~");
        ros::NodeHandle nh;
        pnh.param("move_group_arm", move_group_arm, std::string("arm"));
        pnh.param("move_group_gripper", move_group_gripper, std::string("gripper"));
        arm = new moveit::planning_interface::MoveGroupInterface(move_group_arm);
        gripper  = new moveit::planning_interface::MoveGroupInterface(move_group_gripper);
    }

    bool executePick() const {
        ros::NodeHandle n("~");
        arm->setPlanningTime(20.0);
        return arm->planGraspsAndPick() == moveit_msgs::MoveItErrorCodes::SUCCESS;
    }
};

int main(int argc, char **argv) {
    ros::init(argc, argv, "pick_object");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    ros::NodeHandle nh("~");

    PickObject po;

    bool success = false;

    ROS_INFO("Picking Object");
    while (ros::ok()) {
        if (success) {
            po.arm->setNamedTarget("extended");
            while (!po.arm->move())
                ROS_ERROR("moving to extended pose failed.");
            po.gripper->setNamedTarget("pinch_open");
            while (!po.gripper->move())
                ROS_ERROR("opening gripper failed.");
            po.arm->setNamedTarget("home");
            while (!po.arm->move())
                ROS_ERROR("moving home failed.");
            ros::Duration(5).sleep();
        }
        success = po.executePick();
    }

    return 0;
}
