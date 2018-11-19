User manual
===========

.. currentmodule:: otlm


Linear regression models
------------------------

The goal of this module is to improve creation and diagnostics of linear regression models.
Let us consider the general linear regression model:

.. math::

    Y \,=\, X \,\beta\, +\, \epsilon

Where :math:`X` is the design matrix of explanatory variables,
:math:`Y` is the vector of response values,
:math:`\beta` is a vector of unknown parameters to be estimated,
and :math:`\epsilon` is the error vector, which is assumed to be Gaussian.

Creation of linear models
-------------------------

.. autosummary::
    :toctree: _generated/
    :template: class.rst_t

    LinearModelAlgorithm

Stepwise method
---------------

.. autosummary::
    :toctree: _generated/
    :template: class.rst_t

    LinearModelStepwiseAlgorithm


Linear model analysis
---------------------

Class :class:`~otlm.LinearModelAnalysis` gives access to the following data:

* Linear model formula, in a textual form
* Informations about residuals (minimum, maximum, median, mean, quantiles, standard deviation)
* For each factor,

  - its coefficient
  - its standard error
  - p-value for Student test
  - A visual symbol for significance test

* Number of degrees of freedom
* Coefficients :math:`R^2` and adjusted :math:`R^2`
* p-value of the Fisher test
* Normality tests on residuals (Kolmogorov-Smirnov, Anderson-Darling and :math:`\chi^2`)

It also provides several plots:

* :func:`~otlm.LinearModelAnalysis.drawResidualsVsFitted` plots standardized residuals
  :math:`\tilde{\epsilon}` vs. fitted values
* :func:`~otlm.LinearModelAnalysis.drawQQplot` plots :math:`\sqrt{|\tilde{\epsilon}_i|}`
  vs. theoretical quantiles.
* :func:`~otlm.LinearModelAnalysis.drawScaleLocation` plots :math:`\sqrt{\tilde{\epsilon}_i}`
  vs. fitted values.
* :func:`~otlm.LinearModelAnalysis.drawCookDistance` plots an histogram of Cook's distance
* :func:`~otlm.LinearModelAnalysis.drawResidualsVsLeverages` plots standardized
  residuals :math:`\tilde{\epsilon}` vs leverages.
  Moreover, this plot also contains contour plot of function

  .. math:: D(x,y) = \frac{y^2}{p}\left(\frac{x}{1-x}\right)

  for levels 0.5 and 1.
* :func:`~otlm.LinearModelAnalysis.drawCookVsLeverages` plots Cook's distance
  :math:`\tilde{\epsilon}` vs :math:`\frac{h_i}{1-h_i}`.
  Moreover, this plot also contains isolines of function

  .. math:: \frac{py}{x}

.. autosummary::
    :toctree: _generated/
    :template: class.rst_t

    LinearModelAnalysis
    LinearModelResult

