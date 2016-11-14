// SWIG file MonomialFactory.i

%{
#include "otlm/MonomialFactory.hxx"
%}

%include MonomialFactory_doc.i

%include otlm/MonomialFactory.hxx
namespace OTLM { %extend MonomialFactory { MonomialFactory(const MonomialFactory & other) { return new OTLM::MonomialFactory(other); } } }
