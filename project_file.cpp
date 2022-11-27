/*
Date: 22nd of November 2022

Authors: Adrien Lasserre (adrien.lasserre@ecam.fr) and Gwenn Durpoix-Espinasson (g.durpoix-espinasson@ecam.fr)

Context: This cpp file is part of the second lab of the Advanced_Robotics course at ECAM. This course is taught by Guillaume Gibert (guillaume.gibert@ecam.fr).
		The main goal of this file is to create a ROS node publishing on the /cmd_vel topic of a Turtlebot3 educational robot. It also subscribes to /scan to detect
		the obstacles on the way of the robot.
		
*/


#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>

//max values for lin vel 0.22 and angular vel 2.84

bool obstacle_present=false;

void obstacleCallback(const sensor_msgs::LaserScan& message){
	
	float thresh_min=0.12;
	float thresh_max=0.25;
	
	// this for-loop constrains the values of the scan using tresh_min and tresh_max
	for(int count = 0; count < 15; count++){
		
		if( (message.ranges[count] >=thresh_min && message.ranges[count]<= thresh_max) || (message.ranges[359-count] >=thresh_min && message.ranges[359-count]<= thresh_max)){
			
			obstacle_present=true;
			break;
		}
		else{
			obstacle_present=false;
		}
		
	}
	
	
}

	
void walk(geometry_msgs::Twist& twist, float speed_input){

    //intialisation of the values for the twist messages
    twist.linear.x=0.0;
    twist.linear.y=0.0;
    twist.linear.z=0.0;
    twist.angular.x=0.0;
    twist.angular.y=0.0;
    twist.angular.z=0.0;

    //use of the speed input
    twist.linear.x=speed_input;

}

void turn_random(geometry_msgs::Twist& twist, float random_angular_speed){

    //initialisation of the values for the twist messages
    twist.linear.x=0.0;
    twist.linear.y=0.0;
    twist.linear.z=0.0;
    twist.angular.x=0.0;
    twist.angular.y=0.0;
    twist.angular.z=0.0;

    //randomise the angle
    twist.angular.z=random_angular_speed;

}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "commander");
	ros::NodeHandle nh;
	ros::Publisher commanderPublisher =
		nh.advertise<geometry_msgs::Twist>("cmd_vel", 1);
	ros::Subscriber commanderSubscriber =
		nh.subscribe("scan", 1, obstacleCallback);
	ros::Rate loopRate(5);
	
	while(ros::ok()){

                geometry_msgs::Twist twist;
		
		if(obstacle_present==true){
			turn_random(twist, 2);
		}
		else{
			walk(twist, 0.1);
		}
		
                commanderPublisher.publish(twist);
		ros::spinOnce();
		loopRate.sleep();
	}
	
	return 0;
}
