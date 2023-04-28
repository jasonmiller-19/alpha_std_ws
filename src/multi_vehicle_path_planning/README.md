# ALPHA Standard AUV.

## Introduction
This repository includes configuration for standard ALPHA AUV.
- This repository is built based on our previous vehicle repositores, `alpha_sci_auv` and `alpha_auv`. The major change is that we have gone through the TF setup for the vehicle to make urdf and stonefish are consistent and representing the same frames in the correct orientation.
- The readme content is an updated version of the previous readme [here](https://github.com/uri-ocean-robotics/alpha_sci_auv), created by Emir Cem Gezer.
- The standard ALPHA AUV uses AHRS/IMU, DVL, GPS to localize itself.
- ALPHA uses our [mvp_control](https://github.com/uri-ocean-robotics/mvp_control) and [mvp_mission](https://github.com/uri-ocean-robotics/mvp_mission) for low-level pose control and vehicle guidance.
- The localization configuration we have here runs a single `robot_localization` stack. We used `navsat_transform` node to anchor `odom` to a specific gps coordination. Then, the `robot_localzation` fuses the IMU, DVL, GPS odometry(output from the `navsat_transform` node) for localzation. When the AUV is resurfaced, sudden position jump is visible, similar to a real mission.
- Currently, we only have simulation configured. Later, the physical system configuration will be included and the standard vehicle could be launch using another launch file.
- A TF configuration setup guide is currently in preparation, and will be made available here soon.
- This repository is actively maintained by the Smart Ocean Systems Laboratory, GSO, URI.


- ROS version Noetic,
- Ubuntu 20.04


## Directory structure
- `alpha_std_auv`: Meta package for the standard ALPHA AUV.

- `alpha_std_bringup`: Launch files to bring the vehicle/simulation up runnning

- `alpha_std_config`: Configuration files for helm, controller, and devices. `mvp_mission` state machine is configured in `/mission/config/helm.yaml`. Parameters for different behaviors program for the helm is located in `/mission/param`. `mvp_control` configuration is under `/config/control.yaml`.

- `alpha_std_description`: URDF files, rviz configuration, and vehicle mesh

- `alpha_std_stonefish`: ALPHA scenario files for the `Stonefish` simulator.


## Instllation

### Install the Stonefish simulator
- We use [Stonefish](https://stonefish.readthedocs.io/en/latest/install.html) Simulator. You can clone it from [here](https://github.com/uri-ocean-robotics/stonefish), a fork from the [original_repo](https://github.com/patrykcieslak/stonefish).

- Download the stonefish simulator **to another location outside your ROS workspace**
```bash
git clone https://github.com/uri-ocean-robotics/stonefish
```

- Install dependencies using `sudo apt install` (instruction from the [Stonefish](https://github.com/patrykcieslak/stonefish))
    * **OpenGL Mathematics library** (libglm-dev, version >= 0.9.9.0)
    * **SDL2 library** (libsdl2-dev, may need the following fix!)
        1. Install SDL2 library from the repository.
        2. `cd /usr/lib/x86_64-linux-gnu/cmake/SDL2/`
        3. `sudo vim sdl2-config.cmake`
        4. Remove space after "-lSDL2".
        5. Save file.
    * **Freetype library** (libfreetype6-dev)

- Build and install the stonefish
    ```bash
    cd stonefish
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make -j$(nproc)
    sudo make install
    ```


### Setup ALPHA Standard Repo
- Clone `alpha_std_auv` repo
    ```bash
    git clone https://github.com/GSO-soslab/alpha_std_auv
    cd alpha_auv
    git submodule update --init --recursive
    ```

- Install pip and setup python3 as default
    ```bash
    sudo apt install python3-pip
    ```

- Clone [alpha_core] repo which include other hardware related source code, sensor drivers, and other utilities.

    ```bash
    git clone --single-branch --branch noetic-devel https://github.com/uri-ocean-robotics/alpha_core.git
    ```

### Install ROS-MVP 
Currently MVP packages should be build from the source.
Target platform must be Ubuntu 20.04 because of the dependencies.

Pull repository and other dependencies
```bash
git clone --single-branch --branch noetic-devel https://github.com/uri-ocean-robotics/mvp_msgs
git clone --single-branch --branch noetic-devel https://github.com/uri-ocean-robotics/mvp_control
git clone --single-branch --branch noetic-devel https://github.com/uri-ocean-robotics/mvp_mission
git clone --single-branch --branch noetic-devel https://github.com/uri-ocean-robotics/stonefish_mvp
```
**stonefish_mvp** is a wrapper modified from [stonefish_ros](https://github.com/patrykcieslak/stonefish_ros) for ROS interface with ROS-MVP.

### Install Dependencies

Install dependencies
```bash
rosdep install --from-paths src --ignore-src --rosdistro ${ROS_DISTRO} -y
```

### Compile the code
go back to the ROS Workspace dir (e.g., catkin_ws), then do
```bash
catkin_make
```

## Quick test
- Bring up the ALPHA Standard AUV with the Stonefish simualtor.

```bash
roslaunch alpha_std_bringup bringup_simulation.launch
```

- Enable the controller in a separated terminal
```bash
rosservice call /alpha_std/controller/enable
```

- Start a path following mission in local frame where your waypoint is defined in `alpha_std_config/mission/param/path_local.yaml`

```bash
rosservice call /alpha_std/helm/change_state "state: 'survey_local'"
```

- OR, try path following in a global frame where your waypoint is defined in latitude and longitude in `alpha_std_config/mission/param/gps_wpt.yaml`

```bash
rosservice call /alpha_std/helm/change_state "state: 'survey_global'"
```

- You can put AUV in idle anytime by changing the state of the helm

```bash
rosservice call /alpha_std/helm/change_state "state: 'start'"
```

- Note: Make sure you selected the correct topics for the Markers in the RViz window.


## Citation

The ALPHA paper:

```
@inproceedings{
    ALPHA_PAPER,
    title = {Acrobatic Low-cost Portable Hybrid AUV (ALPHA): System Design and Preliminary Results},
    author={Zhou, Mingxi and Gezer, Emir Cem and McConnell, William and Yuan, Chengzhi},
    booktitle={OCEANS 2022: Hampton Roads},
    year={2022},
    organization={IEEE}
}
```

The MVP paper:

```
@inproceedings{
    ALPHA_PAPER,
    title = {Working toward the development of a generic marine vehicle framework: ROS-MVP},
    author={Gezer, Emir Cem and Zhou, Mingxi and Zhao, LIN and McConnell, William},
    booktitle={OCEANS 2022: Hampton Roads},
    year={2022},
    organization={IEEE}
}
```



## Funding
This work is supported by the [National Science Foundation](https://www.nsf.gov/) award [#2154901](https://www.nsf.gov/awardsearch/showAward?AWD_ID=2154901&HistoricalAwards=false) and award [#2221676](https://www.nsf.gov/awardsearch/showAward?AWD_ID=2221676&HistoricalAwards=false)