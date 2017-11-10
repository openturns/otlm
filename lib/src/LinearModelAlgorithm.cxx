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
#include <fstream>
#include "otlm/LinearModelAlgorithm.hxx"
#include "otlm/LinearModelStepwiseAlgorithm.hxx"
#include "openturns/SymbolicFunction.hxx"

using namespace OT;

namespace OTLM
{

CLASSNAMEINIT(LinearModelAlgorithm)

/* Default constructor */
LinearModelAlgorithm::LinearModelAlgorithm()
  : MetaModelAlgorithm()
  , inputSample_(0, 0)
  , outputSample_(0, 0)
  , result_()
  , hasRun_(false)
{
  // Nothing to do
}

/* Parameters constructor */
LinearModelAlgorithm::LinearModelAlgorithm(const Sample & inputSample,
    const Sample & outputSample)
  : MetaModelAlgorithm()
  , inputSample_(0, 0)
  , outputSample_(0, 0)
  , result_()
  , hasRun_(false)
{
  // Check the sample sizes
  if (inputSample.getSize() != outputSample.getSize())
    throw InvalidArgumentException(HERE) << "In LinearModelAlgorithm::LinearModelAlgorithm, input sample size (" << inputSample.getSize() << ") does not match output sample size (" << outputSample.getSize() << ").";
  // Set samples
  inputSample_ = inputSample;
  outputSample_ = outputSample;
  Collection<Function> functions;
  const Description inputDescription(inputSample_.getDescription());
  functions.add(SymbolicFunction(inputSample_.getDescription(), Description(1, "1")));
  for(UnsignedInteger i = 0; i < inputSample_.getDimension(); ++i)
  {
    functions.add(SymbolicFunction(inputSample_.getDescription(), Description(1, inputDescription[i])));
  }
  basis_ = Basis(functions);
}


/* Parameters constructor */
LinearModelAlgorithm::LinearModelAlgorithm(const Sample & inputSample,
    const Basis & basis,
    const Sample & outputSample)
  : MetaModelAlgorithm()
  , inputSample_(0, 0)
  , outputSample_(0, 0)
  , result_()
  , hasRun_(false)
{
  // Check the sample sizes
  if (inputSample.getSize() != outputSample.getSize())
    throw InvalidArgumentException(HERE) << "In LinearModelAlgorithm::LinearModelAlgorithm, input sample size (" << inputSample.getSize() << ") does not match output sample size (" << outputSample.getSize() << ").";
  // Set samples
  inputSample_ = inputSample;
  outputSample_ = outputSample;
  basis_ = basis;
}


/* Virtual constructor */
LinearModelAlgorithm * LinearModelAlgorithm::clone() const
{
  return new LinearModelAlgorithm(*this);
}


/* Perform regression */
void LinearModelAlgorithm::run()
{
  // Do not run again if already computed
  if (hasRun_) return;
  Indices indices(basis_.getSize());
  indices.fill();
  LinearModelStepwiseAlgorithm step(inputSample_, basis_, outputSample_, indices, true, 2.0, 0);
  result_ = step.getResult();
  hasRun_ = true;
}


/* String converter */
String LinearModelAlgorithm::__repr__() const
{
  OSS oss;
  oss << "class=" << getClassName()
      << ", inputSample=" << inputSample_
      << ", basis=" << basis_
      << ", outputSample=" << outputSample_
      << ", result=" << result_;
  return oss;
}


Sample LinearModelAlgorithm::getInputSample() const
{
  return inputSample_;
}


Basis LinearModelAlgorithm::getBasis() const
{
  return basis_;
}


Sample LinearModelAlgorithm::getOutputSample() const
{
  return outputSample_;
}


LinearModelResult LinearModelAlgorithm::getResult()
{
  run();
  return result_;
}


/* Method save() stores the object through the StorageManager */
void LinearModelAlgorithm::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "inputSample_", inputSample_ );
  adv.saveAttribute( "basis_", basis_ );
  adv.saveAttribute( "outputSample_", outputSample_ );
  adv.saveAttribute( "result_", result_ );
  adv.saveAttribute( "hasRun_", hasRun_ );
}

/* Method load() reloads the object from the StorageManager */
void LinearModelAlgorithm::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "inputSample_", inputSample_ );
  adv.loadAttribute( "basis_", basis_ );
  adv.loadAttribute( "outputSample_", outputSample_ );
  adv.loadAttribute( "result_", result_ );
  adv.loadAttribute( "hasRun_", hasRun_ );
}

} /* namespace OTLM */
