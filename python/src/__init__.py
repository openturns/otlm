"""
    otlmr --- An OpenTURNS module
    ==================================

    Contents
    --------
      'otlmr' is a module for OpenTURNS

"""

import sys
if sys.platform.startswith('win'):
    # this ensures OT dll is loaded
    import openturns

from .otlmr import *

__version__ = '0.0'

