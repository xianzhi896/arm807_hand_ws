/*
 * @Author: xbw-ubuntu 15116921911@example.com
 * @Date: 2022-08-26 16:49:09
 * @LastEditors: xbw-ubuntu 15116921911@example.com
 * @LastEditTime: 2022-09-11 11:08:44
 * @FilePath: /catkin_ws/src/real_arm/include/real_arm/RealRobot.h
 * @Description: 尝试编写action-serve，.h
 */
#ifndef REALROBOT_H
#define REALROBOT_H

#include "common.h"

#define N50 51200 * 50 / 3.1415926536 / 2
#define N51 51200 * 51 / 3.1415926536 / 2

struct ArmJoint
{
	float position;		 //位置
	float velocity;		 //速度
	float accelerations; //加速度
	float effort;		 //力
	float N;
	float joint_init;
	float motor_init;
	int same_dir;
};

class RealRobot
{
public:
	//传入action的名称
	RealRobot(string name);
	virtual ~RealRobot();

	// timer回调函数，用于接收下位机数据
	void timerCallback(const ros::TimerEvent &e);

	// goal回调函数
	void executeCB(const control_msgs::FollowJointTrajectoryGoalConstPtr &goal);

	//发布消息
	void jointStateUpdate();

	//路径执行
	void trackMove(trajectory_msgs::JointTrajectory trajectory);

	//数据写入下位机并执行
	void armWrite(trajectory_msgs::JointTrajectoryPoint point);

	//从下位机读取数据
	void armRead();

	void speeddesign(trajectory_msgs::JointTrajectory trajectory);

private:
	//-----------节点和定时器配置-----------
	const int joint_count;			   //自由度
	std::vector<string> joint_name;	   //关节名称
	ArmJoint joints[6];				   //关节
	ros::NodeHandle nh_;			   //句柄实例
	ros::Time time_current, time_prev; // ros系统时间
	ros::Timer timer;				   // ros定时器
	double period;					   //定时器周期

	//--------------串口配置----------------
	serial::Serial sp;	//创建一个serial类
	serial::Timeout to; //创建timeout

	//-------------串口接收参数-------------
	std::string str;
	std::string cjson_str_get;
	cJSON *cjson_obj_get;
	cJSON *cjson_data_get;
	cJSON *cjson_item_get;
	int32_t master[12]; // master to host :realloc,realspd
	int skill_array_size;
	bool cjson_start_get;

	//-------------串口发送参数--------------
	std::string cjson_str_send;
	cJSON *cjson_obj_send;
	cJSON *cjson_data_send;
	int32_t host[12]; // host to master :camdloc,camdspd

	//-------------action相关--------------
	// action名称
	string action_name;

	//定义action服务端实例
	// actionlib::ActionServer<control_msgs::FollowJointTrajectoryAction>  as_;
	actionlib::SimpleActionServer<control_msgs::FollowJointTrajectoryAction> as_;

	//反馈实例
	control_msgs::FollowJointTrajectoryFeedback feedback_;

	//用来反馈action目标的执行情况，客户端由此可以得知服务端是否执行成功了
	control_msgs::FollowJointTrajectoryResult result_;

	//路径点容器
	vector<trajectory_msgs::JointTrajectoryPoint> waypoints;

	//关节状态发布者 消息实例
	ros::Publisher joint_pub_;
	sensor_msgs::JointState msg;
};

#endif // ABBROBOT_H