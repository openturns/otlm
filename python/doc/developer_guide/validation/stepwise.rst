
Stepwise method
---------------

In forward selection, the R ``step`` method does not try all remaining variables; as can be seen in
the first listing, it first considers only monomials, and this is only after adding ``X3`` and ``X4``
that it will check for ``X3*X4``.
For this reason, there are some differences between R and this module.

Use case 1
^^^^^^^^^^

R listing:


.. code:: r

    ### DESIGN
    X <- data.frame(cbind(X1,X2,X3,X4))
    names(X) <- c("X1","X2","X3","X4")
    
    ### LINEAR MODEL
    myLinearModel <- function(X) {
        beta <- c (14 , -7 , -17 , -7 , -3 ,13 , -16 , -4 ,12 ,3 ,13 ,20 ,17 , -10 ,7)
        Design <- cbind(rep(1,dim(X)[1]),X[,1],X[,2],X[,3],X[,4], X[,3]^2,X[,4]^2,X[,1]^2,X[,1]*X[,2],
                           X[,2]*X[,4] , X[,3]*X[,4] , X[,1]*X[,2]*X[,3] , X[,1]^3 ,X[,2]^3 ,X[,4]^3 )
        Y <- Design%*%as.matrix(beta)
        return(Y)
    }
    
    ### OBSERVATIONS
    Y <- myLinearModel(X) + residuals
    
    ### MIN/MAX/START MODELS
    model_min <- lm(Y~1 , data=X)
    model_max <- lm(Y~(X1+X2+X3+X4)^3+I(X1^2)+I(X1^3)+I(X2^2)+I(X2^3)
                                     +I(X3^2)+I(X3^3)+I(X4^2)+I(X4^3), data=X)
    model_0 <- lm(Y~X1+X2+X3+X4 , data=X)
    
    
    ### STEPWISE PROCEDURE
    
    ## Forward
    #AIC
    lm_forward_AIC <- step( model_min , scope=list(lower=model_min , upper=model_max) , direction="forward" , k=2)
    #BIC
    lm_forward_BIC <- step( model_min , scope=list(lower=model_min , upper=model_max) , direction="forward" , k=log(100))
    
    
    ## Backward
    #AIC
    lm_backward_AIC <- step( model_max , scope=list(lower=model_min , upper=model_max) , direction="backward" , k=2)
    #BIC
    lm_backward_BIC <- step( model_max , scope=list(upper=model_max , lower=model_min) , direction="backward" , k=log(100))
    
    ## Both
    #AIC
    lm_both_AIC <- step( model_0 , scope=list(lower=model_min , upper=model_max) , direction="both" , k=2)
    #BIC
    lm_both_BIC <- step( model_0 , scope=list(upper=model_max , lower=model_min) , direction="both" , k=log(100))
    
    
    ### ANOVA
    summary(lm_forward_AIC)
    AIC(lm_forward_AIC)
    
    summary(lm_forward_BIC)
    BIC(lm_forward_BIC)
    
    summary(lm_backward_AIC)
    AIC(lm_backward_AIC)
    
    summary(lm_backward_BIC)
    BIC(lm_backward_BIC)
    
    summary(lm_both_AIC)
    AIC(lm_both_AIC)
    
    summary(lm_both_BIC)
    BIC(lm_both_BIC)

Output

