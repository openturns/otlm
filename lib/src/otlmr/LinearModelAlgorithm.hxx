//                                               -*- C++ -*-
/**
 *  @brief LinearModelAlgorithm implements the linear model
 *
 *  Copyright 2005-2016 Airbus-EDF-IMACS-Phimeca
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
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef OTLMR_LINEARMODELALGORITHM_HXX
#define OTLMR_LINEARMODELALGORITHM_HXX

#include "openturns/MetaModelAlgorithm.hxx"
#include "openturns/NumericalSample.hxx"
#include "otlmr/LinearModelResult.hxx"
#include "otlmr/otlmrprivate.hxx"

namespace OTLMR
{


/**
 * @class LinearModelAlgorithm
 *
 * LinearModelAlgorithm implements the notion of linear model
 */

class OTLMR_API LinearModelAlgorithm :
  public OT::MetaModelAlgorithm
{
  CLASSNAME;

public:

  /** Default constructor is private */
  LinearModelAlgorithm();

  /** Parameters constructor */
  LinearModelAlgorithm(const OT::NumericalSample & inputSample,
                       const OT::NumericalSample & outputSample);

  /** Parameters constructor */
  LinearModelAlgorithm(const OT::NumericalSample & inputSample,
                       const OT::Basis & basis,
                       const OT::NumericalSample & outputSample);

  /** Virtual constructor */
  virtual LinearModelAlgorithm * clone() const;


  /** String converter */
  OT::String __repr__() const;

  /** Sample accessors */
  OT::NumericalSample getInputSample() const;
  OT::NumericalSample getOutputSample() const;

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
  OT::NumericalSample inputSample_;

  /** The basis */
  OT::Basis basis_;

  // The associated output data
  OT::NumericalSample outputSample_;

  /** Result */
  LinearModelResult result_;

  /** Bool to tell if optimization has run */
  OT::Bool hasRun_;

}; /* class LinearModelAlgorithm */


} /* namespace OTLMR */

#endif /* OTLMR_LINEARMODELALGORITHM_HXX */
