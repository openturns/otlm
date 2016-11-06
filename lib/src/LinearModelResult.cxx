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
#include "otlmr/LinearModelResult.hxx"
#include "openturns/PersistentObjectFactory.hxx"
#include "openturns/NumericalMathFunction.hxx"
#include "openturns/LinearModel.hxx"
#include "openturns/OSS.hxx"


using namespace OT;

namespace OTLMR
{

CLASSNAMEINIT(LinearModelResult);

static const Factory<LinearModelResult> Factory_LinearModelResult;

/* Default constructor */
LinearModelResult::LinearModelResult()
  : MetaModelResult()
{
  // Nothing to do
}

/* Parameter constructor */
LinearModelResult::LinearModelResult(const NumericalSample & inputSample,
                                     const Basis & basis,
                                     const Matrix & design,
                                     const NumericalSample & outputSample,
                                     const NumericalMathFunction & metaModel,
                                     const NumericalPoint & trendCoefficients,
                                     const String & formula,
                                     const Description & coefficientsNames,
                                     const NumericalSample & sampleResiduals,
                                     const NumericalPoint & diagonalGramInverse,
                                     const NumericalPoint & leverages,
                                     const NumericalPoint & cookDistances)
  : MetaModelResult(NumericalMathFunction(inputSample, outputSample), metaModel, NumericalPoint(1, 0.0), NumericalPoint(1, 0.0))
  , inputSample_(inputSample)
  , basis_(basis)
  , design_(design)
  , outputSample_(outputSample)
  , condensedFormula_(formula)
  , coefficientsNames_(coefficientsNames)
  , beta_(trendCoefficients)
  , sampleResiduals_(sampleResiduals)
  , diagonalGramInverse_(diagonalGramInverse)
  , leverages_(leverages)
  , cookDistances_(cookDistances)
{
  const UnsignedInteger size = inputSample.getSize();
  if (size != outputSample.getSize())
    throw InvalidArgumentException(HERE) << "In LinearModelResult::LinearModelResult, input & output sample have different size. input sample size = " << size << ", output sample size = " << outputSample.getSize();
  // Compute standardized residuals
  computeStandardizedResiduals();
}

/* Virtual constructor */
LinearModelResult * LinearModelResult::clone() const
{
  return new LinearModelResult(*this);
}


/* String converter */
String LinearModelResult::__repr__() const
{
  return OSS(true) << "class=" << getClassName();
}


/* Input sample accessor */
NumericalSample LinearModelResult::getInputSample() const
{
  return inputSample_;
}


/* Output sample accessor */
NumericalSample LinearModelResult::getOutputSample() const
{
  return outputSample_;
}

/* Fitted sample accessor */
NumericalSample LinearModelResult::getFittedSample() const
{
  return metaModel_(inputSample_);
}

/* Formula accessor */
NumericalPoint LinearModelResult::getTrendCoefficients() const
{
  return beta_;
}

/* Formula accessor */
String LinearModelResult::getFormula() const
{
  return condensedFormula_;
}

Description LinearModelResult::getCoefficientsNames() const
{
  return coefficientsNames_;
}

NumericalSample LinearModelResult::getSampleResiduals() const
{
  return sampleResiduals_;
}

NumericalSample LinearModelResult::getStandardizedResiduals() const
{
  return standardizedResiduals_;
}

NumericalPoint LinearModelResult::getLeverages() const
{
  return leverages_;
}

NumericalPoint LinearModelResult::getDiagonalGramInverse() const
{
  return diagonalGramInverse_;
}

NumericalPoint LinearModelResult::getCookDistances() const
{
  return cookDistances_;
}

void LinearModelResult::computeStandardizedResiduals()
{
  NumericalPoint sigma2(sampleResiduals_.computeRawMoment(2));
  const UnsignedInteger n = sampleResiduals_.getSize();
  const UnsignedInteger pPlusOne = beta_.getDimension();
  const NumericalScalar factor = n * sigma2[0] / (n - pPlusOne);
  standardizedResiduals_ = NumericalSample(n, 1);
  for(UnsignedInteger i = 0; i < n; ++i)
  {
    standardizedResiduals_(i, 0) = sampleResiduals_(i, 0) / std::sqrt(factor * (1.0 - leverages_[i]));
  }
}

/* Method save() stores the object through the StorageManager */
void LinearModelResult::save(Advocate & adv) const
{
  MetaModelResult::save(adv);
  adv.saveAttribute( "inputSample_", inputSample_ );
  adv.saveAttribute( "basis_", basis_ );
  adv.saveAttribute( "design_", design_ );
  adv.saveAttribute( "outputSample_", outputSample_ );
  adv.saveAttribute( "beta_", beta_ );
  adv.saveAttribute( "condensedFormula_", condensedFormula_ );
  adv.saveAttribute( "coefficientsNames_", coefficientsNames_ );
  adv.saveAttribute( "standardizedResiduals_", standardizedResiduals_ );
  adv.saveAttribute( "leverages_", leverages_ );
  adv.saveAttribute( "cookDistances_", cookDistances_ );
}


/* Method load() reloads the object from the StorageManager */
void LinearModelResult::load(Advocate & adv)
{
  MetaModelResult::load(adv);
  adv.loadAttribute( "inputSample_", inputSample_ );
  adv.loadAttribute( "basis_", basis_ );
  adv.loadAttribute( "design_", design_ );
  adv.loadAttribute( "outputSample_", outputSample_ );
  adv.loadAttribute( "beta_", beta_ );
  adv.loadAttribute( "condensedFormula_", condensedFormula_ );
  adv.loadAttribute( "coefficientsNames_", coefficientsNames_ );
  adv.loadAttribute( "standardizedResiduals_", standardizedResiduals_ );
  adv.loadAttribute( "leverages_", leverages_ );
  adv.loadAttribute( "cookDistances_", cookDistances_ );
}


} /* namespace OTLMR */
