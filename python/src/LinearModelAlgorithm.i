// SWIG file LinearModelAlgorithm.i

%{
#include "otlmr/LinearModelAlgorithm.hxx"
%}

%include LinearModelAlgorithm_doc.i

%rename(LinearModelAlgorithm_operator___eq__) OT::operator ==(const LinearModelAlgorithm & lhs, const LinearModelAlgorithm & rhs);

%include otlmr/LinearModelAlgorithm.hxx
namespace OTLMR { %extend LinearModelAlgorithm { LinearModelAlgorithm(const LinearModelAlgorithm & other) { return new OTLMR::LinearModelAlgorithm(other); } } }
