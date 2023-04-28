# ALPHA Standard AUV Stonefish Setup Guide

## Introduction
This document describes how each sensor/actuator on the ALPHA is configured on the actual vehicle and in the stonefish simulator. Herein, we use the standard ALPHA vehicle (`alpha_std_auv`) as an example. The standard vehicle has an IMU, DVL, and four thrusters (two vertical thrusters, one horizont thruster, and one main thruster in the back).
Without following the instruction, you may encounter weird performanace when simulating the ALPHA, especailly when using the MVP control, MVP mission and robot localization with GPS integrated.
**Caution**
- DVL configuration in the Stonefish maybe changed to match the physical system.
- IMU transform node is not updated in the actual repository. Please check the pull request to see the changes. We will try to merge the changes asap.

## Common Frames.
### ***World related frames***
- **NED**: North-East-Down is widely used for marine vehicles. `Stonefish` simulator uses the **NED** frame. As a result, the odometry sensor in the Stonefish are generated based on the **NED** frame. 
- **world_ned**: we can define this as a fixed world frame based on the NED convention. The origin of world_ned can have a offset from **NED** in the x-y plane
- **ENU**: East-North-Up frame is the convention in ROS. For example, robot localization package accepts data in ENU and outputs odometry data in ENU frame. **Caution**: When an AHRS produces data in ENU frame, 1) the heading will be relative to the east and 2) it will increase when rotating it counter-clockwise when looking from above. We need to make sure both conditions are meet before importing the data into any localization packages in ROS.
- **world**: we can define this as a fixed world frame based on the ENU convention.

### ***Robot related frames***. We suggest to have namespaces for each robot related frames.
- **odom**: is the frame the robot's odometry based on. It is normally defined when a robot is spawn. In ROS odom should follow the ENU convention. Thus, it can be aligned with the `world_enu` or have a translation from the `world_enu`.
- **base_link**: is a link attacehd to the vehicle. The odometry message indicates the translation and rotation between the `baselink` and the `odom` frame.
-  **cg_link**: is a link we defined located at the center of gravity of the vehicle. This link follows the convention SNAME convention. Therefore, x is pointed forward, y is pointed starboard, and z is pointed downward. The `cg_link` and `base_link` are 180 deg apart in roll. Our `MVP_control`, by default, use the `cg_link`. If the node subscribed to a odometry in enu, the software will transform it into the `cg_link`.
- **imu**: is defined based on the imu orientation and position on the vehicle. It is axis should be aligned with the x-y-z arrows displayed on the sensor. **Caution** if the IMU you are using is providing orientation estimate (known as AHRS), you need to convert the orientation measurements in to ENU since ROS, by default, thinks the orientation data in an IMU message is in the ENU frame. See ***Sensor setup in the Stonefish*** section to find out how we change the orientations.
- **dvl**: is defined based on the location of the dvl. In an actual DVL, the sensor will indicate the x-y-z axis. Normally, the z-axis is pointed towards the seabed, x-axis is pointed forward, and y-axis is pointed to the starboard. **Caution**: in the `Stonefish`, we found the DVL has to point upward in order to get a valid measurement on the seafloor. ***Sensor setup in the Stonefish*** section to find out how we deal with that. 
- **gps**: is defined based on the gps location and oriention on the vehicle. 
- **pressure_sensor**: it has the same orientation as the `cg_link` but offseted based on the location of the pressure sensor. When the vehicle is going down, we expect a positive pressure/depth reading.

### ***Sensor setup in the Stonefish*** ###
- Stonefish uses NED convention. Therefore, in the UI you will see the earth frame with x(red) pointed north, y(green) pointed east,a nd z(blue) pointed downward. All the following content only make suggestsion on how to make the scnario files used in the Stonefish

- **baselink**: when you  toggle-on the "frames" in the Stonefish UI, it shows a "base_link" this coordinate is actually not the base-link. Instead, it is the center of the graveity of the compond vehicle you imported. **TO MAKE YOUR LIFE EASIER**, we suggest to create a link in the another link called `Base` to be your reference point. See the code below. This frame will be aligned with NED at the very beginning. Then, you can define all other frames based on `Base`.

```
<!-- the actual base frame  -->
           <link name="Base" type="box" physics="submerged">
               <dimensions xyz="0.01 0.02 0.01"/>
               <origin xyz="0.0 0.0 0.0" rpy="0.0 0.0 0.0"/>
               <material name="Neutral"/>
               <look name="Green"/>
           </link>
```
For your vehicle, we still define it in the `Vehicle` frame. Therefore, we need a joint to link our CAD models and the `Base`. And in our case we define the translation and rotation to be zero. Then when we visualize it, it is coincident with our baselink. Then when we visualize it, it is coincident with our baselink. In real world application, this “Base” should be attached to a meaningful location. For example where the IMU is. So in the TF tree, the baselink and the imu_link are algined.

