import math
from typing import List, Tuple

import pytest

from src.transformations import apply_transformations


def _approx_vec(v: Tuple[float, float], expected: Tuple[float, float], tol: float = 1e-3):
    assert pytest.approx(expected[0], abs=tol) == v[0]
    assert pytest.approx(expected[1], abs=tol) == v[1]


def test_apply_transformations_example_sequence():
    v = (1.0, 2.0)
    transforms: List[Tuple[str, float]] = [("ESCALA", 1.5), ("ROT_DEG", 30.5), ("ROT_DEG", -10.0), ("ESCALA", -0.5)]
    results = apply_transformations(v, transforms)
    assert len(results) == 4
    _approx_vec(results[0], (1.5, 3.0))
    _approx_vec(results[1], (-0.229680, 3.345996))
    _approx_vec(results[2], (0.353680, 3.334991))
    _approx_vec(results[3], (-0.176840, -1.667495))


def test_apply_transformations_rotate_minus_90():
    v = (4.0, 2.5)
    transforms = [("ROT_DEG", -90.0)]
    results = apply_transformations(v, transforms)
    assert len(results) == 1
    # rotation -90deg => (2.5, -4)
    _approx_vec(results[0], (2.5, -4.0))
