//                                               -*- C++ -*-
/**
 *  @brief Monomial factory
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
#ifndef OTLM_MONOMIALFACTORY_HXX
#define OTLM_MONOMIALFACTORY_HXX

#include "openturns/OrthogonalUniVariatePolynomialFactory.hxx"
#include "otlm/otlmprivate.hxx"

namespace OTLM
{

/**
 * @class MonomialFactory
 *
 * Monomial factory
 */

class OTLM_API MonomialFactory :
  public OT::OrthogonalUniVariatePolynomialFactory
{
  CLASSNAME

public:

  /** Default constructor */
  MonomialFactory();

  /** Virtual constructor */
  virtual MonomialFactory * clone() const;

  /** Calculate the coefficients of recurrence Pn+1(x) = x * Pn(x)  */
  Coefficients getRecurrenceCoefficients(const OT::UnsignedInteger n) const;

  /** String converter */
  OT::String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(OT::Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(OT::Advocate & adv);

private:

}; /* class MonomialFactory */

} /* namespace OTLM */

#endif /* OTLM_MONOMIALFACTORY_HXX */
