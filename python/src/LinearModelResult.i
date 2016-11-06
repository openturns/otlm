// SWIG file LinearModelResult.i

%{
#include "otlmr/LinearModelResult.hxx"
%}

%include LinearModelResult_doc.i

//%rename(LinearModelResult_operator___eq__) OTLMR::operator ==(const LinearModelResult & lhs, const LinearModelResult & rhs);

%include otlmr/LinearModelResult.hxx
namespace OTLMR { %extend LinearModelResult { LinearModelResult(const LinearModelResult & other) { return new OTLMR::LinearModelResult(other); } } }
