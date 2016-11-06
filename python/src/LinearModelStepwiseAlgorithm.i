// SWIG file LinearModelStepwiseAlgorithm.i

%{
#include "otlmr/LinearModelStepwiseAlgorithm.hxx"
%}

%include LinearModelStepwiseAlgorithm_doc.i

%include otlmr/LinearModelStepwiseAlgorithm.hxx
namespace OTLMR { %extend LinearModelStepwiseAlgorithm { LinearModelStepwiseAlgorithm(const LinearModelStepwiseAlgorithm & other) { return new OTLMR::LinearModelStepwiseAlgorithm(other); } } }
