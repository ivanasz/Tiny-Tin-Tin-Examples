#include <Eigen/Core>
#include <Eigen/Geometry>

#include <glog/logging.h>


class Pose {
	typedef Eigen::Vector3d Vector3d;
	typedef Eigen::Quaterniond Quaterniond;
public:
	Pose(const Quaterniond& orientation,
	     const Vector3d& position);

	Vector3d map(const Vector3d& point) const;

	Vector3d unmap(const Vector3d& point) const;

	Pose inverse();

	Pose operator* (const Pose& other) const;

	const Quaterniond& orientation() const;

	const Vector3d& position() const;

private:
	Quaterniond m_orientation;
	Vector3d m_position;
};

inline const Eigen::Quaterniond&
Pose::orientation() const{
	return m_orientation;
}

inline const Eigen::Vector3d&
Pose::position() const{
	return m_position;
}

inline Pose::Pose(const Quaterniond& orientation,
	          const Vector3d& position)
	: m_orientation(orientation)
	, m_position(position)
{
	CHECK(std::fabs(orientation.norm() - 1) < 1e-15)
		<< "Your quaternion is not normalized:"
		<< orientation.norm();
	// Or, one may prefer
	m_orientation.normalize();
}

inline Eigen::Vector3d
Pose::map(const Eigen::Vector3d& point) const
{
	// Convention: x' = R * x + T
	return m_orientation * point  + m_position;
}

inline Eigen::Vector3d
Pose::unmap(const Eigen::Vector3d& point) const
{
	// Convention: x = R^T * (x' - T)
	return m_orientation.inverse() * (point  -  m_position);
}

inline Pose Pose::inverse()
{
	// Convention: x = R^T * (x' - T)
	//               = R^T * x' + (-R^T *T)
	Quaterniond inv_orientation = m_orientation.inverse();
	Vector3d inv_position = - (inv_orientation * m_position);

	return Pose(inv_orientation, inv_position);
}

inline Pose Pose::operator*(const Pose& other) const
{
	// Convention: Pose * PoseOther
	//              x' = (Pose * PoseOther).map(x)
	//              x' = Pose.map(PoseOther.map(x))
	//              x' = R * (Rother * x + Tother) + T
	//              x' = R * Rother * x + (R * Tother + T)

	Quaterniond orientation = (m_orientation * other.orientation());
	Vector3d position = m_orientation * other.position() + m_position;

	return Pose(orientation, position);
}

int main()
{
	using namespace std;
	typedef Eigen::Vector3d Vector3d;
	typedef Eigen::Quaterniond Quaterniond;

	// Assumptions: Pose is a transformation that
	// takes a point from SENSOR coordinate and
	// brings it into WORLD/GLOBAL/IMU coordinate

	// submap1  pose
	Quaterniond q1(2, 0, 1, -3);
	q1.normalize();
	Pose pose1(q1, Vector3d(1.2, 2, 3));
	Vector3d point1(1, 2, 3);

	// submap2  pose
	Quaterniond q2(2, 0, 1, -3); q2.normalize();
	Pose pose2(q2, Vector3d(1.2, 2.2, 3));
	Vector3d point2(1.1, 2.2, 3.2);


	// CHECK1 : pose.map() and pose.unmap()
	Vector3d point1_again = pose1.unmap(pose1.map(point1));
	double error = (point1 - point1_again).norm();
	CHECK(error < 1e-10) << "error in pose.map() and/or pose.unmap(): "
		             << error;

	// CHECK2 : pose.inverse() and pose.map()
	Pose pose1_inv = pose1.inverse();
	error = (point1 - pose1_inv.map(pose1.map(point1))).norm();
	CHECK(error < 1e-10) << "error in pose.map() and/or pose.inverse(): "
		             << error;

	// CHECK3: pose * operator
	Pose pose_identity  =  pose1_inv * pose1;
	error = (point1 - pose_identity.map(point1)).norm();
	CHECK(error < 1e-10) << "error in pose * operator: "
		             << error;
	// CHECK4: pose * operator
	Pose pose21  =  pose2 * pose1;
	error = (pose2.map(pose1.map(point1)) - pose21.map(point1)).norm();
	CHECK(error < 1e-10) << "error in pose * operator: "
		             << error;

	//------------------------------------------------------//
	// Submaps and their relative pose
	// Assumptions:
	//	submap1 is aligned to submap2
	//	in SENSOR2 coordinate system
	//------------------------------------------------------//

	// Bring point1 in SENSOR1 to SENSOR2 coordinte system
	Pose pose1to2 = pose2 * pose1;
	Vector3d point1_in_sensor2 = pose1to2.map(point1);

	// Calculate alignment/relative pose between 2 submaps in
	// SENSOR2 coordinate system (using ICP or whatever
	Quaterniond alignment_orientation(2, 0, 1, -3);
	alignment_orientation.normalize();
	Vector3d alignment_position(0.5, 0.11, 0.16);
	Pose alignment_pose(alignment_orientation, alignment_position);

	// Check that two submaps align
	Vector3d point1_in_sensor2_aligned
		= alignment_pose.map(point1_in_sensor2);
        LOG(INFO) << "Do your image alignment check in SENSOR2 coordinates";

	// Calculate alignment/relative pose between 2 submaps in
	// WORLD/GLOBAL/IMU coordinate system
	Pose relative_pose = pose2 * alignment_pose * pose2.inverse();

	// Calculate pose1 correction
	// Recall: pose1 is a transform from
	// SENSOR1 to WORLD coordinate system
	Pose pose1_corrected = relative_pose * pose1;

	// Map to WORLD coordinate system
	Vector3d point1_world = pose1_corrected.map(point1);
	Vector3d point2_world = pose2.map(point2);
        LOG(INFO) << "Do your image alignment check in WORLD coordinates";

	LOG(INFO) << "DONE. Your portfolio increase today is: "
		  << (point1_world.norm()
		    + point2_world.norm()
		    + point1_in_sensor2_aligned.norm()) * 10
		 << " %";
	return 0;
}
