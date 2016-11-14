// SWIG file LinearModelAlgorithm.i

%{
#include "otlm/LinearModelAlgorithm.hxx"
%}

%include LinearModelAlgorithm_doc.i

%rename(LinearModelAlgorithm_operator___eq__) OT::operator ==(const LinearModelAlgorithm & lhs, const LinearModelAlgorithm & rhs);

%include otlm/LinearModelAlgorithm.hxx
namespace OTLM { %extend LinearModelAlgorithm { LinearModelAlgorithm(const LinearModelAlgorithm & other) { return new OTLM::LinearModelAlgorithm(other); } } }
