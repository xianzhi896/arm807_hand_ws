#include "real_arm_hand/RealRobot.h"

//这是action的初始化，传入action的名称
/**
 * @breif:初始化一个节点，初始化变量，启动服务端boost::bind(&RealRobot::executeCB, this,_1)：boost这个是固定用法，
 * 就不深究了，它绑定了一个名为RealRobot::executeCB的函数，RealRobot是as_所属的类的名称，
 * executeCB（）是其中的一个成员函数，这个函数写在了as_构造函数的第三个参数位，表示它是一个接受action目标的回调函数（函数名可以自己更改），
 * 当action客户端请求一个动作时，这个动作被认为是一个目标，传递给action服务端，此时就moveit的action客户端而言，
 * 这个目标就是机械臂运动轨迹，服务端接收到目标后，会自动回调executeCB这个函数，而目标（轨迹）会作为参数传递进来。
 * @param {string} name
 * @return {*}
 */
RealRobot::RealRobot(string name)
    : as_(nh_, name, boost::bind(&RealRobot::executeCB, this, _1), false), joint_count(6)
{
    //动作名
    action_name = name;
    ros::NodeHandle nh_private("~");

    //初始化关节变量
    joint_name.resize(joint_count);
    msg.name.resize(joint_count);
    msg.position.resize(joint_count);
    msg.velocity.resize(joint_count);
    msg.effort.resize(joint_count);
    msg.header.frame_id = "";

    //对joint_state的中的名称进行配置，是string格式，比较麻烦点
    stringstream ss;
    ss.clear();
    ss.str("");
    for (size_t i = 0; i < joint_count; i++)
    {
        ss << "joint" << i + 1;
        joint_name[i] = ss.str();
        msg.name[i] = joint_name[i];
        ss.clear();
        ss.str("");
    }
    //--------------------------------电机配置初始化-------------------------------
    float N_init[6] = {N51, N50, N50, N51, N50, N50}; //减速比
    int dir_init[6] = {1, -1, -1, 1, -1, 1};           //转动方向
    float joints_init[6] = {0, 0, 0, 0, 0, 0};        //关节初始位置
    float motors_init[6] = {0, 0, 0, 0, 0, 0};        //电机初始为
    for (int i = 0; i < 6; i++)
    {
        joints[i].N = N_init[i];
        joints[i].same_dir = dir_init[i];
        joints[i].joint_init = joints_init[i];
        joints[i].motor_init = motors_init[i];
    }

    //------------------------------串口相关配置初始化---------------------------------
    to = serial::Timeout::simpleTimeout(100); //创建timeout
    sp.setPort("/dev/ttyUSB0");               //设置要打开的串口名称
    sp.setBaudrate(115200);                   //设置串口通信的波特率
    sp.setTimeout(to);                        //串口设置timeout
    try
    {
        sp.open();
    }
    catch (serial::IOException &e)
    {
        ROS_ERROR_STREAM("Unable to open port.");
    }
    if (sp.isOpen())
        ROS_INFO_STREAM("/dev/ttyUSB0 is opened.");

    //--------串口接收参数初始化----------
    str = "";
    cjson_str_get;
    cjson_obj_get = NULL;
    cjson_data_get = NULL;
    cjson_item_get = NULL;
    skill_array_size = 0;
    cjson_start_get = 0;

    //---------串口发送参数初始化----------
    cjson_str_send;
    cjson_obj_send = cJSON_CreateObject();
    cjson_data_send = cJSON_CreateArray();
    //生成结构
    for (int i = 0; i < 12; i++)
    {
        //这里是初始化的参数，之后再设定。
        cJSON_AddItemToArray(cjson_data_send, cJSON_CreateNumber(0));
    }
    cJSON_AddItemToObject(cjson_obj_send, "host", cjson_data_send);
    //--------------------------------------------------------------------------

    //关节发布者初始化，这里的queue_size是1，目前先这样，之后看情况而定
    joint_pub_ = nh_.advertise<sensor_msgs::JointState>("/joint_states", 1);

    //定时器启动，定时器周期为0.01秒,之后视情况而定。
    period = 0.01;
    timer = nh_.createTimer(ros::Duration(period), &RealRobot::timerCallback, this);

    //服务端启动
    as_.start();
}

RealRobot::~RealRobot()
{
    //释放资源
    cout << "机械臂程序已退出，请等待，并关掉电源，以免电机过热。" << endl;
}

// timer回调函数，用于接收下位机数据
void RealRobot::timerCallback(const ros::TimerEvent &e)
{
    jointStateUpdate();
}

// goal回调函数
void RealRobot::executeCB(const control_msgs::FollowJointTrajectoryGoalConstPtr &goal)
{
    std::cout << "executeCB" << std::endl;

    //数据发送给下位机，下位机用于执行
    trackMove(goal->trajectory);

    //动作完成，反馈结果，设置完成状态
    result_.error_code = result_.SUCCESSFUL;
    as_.setSucceeded(result_);

    /*动作未完成
    result_.error_code = result_.GOAL_TOLERANCE_VIOLATED;
    as_.setAborted(result_);
    */
}

//发布者函数
void RealRobot::jointStateUpdate()
{
    // cout << "jointStateUpdate " << endl;
    armRead();
    // joint_angle = (motor_angle - motor_init) / N * dir + joint_init;
    for (int i = 0; i < 6; i++)
    {
        msg.position[i] = (master[i] - joints[i].motor_init) / joints[i].N * joints[i].same_dir + joints[i].joint_init;
    }
    for (int i = 6; i < 12; i++)
    {
        msg.velocity[i - 6] = (master[i] / joints[i - 6].N);
    }
    msg.header.stamp = ros::Time::now();
    joint_pub_.publish(msg);
}

