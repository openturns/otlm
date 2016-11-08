
Graphs
======

.. code:: python

    from openturns.viewer import View
    import openturns as ot
    import otlmr
    import pandas as pd
    
    # Data is available from R datasets, and can be downloaded from
    # https://vincentarelbundock.github.io/Rdatasets/csv/datasets/LifeCycleSavings.csv
    #
    # First column in this CSV file is country name, use
    # pandas to easily filter it out.
    data = pd.read_csv("LifeCycleSavings.csv", index_col=0)
    
    sample = ot.NumericalSample(data.values)
    sample.setName("LifeCycleSavings")
    sample.setDescription(["sr","pop15","pop75","dpi","ddpi"])
    
    sr    = sample[:,0]
    pop15 = sample[:,1]
    pop75 = sample[:,2]
    dpi   = sample[:,3]
    ddpi  = sample[:,4]
    
    # model1
    outputSample = sample[:,0]
    inputSample = sample[:,1:5]
    
    algo1 = otlmr.LinearModelAlgorithm(inputSample, outputSample)
    result1 = algo1.getResult()
    analysis1 = otlmr.LinearModelAnalysis(algo1.getResult())
    
    for plot in ["drawResidualsVsFitted", "drawScaleLocation", "drawQQplot",
                 "drawCookDistance", "drawResidualsVsLeverages", "drawCookVsLeverages"]:
        graph = getattr(analysis1, plot)()
        graph.draw("graphs_files/model1-"+plot, 640, 480)
    
.. image:: graphs_files/model1-drawResidualsVsFitted.*

.. image:: graphs_files/model1-drawScaleLocation.*

.. image:: graphs_files/model1-drawQQplot.*

.. image:: graphs_files/model1-drawCookDistance.*

.. image:: graphs_files/model1-drawResidualsVsLeverages.*

.. image:: graphs_files/model1-drawCookVsLeverages.*

.. code:: python

    from openturns.viewer import View
    import openturns as ot
    import otlmr
    import pandas as pd
    
    # Data is available from R datasets, and can be downloaded from
    # https://vincentarelbundock.github.io/Rdatasets/csv/datasets/LifeCycleSavings.csv
    #
    # First column in this CSV file is country name, use
    # pandas to easily filter it out.
    data = pd.read_csv("LifeCycleSavings.csv", index_col=0)
    
    sample = ot.NumericalSample(data.values)
    sample.setName("LifeCycleSavings")
    sample.setDescription(["sr","pop15","pop75","dpi","ddpi"])
    
    sr    = sample[:,0]
    pop15 = sample[:,1]
    pop75 = sample[:,2]
    dpi   = sample[:,3]
    ddpi  = sample[:,4]
    
    # model2
    f = ot.NumericalMathFunction('x','x^4','y')
    outputSample = f(sr)
    inputSample = pop75
    inputSample.stack(dpi)
    
    algo2 = otlmr.LinearModelAlgorithm(inputSample, outputSample)
    result2 = algo2.getResult()
    analysis2 = otlmr.LinearModelAnalysis(algo2.getResult())
    
    for plot in ["drawResidualsVsFitted", "drawScaleLocation", "drawQQplot",
                 "drawCookDistance", "drawResidualsVsLeverages", "drawCookVsLeverages"]:
        graph = getattr(analysis2, plot)()
        graph.draw("graphs_files/model2-"+plot, 640, 480)
    
.. image:: graphs_files/model2-drawResidualsVsFitted.*

.. image:: graphs_files/model2-drawScaleLocation.*

.. image:: graphs_files/model2-drawQQplot.*

.. image:: graphs_files/model2-drawCookDistance.*

.. image:: graphs_files/model2-drawResidualsVsLeverages.*

.. image:: graphs_files/model2-drawCookVsLeverages.*

