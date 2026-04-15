import pytest

from src.transformations import process


def test_process_example_case():
    v = (1.0, 2.0)
    s = "ESCALA 1.5 ROT_DEG 30.5 ROT_DEG -10 ESCALA -0.5;"
    out = process(v, s)
    expected = [
        "v = (1.000, 2.000)",
        "Escala a = 1.500, v = (1.500, 3.000)",
        "Rotacion alfa = 0.532 rad, v = (-0.230, 3.346)",
        "Rotacion alfa = -0.175 rad, v = (0.354, 3.335)",
        "Escala a = -0.500, v = (-0.177, -1.668)",
    ]
    assert out == expected


def test_process_case1():
    v = (4.0, -3.0)
    s = "ESCALA 2.5 ESCALA -2 ;"
    out = process(v, s)
    assert out[0] == "v = (4.000, -3.000)"
    assert out[1] == "Escala a = 2.500, v = (10.000, -7.500)"
    assert out[2] == "Escala a = -2.000, v = (-20.000, 15.000)"


def test_process_invalid_transforms_raises():
    v = (1.0, 1.0)
    with pytest.raises(ValueError):
        process(v, "ESCALA 1 ROT_DEG 30")
