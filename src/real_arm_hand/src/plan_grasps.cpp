#include <ros/ros.h>
#include <moveit_msgs/GraspPlanning.h>

#include <mutex>  //用于保护共享数据不被多个线程同时访问
#include <tf/transform_datatypes.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PointStamped.h>
#include <moveit/move_group_interface/move_group_interface.h>
//yolo发布的物体中心点位置信息msg
#include <rs_yolo/Info.h>
// TF2
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

class PlanGrasp {
public:
    PlanGrasp(ros::NodeHandle &n) {
        //yolov5物体中心位置坐标订阅
        grasps_sub_ = n.subscribe("/detect_result_out", 1000,
                                            &PlanGrasp::grasps_callback, this);
        ros::NodeHandle pn("~");

        std::string move_group_arm;
        std::string move_group_gripper;
        pn.param("move_group_arm", move_group_arm, std::string("arm"));
        pn.param("move_group_gripper", move_group_gripper, std::string("gripper"));

        moveit::planning_interface::MoveGroupInterface move_group(move_group_arm);
        moveit::planning_interface::MoveGroupInterface gripper(move_group_gripper);

        // 等待ROS初始化
        ros::WallDuration(1.0).sleep();
        pick(move_group);

        ros::WallDuration(1.0).sleep();
        place(move_group);

    }

    void pick(moveit::planning_interface::MoveGroupInterface& move_group)
    {
        //创建要尝试的抓握向量
        std::vector<moveit_msgs::Grasp> grasps;
        grasps.resize(1);

        //设置grasp_pose的抓取姿势
        grasps[0].grasp_pose.header.frame_id = "base_link";
        tf2::Quaternion orientation;
        orientation.setRPY(3.05989673451415,1.5707963267949,0); //从 roll/pitch/yaw (in radians)创建此四元数 
        grasps[0].grasp_pose.pose.orientation = tf2::toMsg(orientation);
        grasps[0].grasp_pose.pose.position.x = object_position.x;
        grasps[0].grasp_pose.pose.position.y = object_position.y;
        grasps[0].grasp_pose.pose.position.z = object_position.z;

        //设置pre_grasp_approach的预备姿势
        grasps[0].pre_grasp_approach.direction.header.frame_id = "base_link";
        /* 方向设置为正x轴 */
        grasps[0].pre_grasp_approach.direction.vector.x = 1.0;
        grasps[0].pre_grasp_approach.min_distance = 0.095;
        grasps[0].pre_grasp_approach.desired_distance = 0.115;

        //设置post_grasp_retreat的后退姿势
        grasps[0].post_grasp_retreat.direction.header.frame_id = "base_link";
        /* 方向设置为正z轴 */
        grasps[0].post_grasp_retreat.direction.vector.z = 1.0;
        grasps[0].post_grasp_retreat.min_distance = 0.1;
        grasps[0].post_grasp_retreat.desired_distance = 0.25;

        //设置pre_grasp_posture打开爪子
        openGripper(grasps[0].pre_grasp_posture);

        //设置grasp_posture关闭爪子
        closedGripper(grasps[0].grasp_posture);

        //调用pick来抓取物体
        move_group.pick("object", grasps);

    }
    void place(moveit::planning_interface::MoveGroupInterface& move_group)
    {
        //创建物体放置的位置
        std::vector<moveit_msgs::PlaceLocation> place_location;
        place_location.resize(1);

        //设置放置时的姿态
        place_location[0].place_pose.header.frame_id = "base_link";
        tf2::Quaternion orientation;
        orientation.setRPY(3.05989673451415,1.5707963267949,0);  //和pick一样，平着抓
        place_location[0].place_pose.pose.orientation = tf2::toMsg(orientation);

        //设置放置的坐标(往旁边稍微移动一下,x y轴各+1)
        place_location[0].place_pose.pose.position.x = object_position.x+1;
        place_location[0].place_pose.pose.position.y = object_position.y+1;
        place_location[0].place_pose.pose.position.z = object_position.z;


        //设置pre_place_approach的预备姿势
        place_location[0].pre_place_approach.direction.header.frame_id = "base_link";
        /* 方向设置为负z轴 */
        place_location[0].pre_place_approach.direction.vector.z = -1.0;
        place_location[0].pre_place_approach.min_distance = 0.095;
        place_location[0].pre_place_approach.desired_distance = 0.115;

        //设置post_place_retreat的后退姿势
        place_location[0].post_place_retreat.direction.header.frame_id = "base_link";
        /* 方向设置为负y轴 */
        place_location[0].post_place_retreat.direction.vector.y = -1.0;
        place_location[0].post_place_retreat.min_distance = 0.1;
        place_location[0].post_place_retreat.desired_distance = 0.25;

        //设置post_place_posture打开爪子,不关了
        openGripper(place_location[0].post_place_posture);

        //调用place来放置物体
        move_group.place("object", place_location);

        
    }
    void openGripper(trajectory_msgs::JointTrajectory& posture)
    {
        /* 设置关节 */           
        posture.joint_names.resize(1);
        posture.joint_names[0] = "joint7";

        /* 打开 */
        posture.points.resize(1);
        posture.points[0].positions.resize(1);
        posture.points[0].positions[0] = 1;
        posture.points[0].time_from_start = ros::Duration(0.5);
    }

    void closedGripper(trajectory_msgs::JointTrajectory& posture)
    {
        /* 设置关节 */
        posture.joint_names.resize(1);
        posture.joint_names[0] = "joint7";

        /* 闭合 */
        posture.points.resize(1);
        posture.points[0].positions.resize(1);
        posture.points[0].positions[0] = 0;
        posture.points[0].time_from_start = ros::Duration(0.5);
    }

    void grasps_callback(const rs_yolo::Info &msg) 
    {       
        object_position.x=msg.x;
        object_position.y=msg.y;
        object_position.z=msg.z;
        object_position.classification=msg.classification;
        object_position.confidence=msg.confidence;
    }


private:
    std::mutex m_;

    ros::Subscriber grasps_sub_;
    rs_yolo::Info object_position;
};

int main(int argc, char **argv) {
    ros::init(argc, argv, "plan_grasps_service");
    ros::NodeHandle n;
    PlanGrasp pg(n);

    //ros::ServiceServer ss = n.advertiseService("plan_grasps", &PlanGrasp::plan_grasp, &pg);
    ros::spin();

    return 0;
}
