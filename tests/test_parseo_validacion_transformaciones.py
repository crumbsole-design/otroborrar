import pytest

from src.transformations import parse_transformations


def test_parse_transformations_valid():
    s = "ESCALA 1.5 ROT_DEG 30.5 ROT_DEG -10 ESCALA -0.5;"
    expected = [("ESCALA", 1.5), ("ROT_DEG", 30.5), ("ROT_DEG", -10.0), ("ESCALA", -0.5)]
    assert parse_transformations(s) == expected


def test_parse_transformations_whitespace_variants():
    s = "  ESCALA   2   ROT_DEG   90  ; "
    assert parse_transformations(s) == [("ESCALA", 2.0), ("ROT_DEG", 90.0)]


def test_parse_transformations_invalid_missing_semicolon():
    with pytest.raises(ValueError):
        parse_transformations("ESCALA 1 ROT_DEG 30")


def test_parse_transformations_invalid_token():
    with pytest.raises(ValueError):
        parse_transformations("ESCAL 1 ;")


def test_parse_transformations_invalid_value():
    with pytest.raises(ValueError):
        parse_transformations("ESCALA x ;")
