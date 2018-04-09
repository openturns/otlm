//                                               -*- C++ -*-
/**
 *  @brief LinearModelAlgorithm implements the linear model
 *
 *  Copyright 2005-2018 Airbus-EDF-IMACS-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef OTLM_LINEARMODELALGORITHM_HXX
#define OTLM_LINEARMODELALGORITHM_HXX

#include "openturns/MetaModelAlgorithm.hxx"
#include "openturns/Sample.hxx"
#include "otlm/LinearModelResult.hxx"
#include "otlm/otlmprivate.hxx"

namespace OTLM
{


/**
 * @class LinearModelAlgorithm
 *
 * LinearModelAlgorithm implements the notion of linear model
 */

class OTLM_API LinearModelAlgorithm :
  public OT::MetaModelAlgorithm
{
  CLASSNAME

public:

  /** Default constructor is private */
  LinearModelAlgorithm();

  /** Parameters constructor */
  LinearModelAlgorithm(const OT::Sample & inputSample,
                       const OT::Sample & outputSample);

  /** Parameters constructor */
  LinearModelAlgorithm(const OT::Sample & inputSample,
                       const OT::Basis & basis,
                       const OT::Sample & outputSample);

  /** Virtual constructor */
  virtual LinearModelAlgorithm * clone() const;


  /** String converter */
  OT::String __repr__() const;

  /** Sample accessors */
  OT::Sample getInputSample() const;
  OT::Sample getOutputSample() const;

  /** Basis accessor */
  OT::Basis getBasis() const;

  /** Perform regression */
  void run();

  /** result accessor */
  LinearModelResult getResult();

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv);


private:

  // The input data
  OT::Sample inputSample_;

  /** The basis */
  OT::Basis basis_;

  // The associated output data
  OT::Sample outputSample_;

  /** Result */
  LinearModelResult result_;

  /** Bool to tell if optimization has run */
  OT::Bool hasRun_;

}; /* class LinearModelAlgorithm */


} /* namespace OTLM */

#endif /* OTLM_LINEARMODELALGORITHM_HXX */
