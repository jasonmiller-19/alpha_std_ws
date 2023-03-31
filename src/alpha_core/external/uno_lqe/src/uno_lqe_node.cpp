#include "uno_lqe_ros.h"


int main(int argc, char* argv[]) {

    ros::init(argc, argv, "leq_node",ros::init_options::AnonymousName);

    UnoLqeNode node;

    ros::spin();

    return 0;
}
