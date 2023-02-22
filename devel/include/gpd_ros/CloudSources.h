// Generated by gencpp from file gpd_ros/CloudSources.msg
// DO NOT EDIT!


#ifndef GPD_ROS_MESSAGE_CLOUDSOURCES_H
#define GPD_ROS_MESSAGE_CLOUDSOURCES_H


#include <string>
#include <vector>
#include <memory>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <sensor_msgs/PointCloud2.h>
#include <std_msgs/Int64.h>
#include <geometry_msgs/Point.h>

namespace gpd_ros
{
template <class ContainerAllocator>
struct CloudSources_
{
  typedef CloudSources_<ContainerAllocator> Type;

  CloudSources_()
    : cloud()
    , camera_source()
    , view_points()  {
    }
  CloudSources_(const ContainerAllocator& _alloc)
    : cloud(_alloc)
    , camera_source(_alloc)
    , view_points(_alloc)  {
  (void)_alloc;
    }



   typedef  ::sensor_msgs::PointCloud2_<ContainerAllocator>  _cloud_type;
  _cloud_type cloud;

   typedef std::vector< ::std_msgs::Int64_<ContainerAllocator> , typename std::allocator_traits<ContainerAllocator>::template rebind_alloc< ::std_msgs::Int64_<ContainerAllocator> >> _camera_source_type;
  _camera_source_type camera_source;

   typedef std::vector< ::geometry_msgs::Point_<ContainerAllocator> , typename std::allocator_traits<ContainerAllocator>::template rebind_alloc< ::geometry_msgs::Point_<ContainerAllocator> >> _view_points_type;
  _view_points_type view_points;





