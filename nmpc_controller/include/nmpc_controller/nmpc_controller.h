#ifndef NMPC_CONTROLLER_H
#define NMPC_CONTROLLER_H

#include <math.h>
#include <quad_msgs/GRFArray.h>
#include <quad_msgs/LegCommand.h>
#include <quad_msgs/MultiFootPlanDiscrete.h>
#include <quad_msgs/RobotPlan.h>
#include <quad_msgs/RobotState.h>
#include <quad_msgs/RobotStateTrajectory.h>
#include <quad_utils/ros_utils.h>
#include <ros/ros.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

#include <Eigen/Dense>

#include "IpIpoptApplication.hpp"
#include "nmpc_controller/quad_nlp.h"

//! NMPC controller ROS node
/*!
   Wrapper around Quadrupedal MPC that interfaces with our ROS architecture
*/
class NMPCController {
 public:
  /**
   * @brief Constructor for MPCController
   * @param[in] nh ROS NodeHandle to publish and subscribe from
   * @return Constructed object of type MPCController
   */
  NMPCController();

  /**
   * @brief Update the contact and dynamic matrices, solve, and return the
   * output
   * @param[in] initial_state Vector with initial state
   * @param[in] ref_traj Matrix holding desired reference trajectory
   * @param[in] foot_positions Matrix holding foot positions
   * @param[in] contact_schedule Matrix holding the contact schedule
   * @param[in] first_element_duration Time duration to the next plan index
   * @param[in] same_plan_index If the current solving is duplicated in the same
   * index
   * @param[out] state_traj Optimized state trajectory output
   * @param[out] control_traj Optimized control trajectory output
   * @return good_solve
   */
  bool computePlan(const Eigen::VectorXd &initial_state,
                   const Eigen::MatrixXd &ref_traj,
                   const Eigen::MatrixXd &foot_positions,
                   const std::vector<std::vector<bool>> &contact_schedule,
                   Eigen::MatrixXd &state_traj, Eigen::MatrixXd &control_traj);

  bool computeLegPlan(
      const Eigen::VectorXd &initial_state, const Eigen::MatrixXd &ref_traj,
      const Eigen::MatrixXd &foot_positions_body,
      Eigen::MatrixXd &foot_positions_world, Eigen::MatrixXd &foot_velocities,
      const std::vector<std::vector<bool>> &contact_schedule,
      const Eigen::VectorXd &ref_ground_height,
      const double &first_element_duration, const bool &same_plan_index,
      const grid_map::GridMap &terrain, Eigen::MatrixXd &state_traj,
      Eigen::MatrixXd &control_traj);

  /** Method to return the constraint residual for requested data */
  Eigen::VectorXi evalLiftedTrajectoryConstraints(
      Eigen::MatrixXd &state_null_traj);

 private:
  ros::NodeHandle nh_;

  /// Update rate for sending and receiving data;
  double update_rate_;

  SmartPtr<quadNLP> mynlp_;

  SmartPtr<IpoptApplication> app_;

  std::shared_ptr<quad_utils::QuadKD> quadKD_;

  int N_;

  int x_dim_simple_, x_dim_cost_simple_, u_dim_simple_, u_dim_cost_simple_,
      g_dim_simple_, x_dim_complex_, x_dim_cost_complex_, u_dim_complex_,
      u_dim_cost_complex_, g_dim_complex_;

  // Number of states in different components
  static const int n_body_ = 12, n_foot_ = 24, n_joints_ = 24, n_tail_ = 4,
                   m_body_ = 12, m_foot_ = 12, m_tail_ = 2;

  int x_dim_null_;

  double dt_;

  bool require_init_;

  /// Adaptive complexity schedule
  Eigen::VectorXi adaptive_complexity_schedule_;
};

#endif  // MPC_CONTROLLER_H
