
ANOVA Table
===========

.. code:: python

    import openturns as ot
    import otlm
    
    sample = ot.Sample(
     [[ 4.17,4.81 ],
      [ 5.58,4.17 ],
      [ 5.18,4.41 ],
      [ 6.11,3.59 ],
      [ 4.5,5.87 ],
      [ 4.61,3.83 ],
      [ 5.17,6.03 ],
      [ 4.53,4.89 ],
      [ 5.33,4.32 ],
      [ 5.14,4.69 ]])
    ctl = sample[:,0]
    trt = sample[:,1]
    
    inputSample = ot.Sample(ctl.getSize(), [0])
    inputSample.add(ot.Sample(trt.getSize(), [1]))
    inputSample.setDescription(["Trt"])
    
    outputSample = ctl
    outputSample.add(trt)
    outputSample.setDescription(["weight"])
    
    algo = otlm.LinearModelAlgorithm(inputSample, outputSample)
    result = algo.getResult()
    analysis = otlm.LinearModelAnalysis(result)
    print(analysis)

.. parsed-literal::

    Basis( [[Trt]->[1],[Trt]->[Trt]] )
    
    Coefficients:
                 | Estimate   | Std Error  | t value    | Pr(>|t|)   | 
    ------------------------------------------------------------------
    [Trt]->[1]   | 5.032      | 0.155717   | 32.3149    | 2.1528e-17 | 
    [Trt]->[Trt] | -0.371     | 0.159763   | -2.32219   | 0.0321487  | 
    ------------------------------------------------------------------
    
    Residual standard error: 0.69639 on 18 degrees of freedom
    F-statistic: 1.4191 ,  p-value: 0.24822
    ----------------------------------
    Multiple R-squared   | 0.0730776 | 
    Adjusted R-squared   | 0.0215819 | 
    ----------------------------------
    
    ---------------------------------
    Normality test       | p-value  | 
    ---------------------------------
    Anderson-Darling     | 0.316615 | 
    Chi-Squared          | 0.433749 | 
    Kolmogorov-Smirnov   | 0.870208 | 
    ---------------------------------


