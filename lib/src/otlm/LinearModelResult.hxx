//                                               -*- C++ -*-
/**
 *  @brief The result of a linear model estimation
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
#ifndef OTLM_LINEARMODELRESULT_HXX
#define OTLM_LINEARMODELRESULT_HXX

#include "openturns/MetaModelResult.hxx"
#include "openturns/NumericalSample.hxx"
#include "openturns/Basis.hxx"
#include "openturns/Matrix.hxx"
#include "openturns/NumericalMathFunction.hxx"
#include "otlm/otlmprivate.hxx"


namespace OTLM
{

/**
 * @class LinearModelResult
 *
 * The result of a linear model evaluation
 */

class OTLM_API LinearModelResult :
  public OT::MetaModelResult
{
  CLASSNAME;

public:


  /** Default constructor */
  LinearModelResult();

  /* We cannot use a DesignProxy here since it is not persistent */
  /** Parameter constructor */
  LinearModelResult(const OT::NumericalSample & inputSample,
                    const OT::Basis & basis,
                    const OT::Matrix & design,
                    const OT::NumericalSample & outputSample,
                    const OT::NumericalMathFunction & metaModel,
                    const OT::NumericalPoint & trendCoefficients,
                    const OT::String & formula,
                    const OT::Description & coefficientsNames,
                    const OT::NumericalSample & sampleResiduals,
                    const OT::NumericalPoint & diagonalGramInverse,
                    const OT::NumericalPoint & leverages,
                    const OT::NumericalPoint & cookDistances);

  /** Virtual constructor */
  LinearModelResult * clone() const;

  /** String converter */
  OT::String __repr__() const;

  /** Sample accessors */
  OT::NumericalSample getInputSample() const;
  OT::NumericalSample getOutputSample() const;
  OT::NumericalSample getFittedSample() const;

  /** Condensed formula accessor */
  OT::NumericalPoint getTrendCoefficients() const;

  /** Condensed formula accessor */
  OT::String getFormula() const;

  /** Coefficients names accessor */
  OT::Description getCoefficientsNames() const;

  /** Residuals accessor */
  OT::NumericalSample getSampleResiduals() const;

  /** Standardized residuals accessor */
  OT::NumericalSample getStandardizedResiduals() const;

  /** Leverages accessor */
  OT::NumericalPoint getLeverages() const;

  /** Diagonal Gram inverse accessor */
  OT::NumericalPoint getDiagonalGramInverse() const;

  /** Cook distance accessor */
  OT::NumericalPoint getCookDistances() const;

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv);

private:

  /** Compute standardized residuals */
  void computeStandardizedResiduals();

  /** input data */
  OT::NumericalSample inputSample_;

  /** basis */
  OT::Basis basis_;

  /** input data */
  OT::Matrix design_;

  /** output data */
  OT::NumericalSample outputSample_;

  /** Intercept and trend coefficients */
  OT::NumericalPoint beta_;

  /** The formula description */
  OT::String condensedFormula_;

  /** Coefficients names */
  OT::Description coefficientsNames_;

  /** Whole residuals */
  OT::NumericalSample sampleResiduals_;

  /** Standardized residuals */
  OT::NumericalSample standardizedResiduals_;

  /** Diagonal of (Xt X)^{-1} */
  OT::NumericalPoint diagonalGramInverse_;

  /** Leverages */
  OT::NumericalPoint leverages_;

  /** Cook's distances */
  OT::NumericalPoint cookDistances_;

}; /* class LinearModelResult */

} /* namespace OTLM */

#endif /* OTLM_LINEARMODELRESULT_HXX */
