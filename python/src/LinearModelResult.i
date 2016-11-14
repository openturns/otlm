// SWIG file LinearModelResult.i

%{
#include "otlm/LinearModelResult.hxx"
%}

%include LinearModelResult_doc.i

//%rename(LinearModelResult_operator___eq__) OTLM::operator ==(const LinearModelResult & lhs, const LinearModelResult & rhs);

%include otlm/LinearModelResult.hxx
namespace OTLM { %extend LinearModelResult { LinearModelResult(const LinearModelResult & other) { return new OTLM::LinearModelResult(other); } } }
