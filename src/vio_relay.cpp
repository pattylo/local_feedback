#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

using std::placeholders::_1;

class VrpnQosBridge : public rclcpp::Node
{
    public:
    VrpnQosBridge() : Node("vrpn_qos_bridge")
    {
        this->declare_parameter<std::string>("vrpn_topic", "/vrpn_mocap/grl_mod_1/pose");
        this->declare_parameter<std::string>("out_topic", "mavros/vision_pose/pose");
        this->declare_parameter<std::string>("out_frame", "vision");

        vrpn_topic_ = this->get_parameter("vrpn_topic").as_string();
        out_topic_  = this->get_parameter("out_topic").as_string();
        out_frame_  = this->get_parameter("out_frame").as_string();

        // pain in an ass qos here
        rclcpp::QoS sub_qos(rclcpp::KeepLast(10));
        sub_qos.reliability(rclcpp::ReliabilityPolicy::BestEffort);
        sub_qos.durability(rclcpp::DurabilityPolicy::Volatile);

        rclcpp::QoS pub_qos(rclcpp::KeepLast(10));
        pub_qos.reliability(rclcpp::ReliabilityPolicy::Reliable);
        pub_qos.durability(rclcpp::DurabilityPolicy::Volatile);

        pub_ = this->create_publisher<geometry_msgs::msg::PoseStamped>(out_topic_, pub_qos);
        sub_ = this->create_subscription<geometry_msgs::msg::PoseStamped>(
            vrpn_topic_,
            sub_qos,
            std::bind(&VrpnQosBridge::callback, this, _1)
        );

        RCLCPP_INFO(this->get_logger(), "Bridging %s -> %s", vrpn_topic_.c_str(), out_topic_.c_str());
        RCLCPP_INFO(this->get_logger(), "Sub QoS: RELIABLE/KEEP_LAST(10). Pub QoS: BEST_EFFORT/KEEP_LAST(10).");
    }

    private:
    void callback(const geometry_msgs::msg::PoseStamped::SharedPtr msg)
    {
        auto out = geometry_msgs::msg::PoseStamped();

        out.header = msg->header;

        if (out.header.stamp.sec == 0 && out.header.stamp.nanosec == 0)
            out.header.stamp = this->now();
        
        // enforce the desired frame id
        out.header.frame_id = out_frame_;
        out.pose = msg->pose;

        pub_->publish(out);
    }

    std::string vrpn_topic_;
    std::string out_topic_;
    std::string out_frame_;

    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr pub_;
    rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr sub_;

};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<VrpnQosBridge>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}