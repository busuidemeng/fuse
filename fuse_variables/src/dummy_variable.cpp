/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2019, Locus Robotics
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
#include <fuse_variables/dummy_variable.h>

#include <fuse_core/uuid.h>
#include <fuse_variables/fixed_size_variable.h>
#include <fuse_variables/stamped.h>
#include <pluginlib/class_list_macros.hpp>
#include <ros/time.h>

#include <ostream>


namespace fuse_variables
{

DummyVariable::DummyVariable(const ros::Time& stamp, const std::string& quest) :
  fuse_core::Variable(fuse_core::uuid::generate(detail::type(), stamp)),
  quest_(quest),
  stamp_(stamp)
{
}

void DummyVariable::print(std::ostream& stream) const
{
  stream << type() << ":\n"
         << "  uuid: " << uuid() << "\n"
         << "  stamp: " << stamp() << "\n"
         << "  quest: " << quest() << "\n"
         << "  size: " << size() << "\n"
         << "  data:\n"
         << "  - a: " << a() << "\n"
         << "  - b: " << b() << "\n";
}

}  // namespace fuse_variables

// Register this variable with ROS as a plugin. This allows the pluginlib class loader to be used to deserialize
// variables in a generic manner in other classes.
PLUGINLIB_EXPORT_CLASS(fuse_variables::DummyVariable, fuse_core::Variable);