/**
 * @brief: 速度设计，对trajectory中的速度进行再设定
 * @return {*}
 */
void RealRobot::speeddesign(trajectory_msgs::JointTrajectory trajectory)
{
    for (size_t seq = 0; seq < trajectory.points.size(); seq++)
    {
        waypoints.push_back(trajectory.points[seq]);
    }

    //各关节规划距离
    double distance[6] = {};
    double maxdistance = 0;
    for (int i = 0; i < 6; i++)
    {
        distance[i] = abs(waypoints[waypoints.size() - 1].positions[i] - msg.position[i])*joints[i].N;
    }

    //寻找最大距离
    for (int i = 0; i < 6; i++)
    {
        if (distance[i] > maxdistance)
            maxdistance = distance[i];
    }

    for (size_t seq = 0; seq < waypoints.size(); seq++)
    {
        // std::cout<<waypoints[seq]<<std::endl;
        for (int i = 0; i < 6; i++)
        {
            waypoints[seq].velocities[i] = distance[i] / maxdistance * 1.2;
        }
        // std::cout<<" after design "<<waypoints[seq]<<std::endl;
    }

}

//路径执行
void RealRobot::trackMove(trajectory_msgs::JointTrajectory trajectory)
{
    cout << "trackMove start........" << endl;
    feedback_.desired.positions.clear();

    //路径写入wayPoint中
    cout << "\033[1m\033[35mPoints count : \033[0m" << trajectory.points.size() << endl;

    speeddesign(trajectory);

    // for (size_t seq = 0; seq < trajectory.points.size(); seq++)
    // {
    //     waypoints.push_back(trajectory.points[seq]);
    // }

    //将路点的终点写入feedback_中
    //这里用的是直接将waypoints中的终点写入feedback，为什么要这样做？？？
    for (size_t i = 0; i < joint_count; i++)
    {
        feedback_.desired.positions.push_back(waypoints[waypoints.size() - 1].positions[i]);
    }

    //写入虚拟下位机，且下位机按照moveit要求执行运动
    // cout << "waypoints" << waypoints.size() << endl;
    for (size_t seq = 0; seq < waypoints.size(); seq++)
    {
        //这是关键
        cout << "armwrite" << seq << endl;
        armWrite(waypoints[seq]);
        //当前路径点的反馈,按照每个关节进行写入
        for (size_t i = 0; i < joint_count; i++)
        {
            feedback_.actual.positions.push_back(msg.position[i]);
            feedback_.actual.velocities.push_back(msg.velocity[i]);
            feedback_.actual.effort.push_back(msg.effort[i]);
        }
        feedback_.header.stamp = msg.header.stamp;
        as_.publishFeedback(feedback_);

        // 10hz速率进行反馈，频率这个再说
        usleep(10000);
    }

    //路径点执行完毕
    cout << "movetrack finish" << endl;
    waypoints.clear();
}

//数据写入下位机并执行(参考)
void RealRobot::armWrite(trajectory_msgs::JointTrajectoryPoint point)
{
    //更新数据
    // motor_angle = (joint_angle - joint_init) * N * dir + motor_init;
    for (int i = 0; i < 6; i++)
    {
        host[i] = (point.positions[i] - joints[i].joint_init) * joints[i].N * joints[i].same_dir + joints[i].motor_init;
        host[i + 6] = point.velocities[i] * joints[i].N * joints[i].same_dir;
        // host[i + 6] = 512000;
    }
    // cout << "armwrite ok" << host[6] << endl;

    for (int i = 0; i < 12; i++)
    {
        cJSON_ReplaceItemInArray(cjson_data_send, i, cJSON_CreateNumber(host[i]));
    }
    // cout << "updata ok" << endl;
    cjson_str_send = cJSON_Print(cjson_obj_send);
    // cout << "generate str ok" << endl;
    sp.write(cjson_str_send);
    cout << cjson_str_send << endl;
}

//从下位机读取数据(参考)
void RealRobot::armRead()
{
    size_t n = sp.available();
    if (n != 0)
    {
        //读取全部数据，效果要更好一点
        std::string s = sp.read(n);
        for (int i = 0; i < n; i++)
        {
            std::string f(1, s[i]); //将s[i]的char字符强制转换为string类型
            if (f == "{")
            {
                str = ""; //检测为{，清空数据，开始下个结构体
                cjson_start_get = 1;
            }
            else if (f == "}")
            {
                cjson_str_get = str + "}"; //检测为}，结构体读取完毕，对格式进行补足{}
                cjson_start_get = 0;
            }
            if (cjson_start_get)
                str = str + f; //将字符链接为内容；
        }
        // std::cout << s << std::endl;
    }
    // std::cout << n << std::endl;

    // 对读取的数据进行cjson解码
    const char *str2 = cjson_str_get.c_str();
    // std::cout << cjson_str_get << std::endl;
    // printf("%s", str2);
    cjson_obj_get = cJSON_Parse(str2);
    if (cjson_obj_get == NULL)
    {
        printf("parse fail.\n");
    }
    else
    {
        cjson_data_get = cJSON_GetObjectItem(cjson_obj_get, "master");
        int cjson_size = cJSON_GetArraySize(cjson_data_get);
        // std::cout << cjson_size<< std::endl;
        if (cjson_size == 12)
        {
            for (int i = 0; i < 12; i++)
            {
                master[i] = cJSON_GetArrayItem(cjson_data_get, i)->valueint;
                // std::cout << i <<": "<< master[i] << std::endl;
            }
        }
        // printf('motor2loc = %d',master[1]);
    }
}