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
#include "otlm/LinearModelAnalysis.hxx"
#include "openturns/PersistentObjectFactory.hxx"
#include "openturns/NormalityTest.hxx"
#include "openturns/OSS.hxx"
#include "openturns/ResourceMap.hxx"
#include "openturns/Cloud.hxx"
#include "openturns/Curve.hxx"
#include "openturns/Text.hxx"
#include "openturns/Normal.hxx"
#include "openturns/DistFunc.hxx"
#include "openturns/LinearModelTest.hxx"
#include "openturns/FittingTest.hxx"
#include "openturns/HypothesisTest.hxx"
#include "openturns/FisherSnedecor.hxx"
#include "openturns/ChiSquare.hxx"
#include "openturns/UserDefined.hxx"

using namespace OT;

namespace OTLM
{

CLASSNAMEINIT(LinearModelAnalysis);

static const Factory<LinearModelAnalysis> Factory_LinearModelAnalysis;

/* Default constructor */
LinearModelAnalysis::LinearModelAnalysis()
  : PersistentObject()
{
  // Nothing to do
}

/* Parameter constructor */
LinearModelAnalysis::LinearModelAnalysis(const LinearModelResult & linearModelResult)
  : PersistentObject()
  , linearModelResult_(linearModelResult)
{
  // Nothing to do
}

/* Virtual constructor */
LinearModelAnalysis * LinearModelAnalysis::clone() const
{
  return new LinearModelAnalysis(*this);
}


/* String converter */
String LinearModelAnalysis::__repr__() const
{
  return OSS(true)
         << "class=" << getClassName()
         << ", linearModelResult=" << linearModelResult_;
}


/* Method that returns the ANOVA table (ANalyse Of VAriance) */
String LinearModelAnalysis::__str__(const String & offset) const
{
  const NumericalSample estimates(getCoefficientsEstimates());
  const NumericalSample standardErrors(getCoefficientsStandardErrors());
  const NumericalSample tscores(getCoefficientsTScores());
  const NumericalSample pValues(getCoefficientsPValues());
  const Description names(getCoefficientsNames());
  const NumericalScalar sigma2 = getResiduals().computeRawMoment(2)[0];
  const UnsignedInteger dof = getDegreesOfFreedom();
  const UnsignedInteger n = getResiduals().getSize();
  const String separator(" | ");
  const String separatorEndLine(" |");
  size_t twidth = 0; // column title max width
  size_t lwidth = 0; // LHS number max width
  size_t awidth = 0;
  String st;

  OSS oss(true);
  oss.setPrecision(5);
  for (UnsignedInteger i = 0; i < names.getSize(); ++i)
  {
    twidth = std::max( twidth, names[i].size() );
    st = OSS() << estimates[i][0];
    lwidth = std::max( lwidth, st.size() );
    st = OSS() << standardErrors[i][0];
    lwidth = std::max( lwidth, st.size() );
    st = OSS() << tscores[i][0];
    lwidth = std::max( lwidth, st.size() );
    st = OSS() << pValues[i][0];
    lwidth = std::max( lwidth, st.size() );
  }
  awidth = twidth + 5 * separator.size() + 4 * lwidth - 1;

  oss << getFormula() ;
  oss << "\n\nCoefficients:\n"  ;
  oss <<  String( twidth , ' ' ) << separator;
  st = "Estimate";
  oss << st << String( lwidth - st.size(),' ') << separator;
  st = "Std Error";
  oss << st << String( lwidth - st.size(),' ') << separator;
  st = "t value";
  oss << st << String( lwidth - st.size(),' ') << separator;
  st = "Pr(>|t|)";
  oss << st << String( lwidth - st.size(),' ') << separatorEndLine;
  oss << "\n"<<  String( awidth , '-' )<<"\n";
  for (UnsignedInteger i = 0; i < pValues.getSize(); ++i)
  {
    st = names[i];
    oss << st << String( twidth - st.size(), ' ') << separator;
    st = OSS() << estimates[i][0];
    oss << st << String( lwidth - st.size(), ' ') << separator;
    st = OSS() << standardErrors[i][0];
    oss << st << String( lwidth - st.size(), ' ') << separator;
    st = OSS() << tscores[i][0];
    oss << st << String( lwidth - st.size(), ' ') << separator;
    st = OSS() << pValues[i][0];
    oss << st << String( lwidth - st.size(), ' ') << separatorEndLine;
    oss << "\n";
  }
  oss << String( awidth , '-' )<<"\n\n";
  oss << "Residual standard error: "<<  std::sqrt(sigma2 * n /dof)  <<" on "<< dof <<" degrees of freedom\n";
  oss << "F-statistic: " << getFisherScore() << " , " << " p-value: " <<  getFisherPValue() << "\n";

  //  R-squared & Adjusted R-squared tests
  lwidth=0;
  twidth=20;
  const NumericalScalar test1(getRSquared());
  const NumericalScalar test2(getAdjustedRSquared());
  st = OSS() << test1;
  lwidth = std::max( lwidth, st.size() );
  st = OSS() << test2;
  lwidth = std::max( lwidth, st.size() );
  awidth = twidth+2*separator.size()+lwidth-1;
  oss << String( awidth , '-' )<<"\n";
  st = "Multiple R-squared";
  oss << st << String( twidth - st.size(),' ') << separator;
  st = OSS() << test1;
  oss << st << String( lwidth - st.size(),' ') << separatorEndLine;
  oss << "\n";
  st = "Adjusted R-squared";
  oss << st << String( twidth - st.size(),' ') << separator;
  st = OSS() << test2;
  oss << st << String( lwidth - st.size(),' ') << separatorEndLine;
  oss <<"\n"<< String( awidth , '-' )<<"\n";

  // normality tests
  lwidth=0;
  twidth=20;
  const NumericalScalar normalitytest1(getNormalityTestResultAndersonDarling().getPValue());
  const NumericalScalar normalitytest2(getNormalityTestResultChiSquared().getPValue());
  const NumericalScalar normalitytest3(getNormalityTestResultKolmogorovSmirnov().getPValue());
  st = OSS() << normalitytest1;
  lwidth = std::max( lwidth, st.size() );
  st = OSS() << normalitytest2;
  lwidth = std::max( lwidth, st.size() );
  st = OSS() << normalitytest3;
  lwidth = std::max( lwidth, st.size() );
  awidth = twidth+2*separator.size()+lwidth-1;
  oss << "\n"<<  String( awidth , '-' )<<"\n";
  st = "Normality test";
  oss << st << String( twidth - st.size(),' ') << separator;
  st = "p-value";
  oss << st << String( lwidth - st.size(),' ') << separatorEndLine;
  oss << "\n"<<  String( awidth , '-' )<<"\n";
  st = "Anderson-Darling";
  oss << st << String( twidth - st.size(),' ') << separator;
  st = OSS() << normalitytest1;
  oss << st << String( lwidth - st.size(),' ') << separatorEndLine;
  oss << "\n";
  st = "Chi-Squared";
  oss << st << String( twidth - st.size(),' ') << separator;
  st = OSS() << normalitytest2;
  oss << st << String( lwidth - st.size(),' ') << separatorEndLine;
  oss << "\n";
  st = "Kolmogorov-Smirnov";
  oss << st << String( twidth - st.size(),' ') << separator;
  st = OSS() << normalitytest3;
  oss << st << String( lwidth - st.size(),' ') << separatorEndLine;
  oss << "\n"<<  String( awidth , '-' )<<"\n";
  return oss;
}


/* Linear model accessor */
LinearModelResult LinearModelAnalysis::getLinearModelResult() const
{
  return linearModelResult_;
}

String LinearModelAnalysis::getFormula() const
{
  return linearModelResult_.getFormula();
}

Description LinearModelAnalysis::getCoefficientsNames() const
{
  return linearModelResult_.getCoefficientsNames();
}

NumericalSample LinearModelAnalysis::getResiduals() const
{
  return linearModelResult_.getSampleResiduals();
}

NumericalSample LinearModelAnalysis::getStandardizedResiduals() const
{
  return linearModelResult_.getStandardizedResiduals();
}

NumericalSample LinearModelAnalysis::getCoefficientsEstimates() const
{
  NumericalPoint beta(linearModelResult_.getTrendCoefficients());
  NumericalSample result(beta.getDimension(), 1);
  for (UnsignedInteger i = 0; i < beta.getDimension(); ++i)
  {
    result(i, 0) = beta[i];
  }
  return result;
}

NumericalSample LinearModelAnalysis::getCoefficientsStandardErrors() const
{
  const NumericalScalar sigma2(getResiduals().computeRawMoment(2)[0]);
  const UnsignedInteger n = getResiduals().getSize();
  const UnsignedInteger pPlusOne =  linearModelResult_.getCoefficientsNames().getSize();
  const NumericalScalar factor = n * sigma2 / (n - pPlusOne);
  const NumericalPoint diagGramInv(linearModelResult_.getDiagonalGramInverse());
  const UnsignedInteger p = diagGramInv.getSize();
  NumericalSample standardErrors(p, 1);
  for (UnsignedInteger i = 0; i < standardErrors.getSize(); ++i)
  {
    standardErrors(i, 0) = std::sqrt(std::abs(factor * diagGramInv[i]));
  }
  return standardErrors;
}

NumericalSample LinearModelAnalysis::getCoefficientsTScores() const
{
  // The coefficients of linear expansion over their standard error
  const NumericalSample estimates(getCoefficientsEstimates());
  const NumericalSample standardErrors(getCoefficientsStandardErrors());
  NumericalSample tScores(estimates.getSize(), 1);
  for (UnsignedInteger i = 0; i < tScores.getSize(); ++i)
  {
    tScores(i, 0) = estimates(i, 0) / standardErrors(i, 0);
  }
  return tScores;
}

NumericalSample LinearModelAnalysis::getCoefficientsPValues() const
{
  // Interest is Pr(X > |t|) with t the statistic defined as
  // t: = beta / std_dev(beta)
  // t ~ Student(dof)
  // Pr(X > |t|) = Pr(X > sign(t)*t) + Pr(X < -sign(t)*t)
  //             = 2 * Pr(X > sign(t)*t) as Student distribution is symmetric
  //             = 2 * Pr(X > |t|)
  const NumericalSample tScores(getCoefficientsTScores());
  const UnsignedInteger dof = getDegreesOfFreedom();
  NumericalSample pValues(tScores.getSize(), 1);
  for (UnsignedInteger i = 0; i < pValues.getSize(); ++i)
  {
    // true flag define the complementary CDF, ie P(X > t)
    pValues(i, 0) = 2.0 * DistFunc::pStudent(dof, std::abs(tScores(i, 0)), true);
  }
  return pValues;
}

/* Leverages */
NumericalPoint LinearModelAnalysis::getLeverages() const
{
  return linearModelResult_.getLeverages();
}

/* Cook's distances */
NumericalPoint LinearModelAnalysis::getCookDistances() const
{
  return linearModelResult_.getCookDistances();
}

/* Number of degrees of freedom */
UnsignedInteger LinearModelAnalysis::getDegreesOfFreedom() const
{
  const UnsignedInteger n = linearModelResult_.getLeverages().getSize();
  const UnsignedInteger pPlusOne = linearModelResult_.getCoefficientsNames().getSize();
  return n - pPlusOne;
}

/* R-squared test */
NumericalScalar LinearModelAnalysis::getRSquared() const
{
  // Get residuals and output samples
  const NumericalSample residuals(getResiduals());
  const NumericalSample outputSample =(getLinearModelResult().getOutputSample());
  // Define RSS and SYY
  const NumericalScalar RSS = residuals.computeRawMoment(2)[0];
  const NumericalScalar SYY = outputSample.computeCenteredMoment(2)[0];
  const NumericalScalar rSquared = 1.0 - RSS / SYY;
  return rSquared;
}

/* Adjusted R-squared test */
NumericalScalar LinearModelAnalysis::getAdjustedRSquared() const
{
  const UnsignedInteger dof = getDegreesOfFreedom();
  const UnsignedInteger n   = getResiduals().getSize();
  const NumericalScalar R2  = getRSquared();
  return 1.0 - (1.0 - R2) * (n - 1) / dof;
}

/* Fisher test */
NumericalScalar LinearModelAnalysis::getFisherScore() const
{
  // Get residuals and output samples
  const NumericalSample residuals(getResiduals());
  const NumericalSample outputSample =(getLinearModelResult().getOutputSample());
  const UnsignedInteger size = residuals.getSize();
  // Get the number of parameter p
  const UnsignedInteger p = getCoefficientsEstimates().getSize();
  // Define RSS and SYY
  const NumericalScalar RSS = residuals.computeRawMoment(2)[0] * size;
  const NumericalScalar SYY = outputSample.computeCenteredMoment(2)[0] * size;
  const NumericalScalar FStatistic = ((SYY - RSS) / (p - 1)) / (RSS / (size - p));
  return FStatistic;
}

NumericalScalar LinearModelAnalysis::getFisherPValue() const
{
  // size and number of parameters
  const UnsignedInteger size = getResiduals().getSize();
  // Get the number of parameter p
  const UnsignedInteger p = getCoefficientsEstimates().getSize();
  const NumericalScalar FStatistic = getFisherScore();
  return FisherSnedecor(p - 1, size - 1).computeComplementaryCDF(FStatistic);
}

/* Kolmogorov-Smirnov normality test */
TestResult LinearModelAnalysis::getNormalityTestResultKolmogorovSmirnov() const
{
  // We check that residuals are centered with variance = sigma2
  const NumericalSample residuals(getResiduals());
  // Compute Sigma2
  const NumericalScalar sigma2(residuals.computeRawMoment(2)[0]);
  const Normal dist(0.0, std::sqrt(sigma2));
  return FittingTest::Kolmogorov(residuals, dist);
}

/* Anderson-Darling normality test */
TestResult LinearModelAnalysis::getNormalityTestResultAndersonDarling() const
{
  return NormalityTest::AndersonDarlingNormal(getResiduals());
}

/* Chi-Squared normality test */
TestResult LinearModelAnalysis::getNormalityTestResultChiSquared() const
{
  // Chi-Square test for normality (https://en.wikipedia.org/wiki/Pearson's_chi-squared_test#Test_for_fit_of_a_distribution)
  // See also Thode Jr., H.C. (2002): Testing for  Normality. Marcel Dekker, New York
  // Moore, D.S. (1986): Tests of the chi-squared type. In: D'Agostino, R.B. and Stephens, M.A., eds.: Goodness-of-Fit Techniques. Marcel Dekker, New York.
  // From residuals :
  // 1) Define nrClasses := 2 k^{\frac{2}{5}} (k=residual.getSize())
  // 2) Replace residuals per their CDF (under gaussian hypothesis)
  // 3) Split [0,1] onto nrClasses
  // 4) Count CDF per class ==> C_i
  // 5) Under gaussian assumption, equiprobability fixes the expected count E_i ==> E_i = E = k / nrClasses
  // 6) Compute the Pearson statistic P=\sum_{i=1}^{nClasses} \frac{C_{i} - E_{i})^{2}}{E_{i}}
  // 7) Finally compute the pValue as ChiSquared(nrClasses-3).computeComplementaryCDF(P)
  const UnsignedInteger csAdj = ResourceMap::GetAsBool("LinearModelAnalysis-ChiSquareAdjust") ? 2 : 0 ;
  const UnsignedInteger size = getResiduals().getSize();
  const UnsignedInteger nrClasses = static_cast<int>(std::ceil(2.0 * std::pow(size, 2.0 / 5)));
  // Transform data into "uniform" data using CDF
  const Normal normalDistribution(getResiduals().computeMean()[0], getResiduals().computeStandardDeviation()(0,0));
  // Define the cdf values for the class
  const NumericalSample cdfValues(normalDistribution.computeCDF(getResiduals()));
  // Define the classes
  // Define the width of each class
  const NumericalScalar widthClass = 1.0 / nrClasses;
  // To define the table, we use a UserDefined distribution
  const UserDefined userDefinedCDF(cdfValues);
  // Count elements per class
  NumericalPoint count(nrClasses, 0.0);
  NumericalPoint lowerBound(1);
  NumericalPoint upperBound(1);
  for (UnsignedInteger k = 0; k < nrClasses; ++ k)
  {
    lowerBound[0] = k * widthClass;
    upperBound[0] = (k + 1) * widthClass;
    const Interval interval(lowerBound, upperBound);
    // Compute number of elements in the interval
    // Number of elements = total_number * Probability
    count[k] = userDefinedCDF.computeProbability(interval) * size;
  }
  // Define the number of elements per class if independent
  const NumericalScalar theoriticalComponents =  size * 1.0 / nrClasses;
  // Elements of statistic
  NumericalScalar statistic = 0.0;
  for (UnsignedInteger k = 0; k < nrClasses; ++ k)
  {
    statistic += std::pow(count[k] - theoriticalComponents, 2) / theoriticalComponents;
  }
  // Statistic follows a ChiSquare distribution with nrClasses - 3 dof (mu/sigma uknowns)
  const NumericalScalar pValue = ChiSquare(nrClasses - 1 - csAdj).computeComplementaryCDF(statistic);
  // TestResult output
  return TestResult("ChiSquareNormality", true, pValue, 0.01);
}

/* [1] Draw a plot of residuals versus fitted values */
Graph LinearModelAnalysis::drawResidualsVsFitted() const
{
  const NumericalSample inputData(linearModelResult_.getInputSample());
  const NumericalMathFunction metamodel(linearModelResult_.getMetaModel());
  const NumericalSample fitted(metamodel(inputData));
  const NumericalSample residuals(getResiduals());
  const UnsignedInteger size(fitted.getSize());
  NumericalSample dataFull(fitted);
  dataFull.stack(residuals);
  Graph graph("Residuals vs Fitted", "Fitted values", "Residuals", true, "topright");
  Cloud cloud(dataFull, "black", "fcircle");
  graph.add(cloud);
  // Add point identifiers for worst residuals
  UnsignedInteger identifiers(ResourceMap::GetAsUnsignedInteger("LinearModelAnalysis-Identifiers"));
  if (identifiers > 0)
  {
    if (identifiers > size)
      identifiers = size;
    Description annotations(size);
    NumericalSample dataWithIndex(size, 2);
    for(UnsignedInteger i = 0; i < size; ++i)
    {
      dataWithIndex(i, 0) = std::abs(residuals(i, 0));
      dataWithIndex(i, 1) = i;
    }
    const NumericalSample sortedData(dataWithIndex.sortAccordingToAComponent(0));
    Description positions(size, "top");
    for(UnsignedInteger i = 0; i < identifiers; ++i)
    {
      const UnsignedInteger index = sortedData(size - 1 - i, 1);
      annotations[index] = (OSS() << index + 1);
      if (residuals(index, 0) < 0.0)
        positions[index] = "top";
      else
        positions[index] = "bottom";
    }
    Text text(dataFull, annotations, "bottom");
    text.setColor("red");
    text.setTextPositions(positions);
    graph.add(text);
  }
  // Adapt the margins
  NumericalPoint boundingBox(graph.getBoundingBox());
  NumericalScalar width = boundingBox[1] - boundingBox[0];
  NumericalScalar height = boundingBox[3] - boundingBox[2];
  boundingBox[0] -= 0.1 * width;
  boundingBox[1] += 0.1 * width;
  boundingBox[2] -= 0.1 * height;
  boundingBox[3] += 0.1 * height;
  graph.setBoundingBox(boundingBox);
  return graph;
}

/* [2] a Scale-Location plot of sqrt(| residuals |) versus fitted values */
Graph LinearModelAnalysis::drawScaleLocation() const
{
  const NumericalSample inputData(linearModelResult_.getInputSample());
  const NumericalMathFunction metamodel(linearModelResult_.getMetaModel());
  const NumericalSample fitted(metamodel(inputData));
  const NumericalSample stdresiduals(getStandardizedResiduals());
  const UnsignedInteger size(fitted.getSize());
  NumericalSample dataFull(fitted);
  NumericalSample sqrtstdresiduals(size,1);
  for(UnsignedInteger i = 0; i < size; ++i)
  {
    sqrtstdresiduals(i, 0) = std::sqrt(std::abs(stdresiduals(i, 0)));
  }
  dataFull.stack(sqrtstdresiduals);
  Graph graph("Scale-Location", "Fitted values", "|Std. residuals|^0.5", true, "topright");
  Cloud cloud(dataFull, "black", "fcircle");
  graph.add(cloud);
  // Add point identifiers for worst standardized residuals
  UnsignedInteger identifiers(ResourceMap::GetAsUnsignedInteger("LinearModelAnalysis-Identifiers"));
  if (identifiers > 0)
  {
    if (identifiers > size)
      identifiers = size;
    Description annotations(size);
    NumericalSample dataWithIndex(size, 2);
    for(UnsignedInteger i = 0; i < size; ++i)
    {
      dataWithIndex(i, 0) = std::abs(stdresiduals(i, 0));
      dataWithIndex(i, 1) = i;
    }
    const NumericalSample sortedData(dataWithIndex.sortAccordingToAComponent(0));
    Description positions(size, "top");
    for(UnsignedInteger i = 0; i < identifiers; ++i)
    {
      const UnsignedInteger index = sortedData(size - 1 - i, 1);
      annotations[index] = (OSS() << index + 1);
      if (stdresiduals(index, 0) < 0.0)
        positions[index] = "top";
      else
        positions[index] = "bottom";
    }
    Text text(dataFull, annotations, "bottom");
    text.setColor("red");
    text.setTextPositions(positions);
    graph.add(text);
  }
  // Adapt the margins
  NumericalPoint boundingBox(graph.getBoundingBox());
  NumericalScalar width = boundingBox[1] - boundingBox[0];
  NumericalScalar height = boundingBox[3] - boundingBox[2];
  boundingBox[0] -= 0.1 * width;
  boundingBox[1] += 0.1 * width;
  boundingBox[2] -= 0.1 * height;
  boundingBox[3] += 0.1 * height;
  graph.setBoundingBox(boundingBox);
  return graph;
}

/* [3] a Normal quantiles-quantiles plot of standardized residuals */
Graph LinearModelAnalysis::drawQQplot() const
{
  const NumericalSample stdresiduals(getStandardizedResiduals());
  const UnsignedInteger size(stdresiduals.getSize());
  const Normal distribution(1); // Standart normal distribution
  const NumericalSample sortedSample(stdresiduals.sort(0));
  NumericalSample dataFull(size, 2);
  const NumericalScalar step = 1.0 / size;
  for (UnsignedInteger i = 0; i < size; ++i)
  {
    dataFull[i][1] = sortedSample[i][0];
    dataFull[i][0] = distribution.computeQuantile((i + 0.5) * step)[0];
  }
  Graph graph("Normal Q-Q", "Theoretical Quantiles", "Std. residuals", true, "topright");
  Cloud cloud(dataFull, "black", "fcircle");
  graph.add(cloud);
  // Add point identifiers for worst standardized residuals
  UnsignedInteger identifiers(ResourceMap::GetAsUnsignedInteger("LinearModelAnalysis-Identifiers"));
  if (identifiers > 0)
  {
    if (identifiers > size)
      identifiers = size;
    Description annotations(size);
    NumericalSample dataWithIndex1(size, 2);
    NumericalSample dataWithIndex2(size, 2);
    for(UnsignedInteger i = 0; i < size; ++i)
    {
      dataWithIndex1(i, 0) = std::abs(dataFull(i, 1));
      dataWithIndex1(i, 1) = i;
      dataWithIndex2(i, 0) = std::abs(stdresiduals(i, 0));
      dataWithIndex2(i, 1) = i;

    }
    const NumericalSample sortedData1(dataWithIndex1.sortAccordingToAComponent(0));
    const NumericalSample sortedData2(dataWithIndex2.sortAccordingToAComponent(0));
    Description positions(size, "top");
    for(UnsignedInteger i = 0; i < identifiers; ++i)
    {
      const UnsignedInteger index1 = sortedData1(size - 1 - i, 1);
      const UnsignedInteger index2 = sortedData2(size - 1 - i, 1);
      annotations[index1] = (OSS() << index2 + 1);
      if (dataFull(index1, 1) < 0.0)
        positions[index1] = "top";
      else
        positions[index1] = "bottom";
    }
    Text text(dataFull, annotations, "bottom");
    text.setColor("red");
    text.setTextPositions(positions);
    graph.add(text);
  }
  // add line to a normal QQ plot which passes through the first and third quartiles
  NumericalSample diagonal(2, 2);
  NumericalPoint point(2);
  const UnsignedInteger id1Q = 0.25*size-0.5;
  const UnsignedInteger id3Q = 0.75*size-0.5;
  point[0] = (dataFull[id3Q][1]-dataFull[id1Q][1])/(dataFull[id3Q][0]-dataFull[id1Q][0]);
  point[1] = dataFull[id3Q][1]-point[0]* dataFull[id3Q][0];
  diagonal[0][0] = dataFull[0][0];
  diagonal[0][1] = dataFull[0][0]*point[0]+point[1];
  diagonal[1][0] = dataFull[size-1][0];
  diagonal[1][1] = dataFull[size-1][0]*point[0]+point[1];
  Curve curve(diagonal, "red", "dashed", 2);
  graph.add(curve);
  // Adapt the margins
  NumericalPoint boundingBox(graph.getBoundingBox());
  NumericalScalar width = boundingBox[1] - boundingBox[0];
  NumericalScalar height = boundingBox[3] - boundingBox[2];
  boundingBox[0] -= 0.1 * width;
  boundingBox[1] += 0.1 * width;
  boundingBox[2] -= 0.1 * height;
  boundingBox[3] += 0.1 * height;
  graph.setBoundingBox(boundingBox);
  return graph;
}

/* [4] a plot of Cook's distances versus row labels */
Graph LinearModelAnalysis::drawCookDistance() const
{
  const NumericalPoint cookdistances(getCookDistances());
  const UnsignedInteger size(cookdistances.getSize());
  // Add point identifiers for worst Cook's distance
  UnsignedInteger identifiers(ResourceMap::GetAsUnsignedInteger("LinearModelAnalysis-Identifiers"));
  Description annotations(size);
  if (identifiers > 0)
  {
    if (identifiers > size)
      identifiers = size;
    NumericalSample dataWithIndex(size, 2);
    for(UnsignedInteger i = 0; i < size; ++i)
    {
      dataWithIndex(i, 0) = cookdistances[i];
      dataWithIndex(i, 1) = i;
    }
    const NumericalSample sortedData(dataWithIndex.sortAccordingToAComponent(0));
    for(UnsignedInteger i = 0; i < identifiers; ++i)
    {
      const UnsignedInteger index = sortedData(size - 1 - i, 1);
      annotations[index] = (OSS() << index + 1);
    }
  }
  Graph graph("Cook's distance", "Obs. number", "Cook's distance", true, "topright");
  for (UnsignedInteger i = 0; i < size; ++i)
  {
    NumericalSample dataFull(2, 2);
    dataFull(0, 0) = i+1;
    dataFull(0, 1) = 0.0;
    dataFull(1, 0) = i+1;
    dataFull(1, 1) = cookdistances[i];
    Curve curve(dataFull, "black", "solid", 2);
    graph.add(curve);
    if (annotations[i] != "")
    {
      Description desc(2);
      desc[1]=annotations[i];
      Text text(dataFull, desc, "top");
      text.setColor("red");
      graph.add(text);
    }
  }
  // Adapt the margins
  NumericalPoint boundingBox(graph.getBoundingBox());
  NumericalScalar height = boundingBox[3] - boundingBox[2];
  boundingBox[2] -= 0.1 * height;
  boundingBox[3] += 0.1 * height;
  graph.setBoundingBox(boundingBox);
  return graph;
}

/* [5] a plot of residuals versus leverages that adds bands corresponding to Cook's distances of 0.5 and 1. */
Graph LinearModelAnalysis::drawResidualsVsLeverages() const
{
  const NumericalPoint cookdistances(getCookDistances());
  const NumericalPoint leverages(getLeverages());
  const NumericalSample stdresiduals(getStandardizedResiduals());
  const UnsignedInteger size(stdresiduals.getSize());
  NumericalSample leveragesS(size, 1);
  for (UnsignedInteger i = 0; i < size; ++i)
  {
   leveragesS(i, 0) = leverages[i];
  }
  NumericalSample dataFull(leveragesS);
  dataFull.stack(stdresiduals);
  Graph graph("Residuals vs Leverage", "Leverage", "Std. residuals", true, "topright");
  Cloud cloud(dataFull, "black", "fcircle");
  graph.add(cloud);
  // Add point identifiers for worst Cook's distance
  UnsignedInteger identifiers(ResourceMap::GetAsUnsignedInteger("LinearModelAnalysis-Identifiers"));
  if (identifiers > 0)
  {
    if (identifiers > size)
      identifiers = size;
    Description annotations(size);
    NumericalSample dataWithIndex(size, 2);
    for(UnsignedInteger i = 0; i < size; ++i)
    {
      dataWithIndex(i, 0) = cookdistances[i];
      dataWithIndex(i, 1) = i;
    }
    const NumericalSample sortedData(dataWithIndex.sortAccordingToAComponent(0));
    Description positions(size, "top");
    for(UnsignedInteger i = 0; i < identifiers; ++i)
    {
      const UnsignedInteger index = sortedData(size - 1 - i, 1);
      annotations[index] = (OSS() << index + 1);
      if (cookdistances[index] < 0.0)
        positions[index] = "top";
      else
        positions[index] = "bottom";
    }
    Text text(dataFull, annotations, "bottom");
    text.setColor("red");
    text.setTextPositions(positions);
    graph.add(text);
  }
  // Adapt the margins
  NumericalPoint boundingBox(graph.getBoundingBox());
  NumericalScalar width = boundingBox[1] - boundingBox[0];
  NumericalScalar height = boundingBox[3] - boundingBox[2];
  boundingBox[0] -= 0.1 * width;
  boundingBox[1] += 0.1 * width;
  boundingBox[2] -= 0.1 * height;
  boundingBox[3] += 0.1 * height;
  graph.setBoundingBox(boundingBox);
  // Add contour plot of Cook's distance
  const UnsignedInteger pPlusOne = linearModelResult_.getCoefficientsNames().getSize();
  const UnsignedInteger step = 100;
  NumericalPoint isovalues(2);
  isovalues[0]=0.5;
  isovalues[1]=1.0;
  NumericalScalar ptx;
  Description annotation(2);
  NumericalSample diagonal1(2,2);
  NumericalSample diagonal2(2,2);
  width = boundingBox[1] - boundingBox[0];
  for(UnsignedInteger k = 0; k < isovalues.getSize(); ++k)
  {
    for(UnsignedInteger i = 0; i < step-1; ++i)
    {
      ptx = boundingBox[0] + i*(width)/step;
      diagonal1[0][0] = ptx;
      diagonal2[0][0] = ptx;
      diagonal1[0][1] =  std::sqrt(std::abs(isovalues[k]*pPlusOne*(1.0-ptx)/ptx));
      diagonal2[0][1] = -diagonal1[0][1];
      ptx = boundingBox[0] + (i+1)*(width)/step;
      diagonal1[1][0] = ptx;
      diagonal2[1][0] = ptx;
      diagonal1[1][1] =  std::sqrt(std::abs(isovalues[k]*pPlusOne*(1.0-ptx)/ptx));
      diagonal2[1][1] = -diagonal1[1][1];
      Curve curve1(diagonal1, "red", "solid", 1);
      Curve curve2(diagonal2, "red", "solid", 1);
      graph.add(curve1);
      graph.add(curve2);
    }
    annotation[0] = (OSS() << isovalues[k]);
    Cloud cloud1(diagonal1, "red", "dot");
    graph.add(cloud1);
    Text text1(diagonal1, annotation, "top");
    text1.setColor("red");
    graph.add(text1);
    Cloud cloud2(diagonal2, "red", "dot");
    graph.add(cloud2);
    Text text2(diagonal2, annotation, "bottom");
    text2.setColor("red");
    graph.add(text2);
  }
  Curve curve1(diagonal1, "red", "solid", 1);
  curve1.setLegend("Cook's distance");
  graph.add(curve1);
  return graph;
}

/* [6] a plot of Cook's distances versus leverage/(1-leverage) */
Graph LinearModelAnalysis::drawCookVsLeverages() const
{
  const NumericalPoint leverages(getLeverages());
  const NumericalPoint cookdistances(getCookDistances());
  const UnsignedInteger size(cookdistances.getSize());
  NumericalSample dataFull(size, 2);
  for (UnsignedInteger i = 0; i < size; ++i)
  {
   dataFull(i, 0) = leverages[i] / (1.0 - leverages[i]);
   dataFull(i, 1) = cookdistances[i];
  }
  Graph graph("Cook's dist vs Leverage h[ii]/(1-h[ii])", "Leverage h[ii]/(1-h[ii])", "Cook's distance", true, "topright");
  Cloud cloud(dataFull, "black", "fcircle");
  graph.add(cloud);
  // Add point identifiers for worst Cook's distance
  UnsignedInteger identifiers(ResourceMap::GetAsUnsignedInteger("LinearModelAnalysis-Identifiers"));
  if (identifiers > 0)
  {
    if (identifiers > size)
      identifiers = size;
    Description annotations(size);
    NumericalSample dataWithIndex(size, 2);
    for(UnsignedInteger i = 0; i < size; ++i)
    {
      dataWithIndex(i, 0) = std::abs(cookdistances[i]);
      dataWithIndex(i, 1) = i;
    }
    const NumericalSample sortedData(dataWithIndex.sortAccordingToAComponent(0));
    Description positions(size, "top");
    for(UnsignedInteger i = 0; i < identifiers; ++i)
    {
      const UnsignedInteger index = sortedData(size - 1 - i, 1);
      annotations[index] = (OSS() << index + 1);
      if (cookdistances[index] < 0.0)
        positions[index] = "top";
      else
        positions[index] = "bottom";
    }
    Text text(dataFull, annotations, "bottom");
    text.setColor("red");
    text.setTextPositions(positions);
    graph.add(text);
  }
  // Adapt the margins
  NumericalPoint boundingBox(graph.getBoundingBox());
  NumericalScalar width = boundingBox[1] - boundingBox[0];
  NumericalScalar height = boundingBox[3] - boundingBox[2];
  boundingBox[0] -= 0.1 * width;
  boundingBox[1] += 0.1 * width;
  boundingBox[2] -= 0.1 * height;
  boundingBox[3] += 0.1 * height;
  graph.setBoundingBox(boundingBox);
  // Add contour plot
  NumericalPoint isovalues(6);
  isovalues[0]=0.5;
  isovalues[1]=1.0;
  isovalues[2]=1.5;
  isovalues[3]=2.0;
  isovalues[4]=2.5;
  isovalues[5]=3.0;
  NumericalPoint Pt(2);
  Description annotation(2);
  NumericalSample diagonal(2,2);
  diagonal[0][0] = 0.0;
  diagonal[0][1] = 0.0;
  for(UnsignedInteger k = 0; k < isovalues.getSize(); ++k)
  {
    NumericalScalar coeff=isovalues[k]*isovalues[k];
    Pt[0] = boundingBox[3]/coeff;
    Pt[1] = boundingBox[1]*coeff;
    if (Pt[1] < boundingBox[3]){
      diagonal[1][0] = boundingBox[1];
      diagonal[1][1] = Pt[1];
    }
    if (Pt[0] < boundingBox[1]){
      diagonal[1][0] = Pt[0];
      diagonal[1][1] = boundingBox[3];
    }
    Curve curve(diagonal, "red", "solid", 1);
    graph.add(curve);
    annotation[1] = (OSS() << isovalues[k]);
    Cloud diagonalCloud(diagonal, "red", "dot");
    graph.add(diagonalCloud);
    Text text(diagonal, annotation, "top");
    text.setColor("red");
    graph.add(text);
  }
  return graph;
}

/* Method save() stores the object through the StorageManager */
void LinearModelAnalysis::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "linearModelResult_", linearModelResult_ );
}


/* Method load() reloads the object from the StorageManager */
void LinearModelAnalysis::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "linearModelResult_", linearModelResult_ );
}

} /* namespace OTLM */