.. code::

    ...
    > ### STEPWISE PROCEDURE
    > 
    > ## Forward
    > #AIC
    > lm_forward_AIC <- step( model_min , scope=list(lower=model_min , upper=model_max) , direction="forward" , k=2)
    Start:  AIC=435.91
    Y ~ 1
    
              Df Sum of Sq    RSS    AIC
    + I(X1^3)  1   2449.82 5214.0 399.39
    + I(X1^2)  1   2449.50 5214.3 399.40
    + I(X3^3)  1   2368.07 5295.7 400.95
    + I(X3^2)  1   2196.96 5466.9 404.13
    + X1       1   2153.80 5510.0 404.92
    + X3       1   1758.18 5905.6 411.85
    + I(X2^3)  1   1647.84 6016.0 413.70
    + I(X2^2)  1   1529.15 6134.7 415.65
    + X2       1   1309.35 6354.5 419.17
    <none>                 7663.8 435.91
    + X4       1     30.75 7633.1 437.51
    + I(X4^2)  1     19.71 7644.1 437.65
    + I(X4^3)  1      6.48 7657.3 437.82
    
    Step:  AIC=399.39
    Y ~ I(X1^3)
    
              Df Sum of Sq    RSS    AIC
    + I(X3^3)  1   1980.57 3233.4 353.61
    + I(X3^2)  1   1977.69 3236.3 353.70
    + X3       1   1755.95 3458.0 360.33
    + I(X2^3)  1   1490.22 3723.8 367.73
    + I(X2^2)  1   1456.80 3757.2 368.63
    + X2       1   1294.26 3919.7 372.86
    + X4       1    215.76 4998.2 397.17
    + I(X4^2)  1    185.36 5028.6 397.77
    + I(X4^3)  1    146.43 5067.6 398.54
    <none>                 5214.0 399.39
    + I(X1^2)  1     16.24 5197.8 401.08
    + X1       1      4.00 5210.0 401.32
    
    Step:  AIC=353.61
    Y ~ I(X1^3) + I(X3^3)
    
              Df Sum of Sq    RSS    AIC
    + I(X2^3)  1   2314.62  918.8 229.79
    + I(X2^2)  1   2182.89 1050.5 243.19
    + X2       1   1837.77 1395.7 271.60
    + X4       1    413.32 2820.1 341.94
    + I(X4^2)  1    366.65 2866.8 343.58
    + I(X4^3)  1    313.41 2920.0 345.42
    + I(X1^2)  1    107.67 3125.8 352.23
    <none>                 3233.4 353.61
    + X1       1     43.76 3189.7 354.25
    + I(X3^2)  1     12.71 3220.7 355.22
    + X3       1      7.47 3226.0 355.38
    
    Step:  AIC=229.79
    Y ~ I(X1^3) + I(X3^3) + I(X2^3)
    
              Df Sum of Sq    RSS    AIC
    + X4       1   250.068 668.74 200.02
    + I(X4^2)  1   248.890 669.92 200.20
    + I(X4^3)  1   230.851 687.96 202.86
    + X3       1    49.376 869.44 226.27
    + I(X3^2)  1    30.490 888.32 228.42
    + I(X1^2)  1    29.994 888.82 228.47
    + I(X2^2)  1    28.473 890.34 228.64
    + X2       1    24.443 894.37 229.09
    <none>                 918.81 229.79
    + X1       1    17.215 901.60 229.90
    
    Step:  AIC=200.02
    Y ~ I(X1^3) + I(X3^3) + I(X2^3) + X4
    
              Df Sum of Sq    RSS    AIC
    + X3       1    68.112 600.63 191.28
    + I(X3^2)  1    54.472 614.27 193.53
    + I(X1^2)  1    18.731 650.01 199.18
    <none>                 668.74 200.02
    + X1       1    10.555 658.19 200.43
    + I(X2^2)  1     5.174 663.57 201.25
    + X2       1     4.199 664.55 201.39
    + I(X4^2)  1     2.600 666.14 201.63
    + I(X4^3)  1     1.461 667.28 201.81
    
    Step:  AIC=191.28
    Y ~ I(X1^3) + I(X3^3) + I(X2^3) + X4 + X3
    
              Df Sum of Sq    RSS    AIC
    + X3:X4    1   132.222 468.41 168.42
    + I(X1^2)  1    13.201 587.43 191.06
    <none>                 600.63 191.28
    + X1       1     8.781 591.85 191.81
    + I(X3^2)  1     7.303 593.33 192.06
    + I(X4^2)  1     5.537 595.10 192.35
    + I(X4^3)  1     4.005 596.63 192.61
    + X2       1     0.402 600.23 193.22
    + I(X2^2)  1     0.097 600.54 193.26
    
    Step:  AIC=168.42
    Y ~ I(X1^3) + I(X3^3) + I(X2^3) + X4 + X3 + X4:X3
    
              Df Sum of Sq    RSS    AIC
    + I(X4^2)  1   25.9297 442.48 164.72
    + I(X4^3)  1   25.5292 442.88 164.81
    <none>                 468.41 168.42
    + I(X3^2)  1    4.6876 463.72 169.41
    + I(X1^2)  1    2.0693 466.34 169.97
    + X1       1    0.9149 467.50 170.22
    + I(X2^2)  1    0.7832 467.63 170.25
    + X2       1    0.3309 468.08 170.35
    
    Step:  AIC=164.72
    Y ~ I(X1^3) + I(X3^3) + I(X2^3) + X4 + X3 + I(X4^2) + X4:X3
    
              Df Sum of Sq    RSS    AIC
    <none>                 442.48 164.72
    + I(X3^2)  1    4.0023 438.48 165.81
    + I(X1^2)  1    2.5372 439.94 166.15
    + X1       1    1.6099 440.87 166.36
    + I(X4^3)  1    0.0320 442.45 166.72
    + I(X2^2)  1    0.0249 442.46 166.72
    + X2       1    0.0014 442.48 166.72
    
    ...
    
    > ### ANOVA
    > summary(lm_forward_AIC)
    
    Call:
    lm(formula = Y ~ I(X1^3) + I(X3^3) + I(X2^3) + X4 + X3 + I(X4^2) + 
        X4:X3, data = X)
    
    Residuals:
        Min      1Q  Median      3Q     Max 
    -4.8365 -1.3917 -0.1784  1.3654  6.4746 
    
    Coefficients:
                Estimate Std. Error t value Pr(>|t|)    
    (Intercept)   6.0459     1.1309   5.346 6.50e-07 ***
    I(X1^3)      18.2477     0.8509  21.445  < 2e-16 ***
    I(X3^3)      11.9240     1.9668   6.062 2.92e-08 ***
    I(X2^3)     -16.6636     0.7983 -20.875  < 2e-16 ***
    X4           -6.1208     3.6041  -1.698   0.0928 .  
    X3           -0.2942     2.3979  -0.123   0.9026    
    I(X4^2)      -7.6170     3.2805  -2.322   0.0224 *  
    X4:X3        16.4005     2.9115   5.633 1.91e-07 ***
    ---
    Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1
    
    Residual standard error: 2.193 on 92 degrees of freedom
    Multiple R-squared:  0.9423,	Adjusted R-squared:  0.9379 
    F-statistic: 214.5 on 7 and 92 DF,  p-value: < 2.2e-16
    
    > AIC(lm_forward_AIC)
    [1] 450.5105
    > 
    > summary(lm_forward_BIC)
    
    Call:
    lm(formula = Y ~ I(X1^3) + I(X3^3) + I(X2^3) + X4 + X3 + I(X4^2) + 
        X4:X3, data = X)
    
    Residuals:
        Min      1Q  Median      3Q     Max 
    -4.8365 -1.3917 -0.1784  1.3654  6.4746 
    
    Coefficients:
                Estimate Std. Error t value Pr(>|t|)    
    (Intercept)   6.0459     1.1309   5.346 6.50e-07 ***
    I(X1^3)      18.2477     0.8509  21.445  < 2e-16 ***
    I(X3^3)      11.9240     1.9668   6.062 2.92e-08 ***
    I(X2^3)     -16.6636     0.7983 -20.875  < 2e-16 ***
    X4           -6.1208     3.6041  -1.698   0.0928 .  
    X3           -0.2942     2.3979  -0.123   0.9026    
    I(X4^2)      -7.6170     3.2805  -2.322   0.0224 *  
    X4:X3        16.4005     2.9115   5.633 1.91e-07 ***
    ---
    Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1
    
    Residual standard error: 2.193 on 92 degrees of freedom
    Multiple R-squared:  0.9423,	Adjusted R-squared:  0.9379 
    F-statistic: 214.5 on 7 and 92 DF,  p-value: < 2.2e-16
    
    > BIC(lm_forward_BIC)
    [1] 473.9571
    > 
    > summary(lm_backward_AIC)
    
    Call:
    lm(formula = Y ~ X1 + X2 + X3 + X4 + I(X1^3) + I(X2^3) + I(X3^2) + 
        I(X4^3) + X1:X2 + X1:X3 + X1:X4 + X2:X3 + X2:X4 + X3:X4 + 
        X1:X2:X3 + X2:X3:X4, data = X)
    
    Residuals:
        Min      1Q  Median      3Q     Max 
    -1.8223 -0.6065 -0.1239  0.5478  2.1709 
    
    Coefficients:
                Estimate Std. Error t value Pr(>|t|)    
    (Intercept)  14.8638     0.9518  15.617  < 2e-16 ***
    X1           -5.7515     1.6671  -3.450 0.000884 ***
    X2          -19.2572     1.9888  -9.683 2.82e-15 ***
    X3           -9.6846     2.0988  -4.614 1.42e-05 ***
    X4          -11.4020     1.6285  -7.001 6.07e-10 ***
    I(X1^3)      14.5914     0.9685  15.065  < 2e-16 ***
    I(X2^3)     -12.0528     0.9520 -12.660  < 2e-16 ***
    I(X3^2)      13.9339     1.3449  10.361  < 2e-16 ***
    I(X4^3)      -3.2383     0.9251  -3.500 0.000750 ***
    X1:X2        11.4580     2.4410   4.694 1.05e-05 ***
    X1:X3         1.8396     2.6413   0.696 0.488072    
    X1:X4        -3.5140     1.3175  -2.667 0.009197 ** 
    X2:X3         6.3360     3.2129   1.972 0.051936 .  
    X2:X4        10.4012     2.7908   3.727 0.000353 ***
    X3:X4        16.6128     2.4389   6.812 1.42e-09 ***
    X1:X2:X3     14.5751     4.2878   3.399 0.001041 ** 
    X2:X3:X4     -9.2093     4.2582  -2.163 0.033439 *  
    ---
    Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1
    
    Residual standard error: 0.8908 on 83 degrees of freedom
    Multiple R-squared:  0.9914,	Adjusted R-squared:  0.9897 
    F-statistic: 598.4 on 16 and 83 DF,  p-value: < 2.2e-16
    
    > AIC(lm_backward_AIC)
    [1] 278.0353
    > 
    > summary(lm_backward_BIC)
    
    Call:
    lm(formula = Y ~ X1 + X2 + X3 + X4 + I(X1^3) + I(X2^3) + I(X3^2) + 
        I(X4^3) + X1:X2 + X1:X3 + X1:X4 + X2:X3 + X2:X4 + X3:X4 + 
        X1:X2:X3 + X2:X3:X4, data = X)
    
    Residuals:
        Min      1Q  Median      3Q     Max 
    -1.8223 -0.6065 -0.1239  0.5478  2.1709 
    
    Coefficients:
                Estimate Std. Error t value Pr(>|t|)    
    (Intercept)  14.8638     0.9518  15.617  < 2e-16 ***
    X1           -5.7515     1.6671  -3.450 0.000884 ***
    X2          -19.2572     1.9888  -9.683 2.82e-15 ***
    X3           -9.6846     2.0988  -4.614 1.42e-05 ***
    X4          -11.4020     1.6285  -7.001 6.07e-10 ***
    I(X1^3)      14.5914     0.9685  15.065  < 2e-16 ***
    I(X2^3)     -12.0528     0.9520 -12.660  < 2e-16 ***
    I(X3^2)      13.9339     1.3449  10.361  < 2e-16 ***
    I(X4^3)      -3.2383     0.9251  -3.500 0.000750 ***
    X1:X2        11.4580     2.4410   4.694 1.05e-05 ***
    X1:X3         1.8396     2.6413   0.696 0.488072    
    X1:X4        -3.5140     1.3175  -2.667 0.009197 ** 
    X2:X3         6.3360     3.2129   1.972 0.051936 .  
    X2:X4        10.4012     2.7908   3.727 0.000353 ***
    X3:X4        16.6128     2.4389   6.812 1.42e-09 ***
    X1:X2:X3     14.5751     4.2878   3.399 0.001041 ** 
    X2:X3:X4     -9.2093     4.2582  -2.163 0.033439 *  
    ---
    Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1
    
    Residual standard error: 0.8908 on 83 degrees of freedom
    Multiple R-squared:  0.9914,	Adjusted R-squared:  0.9897 
    F-statistic: 598.4 on 16 and 83 DF,  p-value: < 2.2e-16
    
    > BIC(lm_backward_BIC)
    [1] 324.9284
    > 
    > summary(lm_both_AIC)
    
    Call:
    lm(formula = Y ~ X1 + X2 + X3 + X4 + I(X3^3) + I(X2^3) + I(X4^3) + 
        I(X1^3) + X1:X2 + X3:X4 + X1:X3 + X2:X3 + X2:X4 + X1:X4 + 
        X1:X2:X3 + X2:X3:X4, data = X)
    
    Residuals:
        Min      1Q  Median      3Q     Max 
    -1.8262 -0.5757 -0.1136  0.5819  2.1623 
    
    Coefficients:
                Estimate Std. Error t value Pr(>|t|)    
    (Intercept)  14.2255     0.9462  15.034  < 2e-16 ***
    X1           -5.4113     1.6658  -3.249 0.001675 ** 
    X2          -19.0700     1.9910  -9.578 4.56e-15 ***
    X3           -3.8607     1.8558  -2.080 0.040572 *  
    X4          -11.5169     1.6280  -7.074 4.38e-10 ***
    I(X3^3)       9.1562     0.8829  10.370  < 2e-16 ***
    I(X2^3)     -12.3189     0.9557 -12.890  < 2e-16 ***
    I(X4^3)      -3.3016     0.9245  -3.571 0.000594 ***
    I(X1^3)      14.3734     0.9717  14.792  < 2e-16 ***
    X1:X2        11.0549     2.4375   4.535 1.92e-05 ***
    X3:X4        17.1474     2.4374   7.035 5.22e-10 ***
    X1:X3         0.8662     2.6440   0.328 0.744023    
    X2:X3         6.6310     3.2090   2.066 0.041911 *  
    X2:X4        10.5725     2.7884   3.792 0.000283 ***
    X1:X4        -3.1422     1.3231  -2.375 0.019861 *  
    X1:X2:X3     15.4381     4.2834   3.604 0.000533 ***
    X2:X3:X4    -10.1590     4.2559  -2.387 0.019258 *  
    ---
    Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1
    
    Residual standard error: 0.8904 on 83 degrees of freedom
    Multiple R-squared:  0.9914,	Adjusted R-squared:  0.9898 
    F-statistic:   599 on 16 and 83 DF,  p-value: < 2.2e-16
    
    > AIC(lm_both_AIC)
    [1] 277.9309
    > 
    > summary(lm_both_BIC)
    
    Call:
    lm(formula = Y ~ X1 + X2 + X3 + X4 + I(X3^3) + I(X2^3) + I(X4^3) + 
        I(X1^3) + X1:X2 + X3:X4 + X1:X3 + X2:X3 + X2:X4 + X1:X4 + 
        X1:X2:X3 + X2:X3:X4, data = X)
    
    Residuals:
        Min      1Q  Median      3Q     Max 
    -1.8262 -0.5757 -0.1136  0.5819  2.1623 
    
    Coefficients:
                Estimate Std. Error t value Pr(>|t|)    
    (Intercept)  14.2255     0.9462  15.034  < 2e-16 ***
    X1           -5.4113     1.6658  -3.249 0.001675 ** 
    X2          -19.0700     1.9910  -9.578 4.56e-15 ***
    X3           -3.8607     1.8558  -2.080 0.040572 *  
    X4          -11.5169     1.6280  -7.074 4.38e-10 ***
    I(X3^3)       9.1562     0.8829  10.370  < 2e-16 ***
    I(X2^3)     -12.3189     0.9557 -12.890  < 2e-16 ***
    I(X4^3)      -3.3016     0.9245  -3.571 0.000594 ***
    I(X1^3)      14.3734     0.9717  14.792  < 2e-16 ***
    X1:X2        11.0549     2.4375   4.535 1.92e-05 ***
    X3:X4        17.1474     2.4374   7.035 5.22e-10 ***
    X1:X3         0.8662     2.6440   0.328 0.744023    
    X2:X3         6.6310     3.2090   2.066 0.041911 *  
    X2:X4        10.5725     2.7884   3.792 0.000283 ***
    X1:X4        -3.1422     1.3231  -2.375 0.019861 *  
    X1:X2:X3     15.4381     4.2834   3.604 0.000533 ***
    X2:X3:X4    -10.1590     4.2559  -2.387 0.019258 *  
    ---
    Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1
    
    Residual standard error: 0.8904 on 83 degrees of freedom
    Multiple R-squared:  0.9914,	Adjusted R-squared:  0.9898 
    F-statistic:   599 on 16 and 83 DF,  p-value: < 2.2e-16
    
    > BIC(lm_both_BIC)
    [1] 324.8239
    > 

