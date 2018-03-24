// -*- c++ -*-
/*
 * Copyright (C) 2015 Love Park Robotics, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distribted on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __O3D3XX_SPATIAL_FILTER_CONFIG_H__
#define __O3D3XX_SPATIAL_FILTER_CONFIG_H__

#include <functional>
#include <memory>
#include <unordered_map>
#include <string>

namespace o3d3xx
{
  /**
   * Base class for spatial filters.
   */
  class SpatialFilterConfig
  {
  public:
    /**
     * Mask sizes
     */
    enum class mask_size : int { _3x3 = 0, _5x5 = 1 };

    using Ptr = std::shared_ptr<SpatialFilterConfig>;
    SpatialFilterConfig(int type = 0, const std::string& type_str = "Off");

    /**
     * Factory function for instantiating a SpatialFilterConfig instance
     * from a JSON string.
     *
     * @param[in] json A string of json encoding the filter config parameters.
     *
     * @param[in] filt_ptr A shared pointer to a SpatialFilterConfig to
     *                     bootstrap the default values from.
     *
     * @return A shared pointer to a SpatialFilterConfig reflecting the
     *         passed in JSON encoding.
     */
    static o3d3xx::SpatialFilterConfig::Ptr
    FromJSON(const std::string& json,
             o3d3xx::SpatialFilterConfig::Ptr filt_ptr = nullptr);

    /**
     * Serializes the current state of the filter config to JSON
     */
    std::string ToJSON() const;

    // accessor/mutators
    int Type() const noexcept;
    std::string TypeStr() const noexcept;

    int MaskSize() const;
    void SetMaskSize(int size);

    std::string MaskSizeStr() const;

  protected:
    /** Type id for the filter */
    int type_;

    /** Human readable string that decodes the type id */
    std::string type_str_;

    /** Id for mask size */
    int mask_size_;

  }; // end: class SpatialFilterConfig

  class SpatialMeanFilterConfig : public SpatialFilterConfig
  {
  public:
    using Ptr = std::shared_ptr<SpatialMeanFilterConfig>;
    SpatialMeanFilterConfig();

  }; // end: class SpatialMeanFilterConfig

  class SpatialMedianFilterConfig : public SpatialFilterConfig
  {
  public:
    using Ptr = std::shared_ptr<SpatialMedianFilterConfig>;
    SpatialMedianFilterConfig();

  }; // end: class SpatialMedianFilterConfig

  class SpatialBilateralFilterConfig : public SpatialFilterConfig
  {
  public:
    using Ptr = std::shared_ptr<SpatialBilateralFilterConfig>;
    SpatialBilateralFilterConfig();

  }; // end: class SpatialBilateralFilterConfig

} // end: namespace o3d3xx

#endif // __O3D3XX_SPATIAL_FILTER_CONFIG_H__
