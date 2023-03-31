/*
    This file is part of ALPHA AUV project.

    This project is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This project is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the project.  If not, see <https://www.gnu.org/licenses/>.

*/


#include "usbl_node.hpp"
#include "geometry_msgs/PolygonStamped.h"
#include "geometry_msgs/Pose.h"

USBLNode::USBLNode()
{

    m_nh.reset(new ros::NodeHandle(""));
    m_pnh.reset(new ros::NodeHandle("~"));

    m_pnh->param<std::string>("frame_id", m_frame_id, "world");

    m_usbl_subscriber = m_nh->subscribe(
        "usbl", 10, &USBLNode::f_usbl_callback, this);

    m_waypoint_publisher = m_nh->advertise
        <geometry_msgs::PolygonStamped>("update_waypoints", 10);

}

void USBLNode::f_usbl_callback(const geometry_msgs::Pose &msg)
{
    geometry_msgs::PolygonStamped new_waypoint;

    //Do I need current global position? Another subscriber?


    new_waypoint.header.stamp = ros::Time::now();

    m_waypoint_publisher.publish(new_waypoint);

}

int main(int argc, char* argv[]) {

    ros::init(argc, argv, "usbl_nav");

    USBLNode d;

    ros::spin();

    return 0;
}