  typedef boost::shared_ptr< ::gpd_ros::CloudSources_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::gpd_ros::CloudSources_<ContainerAllocator> const> ConstPtr;

}; // struct CloudSources_

typedef ::gpd_ros::CloudSources_<std::allocator<void> > CloudSources;

typedef boost::shared_ptr< ::gpd_ros::CloudSources > CloudSourcesPtr;
typedef boost::shared_ptr< ::gpd_ros::CloudSources const> CloudSourcesConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::gpd_ros::CloudSources_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::gpd_ros::CloudSources_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::gpd_ros::CloudSources_<ContainerAllocator1> & lhs, const ::gpd_ros::CloudSources_<ContainerAllocator2> & rhs)
{
  return lhs.cloud == rhs.cloud &&
    lhs.camera_source == rhs.camera_source &&
    lhs.view_points == rhs.view_points;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::gpd_ros::CloudSources_<ContainerAllocator1> & lhs, const ::gpd_ros::CloudSources_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace gpd_ros

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsMessage< ::gpd_ros::CloudSources_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::gpd_ros::CloudSources_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::gpd_ros::CloudSources_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::gpd_ros::CloudSources_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::gpd_ros::CloudSources_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::gpd_ros::CloudSources_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::gpd_ros::CloudSources_<ContainerAllocator> >
{
  static const char* value()
  {
    return "db42f0bd3c98d8b681c7942579de88bd";
  }

  static const char* value(const ::gpd_ros::CloudSources_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xdb42f0bd3c98d8b6ULL;
  static const uint64_t static_value2 = 0x81c7942579de88bdULL;
};

template<class ContainerAllocator>
struct DataType< ::gpd_ros::CloudSources_<ContainerAllocator> >
{
  static const char* value()
  {
    return "gpd_ros/CloudSources";
  }

  static const char* value(const ::gpd_ros::CloudSources_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::gpd_ros::CloudSources_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# This message holds a point cloud that can be a combination of point clouds \n"
"# from different camera sources (at least one). For each point in the cloud, \n"
"# this message also stores the index of the camera that produced the point.\n"
"\n"
"# The point cloud.\n"
"sensor_msgs/PointCloud2 cloud\n"
"\n"
"# For each point in the cloud, the index of the camera that acquired the point.\n"
"std_msgs/Int64[] camera_source\n"
"\n"
"# A list of camera positions at which the point cloud was acquired.\n"
"geometry_msgs/Point[] view_points\n"
"================================================================================\n"
"MSG: sensor_msgs/PointCloud2\n"
"# This message holds a collection of N-dimensional points, which may\n"
"# contain additional information such as normals, intensity, etc. The\n"
"# point data is stored as a binary blob, its layout described by the\n"
"# contents of the \"fields\" array.\n"
"\n"
"# The point cloud data may be organized 2d (image-like) or 1d\n"
"# (unordered). Point clouds organized as 2d images may be produced by\n"
"# camera depth sensors such as stereo or time-of-flight.\n"
"\n"
"# Time of sensor data acquisition, and the coordinate frame ID (for 3d\n"
"# points).\n"
"Header header\n"
"\n"
"# 2D structure of the point cloud. If the cloud is unordered, height is\n"
"# 1 and width is the length of the point cloud.\n"
"uint32 height\n"
"uint32 width\n"
"\n"
"# Describes the channels and their layout in the binary data blob.\n"
"PointField[] fields\n"
"\n"
"bool    is_bigendian # Is this data bigendian?\n"
"uint32  point_step   # Length of a point in bytes\n"
"uint32  row_step     # Length of a row in bytes\n"
"uint8[] data         # Actual point data, size is (row_step*height)\n"
"\n"
"bool is_dense        # True if there are no invalid points\n"
"\n"
"================================================================================\n"
"MSG: std_msgs/Header\n"
"# Standard metadata for higher-level stamped data types.\n"
"# This is generally used to communicate timestamped data \n"
"# in a particular coordinate frame.\n"
"# \n"
"# sequence ID: consecutively increasing ID \n"
"uint32 seq\n"
"#Two-integer timestamp that is expressed as:\n"
"# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n"
"# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n"
"# time-handling sugar is provided by the client library\n"
"time stamp\n"
"#Frame this data is associated with\n"
"string frame_id\n"
"\n"
"================================================================================\n"
"MSG: sensor_msgs/PointField\n"
"# This message holds the description of one point entry in the\n"
"# PointCloud2 message format.\n"
"uint8 INT8    = 1\n"
"uint8 UINT8   = 2\n"
"uint8 INT16   = 3\n"
"uint8 UINT16  = 4\n"
"uint8 INT32   = 5\n"
"uint8 UINT32  = 6\n"
"uint8 FLOAT32 = 7\n"
"uint8 FLOAT64 = 8\n"
"\n"
"string name      # Name of field\n"
"uint32 offset    # Offset from start of point struct\n"
"uint8  datatype  # Datatype enumeration, see above\n"
"uint32 count     # How many elements in the field\n"
"\n"
"================================================================================\n"
"MSG: std_msgs/Int64\n"
"int64 data\n"
"================================================================================\n"
"MSG: geometry_msgs/Point\n"
"# This contains the position of a point in free space\n"
"float64 x\n"
"float64 y\n"
"float64 z\n"
;
  }

  static const char* value(const ::gpd_ros::CloudSources_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::gpd_ros::CloudSources_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.cloud);
      stream.next(m.camera_source);
      stream.next(m.view_points);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct CloudSources_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::gpd_ros::CloudSources_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::gpd_ros::CloudSources_<ContainerAllocator>& v)
  {
    s << indent << "cloud: ";
    s << std::endl;
    Printer< ::sensor_msgs::PointCloud2_<ContainerAllocator> >::stream(s, indent + "  ", v.cloud);
    s << indent << "camera_source[]" << std::endl;
    for (size_t i = 0; i < v.camera_source.size(); ++i)
    {
      s << indent << "  camera_source[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::std_msgs::Int64_<ContainerAllocator> >::stream(s, indent + "    ", v.camera_source[i]);
    }
    s << indent << "view_points[]" << std::endl;
    for (size_t i = 0; i < v.view_points.size(); ++i)
    {
      s << indent << "  view_points[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::geometry_msgs::Point_<ContainerAllocator> >::stream(s, indent + "    ", v.view_points[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // GPD_ROS_MESSAGE_CLOUDSOURCES_H
