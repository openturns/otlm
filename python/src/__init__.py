"""
    otlm --- An OpenTURNS module
    ==================================

    Contents
    --------
      'otlm' is a module for OpenTURNS

"""

import sys
if sys.platform.startswith('win'):
    # this ensures OT dll is loaded
    import openturns

from .otlm import *

__version__ = '0.1'