With OpenTURNS:

.. code:: python

    import openturns as ot
    import otlm
    from math import log
    
    sample = ot.Sample.ImportFromTextFile("../../examples/DATA_test1.csv", ",")
    
    X = sample[:, 1:5]
    R = sample[:, 0]
    
    myLinearModel = ot.Function(['x1', 'x2', 'x3', 'x4'], ['y'],
        ['14 - 7*x1 - 17*x2 - 7 *x3 - 3*x4 + 13*x3^2 - 16*x4^2 ' +
           ' - 4*x1^2 + 12*x1*x2 + 3*x2*x4 + 13*x3*x4 + 20*x1*x2*x3 ' +
           ' + 17*x1^3 - 10*x2^3 + 7*x4^3'])
    
    Y = myLinearModel(X) + R
    
    ################################################################################################
    # Build a model Y~(X1+X2+X3+X4)^3+I(Xi)^2+I(Xi)^3
    dim = X.getDimension()
    enumerateFunction = ot.EnumerateFunction(dim)
    factory = ot.TensorizedUniVariateFunctionFactory([ot.MonomialFunctionFactory()]*dim, enumerateFunction)
    
    # Build 'interactions' as a list of list [a1,a2,a3,a4], and we will generate tensorized
    # polynomials x1^a1*x2^a2*x3^a3*x4^a4.
    
    # Y ~ (X1+X2+X3+X4)^4
    interactions = [x for x in ot.Tuples([2]*dim).generate()]
    # Remove X1*X2*X3*X4 to obtain Y ~ (X1+X2+X3+X4)^3
    interactions.pop(interactions.index([1]*dim))
    for i in range(dim):
      indices = [0]*dim
      indices[i] = 2
      # Y ~ I(Xi)^2
      interactions.append(indices[:])
      # Y ~ I(Xi)^3
      indices[i] = 3
      interactions.append(indices[:])
    
    funcs = [factory.build(enumerateFunction.inverse(indices)) for indices in interactions]
    [f.getEvaluation().setInputDescription(X.getDescription()) for f in funcs]
    basis = ot.Basis(funcs)
    ################################################################################################
    
    i_min = [interactions.index([0,0,0,0])]
    i_0 = i_min[:]
    for i in range(dim):
      indices = [0]*dim
      indices[i] = 1
      i_0.append(interactions.index(indices))
    
    #---------------- Forward / Backward------------------- 
    #   X: input sample
    #   basis : Basis
    #   Y: output sample
    #   i_min:  indices of minimal model
    #   direction: Boolean (True FORWARD, False BACKWARD)
    #   penalty: multiplier of number of degrees of freedom
    #   maxiteration: maximum number of iterations
    
    #---------------- Both------------------- 
    #   X: input sample
    #   basis : Basis
    #   Y: output sample
    #   i_min : indices of minimal model
    #   i_0   : indices of start model
    #   penalty: multiplier of number of degrees of freedom
    #   maxiteration: maximum number of iterations
    
    penalty_BIC = log(X.getSize())
    penalty_AIC = 2.
    maxiteration = 1000
    
    for k in [penalty_AIC, penalty_BIC]:
      ## Forward / Backward
      if k==penalty_AIC:  IC =" AIC "
      if k==penalty_BIC:  IC =" BIC "  
      for forward in [True, False]:
        algo = otlm.LinearModelStepwiseAlgorithm(X, basis, Y, i_min, forward, k, maxiteration)
        algo_result = otlm.LinearModelAnalysis(algo.getResult())
        print("{0:~^60s}".format(""))
        if forward==True : print(" Forward " +IC)
        else             : print(" Backward "+IC)
        print("{0:~^60s}".format(""))
        print(algo_result)
      ## Both
      algo = otlm.LinearModelStepwiseAlgorithm(X, basis, Y, i_min, i_0, k, maxiteration)
      algo_result = otlm.LinearModelAnalysis(algo.getResult())
      print("{0:~^60s}".format(""))
      print(" Both "+IC)
      print("{0:~^60s}".format(""))
      print(algo_result)

