// SWIG file LinearModelAnalysis.i

%{
#include "otlm/LinearModelAnalysis.hxx"
%}

%include LinearModelAnalysis_doc.i

%include otlm/LinearModelAnalysis.hxx
namespace OTLM { %extend LinearModelAnalysis { LinearModelAnalysis(const LinearModelAnalysis & other) { return new OTLM::LinearModelAnalysis(other); } } }
