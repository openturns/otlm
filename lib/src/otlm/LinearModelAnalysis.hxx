//                                               -*- C++ -*-
/**
 *  @brief The linear model analysis
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
#ifndef OTLM_LINEARMODELANALYSIS_HXX
#define OTLM_LINEARMODELANALYSIS_HXX

#include "openturns/PersistentObject.hxx"
#include "openturns/Sample.hxx"
#include "openturns/Description.hxx"
#include "openturns/TestResult.hxx"
#include "openturns/Graph.hxx"
#include "otlm/LinearModelResult.hxx"
#include "otlm/otlmprivate.hxx"


namespace OTLM
{

/**
 * @class LinearModelAnalysis
 *
 * The linear model analysis
 */

class OTLM_API LinearModelAnalysis :
  public OT::PersistentObject
{
  CLASSNAME

public:

  /** Default constructor */
  LinearModelAnalysis();

  /** Parameter constructor */
  explicit LinearModelAnalysis(const LinearModelResult & linearModelResult);

  /** Virtual constructor */
  LinearModelAnalysis * clone() const;

  /** String converter */
  OT::String __repr__() const;

  /** Method that returns the ANOVA table (ANalyse Of VAriance) */
  OT::String __str__(const OT::String & offset = "") const;

  /** Linear model accessor */
  LinearModelResult getLinearModelResult() const;

  /** Accessors to data from ANOVA table */
  OT::String getFormula() const;
  OT::Description getCoefficientsNames() const;
  OT::Sample getResiduals() const;
  OT::Sample getStandardizedResiduals() const;
  OT::Sample getCoefficientsEstimates() const;
  OT::Sample getCoefficientsStandardErrors() const;
  OT::Sample getCoefficientsTScores() const;
  OT::Sample getCoefficientsPValues() const;

  /**  leverages */
  OT::Point getLeverages() const;

  /**  Cook's distances */
  OT::Point getCookDistances() const;

  /** Number of degrees of freedom */
  OT::UnsignedInteger getDegreesOfFreedom() const;

  /** R-squared */
  OT::Scalar getRSquared() const;

  /** Adjusted R-squared */
  OT::Scalar getAdjustedRSquared() const;

  /** Fisher test */
  OT::Scalar getFisherScore() const;
  OT::Scalar getFisherPValue() const;

  /** Kolmogorov-Smirnov normality test */
  OT::TestResult getNormalityTestResultKolmogorovSmirnov() const;

  /** Anderson-Darling normality test */
  OT::TestResult getNormalityTestResultAndersonDarling() const;

  /** Chi-Squared normality test */
  OT::TestResult getNormalityTestResultChiSquared() const;

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv);

  /** [1] Draw a plot of residuals versus fitted values  */
  OT::Graph drawResidualsVsFitted() const;

  /** [2] a Scale-Location plot of sqrt(| residuals |) versus fitted values */
  OT::Graph drawScaleLocation() const;

  /** [3] a Normal quantiles-quantiles plot of standardized residuals */
  OT::Graph drawQQplot() const;

  /** [4] a plot of Cook's distances versus row labels */
  OT::Graph drawCookDistance() const;

  /** [5] a plot of residuals versus leverages that adds bands corresponding to Cook's distances of 0.5 and 1. */
  OT::Graph drawResidualsVsLeverages() const;

  /** [6] a plot of Cook's distances versus leverage/(1-leverage) */
  OT::Graph drawCookVsLeverages() const;

private:

  /** linear model result */
  LinearModelResult linearModelResult_;

}; /* class LinearModelAnalysis */

} /* namespace OTLM */

#endif /* OTLM_LINEARMODELANALYSIS_HXX */