```
 <joint name="Joint1" type="fixed">
               <parent name="Vehicle"/>
               <child name="Base"/>
               <origin xyz="0.0 0.0 0.0" rpy="0.0 0.0 0.0"/>
               <axis xyz="1.0 0.0 0.0"/>
           </joint>
```

- **imu_sf**: is a frame attached to the imu in the stonefish. On the ALPHA standard vehicle, the imu's z-axis is pointed upward, x-axis is pointed forward, and y-axis is pointed port. Therefore, in the Stonefish, we have `imu_sf` rotated 180deg around x from the `Base` as shown below.readings from the imu.
```
<sensor name="imu_sf" type="imu" rate="20.0">
               <link name="Base"/>
               <origin rpy="3.1415926 0.0 0.0" xyz="0.0 0.0 -0.05"/>
               <noise angle="0.000001745" angular_velocity="0.00001745"/>
               <ros_publisher topic="/${robot_name}/imu/stonefish/data"/>
            </sensor>
```

 We named the link with suffix becasuse Stonefish's IMU's orientation is measured in NED frame. Before, we use it for localization, we need use the [imu_transformer node](https://github.com/uri-ocean-robotics/alpha_core/blob/noetic-devel/alpha_localization/src/imu_ned_enu.cpp) to adjust the orientation. The code below showing how to use the node in the launch file.
 ```
 <node ns="$(arg robot_name)" name="imu_ned_to_enu" pkg="alpha_localization" type="imu_ned_to_enu" output="screen">
            <remap from="imu_in/data" to="imu/stonefish/data"/>
            <remap from="imu_out/data" to="imu/data"/>
            <param name="frame_id" value="$(arg robot_name)/imu"/>
        </node>
 ```
- **dvl_sf**: as mentioned earlier, the DVL frame is slightly different than the real system. In the Stonefish, you need to point the z-axis of the DVL upward to get valid bottom lock from the seabed. Therefore, we rolled the DVl axis 180 deg as shown below. And we have suffix to indicate the frame is from the stonefish.
```
 <sensor name="dvl_sf" type="dvl" rate="5.0">
                <link name="Base"/>
                <origin rpy="3.1415926 0.0 0.0" xyz="-0.65 0.0 0.1"/>
                <specs beam_angle="30.0"/>
                <range velocity="9.0 9.0 9.0" altitude_min="0.2" altitude_max="200.0"/>
                <noise velocity="0.015" altitude="0.001"/>
                <ros_publisher topic="/${robot_name}/dvl/twist" altitude_topic="/${robot_name}/dvl/altitude"/>
            </sensor>
```
In the URDF file, we need to redefine an actual dvl frame. So on the real vehicle, the data will be published in the dvl frame and we don't need to change our TF tree. You can do it using the following example where we rotated two frames with 180 degrees in roll.
```
    <link name="dvl"/>
    <joint name="dvl_joint" type="fixed">
        <origin xyz="-0.22 0.0 -0.08" rpy="3.1415926 0.0 0.0"/>
        <parent link="base_link"/>
        <child link="dvl"/>
    </joint>
     <link name="dvl_sf"/>
    <joint name="dvl_sf_joint" type="fixed">
        <origin xyz="-0.22 0.0 -0.08" rpy="0.0 0.0 0.0"/>
        <parent link="base_link"/>
        <child link="dvl_sf"/>
```

- **Thruster frames**: for the thrusters, we need to follow the followin rules to make MVP_control happy. First, your thruster accepts commands between -1 to 1. Second, the positive command will create a positive thrust in the thruster's x axis. Then, you can define the TF between your thruster and the vehicle based on the `Base` frame. The following example shows how we defined the vertical thruster near the bow. The thruster frame is pitched -90 degree from our `Base`. Thus, its x-axis is pointed downward.
```
 <actuator name="ThrusterHeaveBow" type="thruster">
                <link name="Base"/>
                <origin rpy="0.0 -1.571 0.0" xyz="-0.2857 0.0 0.0"/>
                <specs thrust_coeff="0.4315" thrust_coeff_backward="0.3404" torque_coeff="0.01" max_rpm="3000.0"/>
                <propeller diameter="0.08" right="true">
                    <mesh filename="prop.obj" scale="1.0"/>
                    <material name="Neutral"/>
                    <look name="white"/>
                </propeller>
                <ros_subscriber topic="/${robot_name}/control/thruster/heave_bow"/>
            </actuator>
```

- ***Summary***
A full example of our Stonefish scenario file setup can be found in this repository [here](https://github.com/GSO-soslab/alpha_std_auv/blob/noetic-devel/alpha_std_stonefish/description/alpha_std.scn.xacro)
And the URDF description is [here](https://github.com/GSO-soslab/alpha_std_auv/blob/noetic-devel/alpha_std_description/urdf/base.urdf.xacro)
