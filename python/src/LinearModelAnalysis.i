// SWIG file LinearModelAnalysis.i

%{
#include "otlmr/LinearModelAnalysis.hxx"
%}

%include LinearModelAnalysis_doc.i

%include otlmr/LinearModelAnalysis.hxx
namespace OTLMR { %extend LinearModelAnalysis { LinearModelAnalysis(const LinearModelAnalysis & other) { return new OTLMR::LinearModelAnalysis(other); } } }
