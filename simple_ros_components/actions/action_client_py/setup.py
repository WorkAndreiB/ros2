from setuptools import find_packages, setup

package_name = "action_client_py"

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
    description="Python action client used to test actions functionalities",
    license="BSD-3-Clause",
    extras_require={
        "test": [
            "pytest",
        ],
    },
    entry_points={
        "console_scripts": [
            "add_until_client = action_client_py.add_until_client:main"
        ],
    },
)