Output:

.. code::

    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     Forward  AIC
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Basis( [1,X1,X2,(X1) * (X2),X3,(X1) * (X3),(X2) * (X3),(X1) * (X2) * (X3),X4,(X1) * (X4),(X2) * (X4),(X1) * (X2) * (X4),(X3) * (X4),(X1) * (X3) * (X4),(X2) * (X3) * (X4),X1^2,X1^3,X2^2,X2^3,X3^2,X3^3,X4^2,X4^3]#23 )
    
    Coefficients:
                       | Estimate    | Std Error   | t value     | Pr(>|t|)    |
    ----------------------------------------------------------------------------
    1                  | 10.9814     | 0.65648     | 16.7277     | 1.27112e-28 |
    (X1) * (X3)        | -3.88875    | 1.9846      | -1.95946    | 0.0533367   |
    X2^2               | 7.18917     | 8.49215     | 0.846566    | 0.399614    |
    (X1) * (X2) * (X3) | 18.6503     | 2.15953     | 8.63624     | 2.94296e-13 |
    X3^3               | 9.55935     | 0.861055    | 11.1019     | 3.21154e-18 |
    X1^3               | 14.3043     | 1.03648     | 13.8009     | 2.04805e-23 |
    (X1) * (X4)        | -11.6087    | 1.83235     | -6.33539    | 1.08208e-08 |
    (X1) * (X2) * (X4) | 9.8174      | 2.11831     | 4.63455     | 1.28003e-05 |
    (X1) * (X3) * (X4) | 5.86969     | 2.9541      | 1.98696     | 0.0501475   |
    X4^2               | -4.64921    | 1.50572     | -3.0877     | 0.00272418  |
    (X3) * (X4)        | 9.9525      | 1.56499     | 6.35948     | 9.73106e-09 |
    X2                 | -16.4775    | 3.73193     | -4.41527    | 2.94963e-05 |
    X2^3               | -16.4804    | 5.66605     | -2.90862    | 0.00463118  |
    (X1) * (X2)        | 4.14884     | 1.9234      | 2.15704     | 0.033826    |
    X4                 | -3.25933    | 1.65126     | -1.97385    | 0.0516471   |
    ----------------------------------------------------------------------------
    
    Residual standard error: 0.93463 on 85 degrees of freedom
    F-statistic: 620.59 ,  p-value: 0
    ---------------------------------
    Multiple R-squared   | 0.990312 |
    Adjusted R-squared   | 0.988716 |
    ---------------------------------
    
    ---------------------------------
    Normality test       | p-value  |
    ---------------------------------
    Anderson-Darling     | 0.132184 |
    Chi-Squared          | 0.562718 |
    Kolmogorov-Smirnov   | 0.620456 |
    ---------------------------------
    
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     Backward  AIC
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Basis( [1,X1,X2,(X1) * (X2),X3,(X1) * (X3),(X2) * (X3),(X1) * (X2) * (X3),X4,(X1) * (X4),(X2) * (X4),(X1) * (X2) * (X4),(X3) * (X4),(X1) * (X3) * (X4),(X2) * (X3) * (X4),X1^2,X1^3,X2^2,X2^3,X3^2,X3^3,X4^2,X4^3]#23 )
    
    Coefficients:
                       | Estimate    | Std Error   | t value     | Pr(>|t|)    |
    ----------------------------------------------------------------------------
    1                  | 14.4696     | 0.85367     | 16.9499     | 1.10055e-28 |
    X1                 | -4.96924    | 1.13775     | -4.36761    | 3.6037e-05  |
    X2                 | -19.6298    | 1.96072     | -10.0115    | 6.24137e-16 |
    (X1) * (X2)        | 11.8902     | 2.08797     | 5.6946      | 1.82096e-07 |
    X3                 | -8.79793    | 1.85144     | -4.75195    | 8.3525e-06  |
    (X2) * (X3)        | 6.52733     | 2.99453     | 2.17975     | 0.0321087   |
    (X1) * (X2) * (X3) | 13.9549     | 3.1442      | 4.43829     | 2.76759e-05 |
    X4                 | -10.1758    | 1.96078     | -5.18967    | 1.46874e-06 |
    (X1) * (X4)        | -5.60052    | 2.132       | -2.62688    | 0.0102606   |
    (X2) * (X4)        | 10.2331     | 2.74482     | 3.72817     | 0.000351322 |
    (X3) * (X4)        | 14.3123     | 3.13008     | 4.57249     | 1.66633e-05 |
    (X1) * (X3) * (X4) | 3.92134     | 3.01451     | 1.30082     | 0.19692     |
    (X2) * (X3) * (X4) | -8.77332    | 4.2304      | -2.07388    | 0.0411906   |
    X1^3               | 14.768      | 0.974922    | 15.1479     | 1.30072e-25 |
    X2^3               | -11.8816    | 0.958026    | -12.4021    | 1.36769e-20 |
    X3^2               | 14.0536     | 1.33682     | 10.5127     | 6.33754e-17 |
    X4^3               | -3.25468    | 0.917225    | -3.54839    | 0.000640898 |
    ----------------------------------------------------------------------------
    
    Residual standard error: 0.88446 on 83 degrees of freedom
    F-statistic: 607.11 ,  p-value: 0
    ---------------------------------
    Multiple R-squared   | 0.991528 |
    Adjusted R-squared   | 0.989895 |
    ---------------------------------
    
    ---------------------------------
    Normality test       | p-value  |
    ---------------------------------
    Anderson-Darling     | 0.197651 |
    Chi-Squared          | 0.125636 |
    Kolmogorov-Smirnov   | 0.482042 |
    ---------------------------------
    
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     Both  AIC
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Basis( [1,X1,X2,(X1) * (X2),X3,(X1) * (X3),(X2) * (X3),(X1) * (X2) * (X3),X4,(X1) * (X4),(X2) * (X4),(X1) * (X2) * (X4),(X3) * (X4),(X1) * (X3) * (X4),(X2) * (X3) * (X4),X1^2,X1^3,X2^2,X2^3,X3^2,X3^3,X4^2,X4^3]#23 )
    
    Coefficients:
                       | Estimate    | Std Error   | t value     | Pr(>|t|)    |
    ----------------------------------------------------------------------------
    1                  | 12.7086     | 0.624644    | 20.3453     | 7.94404e-35 |
    X1                 | -5.46036    | 1.10552     | -4.93919    | 3.75912e-06 |
    X2                 | -15.855     | 1.19992     | -13.2134    | 1.65462e-22 |
    X4                 | -9.23488    | 1.1925      | -7.74411    | 1.63394e-11 |
    (X1) * (X2) * (X3) | 16.9437     | 1.53474     | 11.0401     | 3.14529e-18 |
    X3^3               | 8.78174     | 0.688698    | 12.7512     | 1.29079e-21 |
    (X1) * (X2)        | 11.2068     | 1.39616     | 8.02686     | 4.36462e-12 |
    (X3) * (X4)        | 11.8023     | 1.03538     | 11.399      | 5.99206e-19 |
    X4^3               | -2.95993    | 0.922675    | -3.20799    | 0.00187159  |
    (X2) * (X4)        | 4.98183     | 1.18419     | 4.20694     | 6.26974e-05 |
    X1^3               | 14.1106     | 0.968097    | 14.5756     | 4.49373e-25 |
    X2^3               | -12.3915    | 0.951199    | -13.0273    | 3.77309e-22 |
    (X1) * (X4)        | -2.78756    | 1.28899     | -2.1626     | 0.0333158   |
    ----------------------------------------------------------------------------
    
    Residual standard error: 0.90426 on 87 degrees of freedom
    F-statistic: 773.79 ,  p-value: 0
    ---------------------------------
    Multiple R-squared   | 0.990718 |
    Adjusted R-squared   | 0.989437 |
    ---------------------------------
    
    ---------------------------------
    Normality test       | p-value  |
    ---------------------------------
    Anderson-Darling     | 0.128032 |
    Chi-Squared          | 0.317021 |
    Kolmogorov-Smirnov   | 0.606542 |
    ---------------------------------
    
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     Forward  BIC
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Basis( [1,X1,X2,(X1) * (X2),X3,(X1) * (X3),(X2) * (X3),(X1) * (X2) * (X3),X4,(X1) * (X4),(X2) * (X4),(X1) * (X2) * (X4),(X3) * (X4),(X1) * (X3) * (X4),(X2) * (X3) * (X4),X1^2,X1^3,X2^2,X2^3,X3^2,X3^3,X4^2,X4^3]#23 )
    
    Coefficients:
                       | Estimate    | Std Error   | t value     | Pr(>|t|)    |
    ----------------------------------------------------------------------------
    1                  | 10.3793     | 0.591016    | 17.5618     | 3.48943e-30 |
    (X1) * (X3)        | -4.29201    | 2.00702     | -2.13849    | 0.0353128   |
    X2^2               | 5.46835     | 8.58833     | 0.636719    | 0.525999    |
    (X1) * (X2) * (X3) | 17.9846     | 2.16866     | 8.29297     | 1.34925e-12 |
    X3^3               | 9.92345     | 0.85511     | 11.6049     | 2.75263e-19 |
    X1^3               | 14.3108     | 1.05378     | 13.5804     | 4.16424e-23 |
    (X1) * (X4)        | -12.4907    | 1.8067      | -6.91358    | 7.84586e-10 |
    (X1) * (X2) * (X4) | 9.36335     | 2.14094     | 4.37347     | 3.41352e-05 |
    (X1) * (X3) * (X4) | 7.80951     | 2.83236     | 2.75724     | 0.00711774  |
    X4^2               | -6.96187    | 0.96153     | -7.2404     | 1.76982e-10 |
    (X3) * (X4)        | 8.88511     | 1.49311     | 5.95073     | 5.62382e-08 |
    X2                 | -16.1115    | 3.78956     | -4.25155    | 5.37021e-05 |
    X2^3               | -15.1689    | 5.72092     | -2.65148    | 0.00954014  |
    (X1) * (X2)        | 4.98412     | 1.9076      | 2.61277     | 0.0105991   |
    ----------------------------------------------------------------------------
    
    Residual standard error: 0.95024 on 86 degrees of freedom
    F-statistic: 646.27 ,  p-value: 0
    ---------------------------------
    Multiple R-squared   | 0.989867 |
    Adjusted R-squared   | 0.988336 |
    ---------------------------------
    
    ---------------------------------
    Normality test       | p-value  |
    ---------------------------------
    Anderson-Darling     | 0.384558 |
    Chi-Squared          | 0.808498 |
    Kolmogorov-Smirnov   | 0.623556 |
    ---------------------------------
    
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     Backward  BIC
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Basis( [1,X1,X2,(X1) * (X2),X3,(X1) * (X3),(X2) * (X3),(X1) * (X2) * (X3),X4,(X1) * (X4),(X2) * (X4),(X1) * (X2) * (X4),(X3) * (X4),(X1) * (X3) * (X4),(X2) * (X3) * (X4),X1^2,X1^3,X2^2,X2^3,X3^2,X3^3,X4^2,X4^3]#23 )
    
    Coefficients:
                       | Estimate    | Std Error   | t value     | Pr(>|t|)    |
    ----------------------------------------------------------------------------
    1                  | 13.5485     | 0.717251    | 18.8896     | 2.3713e-32  |
    X1                 | -5.50901    | 1.12662     | -4.88987    | 4.6462e-06  |
    X2                 | -16.0279    | 1.21304     | -13.2129    | 2.06943e-22 |
    (X1) * (X2)        | 11.1859     | 1.39804     | 8.00117     | 5.26182e-12 |
    X3                 | -6.52078    | 1.4938      | -4.36523    | 3.52047e-05 |
    (X1) * (X2) * (X3) | 17.3258     | 1.54802     | 11.1922     | 1.817e-18   |
    X4                 | -9.55887    | 1.21411     | -7.87314    | 9.5434e-12  |
    (X1) * (X4)        | -2.93384    | 1.3133      | -2.23395    | 0.0280807   |
    (X2) * (X4)        | 5.13282     | 1.22812     | 4.17941     | 6.99762e-05 |
    (X3) * (X4)        | 12.2915     | 1.27098     | 9.67088     | 2.12107e-15 |
    X1^3               | 14.2361     | 0.969488    | 14.6842     | 3.72177e-25 |
    X2^3               | -12.2284    | 0.949904    | -12.8733    | 9.23801e-22 |
    X3^2               | 13.9612     | 1.3549      | 10.3042     | 1.10315e-16 |
    X4^3               | -2.96765    | 0.927461    | -3.19976    | 0.00192691  |
    ----------------------------------------------------------------------------
    
    Residual standard error: 0.90562 on 86 degrees of freedom
    F-statistic: 712.19 ,  p-value: 0
    ---------------------------------
    Multiple R-squared   | 0.990797 |
    Adjusted R-squared   | 0.989405 |
    ---------------------------------
    
    ---------------------------------
    Normality test       | p-value  |
    ---------------------------------
    Anderson-Darling     | 0.434157 |
    Chi-Squared          | 0.355962 |
    Kolmogorov-Smirnov   | 0.807624 |
    ---------------------------------
    
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     Both  BIC
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Basis( [1,X1,X2,(X1) * (X2),X3,(X1) * (X3),(X2) * (X3),(X1) * (X2) * (X3),X4,(X1) * (X4),(X2) * (X4),(X1) * (X2) * (X4),(X3) * (X4),(X1) * (X3) * (X4),(X2) * (X3) * (X4),X1^2,X1^3,X2^2,X2^3,X3^2,X3^3,X4^2,X4^3]#23 )
    
    Coefficients:
                       | Estimate    | Std Error   | t value     | Pr(>|t|)    |
    ----------------------------------------------------------------------------
    1                  | 12.7086     | 0.624644    | 20.3453     | 7.94404e-35 |
    X1                 | -5.46036    | 1.10552     | -4.93919    | 3.75912e-06 |
    X2                 | -15.855     | 1.19992     | -13.2134    | 1.65462e-22 |
    X4                 | -9.23488    | 1.1925      | -7.74411    | 1.63394e-11 |
    (X1) * (X2) * (X3) | 16.9437     | 1.53474     | 11.0401     | 3.14529e-18 |
    X3^3               | 8.78174     | 0.688698    | 12.7512     | 1.29079e-21 |
    (X1) * (X2)        | 11.2068     | 1.39616     | 8.02686     | 4.36462e-12 |
    (X3) * (X4)        | 11.8023     | 1.03538     | 11.399      | 5.99206e-19 |
    X4^3               | -2.95993    | 0.922675    | -3.20799    | 0.00187159  |
    (X2) * (X4)        | 4.98183     | 1.18419     | 4.20694     | 6.26974e-05 |
    X1^3               | 14.1106     | 0.968097    | 14.5756     | 4.49373e-25 |
    X2^3               | -12.3915    | 0.951199    | -13.0273    | 3.77309e-22 |
    (X1) * (X4)        | -2.78756    | 1.28899     | -2.1626     | 0.0333158   |
    ----------------------------------------------------------------------------
    
    Residual standard error: 0.90426 on 87 degrees of freedom
    F-statistic: 773.79 ,  p-value: 0
    ---------------------------------
    Multiple R-squared   | 0.990718 |
    Adjusted R-squared   | 0.989437 |
    ---------------------------------
    
    ---------------------------------
    Normality test       | p-value  |
    ---------------------------------
    Anderson-Darling     | 0.128032 |
    Chi-Squared          | 0.317021 |
    Kolmogorov-Smirnov   | 0.606542 |
    ---------------------------------

Use case 2
^^^^^^^^^^

.. code:: r

    ### DATA
    
    data <- rbind(
    c(676 ,33 ,5.00 ,1441.67 ,35185.5 ,16.4524) ,
    c(516 ,35 ,4.75 ,1299.19 ,28170.4 ,13.9852) ,
    c(1052 ,32 ,4.20 ,1154.27 ,26455.0 ,15.3276) ,
    c(868 ,30 ,4.40 ,1045.15 ,25072.9 ,17.3128) ,
    c(1008 ,33 ,5.55 ,521.62 ,31664.2 ,22.3312) ,
    c(436 ,33 ,5.05 ,1273.02 ,25491.7 ,12.2778) ,
    c(544 ,36 ,4.25 ,1346.35 ,20877.3 ,17.8225) ,
    c(680 ,30 ,4.45 ,1253.88 ,25621.3 ,14.3516) ,
    c(640 ,38 ,4.75 ,1242.65 ,27587.3 ,13.6826) ,
    c(492 ,30 ,4.60 ,1281.95 ,26511.7 ,11.7566) ,
    c(984 ,30 ,4.10 ,553.69 ,7886.5 ,9.8820) ,
    c(1400 ,37 ,3.45 ,494.74 ,14596.0 ,16.6752) ,
    c(1276 ,33 ,3.45 ,525.97 ,9826.8 ,12.3730) ,
    c(1736 ,36 ,4.10 ,571.14 ,11978.4 ,9.4058) ,
    c(1004 ,30 ,3.50 ,408.64 ,10368.6 ,14.9302) ,
    c(396 ,30 ,3.25 ,646.65 ,17307.4 ,31.2865) ,
    c(352 ,27 ,3.35 ,514.03 ,12822.0 ,30.1652) ,
    c(328 ,29 ,3.20 ,350.73 ,8582.6 ,28.5901) ,
    c(392 ,34 ,3.35 ,496.29 ,12369.5 ,19.8795) ,
    c(236 ,36 ,3.30 ,580.92 ,14731.9 ,18.5056) ,
    c(392 ,30 ,3.25 ,535.82 ,15060.6 ,22.1344) ,
    c(268 ,28 ,3.25 ,490.34 ,11056.3 ,28.6101) ,
    c(252 ,31 ,3.20 ,552.39 ,8118.9 ,23.1908) ,
    c(236 ,31 ,3.20 ,661.32 ,13009.5 ,24.6917) ,
    c(340 ,35 ,3.35 ,672.15 ,15003.7 ,22.6758) ,
    c(2436 ,29 ,7.10 ,528.65 ,10225.0 ,0.3729) ,
    c(2216 ,35 ,7.35 ,563.13 ,8024.2 ,0.2703) ,
    c(2096 ,35 ,7.45 ,497.96 ,10393.0 ,0.3205) ,
    c(1660 ,30 ,7.45 ,458.38 ,8711.6 ,0.2648) ,
    c(2272 ,30 ,7.40 ,498.25 ,10239.6 ,0.2105) ,
    c(824 ,26 ,4.85 ,936.26 ,20436.0 ,18.9875) ,
    c(1196 ,29 ,4.60 ,894.79 ,12519.9 ,20.9687) ,
    c(1960 ,25 ,5.20 ,941.36 ,18979.0 ,23.9841) ,
    c(2080 ,26 ,4.75 ,1038.79 ,22986.1 ,19.9727) ,
    c(1764 ,26 ,5.20 ,898.05 ,11704.5 ,21.3864) ,
    c(412 ,25 ,4.55 ,989.87 ,17721.0 ,23.7063) ,
    c(416 ,26 ,3.95 ,951.28 ,16485.2 ,30.5589) ,
    c(504 ,26 ,3.70 ,939.83 ,17101.3 ,26.8415) ,
    c(492 ,27 ,3.75 ,925.42 ,17849.0 ,27.7292) ,
    c(636 ,27 ,4.15 ,954.11 ,16949.6 ,21.5699) ,
    c(1756 ,24 ,5.60 ,720.72 ,11344.6 ,19.6531) ,
    c(1232 ,27 ,5.35 ,782.09 ,14752.4 ,20.3295) ,
    c(1400 ,26 ,5.50 ,773.30 ,13649.8 ,19.5880) ,
    c(1620 ,28 ,5.50 ,829.26 ,14533.0 ,20.1328) ,
    c(1560 ,28 ,5.40 ,856.96 ,16892.2 ,19.2420)
    )
    
    DATA = data.frame(data)
    names(DATA) = c("BIO","SAL","pH","K","Na","Zn")
    
    ### SAVE DATA 
    write.csv(DATA, file="DATA_test2.csv",row.names=FALSE)
    
    
    ### MIN/MAX MODELS
    model_min <- lm( BIO~ 1 , data=DATA)
    model_max <- lm( BIO~SAL+pH+K+Na+Zn , data=DATA)
    
    
    ### STEPWISE PROCEDURE
    ## Forward
    model_0 <- lm( BIO~ 1 , data=DATA)
    #AIC
    lm_forward_AIC <- step( model_min , scope=list(lower=model_min , upper=model_max) , direction="forward" , k=2)
    #BIC
    lm_forward_BIC <- step( model_min , scope=list(lower=model_min , upper=model_max) , direction="forward" , k=log(45))
    
    ## Backward
    model_0 <- lm( BIO~SAL+pH+K+Na+Zn , data=DATA)
    #AIC
    lm_backward_AIC <- step( model_max , scope=list( lower=model_min , upper=model_max) , direction="backward" , k=2)
    #BIC
    lm_backward_BIC <- step( model_max , scope=list( upper=model_max , lower=model_min) , direction="backward" , k=log(45))
    ## Both
    model_0 <- lm( BIO~ 1 , data=DATA)
    #AIC
    lm_both_AIC <- step( model_0 , scope=list( lower=model_min , upper=model_max) , direction="both" , k=2)
    #BIC
    lm_both_BIC <- step( model_0 , scope=list( upper=model_max , lower=model_min) , direction="both" , k=log (45))
    
    
    ### ANOVA
    
    summary(lm_forward_AIC)
    AIC(lm_forward_AIC)
    
    summary(lm_forward_BIC)
    BIC(lm_forward_BIC)
    
    summary(lm_backward_AIC)
    AIC(lm_backward_AIC)
    
    summary(lm_backward_BIC)
    BIC(lm_backward_BIC)
    
    summary(lm_both_AIC)
    AIC(lm_both_AIC)
    
    summary(lm_both_BIC)
    BIC(lm_both_BIC)

Output:

.. code::

    ...
    > ### ANOVA
    > 
    > summary(lm_forward_AIC)
    
    Call:
    lm(formula = BIO ~ pH + Na, data = DATA)
    
    Residuals:
        Min      1Q  Median      3Q     Max 
    -677.93 -229.76  -97.47  207.51 1168.40 
    
    Coefficients:
                  Estimate Std. Error t value Pr(>|t|)    
    (Intercept) -4.757e+02  2.735e+02  -1.739   0.0893 .  
    pH           4.049e+02  4.777e+01   8.477 1.22e-10 ***
    Na          -2.333e-02  8.655e-03  -2.695   0.0101 *  
    ---
    Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1
    
    Residual standard error: 394.9 on 42 degrees of freedom
    Multiple R-squared:  0.6584,	Adjusted R-squared:  0.6422 
    F-statistic: 40.48 on 2 and 42 DF,  p-value: 1.596e-10
    
    > AIC(lm_forward_AIC)
    [1] 670.666
    > 
    > summary(lm_forward_BIC)
    
    Call:
    lm(formula = BIO ~ pH + Na, data = DATA)
    
    Residuals:
        Min      1Q  Median      3Q     Max 
    -677.93 -229.76  -97.47  207.51 1168.40 
    
    Coefficients:
                  Estimate Std. Error t value Pr(>|t|)    
    (Intercept) -4.757e+02  2.735e+02  -1.739   0.0893 .  
    pH           4.049e+02  4.777e+01   8.477 1.22e-10 ***
    Na          -2.333e-02  8.655e-03  -2.695   0.0101 *  
    ---
    Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1
    
    Residual standard error: 394.9 on 42 degrees of freedom
    Multiple R-squared:  0.6584,	Adjusted R-squared:  0.6422 
    F-statistic: 40.48 on 2 and 42 DF,  p-value: 1.596e-10
    
    > BIC(lm_forward_BIC)
    [1] 677.8926
    > 
    > summary(lm_backward_AIC)
    
    Call:
    lm(formula = BIO ~ SAL + pH + K + Zn, data = DATA)
    
    Residuals:
       Min     1Q Median     3Q    Max 
    -749.1 -229.2  -94.2  127.2 1037.4 
    
    Coefficients:
                 Estimate Std. Error t value Pr(>|t|)   
    (Intercept) 1505.4882  1133.6940   1.328  0.19172   
    SAL          -35.9433    21.4761  -1.674  0.10201   
    pH           293.8611    84.4738   3.479  0.00123 **
    K             -0.4388     0.2024  -2.168  0.03615 * 
    Zn           -23.4519    14.0399  -1.670  0.10265   
    ---
    Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1
    
    Residual standard error: 394.8 on 40 degrees of freedom
    Multiple R-squared:  0.6749,	Adjusted R-squared:  0.6423 
    F-statistic: 20.76 on 4 and 40 DF,  p-value: 2.528e-09
    
    > AIC(lm_backward_AIC)
    [1] 672.4482
    > 
    > summary(lm_backward_BIC)
    
    Call:
    lm(formula = BIO ~ pH + K, data = DATA)
    
    Residuals:
       Min     1Q Median     3Q    Max 
    -679.4 -253.4  -95.4  259.4 1135.8 
    
    Coefficients:
                 Estimate Std. Error t value Pr(>|t|)    
    (Intercept) -506.9774   279.7714  -1.812   0.0771 .  
    pH           412.0395    48.4975   8.496 1.15e-10 ***
    K             -0.4871     0.2032  -2.397   0.0211 *  
    ---
    Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1
    
    Residual standard error: 401.1 on 42 degrees of freedom
    Multiple R-squared:  0.6476,	Adjusted R-squared:  0.6308 
    F-statistic: 38.59 on 2 and 42 DF,  p-value: 3.079e-10
    
    > BIC(lm_backward_BIC)
    [1] 679.3007
    > 
    > summary(lm_both_AIC)
    
    Call:
    lm(formula = BIO ~ pH + Na, data = DATA)
    
    Residuals:
        Min      1Q  Median      3Q     Max 
    -677.93 -229.76  -97.47  207.51 1168.40 
    
    Coefficients:
                  Estimate Std. Error t value Pr(>|t|)    
    (Intercept) -4.757e+02  2.735e+02  -1.739   0.0893 .  
    pH           4.049e+02  4.777e+01   8.477 1.22e-10 ***
    Na          -2.333e-02  8.655e-03  -2.695   0.0101 *  
    ---
    Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1
    
    Residual standard error: 394.9 on 42 degrees of freedom
    Multiple R-squared:  0.6584,	Adjusted R-squared:  0.6422 
    F-statistic: 40.48 on 2 and 42 DF,  p-value: 1.596e-10
    
    > AIC(lm_both_AIC)
    [1] 670.666
    > 
    > summary(lm_both_BIC)
    
    Call:
    lm(formula = BIO ~ pH + Na, data = DATA)
    
    Residuals:
        Min      1Q  Median      3Q     Max 
    -677.93 -229.76  -97.47  207.51 1168.40 
    
    Coefficients:
                  Estimate Std. Error t value Pr(>|t|)    
    (Intercept) -4.757e+02  2.735e+02  -1.739   0.0893 .  
    pH           4.049e+02  4.777e+01   8.477 1.22e-10 ***
    Na          -2.333e-02  8.655e-03  -2.695   0.0101 *  
    ---
    Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1
    
    Residual standard error: 394.9 on 42 degrees of freedom
    Multiple R-squared:  0.6584,	Adjusted R-squared:  0.6422 
    F-statistic: 40.48 on 2 and 42 DF,  p-value: 1.596e-10
    
    > BIC(lm_both_BIC)
    [1] 677.8926
    > 

With OpenTURNS:

.. code:: python

    import openturns as ot
    import otlm
    from math import log
    
    sample = ot.Sample.ImportFromTextFile("../../examples/DATA_test2.csv", ",")
    
    X = sample[:, 1:6]
    Y = sample[:, 0]
    
    ################################################################################################
    # Build a model BIO~SAL+pH+K+Na+Zn
    dim = X.getDimension()
    enumerateFunction = ot.EnumerateFunction(dim)
    factory = ot.TensorizedUniVariateFunctionFactory([ot.MonomialFunctionFactory()]*dim, enumerateFunction)
    
    # Build 'interactions' as a list of list [a1,a2,a3,a4,a5], and we will generate tensorized
    # polynomials SAL^a1*pH^a2*K^a3*Na^a4*Zn^a5.
    
    # BIO~SAL+pH+K+Na+Zn
    interactions = []
    interactions.append([0]*dim)
    for i in range(dim):
      indices = [0]*dim
      indices[i] = 1
      # Y ~ I(Xi)^1
      interactions.append(indices[:])
    
    funcs = [factory.build(enumerateFunction.inverse(indices)) for indices in interactions]
    [f.getEvaluation().setInputDescription(X.getDescription()) for f in funcs]
    basis = ot.Basis(funcs)
    ################################################################################################
    
    i_min = [interactions.index([0,0,0,0,0])]
    i_0 = i_min[:]
    
    #---------------- Forward / Backward------------------- 
    #   X: input sample
    #   basis : Basis
    #   Y: output sample
    #   i_min:  indices of minimal model
    #   direction: Boolean (True FORWARD, False BACKWARD)
    #   penalty: multiplier of number of degrees of freedom
    #   maxiteration: maximum number of iterations
    
    #---------------- Both------------------- 
    #   X: input sample
    #   basis : Basis
    #   Y: output sample
    #   i_min : indices of minimal model
    #   i_0   : indices of start model
    #   penalty: multiplier of number of degrees of freedom
    #   maxiteration: maximum number of iterations
    
    penalty_BIC = log(X.getSize())
    penalty_AIC = 2.
    maxiteration = 1000
    
    for k in [penalty_AIC, penalty_BIC]:
      ## Forward / Backward
      if k==penalty_AIC:  IC =" AIC "
      if k==penalty_BIC:  IC =" BIC "  
      for forward in [True, False]:
        algo = otlm.LinearModelStepwiseAlgorithm(X, basis, Y, i_min, forward, k, maxiteration)
        algo_result = otlm.LinearModelAnalysis(algo.getResult())
        print("{0:~^60s}".format(""))
        if forward==True : print(" Forward " +IC)
        else             : print(" Backward "+IC)
        print("{0:~^60s}".format(""))
        print(algo_result)
      ## Both
      algo = otlm.LinearModelStepwiseAlgorithm(X, basis, Y, i_min, i_0, k, maxiteration)
      algo_result = otlm.LinearModelAnalysis(algo.getResult())
      print("{0:~^60s}".format(""))
      print(" Both "+IC)
      print("{0:~^60s}".format(""))
      print(algo_result)
    
Output:

.. code::

    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     Forward  AIC 
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Basis( [1,SAL,pH,K,Na,Zn] )
    
    Coefficients:
       | Estimate    | Std Error   | t value     | Pr(>|t|)    | 
    ------------------------------------------------------------
    1  | -475.729    | 273.522     | -1.73927    | 0.0893122   | 
    pH | 404.948     | 47.7698     | 8.47707     | 1.22026e-10 | 
    Na | -0.0233259  | 0.00865512  | -2.69504    | 0.0100775   | 
    ------------------------------------------------------------
    
    Residual standard error: 394.85 on 42 degrees of freedom
    F-statistic: 40.481 ,  p-value: 1.0636e-10
    ---------------------------------
    Multiple R-squared   | 0.658433 | 
    Adjusted R-squared   | 0.642168 | 
    ---------------------------------
    
    ------------------------------------
    Normality test       | p-value     | 
    ------------------------------------
    Anderson-Darling     | 0.000429122 | 
    Chi-Squared          | 0.000909433 | 
    Kolmogorov-Smirnov   | 0.0481525   | 
    ------------------------------------
    
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     Backward  AIC 
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Basis( [1,SAL,pH,K,Na,Zn] )
    
    Coefficients:
        | Estimate   | Std Error  | t value    | Pr(>|t|)   | 
    ---------------------------------------------------------
    1   | 1505.49    | 1133.69    | 1.32795    | 0.191723   | 
    SAL | -35.9433   | 21.4761    | -1.67364   | 0.102005   | 
    pH  | 293.861    | 84.4738    | 3.47873    | 0.00123036 | 
    K   | -0.438818  | 0.202384   | -2.16824   | 0.0361463  | 
    Zn  | -23.4519   | 14.0399    | -1.67038   | 0.102653   | 
    ---------------------------------------------------------
    
    Residual standard error: 394.76 on 40 degrees of freedom
    F-statistic: 20.756 ,  p-value: 1.1414e-09
    ---------------------------------
    Multiple R-squared   | 0.674859 | 
    Adjusted R-squared   | 0.642344 | 
    ---------------------------------
    
    -----------------------------------
    Normality test       | p-value    | 
    -----------------------------------
    Anderson-Darling     | 0.00124009 | 
    Chi-Squared          | 0.0204981  | 
    Kolmogorov-Smirnov   | 0.338063   | 
    -----------------------------------
    
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     Both  AIC 
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Basis( [1,SAL,pH,K,Na,Zn] )
    
    Coefficients:
       | Estimate    | Std Error   | t value     | Pr(>|t|)    | 
    ------------------------------------------------------------
    1  | -475.729    | 273.522     | -1.73927    | 0.0893122   | 
    pH | 404.948     | 47.7698     | 8.47707     | 1.22026e-10 | 
    Na | -0.0233259  | 0.00865512  | -2.69504    | 0.0100775   | 
    ------------------------------------------------------------
    
    Residual standard error: 394.85 on 42 degrees of freedom
    F-statistic: 40.481 ,  p-value: 1.0636e-10
    ---------------------------------
    Multiple R-squared   | 0.658433 | 
    Adjusted R-squared   | 0.642168 | 
    ---------------------------------
    
    ------------------------------------
    Normality test       | p-value     | 
    ------------------------------------
    Anderson-Darling     | 0.000429122 | 
    Chi-Squared          | 0.000909433 | 
    Kolmogorov-Smirnov   | 0.0481525   | 
    ------------------------------------
    
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     Forward  BIC 
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Basis( [1,SAL,pH,K,Na,Zn] )
    
    Coefficients:
       | Estimate    | Std Error   | t value     | Pr(>|t|)    | 
    ------------------------------------------------------------
    1  | -475.729    | 273.522     | -1.73927    | 0.0893122   | 
    pH | 404.948     | 47.7698     | 8.47707     | 1.22026e-10 | 
    Na | -0.0233259  | 0.00865512  | -2.69504    | 0.0100775   | 
    ------------------------------------------------------------
    
    Residual standard error: 394.85 on 42 degrees of freedom
    F-statistic: 40.481 ,  p-value: 1.0636e-10
    ---------------------------------
    Multiple R-squared   | 0.658433 | 
    Adjusted R-squared   | 0.642168 | 
    ---------------------------------
    
    ------------------------------------
    Normality test       | p-value     | 
    ------------------------------------
    Anderson-Darling     | 0.000429122 | 
    Chi-Squared          | 0.000909433 | 
    Kolmogorov-Smirnov   | 0.0481525   | 
    ------------------------------------
    
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     Backward  BIC 
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Basis( [1,SAL,pH,K,Na,Zn] )
    
    Coefficients:
       | Estimate    | Std Error   | t value     | Pr(>|t|)    | 
    ------------------------------------------------------------
    1  | -506.977    | 279.771     | -1.81211    | 0.0771222   | 
    pH | 412.04      | 48.4975     | 8.49609     | 1.14894e-10 | 
    K  | -0.487098   | 0.203211    | -2.397      | 0.0210566   | 
    ------------------------------------------------------------
    
    Residual standard error: 401.08 on 42 degrees of freedom
    F-statistic: 38.587 ,  p-value: 2.0936e-10
    ---------------------------------
    Multiple R-squared   | 0.647576 | 
    Adjusted R-squared   | 0.630794 | 
    ---------------------------------
    
    ------------------------------------
    Normality test       | p-value     | 
    ------------------------------------
    Anderson-Darling     | 0.00130208  | 
    Chi-Squared          | 0.000909433 | 
    Kolmogorov-Smirnov   | 0.121207    | 
    ------------------------------------
    
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     Both  BIC 
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Basis( [1,SAL,pH,K,Na,Zn] )
    
    Coefficients:
       | Estimate    | Std Error   | t value     | Pr(>|t|)    | 
    ------------------------------------------------------------
    1  | -475.729    | 273.522     | -1.73927    | 0.0893122   | 
    pH | 404.948     | 47.7698     | 8.47707     | 1.22026e-10 | 
    Na | -0.0233259  | 0.00865512  | -2.69504    | 0.0100775   | 
    ------------------------------------------------------------
    
    Residual standard error: 394.85 on 42 degrees of freedom
    F-statistic: 40.481 ,  p-value: 1.0636e-10
    ---------------------------------
    Multiple R-squared   | 0.658433 | 
    Adjusted R-squared   | 0.642168 | 
    ---------------------------------
    
    ------------------------------------
    Normality test       | p-value     | 
    ------------------------------------
    Anderson-Darling     | 0.000429122 | 
    Chi-Squared          | 0.000909433 | 
    Kolmogorov-Smirnov   | 0.0481525   | 
    ------------------------------------
    
