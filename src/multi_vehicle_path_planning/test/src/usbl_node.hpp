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

    Author: Emir Cem Gezer
    Email: emircem@uri.edu;emircem.gezer@gmail.com
    Year: 2022

    Copyright (C) 2022 Smart Ocean Systems Laboratory
*/

#pragma once

#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "std_srvs/Empty.h"
#include "visualization_msgs/MarkerArray.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "geometry_msgs/PolygonStamped.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/PointStamped.h"
#include "geometry_msgs/TransformStamped.h"
#include <mvp_msgs/ChangeState.h>

class USBLNode{

private:

    ros::NodeHandlePtr nh;

    ros::NodeHandlePtr pnh;

    ros::Publisher waypoint_publisher;

    ros::Publisher depth_publisher;

    ros::Subscriber usbl_subscriber;

    ros::ServiceClient survey_state_client;

    ros::ServiceClient controller_client;

    tf2_ros::Buffer tfBuffer;

    std::shared_ptr<tf2_ros::TransformListener> transform_listener;
    

    void f_usbl_callback(const visualization_msgs::MarkerArrayConstPtr& msg);

public:

    USBLNode();

};

