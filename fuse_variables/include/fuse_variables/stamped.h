/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2018, Locus Robotics
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef FUSE_VARIABLES_STAMPED_H
#define FUSE_VARIABLES_STAMPED_H

#include <fuse_core/macros.h>
#include <fuse_core/uuid.h>
#include <ros/node_handle.h>
#include <ros/time.h>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

// #include <cereal/types/polymorphic.hpp>


namespace fuse_variables
{

/**
 * @brief A class that provides a timestamp and device id
 *
 * This is intended to be used as secondary base class (multiple inheritance) for variables that are time-varying.
 * Some common examples include robot poses or velocities. This is in contrast to variables that represent unknown
 * but fixed quantities, such as the world position of landmarks, or possibly calibration values that are assumed
 * constant.
 */
class Stamped
{
public:
  SMART_PTR_ALIASES_ONLY(Stamped);

  /**
   * @brief Default constructor
   */
  Stamped() = default;

  /**
   * @brief Constructor
   */
  explicit Stamped(const ros::Time& stamp, const fuse_core::UUID& device_id = fuse_core::uuid::NIL) :
    device_id_(device_id),
    stamp_(stamp)
  {}

  /**
   * @brief Destructor
   */
  virtual ~Stamped() = default;

  /**
   * @brief Read-only access to the associated timestamp.
   */
  const ros::Time& stamp() const { return stamp_; }

  /**
   * @brief Read-only access to the associated device ID.
   */
  const fuse_core::UUID& deviceId() const { return device_id_; }

  /**
   * @brief Serialize the members
   */
  template<class Archive>
  void serialize(Archive& archive, const unsigned int version)
  {
    archive & device_id_;
    archive & stamp_;
  }

private:
  fuse_core::UUID device_id_;  //!< The UUID associated with this specific device or hardware
  ros::Time stamp_;  //!< The timestamp associated with this variable instance
};

/**
 * @brief Load a device id from the parameter server
 *
 * This function first checks if the 'device_id' parameter is available. If so, it attempts to load the device_id
 * using that parameter. There are a few supported formats:
 * - "01234567-89AB-CDEF-0123-456789ABCDEF"
 * - "01234567-89ab-cdef-0123-456789abcdef"
 * - "0123456789abcdef0123456789abcdef"
 * - "{01234567-89ab-cdef-0123-456789abcdef}"
 *
 * If the 'device_id' parameter doesn't exist, this function checks for the 'device_name' parameter. If found, a
 * device_id is generated by computing a hash on the provided string. See fuse_core::uuid::generate(const std::string&)
 * for details.
 *
 * If neither parameter exits, the device_id is populated by all zeros, the so-called NIL UUID.
 *
 * Will throw if the device_id parameter is not in an expected format.
 *
 * @param[in] node_handle A node handle in the desired parameter namespace
 * @return                A device UUID
 */
fuse_core::UUID loadDeviceId(const ros::NodeHandle& node_handle);

}  // namespace fuse_variables

#endif  // FUSE_VARIABLES_STAMPED_H
