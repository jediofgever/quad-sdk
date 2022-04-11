#include <gtest/gtest.h>
#include <nmpc_controller/nmpc_controller.h>
#include <ros/ros.h>

#include <chrono>

TEST(NMPCTest, testTailMPC) {
  int N_, N_tail_;
  double dt_;
  ros::param::get("/nmpc_controller/leg/horizon_length", N_);
  ros::param::get("/nmpc_controller/distributed_tail/horizon_length", N_tail_);
  ros::param::get("/nmpc_controller/leg/step_length", dt_);

  std::shared_ptr<NMPCController> leg_planner_ =
      std::make_shared<NMPCController>(0);
  std::shared_ptr<NMPCController> distributed_tail_planner_ =
      std::make_shared<NMPCController>(2);

  Eigen::VectorXd current_state_(12);
  current_state_.fill(0);
  current_state_(2) = 0.27;
  current_state_(5) = 1.57;
  current_state_(6) = -0.75;

  Eigen::MatrixXd ref_body_plan_(N_ + 1, 12);
  ref_body_plan_.fill(0);
  ref_body_plan_.row(0) = current_state_.transpose();
  ref_body_plan_.col(2).fill(0.27);
  ref_body_plan_.col(5).fill(1.57);
  ref_body_plan_.col(6).fill(-0.75);
  for (size_t i = 1; i < N_ + 1; i++) {
    ref_body_plan_(i, 0) =
        ref_body_plan_(i - 1, 0) + dt_ * ref_body_plan_(i - 1, 6);
  }

  Eigen::MatrixXd foot_positions_body_(N_, 12);
  for (size_t i = 0; i < N_; i++) {
    foot_positions_body_.row(i) << 0.2263, 0.098, -0.27, -0.2263, 0.098, -0.27,
        0.2263, -0.098, -0.27, -0.2263, -0.098, -0.27;
  }

  std::vector<std::vector<bool>> adpative_contact_schedule_;
  adpative_contact_schedule_.resize(N_);
  for (size_t i = 0; i < N_; i++) {
    adpative_contact_schedule_.at(i).resize(4);
    if (i % 12 < 6) {
      adpative_contact_schedule_.at(i) = {true, false, false, true};
    } else {
      adpative_contact_schedule_.at(i) = {false, true, true, false};
    }
  }

  Eigen::VectorXd ref_ground_height(N_ + 1);
  ref_ground_height.fill(0);

  Eigen::MatrixXd body_plan_(N_ + 1, 12);
  Eigen::MatrixXd grf_plan_(N_, 12);

  Eigen::VectorXd tail_current_state_(4);
  tail_current_state_.fill(0);
  // tail_current_state_(0) = 0.76;

  Eigen::MatrixXd ref_tail_plan_(N_tail_ + 1, 4);
  ref_tail_plan_.fill(0);
  // ref_tail_plan_.col(0).fill(0.76);

  Eigen::MatrixXd tail_plan_(N_tail_ + 1, 16);
  Eigen::MatrixXd tail_torque_plan_(N_tail_, 14);

  double first_element_duration = dt_;

  bool same_plan_index = false;

  std::chrono::steady_clock::time_point tic, toc;

  for (size_t i = 0; i < 12; i++) {
    tic = std::chrono::steady_clock::now();

    leg_planner_->computeLegPlan(
        current_state_, ref_body_plan_, foot_positions_body_,
        adpative_contact_schedule_, ref_ground_height, first_element_duration,
        same_plan_index, body_plan_, grf_plan_);

    toc = std::chrono::steady_clock::now();
    std::cout << "Leg time difference = "
              << std::chrono::duration_cast<std::chrono::microseconds>(toc -
                                                                       tic)
                     .count()
              << "[µs]" << std::endl;

    tic = std::chrono::steady_clock::now();

    distributed_tail_planner_->computeDistributedTailFullPlan(
        current_state_, ref_body_plan_.topRows(N_tail_ + 1),
        foot_positions_body_.topRows(N_tail_),
        std::vector<std::vector<bool>>(
            adpative_contact_schedule_.begin(),
            adpative_contact_schedule_.begin() + N_tail_),
        tail_current_state_, ref_tail_plan_, body_plan_.topRows(N_tail_ + 1),
        grf_plan_.topRows(N_tail_), ref_ground_height.topRows(N_tail_ + 1),
        first_element_duration, same_plan_index, tail_plan_, tail_torque_plan_);

    toc = std::chrono::steady_clock::now();
    std::cout << "Tail time difference = "
              << std::chrono::duration_cast<std::chrono::microseconds>(toc -
                                                                       tic)
                     .count()
              << "[µs]" << std::endl;

    current_state_.segment(0, 6) = tail_plan_.block(1, 0, 1, 6).transpose();
    current_state_.segment(6, 6) = tail_plan_.block(1, 8, 1, 6).transpose();
    tail_current_state_.segment(0, 2) =
        tail_plan_.block(1, 6, 1, 2).transpose();
    tail_current_state_.segment(2, 2) =
        tail_plan_.block(1, 14, 1, 2).transpose();

    ref_body_plan_.topRows(N_) = ref_body_plan_.bottomRows(N_);
    ref_body_plan_(N_, 0) =
        ref_body_plan_(N_ - 1, 0) + dt_ * ref_body_plan_(N_ - 1, 6);

    std::rotate(adpative_contact_schedule_.begin(),
                adpative_contact_schedule_.begin() + 1,
                adpative_contact_schedule_.end());
  }

  for (size_t i = 0; i < 15; i++) {
    std::vector<std::vector<bool>> adpative_contact_schedule_2 =
        adpative_contact_schedule_;
    if (i < 6) {
      for (size_t j = 0; j < 6 - i; j++) {
        adpative_contact_schedule_2.at(j) = {false, false, false, true};
      }
    }
    if (i > 11) {
      for (size_t j = 0; j < 18 - i; j++) {
        adpative_contact_schedule_2.at(j) = {false, false, false, true};
      }
    }

    tic = std::chrono::steady_clock::now();

    leg_planner_->computeLegPlan(
        current_state_, ref_body_plan_, foot_positions_body_,
        adpative_contact_schedule_2, ref_ground_height, first_element_duration,
        same_plan_index, body_plan_, grf_plan_);

    toc = std::chrono::steady_clock::now();
    std::cout << "Leg time difference = "
              << std::chrono::duration_cast<std::chrono::microseconds>(toc -
                                                                       tic)
                     .count()
              << "[µs]" << std::endl;

    tic = std::chrono::steady_clock::now();

    distributed_tail_planner_->computeDistributedTailFullPlan(
        current_state_, ref_body_plan_.topRows(N_tail_ + 1),
        foot_positions_body_.topRows(N_tail_),
        std::vector<std::vector<bool>>(
            adpative_contact_schedule_2.begin(),
            adpative_contact_schedule_2.begin() + N_tail_),
        tail_current_state_, ref_tail_plan_, body_plan_.topRows(N_tail_ + 1),
        grf_plan_.topRows(N_tail_), ref_ground_height.topRows(N_tail_ + 1),
        first_element_duration, same_plan_index, tail_plan_, tail_torque_plan_);

    toc = std::chrono::steady_clock::now();
    std::cout << "Tail time difference = "
              << std::chrono::duration_cast<std::chrono::microseconds>(toc -
                                                                       tic)
                     .count()
              << "[µs]" << std::endl;

    current_state_.segment(0, 6) = tail_plan_.block(1, 0, 1, 6).transpose();
    current_state_.segment(6, 6) = tail_plan_.block(1, 8, 1, 6).transpose();
    tail_current_state_.segment(0, 2) =
        tail_plan_.block(1, 6, 1, 2).transpose();
    tail_current_state_.segment(2, 2) =
        tail_plan_.block(1, 14, 1, 2).transpose();

    ref_body_plan_.topRows(N_) = ref_body_plan_.bottomRows(N_);
    ref_body_plan_(N_, 0) =
        ref_body_plan_(N_ - 1, 0) + dt_ * ref_body_plan_(N_ - 1, 6);

    std::rotate(adpative_contact_schedule_.begin(),
                adpative_contact_schedule_.begin() + 1,
                adpative_contact_schedule_.end());
  }

  for (size_t i = 0; i < 12; i++) {
    tic = std::chrono::steady_clock::now();

    leg_planner_->computeLegPlan(
        current_state_, ref_body_plan_, foot_positions_body_,
        adpative_contact_schedule_, ref_ground_height, first_element_duration,
        same_plan_index, body_plan_, grf_plan_);

    toc = std::chrono::steady_clock::now();
    std::cout << "Leg time difference = "
              << std::chrono::duration_cast<std::chrono::microseconds>(toc -
                                                                       tic)
                     .count()
              << "[µs]" << std::endl;

    tic = std::chrono::steady_clock::now();

    distributed_tail_planner_->computeDistributedTailFullPlan(
        current_state_, ref_body_plan_.topRows(N_tail_ + 1),
        foot_positions_body_.topRows(N_tail_),
        std::vector<std::vector<bool>>(
            adpative_contact_schedule_.begin(),
            adpative_contact_schedule_.begin() + N_tail_),
        tail_current_state_, ref_tail_plan_, body_plan_.topRows(N_tail_ + 1),
        grf_plan_.topRows(N_tail_), ref_ground_height.topRows(N_tail_ + 1),
        first_element_duration, same_plan_index, tail_plan_, tail_torque_plan_);

    toc = std::chrono::steady_clock::now();
    std::cout << "Tail time difference = "
              << std::chrono::duration_cast<std::chrono::microseconds>(toc -
                                                                       tic)
                     .count()
              << "[µs]" << std::endl;

    current_state_.segment(0, 6) = tail_plan_.block(1, 0, 1, 6).transpose();
    current_state_.segment(6, 6) = tail_plan_.block(1, 8, 1, 6).transpose();
    tail_current_state_.segment(0, 2) =
        tail_plan_.block(1, 6, 1, 2).transpose();
    tail_current_state_.segment(2, 2) =
        tail_plan_.block(1, 14, 1, 2).transpose();

    ref_body_plan_.topRows(N_) = ref_body_plan_.bottomRows(N_);
    ref_body_plan_(N_, 0) =
        ref_body_plan_(N_ - 1, 0) + dt_ * ref_body_plan_(N_ - 1, 6);

    std::rotate(adpative_contact_schedule_.begin(),
                adpative_contact_schedule_.begin() + 1,
                adpative_contact_schedule_.end());
  }

  EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  ros::init(argc, argv, "nmpc_controller_tester");

  return RUN_ALL_TESTS();
}
