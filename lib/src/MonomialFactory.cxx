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
#include "otlmr/MonomialFactory.hxx"
#include "openturns/PersistentObjectFactory.hxx"
#include "openturns/Dirac.hxx"
#include "openturns/Exception.hxx"

using namespace OT;

namespace OTLMR
{

CLASSNAMEINIT(MonomialFactory);

static const Factory<MonomialFactory> Factory_MonomialFactory;


/* Default constructor */
MonomialFactory::MonomialFactory()
  : OrthogonalUniVariatePolynomialFactory(Dirac(0.0))
{
  initializeCache();
}


/* Virtual constructor */
MonomialFactory * MonomialFactory::clone() const
{
  return new MonomialFactory(*this);
}


MonomialFactory::Coefficients MonomialFactory::getRecurrenceCoefficients(const UnsignedInteger n) const
{
  Coefficients recurrenceCoefficients(3, 0.0);
  recurrenceCoefficients[0] = 1.0;
  return recurrenceCoefficients;
}


/* String converter */
String MonomialFactory::__repr__() const
{
  return OSS() << "class=" << getClassName()
         << " measure=" << measure_;
}


/* Method save() stores the object through the StorageManager */
void MonomialFactory::save(Advocate & adv) const
{
  OrthogonalUniVariatePolynomialFactory::save(adv);
}


/* Method load() reloads the object from the StorageManager */
void MonomialFactory::load(Advocate & adv)
{
  OrthogonalUniVariatePolynomialFactory::load(adv);
}

} /* namespace OTLMR */
