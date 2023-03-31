#include "uno_lqe_ros.h"
#include "memory"

UnoLqeNode::UnoLqeNode() {

    m_nh.reset(new ros::NodeHandle(""));

    m_pnh.reset(new ros::NodeHandle("~"));

    double A, B, H, Q, R, P, x;

    m_pnh->param<double>("A", A, 1.0);

    m_pnh->param<double>("B", B, 0.0);

    m_pnh->param<double>("H", H, 1.0);

    m_pnh->param<double>("Q", Q, 1e-3);

    m_pnh->param<double>("R", R, 1e-3);

    m_pnh->param<double>("P", P, 1.0);

    m_pnh->param<double>("x", x, 0.0);

    m_filter.reset(new UnoLqe(x, A, B, H, Q, R, P));

    std::string in_topic;
    m_pnh->param<std::string>("in_topic", in_topic, "lqe_input");

    std::string out_topic;
    m_pnh->param<std::string>("out_topic", out_topic, "lqe_output");

    m_subscriber = m_nh->subscribe(in_topic, 10, &UnoLqeNode::f_callback, this);

    m_publisher = m_nh->advertise<mvp_msgs::Float64StampedWithVariance>(out_topic, 10);

}

void UnoLqeNode::f_callback(const mvp_msgs::Float64Stamped::ConstPtr& msg) {
    auto z = msg->data;

    m_filter->predict();

    m_filter->update(z);

    mvp_msgs::Float64StampedWithVariance out;

    m_filter->state(&(out.data), &(out.variance));

    out.header = msg->header;

    m_publisher.publish(out);
}