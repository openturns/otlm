#! /usr/bin/env python

from __future__ import print_function
import openturns as ot
import otlmr as lm
from math import sin

ot.TESTPREAMBLE()

try:

    # lm build
    print("Fit y ~ 3 - 2 x + 0.05 * sin(x) model using 20 points (sin(x) ~ noise)")
    size = 20
    oneSample = ot.NumericalSample(size, 1)
    twoSample = ot.NumericalSample(size, 1)
    for i in range(size):
        oneSample[i,0] = 7.0 * sin(-3.5 + (6.5 * i) / (size - 1.0)) + 2.0
        twoSample[i,0] = -2.0 * oneSample[i, 0] + 3.0 + 0.05 * sin(oneSample[i, 0])

    test = lm.LinearModelAlgorithm(oneSample, twoSample)
    result = lm.LinearModelResult(test.getResult())
    analysis = lm.LinearModelAnalysis(result)
    print(analysis)
    # Compute confidence level (95%) for coefficients estimate (1-alpha = 0.95)
    alpha = 0.05
    lower_bounds = analysis.getCoefficientsEstimates() - analysis.getCoefficientsStandardErrors() * ot.Student(analysis.getDegreesOfFreedom()).computeQuantile(1 - alpha)
    upper_bounds = analysis.getCoefficientsEstimates() + analysis.getCoefficientsStandardErrors() * ot.Student(analysis.getDegreesOfFreedom()).computeQuantile(1 - alpha)
    # lower and upper bounds as NumericalPoint
    lower_bounds = map(lambda x: list(x)[0], list(lower_bounds))
    upper_bounds = map(lambda x: list(x)[0], list(upper_bounds))
    # interval confidence bounds
    interval = ot.Interval(lower_bounds, upper_bounds)
    print("confidence intervals with level=%1.2f : %s" %(1 - alpha, interval))

    print("")
    print("")
    print("")
    print( "Fit y ~ 1 + 0.1 x + 10 x^2 model using 100 points")
    ot.RandomGenerator.SetSeed(0)
    size = 100
    # Define a linespace from 0 to 10 with size points
    # We use a Box expermient ==> remove 0 & 1 points
    experiment = ot.Box([size - 2]);
    X = experiment.generate();
    # X is defined in [0,1]
    X *= [10];
    # Stack X2
    X2 = ot.NumericalSample(X);
    for i in range(size):
      X2[i, 0] = X[i, 0] * X2[i, 0]
    X.stack(X2)
    # Define y
    Y = ot.NumericalSample(size, 1);
    for i in range(size):
      Y[i, 0] = 1.0 +  0.1 * X[i, 0] + 10.0 * X[i, 0] * X[i, 0]  + 0.1 * ot.DistFunc.rNormal() ;
    test = lm.LinearModelAlgorithm(X, Y)
    result = test.getResult()
    analysis = lm.LinearModelAnalysis(result)
    print(analysis)
    # Compute confidence level (95%) for coefficients estimate (1-alpha = 0.95)
    alpha = 0.05
    lower_bounds = analysis.getCoefficientsEstimates() - analysis.getCoefficientsStandardErrors() * ot.Student(analysis.getDegreesOfFreedom()).computeQuantile(alpha / 2.0, True)
    upper_bounds = analysis.getCoefficientsEstimates() + analysis.getCoefficientsStandardErrors() * ot.Student(analysis.getDegreesOfFreedom()).computeQuantile(alpha / 2.0, True)
    # lower and upper bounds as NumericalPoint
    lower_bounds = map(lambda x: list(x)[0], list(lower_bounds))
    upper_bounds = map(lambda x: list(x)[0], list(upper_bounds))
    # interval confidence bounds
    interval = ot.Interval(lower_bounds, upper_bounds)
    print("confidence intervals with level=%1.2f : %s" %(1 - alpha, interval))

except:
    import sys
    print("t_LinearModelAlgorithm_std.py", sys.exc_info()[0], sys.exc_info()[1])
