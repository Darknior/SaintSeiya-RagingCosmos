#!/usr/bin/env python3
"""Compatibility entrypoint: validate the retained native menus in all locales."""
import runpy
from pathlib import Path
runpy.run_path(str(Path(__file__).with_name('test_native_localized_menus.py')),run_name='__main__')
