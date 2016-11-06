//                                               -*- C++ -*-
/**
 *  @brief Factory for linear model
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
#ifndef OTLMR_LINEARMODELSTEPWISEFACTORY_HXX
#define OTLMR_LINEARMODELSTEPWISEFACTORY_HXX

#include "openturns/PersistentObject.hxx"
#include "openturns/NumericalSample.hxx"
#include "openturns/Matrix.hxx"
#include "openturns/Indices.hxx"
#include "openturns/Basis.hxx"
#include "otlmr/LinearModelResult.hxx"
#include "otlmr/otlmrprivate.hxx"

namespace OTLMR
{

/**
 * @class LinearModelStepwiseAlgorithm
 *
 * The class building linear model using stepwise regression
 */

class OTLMR_API LinearModelStepwiseAlgorithm :
  public OT::PersistentObject
{
  CLASSNAME;

public:

  enum Direction { BACKWARD = -1, BOTH = 0, FORWARD = 1 };

  /** Default constructor */
  LinearModelStepwiseAlgorithm();

  /** Parameters constructor FORWARD and BACKWARD */
  LinearModelStepwiseAlgorithm(const OT::NumericalSample & inputSample,
                               const OT::Basis & basis,
                               const OT::NumericalSample & outputSample,
                               const OT::Indices & minimalIndices,
                               const OT::Bool isForward = true,
                               const OT::NumericalScalar penalty = -1.0 /* < 0 means BIC, by convention */,
                               const OT::UnsignedInteger maximumIterationNumber = 1000);

  /** Parameters constructor BOTH */
  LinearModelStepwiseAlgorithm(const OT::NumericalSample & inputSample,
                               const OT::Basis & basis,
                               const OT::NumericalSample & outputSample,
                               const OT::Indices & minimalIndices,
                               const OT::Indices & startIndices,
                               const OT::NumericalScalar penalty = -1.0 /* < 0 means BIC, by convention */,
                               const OT::UnsignedInteger maximumIterationNumber = 1000);

  /** Virtual constructor */
  virtual LinearModelStepwiseAlgorithm * clone() const;

  /** String converter */
  OT::String __repr__() const;
  OT::String __str__(const OT::String & offset = "") const;

  /** Sample accessors */
  OT::NumericalSample getInputSample() const;
  OT::NumericalSample getOutputSample() const;

  /** Direction accessors */
  Direction getDirection() const;

  /** Penalty accessors */
  OT::NumericalScalar getPenalty() const;

  /** Maximum number of iterations accessors */
  OT::UnsignedInteger getMaximumIterationNumber() const;

  /** Condensed formula accessor */
  OT::String getFormula() const;

  /** Perform regression */
  void run();

  /** Result accessor */
  LinearModelResult getResult();

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv);

private:

  /** Compute the likelihood function */
  OT::NumericalScalar computeLogLikelihood();

  /** Build currentX_ and currentIndices_ from given indices */
  void buildCurrentMatrixFromIndices(const OT::Indices & columns);

  /** Input sample */
  OT::NumericalSample inputSample_;

  /** The monomials collection */
  OT::Basis basis_;

  /** Output sample */
  OT::NumericalSample outputSample_;

  /** The direction of the stepwise regression method */
  Direction direction_;

  /** The multiple of the degrees of freedom used for the penalty of the stepwise regression method
      - 2      Akaike   information criterion (AIC)
      - log(n) Bayesian information criterion (BIC)  */
  OT::NumericalScalar penalty_;

  /** The maximum number of iterations of the stepwise regression method */
  OT::UnsignedInteger maximumIterationNumber_;

  /** The indices of minimal model */
  OT::Indices minimalIndices_;

  /** The indices of start model */
  OT::Indices startIndices_;

  /** The formula description */
  OT::String condensedFormula_;

  /** The output data, stored as a matrix  */
  OT::Matrix Y_;

  /** The matrix X_{max} containing all monomials */
  OT::Matrix maxX_;

  /** The current matrix */
  OT::Matrix currentX_;

  /** QR decomposition of the current matrix */
  OT::Matrix currentQ_;
  OT::Matrix currentInvRt_;

  /** The current residual R=Y - Q Q^T Y */
  OT::Matrix currentResidual_;

  /** The indices of current model */
  OT::Indices currentIndices_;

  /** Result */
  LinearModelResult result_;

  /** Whether result is cached */
  OT::Bool hasRun_;

}; /* class LinearModelStepwiseAlgorithm */

} /* namespace OTLMR */

#endif /* OTLMR_LINEARMODELSTEPWISEFACTORY_HXX */
