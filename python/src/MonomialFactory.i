// SWIG file MonomialFactory.i

%{
#include "otlmr/MonomialFactory.hxx"
%}

%include MonomialFactory_doc.i

%include otlmr/MonomialFactory.hxx
namespace OTLMR { %extend MonomialFactory { MonomialFactory(const MonomialFactory & other) { return new OTLMR::MonomialFactory(other); } } }
