from setuptools import find_packages, setup

package_name = "action_server_py"

setup(
    name=package_name,
    version="0.0.0",
    packages=find_packages(exclude=["test"]),
    data_files=[
        ("share/ament_index/resource_index/packages", ["resource/" + package_name]),
        ("share/" + package_name, ["package.xml"]),
    ],
    install_requires=["setuptools"],
    zip_safe=True,
    maintainer="root",
    maintainer_email="andrei.baciu94@gmail.com",
    description="Python action server for demonstrating simple ROS 2 actions.",
    license="BSD-3-Clause",
    extras_require={
        "test": [
            "pytest",
        ],
    },
    entry_points={
        "console_scripts": [
            "add_until_server = action_server_py.add_until_server:main"
        ],
    },
)
