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

    Author: Jason Miller
    Email: jason_miller@uri.edu
    Year: 2023

    Copyright (C) 2023 Smart Ocean Systems Laboratory
*/

#include "usbl_node.hpp"

USBLNode::USBLNode() {
    nh.reset(new ros::NodeHandle(""));

    usbl_subscriber = nh->subscribe("/follower/usbl", 10, &USBLNode::f_usbl_callback, this);
    waypoint_publisher = nh->advertise<geometry_msgs::PolygonStamped>("/follower/helm/path_local/update_waypoints", 10);
    depth_publisher = nh->advertise<std_msgs::Float64>("/follower/helm/depth_tracking/desired_depth", 10);
    survey_state_client = nh->serviceClient<mvp_msgs::ChangeState>("/follower/helm/change_state");
    controller_client = nh->serviceClient<std_srvs::Empty>("/follower/controller/enable");
    
    transform_listener.reset(new tf2_ros::TransformListener(tfBuffer));

    std_srvs::Empty enable;
    
    ros::service::waitForService("/follower/controller/enable", ros::Duration(5));

    controller_client.call(enable);

}

void USBLNode::f_usbl_callback(const visualization_msgs::MarkerArrayConstPtr &msg) {

    geometry_msgs::PolygonStamped waypoint;
    geometry_msgs::PointStamped tf_usbl_depth;
    geometry_msgs::Point32 p;
    std_msgs::Float64 depth_cmd;
    mvp_msgs::ChangeState srv;
    

    //Call the helm behavior survey_local (could be survey_global if the waypoint publisher is updated to path_global)
    srv.request.state = "survey_local";
    survey_state_client.call(srv);

    //Format USBL data in waypoint type format
    waypoint.header.frame_id = "follower/usbl";
    p.x = msg->markers[0].pose.position.x;
    p.y = msg->markers[0].pose.position.y;
    waypoint.polygon.points.emplace_back(p);
    
    //TF the usbl depth to world_ned frame
    tf_usbl_depth.header.frame_id = "follower/usbl";
    tf_usbl_depth.point.z = (msg->markers[0].pose.position.z);
    auto t = tfBuffer.transform(tf_usbl_depth, "follower/world_ned", ros::Duration(1.0));

    depth_cmd.data = static_cast<float>(t.point.z);

    //Publish X&Y point to waypoint_pub and Z to depth_pub
    waypoint_publisher.publish(waypoint);
    depth_publisher.publish(depth_cmd);
}


int main(int argc, char* argv[]) {

    ros::init(argc, argv, "usbl");

    USBLNode d;

    ros::spin();

    return 0;
}