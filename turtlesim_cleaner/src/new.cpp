#include <bits/stdc++.h>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
#include <turtlesim/Spawn.h>
#include <turtlesim/Kill.h>

using namespace std;

struct point{
    float x;
    float y;
};

using namespace std;

ros::Publisher vel_publisher;
ros::Subscriber pose_subscriber;

const double PI=3.14159265359;

void move(double speed,double distance );
void rotate(double angular_speed,float angle,bool clockwise);
double degrees2radians(double angle_in_degrees);

turtlesim::Pose pose;



void poseCallback(const turtlesim::Pose::ConstPtr& message){
    pose.x = message->x;
    pose.y = message->y;
    pose.theta = message->theta;
}

int main(int argc, char** argv){
    
    ros::init(argc, argv, "movealongpath");
    ros::NodeHandle n;
    ros::Publisher vel_publisher = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);
    ros::Subscriber pose_subscriber =  n.subscribe("/turtle1/pose", 10, &poseCallback);
    vector<point> vec;
   
    ifstream myfile;
    myfile.open("/home/debajit/ip/save.txt",std::ios::app);
    float p;
    if(myfile.is_open()){
        while(myfile>>p){
            //cout<<1<<endl;
            point chk;
            chk.x = p*11/610.0;
            myfile>>p;
            chk.y = p*11/610.0;
            //cout<<tmp.x<<" "<<tmp.y<<endl;
            vec.push_back(chk);
        }
    }
    myfile.close();
    int j=vec.size()-1, i=0;

    ros::ServiceClient clientkill = n.serviceClient<turtlesim::Kill>("kill");
    turtlesim::Kill::Request reqkill;
    turtlesim::Kill::Response reskill;
    reqkill.name = "turtle1";
    clientkill.call(reqkill, reskill);

    ros::ServiceClient clientspawn = n.serviceClient<turtlesim::Spawn>("spawn");
    turtlesim::Spawn::Request reqspawn;
    turtlesim::Spawn::Response resspawn;
    reqspawn.x = 0;
    reqspawn.y = 0;
    reqspawn.theta = 0;
    reqspawn.name = "turtle1";

    clientspawn.call(reqspawn, resspawn);
    
    ros::Rate loop_rate(5);

    while(ros::ok())
    {
        geometry_msgs::Twist message;
        while(j>0)
        {
            float theta = (vec[j].x-pose.x != 0)? atan((float)(vec[j].y-pose.y)/(float)(vec[j].x-pose.x)) : 1.57;
            if (theta<0 && pose.theta>0 && (pose.theta-theta)>=1.57)
            {
                while(abs(3.14+theta-pose.theta)>0.001){
                    std::cout<<"1"<<endl;
                    message.linear.x = 0;
                    message.angular.z = 4*(3.14+theta-pose.theta);
                    vel_publisher.publish(message);
                    std::cout<<message.angular.z<<"\n";
                    ros::spinOnce(); 
                   }
            }
            else{
                while(abs(theta-pose.theta)>0.001){
                    //std::cout<<"1"<<endl;
                    message.linear.x = 0;
                    message.angular.z = 4*(theta-pose.theta);
                    vel_publisher.publish(message);
                    std::cout<<message.angular.z<<"\n";
                    ros::spinOnce(); 
                   }
            }
            while(sqrt((vec[j].x-pose.x)*(vec[j].x-pose.x)+(vec[j].y-pose.y)*(vec[j].y-pose.y))>=0.1){
                message.linear.x = 4*sqrt((vec[j].x-pose.x)*(vec[j].x-pose.x)+(vec[j].y-pose.y)*(vec[j].y-pose.y));
                std::cout<<"2"<<endl;
                message.angular.z = 0;
                vel_publisher.publish(message);
                std::cout<<(vec[j].x-pose.x)*(vec[j].x-pose.x)+(vec[j].y-pose.y)*(vec[j].y-pose.y)<<"\n";
                ros::spinOnce();
            }
            j--;
        }   
        message.linear.x = 0;
        message.angular.z = 0;
        vel_publisher.publish(message);
        ros::spinOnce();
    }
}

/*void moveGoal(turtlesim::Pose goal_pose,double distance_tolerance){
    geometry_msgs::Twist vel_msg;

    ros::Rate loop_rate(10);

    do{
        cout<<getDistance(turtlesim_pose.x,turtlesim_pose.y,goal_pose.x,goal_pose.y)<<endl;
        vel_msg.linear.x=1.5*getDistance(turtlesim_pose.x,turtlesim_pose.y,goal_pose.x,goal_pose.y);
        vel_msg.linear.y=0;
        vel_msg.linear.z=0;

        vel_msg.angular.x=0;
        vel_msg.angular.y=0;
        vel_msg.angular.z=4*(atan2(goal_pose.y-turtlesim_pose.y,goal_pose.x-turtlesim_pose.x));

        velocity_publisher.publish(vel_msg);

        loop_rate.sleep();
        ros::spinOnce();

    }while(getDistance(turtlesim_pose.x,turtlesim_pose.y,goal_pose.x,goal_pose.y)>distance_tolerance);

    cout<<"end move goal"<<endl;
    vel_msg.linear.x=0;
    vel_msg.angular.z=0;
    velocity_publisher.publish(vel_msg);

}


/*void move(double speed , double distance )
{
    geometry_msgs::Twist vel_msg;
    
    vel_msg.linear.x=abs(speed);
    vel_msg.linear.y=0;
    vel_msg.linear.z=0;

    vel_msg.angular.x=0;
    vel_msg.angular.y=0;
    vel_msg.angular.z=0;

    double t0=ros::Time::now().toSec();
    double current_distance=0.0;
    ros::Rate loop_rate(100);
    int i=0;
    do{
        velocity_publisher.publish(vel_msg);
        double t1=ros::Time::now().toSec();
        current_distance=speed*(t1-t0);
        //speed=
        ros::spinOnce();
        loop_rate.sleep();
    }while(current_distance<distance);
    vel_msg.linear.x=0;
    velocity_publisher.publish(vel_msg);
    
} 

void rotate(double angular_speed,float relative_angle,bool clockwise)
{
    geometry_msgs::Twist vel_msg;

   
    if(clockwise)
        vel_msg.angular.z=-abs(angular_speed);
    else
        vel_msg.angular.z=abs(angular_speed);

    vel_msg.linear.x=0;
    vel_msg.linear.y=0;
    vel_msg.linear.z=0;
    vel_msg.angular.x=0;
    vel_msg.angular.y=0;




    double current_angle=0.0;
    double t0=ros::Time::now().toSec();
    do{
        velocity_publisher.publish(vel_msg);
        double t1=ros::Time::now().toSec();
        current_angle=angular_speed*(t1-t0);
        //ros::spinOnce();
        //loop_rate.sleep();

    }while(current_angle<relative_angle);

    vel_msg.angular.z=0;
    velocity_publisher.publish(vel_msg);

}

double degrees2radians(double angle_in_degrees){
    return angle_in_degrees*PI/180.0;
}

/*double setDesiredOrientation(double desired_angle_radians){
    double relative_angle_radians =desired_angle_radians -turtlesim_pose.theta;
    bool clockwise = ((relative_angle_radians<0)?true:false);
    rotate(abs(relative_angle_radians)/4,abs(relative_angle_radians),clockwise);
}*/


