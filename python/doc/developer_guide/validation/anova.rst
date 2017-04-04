ANOVA table
-----------

R listing:

.. code:: r

    # Output variables : weight of the plants
    ctl <- c(4.17 ,5.58 ,5.18 ,6.11 ,4.50 ,4.61 ,5.17 ,4.53 , 5.33 ,5.14)
    trt <- c(4.81 ,4.17 ,4.41 ,3.59 ,5.87 ,3.83 ,6.03 ,4.89 , 4.32 ,4.69)

    # Input variables :
    # - group " Ctl " : control group ( plant with standard conditions )
    # - group " Trt " : Treatment group ( plant with nutritionally enriched environment )

    group <- gl( 2, 10, 20, labels = c("Ctl","Trt"))
    weight <- c(ctl,trt)

    lm.D9 <- lm(weight ~ group)
    summary(lm.D9)

Output

.. code::

    Call:
    lm(formula = weight ~ group)

    Residuals:
        Min      1Q  Median      3Q     Max
    -1.0710 -0.4938  0.0685  0.2462  1.3690

    Coefficients:
                Estimate Std. Error t value Pr(>|t|)
    (Intercept)   5.0320     0.2202  22.850 9.55e-15 ***
    groupTrt     -0.3710     0.3114  -1.191    0.249
    ---
    Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1

    Residual standard error: 0.6964 on 18 degrees of freedom
    Multiple R-squared:  0.07308,   Adjusted R-squared:  0.02158
    F-statistic: 1.419 on 1 and 18 DF,  p-value: 0.249

With OpenTURNS:

.. code:: python

    import openturns as ot
    import otlm

    ctl =ot.Sample([[4.17], [5.58], [5.18], [6.11], [4.50], [4.61], [5.17], [4.53], [5.33], [5.14]])
    trt =ot.Sample([[4.81], [4.17], [4.41], [3.59], [5.87], [3.83], [6.03], [4.89], [4.32], [4.69]])

    inputSample = ot.Sample(ctl.getSize(), [0])
    inputSample.add(ot.Sample(trt.getSize(), [1]))
    inputSample.setDescription(["Trt"])

    outputSample = ot.Sample(ctl)
    outputSample.add(trt)
    outputSample.setDescription(["weight"])

    algo = otlm.LinearModelAlgorithm(inputSample, outputSample)
    result = algo.getResult()
    analysis = otlm.LinearModelAnalysis(result)
    print(analysis)

Output

.. parsed-literal::

    Basis( [[Trt]->[1],[Trt]->[Trt]] )
    
    Coefficients:
                 | Estimate    | Std Error   | t value     | Pr(>|t|)    | 
    ----------------------------------------------------------------------
    [Trt]->[1]   | 5.032       | 0.220218    | 22.8501     | 9.54713e-15 | 
    [Trt]->[Trt] | -0.371      | 0.311435    | -1.19126    | 0.249023    | 
    ----------------------------------------------------------------------
    
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

