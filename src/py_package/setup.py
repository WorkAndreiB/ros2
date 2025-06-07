from setuptools import find_packages, setup

package_name = 'py_package'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='root',
    maintainer_email='andrei.baciu94@gmail.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            "node = py_package.node:main",
            "robot_news_station = py_package.robot_news_station:main",
            "robot_news_station_listener = py_package.robot_news_station_listener:main"
        ],
    },
)
