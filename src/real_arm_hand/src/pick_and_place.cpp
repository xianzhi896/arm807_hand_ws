/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-02-07 21:16:00
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-02-16 21:52:42
 * @FilePath: /arm807_hand_ws/src/real_arm_hand/src/pick_and_place.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
'''
Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
Date: 2023-02-07 21:16:00
LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
LastEditTime: 2023-02-07 21:16:02
FilePath: /arm807_hand_ws/src/real_arm_hand/src/pick.py
Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
'''
#include "real_arm_hand/PickAndPlace.h"
class PickAndPlace
{
protected:

//   RealRobot robot("arm_position_controller/follow_joint_trajectory");

public:
    PickAndPlace()
        :nh__(""),priv_nh_("~"),mode_state_(0),demo_count_(0),pick_ar_id_(0)
    {
            initServiceClient();
            initSubscribe();
    }
        

    
    
    ~PickAndPlace(){
        if(ros::isStarted()) {
            ros::shutdown();
            ros::waitForShutdown();
        }
    }
 
    void initServiceClient()
    {

    }
    void initSubscribe()
    {
        
    }



    void JointStatesCallback(){

    }
    
    void kinematicsPoseCallback(){

    
    }

    void statesCallback(){
        
    }   
    void objectCallback(){

    }
}


int main(int argc, char *argv[])
{   //节点初始化
    ros::init(argc,argv,"pick_node_controller");
    ROS_INFO("pick_node_controller");
    ros::NodeHandle nh_;
    //定义一个listener
    tf::TransformListener listener(ros::Duration(10));  
    //声明类变量
    PickAndPlace pick_and_place;  
    //定时器：计划在某一速率下执行一次回调操作     
    ros::Timer publish_timer = nh_.createTimer(ros::Duration(0.100)/*100ms*/, \
                                                &PickAndPlace::publishCallback, \
                                                &pick_and_place);
    
    //定义存放变换关系的变量
    tf::StampedTransform transform;
    //监听两个坐标系之间的变换
    while (ros::ok())
    {
        try{
        if(pick_and_place.mode_state_ != DEMO_START ){
            listener.lookupTransform("world", "object", ros::Time(0), transform);
        }
        //ROS_INFO("tf position %.3f , %.3f, %.3f" , transform.getOrigin().x(), transform.getOrigin().y() , transform.getOrigin().z());
        }
        catch (tf::TransformException ex){
        ROS_ERROR("%s",ex.what());
        ros::Duration(1.0).sleep();
        }
        ros::spinOnce();
    }
    return 0;
}

