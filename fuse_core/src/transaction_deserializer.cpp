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
#include <fuse_core/transaction_deserializer.h>

#include <sstream>


namespace fuse_core
{

void serializeTransaction(const fuse_core::Transaction& transaction, fuse_msgs::SerializedTransaction& msg)
{
  std::stringstream stream;
  {
    boost::archive::text_oarchive archive(stream);
    archive << transaction;
  }
  msg.data = stream.str();
}

TransactionDeserializer::TransactionDeserializer() :
  constraint_loader_("fuse_core", "fuse_core::Constraint"),
  variable_loader_("fuse_core", "fuse_core::Variable")
{
  // Load all known plugin libraries
  // I believe the library containing a given Variable or Constraint must be loaded in order to deserialize
  // an object of that type. But I haven't actually tested that theory.
  for (const auto& class_name : constraint_loader_.getDeclaredClasses())
  {
    constraint_loader_.loadLibraryForClass(class_name);
  }
  for (const auto& class_name : variable_loader_.getDeclaredClasses())
  {
    variable_loader_.loadLibraryForClass(class_name);
  }
}

fuse_core::Transaction TransactionDeserializer::deserialize(const fuse_msgs::SerializedTransaction::ConstPtr& msg)
{
  return deserialize(*msg);
}

fuse_core::Transaction TransactionDeserializer::deserialize(const fuse_msgs::SerializedTransaction& msg)
{
  // Create a Variable object using pluginlib. This will throw if the plugin name is not found.
  // The unique ptr returned by pluginlib has a custom deleter. This makes it annoying to return
  // back to the user as the output is not equivalent to fuse_ros::Variable::UniquePtr. A shared_ptr
  // on the other hand is able to capture the custom deleter without modifying the datatype.
  auto transaction = fuse_core::Transaction();
  // Deserialize the message into the Variable. This will throw if something goes wrong in the deserialization.
  std::stringstream stream(msg.data);
  {
    boost::archive::text_iarchive archive(stream);
    archive >> transaction;
  }
  // Return the populated variable. UniquePtrs are automatically promoted to SharedPtrs.
  return transaction;
}

}  // namespace fuse_core
