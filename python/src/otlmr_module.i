// SWIG file otlmr_module.i

%module(docstring="otlmr module") otlmr

%{
#include <openturns/OT.hxx>
#include <openturns/PythonWrappingFunctions.hxx>
%}

// Prerequisites needed
%include typemaps.i
%include exception.i
%ignore *::load(OT::Advocate & adv);
%ignore *::save(OT::Advocate & adv) const;

%import base_module.i
%import uncertainty_module.i

// The new classes
%include otlmr/otlmrprivate.hxx
%include MonomialFactory.i
%include LinearModelResult.i
%include LinearModelAlgorithm.i
%include LinearModelAnalysis.i
%include LinearModelStepwiseAlgorithm.i

