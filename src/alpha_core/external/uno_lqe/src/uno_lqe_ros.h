#ifndef UNO_LQE_UNO_LQE_ROS_H
#define UNO_LQE_UNO_LQE_ROS_H

#include "ros/ros.h"
#include "uno_lqe/uno_lqe.h"
#include "mvp_msgs/Float64Stamped.h"
#include "mvp_msgs/Float64StampedWithVariance.h"

class UnoLqeNode {
private:

    ros::NodeHandlePtr m_nh;

    ros::NodeHandlePtr m_pnh;

    UnoLqe::Ptr m_filter;

    ros::Subscriber m_subscriber;

    ros::Publisher m_publisher;

    void f_callback(const mvp_msgs::Float64Stamped::ConstPtr& msg);

public:

    UnoLqeNode();



};

#endif //UNO_LQE_UNO_LQE_ROS_H
