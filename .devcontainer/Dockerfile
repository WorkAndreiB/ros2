# Base image
FROM ubuntu:24.04

# Set working directory
WORKDIR /ros2

# Install system dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    make \
    perl \
    curl \
    python3-dev \
    python3-venv \
    libpq-dev \
    python3-pip \
    git \ 
    terminator \
    && apt-get clean

# Install ROS2 dependencies
RUN apt install -y software-properties-common && \
    add-apt-repository universe && \
    export ROS_APT_SOURCE_VERSION=$(curl -s https://api.github.com/repos/ros-infrastructure/ros-apt-source/releases/latest | grep -F "tag_name" | awk -F\" '{print $4}') && \
    curl -L -o /tmp/ros2-apt-source.deb "https://github.com/ros-infrastructure/ros-apt-source/releases/download/${ROS_APT_SOURCE_VERSION}/ros2-apt-source_${ROS_APT_SOURCE_VERSION}.$(. /etc/os-release && echo $VERSION_CODENAME)_all.deb" && \
    apt install /tmp/ros2-apt-source.deb

# Install ROS2 dev tools
RUN apt-get update && apt-get install -y ros-dev-tools

# Install ROS2 
RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y ros-jazzy-desktop

# Install robotics dependecies
RUN apt-get update && apt-get upgrade -y && apt-get install -y \
    ros-jazzy-urdf-tutorial \
    #tf tree
    ros-jazzy-tf2-tools \
    ros-jazzy-xacro \
    #gazebo dependencies
    ros-jazzy-ros-gz \
    ros-jazzy-moveit

# Install ros2 controllers
RUN apt-get update && apt-get upgrade -y && apt-get install -y \
    ros-jazzy-ros2-control \
    ros-jazzy-ros2-controllers

# Source ROS2 workspace
RUN echo "source /opt/ros/jazzy/setup.bash" >> ~/.bashrc

# Create and activate virtual environment
RUN python3 -m venv /opt/venv

# Make sure the venv pip is up-to-date
RUN /opt/venv/bin/pip install --upgrade pip

# Add virtual environment to PATH
ENV PATH="/opt/venv/bin:$PATH"

# Copy and install Python dependencies
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Environment settings
ENV PYTHONUNBUFFERED=1

# Default command
CMD ["bash"]

WORKDIR /workspace
