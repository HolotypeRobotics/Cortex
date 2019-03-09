
/* ---------------------------------------------------------------------
 * Numenta Platform for Intelligent Computing (NuPIC)
 * Copyright (C) 2013, Numenta, Inc.  Unless you have an agreement
 * with Numenta, Inc., for a separate license for this software code, the
 * following terms and conditions apply:
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero Public License for more details.
 *
 * You should have received a copy of the GNU Affero Public License
 * along with this program.  If not, see http://www.gnu.org/licenses.
 *
 * http://numenta.org/licenses/
 * ---------------------------------------------------------------------
 */

/** @file
 * Interface for the Dimensions class
 */

#ifndef NTA_DIMENSIONS_HPP
#define NTA_DIMENSIONS_HPP

#include <iostream>
#include <sstream>
#include <cctype>
#include <vector>
#include <numeric>    // for std::accumulate
#include <nupic/types/Types.hpp>
#include <nupic/utils/Log.hpp>
#include <nupic/math/StlIo.hpp>


namespace nupic {

class Dimensions : public std::vector<UInt> {
public:
  /**
   * Create a new Dimensions object.
   * The dimension in index 0 is the one that moves fastest while iterating.
   * in 2D coordinates, x,y; the x is dimension[0], y is dimension[1].
   * @note Default dimensions are unspecified, see isUnspecified()
   *       Dimensions of size=1 and value [0] = 0 means "not known yet", see isDontCare()
   */
  Dimensions() {};
  Dimensions(UInt x) { push_back(x); }
  Dimensions(UInt x, UInt y) {  push_back(x); push_back(y); }
  Dimensions(UInt x, UInt y, UInt z) { push_back(x); push_back(y); push_back(z); }
  Dimensions(const std::vector<UInt>& v) : std::vector<UInt>(v){};
  Dimensions(const Dimensions& d)  : std::vector<UInt>(d){};

  /**
   * @returns  The count of cells in the grid which is the product of the sizes of
   * the dimensions.
   */
  size_t getCount() const { return((size() > 0) ? std::accumulate(begin(), end(), 1, std::multiplies<UInt>()) : 0);}


  /**
   *
   * There are two "special" values for dimensions:
   *
   * * Dimensions of `[]` (`dims.size()==0`) means "empty dimensions" aka
   *         "unspecified", see isUnspecified() 
   * * Dimensions of `[0]`  (`dims.size()==1 && dims[0] == 0`) means
   *         "in process of being specified but not yet resolved.", see isDontcare()
   *
   * The states that a Dimensions object can have are:
   *
   * * Unspecified - empty; Everything starts out as unspecified.
   *
   * * Dont Care   - We are in the process of setting all dimensions. 
   *                 We have checked direct explicit configuration,
   *                 and trying implied configuration. Not yet resolved.
   *
   *                 For example, if we looked at an input and checked that 
	 *                 it was not configured with a dimension we can mark it
   *                 as isDontCare so that later when we determine the 
   *                 dimensions of the connected output we know that it 
   *                 can also assign it to the input.
   *
   * * Specified   - We have a good dimension. We have at least one dimension.
   *                 It's not the opposite of isUnspecified()!
   *
   * * Invalid     - Some dimension is 0.
   *
   * There is a function to check for each of these states.
   */
  bool isUnspecified() const { return(size() == 0); }
  bool isDontcare()    const { return(size() == 1 && at(0) == 0); }
  bool isInvalid()     const { return(getCount() == 0); }
  bool isSpecified()   const { return(!isInvalid()); }
  static const int DONTCARE = 0;


  inline std::ostream &operator<<(std::ostream &f) {
    f << (std::vector<UInt>&)*this;
    return f;
  }
  inline std::istream &operator>>(std::istream &f) { 
    f >> (std::vector<UInt>&)*this;
  }

  // TODO: may need operator== and operator!=

};


} // namespace nupic

#endif // NTA_DIMENSIONS_HPP
